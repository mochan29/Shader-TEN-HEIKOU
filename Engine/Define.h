#pragma once
#include <string>

/// <summary>
/// マクロ定義など
/// </summary>
/// 
const int WINDOW_WIDTH = 800; //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;} //new=delete
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;} //create=release
#define SAFE_DELETE_ARRAY(p) {if (p!=nullptr) { delete[] p; p=nullptr;}} 