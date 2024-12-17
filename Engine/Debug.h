#pragma once
#include <string>
#include <Windows.h>

namespace Debug
{
	void OutPrint(LPCWSTR value, bool isLineFeed);
	void OutPrint(int value, bool isLineFeed);
	void OutPrint(float value, bool isLineFeed);
};

