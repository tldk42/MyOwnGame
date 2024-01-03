#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#pragma region 윈도우
constexpr char CLASS_NAME[8] = "WinMain";
constexpr char APP_TITLE[12] = "Hello World";
constexpr bool FULLSCREEN = false;
constexpr int WINDOW_WIDTH = 400;
constexpr int WINDOW_HEIGHT = 400;
#pragma endregion

#pragma region 게임
constexpr double PI = 3.14159265;
constexpr float FRAME_RATE = 200.f;
constexpr float MIN_FRAME_RATE = 10.f;
constexpr float MIN_FRAME_TIME = 1.f / FRAME_RATE;
constexpr float MAX_FRAME_TIME = 1.f / MIN_FRAME_RATE;
#pragma endregion

#pragma region 키 매핑
constexpr UCHAR ESC_KEY = VK_ESCAPE;
constexpr UCHAR ALT_KEY = VK_MENU;
constexpr UCHAR ENTER_KEY = VK_RETURN;
#pragma endregion

#pragma region 인덱싱
FORCEINLINE UINT ClampIndexByMaxValue(const UINT Index, const DWORD ClampValue)
{
	return Index > ClampValue - 1 ? ClampValue - 1 : Index;
}
#pragma endregion

template <typename T>
FORCEINLINE void SafeRelease(T& Ptr)
{
	if (Ptr)
	{
		Ptr->Release();
		Ptr = nullptr;
	}
}

#define SAFE_RELEASE SafeRelease

// NOTE: 메모리 해제를 중복으로 하려하면 _CrtisValidHeapPointer(block) 에러가 난다.
template <typename T>
FORCEINLINE void SafeDelete(T& Ptr)
{
	if (Ptr)
	{
		delete Ptr;
		Ptr = nullptr;
	}
}

#define SAFE_DELETE SafeDelete

template <typename T>
FORCEINLINE void SafeDeleteArray(T& Ptr)
{
	if (Ptr)
	{
		delete[] Ptr;
		Ptr = NULL;
	}
}

#define SAFE_DELETE_ARRAY SafeDeleteArray

template <typename T>
FORCEINLINE void SafeOnLostDevice(T& Ptr)
{
	if (Ptr)
	{
		Ptr->OnLoastDevice();
	}
}

#define SAFE_ON_LOST_DEVICE SafeOnLostDevice

template <typename T>
FORCEINLINE void SafeOnResetDevice(T& Ptr)
{
	if (Ptr)
	{
		Ptr->OnResetDevice();
	}
}

#define SAFE_ON_RESET_DEVICE SafeOnResetDevice
