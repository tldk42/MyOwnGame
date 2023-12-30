#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#pragma region 헤더
#include <d3d9.h>
#include "Data/Constants.h"
#include "Error/GameError.h"
#pragma endregion

#pragma region DirectX 포인터 타입
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9
#pragma endregion

#pragma region 색상 타입
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
    ((COLOR_ARGB)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#pragma endregion

class Graphics
{
public:
	Graphics();

	virtual ~Graphics();

	// Releases direct3d and device3d.
	void ReleaseAll();

	/**
	 * \brief Direct3D 초기화 및 Direct3D디바이스 생성
	 * \param hw CreateWindow에서 반환된 윈도우 디바이스 해늘
	 * \param inWidth 디스플레이 버퍼 너비
	 * \param inHeight 디스플레이 버퍼 높이
	 * \param inFullscreen 전체화면
	 */
	void Initialize(HWND hw, int inWidth, int inHeight, bool inFullscreen);

	/* 백 버퍼로 교체*/
	HRESULT ShowBackbuffer();

private:
	/**
	 * \brief D3D 프레젠테이션 매개변수를 초기화
	 * (엔진 내부에서만 사용)
	 */
	void InitD3Dpp();

private:
#pragma region DirectX 포인터 및 관련 변수
	LP_3D direct3d;
	LP_3DDEVICE device3d;
	D3DPRESENT_PARAMETERS d3dpp{};
#pragma endregion

#pragma region 다른 변수들
	HRESULT result{};
	HWND hwnd{};
	bool fullscreen;
	int width;
	int height;
#pragma endregion
};
