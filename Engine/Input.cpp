#include "Input.h"

namespace Input
{
	//DirectInputオブジェクト
	LPDIRECTINPUT8 pDInput = nullptr;

	//キーボード&押されているかどうかを入れておく変数の配列
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr; //デバイスオブジェクト
	BYTE keyState[256] = { 0 }; //現在の各キーの状態
	BYTE prevKeyState[256] = { 0 }; //前フレームでの各キーの状態

	//マウス
	XMVECTOR mousePosition; //現在のマウス位置
	LPDIRECTINPUTDEVICE8 pMouseDevice; //デバイスオブジェクト
	DIMOUSESTATE mouseState; //マウスの状態
	DIMOUSESTATE prevMouseState; //前フレームのマウスの状態

	void Initialize(HWND hWnd)
	{
		HRESULT	hr;

		//DirectInput本体をpDinputに入れる
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"DirectInput周りで何かが失敗", L"Input.cpp", MB_OK);
		}

		//////////////////////////キーボード//////////////////////////
		//デバイスオブジェクトの作成
		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"キーボードのデバイスオブジェクトの作成に失敗", L"Input.cpp", MB_OK);
		}
		//種類を指定
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		//ほかの実行中のアプリに対する優先度を設定
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		//////////////////////////マウス//////////////////////////
		//マウスのデバイスオブジェクトの作成
		hr = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, nullptr);
		if (FAILED(hr))
		{
			MessageBox(NULL, L"マウスのデバイスオブジェクトの作成に失敗", L"Input.cpp", MB_OK);
		}
		//種類を指定
		pMouseDevice->SetDataFormat(&c_dfDIMouse);
		//ほかの実行中のアプリに対する優先度を設定
		pMouseDevice->SetCooperativeLevel(NULL, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	}

	void Update()
	{
		//////////////////////////キーボード//////////////////////////
		//前の状態をコピー
		memcpy(prevKeyState, keyState, sizeof(keyState)); //sizeofのところprevかな？
		//迷子防止
		pKeyDevice->Acquire();
		//その時の全キーの状態を取得
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);

		//////////////////////////マウス//////////////////////////
		//迷子防止
		pMouseDevice->Acquire();
		//前の状態をコピー
		memcpy(&prevMouseState, &mouseState, sizeof(mouseState));
		//その時の全ボタンの状態を取得
		pMouseDevice->GetDeviceState(sizeof(mouseState), &mouseState);

		GetMousePosition();
	}

	void Release()
	{
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pMouseDevice);
	}

	//////////////////////////キーボード//////////////////////////
	bool IsKey(int keyCode)
	{
		//bit演算の時に16進数でやるとかっこいいかもだよね
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

	//////////////////////////マウス//////////////////////////
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