#pragma once
#include <cstdint>
#include <d3d12.h>
#include "ComPtr.h"

class IndexBuffer
{
public:
	IndexBuffer(size_t size, const uint32_t* pInitData = nullptr);
	bool IsValid()
	{
		return m_isValid;
	}
	D3D12_INDEX_BUFFER_VIEW View() const
	{
		return m_view;
	}

private:
	bool m_isValid = false;
	ComPtr<ID3D12Resource> m_pBuffer;
	D3D12_INDEX_BUFFER_VIEW m_view;

	IndexBuffer(const IndexBuffer&) = delete;
	void operator = (const IndexBuffer&) = delete;
};

