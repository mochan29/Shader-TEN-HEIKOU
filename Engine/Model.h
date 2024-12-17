#pragma once
#include <string>
#include "Fbx.h"

namespace Model
{
	struct ModelData //すべてpublic
	{
		Fbx* pFbx_=nullptr; //FBX
		Transform transform_; //Transform
		std::string filename_=""; //FileName
	};

	/// <summary>
	/// モデルの一覧を保存する
	/// </summary>
	/// <param name="filename">モデルの名前</param>
	/// <returns>モデル番号</returns>
	int Load(std::string filename);
	
	/// <summary>
	/// モデルにTransformをつける
	/// </summary>
	/// <param name="hModel">モデル番号</param>
	/// <param name="transform">トランスフォーム</param>
	void SetTransform(int hModel,Transform transform);
	
	/// <summary>
	/// モデル描画
	/// </summary>
	/// <param name="hModel">モデル番号</param>
	void Draw(int hModel);
	void Release();
}

