#pragma once
#include "SharedVertex.h"
#include <vector>

class Porigon
{
public:
	Vertex* GetPoligonVertex()
	{
		SetVertices();
		return &vertices[0];
	}
	size_t GetVertexCount() const
	{
		return std::size(vertices);
	}

protected:
	virtual void SetVertices()
	{
		Vertex vx;
		vx.POSITION = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vx.COLOR = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices.push_back(vx);

		vx.POSITION = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
		vx.COLOR = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices.push_back(vx);

		vx.POSITION = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		vx.COLOR = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices.push_back(vx);
	}

	std::vector<Vertex> vertices;
};

class QuadPorigon : public Porigon
{
protected:
	void  SetVertices()
	{
		Vertex vx;
		vx.POSITION = DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f);
		vx.COLOR = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
		vertices.push_back(vx);

		vx.POSITION = DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f);
		vx.COLOR = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		vertices.push_back(vx);

		vx.POSITION = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
		vx.COLOR = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		vertices.push_back(vx);

		vx.POSITION = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
		vx.COLOR = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		vertices.push_back(vx);
	}
};
