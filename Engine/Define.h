#pragma once
#include <string>

/// <summary>
/// �}�N����`�Ȃ�
/// </summary>
/// 
const int WINDOW_WIDTH = 800; //�E�B���h�E�̕�
const int WINDOW_HEIGHT = 600; //�E�B���h�E�̍���

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;} //new=delete
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;} //create=release
#define SAFE_DELETE_ARRAY(p) {if (p!=nullptr) { delete[] p; p=nullptr;}} 