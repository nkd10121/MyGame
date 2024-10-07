#include "HitEfect.h"
#include "DxLib.h"

HitEfect::HitEfect()
{
}

HitEfect::~HitEfect()
{
}

void HitEfect::Init(Vec2 pos, bool isLastHit)
{
	m_isExist = true;
	m_alpha = 240;
	for (int i = 0; i < 10; i++)
	{
		m_parts[i].pos = pos;
		m_parts[i].vec = { static_cast<float>(GetRand(20) - 10),static_cast<float>(GetRand(20) - 10) };
		m_parts[i].speed = GetRand(2) + 2;
	}

	if (!isLastHit)
	{
		m_size = 0.32f;
	}
	else
	{
		m_size = 1.2f;
	}
}

void HitEfect::Update()
{
	if (!m_isExist)
		return;
	m_alpha -= 4;

	m_angle += DX_TWO_PI_F / 60;

	for (int i = 0; i < 10; i++)
	{
		m_parts[i].pos += m_parts[i].vec.getNormalize() * static_cast<float>(m_parts[i].speed);
	}

	if (m_alpha < 0)
	{
		m_isExist = false;
	}
}

void HitEfect::Draw()
{
	if (!m_isExist)
		return;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	for (int i = 0; i < 10; i++)
	{
		DrawRotaGraph(static_cast<int>(m_parts[i].pos.x), static_cast<int>(m_parts[i].pos.y), m_size, m_angle, m_handle, true);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
