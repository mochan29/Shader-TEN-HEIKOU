#pragma once
#include <string>
#include <list>
#include "Transform.h"

class SphereCollider;

/// <summary>
/// �p���������̃C���^�[�t�F�[�X�N���X
/// </summary>

class GameObject
{
	bool isDead_; //��������

protected:
	std::list<GameObject*> childList_;
	Transform transform_;
	GameObject* pParent_;
	std::string	objectName_;
	SphereCollider* pCollider_;
public:

	GameObject();
	GameObject(GameObject* parent, const std::string& name);
	~GameObject();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// ���
	/// </summary>
	virtual void Release() = 0;

	/// <summary>
	/// �`���C���h���X�g���̎q�N���X�̍X�V
	/// </summary>
	void UpdateSub();
	/// <summary>
	/// �`���C���h���X�g���̎q�N���X�̕`��
	/// </summary>
	void DrawSub();
	/// <summary>
	/// �`���C���h���X�g���̎q�N���X�̉��
	/// </summary>
	void ReleaseSub();

	/// <summary>
	/// �|�W�V�������Z�b�g����֐�
	/// </summary>
	/// <param name="position">position</param>
	void SetPosition(XMFLOAT3 position)
	{
		transform_.position_ = position;
	}
	/// <summary>
	/// �|�W�V�������Z�b�g����֐�
	/// </summary>
	/// <param name="x">x���W</param>
	/// <param name="y">y���W</param>
	/// <param name="z">z���W</param>
	void SetPosition(float x, float y, float z)
	{
		SetPosition(XMFLOAT3(x, y, z));
	}
	/// <summary>
	/// ��]�p���Z�b�g����֐�
	/// </summary>
	/// <param name="position">rotate</param>
	void SetRotate(XMFLOAT3 rotate)
	{
		transform_.rotate_ = rotate;
	}
	/// <summary>
	/// ��]�p���Z�b�g����֐�
	/// </summary>
	/// <param name="x">x�̉�]�p</param>
	/// <param name="y">y�̉�]�p</param>
	/// <param name="z">z�̉�]�p</param>
	void SetRotate(float x, float y, float z)
	{
		SetPosition(XMFLOAT3(x, y, z));
	}
	/// <summary>
	/// �g�嗦���Z�b�g����֐�
	/// </summary>
	/// <param name="position">scale</param>
	void SetScale(XMFLOAT3 scale)
	{
		transform_.scale_ = scale;
	}
	/// <summary>
	/// �g�嗦���Z�b�g����֐�
	/// </summary>
	/// <param name="x">x�̊g�嗦</param>
	/// <param name="y">y�̊g�嗦</param>
	/// <param name="z">z�̊g�嗦</param>
	void SetScale(float x, float y, float z)
	{
		SetScale(XMFLOAT3(x, y, z));
	}

	/// <summary>
	/// ����ł邩�ȃt���O���X�V����֐�
	/// </summary>
	void KillMe();

	void AddCollider(SphereCollider* pColl); //�����̃����o�[�̃R���C�_�[������
	void Collision(GameObject* pTarget); //�����蔻��̏���
	void RoundRobin(GameObject* pTarget); //��������œ����蔻��
	virtual void OnCollision(GameObject* pTarget) {}; //�Ȃɂ���H

	/// <summary>
	/// Instantiate�̃e���v���[�g
	/// </summary>
	/// <typeparam name="T">�^</typeparam>
	/// <param name="pParent_">�e�̖��O</param>
	/// <returns>�߂�l�͖���</returns>
	template <class T>
	T* Instantiate(GameObject* pParent_)
	{
		T* pTmp = new T(pParent_);
		if (pTmp != nullptr)
		{
			pTmp->Initialize();
			childList_.push_back(pTmp);
		}
		return pTmp;
	}

	GameObject* FindObject(std::string objectName);
	GameObject* GetRootJob();
	GameObject* FindChildObject(std::string objectName);

	const std::string& GetObjectName(void) const;
};



