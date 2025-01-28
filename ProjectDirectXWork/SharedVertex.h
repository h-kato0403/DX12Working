#pragma once
#include <d3dx12.h>
#include <directxmath.h>
#include "ComPtr.h"

struct Vertex
{
	DirectX::XMFLOAT3 POSITION;
	DirectX::XMFLOAT3 NORMAL;
	DirectX::XMFLOAT2 UV_POSITION;
	DirectX::XMFLOAT3 TANGENT;
	DirectX::XMFLOAT4 COLOR;

	static const D3D12_INPUT_LAYOUT_DESC InputLayout;

	Vertex& operator=(const Vertex& vx)
	{
		if (this == &vx) // 自己代入チェック
			return *this;

		POSITION = vx.POSITION;
		NORMAL = vx.NORMAL;
		COLOR = vx.COLOR;
		UV_POSITION = vx.UV_POSITION;
		TANGENT = vx.TANGENT;

		return *this;
	}

private:
	static const int InputElementCount = 5;
	static const D3D12_INPUT_ELEMENT_DESC InputElements[InputElementCount];
};

struct Mesh
{
	std::vector<Vertex> Vertices;
	std::vector<uint32_t> Indices;
	std::wstring DiffuseMap;
};

struct alignas(256) Transform
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
};

