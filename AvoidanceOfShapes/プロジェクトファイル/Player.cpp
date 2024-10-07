#include "Player.h"
#include "DxLib.h"
#include <cassert>
#include "Input.h"
#include "Game.h"
#include "AfterImagePlayer.h"
#include "Efects.h"
#include "DashEfect.h"
#include "HitEfect.h"

namespace
{
	//�ړ����x
	constexpr float kBaseSpeed = 6.0f;
	constexpr float kDashSpeed = kBaseSpeed * 4;

	//�L�����N�^�[�̃T�C�Y
	constexpr int kWidth = 14;
	constexpr int kHeight = 14;

	//�c���̍ő吔
	constexpr int kAfterImageNumMax = 64;

	//�G�t�F�N�g�̐�
	constexpr int kEfectMax = 32;
}

Player::Player() :
	m_isInStage(true),
	m_pos(15, 15),
	m_colPos(0,0),
	m_padStickX(0),
	m_padStickY(0),
	m_powerX(0),
	m_powerY(0),
	m_angle(0),
	m_growSpeed(kBaseSpeed),
	m_dashCount(0),
	m_isDash(false),
	m_hitCount(0),
	m_knockBackSpeed(0),
	m_damageDrawFrame(0),
	m_playerInvincibleTime(0)
{
	m_ScreenSizeW = Game::kScreenWidth;
	m_ScreenSizeH = Game::kScreenHeight;

	m_playerHandle[0] = LoadGraph("data/player/player0.png");
	m_playerHandle[1] = LoadGraph("data/player/player1.png");
	m_playerHandle[2] = LoadGraph("data/player/player2.png");
	m_playerHandle[3] = LoadGraph("data/player/player3.png");

	for (int i = 0; i < 4; i++)
	{
		assert(m_playerHandle[i] != -1);
	}

	m_afterImage.resize(kAfterImageNumMax);
	m_efects.resize(kEfectMax);
}

Player::~Player()
{
}

void Player::Init(bool isInStage)
{
	m_isInStage = isInStage;
	if (m_isInStage)
	{
		m_pos.x = 100;
		m_pos.y = 360;
	}
	else
	{
		m_pos.x = Game::kScreenWidth / 4 * 3;
		m_pos.y = Game::kScreenHeight / 2;
	}
}

void Player::Update(Input& input)
{
	m_frame++;
	m_posLog = m_pos;

	//m_damageDrawFrame��0���傫���Ƃ��ɂ��񂾂񏬂�������
	//�ꉞ����ȊO�̎���0��������悤�ɂ���
	if (m_damageDrawFrame >= 0)
	{
		m_damageDrawFrame--;
	}
	else
	{
		m_damageDrawFrame = 0;
	}

	if (m_playerInvincibleTime > 0)
	{
		m_playerInvincibleTime--;
	}
	else
	{
		m_playerInvincibleTime = 0;
	}

	//pad�̃X�e�B�b�N���(�e���L�[�̖��L�[)���擾
	GetJoypadAnalogInput(&m_padStickX, &m_padStickY, DX_INPUT_KEY_PAD1);


	if (m_padStickX == 0 && m_padStickY == 0)
	{
		if (input.IsPushed("UP"))
		{
			m_padStickY = -1000;
		}
		if (input.IsPushed("DOWN"))
		{
			m_padStickY = 1000;
		}
		if (input.IsPushed("LEFT"))
		{
			m_padStickX = -1000;
		}
		if (input.IsPushed("RIGHT"))
		{
			m_padStickX = 1000;
		}
	}

	m_powerX = static_cast<float>(m_padStickX) / 1000;
	m_powerY = static_cast<float>(m_padStickY) / 1000;


	//�v���C���[�̊p�x�𒲐�
	if (m_padStickX != 0 || m_padStickY != 0)
	{
		m_angle = atan2(m_powerX, -m_powerY);
	}
	else
	{
		//����������Ă��Ȃ��Ƃ�������߂�
		m_angle = 0.0;
	}

	//PAD�ł̑���
	m_pos.x += m_growSpeed * m_powerX - m_knockBackSpeed;
	m_pos.y += m_growSpeed * m_powerY;

	//�m�b�N�o�b�N�����Ƃ�
	//�m�b�N�o�b�N�̃X�s�[�h�����񂾂񌸂炷
	if (m_knockBackSpeed != 0)
	{
		m_knockBackSpeed--;
	}

	//��ʊO�ɏo�Ȃ��悤��
	if (m_pos.x + kWidth >= m_ScreenSizeW)
	{
		m_pos.x = static_cast<float>(m_ScreenSizeW - kWidth);
	}
	if (m_pos.x - kWidth <= 0)
	{
		m_pos.x = 0 + kWidth;
	}
	if (m_pos.y - kHeight <= 0)
	{
		m_pos.y = 0 + kHeight;
	}
	if (m_pos.y + kHeight >= m_ScreenSizeH)
	{
		m_pos.y = static_cast<float>(m_ScreenSizeH - kHeight);
	}


	//�_�b�V��
	//�����Ă���ԃJ�E���g�𑝂₵������
	//�������u�Ԃ����_�b�V�����A�����Ă�����񉟂��ƃ_�b�V������悤��
	if(input.IsPushed("dash"))
	{
		m_dashCount++;
	}
	else
	{
		m_dashCount = 0;
	}
	//�������u�ԃ_�b�V��
	if (m_dashCount == 1 && !m_isDash)
	{
		for (int i = 0; i < GetRand(4) + 20; i++)
		{
			CreateAfterImage(m_pos, (m_pos - m_posLog), GetRand(1) + 3, true);
		}

		CreateDashEfect(m_pos);
		
		m_growSpeed = kDashSpeed;
		m_isDash = true;
	}


	//�X�s�[�h�����������̃X�s�[�h��
	if (m_growSpeed >= kBaseSpeed)
	{
		m_growSpeed--;
		if (m_growSpeed < kBaseSpeed)
		{
			m_isDash = false;
		}
	}

	if (m_pos - m_posLog != Vec2{} && !m_isDash && m_hitCount < 4)
	{
		if (m_frame % 2 == 0)
		{
			CreateAfterImage(m_pos, (m_pos - m_posLog), 4, false);
		}
	}

	m_colPos = m_pos;

	//�����蔻��̍X�V
	//�_�b�V�����Ă���Ƃ����_���[�W���󂯂���̖��G���Ԃ̊Ԃ͓����蔻����Ȃ���
	if (m_isDash  || m_playerInvincibleTime != 0)
	{
		if (m_isInStage)
		{
			m_colPos = { -150,-150 };
		}
	}

	//�G�Ƃ̓����蔻��̐ݒ�
	m_col.SetCenter(m_colPos.x, m_colPos.y, kWidth * 2, kHeight * 2);
	//�S�[���Ƃ̓����蔻��̐ݒ�
	m_colOfGoal.SetCenter(m_pos.x, m_pos.y, kWidth * 2, kHeight * 2);

	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			m_afterImage[i]->Update();

			//�g�p�ς݂̓G�L�����N�^���폜����K�v������
			if (!m_afterImage[i]->isExist())
			{
				//���������������
				delete m_afterImage[i];
				m_afterImage[i] = nullptr;	//�g���Ă��Ȃ��Ƃ킩��悤��
			}
		}
	}

	for (int i = 0; i < m_efects.size(); i++)
	{
		if (m_efects[i])
		{
			m_efects[i]->Update();

			//�g�p�ς݂̓G�L�����N�^���폜����K�v������
			if (!m_efects[i]->isExist())
			{
				//���������������
				delete m_efects[i];
				m_efects[i] = nullptr;	//�g���Ă��Ȃ��Ƃ킩��悤��
			}
		}
	}
}

