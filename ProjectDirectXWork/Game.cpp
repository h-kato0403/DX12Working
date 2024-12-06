#include "ConsoleLog.h"
#include "Engine.h"
#include "Application.h"
#include "SharedVertex.h"
#include "Game.h"
//#include "VertexBuffer.h"

Console gameDebug = Console::Get();
VertexBuffer* buffer;

bool Game::Initialize()
{
    gameDebug.Log("Game Initialize");

    Vertex vx[3] = {};
	auto vertexSize = sizeof(Vertex) * std::size(vx);
	auto vertexStride = sizeof(Vertex);
	m_vertexBuffer = new VertexBuffer(vertexSize, vertexStride, vx);
	if (!m_vertexBuffer->IsValid())
	{
		gameDebug.Log("頂点バッファの生成に失敗\n");
		return false;
	}



    return true;
}

bool Game::Update()
{
    return false;
}

bool Game::Render()
{
    return false;
}
