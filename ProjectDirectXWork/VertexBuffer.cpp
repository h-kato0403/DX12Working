#include "VertexBuffer.h"
#include "Engine.h"
#include "ConsoleLog.h"

Console debugVB = Console::Get();

VertexBuffer::VertexBuffer(size_t size, size_t stride, const void* pInitData)
{
	CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	HRESULT hr = Engine::Get().Device()->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pBuffer.GetAddressOf())
	);
	if (FAILED(hr))
	{
		debugVB.Log("FAILED VertexBuffer - CreateCommittedResource");
		return;
	}

	m_view.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
	m_view.SizeInBytes = static_cast<UINT>(size);
	m_view.StrideInBytes = static_cast<UINT>(stride);

	if (pInitData != nullptr)
	{
		void* ptr = nullptr;
		hr = m_pBuffer->Map(
			0,
			nullptr,
			&ptr
		);
		if (FAILED(hr))
		{
			debugVB.Log("FAILED VertexBuffer - Map");
			return;
		}

		memcpy(ptr, pInitData, size);

		m_pBuffer->Unmap(0, nullptr);
	}

	m_isValid = true;
}
