#include "Stage.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Camera.h"

Stage::Stage(GameObject* parent)
	:GameObject(parent, "Stage"), pConstantBuffer_(nullptr)
{
	hModel_ = -1;
	doModel_[0] = -1;
	itaModel_ = -1;
}

void Stage::Initialize()
{
	hModel_ = Model::Load("Assets/Models/player.fbx");
	doModel_[0] = Model::Load("Assets/Models/donuts1.fbx");
	doModel_[1] = Model::Load("Assets/Models/donuts2.fbx");
	doModel_[2] = Model::Load("Assets/Models/donuts3.fbx");
	doModel_[3] = Model::Load("Assets/Models/donuts4.fbx");
	itaModel_ = Model::Load("Assets/Models/ita.fbx");

	for (int i = 0; i < 4; i++)
	{
		do_[i].scale_ = { 0.5f,0.5f,0.5f };
		do_[i].position_.x =-5+(10/4)*(i+1);
	}
	ita_.position_.y = -3.5;
	ita_.scale_ = { 2.0f,2.0f,2.0f };

	Camera::SetPosition({ 0.0f,3.0f,-10.0f });
	Camera::SetTarget({ 0.0f,0.8f,0.0f });
	InitConstantBuffer();
}

void Stage::Update()
{
	if (Input::IsKey(DIK_A))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x - 0.1f,p.y,p.z,p.w };
		Direct3D::SetLightPos(p);

	}
	if (Input::IsKey(DIK_D))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x + 0.1f,p.y,p.z,p.w };
		Direct3D::SetLightPos(p);

	}
	if (Input::IsKey(DIK_W))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x ,p.y,p.z + 0.1f,p.w };
		Direct3D::SetLightPos(p);

	}
	if (Input::IsKey(DIK_S))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x ,p.y,p.z - 0.1f,p.w };
		Direct3D::SetLightPos(p);

	}
	if (Input::IsKey(DIK_UP))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x,p.y + 0.1f,p.z,p.w };
		Direct3D::SetLightPos(p);

	}
	if (Input::IsKey(DIK_DOWN))
	{
		XMFLOAT4 p = Direct3D::GetLightPos();
		p = { p.x,p.y - 0.1f,p.z,p.w };
		Direct3D::SetLightPos(p);
	}

	CONSTBUFFER_STAGE cb;
	cb.lightPosition = Direct3D::GetLightPos();
	XMStoreFloat4(&cb.eyePosition, Camera::GetPosition());

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata); //GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb)); //データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開
	Direct3D::pContext->VSSetConstantBuffers(1, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(1, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	for (int i = 0; i < 4; i++)
	{
		do_[i].rotate_.y += 1.0f;
	}
}

void Stage::Draw()
{
	tr_.position_ = { Direct3D::GetLightPos().x,Direct3D::GetLightPos().y ,Direct3D::GetLightPos().z };
	Model::SetTransform(hModel_, tr_);
	Model::Draw(hModel_);
	for (int i = 0; i < 4; i++)
	{
		Model::SetTransform(doModel_[i], do_[i]);
		Model::Draw(doModel_[i]);
	}
	Model::SetTransform(itaModel_, ita_);
	Model::Draw(itaModel_);
}

void Stage::InitConstantBuffer()
{
	//Quadと一緒
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTBUFFER_STAGE);
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
		MessageBox(NULL, L"コンスタントバッファの作成に失敗", L"Stage.cpp", MB_OK);
	}
}

void Stage::Release()
{
}