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

    if (!CreateCommandQueue())
    {
        debug.Log("Failed CreateCommandQueue");
        return false;
    }

    if (!CreateSwapChain())
    {
        debug.Log("Failed CreateSwapChain");
        return false;
    }

    if (!CreateCommandList())
    {
        debug.Log("Failed CreateCommandList");
        return false;
    }

    if (!CreateFence())
    {
        debug.Log("Failed CreateFence");
        return false;
    }

    CreateViewPort();
    CreateScissorRect();

    if (!CreateRenderTarget())
    {
        debug.Log("Failed CreateRenderTarget");
        return false;
    }

    if (!CreateDepthStencil())
    {
        debug.Log("Failed CreateDepthStencil");
        return false;
    }

    debug.Log("Render Engine Initialize");
    return true;
}

void Engine::RenderBegin()
{
    //  レンダ―ターゲットを現在のバッファのものに更新
    m_currentRenderTarget = m_pRenderTargets[m_currentBackBufferIndex].Get();

    //  コマンド初期化
    m_pAllocator[m_currentBackBufferIndex]->Reset();
    m_pCommandList->Reset(
        m_pAllocator[m_currentBackBufferIndex].Get(),
        nullptr
    );

    //  ビューポート、シザー矩形セット
    m_pCommandList->RSSetViewports(1, &m_viewport);
    m_pCommandList->RSSetScissorRects(1, &m_scissor);

    //  現フレームのRTVのDescriptorHeap開始アドレス
    D3D12_CPU_DESCRIPTOR_HANDLE currentRtvHandler = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();
    currentRtvHandler.ptr += m_currentBackBufferIndex * m_rtvDescriptionSize;

    //  DSVのDescriptorHeap開始アドレス
    D3D12_CPU_DESCRIPTOR_HANDLE currentDsvHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();

    //  RTV使用可能まで待つ
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_currentRenderTarget,
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
    m_pCommandList->ResourceBarrier(1, &barrier);

    //  RTVセット
    m_pCommandList->OMSetRenderTargets(
        1,
        &currentRtvHandler,
        FALSE,
        &currentDsvHandle
    );

    //  RTVクリア
    m_pCommandList->ClearRenderTargetView(
        currentRtvHandler,
        m_clearColor,
        0,
        nullptr
    );

    //  DSVクリア
    m_pCommandList->ClearDepthStencilView(
        currentDsvHandle,
        D3D12_CLEAR_FLAG_DEPTH,
        1.0f,
        0,
        0,
        nullptr
    );
}

void Engine::RenderEnd()
{
    // RTV書き込み終了待ち
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        m_currentRenderTarget, 
        D3D12_RESOURCE_STATE_RENDER_TARGET, 
        D3D12_RESOURCE_STATE_PRESENT
    );
    m_pCommandList->ResourceBarrier(1, &barrier);

    // コマンド記録終了
    m_pCommandList->Close();

    // コマンド実行
    ID3D12CommandList* ppCmdLists[] = { m_pCommandList.Get() };
    m_pCommandQueue->ExecuteCommandLists(1, ppCmdLists);

    // スワップチェーン切替
    m_pSwapChain->Present(1, 0);

    WaitRender();

    // バックバッファ番号更新
    m_currentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();
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

bool Engine::CreateCommandQueue()
{
    D3D12_COMMAND_QUEUE_DESC desc = {};
    desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    desc.NodeMask = 0;

    HRESULT hr = m_pDevice->CreateCommandQueue(
        &desc,
        IID_PPV_ARGS(m_pCommandQueue.ReleaseAndGetAddressOf())
    );

    return SUCCEEDED(hr);
}

bool Engine::CreateSwapChain()
{
    IDXGIFactory4* pFactory = nullptr;
    HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
    if (FAILED(hr))
    {
        debug.Log("Failed CreateSwapChain - CreateDXGIFactory1");
        return false;
    }

    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferDesc.Width = m_frameBufferWidth;
    desc.BufferDesc.Height = m_frameBufferHeight;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = FRAME_BUFFER_COUNT;
    desc.OutputWindow = m_hwnd;
    desc.Windowed = TRUE;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGISwapChain* pSwapChain = nullptr;
    hr = pFactory->CreateSwapChain(
        m_pCommandQueue.Get(),
        &desc,
        &pSwapChain
    );
    if (FAILED(hr))
    {
        pFactory->Release();
        debug.Log("Failed CreateSwapChain - CreateSwapChain");
        return false;
    }

    hr = pSwapChain->QueryInterface(IID_PPV_ARGS(m_pSwapChain.ReleaseAndGetAddressOf()));
    if (FAILED(hr))
    {
        pFactory->Release();
        pSwapChain->Release();
        debug.Log("Failed CreateSwapChain - QueryInterface");
        return false;
    }

    m_currentBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();

    pFactory->Release();
    pSwapChain->Release();

    return true;
}

