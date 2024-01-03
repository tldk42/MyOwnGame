#include "Input/JInput.h"

#include "Error/GameError.h"


JInput::JInput()
	: CharIn(0),
	  bNewLine(true),
	  MouseX(0),
	  MouseRawX(0),
	  MouseRawY(0),
	  bMouseLButton(false),
	  bMouseMButton(false),
	  bMouseRButton(false),
	  bMouseX1Button(false),
	  bMouseX2Button(false),
	  ThumbStickDeadZone(GAMEPAD_THUMBSTICK_DEADZONE),
	  TriggerDeadzone(GAMEPAD_TRIGGER_DEADZONE)
{
	// 입력 키 초기화
	for (bool& bKeyDown : KeysDown)
	{
		bKeyDown = false;
	}

	// 입력(눌리는) 키 초기화
	for (bool& bPressedKeyDown : KeysPressed)
	{
		bPressedKeyDown = false;
	}

	// 컨트롤러 진동 초기화
	for (ControllerState& cs : Controllers)
	{
		cs.VibrateTimeLeft = 0;
		cs.VibrateTImeRight = 0;
	}
}

JInput::~JInput()
{
	if (bMouseCaptured)
	{
		ReleaseCapture();
	}
}

void JInput::Initialize(HWND hwnd, bool bCapture)
{
	try
	{
		bMouseCaptured = bCapture;

		// 고밀도 마우스 등록
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = hwnd;
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

		if (bMouseCaptured)
			SetCapture(hwnd);

		// 컨트롤러 상태 비우기
		ZeroMemory(Controllers, sizeof(ControllerState) * MAX_CONTROLLERS);

		CheckControllers();
	}
	catch (...)
	{
		throw GameError(GameErrorNS::FATAL_ERROR, "입력 시스템 초기화 실패");
	}
}

void JInput::KeyDown(const WPARAM wParam)
{
	if (wParam < InputNS::KEYS_ARRAY_LEN)
	{
		KeysDown[wParam] = true;
		KeysPressed[wParam] = true;
	}
}

void JInput::KeyUp(WPARAM wParam)
{
	if (wParam < InputNS::KEYS_ARRAY_LEN)
	{
		KeysDown[wParam] = false;
	}
}

void JInput::KeyIn(WPARAM wParam)
{
	if (bNewLine)
	{
		TextIn.clear();
		bNewLine = false;
	}

	// 입력이 백스페이스
	if (wParam == '\b')
	{
		if (TextIn.length() > 0)
		{
			TextIn.erase(TextIn.size() - 1);
		}
	}
	// 문자 추가
	else
	{
		TextIn += wParam;
		CharIn = wParam;
	}

	// 입력이 Return -> 새로운 라인
	if (static_cast<char>(wParam) == '\r')
	{
		bNewLine = true;
	}
}


bool JInput::IsKeyDown(const UCHAR vKey) const
{
	if (vKey < InputNS::KEYS_ARRAY_LEN)
		return KeysDown[vKey];
	return false;
}

bool JInput::WasKeyPressed(UCHAR vKey) const
{
	if (vKey < InputNS::KEYS_ARRAY_LEN)
		return KeysPressed[vKey];
	return false;
}

bool JInput::AnyKeyPressed() const
{
	// std::any_of() 대체 가능
	for (const bool bKeyPressed : KeysPressed)
	{
		if (bKeyPressed)
		{
			return true;
		}
	}
	return false;
}

void JInput::ClearKeyPress(const UCHAR vKey)
{
	if (vKey < InputNS::KEYS_ARRAY_LEN)
	{
		KeysPressed[vKey] = false;
	}
}

void JInput::Clear(UCHAR KeysPressed)
{
}

void JInput::MouseIn(LPARAM Long)
{
}

void JInput::MouseRawIn(LPARAM Long)
{
}

void JInput::CheckControllers()
{
}

void JInput::ReadControllers()
{
}

BYTE JInput::GetGamepadLeftTrigger(UINT Index) const
{
	BYTE value = GetGamepadLeftTriggerUndead(Index);

	if (value > TriggerDeadzone)
	{
		value = (value - TriggerDeadzone) * 255 / (255 - TriggerDeadzone);
	}
	else
	{
		value = 0;
	}

	return value;
}

SHORT JInput::GetGamepadThumbLX(UINT Index) const
{
	int x = GetGamepadThumbLXUndead(Index);

	if (x > ThumbStickDeadZone)
	{
		x = (x - TriggerDeadzone) * 32767 / (32767 - TriggerDeadzone);
	}
	else if (x < -ThumbStickDeadZone)
	{
		x = (x + ThumbStickDeadZone) * 32767 / (32767 - ThumbStickDeadZone);
	}
	else
	{
		x = 0;
	}

	return static_cast<SHORT>(x);
}

SHORT JInput::GetGamepadThumbLY(UINT Index) const
{
	int x = GetGamepadThumbLYUndead(Index);

	if (x > ThumbStickDeadZone)
	{
		x = (x - TriggerDeadzone) * 32767 / (32767 - TriggerDeadzone);
	}
	else if (x < -ThumbStickDeadZone)
	{
		x = (x + ThumbStickDeadZone) * 32767 / (32767 - ThumbStickDeadZone);
	}
	else
	{
		x = 0;
	}

	return static_cast<SHORT>(x);
}

BYTE JInput::GetGamepadRightTrigger(UINT Index) const
{
	BYTE value = GetGamepadRightTriggerUndead(Index);

	if (value > TriggerDeadzone)
	{
		value = (value - TriggerDeadzone) * 255 / (255 - TriggerDeadzone);
	}
	else
	{
		value = 0;
	}

	return value;
}

SHORT JInput::GetGamepadThumbRX(UINT Index) const
{
	int x = GetGamepadThumbRXUndead(Index);

	if (x > ThumbStickDeadZone)
	{
		x = (x - TriggerDeadzone) * 32767 / (32767 - TriggerDeadzone);
	}
	else if (x < -ThumbStickDeadZone)
	{
		x = (x + ThumbStickDeadZone) * 32767 / (32767 - ThumbStickDeadZone);
	}
	else
	{
		x = 0;
	}

	return static_cast<SHORT>(x);
}

SHORT JInput::GetGamepadThumbRY(UINT Index) const
{
	int y = GetGamepadThumbRYUndead(Index);

	if (y > ThumbStickDeadZone)
	{
		y = (y - TriggerDeadzone) * 32767 / (32767 - TriggerDeadzone);
	}
	else if (y < -ThumbStickDeadZone)
	{
		y = (y + ThumbStickDeadZone) * 32767 / (32767 - ThumbStickDeadZone);
	}
	else
	{
		y = 0;
	}

	return static_cast<SHORT>(y);
}

void JInput::VibrateControllers(const float FrameTime)
{
	for (int i = 0; i < MAX_CONTROLLERS; ++i)
	{
		if (Controllers[i].bConnected)
		{
			Controllers[i].VibrateTimeLeft -= FrameTime;
			if (Controllers[i].VibrateTimeLeft < 0)
			{
				Controllers[i].VibrateTimeLeft = 0;
				Controllers[i].Vibration.wLeftMotorSpeed = 0;
			}

			Controllers[i].VibrateTImeRight -= FrameTime;
			if (Controllers[i].VibrateTImeRight < 0)
			{
				Controllers[i].VibrateTImeRight = 0;
				Controllers[i].Vibration.wRightMotorSpeed = 0;
			}
			XInputSetState(i, &Controllers[i].Vibration);
		}
	}
}
