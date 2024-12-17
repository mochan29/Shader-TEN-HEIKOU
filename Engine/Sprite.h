#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include <vector>
#include "Transform.h"

/// <summary>
/// �X�v���C�g�̕\���Ɋւ���N���X
/// </summary>

using std::vector;
using namespace DirectX;

class Sprite	
{
	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
	};
	//�R���X�^���g�o�b�t�@�[ �A�v��������V�F�[�_�[�ɖ��t���[���n���������
	struct CONSTANT_BUFFER
	{
		XMMATRIX matW;
	};

	uint64_t vertexNum_; //���_��
	vector<VERTEX> vertices_; //���_���
	vector<int> indices_; //�C���f�b�N�X���
	ID3D11Buffer* pVertexBuffer_; //���_�o�b�t�@
	uint64_t indexNum_; //�C���f�b�N�X��	
	ID3D11Buffer* pIndexBuffer_; //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_; //�e�N�X�`���[�ւ̃|�C���^


	//Initialize����Ă΂��֐�
	void InitVertexData(); //���_��񏀔�
	HRESULT CreateVertexBuffer(); //���_�o�b�t�@�쐬
	void InitIndexData(); //�C���f�b�N�X��񏀔�
	HRESULT CreateIndexBuffer(); //�C���f�b�N�X�o�b�t�@�쐬
	HRESULT CreateConstantBuffer(); //�R���X�^���g�o�b�t�@�쐬
	HRESULT LoadTexture(std::string fileName); //�e�N�X�`���̃��[�h
	//Draw����Ă΂��֐�
	void PassDataToCB(DirectX::XMMATRIX worldMatrix); //�R���X�^���g�o�b�t�@�Ɋe�����n��
	void SetBufferToPipeline(); //�e�o�b�t�@���p�C�v���C���ɃZ�b�g

public:
	Sprite();
	~Sprite() {};
	HRESULT Load(std::string fileName); //�������p
	void Draw(Transform& transform);
	void Release();
};

