#pragma once
#include "GameObject.h"

/// <summary>
/// Scene�S�̂��Ǘ�����N���X
/// </summary>

enum SCENE_ID
{
	SCENE_ID_TEST = 0,
};

class SceneManager :
	public GameObject
{
	SCENE_ID currentSceneID_;
	SCENE_ID nextSceneID_;
public:
	SceneManager(GameObject* parent);
	~SceneManager();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	/// <summary>
	/// �V�[����؂�ւ���
	/// </summary>
	/// <param name="nextScene">���̃V�[����</param>
	void ChangeScene(SCENE_ID nextScene) { nextSceneID_ = nextScene; }
};

