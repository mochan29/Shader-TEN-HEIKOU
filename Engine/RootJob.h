#pragma once
#include "GameObject.h"

/// <summary>
/// ���ׂĂ̐e�ɂȂ�N���X
/// </summary>

class RootJob :
	public GameObject
{
public:
	RootJob(GameObject* parent);
	~RootJob();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};

