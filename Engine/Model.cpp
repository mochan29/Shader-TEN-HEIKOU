#include "Model.h"
#include "Define.h"

namespace Model
{
	std::vector<ModelData*> modelList;
}

int Model::Load(std::string filename)
{
	ModelData* pData = new ModelData;
	pData->filename_ = filename;
	pData->pFbx_ = nullptr;

	for (auto& m : modelList)
	{
		if (m->filename_ == filename) //ファイル名が一致するものがある=ロード済
		{
			pData->pFbx_ = m->pFbx_; //前に使ったやつをそのまま使う モデルのアドレスがリストに追加
			//すでに存在していたらreturn -1
			break;
		}
	}
	if (!pData->pFbx_) //モデルが空=リストにモデルがないとき
	{
		pData->pFbx_ = new Fbx;
		pData->pFbx_->Load(filename); //ファイル名のモデルをロード
	}
	modelList.push_back(pData);
	return (modelList.size() - 1);
}

void Model::SetTransform(int hModel, Transform transform)
{
	modelList[hModel]->transform_ = transform;
}

void Model::Draw(int hModel)
{
	modelList[hModel]->pFbx_->Draw(modelList[hModel]->transform_);
}

void Model::Release()
{
	bool isReferred = false; //まだ参照されているか

	//モデル同士で比較
	for (int j = 0; j < modelList.size(); j++)
	{
		for(int i = j+1; i < modelList.size(); i++)
		{
			if (modelList[j]->pFbx_ == modelList[i]->pFbx_)
			{
				isReferred = true;
				break;
			}
		}
		if (!isReferred)
		{
			SAFE_DELETE(modelList[j]->pFbx_); //中身消す
		}
		SAFE_DELETE(modelList[j]); //リスト自体消す
	}
	modelList.clear(); //コンテナ消
}
