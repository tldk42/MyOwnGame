#include "Data/Constants.h"
#include "Graphics/JGraphics.h"

#include "Error/GameError.h"


JGraphics::JGraphics()
	:
	Direct3D(nullptr),
	Device3D(nullptr),
	Fullscreen(false),
	Width(WINDOW_WIDTH),
	Height(WINDOW_HEIGHT)
{
}

JGraphics::~JGraphics()
{
	ReleaseAll();
}

void JGraphics::InitD3Dpp()

{
	try
	{
		ZeroMemory(&D3DPP, sizeof(D3DPP)); // D3DPP ����ü�� 0���� �ʱ�ȭ

		D3DPP.BackBufferWidth = Width;
		D3DPP.BackBufferHeight = Height;
		D3DPP.BackBufferFormat = Fullscreen ? D3DFMT_X8R8G8B8 : D3DFMT_UNKNOWN;
		// ��üȭ�� ? 24��Ʈ ���� : ����ũž ���� ���
		D3DPP.BackBufferCount = 1;
		D3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
		D3DPP.hDeviceWindow = Hwnd;
		D3DPP.Windowed = !Fullscreen;
		D3DPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch (...)
	{
		throw GameError(GameErrorNS::FATAL_ERROR,
		                "Error initializing D3D presentation parameters");
	}
}

void JGraphics::Initialize(HWND hw, int inWidth, int inHeight,
                           bool inFullscreen)
{
	Hwnd = hw;
	Width = inWidth;
	Height = inHeight;
	Fullscreen = inFullscreen;

	// D3D ���� �� �ʱ�ȭ
	Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (Direct3D == nullptr)
		throw GameError(GameErrorNS::FATAL_ERROR,
		                "Error initializing Direct3D");

	InitD3Dpp();

	if (Fullscreen)
	{
		if (IsAdapterCompatible())
		{
			D3DPP.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		}
		else
		{
			throw GameError(GameErrorNS::FATAL_ERROR,
			                "�׷��� ��ġ / ����Ͱ� ȣȯ���� ����");
		}
	}

	D3DCAPS9 caps;
	DWORD behavior;
	Result = Direct3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// D3D Device ���� �� �ʱ�ȭ
	Result = Direct3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		Hwnd,
		behavior,
		&D3DPP,
		&Device3D);

	if (FAILED(Result))
		throw(GameError(GameErrorNS::FATAL_ERROR,
		                "Error creating Direct3D device"));
}

HRESULT JGraphics::Reset()
{
	InitD3Dpp(); // D3D Presentation Parameters �ʱ�ȭ
	Result = Device3D->Reset(&D3DPP); // Graphic Device Reset
	return Result;
}

HRESULT JGraphics::GetDeviceState() const
{
	return Device3D == nullptr ? E_FAIL : Device3D->TestCooperativeLevel();
}

HRESULT JGraphics::BeginScene() const
{
	if (Device3D == nullptr)
		return E_FAIL;

	Device3D->Clear(0, nullptr, D3DCLEAR_TARGET, BackColor, 1.f, 0);

	return Device3D->BeginScene();
}

HRESULT JGraphics::EndScene() const
{
	return Device3D == nullptr ? E_FAIL : Device3D->EndScene();
}

void JGraphics::ReleaseAll()
{
	SAFE_RELEASE(Device3D);
	SAFE_RELEASE(Direct3D);
}


HRESULT JGraphics::ShowBackbuffer()
{
	Result = E_FAIL;
	Device3D->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 0.0f,
	                0);

	Result = Device3D->Present(nullptr, nullptr, nullptr, nullptr);

	return Result;
}

bool JGraphics::IsAdapterCompatible()
{
	const UINT modes = Direct3D->GetAdapterModeCount(
		D3DADAPTER_DEFAULT, D3DPP.BackBufferFormat);

	for (UINT i = 0; i < modes; ++i)
	{
		Result = Direct3D->EnumAdapterModes(
			D3DADAPTER_DEFAULT, D3DPP.BackBufferFormat, i, &pMode);

		if (pMode.Height == D3DPP.BackBufferHeight && pMode.Width == D3DPP.
			BackBufferWidth && pMode.RefreshRate >= D3DPP.
			FullScreen_RefreshRateInHz)
		{
			return true;
		}
	}

	return false;
}
