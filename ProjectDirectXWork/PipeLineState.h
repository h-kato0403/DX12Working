#pragma once
#include "ComPtr.h"
#include <d3dx12.h>
#include <string>

#pragma comment(lib, "d3dcompiler.lib")

class PipeLineState
{
public:
	PipeLineState();

	void Create();
	void SetVertexShader(std::wstring path);
	void SetPixelShader(std::wstring path);

	bool IsValid()
	{
		return m_isValid;
	}
	void SetInputLayout(D3D12_INPUT_LAYOUT_DESC layout)
	{
		desc.InputLayout = layout;
	}
	void SetRootSignature(ID3D12RootSignature* rootSignature)
	{
		desc.pRootSignature = rootSignature;
	}
	ID3D12PipelineState* Get()
	{
		return m_pPipelineState.Get();
	}

private:
	bool m_isValid = false;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
	ComPtr<ID3D12PipelineState> m_pPipelineState = nullptr;
	ComPtr<ID3DBlob> m_pVSBlob;
	ComPtr<ID3DBlob> m_pPSBlob;
};

