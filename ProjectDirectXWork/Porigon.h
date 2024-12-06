#pragma once
#include "SharedVertex.h"

class Porigon
{
public:
	Porigon()
	{

	}


	Vertex* GetTrianglePoligonVertex()
	{
		return triangleVertices;
	}

private:
	void SetTriangleVertex()
	{
		triangleVertices[0].POSITION = DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f);
		triangleVertices[0].COLOR = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);

		triangleVertices[1].POSITION = DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f);
		triangleVertices[1].COLOR = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

		triangleVertices[2].POSITION = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
		triangleVertices[2].COLOR = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	Vertex triangleVertices[3] = {};
};

