#pragma once
#include <d3d12.h>
#include "ComPtr.h"

class VertexBuffer
{
public:
	VertexBuffer(
		size_t size,
		size_t stride,
		const void* pInitData
	);
	D3D12_VERTEX_BUFFER_VIEW View() const
	{
		return m_view;
	}
	bool IsValid() 
	{
		return m_isValid;
	}

private:
	bool m_isValid = false;
	ComPtr<ID3D12Resource> m_pBuffer = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_view = {};
	VertexBuffer(const VertexBuffer&) = delete;
	void operator = (const VertexBuffer&) = delete;
};

