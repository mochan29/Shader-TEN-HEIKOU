#include "Fbx.h"
#include "Camera.h"
#include <filesystem>
#include <DirectXCollision.h>
#include "Define.h"
#include "Debug.h"
#include "Input.h"

namespace fs = std::filesystem;

Fbx::Fbx()
{
	vertexCount_ = -1;
	polygonCount_ = -1;
	materialCount_ = -1;

	index_ = nullptr;

	pVertexBuffer_ = nullptr;
	pIndexBuffer_ = nullptr;
	pConstantBuffer_ = nullptr;
	isPoint = true;
}

//ロードしていろいろ初期化
HRESULT Fbx::Load(std::string fileName)
{
	//マネージャを生成
	FbxManager* pFbxManager = FbxManager::Create();

	//インポーターを生成
	FbxImporter* fbxImporter = FbxImporter::Create(pFbxManager, "imp");
	fbxImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

	//シーンオブジェクトにFbxファイルの情報を流し込む
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "fbxscene");
	fbxImporter->Import(pFbxScene);
	fbxImporter->Destroy();

	//メッシュ情報を取得
	FbxNode* rootNode = pFbxScene->GetRootNode();
	FbxNode* pNode = rootNode->GetChild(0);
	FbxMesh* mesh = pNode->GetMesh();

	//各情報の個数を取得
	vertexCount_ = mesh->GetControlPointsCount();
	polygonCount_ = mesh->GetPolygonCount();
	materialCount_ = pNode->GetMaterialCount();

	//パスを作る
	fs::path cPath, basePath;
	cPath = fs::current_path();
	basePath = cPath;
	string subDir("Assets\\Models");
	fs::path subPath(cPath.string() + "\\" + subDir);
	assert(fs::exists(subPath));
	fs::current_path(subPath);

	InitVertex(mesh);
	InitIndex(mesh);
	InitConstantBuffer();
	InitMaterial(pNode);

	//カレントパスを戻す
	fs::current_path(basePath);

	//マネージャ解放
	pFbxManager->Destroy();
	return S_OK;
}

void Fbx::InitVertex(fbxsdk::FbxMesh* mesh)
{
	//頂点情報を入れる配列の初期化
	vertices_ = std::vector<VERTEX>(vertexCount_);

	//全ポリゴン
	for (DWORD poly = 0; poly < (DWORD)polygonCount_; poly++)
	{
		//3頂点分
		for (int vertex = 0; vertex < 3; vertex++)
		{
			//調べる頂点の番号
			int index = mesh->GetPolygonVertex(poly, vertex);

			//頂点の位置
			FbxVector4 pos = mesh->GetControlPointAt(index);
			vertices_[index].position = XMVectorSet((float)pos[0], (float)pos[1], -(float)pos[2], 0.0f);

			//頂点に割り当てられたUV
			FbxLayerElementUV* pUV = mesh->GetLayer(0)->GetUVs();
			int uvIndex = mesh->GetTextureUVIndex(poly, vertex, FbxLayerElement::eTextureDiffuse);
			FbxVector2  uv = pUV->GetDirectArray().GetAt(uvIndex);
			vertices_[index].uv = XMVectorSet((float)uv.mData[0], (float)(1.0f - uv.mData[1]), 0.0f, 0.0f);

			//頂点の法線
			FbxLayerElementNormal* leNormal = mesh->GetLayer(0)->GetNormals();
			FbxLayerElement::EMappingMode mp = leNormal->GetMappingMode();
			//FbxVector4 Normal;
			//mesh->GetPolygonVertexNormal(poly, vertex, Normal);	//i番目のポリゴンの,j番目の頂点の法線をゲット
			FbxVector4 Normal = leNormal->GetDirectArray().GetAt(index);
			vertices_[index].normal = XMVectorSet((float)Normal[0], (float)Normal[1], -(float)Normal[2], 0.0f);
		}
	}
	//頂点バッファ作成
	HRESULT hr;
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(VERTEX) * vertexCount_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices_.data();
	hr = Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"頂点バッファの作成に失敗", L"Fbx.cpp", MB_OK);
	}
}

