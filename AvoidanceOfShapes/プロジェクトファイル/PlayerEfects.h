#pragma once
#include "Util/Vec2.h"
//敵当たった時、ゴールした時のエフェクト的なの

class PlayerEfects
{
public:
	PlayerEfects();
	~PlayerEfects();

	bool isExist() { return m_isExist; }

	void GoalEfectInit(Vec2 pos);
	void GoalEfectUpdate();
	void GoalEfectDraw();

	void DashEfectInit(Vec2 pos);
	void DashEfectUpdate();
	void DashEfectDraw();

	void HitEfectInit(Vec2 pos);
	void HitEfectUpdate();
	void HitEfectDraw();


private:
	bool m_isExist = false;

	int frame = 0;
	Vec2 m_pos;
	int rad = 20;
	int lineThick = 20;

	//goalEfect
	float angle = 0;

	//dashEfect
	int alpha = 160;
};

