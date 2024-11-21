#include "Engine.h"
#include <d3d12.h>
#include <stdio.h>
#include <Windows.h>
#include "ConsoleLog.h"

Console debug = Console::Get();

bool Engine::Init(HWND hwnd, UINT windowWidth, UINT windowHeight)
{
    m_hwnd = hwnd;
    m_frameBufferWidth = windowWidth;
    m_frameBufferHeight = windowHeight;

    if (!CreateDevice())
    {
        debug.Log("Failed CreateDevice");
        return false;
    }

    debug.Log("Render Engine Initialize");
    return true;
}

void Engine::RenderBegin()
{
}

void Engine::RenderEnd()
{
}

bool Engine::CreateDevice()
{
    HRESULT hr = D3D12CreateDevice(
        nullptr,
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(m_pDevice.ReleaseAndGetAddressOf())
    );
    return SUCCEEDED(hr);
}
