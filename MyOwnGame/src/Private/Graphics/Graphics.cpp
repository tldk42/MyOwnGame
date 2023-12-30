#include "Graphics/Graphics.h"

#include "Error/GameError.h"


Graphics::Graphics()
	:
	direct3d(nullptr),
	device3d(nullptr),
	fullscreen(false),
	width(WINDOW_WIDTH),
	height(WINDOW_HEIGHT)
{
}

Graphics::~Graphics()
{
	ReleaseAll();
}

void Graphics::InitD3Dpp()

{
	try
	{
		ZeroMemory(&d3dpp, sizeof(d3dpp)); // d3dpp 구조체를 0으로 초기화

		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		d3dpp.BackBufferFormat = fullscreen ? D3DFMT_X8R8G8B8 : D3DFMT_UNKNOWN; // 전체화면 ? 24비트 색상 : 데스크탑 설정 사용
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = !fullscreen;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch (...)
	{
		throw GameError(GameErrorNS::FATAL_ERROR,
		                "Error initializing D3D presentation parameters");
	}
}

void Graphics::Initialize(HWND hw, int inWidth, int inHeight, bool inFullscreen)
{
	hwnd = hw;
	width = inWidth;
	height = inHeight;
	fullscreen = inFullscreen;

	// D3D 생성 및 초기화
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == nullptr)
		throw(GameError(GameErrorNS::FATAL_ERROR,
		                "Error initializing Direct3D"));

	InitD3Dpp();

	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// D3D Device 생성 및 초기화
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&d3dpp,
		&device3d);

	if (FAILED(result))
		throw(GameError(GameErrorNS::FATAL_ERROR,
		                "Error creating Direct3D device"));
}

void Graphics::ReleaseAll()
{
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}


HRESULT Graphics::ShowBackbuffer()
{
	result = E_FAIL;
	device3d->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 0.0f,
	                0);

	result = device3d->Present(nullptr, nullptr, nullptr, nullptr);

	return result;
}
