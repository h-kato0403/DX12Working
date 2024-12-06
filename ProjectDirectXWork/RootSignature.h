#pragma once
#include "ComPtr.h"

struct ID3D12RootSignature;

class RootSignature
{
public:
	RootSignature();
	bool IsValid()
	{
		return m_isValid;
	}
	ID3D12RootSignature* GetRootSignature()
	{
		return m_pRootSignature.Get();
	}

private:
	bool m_isValid = false;
	ComPtr<ID3D12RootSignature> m_pRootSignature = nullptr;
};

