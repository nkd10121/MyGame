#include "EnemyBurst.h"
#include "EnemyBurstedCircle.h"
#include "StageBase.h"

namespace
{
	//�j�􂵂���̋��̐�
	constexpr int kBurstedCircleNum = 8;

	//�����
	const Vec2 kUp = { 0.0f,1.0f };
	//�E����
	const Vec2 kRight = { 1.0f,0.0f };
	//������
	const Vec2 kDown = { 0.0f,-1.0f };
	//������
	const Vec2 kLeft = { -1.0f,0.0f };
	//��]�����邽�߂ɖ��t���[��angle�ɑ����萔
	constexpr float kAddAngle = 0.14f;
	//�_�ł���Ԋu�̃t���[��
	constexpr int kChangeColorIntervalFrame = 4;
	//�_�ł����n�߂�t���[��
	constexpr int kStartChangeColerFrame = 24;
	//rad�ɑ�����
	constexpr int kAddRadius = 2;
	//Box�̃T�C�Y���o���Ƃ���4����1�ɂ��邽�߂̒萔
	constexpr float kQuarter = 0.25f;
	//speed���疈�t���[��������
	constexpr float kDelSpeed = 0.8f;
	//�����蔻��̍���
	constexpr int kColOffset = 4;
}

EnemyBurst::EnemyBurst(StageBase* scene):
	m_pStage	(scene),
	m_frame		(0),
	m_bigFrame	(0),
	m_moveFrame	(0),
	m_offset	(0.0f,0.0f),
	m_angle		(0.0f),
	m_radius	(4),
	m_growSpeed		(0.0f),
	m_diagonal	(0.0f),
	m_vec		(static_cast<float>(GetRand(1280) - 640), static_cast<float>(GetRand(960) - 480)),
	m_isBurst	(false),
	m_circlePos	(0.0f,0.0f)
{
}

EnemyBurst::~EnemyBurst()
{
}

void EnemyBurst::Init(Vec2 pos, Vec2 vec, int speed, int firstFrame, int secondFrame)
{
	/*�G�̏��̐ݒ�*/
	//���W�̐ݒ�
	m_pos = pos;
	//�ړ������̐ݒ�
	m_vec = vec;
	//�ړ����x�̐ݒ�
	m_growSpeed = static_cast<float>(speed);

	//�傫���Ȃ�t���[�����̐ݒ�
	m_bigFrame = firstFrame;
	//�ړ�����t���[�����̐ݒ�
	m_moveFrame = secondFrame;

	//���݃t���O��true�ɂ���
	m_isExist = true;
	
}

void EnemyBurst::Update()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		//�������^�[��
		return;
	}

	//�t���[�����𑪂�
	m_frame++;

	//��]�����邽�߂�angle�𑝂₵������
	m_angle += kAddAngle;

	//�j�􂷂�^�C�~���O���킩��悤�ɐF��_�ł�����
	if (m_frame % kChangeColorIntervalFrame == 0 && m_frame > m_moveFrame - kStartChangeColerFrame)
	{
		//��u�������ɂ���
		m_color = 0xffffff;
	}
	else
	{
		//���̂ق��̎��͒ʏ�̐F�ɂ���
		m_color = 0xff00ff;
	}

	//�ŏ��ɐݒ肵���t���[���𒴂���܂�
	if (m_frame <= m_bigFrame)
	{
		//�傫���Ȃ�
		m_radius += kAddRadius;
		//��]����
		m_diagonal = static_cast<float>(m_radius) + (static_cast<float>(m_radius) * kQuarter);
	}

	//�ړ�����t���[���𒴂���܂ňړ�����
	if (m_frame <= m_moveFrame)
	{
		//�ړ�������
		m_pos += m_vec.getNormalize() * m_growSpeed;

		//�����蔻��̐ݒ�
		m_col.SetCircle(m_pos.x,m_pos.y, m_radius);

		//�l�p��xy���ꂼ��̍��W���o�����߂̌v�Z
		m_offset.x = cosf(m_angle) * m_diagonal;
		m_offset.y = sinf(m_angle) * m_diagonal;
	}
	//��̏����ȊO�̃^�C�~���O�ł܂��j�􂵂Ă��Ȃ��Ƃ�
	else if (!m_isBurst)
	{
		//�j��t���O��true�ɂ���
		m_isBurst = true;

		//�j���̓G�ɍ��W��n����
		m_circlePos = m_pos;

		//8�����Ɉړ�����G�𐶐�����
		CreateBurstedCircle(m_circlePos, kUp);
		CreateBurstedCircle(m_circlePos, kUp + kRight);
		CreateBurstedCircle(m_circlePos, kRight);
		CreateBurstedCircle(m_circlePos, kRight + kDown);
		CreateBurstedCircle(m_circlePos, kDown);
		CreateBurstedCircle(m_circlePos, kDown + kLeft);
		CreateBurstedCircle(m_circlePos, kLeft);
		CreateBurstedCircle(m_circlePos, kLeft + kUp);
	}

	//������Ƃ��ړ��X�s�[�h��������
	m_growSpeed -= kDelSpeed;
	//0��菬�����Ȃ�����0�ɕ␳����
	if (m_growSpeed < 0)
	{
		m_growSpeed = 0.0f;
	}

	//�w�肵���t���[�����߂���Ƒ��݃t���O��false�ɂ���
	if (m_frame > m_moveFrame)
	{
		m_isExist = false;
	}

	//�����蔻��̐ݒ�
	m_col.SetCircle(m_pos.x, m_pos.y, m_radius + kColOffset);
}

void EnemyBurst::Draw()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		return;
	}

	//�ړ��t���[���𒴂���܂ł͖{�̂�`�悷��
	if (m_frame <= m_moveFrame)
	{
		//�{�̂̎l�p�̕����̕`��
		DrawQuadrangle(
			static_cast<int>(m_pos.x - m_offset.x), static_cast<int>(m_pos.y - m_offset.y),
			static_cast<int>(m_pos.x + m_offset.y), static_cast<int>(m_pos.y - m_offset.x),
			static_cast<int>(m_pos.x + m_offset.x), static_cast<int>(m_pos.y + m_offset.y),
			static_cast<int>(m_pos.x - m_offset.y), static_cast<int>(m_pos.y + m_offset.x),
			m_color,
			true
		);

		//�{�̂̉~�̕����̕`��
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_radius, m_color, true);

#ifdef _DEBUG
		//m_colRect.DrawRectCircle(0xffffff, false);
		m_col.DrawColCircle(0xffffff);
#endif
	}
}

void EnemyBurst::CreateBurstedCircle(Vec2 pos, Vec2 vec)
{
	//�����ō�邾�������Update��Draw��GamePlayingScene�ɓn���Ă��܂�
	EnemyBurstedCircle* enemy = new EnemyBurstedCircle;
	enemy->Init(pos, vec);
	m_pStage->AddEnemy(enemy);
}
