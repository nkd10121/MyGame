#include <DxLib.h>
#include <cmath>
#include "Input.h"
#include "Application.h"
#include "Game.h"
#include "SceneManager.h"
#include "DrawBGparts.h"
#include "TitleScene.h"
#include "StageSelect.h"
#include "OptionScene.h"

namespace
{
	//�t�F�[�h�C���A�t�F�[�h�A�E�g�ɂ����鎞��
	constexpr int kFadeFrame = 30;

	//�{�^�������E�ɓ�������
	constexpr int kButtonMoveSpeed = 24;
	//�w�i�̎l�p�̐�
	constexpr int kBGnum= 10;

	//�f�����[�r�[������n�߂�t���[��
	constexpr int kPlayMovieFrame = 600;

	//�v���C�{�^����y���W
	constexpr int kPlayButtonY = 380;
	//�I�v�V�����{�^����y���W
	constexpr int kOptionButtonY = 560;
	//��߂�{�^����y���W
	constexpr int kQuitButtonY = 740;
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	m_playButtonPosX(680),
	m_optionButtonPosX(800),
	m_quitButtonPosX(800),
	m_movieHandle(0)
{
	//�^�C�g���ŗ�������̃n���h�����擾
	m_movieHandle = m_manager.m_gManager.GetHandle("TitleMovie");

	//�w�i�̎l�p�̏���������
	m_parts.resize(kBGnum);
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i] = new DrawBGparts;
		m_parts[i]->Init(false);
	}

	m_fadeFrame = 60;
	updateFunc_ = &TitleScene::FadeInUpdate;
	drawFunc_ = &TitleScene::FadeOutDraw;
}

TitleScene::~TitleScene()
{

}

void TitleScene::Update(Input& input)
{
	//BGM�𗬂�
	m_manager.m_sManager.PlayBGM("Title", true);

	(this->*updateFunc_)(input);
	m_frame++;

	//�w�i�̎l�p�̍X�V����
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i]->Update();
	}

	//�^�C�g�����S��BGM�ƍ��킹�ē����悤��
	if (m_frame == 70)
	{
		m_titleSize = m_titleSize * 1.08f;

	}
	else if (m_frame== 140)
	{
		m_titleSize = m_titleSize * 1.08f;
		m_frame = 0;
	}
	else
	{
		m_titleSize = 0.28f;
	}

	//play�{�^����I�����Ă���ꍇ
	if (m_nowButton == Play)
	{
		//play�{�^�������ɓ�����
		if (m_playButtonPosX >= 680)
		{
			m_playButtonPosX -= kButtonMoveSpeed;
		}
	}
	//�I�����Ă��Ȃ��ꍇ
	else
	{
		//�E�ɓ�����
		if (m_playButtonPosX <= 800)
		{
			m_playButtonPosX += kButtonMoveSpeed;
		}
	}

	if (m_nowButton == Option)
	{
		if (m_optionButtonPosX >= 680)
		{
			m_optionButtonPosX -= kButtonMoveSpeed;
		}
	}
	else
	{
		if (m_optionButtonPosX <= 800)
		{
			m_optionButtonPosX += kButtonMoveSpeed;
		}
	}

	if (m_nowButton == Quit)
	{
		if (m_quitButtonPosX >= 680)
		{
			m_quitButtonPosX -= kButtonMoveSpeed;
		}
	}
	else
	{
		if (m_quitButtonPosX <= 800)
		{
			m_quitButtonPosX += kButtonMoveSpeed;
		}
	}

	if (m_waitFrame == kPlayMovieFrame)
	{
		m_isPlayMovie = true;
	}

	if (m_isPlayMovie)
	{
		m_manager.m_sManager.FadeOutBGM("Title", m_waitFrame - kPlayMovieFrame);

		if (m_waitFrame > kPlayMovieFrame)
		{
			if (GetMovieStateToGraph(m_movieHandle) == 0)
			{
				m_waitFrame = 0;

				m_frame = 0;
				m_isPlayMovie = false;

				PauseMovieToGraph(m_movieHandle);
				SeekMovieToGraph(m_movieHandle, 0);
			}

			if (GetJoypadInputState(DX_INPUT_KEY_PAD1) || CheckHitKeyAll() != 0)
			{
				m_waitFrame = 0;

				m_frame = 0;
				m_isPlayMovie = false;

				PauseMovieToGraph(m_movieHandle);
				SeekMovieToGraph(m_movieHandle, 0);
			}
		}	
	}
}

