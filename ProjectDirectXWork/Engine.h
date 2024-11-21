#pragma once
#include <d3d12.h>
#include <dxgi.h>
#include <dxgi1_4.h>
#include "ComPtr.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

class Engine
{
public:
	static Engine& Get()
	{
		static Engine instance;
		return instance;
	}

	bool Init(
		HWND hwnd,
		UINT windowWidth,
		UINT windowHeight
	);
	void RenderBegin();
	void RenderEnd();


	ID3D12Device6* Device();
	ID3D12GraphicsCommandList* CommandList();
	UINT CurrentBackBufferIndex();


	static const int FRAME_BUFFER_COUNT = 2;

private:
	bool CreateDevice();
	bool CreateCommandQueue();
	bool CreateSwapChain();
	bool CreateCommandList();
	bool CreateFence();
	bool CreateViewPort();
	bool CreateScissorRect();

	bool CreateRenderTarget();
	bool CreateDepthStencil();

	void WaitRender();

	HWND m_hwnd;
	UINT m_frameBufferWidth = 0;
	UINT m_frameBufferHeight = 0;
	UINT m_currentBackBufferIndex = 0;

	ComPtr<ID3D12Device6> m_pDevice = nullptr;
	ComPtr<ID3D12CommandQueue> m_pCommandQueue = nullptr;
	ComPtr<IDXGISwapChain3> m_pSwapChain = nullptr;
	ComPtr<ID3D12CommandAllocator> m_pAllocator[FRAME_BUFFER_COUNT] = { nullptr };
	ComPtr<ID3D12GraphicsCommandList> m_pCommandList = nullptr;
	HANDLE m_fenceEvent = nullptr;
	ComPtr<ID3D12Fence> m_pFence = nullptr;
	UINT64 m_fenceValue[FRAME_BUFFER_COUNT];
	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissor;

	UINT m_rtvDescriptionSize = 0;
	ComPtr<ID3D12DescriptorHeap> m_pRtvHeap = nullptr;
	ComPtr<ID3D12Resource> m_pRenderTargets[FRAME_BUFFER_COUNT] = { nullptr };

	UINT m_dsvDescriptorSize = 0;
	ComPtr<ID3D12DescriptorHeap> m_pDsvHeap = nullptr;
	ComPtr<ID3D12Resource> m_pDepthStencilBuffer = nullptr;

	ID3D12Resource* m_currentRenderTarget = nullptr;
};

