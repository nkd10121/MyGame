#include "DrawRotationCirlce.h"
#include "DxLib.h"

namespace
{
	//�v���C���[�̎�������]����̂ɂ�����t���[����
	constexpr int kRotFrame = 44;

	//�o���Ă����ߋ��̈ʒu���̐�
	constexpr int kPosLogNum = 8;
}

DrawRotationCirlce::DrawRotationCirlce() :
	m_center(0, 0),
	m_pos1(0, 0),
	m_pos2(0, 0),
	m_frameCount(0),
	m_angle(0),
	m_circleRadius(2),
	m_rotateRadius(0),
	m_isWhite(false),
	m_isClockwise(true)
{
	m_posLog1.resize(kPosLogNum);
	m_posLog2.resize(kPosLogNum);
}

DrawRotationCirlce::~DrawRotationCirlce()
{
}

void DrawRotationCirlce::Init(int Rr, bool clockwise, bool white)
{
	m_rotateRadius = Rr;
	m_isClockwise = clockwise;
	m_isWhite = white;
}

void DrawRotationCirlce::Update(Vec2 pos)
{
	m_center = pos;

	//�c���̃��O�����炷
	for (int i = static_cast<int>(m_posLog1.size()) - 1; i >= 1; i--)
	{
		m_posLog1[i] = m_posLog1[i - 1];
	}
	m_posLog1[0] = m_pos1;

	for (int i = static_cast<int>(m_posLog2.size()) - 1; i >= 1; i--)
	{
		m_posLog2[i] = m_posLog2[i - 1];
	}
	m_posLog2[0] = m_pos2;

	//�p�x�̍X�V
	//kRotFrame�����Ĉ��]����
	m_angle += DX_TWO_PI_F / kRotFrame;

	Vec2 offset1;	//�v���C���[���猩���ʒu
	Vec2 offset2;	//�v���C���[���猩���ʒu

	if (m_isClockwise)
	{
		offset1.x = cosf(m_angle) * m_rotateRadius;
		offset1.y = sinf(m_angle) * m_rotateRadius;

		offset2.x = -cosf(m_angle) * m_rotateRadius;
		offset2.y = -sinf(m_angle) * m_rotateRadius;
	}
	else
	{
		offset1.x = cosf(m_angle) * m_rotateRadius;
		offset1.y = -sinf(m_angle) * m_rotateRadius;

		offset2.x = -cosf(m_angle) * m_rotateRadius;
		offset2.y = sinf(m_angle) * m_rotateRadius;
	}

	m_pos1 = m_center + offset1;
	m_pos2 = m_center + offset2;
}

void DrawRotationCirlce::Draw()
{

	//�c���̏���1
	for (int i = 0; i < m_posLog1.size(); i++)
	{
		int alpha = 255 - (20 * (i + 1));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		if (m_isWhite)
		{
			DrawCircle(static_cast<int>(m_posLog1[i].x), static_cast<int>(m_posLog1[i].y), m_circleRadius, 0xffffff, true);
		}
		else
		{
			DrawCircle(static_cast<int>(m_posLog1[i].x), static_cast<int>(m_posLog1[i].y), m_circleRadius, 0x00ffff, true);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	//�c���̏���2
	for (int i = 0; i < m_posLog2.size(); i++)
	{
		int alpha = 255 - (20 * (i + 1));
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

		if (m_isWhite)
		{
			DrawCircle(static_cast<int>(m_posLog2[i].x), static_cast<int>(m_posLog2[i].y), m_circleRadius, 0xffffff, true);
		}
		else
		{
			DrawCircle(static_cast<int>(m_posLog2[i].x), static_cast<int>(m_posLog2[i].y), m_circleRadius, 0x00ffff, true);
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}

	if (m_isWhite)
	{
		DrawCircle(static_cast<int>(m_pos1.x), static_cast<int>(m_pos1.y), m_circleRadius, 0xffffff, true);
		DrawCircle(static_cast<int>(m_pos2.x), static_cast<int>(m_pos2.y), m_circleRadius, 0xffffff, true);
	}
	else
	{
		DrawCircle(static_cast<int>(m_pos1.x), static_cast<int>(m_pos1.y), m_circleRadius, 0x00ffff, true);
		DrawCircle(static_cast<int>(m_pos2.x), static_cast<int>(m_pos2.y), m_circleRadius, 0x00ffff, true);
	}
}
