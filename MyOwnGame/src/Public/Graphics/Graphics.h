#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#pragma region ���
#include <d3d9.h>
#include "Data/Constants.h"
#include "Error/GameError.h"
#pragma endregion

#pragma region DirectX ������ Ÿ��
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D       LPDIRECT3D9
#pragma endregion

#pragma region ���� Ÿ��
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
	 * \brief Direct3D �ʱ�ȭ �� Direct3D����̽� ����
	 * \param hw CreateWindow���� ��ȯ�� ������ ����̽� �ش�
	 * \param inWidth ���÷��� ���� �ʺ�
	 * \param inHeight ���÷��� ���� ����
	 * \param inFullscreen ��üȭ��
	 */
	void Initialize(HWND hw, int inWidth, int inHeight, bool inFullscreen);

	/* �� ���۷� ��ü*/
	HRESULT ShowBackbuffer();

private:
	/**
	 * \brief D3D ���������̼� �Ű������� �ʱ�ȭ
	 * (���� ���ο����� ���)
	 */
	void InitD3Dpp();

private:
#pragma region DirectX ������ �� ���� ����
	LP_3D direct3d;
	LP_3DDEVICE device3d;
	D3DPRESENT_PARAMETERS d3dpp{};
#pragma endregion

#pragma region �ٸ� ������
	HRESULT result{};
	HWND hwnd{};
	bool fullscreen;
	int width;
	int height;
#pragma endregion
};
