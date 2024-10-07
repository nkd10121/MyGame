#include "StageSelect.h"
#include "Input.h"
#include "Game.h"
#include "SceneManager.h"
#include "PauseScene.h"
#include <DxLib.h>

#include "DrawBGparts.h"

#include "AfterImagePlayer.h"

#include "StageTutorial.h"
#include "Stage1.h"
#include "Stage2.h"

//�X�e�[�W��������Ă����`���ɂ��邩�ǂ���
//false:�ŏ�����S�X�e�[�W����Atrue:�X�e�[�W���N���A���ĉ�����Ă���
#define isSTAGECLEARFORMAT true

namespace
{
	//�t�F�[�h�C���A�t�F�[�h�A�E�g�ɂ����鎞��
	constexpr int kFadeFrame = 30;

	//�w�i�̎l�p�̍ő吔
	constexpr int kBGnum = 10;

	//�v���C���[�������X�s�[�h
	constexpr int kMoveSpeed = 20;
	//�v���C���[�̊�{���W
	constexpr float kPlayerPosX = 640.0f;
	constexpr float kPlayerPosY = 600.0f;
	//�v���C���[�����n����܂ł̃t���[��
	constexpr int kPlayerLandingFrame = 66;
	//�v���C���[�̃W�����v��
	constexpr int kPlayerJumpPower = 180;

	//�g�����W�V�����p��box�̃T�C�Y
	constexpr int kWidth = 80;
	constexpr int kHeight = 80;
	//�g�����W�V�����p��Box�̐�
	constexpr int kWidthNum = 16;
	constexpr int kHeightNum = 9;

	//�X�e�[�W�ڍׂ̊�{�l
	constexpr int kStageAlphaBase = 200;
	//�X�e�[�W�ڍׂ̖��Â̑J�ڑ��x
	constexpr float kStageAlphaSpeed = 3.0f;

	constexpr int kDetailBasePosX = 640;
	constexpr int kDetailBasePosY = 360;

}


StageSelect::StageSelect(SceneManager& mgr) :
	Scene(mgr)
{
	//�w�i�̎l�p�̏�����
	m_parts.resize(kBGnum);
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i] = new DrawBGparts;
		m_parts[i]->Init(true);
	}

	//�g�����W�V�����̏���
	m_posX.resize(kWidthNum);
	for (int i = 0; i < m_posX.size(); i++)
	{
		m_posX[i] = static_cast<int>(kWidth * 0.5f) + kWidth * i;
	}

	m_posY.resize(kHeightNum);
	for (int i = 0; i < m_posY.size(); i++)
	{
		m_posY[i] = static_cast<int>(kHeight * 0.5f) + kHeight * i;
	}

	//�Q�[����ʕ`���̐���
	//��ʃT�C�Y�Ɠ����傫���̃O���t�B�b�N�f�[�^�𐶐�����
	m_gameScreenHandle = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	//�v���C���[�̊�{���W����
	m_playerPos.x = kPlayerPosX;
	m_playerPos.y = kPlayerPosY;

	//�ŏ��̓`���[�g���A����I�����Ă����Ԃɂ���
	m_next = nTutorial;

	updateFunc_ = &StageSelect::FadeInUpdate;
	drawFunc_ = &StageSelect::FadeOutDraw;
	m_fadeFrame = 60;

}

StageSelect::~StageSelect()
{
	DeleteGraph(m_gameScreenHandle);
}

