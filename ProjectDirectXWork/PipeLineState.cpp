#include "PipeLineState.h"
#include "Engine.h"
#include <d3dcompiler.h>

PipeLineState::PipeLineState()
{
	desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	desc.SampleMask = UINT_MAX;
	desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	desc.NumRenderTargets = 1;
	desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
}

void PipeLineState::Create()
{
	HRESULT hr = Engine::Get().Device()->CreateGraphicsPipelineState(
		&desc, 
		IID_PPV_ARGS(m_pPipelineState.ReleaseAndGetAddressOf())
	);
	if (FAILED(hr))
	{
		printf("PipeLienState Create - CreateGraphicsPipelineState");
		return;
	}

	m_isValid = true;
}

void PipeLineState::SetVertexShader(std::wstring path)
{
	HRESULT hr = D3DReadFileToBlob(path.c_str(), m_pVSBlob.GetAddressOf());
	if (FAILED(hr))
	{
		printf("SetVertexShader - D3DReadFileToBlob");
		return;
	}

	desc.VS = CD3DX12_SHADER_BYTECODE(m_pVSBlob.Get());
}

void PipeLineState::SetPixelShader(std::wstring path)
{
	HRESULT hr = D3DReadFileToBlob(path.c_str(), m_pPSBlob.GetAddressOf());
	if (FAILED(hr))
	{
		printf("SetPixelShader - D3DReadFileToBlob");
		return;
	}

	desc.PS = CD3DX12_SHADER_BYTECODE(m_pPSBlob.Get());
}

