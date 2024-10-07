#pragma once
#include "Efects.h"

class DashEfect : public Efects
{
public:
	DashEfect();
	~DashEfect();

	void Init(Vec2 pos, bool isLastHit = false)override;
	void Update()override;
	void Draw()override;

private:

};

