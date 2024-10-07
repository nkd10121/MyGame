#include "EnemyBox.h"
#include "DxLib.h"

namespace
{
	//�傫�������������邽�߂̒萔
	constexpr int kDelSize = 8;

	constexpr unsigned int kDefaultColor = 0xff00ff;
	constexpr unsigned int kWhiteColor = 0xffffff;
	constexpr unsigned int kPreColor = 0x440044;
}

EnemyBox::EnemyBox() :
	m_pos			(0.0f, 0.0f),
	m_sizeMax		(0),
	m_size			(0),
	m_growSizeSpeed	(0),
	m_color			(0xff00ff),
	m_isMax			(false),
	m_isDrawPre		(false)
{
}

EnemyBox::~EnemyBox()
{
}

void EnemyBox::Init(Vec2 pos, int size,int speed, int firstFrame, int secondFrame,bool isPre)
{
	/*���̐ݒ�*/
	//���݃t���O��true�ɂ���
	m_isExist = true;

	//���W��ݒ肷��
	m_pos = pos;
	//�ő�T�C�Y��ݒ肷��
	m_sizeMax = size;
	//�ҋ@�t���[�����̐ݒ�
	m_waitFrame = firstFrame;
	//�����n�߂�t���[�����̐ݒ�
	m_disappearFrame = secondFrame;

	//�傫���Ȃ�͂₳�̐ݒ�
	m_growSizeSpeed = speed;
	//�\������`�悷�邩�ǂ�����ݒ�
	m_isDrawPre = isPre;
}

void EnemyBox::Update()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		return;
	}

	//�t���[�������X�V
	m_frame++;

	//�ŏ��Ɏw�肵���t���[�����߂���Ǝ��̂��o�Ă���
	if (m_frame >= m_waitFrame)
	{
		if (m_frame < m_disappearFrame)
		{
			//�����ő�T�C�Y�ɂȂ�����
			if (m_size > m_sizeMax + static_cast<int>(m_sizeMax * 0.1f))
			{
				//�ő�T�C�Y�ɂȂ������t���O��true�ɂ���
				m_isMax = true;
				//�F�𔒂ɂ���
				m_color = kWhiteColor;
			}
			else
			{
				//����ȊO�̎��̓f�t�H���g�J���[�ɂ���
				m_color = kDefaultColor;
			}

			//�ő�ɂȂ��Ă��Ȃ��Ƃ��͑傫���Ȃ�
			if (!m_isMax)
			{
				m_size += m_growSizeSpeed;
			}
			//��u�ő�T�C�Y���傫���Ȃ邽�ߕ␳����
			else if (m_size > m_sizeMax)
			{
				m_size -= kDelSize;
			}
		}

		//���̂̓����蔻��̐ݒ�
		m_col.SetBox(m_pos.x - m_size, m_pos.y - m_size,
			m_pos.x + m_size, m_pos.y + m_size);
	}
	else
	{
		//���̂̓����蔻���ݒ肷��܂ł̊ԁA�����蔻�肪{0,0}�ɂ����Ē[�ɍs���Ɠ������Ă��܂�����
		//���̃o�O�΍�̏���
		m_col.SetBox(-1, -1, -1, -1);
	}

	//��ԖڂɎw�肵���t���[�����߂���Ə������Ȃ�
	if (m_frame >= m_disappearFrame)
	{
		m_size -= m_growSizeSpeed;
	}

	//�T�C�Y��0��菬�����Ȃ�Ƒ��݂�����
	if (m_size < 0)
	{
		m_isExist = false;
	}
}

void EnemyBox::Draw()
{
	//���݃t���O��false�̂Ƃ��͏��������Ȃ�
	if (!m_isExist)
	{
		return;
	}

	//�\�����̕`��t���O��true�Ȃ�
	if (m_isDrawPre)
	{
		//�\���̕`��
		if (!m_isMax)
		{
			DrawBox(static_cast<int>(m_pos.x) - m_sizeMax, static_cast<int>(m_pos.y) - m_sizeMax,
				static_cast<int>(m_pos.x) + m_sizeMax, static_cast<int>(m_pos.y) + m_sizeMax, kPreColor, true);
		}
	}

	//�{�̂̕`��
	DrawBox(static_cast<int>(m_pos.x) - m_size, static_cast<int>(m_pos.y) - m_size,
		static_cast<int>(m_pos.x) + m_size, static_cast<int>(m_pos.y) + m_size, m_color, true);

#ifdef _DEBUG
	m_col.DrawColBox(0xffffff);
#endif
}
