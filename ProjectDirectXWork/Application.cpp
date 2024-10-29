#include "Application.h"
#include "ConsoleLog.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			break;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Application::StartApplication(const TCHAR* appName)
{
	InitializeWindow(appName);
	Main();
}

void Application::InitializeWindow(const TCHAR* appName)
{
	hInst = GetModuleHandle(nullptr);
	if (hInst == nullptr)
	{
		return;
	}

	//	�E�B���h�E�̐ݒ�
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInst, IDC_ARROW);
	wc.hbrBackground = GetSysColorBrush(COLOR_BACKGROUND);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = appName;
	wc.hIconSm = LoadIcon(hInst, IDI_APPLICATION);
	// �E�B���h�E�N���X�̓o�^�B
	RegisterClassEx(&wc);
	//	 �E�B���h�E�T�C�Y�̐ݒ�
	RECT rect = {};
	rect.right = static_cast<LONG>(WINDOW_WIDTH);
	rect.bottom = static_cast<LONG>(WINDOW_HEIGHT);
	//	�E�B���h�E�T�C�Y�𒲐�
	auto style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
	AdjustWindowRect(&rect, style, FALSE);

	//	�E�B���h�E�̐���
	hWnd = CreateWindowEx(
		0,
		appName,
		appName,
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		nullptr,
		nullptr,
		hInst,
		nullptr
	);

	//	 �E�B���h�E��\��
	ShowWindow(hWnd, SW_SHOWNORMAL);
	//	 �E�B���h�E�Ƀt�H�[�J�X����
	SetFocus(hWnd);
}

void Application::Main()
{
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE == TRUE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//	�`�揈��
			Render();
		}
	}
}

void Application::Render()
{
	
}
