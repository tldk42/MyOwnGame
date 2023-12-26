#define WIN32_LEAN_AND_MEAN

#include <windows.h>

HINSTANCE hInst;

constexpr WCHAR CLASS_NAME[8] = L"WinMain";
constexpr WCHAR APP_TITLE[12] = L"Hello World";
constexpr int WINDOW_WIDTH = 400;
constexpr int WINDOW_HEIGHT = 400;

bool CreateMainWindow(HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd)
{
	MSG msg;

	if (!CreateMainWindow(hInstance, nShowCmd))
		return false;

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
	}

	return msg.wParam;
}

bool CreateMainWindow(HINSTANCE hInstance, int nShowCmd)
{
	WNDCLASSEX wcx;
	HWND hwnd;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WinProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = nullptr;
	wcx.hCursor = LoadCursor(nullptr, IDC_ARROW);

	wcx.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	wcx.lpszMenuName = nullptr;
	wcx.lpszClassName = CLASS_NAME;
	wcx.hIconSm = nullptr;

	if (!RegisterClassEx(&wcx))
		return false;

	hwnd = CreateWindow(CLASS_NAME, APP_TITLE, WS_OVERLAPPEDWINDOW,
	                    CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,
	                    WINDOW_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hwnd)
		return false;

	ShowWindow(hwnd, nShowCmd);

	UpdateWindow(hwnd);
	return true;
}

LRESULT WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
