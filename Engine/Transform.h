#pragma once
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// �ʒu�A�����A�g�嗦�Ȃǂ��Ǘ�����N���X
/// </summary>
 
class Transform
{
	XMMATRIX matTranslate_; //�ړ��s��
	XMMATRIX matRotate_; //��]�s��	
	XMMATRIX matScale_; //�g��s��

public:
	XMFLOAT3 position_;	//�ʒu
	XMFLOAT3 rotate_;	//����
	XMFLOAT3 scale_;	//�g�嗦
	Transform* pParent_;
	Transform();
	~Transform();

	/// <summary>
	/// �e�s��̌v�Z
	/// </summary>
	void Calculation();

	/// <summary>
	/// ���[���h�s����擾
	/// </summary>
	/// <returns>���̎��_�ł̃��[���h�s��</returns>
	XMMATRIX GetWorldMatrix();

	/// <summary>
	/// �Ղ낶���������H�H�H�s����擾
	/// </summary>
	/// <returns>���̎��_�ł̃v���W�F�N�V�����H�s��</returns>
	XMMATRIX GetNormalMatrix();
};