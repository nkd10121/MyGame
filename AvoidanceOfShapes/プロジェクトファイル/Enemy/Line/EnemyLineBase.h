#pragma once
#include "../../Util/Vec2.h"
#include "../../Util/Col.h"

/// <summary>
/// EnemyLine�n�̊��N���X
/// </summary>
/// 
class EnemyLineBase
{
public:
	EnemyLineBase();
	virtual ~EnemyLineBase();

	//�������z�֐�(�p����ŃI�[�o�[���[�h����)
	
	/// <summary>
	/// Line�p�̏������֐�
	/// </summary>
	/// <param name="pos">�\�����W</param>
	/// <param name="width">�G�̕�(�傫��)</param>
	virtual void Init(Vec2 pos, int width, int firstFrame, int secondFrame) {};

	/// <summary>
	/// Box�p�̏������֐�
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="size">�傫��</param>
	/// <param name="speed">�傫���Ȃ鑬��</param>
	/// <param name="firstFrame">�{�̂��`�悳���܂ł̃t���[��</param>
	/// <param name="secondFrame">�{�̂��������Ȃ�n�߂�t���[��</param>
	/// <param name="isPre">�\�����̗L���@true:�\��������,false:�\�����Ȃ�</param>
	virtual void Init(Vec2 pos, int size, int speed, int firstFrame, int secondFrame,bool isPre) {};
	void Init() {};
	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// �����蔻��̍��W���擾����
	/// </summary>
	/// <returns>�����蔻��̍��W</returns>
	Col GetColRect() const { return m_col; }

	int GetShakeFrame() { return m_shakeFrame; }

	/// <summary>
	/// ��ʓ��ɑ��݂��邩�̃t���O��Ԃ�
	/// </summary>
	/// <returns>���݂��Ă��邩�t���O</returns>
	bool isExist() const { return m_isExist; }

protected:
	//�E��̍��Wx,y
	Vec2 m_startPos;
	//�����̍��Wx,y
	Vec2 m_endPos;

	//�����蔻��̍��W
	Col m_col;

	int m_frame;
	int m_waitFrame;
	int m_disappearFrame;

	int m_color;

	int m_shakeFrame;
	bool m_isShake;

	bool m_isExist;
};

