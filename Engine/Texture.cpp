#include "Texture.h"
#include <wincodec.h>
#include "DirectXTex.h"
#include "Define.h"

//リンカ
#pragma comment(lib, "DirectXTex.lib")

using namespace DirectX;

Texture::Texture()
{
	pSampler_ = nullptr;
	pSRV_ = nullptr;
}

Texture::~Texture()
{
}

HRESULT Texture::Load(std::string fileName)
{
	HRESULT hr;

	TexMetadata metadata; //画像の付属情報
	ScratchImage image; //画像本体

	//ファイルを読む
	std::wstring wstr(fileName.begin(), fileName.end()); //string->wchar_t* の変換 LPCWSTR=const wchar_t*
	hr = LoadFromWICFile(wstr.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, &metadata, image);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"失敗: テクスチャファイルの読み込み", L"Texture.cpp", MB_OK);
		return hr;
	}

	//サンプラー作成
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; //LINEAR: 滑らかにぼやける POINT: マイクラぽい粗さ
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"失敗: サンプラー作成", L"Texture.cpp", MB_OK);
		return hr;
	}

	//シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	hr = CreateShaderResourceView(Direct3D::pDevice, image.GetImages(), image.GetImageCount(), metadata, &pSRV_);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"失敗: シェーダーリソースビュー", L"Texture.cpp", MB_OK);
		return hr;
	}

	return hr;
}

void Texture::Release()
{
	SAFE_RELEASE(pSampler_);
	SAFE_RELEASE(pSRV_);
}