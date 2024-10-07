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

	//����x���W�̐ݒ�
	m_startPos.x = pos.x;
	m_endPos.x = m_startPos.x + width;

	//����y���W�̐ݒ�
	m_startPos.y = 0;
	m_endPos.y = 0;

	m_waitFrame = firstFrame;
	m_disappearFrame = secondFrame;

}

void EnemyLineFromUpper::Update()
{
	//�������݂��Ȃ�������Update�����s���Ȃ�
	if (!m_isExist)
		return;

	m_frame++;

	m_color = 0xff00ff;

	//secondTime�ȍ~(�߂��Ă���)
	if (m_frame >= m_disappearFrame)
	{
		m_endPos.y -= 180;
	}
	//firstTime~secondTime�̊�(�{����)
	else if (m_frame >= m_waitFrame)
	{
		m_endPos.y += 180;
		m_color = 0xffffff;
	}
	//0~firstTime�̊�(�\������I��)
	else if (m_frame >= 0)
	{
		m_endPos.y += 0.4f;
	}

	//�G���W�̕␳
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

	//�G���W�̕␳&isExist�t���O�̏���
	if (m_endPos.y <= 0)
	{
		m_endPos.y = 0;
		m_isExist = false;
	}

	if (m_shakeFrame != 0)
	{
		m_shakeFrame--;
	}


	//�����蔻��̐ݒ�
	m_col.SetBox(m_startPos.x, m_startPos.y,
		m_endPos.x, m_endPos.y);
}

void EnemyLineFromUpper::Draw()
{
	//�������݂��Ȃ�������Update�����s���Ȃ�
	if (!m_isExist)
		return;

	//�\�����I�Ȃ̂̕`��
	if (m_frame <= m_disappearFrame)
	{
		DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
			static_cast<int>(m_endPos.x), Game::kScreenHeight,
			0x440044, true);
	}

	//�{�̂̕`��
	DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
		static_cast<int>(m_endPos.x), static_cast<int>(m_endPos.y),
		m_color, true);


#ifdef _DEBUG
	//DrawFormatString(0, 720 - 16, 0xffffff, "endPos.y : %d", endPos.y);
	m_col.DrawColBox(0xffffff);
#endif
}