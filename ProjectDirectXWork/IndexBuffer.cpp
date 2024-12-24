#include "IndexBuffer.h"
#include "Engine.h"
#include "ConsoleLog.h"
#include <d3dx12.h>

IndexBuffer::IndexBuffer(size_t size, const uint32_t* pInitData)
{
	CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(size);

	// ƒŠƒ\[ƒX‚ð¶¬
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
		Console::Get().Log("IndexBuffer - CreateCommittedResource");
		return;
	}

	m_view = {};
	m_view.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
	m_view.Format = DXGI_FORMAT_R32_UINT;
	m_view.SizeInBytes = static_cast<UINT>(size);

	if (pInitData != nullptr)
	{
		void* ptr = nullptr;
		hr = m_pBuffer->Map(0, nullptr, &ptr);
		if (FAILED(hr))
		{
			Console::Get().Log("IndexBuffer - Map");
			return;
		}

		memcpy(ptr, pInitData, size);

		m_pBuffer->Unmap(0, nullptr);
	}
	m_isValid = true;
}
