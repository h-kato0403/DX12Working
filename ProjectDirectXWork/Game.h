#pragma once
#include "VertexBuffer.h"
#include "Porigon.h"
#include "ConstantBuffer.h"
#include "RootSignature.h"
#include "PipeLineState.h"
#include "IndexBuffer.h"
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
	bool CreatePipeLineState();

	VertexBuffer* m_pVertexBuffer = nullptr;
	RootSignature* m_pRootSignature = nullptr;
	PipeLineState* m_pPipelineState = nullptr;
	IndexBuffer* m_pIndexBuffer = nullptr;

	QuadPorigon m_porigon;
};

