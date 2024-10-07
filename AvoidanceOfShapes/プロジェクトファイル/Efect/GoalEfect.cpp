#include "GoalEfect.h"
#include "DxLib.h"

GoalEfect::GoalEfect()
{
}

GoalEfect::~GoalEfect()
{
}

void GoalEfect::Init(Vec2 pos, bool isLastHit)
{
	m_isExist = true;
	m_pos = pos;
}

void GoalEfect::Update()
{
	if (!m_isExist)
		return;
	m_frame++;
	m_angle += DX_TWO_PI_F / 40;
	m_lineThick = abs(cosf(m_angle) * 4) + 0.8f;

	if (m_frame >= 60 && m_frame < 100)
	{
		if (m_rad > -60)
		{
			m_rad -= 4;
		}
	}

	if (m_frame >= 100)
	{
		m_rad += 24;
	}
}

void GoalEfect::Draw()
{
	if (!m_isExist)
		return;
	if (m_frame >= 10)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad, 0x008888, false, static_cast<int>(m_lineThick));
	}

	if (m_frame >= 20)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad + 20, 0x008888, false, static_cast<int>(m_lineThick));
	}

	if (m_frame >= 30)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad + 40, 0x008888, false, static_cast<int>(m_lineThick));
	}

	if (m_frame >= 40)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad + 60, 0x008888, false, static_cast<int>(m_lineThick));
	}
}