void StageSelect::Update(Input& input)
{
	//�X�e�[�W���N���A���ăX�e�[�W�Z���N�g�ɖ߂��Ă����Ƃ��A�N���A�����X�e�[�W��I�����Ă����Ԃɂ��鏈��
	if (m_isFirst)
	{
		//TODO:if���̒��g���N���A�t���O�ɂ���
		//�X�e�[�W1�̃A�h���X�ƒ��O�̃V�[���̃A�h���X�������ꍇ
		if (m_manager.m_stage1 == m_manager.m_logScene)
		{
			//�X�e�[�W1��I�����Ă����Ԃɂ���
			m_next = nStage1;
			m_detailX = -kDetailBasePosX * m_next;
		}
		//�X�e�[�W2�̃A�h���X�ƒ��O�̃V�[���̃A�h���X�������ꍇ
		if (m_manager.m_stage2 == m_manager.m_logScene)
		{
			//�X�e�[�W2��I�����Ă����Ԃɂ���
			m_next = nStage2;
			m_detailX = -kDetailBasePosX * m_next;
		}

		m_isFirst = false;
	}

	m_frame++;

	//�w�i�̍X�V����
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i]->Update();
	}


	if (m_next == nTutorial)
	{
		if (m_detailX < 0)
		{
			m_detailX += kMoveSpeed;
		}
		if (m_detailX == 0)
		{
			m_isMoving = false;
		}
	}

	if (m_next == nStage1)
	{
		int basePos = -kDetailBasePosX * m_next;
		if (m_detailX > basePos)
		{
			m_detailX -= kMoveSpeed;
		}

		if (m_detailX < basePos)
		{
			m_detailX += kMoveSpeed;
		}

		if (m_detailX == basePos)
		{
			m_isMoving = false;
		}
	}


	if (m_next == nStage2)
	{
		int basePos = -kDetailBasePosX * m_next;
		if (m_detailX > basePos)
		{
			m_detailX -= kMoveSpeed;
		}

		if (m_detailX == basePos)
		{
			m_isMoving = false;
		}
	}

	//�����Ă���Ƃ���BGM�̏���
	if (m_isMoving)
	{
		//BGM���t�F�[�h�A�E�g������
		m_soundFadeFrame++;
		m_manager.m_sManager.FadeOutBGM("Tutorial", m_soundFadeFrame);
		m_manager.m_sManager.FadeOutBGM("stage1", m_soundFadeFrame);
		m_manager.m_sManager.FadeOutBGM("stage2", m_soundFadeFrame);
	}
	else
	{
		//�����Ă��Ȃ��Ƃ��t�F�[�h�A�E�g�p�ϐ���0�ɂ���
		m_soundFadeFrame = 0;
	}

	//�����Ă���Ƃ��̃v���C���[�̏���
	if (m_isMoving)
	{
		
		m_angle -= DX_TWO_PI_F / kPlayerLandingFrame;

		//�v���C���[��y���W����{���W��y���W�𒴂��Ă������{���W�ɕ␳����
		if (m_playerPos.y > kPlayerPosY)
		{
			m_playerPos.y = kPlayerPosY;
			m_angle = 0;
		}

		//y���W�̈ړ��̂��߂̏���
		m_offsetY = sinf(m_angle) * kPlayerJumpPower;


		m_playerPos.y = kPlayerPosY + m_offsetY;
	}
	else
	{
		m_playerPos.y = kPlayerPosY;
		m_offsetY = 0;
		m_angle = 0;
	}

	if (m_next > m_nextLog)
	{
		m_playerRotationAngle = -m_angle;
	}
	if (m_next < m_nextLog)
	{
		m_playerRotationAngle = m_angle;
	}


#if isSTAGECLEARFORMAT
	/*�I�����Ă���X�e�[�W�𖾂邭�A
	�I�����Ă��Ȃ��X�e�[�W���Â��`�悷�邽�߂̏���*/
	m_stageAlpha[nTutorial] = static_cast<int>(-m_detailX / kStageAlphaSpeed);
	if (m_stageAlpha[nTutorial] > kStageAlphaBase)
	{
		m_stageAlpha[nTutorial] = kStageAlphaBase;
	}

	if (m_manager.GetStageClearFlag(0) == true)
	{
		m_stageAlpha[nStage1] = static_cast<int>(abs((m_detailX + kDetailBasePosX) / kStageAlphaSpeed));
		if (m_stageAlpha[nStage1] > kStageAlphaBase)
		{
			m_stageAlpha[nStage1] = kStageAlphaBase;
		}
	}

	if (m_manager.GetStageClearFlag(1) == true)
	{
		m_stageAlpha[nStage2] = static_cast<int>((m_detailX + kDetailBasePosX * nStage2) / kStageAlphaSpeed);
		if (m_stageAlpha[nStage2] > kStageAlphaBase)
		{
			m_stageAlpha[nStage2] = kStageAlphaBase;
		}
	}
