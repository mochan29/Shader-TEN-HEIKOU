#pragma once
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// 位置、向き、拡大率などを管理するクラス
/// </summary>
 
class Transform
{
	XMMATRIX matTranslate_; //移動行列
	XMMATRIX matRotate_; //回転行列	
	XMMATRIX matScale_; //拡大行列

public:
	XMFLOAT3 position_;	//位置
	XMFLOAT3 rotate_;	//向き
	XMFLOAT3 scale_;	//拡大率
	Transform* pParent_;
	Transform();
	~Transform();

	/// <summary>
	/// 各行列の計算
	/// </summary>
	void Calculation();

	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	/// <returns>その時点でのワールド行列</returns>
	XMMATRIX GetWorldMatrix();

	/// <summary>
	/// ぷろじぇくしょん？？？行列を取得
	/// </summary>
	/// <returns>その時点でのプロジェクション？行列</returns>
	XMMATRIX GetNormalMatrix();
};