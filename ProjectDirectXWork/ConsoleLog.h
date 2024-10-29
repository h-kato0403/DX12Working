#pragma once
#include <iostream>

class Console
{
public:
	static Console& Get()
	{
		static Console instance;
		return instance;
	}

	void Log(const char* text)
	{
		std::cout << text << std::endl;
	}
};

