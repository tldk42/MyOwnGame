#define CRTDBG_MAP_ALLOC       // �޸� ���� Ȯ�ο�
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cstdlib>             // �޸� ���� Ȯ�ο�
#include <crtdbg.h>             // �޸� ���� Ȯ�ο�
#include "Graphics/Graphics.h"


HINSTANCE hInst;
Graphics* graphics;

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
	HWND hwnd = nullptr;

	if (!CreateMainWindow(hwnd, hInstance, nShowCmd))
		return 1;

	try
	{
		graphics = new Graphics();
		graphics->Initialize(hwnd, WINDOW_WIDTH, WINDOW_HEIGHT, FULLSCREEN);

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
				graphics->ShowBackbuffer();
			}
		}
		SAFE_DELETE(graphics);
		return msg.wParam;
	}
	catch (const GameError& error)
	{
		MessageBox(nullptr, error.GetMessage(), "Error", MB_OK);
	}
	catch (...)
	{
		MessageBox(nullptr, "�� �� ���� ���� �߻�.", "Error", MB_OK);
	}

	SAFE_DELETE(graphics);
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

	// ������ ����
	hwnd = CreateWindow(CLASS_NAME, APP_TITLE, WS_OVERLAPPEDWINDOW,
	                    CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
	                    WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
		return false;

	ShowWindow(hwnd, nShowCmd);

	UpdateWindow(hwnd);
	return true;
}

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	// case WM_CHAR:
	// 	switch (wParam)
	// 	{
	// 	case 0x08:
	// 	case 0x09:
	// 	case 0x0A:
	// 	case 0x0D:
	// 	case 0x1B:
	// 		MessageBeep((UINT)-1);
	// 		return 0;
	// 	default:
	// 		ch = (TCHAR)wParam;
	// 		InvalidateRect(hWnd, nullptr, TRUE);
	// 		return 0;
	// 	}
	// case WM_PAINT:
	// 	hdc = BeginPaint(hWnd, &ps);
	// 	GetClientRect(hWnd, &rect);
	// 	TextOut(hdc, rect.right / 2, rect.bottom / 2, &ch, 1);
	// 	EndPaint(hWnd, &ps);
	// 	return 0;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
