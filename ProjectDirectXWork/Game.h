#pragma once
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "Engine.h"

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
	bool CreateVertexBuffer();
	bool CreateConstantBuffer();
	bool CreateRootSignature();

	VertexBuffer* m_pVertexBuffer = nullptr;
	RootSignature* m_pRootSignature = nullptr;



};