void TitleScene::Draw()
{
	//BG
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i]->Draw();
	}

	//���S
	m_manager.Draw(340, m_titlePosY, m_titleSize, 0, "Title");

	//�{�^��
	m_manager.Draw(m_playButtonPosX, kPlayButtonY, 1.0, 0, "Button1");
	DrawStringToHandle(m_playButtonPosX + 148, kPlayButtonY - 180, "�X�^�[�g", 0xffffff, m_manager.ReturnFontHandle());

	m_manager.Draw(m_optionButtonPosX, kOptionButtonY, 1.0, 0, "Button2");
	DrawStringToHandle(m_optionButtonPosX + 148, kOptionButtonY - 180, "�I�v�V����", 0xffffff, m_manager.ReturnFontHandle());

	m_manager.Draw(m_quitButtonPosX, kQuitButtonY, 1.0, 0, "Button3");
	DrawStringToHandle(m_quitButtonPosX + 148, kQuitButtonY - 180, "��߂�", 0xffffff, m_manager.ReturnFontHandle());

#ifdef _DEBUG
	DrawFormatString(0, 16, 0xff0000, "%d", m_waitFrame);
#endif
	(this->*drawFunc_)();

	if (m_isPlayMovie)
	{
		PlayMovieToGraph(m_movieHandle);
		DrawGraph(0, 0, m_movieHandle, false);

		m_manager.m_gManager.Draw(640, 360, 1.0f, 0.0f, "DemoText");
	}
}

void TitleScene::FadeInUpdate(Input&)
{
	m_fadeFrame--;
	if (m_fadeFrame <= 0)
	{
		//�t�F�[�h�C�����I��������Ԃ�J�ڂ���
		//���̑J�ڐ�
		updateFunc_ = &TitleScene::NormalUpdate;
		drawFunc_ = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	//���O���c��
	m_nowButtonLog = m_nowButton;

	//���[�r�[���̓V�[���J�ڂ������Ȃ�
	if (!m_isPlayMovie)
	{
		//��������������Ƃ�
		if (input.IsTriggered("UP"))
		{
			//�v���C�{�^����I�����Ă���ꍇ
			if (m_nowButton == Play)
			{
				//���ɖ߂��H
				/*nowButton = Quit;*/
			}
			//�I�v�V�����{�^����I�����Ă���ꍇ
			else if (m_nowButton == Option)
			{
				m_manager.m_sManager.PlaySE("moveSE");
				//�v���C�{�^����I�����Ă����Ԃɂ���
				m_nowButton = Play;
			}
			//��߂�{�^����I�����Ă���ꍇ
			else if (m_nowButton == Quit)
			{
				m_manager.m_sManager.PlaySE("moveSE");
				//�I�v�V�����{�^����I�����Ă����Ԃɂ���
				m_nowButton = Option;
			}
		}

		//���������������Ƃ�
		if (input.IsTriggered("DOWN"))
		{
			if (m_nowButton == Play)
			{
				m_manager.m_sManager.PlaySE("moveSE");
				//�I�v�V�����{�^����I�����Ă����Ԃɂ���
				m_nowButton = Option;
			}
			else if (m_nowButton == Option)
			{
				m_manager.m_sManager.PlaySE("moveSE");
				//��߂�{�^����I�����Ă����Ԃɂ���
				m_nowButton = Quit;
			}
			else if (m_nowButton == Quit)
			{
				//now = Play;
			}
		}

		//OK�{�^�����������Ƃ�
		if (input.IsTriggered("OK"))
		{
			//�v���C�{�^����I�����Ă���ƃt�F�[�h�A�E�g���n�߂�
			if (m_nowButton == Play)
			{
				m_manager.m_sManager.PlaySE("enterSE");

				updateFunc_ = &TitleScene::FadeOutUpdate;
				drawFunc_ = &TitleScene::FadeOutDraw;
				m_fadeFrame = 0;
			}

			//�I�v�V�����{�^����OK���������Ƃ�
			if (m_nowButton == Option/* || input.IsTriggered("pause")*/)
			{
				m_manager.m_sManager.PlaySE("enterSE");

				//�I�v�V������ʂ�\������
				m_manager.PushScene(std::make_shared<OptionScene>(m_manager));
			}

			//��߂�{�^����OK���������Ƃ�
			if (m_nowButton == Quit)
			{
				//�v���W�F�N�g���I������
				Application::GetInstance().Terminate();
			}
		}
	}

	//�J�[�\���̑��삪�Ȃ��Ƃ��A���[�r�[�������܂ł̃J�E���g��i�߂�
	if (m_nowButton == m_nowButtonLog)
	{
		m_waitFrame++;
	}
	else
	{
		m_waitFrame = 0;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	m_fadeFrame++;
	m_manager.m_sManager.FadeOutBGM("Title", m_fadeFrame);
	if (m_fadeFrame >= kFadeFrame)
	{
		//�t�F�[�h�A�E�g���I�������Scene��ς���
		if (m_nowButton == Play)
		{
			m_manager.ChangeScene(std::make_shared<StageSelect>(m_manager));
		}
	}
}

void TitleScene::FadeOutDraw()
{
#ifdef _DEBUG
	DrawString(0, 0, "TitleScene", 0xffffff);
#endif

	float rate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);
	float alpha = 255.0f * rate;
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(alpha));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
#ifdef _DEBUG
	DrawString(0, 0, "TitleScene", 0xffffff);
#endif
}