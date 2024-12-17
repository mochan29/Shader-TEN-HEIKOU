#include <Windows.h>
#include <cstdlib>
#include "Engine/Direct3D.h"
#include "Engine/Camera.h"
#include "Engine/Input.h"
#include "Engine/RootJob.h"
#include "Engine/Define.h"
#include "Engine/Model.h"

//リンカ
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib, "winmm.lib")

//定数宣言
const wchar_t* WIN_CLASS_NAME = L"SampleClass"; //ウィンドウクラス名
const wchar_t* APP_NAME = L"さんんんぷるんぷるん"; //アプリケーション名


//ルートジョブ作成=すべての親
RootJob* pRootJob = nullptr;

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); //CALLBACK->何かあったとき呼ばれる

//エントリーポイント プログラムのはじまり
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//ウィンドウクラス(設計図)を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX); //この構造体のサイズ
	wc.hInstance = hInstance; //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME; //ウィンドウクラス名
	wc.lpfnWndProc = WndProc; //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW; //スタイル(デフォルト)
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); //マウスカーソル
	wc.lpszMenuName = NULL; //メニュー(なし)
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景(白)
	RegisterClassEx(&wc); //クラスを登録

	//ウィンドウサイズの計算(クライアント領域を指定の数値にするための計算)
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left; //ウィンドウ幅
	int winH = winRect.bottom - winRect.top; //ウィンドウ高さ

	//ウィンドウを作成 実際に作業するところ->クライアント領域
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME, //ウィンドウクラス名
		APP_NAME, //タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル(普通のウィンドウ)
		CW_USEDEFAULT, //表示位置左(おまかせ)
		CW_USEDEFAULT, //表示位置上(おまかせ)
		winW, //ウィンドウ幅
		winH, //ウィンドウ高さ
		NULL, //親ウインドウ(なし)
		NULL, //メニュー(なし)
		hInstance, //インスタンス
		NULL //パラメータ(なし)
	);

	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	//Direct3D初期化
	HRESULT hr = Direct3D::Initialize(winW, winH, hWnd);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"失敗: Direct3D初期化", L"WinMain.cpp", MB_OK);
		return 0;
	}

	//Camera初期化
	Camera::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT);

	//Input初期化
	Input::Initialize(hWnd);

	//RouteJob初期化
	pRootJob = new RootJob(nullptr);
	pRootJob->Initialize();

	//メッセージループ(何か起きるのを待つ)
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//ウィンドウメッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);


			if (Input::IsKey(DIK_ESCAPE))
			{
				PostQuitMessage(0);
			}

		}
		//ウィンドウメッセージなし
		else
		{
			//FPSを表示
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


			//////////ゲームの一連の処理たち//////////
			//システム系更新
			Camera::Update();
			Input::Update();

			//親子全員更新
			pRootJob->UpdateSub();

			//描画処理はじまり
			Direct3D::BeginDraw();

			//親子全員描画
			pRootJob->DrawSub();

			//描画処理おしまい
			Direct3D::EndDraw();
		}
	}

	//もろもろの削除
	Model::Release();
	pRootJob->ReleaseSub();
	SAFE_DELETE(pRootJob);
	Direct3D::Release();
	Input::Release();

	return 0;
}

//ウィンドウプロシージャ(何かあった時によばれる関数)msg=何が起きたか(n番はキー入力で～とか)
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: //Windowの×ボタンが押されたとき(閉じられたとき)
		PostQuitMessage(0);  //プログラム終了
		return 0;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*
WM_DESTROY: ウィンドウが閉じられた
WM_CLOSE xボタンで閉じられた
WM_ACTIVE ウィンドウが前面に来た
*/