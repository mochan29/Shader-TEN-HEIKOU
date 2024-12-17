#include "GameObject.h"
#include "SphereCollider.h"
#include "Define.h"

GameObject::GameObject()
{
	pParent_ = nullptr;
	isDead_ = false;
	pCollider_ = nullptr;
}

GameObject::GameObject(GameObject* parent, const std::string& name)
{
	pParent_ = parent;
	objectName_ = name;
	isDead_ = false;
	pCollider_ = nullptr;

	if (parent != nullptr)
	{
		this->transform_.pParent_ = &(parent->transform_);
	}
}

GameObject::~GameObject()
{
}

void GameObject::UpdateSub()
{
	Update();
	RoundRobin(GetRootJob());

	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->UpdateSub();
	}

	for (auto itr = childList_.begin(); itr != childList_.end();)
	{
		if ((*itr)->isDead_)
		{
			(*itr)->ReleaseSub();
			SAFE_DELETE(*itr);
			itr = childList_.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void GameObject::DrawSub()
{
	Draw();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->DrawSub();
	}
}

void GameObject::ReleaseSub()
{
	Release();
	for (auto itr = childList_.begin(); itr != childList_.end(); itr++)
	{
		(*itr)->ReleaseSub();
	}
}

void GameObject::KillMe()
{
	isDead_ = true;
}

void GameObject::AddCollider(SphereCollider* pColl)
{
	pCollider_ = pColl;
}

void GameObject::Collision(GameObject* pTarget)
{
	if (this->pCollider_ == nullptr || pTarget->pCollider_ == nullptr || this == pTarget)
		return;

	XMVECTOR me = XMLoadFloat3(&(transform_.position_));
	XMVECTOR ta = XMLoadFloat3(&(pTarget->transform_.position_));

	float dist = XMVectorGetX(XMVector3Length(me - ta));
	float rdist = this->pCollider_->GetRadius() + pTarget->pCollider_->GetRadius();
	if (dist <= rdist)
	{
		//当たってる時の処理
		OnCollision(pTarget);
	}
}

void GameObject::RoundRobin(GameObject* pTarget)
{
	//自分とターゲットの当たり判定
	//自分とターゲットの子オブジェクト全部を当たり判定する(再帰)
	if (this->pCollider_ == nullptr)
	{
		return;
	}
	if (pTarget->pCollider_ != nullptr)
	{
		Collision(pTarget);
	}
	for (auto& i : pTarget->childList_)
	{
		RoundRobin(i);
	}
}

GameObject* GameObject::FindObject(std::string objectName)
{
	GameObject* rootjob = GetRootJob();
	GameObject* res = rootjob->FindChildObject(objectName);
	return res;
}

GameObject* GameObject::GetRootJob()
{
	if (pParent_ == nullptr)
	{
		return this;
	}
	return pParent_->GetRootJob();
}

GameObject* GameObject::FindChildObject(std::string objectName)
{
	if (this->objectName_ == objectName)
	{
		return (this);
	}
	else
	{
		for (auto itr : childList_)
		{
			GameObject* obj = itr->FindChildObject(objectName);
			if (obj != nullptr)
			{
				return obj;
			}
		}
	}
	return nullptr;
}

const std::string& GameObject::GetObjectName(void) const
{
	return objectName_;
}