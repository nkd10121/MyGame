#pragma once
#include "Efects.h"

struct EfectParts
{
	Vec2 pos{};
	Vec2 vec{};
	int speed = 0;
};

class HitEfect : public Efects
{
public:
	HitEfect();
	~HitEfect();

	void Init(Vec2 pos, bool isLastHit = false)override;
	void Update()override;
	void Draw()override;
private:
	//HitEfect
	EfectParts m_parts[10];

	float m_size = 0;
};

