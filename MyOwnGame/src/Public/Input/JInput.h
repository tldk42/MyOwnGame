#pragma once
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include <XInput.h>

#include "Data/Constants.h"

// for high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif

namespace InputNS
{
	constexpr int KEYS_ARRAY_LEN = 256;

	constexpr UCHAR KEYS_DOWN = 1;
	constexpr UCHAR KEYS_PRESSED = 2;
	constexpr UCHAR MOUSE = 4;
	constexpr UCHAR TEXT_IN = 8;
	constexpr UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE +
		TEXT_IN;
}

constexpr short GAMEPAD_THUMBSTICK_DEADZONE = (short)(0.20f * 0X7FFF);
constexpr short GAMEPAD_TRIGGER_DEADZONE = 20;
constexpr DWORD MAX_CONTROLLERS = 4;
constexpr DWORD GAMEPAD_DPAD_UP = 0x0001;
constexpr DWORD GAMEPAD_DPAD_DOWN = 0x0002;
constexpr DWORD GAMEPAD_DPAD_LEFT = 0x0004;
constexpr DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
constexpr DWORD GAMEPAD_START_BUTTON = 0x0010;
constexpr DWORD GAMEPAD_BACK_BUTTON = 0x0020;
constexpr DWORD GAMEPAD_LEFT_THUMB = 0x0040;
constexpr DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
constexpr DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
constexpr DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
constexpr DWORD GAMEPAD_A = 0x1000;
constexpr DWORD GAMEPAD_B = 0x2000;
constexpr DWORD GAMEPAD_X = 0x4000;
constexpr DWORD GAMEPAD_Y = 0x8000;

struct ControllerState
{
	XINPUT_STATE State;
	XINPUT_VIBRATION Vibration;
	float VibrateTimeLeft;
	float VibrateTImeRight;
	bool bConnected;
};

class JInput
{
public:
	JInput();

	virtual ~JInput();

	void Initialize(HWND hwnd, bool bCapture);

#pragma region Ű����
	/* Ű�� ���� ���·� ����*/
	void KeyDown(WPARAM wParam);
	/* Ű�� �ȴ��� ���·� ����*/
	void KeyUp(WPARAM wParam);
	/* �Էµ� ���ڸ� ����*/
	void KeyIn(WPARAM wParam);

	bool IsKeyDown(UCHAR vKey) const;
	bool WasKeyPressed(UCHAR vKey) const;
	bool AnyKeyPressed() const;

	void ClearKeyPress(UCHAR vKey);
	void Clear(UCHAR KeysPressed);

	FORCEINLINE void ClearAll()
	{
		Clear(InputNS::KEYS_MOUSE_TEXT);
	}

	FORCEINLINE void ClearTextIn()
	{
		TextIn.clear();
	}

	FORCEINLINE void ClearCharIn()
	{
		CharIn = 0;
	}

	FORCEINLINE std::string GetTextIn() const
	{
		return TextIn;
	}

	FORCEINLINE char GetCharIn() const
	{
		return CharIn;
	}

	FORCEINLINE void SetTextIn(const std::string& InString)
	{
		TextIn = InString;
	}
#pragma endregion

#pragma region ���콺
	void MouseIn(LPARAM Long);
	void MouseRawIn(LPARAM Long);

	FORCEINLINE int GetMouseX() const
	{
		return MouseX;
	}

	FORCEINLINE int GetMouseY() const
	{
		return MouseY;
	}

	FORCEINLINE int GetMouseRawX() const
	{
		return MouseRawX;
	}

	FORCEINLINE int GetMouseRawY() const
	{
		return MouseRawY;
	}

	FORCEINLINE int GetMouseWheel() const
	{
		return MouseWheel;
	}

	FORCEINLINE bool GetMouseLButton() const
	{
		return bMouseLButton;
	}

	FORCEINLINE bool GetMouseMButton() const
	{
		return bMouseMButton;
	}

	FORCEINLINE bool GetMouseRButton() const
	{
		return bMouseRButton;
	}

	FORCEINLINE bool GetMouseX1Button() const
	{
		return bMouseX1Button;
	}

	FORCEINLINE bool GetMouseX2Button() const
	{
		return bMouseX2Button;
	}

	FORCEINLINE void SetMouseLButton(const bool Cond)
	{
		bMouseLButton = Cond;
	}

	FORCEINLINE void SetMouseMButton(const bool Cond)
	{
		bMouseMButton = Cond;
	}

	FORCEINLINE void SetMouseRButton(const bool Cond)
	{
		bMouseRButton = Cond;
	}

	FORCEINLINE void SetMouseXButton(const WPARAM wParam)
	{
		bMouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		bMouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}
#pragma endregion

#pragma region ���� �е� XBOX
	void CheckControllers();
	void ReadControllers();

	FORCEINLINE void SetThumbstickDeadzone(const short InDeadzone)
	{
		ThumbStickDeadZone = abs(InDeadzone);
	}

