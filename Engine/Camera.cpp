#include "Camera.h"

namespace Camera
{
	//�ϐ�
	XMVECTOR position_; //�J�����̈ʒu(���_)
	XMVECTOR target_; //����ʒu(�œ_)
	XMMATRIX viewMatrix_; //�r���[�s��
	XMMATRIX projMatrix_; //�v���W�F�N�V�����s��
}

void Camera::Initialize(int WINDOW_WIDTH, int WINDOW_HEIGHT)
{
	position_ = XMVectorSet(0, 3, -10, 0); //�J�����̈ʒu
	target_ = XMVectorSet(0, 0, 0, 0); //�J�����̏œ_

	//�v���W�F�N�V�����s��
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
}

void Camera::Update()
{
	//�r���[�s��̍쐬
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));
}

void Camera::SetPosition(XMVECTOR position)
{
	position_ = position;
}

XMVECTOR Camera::GetPosition()
{
	return position_;
}

void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}

XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}