void Player::Draw()
{
	//�c���̕`��
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			m_afterImage[i]->Draw();
		}
	}

	for (int i = 0; i < m_efects.size(); i++)
	{
		if (m_efects[i])
		{
			m_efects[i]->Draw();
		}
	}

	//�_���[�W���󂯂��Ƃ��_�ł���悤��
	if (m_damageDrawFrame % 6 != 4)
	{
		//�v���C���[�̕`��
		DrawRotaGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), 1.0, m_angle, m_playerHandle[m_hitCount], true);
	}
	//�v���C���[�̘g
	//DrawBox(m_pos.x - kWidth, m_pos.y - kHeight, m_pos.x + kWidth, m_pos.y + kHeight, GetColor(24, 24, 24), false);

#ifdef _DEBUG
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "X�����̓��� : %d", m_padStickX);
	//DrawFormatString(0, 16, GetColor(255, 255, 255), "Y�����̓��� : %d", m_padStickY);

	////DrawFormatString(0, 0, GetColor(255, 255, 255), "m_speed : %f", m_speed);
	//DrawFormatString(0, 32, GetColor(255, 255, 255), "Xpos : %f", m_pos.x);
	//DrawFormatString(0, 48, GetColor(255, 255, 255), "Ypos : %f", m_pos.y);

	//DrawFormatString(0, 64, GetColor(255, 255, 255), "hitCount : %d", m_hitCount);

	m_col.DrawColBox(0x000000);
#endif
}

void Player::SetPlayerHandle(int handle0, int handle1, int handle2, int handle3)
{
	m_playerHandle[0] = handle0;
	m_playerHandle[1] = handle1;
	m_playerHandle[2] = handle2;
	m_playerHandle[3] = handle3;
}

void Player::HitEnemy()
{
	//�m�b�N�o�b�N�X�s�[�h��^����
	m_knockBackSpeed = 20;

	m_damageDrawFrame = 180;
	m_playerInvincibleTime = 180;

	if (m_hitCount < 3)
	{
		CreateHitEfect(m_pos);
	}
	else
	{
		CreateHitEfect(m_pos,true);
	}
	StartJoypadVibration(DX_INPUT_PAD1, 1000, 200,1);

	if (m_isInStage)
	{
		m_hitCount++;
	}
}

void Player::CreateAfterImage(Vec2 pos, Vec2 vec, int size, bool isDash)
{
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (!m_afterImage[i])
		{
			m_afterImage[i] = new AfterImagePlayer;
			m_afterImage[i]->Init(pos, vec,size,isDash);
			return;
		}
	}
}

void Player::CreateDashEfect(Vec2 pos, bool isLastHit)
{
	for (int i = 0; i < m_efects.size(); i++)
	{
		if (!m_efects[i])
		{
			m_efects[i] = new DashEfect;
			m_efects[i]->Init(pos, isLastHit);
			return;
		}
	}
}

void Player::CreateHitEfect(Vec2 pos, bool isLastHit)
{
	for (int i = 0; i < m_efects.size(); i++)
	{
		if (!m_efects[i])
		{
			m_efects[i] = new HitEfect;
			m_efects[i]->Init(pos, isLastHit);
			m_efects[i]->SetGraphHandle(m_playerHandle[0]);
			return;
		}
	}
}
