#pragma once
#include<windows.h>

class Application
{
	const UINT WINDOW_WIDTH = 1080;
	const UINT WINDOW_HEIGHT = 720;

public:
	static Application& Get()
	{
		static Application instance;
		return instance;
	}

	void StartApplication(const TCHAR* appName);
private:

	void InitializeWindow(const TCHAR* appName);
	void Main();
	void Render();

private:
	HINSTANCE hInst;
	HWND hWnd = NULL;
};