bool Engine::CreateCommandList()
{
    HRESULT hr;
    for (int i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        hr = m_pDevice->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(m_pAllocator[i].ReleaseAndGetAddressOf())
        );
    }

    if (FAILED(hr))
    {
        debug.Log("Failed CreateCommandList - CreateCommandAllocator");
        return false;
    }

    hr = m_pDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        m_pAllocator[m_currentBackBufferIndex].Get(),
        nullptr,
        IID_PPV_ARGS(&m_pCommandList)
    );

    if (FAILED(hr))
    {
        debug.Log("Failed CreateCommandList - CreateCommandList");
        return false;
    }

    m_pCommandList->Close();

    return true;
}

bool Engine::CreateFence()
{
    for (int i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        m_fenceValue[i] = 0;
    }

    HRESULT hr = m_pDevice->CreateFence(
        0,
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(m_pFence.ReleaseAndGetAddressOf())
    );
    if (FAILED(hr))
    {
        debug.Log("Failed CreateFence - CreateFence");
        return false;
    }

    m_fenceValue[m_currentBackBufferIndex]++;

    m_fenceEvent = CreateEvent(
        nullptr,
        FALSE,
        FALSE,
        nullptr
    );

    return m_fenceEvent != nullptr;
}

void Engine::CreateViewPort()
{
    m_viewport.TopLeftX = 0;
    m_viewport.TopLeftY = 0;
    m_viewport.Width = static_cast<float>(m_frameBufferWidth);
    m_viewport.Height = static_cast<float>(m_frameBufferHeight);
    m_viewport.MinDepth = 0.0f;
    m_viewport.MaxDepth = 1.0f;
}

void Engine::CreateScissorRect()
{
    m_scissor.left = 0;
    m_scissor.right = m_frameBufferWidth;
    m_scissor.top = 0;
    m_scissor.bottom = m_frameBufferHeight;
}

bool Engine::CreateRenderTarget()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = FRAME_BUFFER_COUNT;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT hr = m_pDevice->CreateDescriptorHeap(
        &desc,
        IID_PPV_ARGS(m_pRtvHeap.ReleaseAndGetAddressOf())
    );
    if (FAILED(hr))
    {
        debug.Log("Failed CreateRenderTarget - CreateDescriptorHeap");
        return false;
    }

    m_rtvDescriptionSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT i = 0; i < FRAME_BUFFER_COUNT; i++)
    {
        m_pSwapChain->GetBuffer(
            i,
            IID_PPV_ARGS(m_pRenderTargets[i].ReleaseAndGetAddressOf())
        );
        m_pDevice->CreateRenderTargetView(
            m_pRenderTargets[i].Get(),
            nullptr,
            rtvHandle
        );
        rtvHandle.ptr += m_rtvDescriptionSize;
    }

    return true;
}

bool Engine::CreateDepthStencil()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.NumDescriptors = 1;
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT hr = m_pDevice->CreateDescriptorHeap(
        &desc,
        IID_PPV_ARGS(&m_pDsvHeap)
    );
    if (FAILED(hr))
    {
        debug.Log("Failed CreateDepthStencil - CreateDescriptorHeap");
        return false;
    }


    m_dsvDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

    D3D12_CLEAR_VALUE dsvClearVal;
    dsvClearVal.Format = DXGI_FORMAT_D32_FLOAT;
    dsvClearVal.DepthStencil.Depth = 1.0f;
    dsvClearVal.DepthStencil.Stencil = 0;

    CD3DX12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D,
        0,
        m_frameBufferWidth,
        m_frameBufferHeight,
        1,
        1,
        DXGI_FORMAT_D32_FLOAT,
        1,
        0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE
    );

    hr = m_pDevice->CreateCommittedResource(
        &heapProp,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &dsvClearVal,
        IID_PPV_ARGS(m_pDepthStencilBuffer.ReleaseAndGetAddressOf())
    );
    if (FAILED(hr))
    {
        debug.Log("Failed CreateDepthStencil - CreateCommittedResource");
        return false;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = m_pDsvHeap->GetCPUDescriptorHandleForHeapStart();

    m_pDevice->CreateDepthStencilView(
        m_pDepthStencilBuffer.Get(),
        nullptr,
        dsvHandle
    );

    return true;
}

void Engine::WaitRender()
{
    //  描画待ち
    const UINT64 fenceValue = m_fenceValue[m_currentBackBufferIndex];
    m_pCommandQueue->Signal(m_pFence.Get(), fenceValue);
    m_fenceValue[m_currentBackBufferIndex]++;

    //  次フレーム描画準備待機
    if (m_pFence->GetCompletedValue() < fenceValue)
    {
        HRESULT hr = m_pFence->SetEventOnCompletion(fenceValue, m_fenceEvent);
        if (FAILED(hr))
        {
            return;
        }

        if (WAIT_OBJECT_0 != WaitForSingleObjectEx(
            m_fenceEvent,
            INFINITE,
            FALSE
        ))
        {
            return;
        }
    }
}