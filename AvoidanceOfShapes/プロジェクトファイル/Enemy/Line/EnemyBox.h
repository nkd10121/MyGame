#pragma once
#include "EnemyLineBase.h"
class EnemyBox : public EnemyLineBase
{
public:
	EnemyBox();
	~EnemyBox();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="size">�ő�̑傫��</param>
	/// <param name="speed">�傫���Ȃ鑬��</param>
	/// <param name="drawFrame">�{�̂��`�悳���܂ł̃t���[��</param>
	/// <param name="smallFrame">�{�̂��������Ȃ�n�߂�t���[��</param>
	/// <param name="isPre">�\�����̗L���@true:�\��������,false:�\�����Ȃ�</param>
	void Init(Vec2 pos, int size, int speed, int drawFrame, int smallFrame,bool isPre)override;

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update()override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw()override;

private:
	//���݂̒��S���W
	Vec2 m_pos;
	//�傫���̍ő�l
	int m_sizeMax;
	//���݂̑傫��
	int m_size;
	//�傫���Ȃ鑬��
	int m_growSizeSpeed;
	//�{�̂̐F
	unsigned int m_color;

	//���݂̑傫�����傫���̍ő�l�Ɠ���������ȏ�ɂȂ������ǂ���
	bool m_isMax;
	//�\�����̕`��̗L��
	bool m_isDrawPre;
};

