#pragma once
#include <d3d11.h>
#include <string>
#include <wrl.h>
#include "Direct3D.h"

/// <summary>
/// �e�N�X�`���Ɋւ���N���X
/// </summary>

using namespace Microsoft::WRL;
using std::string;

class Texture
{
	ID3D11SamplerState* pSampler_; //�T���v���[(�e�N�X�`���̒����)
	ID3D11ShaderResourceView* pSRV_; //�V�F�[�_�[���\�[�X�r���[

public:
	Texture();
	~Texture();

	/// <summary>
	/// �摜�����[�h
	/// </summary>
	/// <param name="fileName">�摜�t�@�C����</param>
	/// <returns>����/���s</returns>
	HRESULT Load(string fileName);
	void Release();

	/// <summary>
	/// �T���v���[�̎擾
	/// </summary>
	/// <returns>�T���v���[</returns>
	ID3D11SamplerState* GetSampler() { return pSampler_; };

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[�̎擾
	/// </summary>
	/// <returns>�V�F�[�_�[���\�[�X�r���[</returns>
	ID3D11ShaderResourceView* GetSRV() { return pSRV_; };
};