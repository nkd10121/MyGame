#include "EnemyCircle.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	//Boxのサイズを出すときに4分の1にするための定数
	constexpr float kQuarter = 0.25f;
	//二分の一にするための定数
	constexpr float kHalf = 0.5f;
	//エフェクトの半径を小さくするための定数
	constexpr int kDelEfectRadius = 4;
	//移動する速さ
	constexpr int kMoveSpeedNum = 8;
	//回転する2つの四角の回転の差分
	constexpr float kAngleOffset = 0.75f;
}

EnemyCircle::EnemyCircle() :
	m_moveVec			(0.0f,0.0f),
	m_rad				(0),
	m_isSmooth			(false),
	m_angle				(0.0f),
	m_diagonal			(0),
	m_efectCirclePos	(0),
	m_effectCircleRad	(0)
{

}

EnemyCircle::~EnemyCircle()
{

}

void EnemyCircle::Init(float pos, int radius, bool isSmooth)
{
	//存在フラグをtrueにする
	m_isExist = true;

	//形の設定
	m_isSmooth = isSmooth;
	if (m_isSmooth)
	{
		m_rad = radius;
	}
	else
	{
		m_rad = radius;
		m_diagonal = m_rad + static_cast<int>(m_rad * kQuarter);
	}
	//登場時のエフェクトの半径を設定
	m_effectCircleRad = radius;

	//始点座標と終点座標を入れる
	m_startPos = { Game::kScreenWidth , pos};
	m_endPos = { 0 ,m_startPos.y };

	//座標の設定
	m_pos.x = m_startPos.x + m_rad;
	m_pos.y = m_startPos.y;

	//移動ベクトルを求める
	m_moveVec = m_endPos - m_startPos;

	//正規化
	m_moveVec = m_moveVec.getNormalize();

	m_efectCirclePos = static_cast<int>(m_pos.x) - m_rad;

}

void EnemyCircle::Update()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		return;
	}

	//登場時のエフェクトの半径をだんだん小さくする
	if (m_effectCircleRad > 0)
	{
		m_effectCircleRad -= kDelEfectRadius;
	}

	//移動処理
	m_pos += m_moveVec * kMoveSpeedNum;

	//画面外にでたら存在フラグをfalseにする
	if (0 >  m_pos.x + m_rad)
	{
		m_isExist = false;
	}

	//当たり判定の設定
	//円形の時
	if (m_isSmooth)
	{
		m_col.SetCircle(m_pos.x, m_pos.y, m_rad);
	}
	//とげとげ形の時
	else
	{
		m_col.SetCircle(m_pos.x, m_pos.y, m_rad + static_cast<int>(m_rad * kQuarter * kHalf));
		m_angle +=0.1f;
	}
}

void EnemyCircle::Draw()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
		return;

	//本体の描画
	//円形の時
	if (m_isSmooth)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad, 0xff00ff, true);
	}
	//とげとげ形の時
	else
	{
		//回転する四角の頂点座標を出すためのoffsetを計算する
		float offsetX1 = cosf(m_angle) * m_diagonal;
		float offsetY1 = sinf(m_angle) * m_diagonal;

		float offsetX2 = cosf(m_angle + kAngleOffset) * m_diagonal;
		float offsetY2 = sinf(m_angle + kAngleOffset) * m_diagonal;

		//回転する四角を描画する
		DrawQuadrangle(
			static_cast<int>(m_pos.x - offsetX2), static_cast<int>(m_pos.y - offsetY2),
			static_cast<int>(m_pos.x + offsetY2), static_cast<int>(m_pos.y - offsetX2),
			static_cast<int>(m_pos.x + offsetX2), static_cast<int>(m_pos.y + offsetY2),
			static_cast<int>(m_pos.x - offsetY2), static_cast<int>(m_pos.y + offsetX2),
			0xff00ff,
			true
		);

		//回転する四角を描画する
		DrawQuadrangle(
			static_cast<int>(m_pos.x + offsetX1), static_cast<int>(m_pos.y + offsetY1),
			static_cast<int>(m_pos.x + offsetY1), static_cast<int>(m_pos.y - offsetX1),
			static_cast<int>(m_pos.x - offsetX1), static_cast<int>(m_pos.y - offsetY1),
			static_cast<int>(m_pos.x - offsetY1), static_cast<int>(m_pos.y + offsetX1),
			0xff00ff,
			true);

		//円の本体を描画する
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad, 0xff00ff, true);
	}

	//エフェクトの描画
	if (m_effectCircleRad >= 0)
	{
		DrawCircle(m_efectCirclePos, static_cast<int>(m_pos.y), m_effectCircleRad, 0xffffff, true);
	}

#ifdef _DEBUG
	m_col.DrawColCircle(0xffffff);
#endif
}
