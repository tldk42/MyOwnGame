#define CRTDBG_MAP_ALLOC       // �޸� ���� Ȯ�ο�
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cstdlib>             // �޸� ���� Ȯ�ο�
#include <crtdbg.h>             // �޸� ���� Ȯ�ο�

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
	// ����� ����� �޸� ���� üũ
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
		MessageBox(nullptr, "�� �� ���� ���� �߻�.", "Error", MB_OK);
	}

	SAFE_DELETE(game);
	return 0;
}

/**
 * \brief ���� ������ ����
 * \return ���� ����
 */
bool CreateMainWindow(HWND& hwnd, HINSTANCE hInstance, int nShowCmd)
{
	WNDCLASSEX wcx;

	// ������ ���� ���� �ʱ�ȭ
	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW; // ������ ����� �ٽ� �׷���
	wcx.lpfnWndProc = WinProc; // ������ ���ν��� ������
	wcx.cbClsExtra = 0; // Ŭ���� �߰� �޸�
	wcx.cbWndExtra = 0; // ������ �߰� �޸�
	wcx.hInstance = hInstance; // �ڵ� �ν��Ͻ�

	wcx.hIcon = nullptr;
	wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));

	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = nullptr;

	// ������ Ŭ���� ���
	if (!RegisterClassEx(&wcx))
		return false;

	constexpr DWORD style = FULLSCREEN
		                        ? (WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP)
		                        : WS_OVERLAPPEDWINDOW;

	// ������ ����
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
		// ���� Ŭ���̾�Ʈ ����
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);

		// ���� ũ��� Ÿ��Ʋ�� �׵θ��� ���ԵǹǷ� �ٽ� ���.
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
