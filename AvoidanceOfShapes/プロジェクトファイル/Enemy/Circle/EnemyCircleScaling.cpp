#include "EnemyCircleScaling.h"

namespace
{
	//�\�������������̂ɂ�����t���[��
	constexpr int kRotFrame = 60;
	//���a��傫�����邽�߂̒萔
	constexpr int kAddCircle = 12;
	//4����1�ɂ��邽�߂̒萔
	constexpr float kQuarter = 0.25f;
	//�_�ł���Ԋu�̃t���[��
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
	/*�G�̏��̐ݒ�*/
	//���W�̐ݒ�
	m_pos = pos;
	//�ړ������̐ݒ�
	m_radius = rad;

	//�ҋ@�t���[�����̐ݒ�
	m_waitFrame = wait;
	//�`��t���[�����̐ݒ�
	m_drawFrame = draw;

	//�T�C�Y���������Ȃ�X�s�[�h�̐ݒ�
	m_radiusSmallSpeed = speed;

	//���݃t���O��true�ɂ���
	m_isExist = true;
}

void EnemyCircleScaling::Update()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		return;
	}

	//�t���[�������X�V
	m_frame++;

	//�\�����̓_����]�����邽�߂�angle���X�V
	m_angle += DX_TWO_PI_F / kRotFrame;

	//�{�̂̓o�ꏈ��
	if (m_waitFrame <= m_frame && m_frame<= m_drawFrame)
	{
		//�\�����̔��a��菬�����Ƃ��͂͂₭�傫���Ȃ�
		if (m_mainCircleRadius <= m_radius)
		{
			m_mainCircleRadius += kAddCircle;
		}
		//�\�����̔��a�����傫���Ȃ�Ə������傫���Ȃ�
		else if (m_mainCircleRadius <= m_radius + m_drawFrame * kQuarter)
		{
			m_mainCircleRadius++;
		}
	}
	//�{�̂������鏈��
	else
	{
		m_mainCircleRadius -= m_radiusSmallSpeed;
	}

	//�{�̂̔��a��0��菬�����Ȃ�Ƒ��݃t���O��false�ɂ���
	if (m_mainCircleRadius < 0)
	{
		m_isExist = false;
	}

	//�����蔻��̐ݒ�
	m_col.SetCircle(m_pos.x, m_pos.y, m_mainCircleRadius);
}

void EnemyCircleScaling::Draw()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		return;
	}

	//�ŏ��̓_���̕`��
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
		//�{�̂̕`��
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
			m_mainCircleRadius, m_defaultColor + m_addColor * (m_frame % kChangeColorIntervalFrame), true);
	}

#ifdef _DEBUG
	m_col.DrawColCircle(0xffffff);
#endif
}
