#include "Application.h"

int wmain(int argc, wchar_t** argv, wchar_t** envp)
{
	Application app = Application::Get();
	app.StartApplication(TEXT("Test Window"));

	return 0;
}