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
/// Fbx�Ɋւ���N���X
/// </summary>

struct RayCastData
{
	XMFLOAT3 start; //���C�̔��ˈʒu
	XMFLOAT3 dir; //����
	bool hit; //�����������ǂ���
	float dist; //�����������
};

class Fbx
{
	//���_���
	struct VERTEX
	{
		XMVECTOR position;
		XMVECTOR uv;
		XMVECTOR normal;
	};

	//�R���X�^���g�o�b�t�@�[ �A�v��������V�F�[�_�[�ɖ��t���[���n���������  �����ƃV�F�[�_�[�̏��Ԃ͓����ɂ���
	struct CONSTANT_BUFFER_MODEL
	{
		XMMATRIX matWVP; //�X�N���[�����W�ϊ��}�g���N�X
		XMMATRIX matW; //���[���h���W�ϊ��}�g���N�X
		XMMATRIX matNormal; //�@���x�N�g���̃��[���h���W�ϊ��p
		XMFLOAT4 diffuseColor; //�}�e���A���̐F(�g�U���ˌW��)
		XMFLOAT4 lightPosition; //�����̈ʒu
		XMFLOAT4 diffuseFactor; //�}�e���A���̌��̋��x �g�U��
		XMFLOAT4 ambientColor;
		XMFLOAT4 specularColor;
		XMFLOAT4 shininess;
		int isTextured; //�e�N�X�`�����\���Ă��邩

	};

	//�}�e���A�� �����ƃV�F�[�_�[�̏��Ԃ͈Ⴍ�Ă�OK
	struct MATERIAL
	{
		Texture* pTexture;
		XMFLOAT4 diffuse; //�x�N�g��
		XMFLOAT4 specular; //���ʔ��ˌW�� �x�N�g��
		XMFLOAT4 shininess; //���ʔ��˂̃p�����[�^ �X�J��
		XMFLOAT4 ambient; //�x�N�g��
		XMFLOAT4 factor; //�X�J��
	};

	int vertexCount_; //���_��
	int polygonCount_; //�|���S����
	int materialCount_; //�}�e���A���̌�
	std::vector<MATERIAL> pMaterialList_; //�}�e���A���̃��X�g
	std::vector<int> indexCount_; //�C���f�b�N�X�̌�

	std::vector<VERTEX> vertices_; //���_��������z��
	ID3D11Buffer* pVertexBuffer_; //���_�o�b�t�@
	std::vector<int>* index_; //�C���f�b�N�X��������z��
	ID3D11Buffer** pIndexBuffer_; //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_; //�R���X�^���g�o�b�t�@
	void InitVertex(fbxsdk::FbxMesh* mesh); //���_�o�b�t�@����
	void InitIndex(fbxsdk::FbxMesh* mesh); //�C���f�b�N�X�o�b�t�@����
	void InitConstantBuffer(); //�R���X�^���g�o�b�t�@����
	void InitMaterial(fbxsdk::FbxNode* pNode); //�}�e���A������
	bool isPoint; //�|�C���g�V�F�[�_�[�ł���
public:
	Fbx();
	HRESULT Load(std::string fileName);
	void Draw(Transform& transform);
	void Release();
	void RayCast(RayCastData& rayData, Transform& transform); //���C�L���X�g�p
};