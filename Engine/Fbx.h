#pragma once

#include <d3d11.h>
#include <fbxsdk.h>
#include <string>
#include "Transform.h"
#include "Texture.h"
#include <vector>

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

/// <summary>
/// Fbxに関するクラス
/// </summary>

struct RayCastData
{
	XMFLOAT3 start; //レイの発射位置
	XMFLOAT3 dir; //向き
	bool hit; //当たったかどうか
	float dist; //距離入れるやつ
};

class Fbx
{
	//頂点情報
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	//コンスタントバッファー アプリ側からシェーダーに毎フレーム渡したい情報  こことシェーダーの順番は同じにして
	struct CONSTANT_BUFFER_MODEL
	{
		XMMATRIX matWVP; //スクリーン座標変換マトリクス
		XMMATRIX matW; //ワールド座標変換マトリクス
		XMMATRIX matNormal; //法線ベクトルのワールド座標変換用
		XMFLOAT4 diffuseColor; //マテリアルの色(拡散反射係数)
		XMFLOAT4 lightPosition; //光源の位置
		XMFLOAT4 diffuseFactor; //マテリアルの光の強度 拡散光
		XMFLOAT4 ambientColor;
		XMFLOAT4 specularColor;
		XMFLOAT4 shininess;
		int isTextured; //テクスチャが貼られているか

	};

	//マテリアル こことシェーダーの順番は違くてもOK
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse; //ベクトル
		XMFLOAT4 specular; //鏡面反射係数 ベクトル
		XMFLOAT4 shininess; //鏡面反射のパラメータ スカラ
		XMFLOAT4 ambient; //ベクトル
		XMFLOAT4 factor; //スカラ
	};

	int vertexCount_; //頂点数
	int polygonCount_; //ポリゴン数
	int materialCount_; //マテリアルの個数
	std::vector<MATERIAL> pMaterialList_; //マテリアルのリスト
	std::vector<int> indexCount_; //インデックスの個数

	std::vector<VERTEX> vertices_; //頂点情報を入れる配列
	ID3D11Buffer* pVertexBuffer_; //頂点バッファ
	std::vector<int>* index_; //インデックス情報を入れる配列
	ID3D11Buffer** pIndexBuffer_; //インデックスバッファ
	ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ
	void InitVertex(fbxsdk::FbxMesh* mesh); //頂点バッファ準備
	void InitIndex(fbxsdk::FbxMesh* mesh); //インデックスバッファ準備
	void InitConstantBuffer(); //コンスタントバッファ準備
	void InitMaterial(fbxsdk::FbxNode* pNode); //マテリアル準備
	bool isPoint; //ポイントシェーダーですか
public:
	Fbx();
	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void Release();
	void RayCast(RayCastData& rayData, Transform& transform); //レイキャスト用
};