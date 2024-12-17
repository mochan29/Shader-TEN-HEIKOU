#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

/// <summary>
/// ポリゴンの表示に関するクラス
/// </summary>

using namespace DirectX;

class Quad
{
private:
	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //法線ベクトル
	};
	//コンスタントバッファー アプリ側からシェーダーに毎フレーム渡したい情報
	struct CONSTANT_BUFFER
	{
		XMMATRIX matWVP;
		XMMATRIX matNormal;
		XMINT2 winSize;
	};

public:
	Quad();
	virtual ~Quad();
	HRESULT Initialize();
	void Draw(Transform& transform);
	void Release();

protected:
	
	ID3D11Buffer* pVertexBuffer_; //頂点バッファ
	ID3D11Buffer* pIndexBuffer_; //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_; //テクスチャー
	int indexNum_;
	std::vector<VERTEX> vertices_;
	int vertexNum_;
	std::vector<int> index_;
	virtual void InitVertexData();
	HRESULT CreateVertexBuffer();
	virtual void InitIndexData();
	HRESULT CreateIndexBuffer();
	HRESULT CreateConstantBuffer();
	HRESULT LoadTexture();
	void PassDataToCB(Transform& transform); //コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline(); //各バッファをパイプラインにセット


};

