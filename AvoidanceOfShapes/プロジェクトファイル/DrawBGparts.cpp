#include "GraphManager.h"
#include "DrawBGparts.h"
#include "DxLib.h"

DrawBGparts::DrawBGparts()
{
	m_gManager.Load("BG", "data/titleParts.png");
	m_handle = m_gManager.GetHandle("BG");


	m_x = GetRand(1280);
	m_y = GetRand(720);
	m_exrate = static_cast<float>(GetRand(200) + 60) * 0.05f;
	m_angle = DX_TWO_PI_F / GetRand(10) * 0.04f;
	m_rotateAngle = static_cast<float>(GetRand(20));
	m_rotateFrame = GetRand(4000) + 2000;
	m_rad = GetRand(400) + 100;
}

DrawBGparts::~DrawBGparts()
{
}

void DrawBGparts::Init(bool isMove)
{
	m_isMove = isMove;
}

void DrawBGparts::Update()
{
	m_rotateSpeed += 0.08f;
	
	if (m_isMove)
	{
		m_frame++;

		if (m_frame % 30 == 0)
		{
			m_mag = 1.1f;
		}
		else
		{
			m_mag = 1.0f;
		}
	}

	m_rotateAngle += DX_TWO_PI_F / m_rotateFrame;
	m_offsetX = cosf(m_rotateAngle) * m_rad;
	m_offsetY = sinf(m_rotateAngle) * m_rad;

	m_x = 640 + static_cast<int>(m_offsetX);
	m_y = 360 + static_cast<int>(m_offsetY);
}

void DrawBGparts::Draw()
{

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawRotaGraph(static_cast<int>(m_x), static_cast<int>(m_y), m_exrate * m_mag, m_angle * m_rotateSpeed, m_handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
