#pragma once
#include "Engine/GameObject.h"
#include <d3d11.h>

class Transform;

struct CONSTBUFFER_STAGE
{
	XMFLOAT4 lightPosition; //光源位置
	XMFLOAT4 eyePosition; //視点位置
};

class Stage :
	public GameObject
{
	int hModel_;
	int doModel_[4];
	int itaModel_;
	Transform tr_;
	Transform do_[4];
	Transform ita_;
	ID3D11Buffer* pConstantBuffer_; //コンスタントバッファ
	void InitConstantBuffer();
public:
	Stage(GameObject* parent);
	~Stage() {};
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};