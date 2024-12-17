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
		if (m->filename_ == filename) //�t�@�C��������v������̂�����=���[�h��
		{
			pData->pFbx_ = m->pFbx_; //�O�Ɏg����������̂܂܎g�� ���f���̃A�h���X�����X�g�ɒǉ�
			//���łɑ��݂��Ă�����return -1
			break;
		}
	}
	if (!pData->pFbx_) //���f������=���X�g�Ƀ��f�����Ȃ��Ƃ�
	{
		pData->pFbx_ = new Fbx;
		pData->pFbx_->Load(filename); //�t�@�C�����̃��f�������[�h
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
	bool isReferred = false; //�܂��Q�Ƃ���Ă��邩

	//���f�����m�Ŕ�r
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
			SAFE_DELETE(modelList[j]->pFbx_); //���g����
		}
		SAFE_DELETE(modelList[j]); //���X�g���̏���
	}
	modelList.clear(); //�R���e�i��
}
