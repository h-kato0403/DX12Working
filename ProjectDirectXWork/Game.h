#pragma once
#include "VertexBuffer.h"
#include "ConstantBuffer.h"

class Game
{
public:
	static Game& Get()
	{
		static Game instance;
		return instance;
	}

	bool Initialize();
	bool Update();
	bool Render();

private:
	VertexBuffer* m_vertexBuffer = nullptr;
	ConstantBuffer* m_constantBuffer = nullptr;
};

