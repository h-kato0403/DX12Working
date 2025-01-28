#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fcntl.h>

#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN

class Console
{
public:
	static Console& Get()
	{
		static Console instance;
		return instance;
	}

	void ConsoleWindowCreate()
	{
		// コンソール作成
		AllocConsole();

		// 標準出力リダイレクト
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr);
		freopen_s(&fp, "CONIN$", "r", stdin);

		Log("Console Window Initialized");
	}
	void ConsoleWindowClose()
	{
		FreeConsole();
	}

	void Log(const char* text)
	{
		std::cout << text << std::endl;
	}
};

