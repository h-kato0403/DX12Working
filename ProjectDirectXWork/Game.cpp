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

	gameDebug.Log("Game Initialize Clear");
    return true;
}

bool Game::Update()
{
    return false;
}

bool Game::Render()
{
    return false;
}

bool Game::CreateVertexBuffer()
{
	Vertex vx[3] = {};
	auto vertexSize = sizeof(Vertex) * std::size(vx);
	auto vertexStride = sizeof(Vertex);
	m_pVertexBuffer = new VertexBuffer(vertexSize, vertexStride, vx);
	if (!m_pVertexBuffer->IsValid())
	{
		gameDebug.Log("Failed CreateVertexBuffer - new VertexBuffer");
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