	FORCEINLINE void SetTriggerDeadzone(const BYTE InDeadzone)
	{
		TriggerDeadzone = InDeadzone;
	}

	FORCEINLINE short GetThumbstickDeadzone() const
	{
		return ThumbStickDeadZone;
	}

	FORCEINLINE BYTE GetTriggerDeadzone() const
	{
		return static_cast<BYTE>(TriggerDeadzone);
	}

	FORCEINLINE const ControllerState* GetControllerState(
		const UINT Index) const
	{
		return &Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)];
	}

	FORCEINLINE bool GetGamepadConnected(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].
			bConnected;
	}

	FORCEINLINE WORD GetGamepadButtons(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons;
	}

	FORCEINLINE bool GetGamepadDPadUp(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].
			bConnected;
	}

	FORCEINLINE bool GetGamepadDPadDown(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_DPAD_DOWN) != 0;
	}

	FORCEINLINE bool GetGamepadDPadLeft(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_DPAD_LEFT) != 0;
	}

	FORCEINLINE bool GetGamepadDPadRight(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_DPAD_RIGHT) != 0;
	}

	FORCEINLINE bool GetGamepadStart(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_START_BUTTON) != 0;
	}

	FORCEINLINE bool GetGamepadBack(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_BACK_BUTTON) != 0;
	}

	FORCEINLINE bool GetGamepadLeftThumb(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_LEFT_THUMB) != 0;
	}

	FORCEINLINE bool GetGamepadRightThumb(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_RIGHT_THUMB) != 0;
	}

	FORCEINLINE bool GetGamepadLeftShoulder(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_LEFT_SHOULDER) != 0;
	}

	FORCEINLINE bool GetGamepadRightShoulder(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_RIGHT_SHOULDER) != 0;
	}

	FORCEINLINE bool GetGamepadA(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_A) != 0;
	}

	FORCEINLINE bool GetGamepadB(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_B) != 0;
	}

	FORCEINLINE bool GetGamepadX(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_X) != 0;
	}

	FORCEINLINE bool GetGamepadY(const UINT Index) const
	{
		return (Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.wButtons & GAMEPAD_Y) != 0;
	}

	FORCEINLINE BYTE GetGamepadLeftTriggerUndead(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.bLeftTrigger;
	}

	FORCEINLINE SHORT GetGamepadThumbLXUndead(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.sThumbLX;
	}

	FORCEINLINE SHORT GetGamepadThumbLYUndead(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.sThumbLY;
	}

	FORCEINLINE BYTE GetGamepadRightTriggerUndead(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.bRightTrigger;
	}

	FORCEINLINE SHORT GetGamepadThumbRXUndead(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.sThumbRX;
	}

	FORCEINLINE SHORT GetGamepadThumbRYUndead(const UINT Index) const
	{
		return Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].State.
			Gamepad.sThumbRY;
	}

	BYTE GetGamepadLeftTrigger(UINT Index) const;
	SHORT GetGamepadThumbLX(UINT Index) const;
	SHORT GetGamepadThumbLY(UINT Index) const;
	BYTE GetGamepadRightTrigger(UINT Index) const;
	SHORT GetGamepadThumbRX(UINT Index) const;
	SHORT GetGamepadThumbRY(UINT Index) const;

	FORCEINLINE void GamepadVibrateLeft(const UINT Index, const DWORD speed,
	                                    const float sec)
	{
		Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].Vibration.
			wLeftMotorSpeed = speed;
		Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].
			VibrateTimeLeft =
			sec;
	}

	FORCEINLINE void GamepadVibrateRight(const UINT Index, const DWORD speed,
	                                     const float sec)
	{
		Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].Vibration.
			wRightMotorSpeed = speed;
		Controllers[ClampIndexByMaxValue(Index, MAX_CONTROLLERS)].
			VibrateTimeLeft =
			sec;
	}

	void VibrateControllers(float FrameTime);

#pragma endregion

private:
	/* Ű�� ���ȴ��� �迭*/
	bool KeysDown[InputNS::KEYS_ARRAY_LEN]{};
	/* Ű�� ������ �ִ��� �迭*/
	bool KeysPressed[InputNS::KEYS_ARRAY_LEN]{};
	/* �Էµ� �ؽ�Ʈ */
	std::string TextIn;
	/* ���������� �Էµ� ���� */
	char CharIn;
	/* �� �ٷ� �����ؾ��ϴ��� */
	bool bNewLine;
	int MouseX;
	int MouseY{};
	int MouseRawX;
	int MouseRawY;
	int MouseWheel{};

	/* ���� ���콺�� ���� �ʿ� */
	RAWINPUTDEVICE Rid[1]{};
	bool bMouseCaptured{};
	bool bMouseLButton;
	bool bMouseMButton;
	bool bMouseRButton;
	bool bMouseX1Button;
	bool bMouseX2Button;

	ControllerState Controllers[MAX_CONTROLLERS]{};
	short ThumbStickDeadZone;
	short TriggerDeadzone;
};
