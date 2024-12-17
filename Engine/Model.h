#pragma once
#include <string>
#include "Fbx.h"

namespace Model
{
	struct ModelData //���ׂ�public
	{
		Fbx* pFbx_=nullptr; //FBX
		Transform transform_; //Transform
		std::string filename_=""; //FileName
	};

	/// <summary>
	/// ���f���̈ꗗ��ۑ�����
	/// </summary>
	/// <param name="filename">���f���̖��O</param>
	/// <returns>���f���ԍ�</returns>
	int Load(std::string filename);
	
	/// <summary>
	/// ���f����Transform������
	/// </summary>
	/// <param name="hModel">���f���ԍ�</param>
	/// <param name="transform">�g�����X�t�H�[��</param>
	void SetTransform(int hModel,Transform transform);
	
	/// <summary>
	/// ���f���`��
	/// </summary>
	/// <param name="hModel">���f���ԍ�</param>
	void Draw(int hModel);
	void Release();
}

