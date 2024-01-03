#include "Game/Game.h"

#include "Data/Constants.h"
#include "Error/GameError.h"
#include "Graphics/JGraphics.h"
#include "Input/JInput.h"

Game::Game()
	: bPaused(false),
	  bInitialized(false)
{
	Input = new JInput;
	Graphics = nullptr;
}

Game::~Game()
{
	DeleteAll();
	ShowCursor(true);
}

LRESULT Game::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (bInitialized)
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			Input->KeyUp(wParam);
			return 0;
		case WM_CHAR:
			Input->KeyIn(wParam);
			return 0;
		case WM_MOUSEMOVE:
			Input->MouseIn(lParam);
			return 0;
		case WM_INPUT:
			Input->MouseRawIn(lParam);
			return 0;
		case WM_LBUTTONDOWN:
			Input->SetMouseLButton(true);
			Input->MouseIn(lParam);
			return 0;
		case WM_LBUTTONUP:
			Input->SetMouseLButton(false);
			Input->MouseIn(lParam);
			return 0;
		case WM_MBUTTONDOWN:
			Input->SetMouseMButton(true);
			Input->MouseIn(lParam);
			return 0;
		case WM_MBUTTONUP:
			Input->SetMouseMButton(false);
			Input->MouseIn(lParam);
			return 0;
		case WM_RBUTTONDOWN:
			Input->SetMouseRButton(true);
			Input->MouseIn(lParam);
			return 0;
		case WM_RBUTTONUP:
			Input->SetMouseRButton(false);
			Input->MouseIn(lParam);
			return 0;
		case WM_XBUTTONUP:
		case WM_XBUTTONDOWN:
			Input->SetMouseXButton(wParam);
			Input->MouseIn(lParam);
			return 0;
		case WM_DEVICECHANGE:
			Input->CheckControllers();
			return 0;

		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Game::Initialize(HWND hwnd)
{
	Hwnd = hwnd;

	Graphics = new JGraphics;
	Graphics->Initialize(Hwnd, WINDOW_WIDTH, WINDOW_HEIGHT, FULLSCREEN);

	Input->Initialize(Hwnd, false);

	if (QueryPerformanceFrequency(&TimerFreq) == false)
	{
		throw(GameError(GameErrorNS::FATAL_ERROR,
		                "Error initializing high resolution timer"));
	}

	QueryPerformanceCounter(&TimeStart);

	bInitialized = true;
}

void Game::Run(HWND hwnd)
{
	if (Graphics == nullptr)
		return;

	QueryPerformanceCounter(&TimeEnd);

	FrameTime = static_cast<float>(TimeEnd.QuadPart - TimeStart.QuadPart) /
		static_cast<float>(TimerFreq.QuadPart);

	/* 프레임에서 남는 시간이 있으면 그 시간동안은 CPU를 절전시킴 */
	if (FrameTime < MIN_FRAME_TIME)
	{
		SleepTime = static_cast<DWORD>((MIN_FRAME_TIME - FrameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(SleepTime);
		timeEndPeriod(1);
		return;
	}

	if (FrameTime > 0.f)
	{
		Fps = (Fps * 0.99f) + (0.01f / FrameTime);
	}

	if (FrameTime > MAX_FRAME_TIME)
	{
		FrameTime = MAX_FRAME_TIME;
	}

	TimeStart = TimeEnd;

	if (!bPaused)
	{
		Update();
		AI();
		Collisions();
		Input->VibrateControllers(FrameTime);
	}
	RenderGame();

	Input->ReadControllers();
	Input->Clear(InputNS::KEYS_PRESSED);
}

void Game::ReleaseAll()
{
}

void Game::ResetAll()
{
}

void Game::DeleteAll()
{
	ReleaseAll();
	SAFE_DELETE(Graphics);
	SAFE_DELETE(Input);
	bInitialized = false;
}


void Game::HandleLostGraphicsDevice()
{
	Result = Graphics->GetDeviceState();

	if (FAILED(Result))
	{
		if (Result == D3DERR_DEVICELOST)
		{
			Sleep(100);
			return;
		}
		else if (Result == D3DERR_DEVICENOTRESET)
		{
			ReleaseAll();
			Result = Graphics->Reset();

			if (FAILED(Result))
			{
				return;
			}

			ResetAll();
		}
	}
}


void Game::RenderGame()
{
	if (SUCCEEDED(Graphics->BeginScene()))
	{
		Render();

		Graphics->EndScene();
	}

	HandleLostGraphicsDevice();

	Graphics->ShowBackbuffer();
}
