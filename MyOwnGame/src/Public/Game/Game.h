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

	/* ���� �ʱ�ȭ*/
	virtual void Initialize(HWND hwnd);

	/* WinMain�� ���� �޽��� �������� �ݺ��ؼ� ����Ǵ� �Լ�*/
	virtual void Run(HWND hwnd);

	/* ����� ��� ���� �޸� ���� & �׷��� ����̽� ����*/
	virtual void ReleaseAll();

	/* ��� ǥ���� ����� & ��� ��ü ����*/
	virtual void ResetAll();

	/* ����� ��� �޸� ����*/
	virtual void DeleteAll();

	/* ���� �������� ������*/
	virtual void RenderGame();

	/* �ν�Ʈ ���°� �� �׷��� ����̽� ó��*/
	virtual void HandleLostGraphicsDevice();

	/* ���� ����*/
	inline void ExitGame() const
	{
		PostMessage(Hwnd, WM_DESTROY, 0, 0);
	}

#pragma region ���� ����

	virtual void Update() = 0;

	virtual void AI() = 0;

	virtual void Collisions() = 0;

	virtual void Render() = 0;

#pragma endregion

#pragma region Getter
	/* Graphics ��ü ��ȯ*/
	inline class JGraphics* GetGraphics() const { return Graphics; }

	/* Input ��ü ��ȯ*/
	inline class JInput* GetInput() const { return Input; }
#pragma endregion

protected:
	class JGraphics* Graphics;
	class JInput* Input;

	HWND Hwnd{};
	HRESULT Result{};

	/* ���� ī���� ���� �� */
	LARGE_INTEGER TimeStart{};
	/* ���� ī���� ���� �� */
	LARGE_INTEGER TimeEnd{};
	/* ���� ī���� �󵵼� */
	LARGE_INTEGER TimerFreq{};

	/* ������ �����ӿ� �ʿ��� �ð� */
	float FrameTime{};
	/* �ʴ� ������ */
	float Fps{};

	DWORD SleepTime{};

	bool bPaused;
	bool bInitialized;
};
