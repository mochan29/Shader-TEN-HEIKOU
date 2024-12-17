#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

/// <summary>
///�`��S��
/// </summary>

//�V�F�[�_��؂�ւ��邽�߂̂��
enum SHADER_TYPE
{
	SHADER_2D,
	SHADER_3D,
	SHADER_POINT,
	SHADER_MAX
};

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

namespace Direct3D
{
	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;
	extern XMINT2 WindowSize;
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//simple2D.hlsl�̏�����
	HRESULT InitShader2D();
	//simple3d.hlsl�̏�����
	HRESULT InitShader3D();
	//pointlightShader.hlsl�̏�����
	HRESULT InitPointShader();

	//�V�F�[�_�[����
	HRESULT InitShader();
	void SetShader(SHADER_TYPE type);

	void BeginDraw();
	void EndDraw();
	void Release();

	void SetGlobalLightVec(XMFLOAT4 lv);
	XMFLOAT4 GetGlobalLightVec();
	XMFLOAT4 GetLightPos();
	void SetLightPos(XMFLOAT4 p);
};

