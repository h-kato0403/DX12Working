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
};

struct alignas(256) Transform
{
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
};