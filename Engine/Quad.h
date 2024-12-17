#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

/// <summary>
/// �|���S���̕\���Ɋւ���N���X
/// </summary>

using namespace DirectX;

class Quad
{
private:
	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal; //�@���x�N�g��
	};
	//�R���X�^���g�o�b�t�@�[ �A�v��������V�F�[�_�[�ɖ��t���[���n���������
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
	
	ID3D11Buffer* pVertexBuffer_; //���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_; //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_; //�e�N�X�`���[
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
	void PassDataToCB(Transform& transform); //�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline(); //�e�o�b�t�@���p�C�v���C���ɃZ�b�g


};