void Fbx::InitIndex(fbxsdk::FbxMesh* mesh)
{
	pIndexBuffer_ = new ID3D11Buffer * [materialCount_];
	indexCount_ = std::vector<int>(materialCount_);
	index_ = new std::vector<int>[materialCount_];

	for (int i = 0; i < materialCount_; i++)
	{
		index_[i] = std::vector<int>(polygonCount_ * 3);
		int count = 0;
		//全ポリゴン
		for (DWORD poly = 0; poly < (DWORD)polygonCount_; poly++)
		{
			FbxLayerElementMaterial* mtl = mesh->GetLayer(0)->GetMaterials();
			int mtlId = mtl->GetIndexArray().GetAt(poly);
			if (mtlId == i)
			{
				//3頂点分
				for (DWORD vertex = 0; vertex < 3; vertex++)
				{
					index_[i][count] = mesh->GetPolygonVertex(poly, 2 - vertex);
					count++;
				}
			}
		}
		indexCount_[i] = count;

		D3D11_BUFFER_DESC   bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * polygonCount_ * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = index_[i].data();
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT hr;
		hr = Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_[i]);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"インデックスバッファの作成に失敗", L"Fbx.cpp", MB_OK);
		}
	}
}

void Fbx::InitConstantBuffer()
{
	//Quadと一緒
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER_MODEL);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	HRESULT hr;
	hr = Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"コンスタントバッファの作成に失敗", L"Fbx.cpp", MB_OK);
	}
}

void Fbx::InitMaterial(fbxsdk::FbxNode* pNode)
{
	pMaterialList_ = std::vector<MATERIAL>(materialCount_);
	for (int i = 0; i < materialCount_; i++)
	{
		//i番目のマテリアル情報を取得
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		//テクスチャ情報
		FbxProperty  lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

		//テクスチャの数数
		int fileTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();

		//テクスチャあり
		if (fileTextureCount)
		{
			FbxFileTexture* textureInfo = lProperty.GetSrcObject<FbxFileTexture>(0);
			const char* textureFilePath = textureInfo->GetRelativeFileName();

			//パスを作る
			fs::path texFile(textureFilePath);

			//ファイルからテクスチャ作成
			pMaterialList_[i].pTexture = new Texture;
			HRESULT hr = pMaterialList_[i].pTexture->Load(texFile.string());
			if (FAILED(hr))
			{
				MessageBox(NULL, L"テクスチャの作成に失敗", L"Fbx.cpp", MB_OK);
			}

			//Fbxからマテリアルの情報をとってくる
			FbxSurfacePhong* pMaterial = (FbxSurfacePhong*)pNode->GetMaterial(i);
			FbxDouble diffuse = pMaterial->DiffuseFactor;
			FbxDouble3 ambient = pMaterial->Ambient;
			pMaterialList_[i].factor = { (float)diffuse, (float)diffuse, (float)diffuse, (float)diffuse };
			pMaterialList_[i].ambient = { (float)ambient[0],(float)ambient[1],(float)ambient[2],1.0f };

			//フォンのモデルかどうか確認
			if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxDouble3 specular = pMaterial->Specular;
				FbxDouble shininess = pMaterial->Shininess; //4つとも同じ値でセット
				//自分のpMaterialList[i]に値を設定
				pMaterialList_[i].specular = { (float)specular[0],(float)specular[1],(float)specular[2],1.0f };
				pMaterialList_[i].shininess = { (float)shininess,(float)shininess,(float)shininess,1.0f };
			}
			else
			{
				pMaterialList_[i].specular = { 0.0f,0.0f,0.0f,1.0f };
				pMaterialList_[i].shininess = {10.0f,10.0f,10.0f,1.0f };
			}
			

		}
		//テクスチャ無し
		else
		{
			pMaterialList_[i].pTexture = nullptr;
			//Fbxからマテリアルの情報をとってくる
			FbxSurfacePhong* pMaterial = (FbxSurfacePhong*)pNode->GetMaterial(i);
			FbxDouble3  diffuse = pMaterial->Diffuse;

			pMaterialList_[i].diffuse = XMFLOAT4((float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f);
			FbxDouble factor = pMaterial->DiffuseFactor;
			pMaterialList_[i].factor = XMFLOAT4((float)factor, (float)factor, (float)factor, (float)factor);
			FbxDouble3 ambient = pMaterial->Ambient;
			pMaterialList_[i].ambient = { (float)ambient[0],(float)ambient[1],(float)ambient[2],1.0f };
			//フォンのモデルかどうか確認
			if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
			{
				FbxDouble3 specular = pMaterial->Specular;
				FbxDouble shininess = pMaterial->Shininess; //4つとも同じ値でセット
				
				//自分のpMaterialList[i]に値を設定
				pMaterialList_[i].specular = { (float)specular[0],(float)specular[1],(float)specular[2],1.0f };
				pMaterialList_[i].shininess= { (float)shininess,(float)shininess,(float)shininess,1.0f };
			}
			else
			{
				pMaterialList_[i].specular = { 1.0f,1.0f,1.0f,1.0f };
				pMaterialList_[i].shininess = { 10.0f,10.0f,10.0f,1.0f };
			}
			

		}
	}
}

