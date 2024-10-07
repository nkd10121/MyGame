#include "EnemyBurstedCircle.h"
#include "Game.h"

namespace
{
	//本体の移動スピード
	constexpr int kSpeed = 8;
	//半径　なぜか12以下にすると当たる時と当たらないときがある
	constexpr int kRadius = 6;
}


EnemyBurstedCircle::EnemyBurstedCircle() :
	m_pos	(0.0f,0.0f),
	m_vec	(0.0f,0.0f)
{
}

EnemyBurstedCircle::~EnemyBurstedCircle()
{
}

void EnemyBurstedCircle::Init(Vec2 pos, Vec2 vec)
{
	//存在フラグをtrueにする
	m_isExist = true;

	/*敵の情報の設定*/
	//座標の設定
	m_pos = pos;
	//移動方向の設定
	m_vec = vec;
}

void EnemyBurstedCircle::Update()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		return;
	} 

	//移動処理
	m_pos += m_vec.getNormalize() * kSpeed;

	//画面外に出たとき、存在フラグをfalseにする
	if (m_pos.x > Game::kScreenWidth || m_pos.x < 0 || m_pos.y > Game::kScreenHeight || m_pos.y < 0)
		m_isExist = false;

	//当たり判定の設定
	m_col.SetCircle(m_pos.x, m_pos.y, kRadius);

}

void EnemyBurstedCircle::Draw()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		return;
	}

	//本体の描画
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), kRadius, 0xff00ff, true);

#ifdef _DEBUG
	//m_colRect.DrawRectCircle(0xffffff, false);
	m_col.DrawColBox(0xffffff);
#endif
}
