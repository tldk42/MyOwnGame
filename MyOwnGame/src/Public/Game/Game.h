#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>


class Game
{
public:
	Game();

	virtual ~Game();

	LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/* 게임 초기화*/
	virtual void Initialize(HWND hwnd);

	/* WinMain의 메인 메시지 루프에서 반복해서 실행되는 함수*/
	virtual void Run(HWND hwnd);

	/* 예약된 모든 비디오 메모리 해제 & 그래픽 디바이스 리셋*/
	virtual void ReleaseAll();

	/* 모든 표면을 재생성 & 모든 개체 리셋*/
	virtual void ResetAll();

	/* 예약된 모든 메모리 삭제*/
	virtual void DeleteAll();

	/* 게임 아이템을 렌더링*/
	virtual void RenderGame();

	/* 로스트 상태가 된 그래픽 디바이스 처리*/
	virtual void HandleLostGraphicsDevice();

	/* 게임 종료*/
	inline void ExitGame() const
	{
		PostMessage(Hwnd, WM_DESTROY, 0, 0);
	}

#pragma region 순수 가상

	virtual void Update() = 0;

	virtual void AI() = 0;

	virtual void Collisions() = 0;

	virtual void Render() = 0;

#pragma endregion

#pragma region Getter
	/* Graphics 개체 반환*/
	inline class JGraphics* GetGraphics() const { return Graphics; }

	/* Input 개체 반환*/
	inline class JInput* GetInput() const { return Input; }
#pragma endregion

protected:
	class JGraphics* Graphics;
	class JInput* Input;

	HWND Hwnd{};
	HRESULT Result{};

	/* 성능 카운터 시작 값 */
	LARGE_INTEGER TimeStart{};
	/* 성능 카운터 종료 값 */
	LARGE_INTEGER TimeEnd{};
	/* 성능 카운터 빈도수 */
	LARGE_INTEGER TimerFreq{};

	/* 마지막 프레임에 필요한 시간 */
	float FrameTime{};
	/* 초당 프레임 */
	float Fps{};

	DWORD SleepTime{};

	bool bPaused;
	bool bInitialized;
};
