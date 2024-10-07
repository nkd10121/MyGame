#pragma once
#include "Efects.h"

class GoalEfect : public Efects
{
public:
	GoalEfect();
	~GoalEfect();

	void Init(Vec2 pos,bool isLastHit = false)override;
	void Update()override;
	void Draw()override;
private:

};

