#include "ConsoleLog.h"
#include "Application.h"
#include "SharedVertex.h"
#include "Game.h"

Console gameDebug = Console::Get();
ConstantBuffer* m_pConstantBuffer[Engine::FRAME_BUFFER_COUNT];


bool Game::Initialize()
{
	if (!CreateVertexBuffer())
	{
		gameDebug.Log("Failed GameInitialize - CreateVertexBuffer");
		return false;
	}
	if (!CreateConstantBuffer())
	{
		gameDebug.Log("Failed GameInitialize - CreateConstantBuffer");
		return false;
	}
	if (!CreateRootSignature())
	{
		gameDebug.Log("Failed GameInitialize - CreateRootSignature");
		return false;
	}
	if (!CreatePipeLineState())
	{
		gameDebug.Log("Failed GameInitialize - CreatePipeLineState");
		return false;
	}

	gameDebug.Log("Game Initialize Clear");
    return true;
}

bool Game::Update()
{
    return false;
}

bool Game::Render()
{
	auto currentIndex = Engine::Get().CurrentBackBufferIndex();
	auto commandList = Engine::Get().CommandList();
	auto vbView = m_pVertexBuffer->View();
	auto ibView = m_pIndexBuffer->View();

	commandList->SetGraphicsRootSignature(m_pRootSignature->GetRootSignature());
	commandList->SetPipelineState(m_pPipelineState->Get());
	commandList->SetGraphicsRootConstantBufferView(0, m_pConstantBuffer[currentIndex]->GetAddress());

	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	commandList->IASetVertexBuffers(0, 1, &vbView);
	commandList->IASetIndexBuffer(&ibView);

	commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

    return true;
}

bool Game::CreateVertexBuffer()
{
	Vertex vx[4];
	Vertex* vertcies = m_porigon.GetPoligonVertex();
	size_t vertexCount = m_porigon.GetVertexCount();

	for (size_t i = 0; i < vertexCount; i++)
	{
		vx[i] = vertcies[i];
	}

	auto vertexSize = sizeof(vx); // 正しいサイズ
	auto vertexStride = sizeof(Vertex);
	m_pVertexBuffer = new VertexBuffer(vertexSize, vertexStride, vx);
	if (!m_pVertexBuffer->IsValid())
	{
		gameDebug.Log("Failed CreateVertexBuffer - new VertexBuffer");
		return false;
	}

	uint32_t indices[] = { 0, 1, 2, 0, 3, 2 }; // これに書かれている順序で描画する

	// インデックスバッファの生成
	auto size = sizeof(uint32_t) * std::size(indices);
	m_pIndexBuffer = new IndexBuffer(size, indices);
	if (!m_pIndexBuffer->IsValid())
	{
		gameDebug.Log("Failed CreateIndexBuffer - new IndexBuffer");
		return false;
	}

	return true;
}

bool Game::CreateConstantBuffer()
{
	auto eyePos    = DirectX::XMVectorSet(0.0f, 0.0f, 5.0f, 0.0f);
	auto targetPos = DirectX::XMVectorZero();
	auto upward    = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	float fov      = DirectX::XMConvertToRadians(37.5);
	auto aspect    = static_cast<float>(Application::Get().WINDOW_WIDTH) / static_cast<float>(Application::Get().WINDOW_HEIGHT);

	for (size_t i = 0; i < Engine::FRAME_BUFFER_COUNT; i++)
	{
		m_pConstantBuffer[i] = new ConstantBuffer(sizeof(Transform));
		if (!m_pConstantBuffer[i]->IsValid())
		{
			gameDebug.Log("Failed CreateConstantBuffer - new ConstantBuffer");
			return false;
		}

		auto ptr = m_pConstantBuffer[i]->GetPtr<Transform>();
		ptr->World = DirectX::XMMatrixIdentity();
		ptr->View = DirectX::XMMatrixLookAtRH(eyePos, targetPos, upward);
		ptr->Proj = DirectX::XMMatrixPerspectiveFovRH(fov, aspect, 0.3f, 1000.0f);
	}

	return true;
}

bool Game::CreateRootSignature()
{
	m_pRootSignature = new RootSignature();
	if (!m_pRootSignature->IsValid())
	{
		gameDebug.Log("Failed CreateRootSignature - new RootSignature");
		return false;
	}

	return true;
}

bool Game::CreatePipeLineState()
{
	m_pPipelineState = new PipeLineState();
	m_pPipelineState->SetInputLayout(Vertex::InputLayout);
	m_pPipelineState->SetRootSignature(m_pRootSignature->GetRootSignature());
	m_pPipelineState->SetVertexShader(L"../x64/Debug/vs.cso");
	m_pPipelineState->SetPixelShader(L"../x64/Debug/ps.cso");
	m_pPipelineState->Create();
	if (!m_pPipelineState->IsValid())
	{
		gameDebug.Log("Failed CreatePipeLineState - new PipeLineState");
		return false;
	}

	return true;
}
