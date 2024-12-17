#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

/// <summary>
/// スプライトの表示に関するクラス
/// </summary>

using std::vector;
using namespace DirectX;

class Sprite	
{
	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};
	//コンスタントバッファー アプリ側からシェーダーに毎フレーム渡したい情報
	struct CONSTANT_BUFFER
	{
		XMMATRIX matW;
	};

	uint64_t vertexNum_; //頂点数
	vector<VERTEX> vertices_; //頂点情報
	vector<int> indices_; //インデックス情報
	ID3D11Buffer* pVertexBuffer_; //頂点バッファ
	uint64_t indexNum_; //インデックス数	
	ID3D11Buffer* pIndexBuffer_; //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_;	//コンスタントバッファ
	Texture* pTexture_; //テクスチャーへのポインタ


	//Initializeから呼ばれる関数
	void InitVertexData(); //頂点情報準備
	HRESULT CreateVertexBuffer(); //頂点バッファ作成
	void InitIndexData(); //インデックス情報準備
	HRESULT CreateIndexBuffer(); //インデックスバッファ作成
	HRESULT CreateConstantBuffer(); //コンスタントバッファ作成
	HRESULT LoadTexture(std::string fileName); //テクスチャのロード
	//Drawから呼ばれる関数
	void PassDataToCB(DirectX::XMMATRIX worldMatrix); //コンスタントバッファに各種情報を渡す
	void SetBufferToPipeline(); //各バッファをパイプラインにセット

public:
	Sprite();
	~Sprite() {};
	HRESULT Load(std::string fileName); //初期化用
	void Draw(Transform& transform);
	void Release();
};

