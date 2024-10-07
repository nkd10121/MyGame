#pragma once
#include "EnemyCircleBase.h"

class EnemyCircleScaling : public EnemyCircleBase
{
public:
	EnemyCircleScaling();
	~EnemyCircleScaling();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rad">�傫��</param>
	/// <param name="wait">�ҋ@����</param>
	/// <param name="draw">�`�掞��</param>
	/// <param name="speed">�������Ȃ鑬��</param>
	void Init(Vec2 pos, int rad,int wait,int draw,int speed)override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private:
	//�t���[����
	int m_frame;
	//�\�����̉�]�p�x
	float m_angle;
	//�\�����̒��S����̋���
	Vec2 m_offset;
	
	//�{�̂��`�悳���܂ł̃t���[����
	int m_waitFrame;
	//�{�̂��`�悳��ď����n�߂�܂ł̃t���[����
	int m_drawFrame;

	//�\�����̔��a
	int m_radius;
	//�{�̂̔��a
	int m_mainCircleRadius;

	//�{�̂��������Ȃ鑬��
	int m_radiusSmallSpeed;

	//�ʏ�̐F(�s���N)
	unsigned int m_defaultColor;
	//�ʏ�̐F�ɉ��Z����F(��)
	unsigned int m_addColor;
};

