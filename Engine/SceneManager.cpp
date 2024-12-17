#include "SceneManager.h"
#include "../TestScene.h"
#include "Define.h"
#include "Model.h"

SceneManager::SceneManager(GameObject* parent)
	:GameObject(parent, "SceneManager")
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TEST;
	nextSceneID_ = currentSceneID_;
	Instantiate<TestScene>(this);
}

void SceneManager::Update()
{
	//次のシーンが現在のシーンと違う ＝ シーンを切り替えなければならない
	if (currentSceneID_ != nextSceneID_)
	{
		auto delScene = childList_.begin();
		(*delScene)->ReleaseSub();
		SAFE_DELETE(*delScene);
		childList_.clear();
		Model::Release();

		//ロードしたデータを全削除


		//次のシーンを作成
		switch (nextSceneID_)
		{
		case SCENE_ID_TEST: Instantiate<TestScene>(this); break;
		}
		//シーン生成に成功したらカレントを更新
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}
