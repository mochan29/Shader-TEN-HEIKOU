#include "Input.h"

namespace Input
{
	//DirectInput�I�u�W�F�N�g
	LPDIRECTINPUT8 pDInput = nullptr;

	//�L�[�{�[�h&������Ă��邩�ǂ��������Ă����ϐ��̔z��
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr; //�f�o�C�X�I�u�W�F�N�g
	BYTE keyState[256] = { 0 }; //���݂̊e�L�[�̏��
	BYTE prevKeyState[256] = { 0 }; //�O�t���[���ł̊e�L�[�̏��

	//�}�E�X
	XMVECTOR mousePosition; //���݂̃}�E�X�ʒu
	LPDIRECTINPUTDEVICE8 pMouseDevice; //�f�o�C�X�I�u�W�F�N�g
	DIMOUSESTATE mouseState; //�}�E�X�̏��
	DIMOUSESTATE prevMouseState; //�O�t���[���̃}�E�X�̏��

	void Initialize(HWND hWnd)
	{
		HRESULT	hr;

		//DirectInput�{�̂�pDinput�ɓ����
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"DirectInput����ŉ��������s", L"Input.cpp", MB_OK);
		}

		//////////////////////////�L�[�{�[�h//////////////////////////
		//�f�o�C�X�I�u�W�F�N�g�̍쐬
		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"�L�[�{�[�h�̃f�o�C�X�I�u�W�F�N�g�̍쐬�Ɏ��s", L"Input.cpp", MB_OK);
		}
		//��ނ��w��
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		//�ق��̎��s���̃A�v���ɑ΂���D��x��ݒ�
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//////////////////////////�}�E�X//////////////////////////
		//�}�E�X�̃f�o�C�X�I�u�W�F�N�g�̍쐬
		hr = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"�}�E�X�̃f�o�C�X�I�u�W�F�N�g�̍쐬�Ɏ��s", L"Input.cpp", MB_OK);
		}
		//��ނ��w��
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		//�ق��̎��s���̃A�v���ɑ΂���D��x��ݒ�
		pMouseDevice->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	}

	void Update()
	{
		//////////////////////////�L�[�{�[�h//////////////////////////
		//�O�̏�Ԃ��R�s�[
		memcpy(prevKeyState, keyState, sizeof(keyState)); //sizeof�̂Ƃ���prev���ȁH
		//���q�h�~
		pKeyDevice->Acquire();
		//���̎��̑S�L�[�̏�Ԃ��擾
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		//////////////////////////�}�E�X//////////////////////////
		//���q�h�~
		pMouseDevice->Acquire();
		//�O�̏�Ԃ��R�s�[
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));
		//���̎��̑S�{�^���̏�Ԃ��擾
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);

		GetMousePosition();
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pMouseDevice);
	}

	//////////////////////////�L�[�{�[�h//////////////////////////
	bool IsKey(int keyCode)
	{
		//bit���Z�̎���16�i���ł��Ƃ������������������
		if (keyState[keyCode] & 0x80)
		{
			return true;
		}

		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		if (IsKey(keyCode) && !(prevKeyState[keyCode] & 0x80))
		{
			return true;
		}

		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		if (!IsKey(keyCode) && (prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//////////////////////////�}�E�X//////////////////////////
	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	XMVECTOR GetMouseMove()
	{
		XMVECTOR result = XMVectorSet((float)mouseState.lX, (float)mouseState.lY, (float)mouseState.lZ, 0);
		return result;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 0);
	}

	bool IsMouseButton(int buttonCode)
	{
		if (mouseState.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsMouseButtonDown(int buttonCode)
	{
		if (IsMouseButton(buttonCode) && !(prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}
	bool IsMouseButtonUp(int buttonCode)
	{
		if (!IsMouseButton(buttonCode) && (prevMouseState.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}
}