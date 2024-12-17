#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

/// <summary>
///描画全般
/// </summary>

//シェーダを切り替えるためのやつ
enum SHADER_TYPE
{
	SHADER_2D,
	SHADER_3D,
	SHADER_POINT,
	SHADER_MAX
};

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

namespace Direct3D
{
	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;
	extern XMINT2 WindowSize;
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//simple2D.hlslの初期化
	HRESULT InitShader2D();
	//simple3d.hlslの初期化
	HRESULT InitShader3D();
	//pointlightShader.hlslの初期化
	HRESULT InitPointShader();

	//シェーダー準備
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

