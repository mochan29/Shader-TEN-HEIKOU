#pragma once
#include <d3d11.h>
#include <string>
#include <wrl.h>
#include "Direct3D.h"

/// <summary>
/// テクスチャに関するクラス
/// </summary>

using namespace Microsoft::WRL;
using std::string;

class Texture
{
	ID3D11SamplerState* pSampler_; //サンプラー(テクスチャの張り方)
	ID3D11ShaderResourceView* pSRV_; //シェーダーリソースビュー

public:
	Texture();
	~Texture();

	/// <summary>
	/// 画像をロード
	/// </summary>
	/// <param name="fileName">画像ファイル名</param>
	/// <returns>成功/失敗</returns>
	HRESULT Load(string fileName);
	void Release();

	/// <summary>
	/// サンプラーの取得
	/// </summary>
	/// <returns>サンプラー</returns>
	ID3D11SamplerState* GetSampler() { return pSampler_; };

	/// <summary>
	/// シェーダーリソースビューの取得
	/// </summary>
	/// <returns>シェーダーリソースビュー</returns>
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; };
};