#else
	/*�I�����Ă���X�e�[�W�𖾂邭�A
	�I�����Ă��Ȃ��X�e�[�W���Â��`�悷�邽�߂̏���*/
	m_stageAlpha[nTutorial] = static_cast<int>(-m_detailX / kStageAlphaSpeed);
	if (m_stageAlpha[nTutorial] > kStageAlphaBase)
	{
		m_stageAlpha[nTutorial] = kStageAlphaBase;
	}

	m_stageAlpha[nStage1] = static_cast<int>(abs((m_detailX + kDetailBasePosX) / kStageAlphaSpeed));
	if (m_stageAlpha[nStage1] > kStageAlphaBase)
	{
		m_stageAlpha[nStage1] = kStageAlphaBase;
	}

	m_stageAlpha[nStage2] = static_cast<int>((m_detailX + kDetailBasePosX * nStage2) / kStageAlphaSpeed);
	if (m_stageAlpha[nStage2] > kStageAlphaBase)
	{
		m_stageAlpha[nStage2] = kStageAlphaBase;
	}
#endif

	(this->*updateFunc_)(input);
}

void StageSelect::Draw()
{
	///////////////////////////////
	SetDrawScreen(m_gameScreenHandle);
	//�`���X�N���[�����N���A����
	ClearDrawScreen();
	///////////////////////////////

	//�w�i�̕`��
	DrawBox(0, 0, 1280, 720, 0x0d1118, true);
	//�w�i��Box�̕`��
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i]->Draw();
	}

	m_manager.m_gManager.Draw(m_detailX + kDetailBasePosX, kDetailBasePosY, 1.0f, 0.0f, "stageSelect0");
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_stageAlpha[nTutorial]);
	m_manager.m_gManager.Draw(m_detailX + kDetailBasePosX, kDetailBasePosY, 1.0f, 0.0f, "stageSelect0B");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_manager.m_gManager.Draw(m_detailX + kDetailBasePosX * (nStage1 + 1), kDetailBasePosY, 1.0f, 0.0f, "stageSelect1");
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_stageAlpha[nStage1]);
	m_manager.m_gManager.Draw(m_detailX + kDetailBasePosX * (nStage1 + 1), kDetailBasePosY, 1.0f, 0.0f, "stageSelect1B");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_manager.m_gManager.Draw(m_detailX + kDetailBasePosX * (nStage2 + 1), kDetailBasePosY, 1.0f, 0.0f, "stageSelect2");
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_stageAlpha[nStage2]);
	m_manager.m_gManager.Draw(m_detailX + kDetailBasePosX * (nStage2 + 1), kDetailBasePosY, 1.0f, 0.0f, "stageSelect2B");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_manager.m_gManager.Draw(static_cast<int>(m_playerPos.x), static_cast<int>(m_playerPos.y), 1.0f, m_playerRotationAngle * 2, "Player");


#ifdef _DEBUG
	DrawFormatString(0, 16, 0xff0000, "%d", m_detailX);
	DrawFormatString(0, 32, 0xff0000, "%d", m_stageAlpha[nTutorial]);
	DrawFormatString(0, 48, 0xff0000, "%d", m_stageAlpha[nStage1]);
	DrawFormatString(0, 64, 0xff0000, "%d", m_stageAlpha[nStage2]);
#endif


	(this->*drawFunc_)();

	///////////////////////////////
	//�o�b�N�o�b�t�@�ɏ������ސݒ�ɖ߂��Ă���
	SetDrawScreen(DX_SCREEN_BACK);

	for (int y = 0; y < m_posY.size(); y++)
	{
		for (int x = 0; x < m_posX.size(); x++)
		{
			DrawRectRotaGraph(m_posX[x], m_posY[y], kWidth * x, kHeight * y, kWidth, kHeight, m_mag, m_fadeoutAngle, m_gameScreenHandle, false);
		}
	}
	///////////////////////////////
}
void StageSelect::FadeInUpdate(Input& input)
{
	m_fadeFrame--;
	if (m_fadeFrame <= 0)
	{
		updateFunc_ = &StageSelect::NormalUpdate;
		drawFunc_ = &StageSelect::NormalDraw;
	}
}

