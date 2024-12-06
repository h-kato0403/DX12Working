#include "ConstantBuffer.h"
#include "Engine.h"
#include "ConsoleLog.h"

Console log = Console::Get();

ConstantBuffer::ConstantBuffer(size_t size)
{
	size_t align = D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT;
	UINT64 sizeAligned = (size + (align - 1)) & ~(align - 1);

	CD3DX12_HEAP_PROPERTIES prop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD); // ヒーププロパティ
	CD3DX12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(sizeAligned);

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
		log.Log("FAILED ConstantBuffer - CreateCommittedResource");
		return;
	}

	hr = m_pBuffer->Map(
		0,
		nullptr,
		&m_pMappedPtr
	);
	if (FAILED(hr))
	{
		log.Log("FAILED ConstantBuffer - Map");
		return;
	}

	m_desc = {};
	m_desc.BufferLocation = m_pBuffer->GetGPUVirtualAddress();
	m_desc.SizeInBytes = UINT(sizeAligned);

	m_isValid = true;
}
