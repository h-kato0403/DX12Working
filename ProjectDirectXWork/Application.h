#pragma once
#include<windows.h>

class Application
{
public:
	static Application& Get()
	{
		static Application instance;
		return instance;
	}

	void StartApplication();

	const UINT WINDOW_WIDTH = 1080;
	const UINT WINDOW_HEIGHT = 720;
	const TCHAR* WINDOW_NAME = TEXT("Game");

private:
	void InitializeWindow();
	void Main();

	HINSTANCE m_hInst;
	HWND m_hWnd = NULL;
};