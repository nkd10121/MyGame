#include "Goal.h"
#include "DxLib.h"

namespace
{
	//プレイヤーの周りを一回転するのにかかるフレーム数
	constexpr int kRotFrame = 90;

	//覚えておく過去の位置情報の数
	constexpr int kPosLogNum = 34;
}

Goal::Goal():
	m_frame(0),
	m_center(1380,360),
	m_isExist(false)
{
	//m_handle = LoadGraph("data/goal.png");
}

Goal::~Goal()
{
}

void Goal::Init()
{
	m_isExist = true;
	m_inSide.Init(32, false, false);
	m_outSide.Init(40, true, true);
}

void Goal::Update()
{
	if (!m_isExist)
	{
		return;
	}

	if (m_center.x >= 1180)
	{
		m_center.x -= 4;
	}


	m_inSide.Update(m_center);
	m_outSide.Update(m_center);

	m_col.SetCircle(m_center.x, m_center.y, 24);
}

void Goal::Draw()
{
	if (!m_isExist)
	{
		return;
	}

	m_inSide.Draw();
	m_outSide.Draw();

	DrawRotaGraph(static_cast<int>(m_center.x) - 20, static_cast<int>(m_center.y),0.4f,0.0f,m_handle,true);

	//DrawQuilateralTriangle(center, 36, 0xdcffff);
	//DrawQuilateralTriangle(center, 10, 0x000000);
}

void Goal::DrawQuilateralTriangle(Vec2 centerPos, int sideLength, unsigned int color)
{
	Vec2 top = { centerPos.x,centerPos.y - (float)(sideLength * 0.6) };
	Vec2 right = { centerPos.x - (float)(sideLength / 2),centerPos.y + (float)(sideLength * 0.34) };
	Vec2 left = { centerPos.x + (float)(sideLength / 2),centerPos.y + (float)(sideLength * 0.34) };

	DrawTriangle(static_cast<int>(top.x), static_cast<int>(top.y), static_cast<int>(right.x), static_cast<int>(right.y), static_cast<int>(left.x), static_cast<int>(left.y), color, true);
#ifdef _DEBUG
	m_col.DrawColCircle(0xffffff);
#endif
}


void Goal::Hit()
{
	m_frame++;
	if (m_frame >= 1)
	{
		m_isExist = false;
	}

}
