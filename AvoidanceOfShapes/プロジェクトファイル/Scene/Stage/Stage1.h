#pragma once
#include "StageBase.h"

class Stage1 : public StageBase
{
public:
	Stage1(SceneManager& mgr);
	~Stage1();
	void Update(Input& input);
	void Draw();

private:
	void CreateFireFlower(bool isRight, int speed);
	void CreateUnderLine(bool isRight);
};

