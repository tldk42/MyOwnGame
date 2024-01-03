#define CRTDBG_MAP_ALLOC       // 메모리 누수 확인용
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cstdlib>             // 메모리 누수 확인용
#include <crtdbg.h>             // 메모리 누수 확인용

#include "Data/Constants.h"
#include "Error/GameError.h"
#include "Graphics/JGraphics.h"
#include "Game/Spacewar.h"


Spacewar* game = nullptr;
HWND ghwnd = nullptr;

bool CreateMainWindow(HWND& hwnd, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd)
{
	// 디버그 빌드시 메모리 누수 체크
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	MSG msg;

	game = new Spacewar;
	if (!CreateMainWindow(ghwnd, hInstance, nShowCmd))
		return 1;

	try
	{
		game->Initialize(ghwnd);

		int done = 0;

		while (!done)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					done = 1;

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				game->Run(ghwnd);
			}
		}
		SAFE_DELETE(game);
		return msg.wParam;
	}
	catch (const GameError& error)
	{
		game->DeleteAll();
		DestroyWindow(ghwnd);
		MessageBox(nullptr, error.GetMessage(), "Error", MB_OK);
	}
	catch (...)
	{
		game->DeleteAll();
		DestroyWindow(ghwnd);
		MessageBox(nullptr, "알 수 없는 에러 발생.", "Error", MB_OK);
	}

	SAFE_DELETE(game);
	return 0;
}

/**
 * \brief 메인 윈도우 생성
 * \return 성공 여부
 */
bool CreateMainWindow(HWND& hwnd, HINSTANCE hInstance, int nShowCmd)
{
	WNDCLASSEX wcx;

	// 윈도우 관련 변수 초기화
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW; // 사이즈 변경시 다시 그려짐
	wcx.lpfnWndProc = WinProc; // 윈도우 프로시저 포인터
	wcx.cbClsExtra = 0; // 클래스 추가 메모리
	wcx.cbWndExtra = 0; // 윈도우 추가 메모리
	wcx.hInstance = hInstance; // 핸들 인스턴스

	wcx.hIcon = nullptr;
	wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));

	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = nullptr;

	// 윈도우 클래스 등록
	if (!RegisterClassEx(&wcx))
		return false;

	constexpr DWORD style = FULLSCREEN
		                        ? (WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP)
		                        : WS_OVERLAPPEDWINDOW;

	// 윈도우 생성
	hwnd = CreateWindow(CLASS_NAME,
	                    APP_TITLE,
	                    style,
	                    CW_USEDEFAULT,
	                    CW_USEDEFAULT,
	                    WINDOW_WIDTH,
	                    WINDOW_HEIGHT,
	                    nullptr,
	                    nullptr,
	                    hInstance,
	                    nullptr);

	if (!hwnd)
		return false;

	if (!FULLSCREEN)
	{
		// 순수 클라이언트 영역
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);

		// 실제 크기는 타이틀과 테두리가 포함되므로 다시 계산.
		MoveWindow(hwnd,
		           0,
		           0,
		           WINDOW_WIDTH + (WINDOW_WIDTH - clientRect.right),
		           WINDOW_HEIGHT + (WINDOW_HEIGHT - clientRect.bottom),
		           TRUE);
	}

	ShowWindow(hwnd, nShowCmd);

	return true;
}

LRESULT WINAPI WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return game->MessageHandler(hwnd, msg, wParam, lParam);
}
