#pragma once
#include "../Line/EnemyLineBase.h"
#include "DxLib.h"

class EnemyBurstedCircle : public EnemyLineBase
{
public:
	EnemyBurstedCircle();
	~EnemyBurstedCircle();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="vec">�ړ�����</param>
	void Init(Vec2 pos,Vec2 vec);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private:
	//���ݍ��W
	Vec2 m_pos;

	//�ړ�����
	Vec2 m_vec;

};