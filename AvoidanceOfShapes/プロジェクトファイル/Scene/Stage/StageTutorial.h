#pragma once
#include "StageBase.h"

class StageTutorial : public StageBase
{
public:
	StageTutorial(SceneManager& mgr);
	~StageTutorial();
	void Update(Input& input);
	void Draw();

private:
};

