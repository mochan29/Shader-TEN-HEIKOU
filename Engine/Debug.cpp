#include "Debug.h"

void Debug::OutPrint(LPCWSTR value, bool isLineFeed)
{
	OutputDebugStringW(value);
	if (isLineFeed)	OutputDebugStringW(L"\n");
}

void Debug::OutPrint(int value, bool isLineFeed)
{
	WCHAR str[256];
	swprintf_s(str, L"%d", value);
	Debug::OutPrint(str, isLineFeed);
}

void Debug::OutPrint(float value, bool isLineFeed)
{
	WCHAR str[256];
	swprintf_s(str, L"%f", value);
	Debug::OutPrint(str, isLineFeed);
}