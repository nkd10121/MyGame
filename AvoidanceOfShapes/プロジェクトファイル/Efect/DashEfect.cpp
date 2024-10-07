#include "DashEfect.h"
#include "DxLib.h"

DashEfect::DashEfect()
{
}

DashEfect::~DashEfect()
{
}

void DashEfect::Init(Vec2 pos, bool isLastHi)
{
	m_isExist = true;
	m_rad = 20;
	m_pos = pos;
}

void DashEfect::Update()
{
	if (!m_isExist)
		return;
	m_rad += 3;
	m_alpha -= 4;
	m_lineThick -= 2;

	if (m_alpha < 0)
	{
		m_isExist = false;
	}
}

void DashEfect::Draw()
{
	if (!m_isExist)
		return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad, 0x008888, false, static_cast<int>(m_lineThick));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, m_alpha);
}
