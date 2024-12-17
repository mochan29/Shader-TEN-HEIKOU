#pragma once
#include "GameObject.h"

/// <summary>
/// ‚·‚×‚Ä‚Ìe‚É‚È‚éƒNƒ‰ƒX
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

