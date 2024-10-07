#pragma once
#include "StageBase.h"

class Stage3 : public StageBase
{
public:
	Stage3(SceneManager& mgr);
	~Stage3();
	void Update(Input& input);
	void Draw();
};