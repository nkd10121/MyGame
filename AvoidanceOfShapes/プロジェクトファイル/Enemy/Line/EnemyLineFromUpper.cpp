#include "EnemyLineFromUpper.h"
#include "DxLib.h"
#include "Game.h"

EnemyLineFromUpper::EnemyLineFromUpper()
{
}

EnemyLineFromUpper::~EnemyLineFromUpper()
{
}

void EnemyLineFromUpper::Init(Vec2 pos, int width, int firstFrame, int secondFrame)
{
	m_isExist = true;

	//初期x座標の設定
	m_startPos.x = pos.x;
	m_endPos.x = m_startPos.x + width;

	//初期y座標の設定
	m_startPos.y = 0;
	m_endPos.y = 0;

	m_waitFrame = firstFrame;
	m_disappearFrame = secondFrame;

}

void EnemyLineFromUpper::Update()
{
	//もし存在しなかったらUpdateを実行しない
	if (!m_isExist)
		return;

	m_frame++;

	m_color = 0xff00ff;

	//secondTime以降(戻っていく)
	if (m_frame >= m_disappearFrame)
	{
		m_endPos.y -= 180;
	}
	//firstTime~secondTimeの間(本動作)
	else if (m_frame >= m_waitFrame)
	{
		m_endPos.y += 180;
		m_color = 0xffffff;
	}
	//0~firstTimeの間(予備動作的な)
	else if (m_frame >= 0)
	{
		m_endPos.y += 0.4f;
	}

	//敵座標の補正
	if (m_endPos.y >= Game::kScreenHeight)
	{
		m_endPos.y = Game::kScreenHeight;
		if (!m_isShake)
		{
			m_shakeFrame = 12;
			m_isShake = true;
		}
		m_color = 0xff00ff;
	}

	//敵座標の補正&isExistフラグの処理
	if (m_endPos.y <= 0)
	{
		m_endPos.y = 0;
		m_isExist = false;
	}

	if (m_shakeFrame != 0)
	{
		m_shakeFrame--;
	}


	//当たり判定の設定
	m_col.SetBox(m_startPos.x, m_startPos.y,
		m_endPos.x, m_endPos.y);
}

void EnemyLineFromUpper::Draw()
{
	//もし存在しなかったらUpdateを実行しない
	if (!m_isExist)
		return;

	//予測線的なのの描画
	if (m_frame <= m_disappearFrame)
	{
		DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
			static_cast<int>(m_endPos.x), Game::kScreenHeight,
			0x440044, true);
	}

	//本体の描画
	DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
		static_cast<int>(m_endPos.x), static_cast<int>(m_endPos.y),
		m_color, true);


#ifdef _DEBUG
	//DrawFormatString(0, 720 - 16, 0xffffff, "endPos.y : %d", endPos.y);
	m_col.DrawColBox(0xffffff);
#endif
}