#include "EnemyCircle.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	//Box�̃T�C�Y���o���Ƃ���4����1�ɂ��邽�߂̒萔
	constexpr float kQuarter = 0.25f;
	//�񕪂̈�ɂ��邽�߂̒萔
	constexpr float kHalf = 0.5f;
	//�G�t�F�N�g�̔��a�����������邽�߂̒萔
	constexpr int kDelEfectRadius = 4;
	//�ړ����鑬��
	constexpr int kMoveSpeedNum = 8;
	//��]����2�̎l�p�̉�]�̍���
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
	//���݃t���O��true�ɂ���
	m_isExist = true;

	//�`�̐ݒ�
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
	//�o�ꎞ�̃G�t�F�N�g�̔��a��ݒ�
	m_effectCircleRad = radius;

	//�n�_���W�ƏI�_���W������
	m_startPos = { Game::kScreenWidth , pos};
	m_endPos = { 0 ,m_startPos.y };

	//���W�̐ݒ�
	m_pos.x = m_startPos.x + m_rad;
	m_pos.y = m_startPos.y;

	//�ړ��x�N�g�������߂�
	m_moveVec = m_endPos - m_startPos;

	//���K��
	m_moveVec = m_moveVec.getNormalize();

	m_efectCirclePos = static_cast<int>(m_pos.x) - m_rad;

}

void EnemyCircle::Update()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		return;
	}

	//�o�ꎞ�̃G�t�F�N�g�̔��a�����񂾂񏬂�������
	if (m_effectCircleRad > 0)
	{
		m_effectCircleRad -= kDelEfectRadius;
	}

	//�ړ�����
	m_pos += m_moveVec * kMoveSpeedNum;

	//��ʊO�ɂł��瑶�݃t���O��false�ɂ���
	if (0 >  m_pos.x + m_rad)
	{
		m_isExist = false;
	}

	//�����蔻��̐ݒ�
	//�~�`�̎�
	if (m_isSmooth)
	{
		m_col.SetCircle(m_pos.x, m_pos.y, m_rad);
	}
	//�Ƃ��Ƃ��`�̎�
	else
	{
		m_col.SetCircle(m_pos.x, m_pos.y, m_rad + static_cast<int>(m_rad * kQuarter * kHalf));
		m_angle +=0.1f;
	}
}

void EnemyCircle::Draw()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
		return;

	//�{�̂̕`��
	//�~�`�̎�
	if (m_isSmooth)
	{
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad, 0xff00ff, true);
	}
	//�Ƃ��Ƃ��`�̎�
	else
	{
		//��]����l�p�̒��_���W���o�����߂�offset���v�Z����
		float offsetX1 = cosf(m_angle) * m_diagonal;
		float offsetY1 = sinf(m_angle) * m_diagonal;

		float offsetX2 = cosf(m_angle + kAngleOffset) * m_diagonal;
		float offsetY2 = sinf(m_angle + kAngleOffset) * m_diagonal;

		//��]����l�p��`�悷��
		DrawQuadrangle(
			static_cast<int>(m_pos.x - offsetX2), static_cast<int>(m_pos.y - offsetY2),
			static_cast<int>(m_pos.x + offsetY2), static_cast<int>(m_pos.y - offsetX2),
			static_cast<int>(m_pos.x + offsetX2), static_cast<int>(m_pos.y + offsetY2),
			static_cast<int>(m_pos.x - offsetY2), static_cast<int>(m_pos.y + offsetX2),
			0xff00ff,
			true
		);

		//��]����l�p��`�悷��
		DrawQuadrangle(
			static_cast<int>(m_pos.x + offsetX1), static_cast<int>(m_pos.y + offsetY1),
			static_cast<int>(m_pos.x + offsetY1), static_cast<int>(m_pos.y - offsetX1),
			static_cast<int>(m_pos.x - offsetX1), static_cast<int>(m_pos.y - offsetY1),
			static_cast<int>(m_pos.x - offsetY1), static_cast<int>(m_pos.y + offsetX1),
			0xff00ff,
			true);

		//�~�̖{�̂�`�悷��
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_rad, 0xff00ff, true);
	}

	//�G�t�F�N�g�̕`��
	if (m_effectCircleRad >= 0)
	{
		DrawCircle(m_efectCirclePos, static_cast<int>(m_pos.y), m_effectCircleRad, 0xffffff, true);
	}

#ifdef _DEBUG
	m_col.DrawColCircle(0xffffff);
#endif
}