void Fbx::Draw(Transform& transform)
{
	Direct3D::SetShader(SHADER_POINT);
	transform.Calculation();
	if (Input::IsKeyDown(DIK_P))
	{
		isPoint = !isPoint;
	}

	if (isPoint)
	{
		Direct3D::SetShader(SHADER_POINT);
	}
	else
	{
		Direct3D::SetShader(SHADER_3D);
	}

	//インデックスバッファーをセット
	for (int i = 0; i < materialCount_; i++)
	{
		CONSTANT_BUFFER_MODEL cb;
		cb.matWVP = XMMatrixTranspose(transform.GetWorldMatrix() * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
		cb.matW = XMMatrixTranspose(transform.GetWorldMatrix());
		cb.matNormal = XMMatrixTranspose(transform.GetNormalMatrix());
		cb.ambientColor = pMaterialList_[i].ambient;
		cb.specularColor = pMaterialList_[i].specular;
		cb.shininess = pMaterialList_[i].shininess;
		cb.diffuseColor = pMaterialList_[i].diffuse;
		cb.lightPosition = Direct3D::GetLightPos();
		cb.diffuseFactor = pMaterialList_[i].factor;

		if (pMaterialList_[i].pTexture == nullptr)
		{
			cb.isTextured = false;
		}
		else
		{
			cb.isTextured = true;
		}

		D3D11_MAPPED_SUBRESOURCE pdata;
		Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); //GPUからのデータアクセスを止める
		memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb)); //データを値を送る

		Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

		//頂点バッファ,インデックスバッファ,コンスタントバッファをパイプラインにセット
		//頂点バッファ
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

		stride = sizeof(int);
		offset = 0;
		Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);

		//コンスタントバッファ
		Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
		Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

		if (!(pMaterialList_[i].pTexture == nullptr))
		{
			//サンプラーとシェーダーリソースビューをシェーダにセット
			ID3D11SamplerState* pSampler = pMaterialList_[i].pTexture->GetSampler();
			Direct3D::pContext->PSSetSamplers(0, 1, &pSampler);

			ID3D11ShaderResourceView* pSRV = pMaterialList_[i].pTexture->GetSRV();
			Direct3D::pContext->PSSetShaderResources(0, 1, &pSRV);

		}
		//描画
		Direct3D::pContext->DrawIndexed(indexCount_[i], 0, 0);
	}
}

void Fbx::Release()
{
	SAFE_RELEASE(pConstantBuffer_);
	for (int i = 0; i < materialCount_; i++)
	{
		SAFE_RELEASE(pIndexBuffer_[i]); //中身消す
	}
	SAFE_DELETE_ARRAY(pIndexBuffer_); //本体消す
	SAFE_RELEASE(pVertexBuffer_);
}

void Fbx::RayCast(RayCastData& rayData, Transform& transform)
{
	//ワールド行列の逆行列を使ってレイをモデルと逆方向に動かす->そのほうが軽い
	transform.Calculation();
	XMMATRIX invWorld = XMMatrixInverse(nullptr, transform.GetWorldMatrix());

	XMVECTOR start = XMLoadFloat3(&rayData.start); //レイの発射位置
	XMVECTOR dir = XMVector3Normalize(XMLoadFloat3(&rayData.dir)); //向き(正規化済み)

	//ローカル座標のままなのでワールド行列をかけてワールド座標に
	//レイを動かしたときの新たな向きを決める
	XMVECTOR end = start + dir;
	start = XMVector3TransformCoord(start, invWorld);
	end = XMVector3TransformCoord(end, invWorld);
	dir = end - start; //モデルが回転したときにはdirが変わる

	//マテリアル毎
	for (int material = 0; material < materialCount_; material++)
	{
		//ポリゴン毎 頂点情報の先頭が入っている
		for (int poly = 0; poly < indexCount_[material] / 3; poly++)
		{
			XMVECTOR v0 = vertices_[index_[material][poly * 3 + 0]].position; //3頂点 poly*3=ポリゴン情報の先頭を指す 3つで1セット
			XMVECTOR v1 = vertices_[index_[material][poly * 3 + 1]].position;
			XMVECTOR v2 = vertices_[index_[material][poly * 3 + 2]].position;

			rayData.hit = TriangleTests::Intersects(start, dir, v0, v1, v2, rayData.dist); //レイの発射位置、向き、3頂点、距離入れるやつ
			if (rayData.hit)
			{
				return;
			}
		}
	}
}