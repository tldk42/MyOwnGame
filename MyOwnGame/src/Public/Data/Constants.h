#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

constexpr char CLASS_NAME[8] = "WinMain";
constexpr char APP_TITLE[12] = "Hello World";
constexpr bool FULLSCREEN = false;
constexpr int WINDOW_WIDTH = 400;
constexpr int WINDOW_HEIGHT = 400;

constexpr UCHAR ESC_KEY = VK_ESCAPE;

template <typename T>
inline void SafeRelease(T& Ptr)
{
	if (Ptr)
	{
		Ptr->Release();
		Ptr = nullptr;
	}
}

#define SAFE_RELEASE SafeRelease

// NOTE: �޸� ������ �ߺ����� �Ϸ��ϸ� _CrtisValidHeapPointer(block) ������ ����.
template <typename T>
inline void SafeDelete(T& Ptr)
{
	if (Ptr)
	{
		delete Ptr;
		Ptr = nullptr;
	}
}

#define SAFE_DELETE SafeDelete

template <typename T>
inline void SafeDeleteArray(T& Ptr)
{
	if (Ptr)
	{
		delete[] Ptr;
		Ptr = NULL;
	}
}

#define SAFE_DELETE_ARRAY SafeDeleteArray
