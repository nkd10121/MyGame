#include "EnemyLineDown.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	//���̐F
	constexpr int kDefaultColor = 0xff00ff;
	//��
	constexpr int kWhiteColor = 0xffffff;
	//�{�̂̈ړ����x
	constexpr int kMoveSpeed = 180;
	//�{�̗̂\������̈ړ����x
	constexpr float kPreMoveSpeed = 0.4f;
	//��ʂ�h�炷����(�t���[����)
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

	//����x���W�̐ݒ�
	m_startPos.x = pos.x;
	m_endPos.x = m_startPos.x + width;

	//����y���W�̐ݒ�
	m_startPos.y = Game::kScreenHeight;
	m_endPos.y = Game::kScreenHeight;

	m_waitFrame = firstFrame;
	m_disappearFrame = secondFrame;

}

void EnemyLineDown::Update()
{
	//�������݂��Ȃ�������Update�����s���Ȃ�
	if (!m_isExist)
		return;

	m_frame++;

	m_color = kDefaultColor;


	//secondTime�ȍ~(�߂��Ă���)
	if (m_frame >= m_disappearFrame)
	{
		m_endPos.y += kMoveSpeed;
	}
	//firstTime~secondTime�̊�(�{����)
	else if (m_frame >= m_waitFrame)
	{
		m_endPos.y -= kMoveSpeed;
		m_color = kWhiteColor;
	}
	//0~firstTime�̊�(�\������I��)
	else if (m_frame >= 0)
	{
		m_endPos.y -= kPreMoveSpeed;
	}

	//�G���W�̕␳&isExist�t���O�̏���
	if (m_endPos.y >= Game::kScreenHeight)
	{
		m_endPos.y = Game::kScreenHeight;
		m_isExist = false;
	}

	//�G���W�̕␳
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

	//��ʂ�h�炷�t���[����0����Ȃ�(�ȏ�)�Ȃ猸�炷
	if (m_shakeFrame != 0)
	{
		m_shakeFrame--;
	}

	//�����蔻��̐ݒ�
	m_col.SetBox(m_startPos.x, m_endPos.y,
		m_endPos.x, m_startPos.y);
}

void EnemyLineDown::Draw()
{
	//�������݂��Ȃ�������Update�����s���Ȃ�
	if (!m_isExist)
		return;

	//�\�����I�Ȃ̂̕`��
	if (m_frame <= m_disappearFrame)
	{
		DrawBox(static_cast<int>(m_startPos.x), static_cast<int>(m_startPos.y),
			static_cast<int>(m_endPos.x), 0,
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

