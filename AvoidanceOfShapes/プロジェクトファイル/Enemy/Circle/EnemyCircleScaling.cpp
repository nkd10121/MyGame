#include "EnemyCircleScaling.h"

namespace
{
	//予測線が一周するのにかかるフレーム
	constexpr int kRotFrame = 60;
	//半径を大きくするための定数
	constexpr int kAddCircle = 12;
	//4分の1にするための定数
	constexpr float kQuarter = 0.25f;
	//点滅する間隔のフレーム
	constexpr int kChangeColorIntervalFrame = 16;
}

EnemyCircleScaling::EnemyCircleScaling():
	m_frame				(0),
	m_angle				(0.0f),
	m_offset			(0.0f,0.0f),
	m_waitFrame			(0),
	m_drawFrame			(0),
	m_radius			(0),
	m_mainCircleRadius	(0),
	m_radiusSmallSpeed	(0),
	m_defaultColor		(0xff00ff),
	m_addColor			(0x001100)
{
}

EnemyCircleScaling::~EnemyCircleScaling()
{
}

void EnemyCircleScaling::Init(Vec2 pos, int rad, int wait,int draw, int speed)
{
	/*敵の情報の設定*/
	//座標の設定
	m_pos = pos;
	//移動方向の設定
	m_radius = rad;

	//待機フレーム数の設定
	m_waitFrame = wait;
	//描画フレーム数の設定
	m_drawFrame = draw;

	//サイズが小さくなるスピードの設定
	m_radiusSmallSpeed = speed;

	//存在フラグをtrueにする
	m_isExist = true;
}

void EnemyCircleScaling::Update()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		return;
	}

	//フレーム数を更新
	m_frame++;

	//予測線の点を回転させるためにangleを更新
	m_angle += DX_TWO_PI_F / kRotFrame;

	//本体の登場処理
	if (m_waitFrame <= m_frame && m_frame<= m_drawFrame)
	{
		//予測線の半径より小さいときははやく大きくなる
		if (m_mainCircleRadius <= m_radius)
		{
			m_mainCircleRadius += kAddCircle;
		}
		//予測線の半径よりも大きくなると少しずつ大きくなる
		else if (m_mainCircleRadius <= m_radius + m_drawFrame * kQuarter)
		{
			m_mainCircleRadius++;
		}
	}
	//本体が消える処理
	else
	{
		m_mainCircleRadius -= m_radiusSmallSpeed;
	}

	//本体の半径が0より小さくなると存在フラグをfalseにする
	if (m_mainCircleRadius < 0)
	{
		m_isExist = false;
	}

	//当たり判定の設定
	m_col.SetCircle(m_pos.x, m_pos.y, m_mainCircleRadius);
}

void EnemyCircleScaling::Draw()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		return;
	}

	//最初の点線の描画
	if (m_frame <= m_waitFrame)
	{
		for (int i = 0; i < m_radius; i++)
		{
			m_offset = { -cosf(m_angle + i) * m_radius , sinf(m_angle + i) * m_radius };

			DrawCircle(static_cast<int>(m_pos.x + m_offset.x), static_cast<int>(m_pos.y + m_offset.y), 1, 0x880088, true);
		}
	}
	else
	{
		//本体の描画
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			m_mainCircleRadius, m_defaultColor + m_addColor * (m_frame % kChangeColorIntervalFrame), true);
	}

#ifdef _DEBUG
	m_col.DrawColCircle(0xffffff);
#endif
}
