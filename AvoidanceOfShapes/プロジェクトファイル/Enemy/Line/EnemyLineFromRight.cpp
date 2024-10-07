#include "EnemyLineFromRight.h"
#include "DxLib.h"
#include "Game.h"

EnemyLineFromRight::EnemyLineFromRight()
{
}

EnemyLineFromRight::~EnemyLineFromRight()
{
}

void EnemyLineFromRight::Init(Vec2 pos, int width, int firstFrame, int secondFrame)
{
	m_isExist = true;

	//����x���W�̐ݒ�
	m_startPos.x = Game::kScreenWidth;
	m_endPos.x = Game::kScreenWidth;

	//����y���W�̐ݒ�
	m_startPos.y = pos.y;
	m_endPos.y = m_startPos.y + width;

	m_waitFrame = firstFrame;
	m_disappearFrame = secondFrame;

}

void EnemyLineFromRight::Update()
{
	//�������݂��Ȃ�������Update�����s���Ȃ�
	if (!m_isExist)
		return;

	m_frame++;

	m_color = 0xff00ff;

	//secondTime�ȍ~(�߂��Ă���)
	if (m_frame >= m_disappearFrame)
	{
		m_endPos.x += 320;
	}
	//firstTime~secondTime�̊�(�{����)
	else if (m_frame >= m_waitFrame )
	{
		m_endPos.x -= 320;
		m_color = 0xffffff;
	}
	//0~firstTime�̊�(�\������I��)
	else if (m_frame >= 0)
	{
		m_endPos.x -= 0.4f;
	}


	//�G���W�̕␳&isExist�t���O�̏���
	if (m_endPos.x >= Game::kScreenWidth)
	{
		m_endPos.x = Game::kScreenWidth;
		m_isExist = false;
	}

	//�G���W�̕␳
	if (m_endPos.x <= 0)
	{
		m_endPos.x = 0;
		if (!m_isShake)
		{
 			m_shakeFrame = 12;
			m_isShake = true;
		}
		m_color = 0xff00ff;
	}

	if (m_shakeFrame != 0)
	{
		m_shakeFrame--;
	}

	//�����蔻��̐ݒ�
	m_col.SetBox(m_endPos.x, m_startPos.y,
		m_startPos.x, m_endPos.y);
}

void EnemyLineFromRight::Draw()
{
	//�������݂��Ȃ�������Update�����s���Ȃ�
	if (!m_isExist)
		return;

	//�\�����I�Ȃ̂̕`��
	if (m_frame <= m_disappearFrame)
	{
		DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
			0, static_cast<int>(m_endPos.y),
			0x440044, true);
	}

	//�{�̂̕`��
	DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
		static_cast<int>(m_endPos.x), static_cast<int>(m_endPos.y),
		0xff00ff, true);


#ifdef _DEBUG
	//DrawFormatString(0, 720 - 16, 0xffffff, "endPos.y : %d", endPos.y);
	m_col.DrawColBox(0xffffff);
#endif
}
