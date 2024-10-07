#pragma once
#include "EnemyCircleBase.h"

class EnemyCircle : public EnemyCircleBase
{
public:
	EnemyCircle();
	~EnemyCircle();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="radius">���a</param>
	/// <param name="isSmooth">�` true:�~�Afalse:�Ƃ��Ƃ�</param>
	void Init(float pos, int radius, bool isSmooth)override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw()override;

private:
	//�ړ��x�N�g��
	Vec2 m_moveVec;

	//�{�̂̔��a
	int m_rad;

	//�~���Ƃ��Ƃ���
	//true:�~�Afalse:�Ƃ��Ƃ�
	bool m_isSmooth;

	//��]����l�p�̉�]�p�x
	float m_angle;
	//��]����l�p�̒��S���璸�_�܂ł̋���
	int m_diagonal;

	//�o�ꎞ�̃G�t�F�N�g�̍��W
	int m_efectCirclePos;
	//�o�ꎞ�̃G�t�F�N�g�̔��a
	int m_effectCircleRad;
};