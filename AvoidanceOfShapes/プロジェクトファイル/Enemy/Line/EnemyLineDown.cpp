#include "EnemyLineDown.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	//元の色
	constexpr int kDefaultColor = 0xff00ff;
	//白
	constexpr int kWhiteColor = 0xffffff;
	//本体の移動速度
	constexpr int kMoveSpeed = 180;
	//本体の予備動作の移動速度
	constexpr float kPreMoveSpeed = 0.4f;
	//画面を揺らす時間(フレーム数)
	constexpr int kShakeFrame = 12;
}


EnemyLineDown::EnemyLineDown()
{
}

EnemyLineDown::~EnemyLineDown()
{
}


void EnemyLineDown::Init(Vec2 pos, int width, int firstFrame, int secondFrame)
{
	m_isExist = true;

	//初期x座標の設定
	m_startPos.x = pos.x;
	m_endPos.x = m_startPos.x + width;

	//初期y座標の設定
	m_startPos.y = Game::kScreenHeight;
	m_endPos.y = Game::kScreenHeight;

	m_waitFrame = firstFrame;
	m_disappearFrame = secondFrame;

}

void EnemyLineDown::Update()
{
	//もし存在しなかったらUpdateを実行しない
	if (!m_isExist)
		return;

	m_frame++;

	m_color = kDefaultColor;


	//secondTime以降(戻っていく)
	if (m_frame >= m_disappearFrame)
	{
		m_endPos.y += kMoveSpeed;
	}
	//firstTime~secondTimeの間(本動作)
	else if (m_frame >= m_waitFrame)
	{
		m_endPos.y -= kMoveSpeed;
		m_color = kWhiteColor;
	}
	//0~firstTimeの間(予備動作的な)
	else if (m_frame >= 0)
	{
		m_endPos.y -= kPreMoveSpeed;
	}

	//敵座標の補正&isExistフラグの処理
	if (m_endPos.y >= Game::kScreenHeight)
	{
		m_endPos.y = Game::kScreenHeight;
		m_isExist = false;
	}

	//敵座標の補正
	if (m_endPos.y <= 0)
	{
		m_endPos.y = 0;
		if (!m_isShake)
		{
			m_shakeFrame = kShakeFrame;
			m_isShake = true;
		}
		m_color = kDefaultColor;
	}

	//画面を揺らすフレームが0じゃない(以上)なら減らす
	if (m_shakeFrame != 0)
	{
		m_shakeFrame--;
	}

	//当たり判定の設定
	m_col.SetBox(m_startPos.x, m_endPos.y,
		m_endPos.x, m_startPos.y);
}

void EnemyLineDown::Draw()
{
	//もし存在しなかったらUpdateを実行しない
	if (!m_isExist)
		return;

	//予測線的なのの描画
	if (m_frame <= m_disappearFrame)
	{
		DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
			static_cast<int>(m_endPos.x), 0,
			0x440044, true);
	}

	//本体の描画
	DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
		static_cast<int>(m_endPos.x), static_cast<int>(m_endPos.y),
		0xff00ff, true);


#ifdef _DEBUG
	//DrawFormatString(0, 720 - 16, 0xffffff, "endPos.y : %d", endPos.y);
	m_col.DrawColBox(0xffffff);
#endif
}

