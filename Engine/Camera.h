#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

/// <summary>
/// �J�����S��
/// </summary>

using namespace DirectX;

namespace Camera
{
	//������(�v���W�F�N�V�����s��쐬)
	void Initialize(int WINDOW_WIDTH, int WINDOW_HEIGHT);

	//�X�V(�r���[�s��쐬)
	void Update();

	//���_(�J�����̈ʒu)��ݒ�
	void SetPosition(XMVECTOR position);
	XMVECTOR GetPosition();

	//�œ_(����ʒu)��ݒ�
	void SetTarget(XMVECTOR target);

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();
};