#pragma once
#include <string>
#include <list>
#include "Transform.h"

class SphereCollider;

/// <summary>
/// 継承される専門のインターフェースクラス
/// </summary>

class GameObject
{
	bool isDead_; //生き死に

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
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// 解放
	/// </summary>
	virtual void Release() = 0;

	/// <summary>
	/// チャイルドリスト内の子クラスの更新
	/// </summary>
	void UpdateSub();
	/// <summary>
	/// チャイルドリスト内の子クラスの描画
	/// </summary>
	void DrawSub();
	/// <summary>
	/// チャイルドリスト内の子クラスの解放
	/// </summary>
	void ReleaseSub();

	/// <summary>
	/// ポジションをセットする関数
	/// </summary>
	/// <param name="position">position</param>
	void SetPosition(XMFLOAT3 position)
	{
		transform_.position_ = position;
	}
	/// <summary>
	/// ポジションをセットする関数
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	/// <param name="z">z座標</param>
	void SetPosition(float x, float y, float z)
	{
		SetPosition(XMFLOAT3(x, y, z));
	}
	/// <summary>
	/// 回転角をセットする関数
	/// </summary>
	/// <param name="position">rotate</param>
	void SetRotate(XMFLOAT3 rotate)
	{
		transform_.rotate_ = rotate;
	}
	/// <summary>
	/// 回転角をセットする関数
	/// </summary>
	/// <param name="x">xの回転角</param>
	/// <param name="y">yの回転角</param>
	/// <param name="z">zの回転角</param>
	void SetRotate(float x, float y, float z)
	{
		SetPosition(XMFLOAT3(x, y, z));
	}
	/// <summary>
	/// 拡大率をセットする関数
	/// </summary>
	/// <param name="position">scale</param>
	void SetScale(XMFLOAT3 scale)
	{
		transform_.scale_ = scale;
	}
	/// <summary>
	/// 拡大率をセットする関数
	/// </summary>
	/// <param name="x">xの拡大率</param>
	/// <param name="y">yの拡大率</param>
	/// <param name="z">zの拡大率</param>
	void SetScale(float x, float y, float z)
	{
		SetScale(XMFLOAT3(x, y, z));
	}

	/// <summary>
	/// 死んでるかなフラグを更新する関数
	/// </summary>
	void KillMe();

	void AddCollider(SphereCollider* pColl); //自分のメンバーのコライダーを検索
	void Collision(GameObject* pTarget); //当たり判定の処理
	void RoundRobin(GameObject* pTarget); //総当たりで当たり判定
	virtual void OnCollision(GameObject* pTarget) {}; //なにこれ？

	/// <summary>
	/// Instantiateのテンプレート
	/// </summary>
	/// <typeparam name="T">型</typeparam>
	/// <param name="pParent_">親の名前</param>
	/// <returns>戻り値は無視</returns>
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



