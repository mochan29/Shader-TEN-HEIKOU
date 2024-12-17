#pragma once
#include <dInput.h>
#include <DirectXMath.h>
#include "Define.h"

/// <summary>
/// Input�S��
/// </summary>
/// 
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

using namespace DirectX;

namespace Input
{
	void Initialize(HWND hwnd);
	void Update();
	void Release();

	//////////////////////////�L�[�{�[�h//////////////////////////

	/// <summary>
	/// �L�[��������Ă��邩
	/// </summary>
	/// <param name="keyCode">���ׂ����L�[�̃R�[�h</param>
	/// <returns>������Ă����true</returns>
	bool IsKey(int keyCode);

	/// <summary>
	/// �L�[������������
	/// </summary>
	/// <param name="keyCode">���ׂ����L�[�̃R�[�h</param>
	/// <returns>�������u�Ԃ�������true</returns>
	bool IsKeyDown(int keyCode);

	/// <summary>
	/// �L�[������������
	/// </summary>
	/// <param name="keyCode">���ׂ����L�[�̃R�[�h</param>
	/// <returns>true</returns>
	bool IsKeyUp(int keyCode);


	//////////////////////////�}�E�X//////////////////////////

	/// <summary>
	/// �}�E�X�J�[�\���̈ʒu���擾
	/// </summary>
	/// <returns>�}�E�X�J�[�\���̈ʒu</returns>
	XMVECTOR GetMousePosition();

	/// <summary>
	/// ���̃t���[���ł̃}�E�X�̈ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�̈ړ���x,y�Az=�z�C�[���̉�]��</returns>
	XMVECTOR GetMouseMove();

	/// <summary>
	/// ���̃t���[���ł̃}�E�X�̈ړ��ʂ��Z�b�g
	/// </summary>
	/// <param name="x">�}�E�X�̈ړ���x</param>
	/// <param name="y">�}�E�X�̈ړ���y</param>
	void SetMousePosition(int x, int y);

	/// <summary>
	/// �}�E�X�̃{�^����������Ă��邩
	/// </summary>
	/// <param name="buttonCode">���ׂ����{�^���̔ԍ�</param>
	/// <returns>������Ă�����true</returns>
	bool IsMouseButton(int buttonCode);

	/// <summary>
	/// �}�E�X�̃{�^��������������(�������ςȂ��͖���)
	/// </summary>
	/// <param name="buttonCode">���ׂ����{�^���̔ԍ�</param>
	/// <returns>�������u�Ԃ�������true</returns>
	bool IsMouseButtonDown(int buttonCode);

	/// <summary>
	/// �}�E�X�̃{�^��������������
	/// </summary>
	/// <param name="buttonCode">���ׂ����{�^���̔ԍ�</param>
	/// <returns>�������u�Ԃ�������true</returns>
	bool IsMouseButtonUp(int buttonCode);
};