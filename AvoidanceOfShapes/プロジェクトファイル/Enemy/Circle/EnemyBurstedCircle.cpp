#include "EnemyBurstedCircle.h"
#include "Game.h"

namespace
{
	//�{�̂̈ړ��X�s�[�h
	constexpr int kSpeed = 8;
	//���a�@�Ȃ���12�ȉ��ɂ���Ɠ����鎞�Ɠ�����Ȃ��Ƃ�������
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
	//���݃t���O��true�ɂ���
	m_isExist = true;

	/*�G�̏��̐ݒ�*/
	//���W�̐ݒ�
	m_pos = pos;
	//�ړ������̐ݒ�
	m_vec = vec;
}

void EnemyBurstedCircle::Update()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		return;
	} 

	//�ړ�����
	m_pos += m_vec.getNormalize() * kSpeed;

	//��ʊO�ɏo���Ƃ��A���݃t���O��false�ɂ���
	if (m_pos.x > Game::kScreenWidth || m_pos.x < 0 || m_pos.y > Game::kScreenHeight || m_pos.y < 0)
		m_isExist = false;

	//�����蔻��̐ݒ�
	m_col.SetCircle(m_pos.x, m_pos.y, kRadius);

}

void EnemyBurstedCircle::Draw()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		return;
	}

	//�{�̂̕`��
	DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), kRadius, 0xff00ff, true);

#ifdef _DEBUG
	//m_colRect.DrawRectCircle(0xffffff, false);
	m_col.DrawColBox(0xffffff);
#endif
}
