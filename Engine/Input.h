#pragma once
#include <dInput.h>
#include <DirectXMath.h>
#include "Define.h"

/// <summary>
/// Input全般
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

	//////////////////////////キーボード//////////////////////////

	/// <summary>
	/// キーが押されているか
	/// </summary>
	/// <param name="keyCode">調べたいキーのコード</param>
	/// <returns>押されていればtrue</returns>
	bool IsKey(int keyCode);

	/// <summary>
	/// キーを今押したか
	/// </summary>
	/// <param name="keyCode">調べたいキーのコード</param>
	/// <returns>押した瞬間だったらtrue</returns>
	bool IsKeyDown(int keyCode);

	/// <summary>
	/// キーを今放したか
	/// </summary>
	/// <param name="keyCode">調べたいキーのコード</param>
	/// <returns>true</returns>
	bool IsKeyUp(int keyCode);


	//////////////////////////マウス//////////////////////////

	/// <summary>
	/// マウスカーソルの位置を取得
	/// </summary>
	/// <returns>マウスカーソルの位置</returns>
	XMVECTOR GetMousePosition();

	/// <summary>
	/// そのフレームでのマウスの移動量を取得
	/// </summary>
	/// <returns>マウスの移動量x,y、z=ホイールの回転量</returns>
	XMVECTOR GetMouseMove();

	/// <summary>
	/// そのフレームでのマウスの移動量をセット
	/// </summary>
	/// <param name="x">マウスの移動量x</param>
	/// <param name="y">マウスの移動量y</param>
	void SetMousePosition(int x, int y);

	/// <summary>
	/// マウスのボタンが押されているか
	/// </summary>
	/// <param name="buttonCode">調べたいボタンの番号</param>
	/// <returns>押されていたらtrue</returns>
	bool IsMouseButton(int buttonCode);

	/// <summary>
	/// マウスのボタンを今押したか(押しっぱなしは無効)
	/// </summary>
	/// <param name="buttonCode">調べたいボタンの番号</param>
	/// <returns>押した瞬間だったらtrue</returns>
	bool IsMouseButtonDown(int buttonCode);

	/// <summary>
	/// マウスのボタンを今放したか
	/// </summary>
	/// <param name="buttonCode">調べたいボタンの番号</param>
	/// <returns>放した瞬間だったらtrue</returns>
	bool IsMouseButtonUp(int buttonCode);
};