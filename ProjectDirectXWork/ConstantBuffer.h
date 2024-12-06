#pragma once
#include "ComPtr.h"
#include <d3dx12.h>

class ConstantBuffer
{
public:
	ConstantBuffer(size_t size);

	bool IsValid()
	{
		return m_isValid;
	}
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const
	{
		return m_desc.BufferLocation;
	}
	D3D12_CONSTANT_BUFFER_VIEW_DESC ViewDesc()
	{
		return m_desc;
	}
	void* GetPtr() const
	{
		return m_pMappedPtr;
	}

	template<typename T>
	T* GetPtr()
	{
		return reinterpret_cast<T*>(GetPtr());
	}

private:
	bool m_isValid = false;
	ComPtr<ID3D12Resource> m_pBuffer;
	D3D12_CONSTANT_BUFFER_VIEW_DESC m_desc;
	void* m_pMappedPtr = nullptr;

	ConstantBuffer(const ConstantBuffer&) = delete;
	void operator = (const ConstantBuffer&) = delete;
};

