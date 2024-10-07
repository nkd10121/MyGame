#include "PlayerEfects.h"
#include "DxLib.h"

PlayerEfects::PlayerEfects()
{
}

PlayerEfects::~PlayerEfects()
{
}

void PlayerEfects::GoalEfectInit(Vec2 pos)
{
	m_isExist = true;
	m_pos = pos;
}

void PlayerEfects::GoalEfectUpdate()
{
	if (!m_isExist)
		return;
	frame++;
	angle += DX_TWO_PI_F / 40;
	lineThick = abs(cosf(angle) * 4) + 2.0f;

	if (frame >= 60 && frame < 100)
	{
		if (rad > -60)
		{
			rad -= 4;
		}
	}

	if (frame >= 100)
	{
		rad += 24;
	}
}

void PlayerEfects::GoalEfectDraw()
{
	if (!m_isExist)
		return;
	if (frame >= 10)
	{
		DrawCircle(m_pos.x, m_pos.y, rad, 0x008888, false, lineThick);
	}

	if (frame >= 20)
	{
		DrawCircle(m_pos.x, m_pos.y, rad + 20, 0x008888, false, lineThick);
	}

	if (frame >= 30)
	{
		DrawCircle(m_pos.x, m_pos.y, rad + 40, 0x008888, false, lineThick);
	}

	if (frame >= 40)
	{
		DrawCircle(m_pos.x, m_pos.y, rad + 60, 0x008888, false, lineThick);
	}
}

void PlayerEfects::DashEfectInit(Vec2 pos)
{
	m_isExist = true;
	rad = 60;
	m_pos = pos;
}

void PlayerEfects::DashEfectUpdate()
{
	if (!m_isExist)
		return;
	rad += 3;
	alpha -= 4;
	lineThick -= 2;

	if (alpha < 0)
	{
		m_isExist = false;
	}
}

void PlayerEfects::DashEfectDraw()
{
	if (!m_isExist)
		return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawCircle(m_pos.x, m_pos.y, rad, 0x008888, false, lineThick);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
}

void PlayerEfects::HitEfectInit(Vec2 pos)
{
}

void PlayerEfects::HitEfectUpdate()
{
}

void PlayerEfects::HitEfectDraw()
{
}
