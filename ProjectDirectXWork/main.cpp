#include "Application.h"
#include "ConsoleLog.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application::Get().StartApplication();

	return 0;
}