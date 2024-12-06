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

	void StartApplication(const TCHAR* appName);

	const UINT WINDOW_WIDTH = 1080;
	const UINT WINDOW_HEIGHT = 720;

private:
	void InitializeWindow(const TCHAR* appName);
	void Main();

	HINSTANCE m_hInst;
	HWND m_hWnd = NULL;
};