void StageSelect::NormalUpdate(Input& input)
{
	//�����Ă��Ȃ��Ƃ�
	if (!m_isMoving)
	{
		m_nextLog = m_next;

		//�I�����Ă���X�e�[�W��BGM�𗬂�
		//PlayBGM�̒��ŗ���Ă����牽�����Ȃ��悤�ɂ��Ă���
		if (m_next == nTutorial)
		{
			m_manager.m_sManager.PlayBGM("Tutorial", true);
		}
		if (m_next == nStage1)
		{
			m_manager.m_sManager.PlayBGM("stage1", true);
		}
		if (m_next == nStage2)
		{
			m_manager.m_sManager.PlayBGM("stage2", true);
		}

		//�E���������Ƃ��I�����Ă���X�e�[�W�����炷
		if (input.IsTriggered("RIGHT"))
		{
			if (m_next == nStage1)
			{
				m_next = nStage2;
				m_isMoving = true;
			}

			if (m_next == nTutorial)
			{
				m_next = nStage1;
				m_isMoving = true;
			}
		}

		//�����������Ƃ��I�����Ă���X�e�[�W�����炷
		if (input.IsTriggered("LEFT"))
		{
			if (m_next == nStage1)
			{
				m_next = nTutorial;
				m_isMoving = true;
			}

			if (m_next == nStage2)
			{
				m_next = nStage1;
				m_isMoving = true;
			}
		}

#if isSTAGECLEARFORMAT
		//OK���������Ƃ��Ƀt�F�[�h�A�E�g����
		if (input.IsTriggered("OK"))
		{
			if (m_next == nTutorial)
			{
				updateFunc_ = &StageSelect::FadeOutUpdate;
				drawFunc_ = &StageSelect::FadeOutDraw;
			}

			if (m_next == nStage1 && m_manager.GetStageClearFlag(0) == true)
			{
				updateFunc_ = &StageSelect::FadeOutUpdate;
				drawFunc_ = &StageSelect::FadeOutDraw;
			}

			if (m_next == nStage2 && m_manager.GetStageClearFlag(1) == true)
			{
				updateFunc_ = &StageSelect::FadeOutUpdate;
				drawFunc_ = &StageSelect::FadeOutDraw;
			}
		}
#else
		//OK���������Ƃ��Ƀt�F�[�h�A�E�g����
		if (input.IsTriggered("OK"))
		{
			updateFunc_ = &StageSelect::FadeOutUpdate;
			drawFunc_ = &StageSelect::FadeOutDraw;
		}
#endif

		//�|�[�Y���������Ƃ�
		if (input.IsTriggered("pause"))
		{
			//����SE�𗬂���
			m_manager.m_sManager.PlaySE("enterSE");

			//�I�����Ă���X�e�[�W��BGM���~�߂�
			if (m_next == nTutorial)
			{
				m_manager.m_sManager.StopBGM("Tutorial");
			}
			if (m_next == nStage1)
			{
				m_manager.m_sManager.StopBGM("stage1");
			}
			if (m_next == nStage2)
			{
				m_manager.m_sManager.StopBGM("stage2");
			}

			//�|�[�Y��ʂ�\������
			m_manager.PushScene(std::make_shared<PauseScene>(m_manager));
		}
	}
}

void StageSelect::FadeOutUpdate(Input& input)
{
	if (m_mag > 0)
	{
		m_mag -= m_magSpeed;
		m_fadeoutAngle -= m_fadeoutAngleSpeed;
	}

	if (m_mag <= 0)
	{
		m_mag = 0;
	}

	if (m_mag == 0)
	{
		//�I�����Ă���X�e�[�W�ɑJ�ڂ���
		if (m_next == nTutorial)
		{
			m_manager.ChangeScene(std::make_shared<StageTutorial>(m_manager));
		}
		if (m_next == nStage1)
		{
			m_manager.ChangeScene(std::make_shared<Stage1>(m_manager));
		}
		if (m_next == nStage2)
		{
			m_manager.ChangeScene(std::make_shared<Stage2>(m_manager));
		}
	}

	//�t�F�[�h�A�E�g�p�̃J�E���^��i�߂�
	m_fadeFrame++;

	//�I�����Ă���X�e�[�W��BGM���t�F�[�h�A�E�g������
	if (m_next == nTutorial)
	{
		m_manager.m_sManager.FadeOutBGM("Tutorial", m_fadeFrame);
	}
	if (m_next == nStage1)
	{
		m_manager.m_sManager.FadeOutBGM("stage1", m_fadeFrame);
	}
	if (m_next == nStage2)
	{
		m_manager.m_sManager.FadeOutBGM("stage2", m_fadeFrame);
	}
}

void StageSelect::FadeOutDraw()
{
#ifdef _DEBUG
	DrawString(0, 0, "StageSelectScene", 0xffffff);
#endif
	float rate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);
	float alpha = 255.0f * rate;
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(alpha));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void StageSelect::NormalDraw()
{
#ifdef _DEBUG
	DrawString(0, 0, "StageSelectScene", 0xffffff);
#endif
}