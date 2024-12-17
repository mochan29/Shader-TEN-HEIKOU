#include <Windows.h>
#include <cstdlib>
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/RootJob.h"
#include "Engine/Define.h"
#include "Engine/Model.h"

//�����J
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "winmm.lib")

//�萔�錾
const wchar_t* WIN_CLASS_NAME = L"SampleClass"; //�E�B���h�E�N���X��
const wchar_t* APP_NAME = L"������Ղ��Ղ��"; //�A�v���P�[�V������


//���[�g�W���u�쐬=���ׂĂ̐e
RootJob* pRootJob = nullptr;

//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); //CALLBACK->�����������Ƃ��Ă΂��

//�G���g���[�|�C���g �v���O�����̂͂��܂�
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//�E�B���h�E�N���X(�݌v�})���쐬
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX); //���̍\���̂̃T�C�Y
	wc.hInstance = hInstance; //�C���X�^���X�n���h��
	wc.lpszClassName = WIN_CLASS_NAME; //�E�B���h�E�N���X��
	wc.lpfnWndProc = WndProc; //�E�B���h�E�v���V�[�W��
	wc.style = CS_VREDRAW | CS_HREDRAW; //�X�^�C��(�f�t�H���g)
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //�A�C�R��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); //�������A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //�}�E�X�J�[�\��
	wc.lpszMenuName = NULL; //���j���[(�Ȃ�)
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //�w�i(��)
	RegisterClassEx(&wc); //�N���X��o�^

	//�E�B���h�E�T�C�Y�̌v�Z(�N���C�A���g�̈���w��̐��l�ɂ��邽�߂̌v�Z)
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left; //�E�B���h�E��
	int winH = winRect.bottom - winRect.top; //�E�B���h�E����

	//�E�B���h�E���쐬 ���ۂɍ�Ƃ���Ƃ���->�N���C�A���g�̈�
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME, //�E�B���h�E�N���X��
		APP_NAME, //�^�C�g���o�[�ɕ\��������e
		WS_OVERLAPPEDWINDOW, //�X�^�C��(���ʂ̃E�B���h�E)
		CW_USEDEFAULT, //�\���ʒu��(���܂���)
		CW_USEDEFAULT, //�\���ʒu��(���܂���)
		winW, //�E�B���h�E��
		winH, //�E�B���h�E����
		NULL, //�e�E�C���h�E(�Ȃ�)
		NULL, //���j���[(�Ȃ�)
		hInstance, //�C���X�^���X
		NULL //�p�����[�^(�Ȃ�)
	);

	//�E�B���h�E��\��
	ShowWindow(hWnd, nCmdShow);

	//Direct3D������
	HRESULT hr = Direct3D::Initialize(winW, winH, hWnd);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"���s: Direct3D������", L"WinMain.cpp", MB_OK);
		return 0;
	}

	//Camera������
	Camera::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

	//Input������
	Input::Initialize(hWnd);

	//RouteJob������
	pRootJob = new RootJob(nullptr);
	pRootJob->Initialize();

	//���b�Z�[�W���[�v(�����N����̂�҂�)
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//�E�B���h�E���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);


			if (Input::IsKey(DIK_ESCAPE))
			{
				PostQuitMessage(0);
			}

		}
		//�E�B���h�E���b�Z�[�W�Ȃ�
		else
		{
			//FPS��\��
			timeBeginPeriod(1);
			static DWORD countFps = 0;
			static DWORD startTime = timeGetTime();
			DWORD nowTime = timeGetTime();
			static DWORD lastUpdateTime = nowTime;
			if (nowTime - startTime >= 1000)
			{
				std::wstring str;
				wsprintf(str.data(), L"%u", countFps);
				SetWindowTextW(hWnd, str.c_str());
				countFps = 0;
				startTime = nowTime;
			}
			if (nowTime - lastUpdateTime <= 1000.0f / 60)
			{
				continue;
			}
			lastUpdateTime = nowTime;
			timeEndPeriod(1);
			countFps++;


			//////////�Q�[���̈�A�̏�������//////////
			//�V�X�e���n�X�V
			Camera::Update();
			Input::Update();

			//�e�q�S���X�V
			pRootJob->UpdateSub();

			//�`�揈���͂��܂�
			Direct3D::BeginDraw();

			//�e�q�S���`��
			pRootJob->DrawSub();

			//�`�揈�������܂�
			Direct3D::EndDraw();
		}
	}

	//�������̍폜
	Model::Release();
	pRootJob->ReleaseSub();
	SAFE_DELETE(pRootJob);
	Direct3D::Release();
	Input::Release();

	return 0;
}

//�E�B���h�E�v���V�[�W��(�������������ɂ�΂��֐�)msg=�����N������(n�Ԃ̓L�[���͂Ł`�Ƃ�)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: //Window�́~�{�^���������ꂽ�Ƃ�(����ꂽ�Ƃ�)
		PostQuitMessage(0);  //�v���O�����I��
		return 0;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
WM_DESTROY: �E�B���h�E������ꂽ
WM_CLOSE x�{�^���ŕ���ꂽ
WM_ACTIVE �E�B���h�E���O�ʂɗ���
*/