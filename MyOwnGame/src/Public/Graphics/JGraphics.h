#pragma once

#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#pragma region ���
#include <d3d9.h>
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

class JGraphics
{
public:
	JGraphics();

	virtual ~JGraphics();

	void ReleaseAll();

	/**
	 * \brief Direct3D �ʱ�ȭ �� Direct3D����̽� ����
	 * \param hw CreateWindow���� ��ȯ�� ������ ����̽� �ش�
	 * \param inWidth ���÷��� ���� �ʺ�
	 * \param inHeight ���÷��� ���� ����
	 * \param inFullscreen ��üȭ��
	 */
	void Initialize(HWND hw, int inWidth, int inHeight, bool inFullscreen);

	/* �׷��� ����̽� ����*/
	HRESULT Reset();

	/* �� ���۷� ��ü*/
	HRESULT ShowBackbuffer();

	/**
	 * \brief ����Ͱ� d3dpp�� ������ �� ������ ����, �ʺ�, �������� �ӵ��� ȣȯ�Ǵ��� Ȯ��.
	 * \return 
	 */
	bool IsAdapterCompatible();

	/* ����̽��� Lost �������� Ȯ�� */
	HRESULT GetDeviceState() const;

	/* Clear �� �� �ʱ�ȭ �� ���� */
	void SetBackColor(const COLOR_ARGB NewColor)
	{
		BackColor = NewColor;
	}

	HRESULT BeginScene() const;
	HRESULT EndScene() const;

private:
	/**
	 * \brief D3D ���������̼� �Ű������� �ʱ�ȭ
	 * (���� ���ο����� ���)
	 */
	void InitD3Dpp();

private:
#pragma region DirectX ������ �� ���� ����
	LP_3D Direct3D;
	LP_3DDEVICE Device3D;
	D3DPRESENT_PARAMETERS D3DPP{};
	D3DDISPLAYMODE pMode;
#pragma endregion

#pragma region �ٸ� ������
	HRESULT Result{};
	HWND Hwnd{};
	bool Fullscreen;
	int Width;
	int Height;
	COLOR_ARGB BackColor;
#pragma endregion
};
