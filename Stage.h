#pragma once
#include "Engine/GameObject.h"
#include <d3d11.h>

class Transform;

struct CONSTBUFFER_STAGE
{
	XMFLOAT4 lightPosition; //�����ʒu
	XMFLOAT4 eyePosition; //���_�ʒu
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
	ID3D11Buffer* pConstantBuffer_; //�R���X�^���g�o�b�t�@
	void InitConstantBuffer();
public:
	Stage(GameObject* parent);
	~Stage() {};
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};