#include "PauseScene.h"
#include "Input.h"
#include "Game.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include <DxLib.h>

#include "Application.h"
#include "AfterImagePlayer.h"

namespace
{
	//�c���̍ő吔
	constexpr int kAfterImageNumMax = 64;

	constexpr int kVolumeEmptyBoxSize = 20;

	const Vec2 kItemPos[ItemNum] = {
		{130.0f,124.0f},
		{80.0f,224.0f},
		{80.0f,324.0f},
		{110.0f,424.0f},
		{376.0f,110.0f},
		{376.0f,210.0f},
		{376.0f,310.0f},
		{600.0f,350.0f}
	};

	//�v���C���[�����t���[��������Ƃ���]���邽�߂�Angle
	constexpr float kRotationPlayerAngle = 0.08f;
	//�c���������t���[���Ԋu
	constexpr int kPlayerAfterImageIntervalFrame = 4;
}

PauseScene::PauseScene(SceneManager& manager) :
	Scene(manager)
{
	//�v���C���[�n���h���̎擾
	m_pHandle = m_manager.m_gManager.GetHandle("Player");
	m_checkHandle = m_manager.m_gManager.GetHandle("Check");

	//���ڂ̏�����
	m_itemInfo[Setting].item = Setting;
	m_itemInfo[Setting].pos = kItemPos[Setting];
	m_itemInfo[Operation].item = Operation;
	m_itemInfo[Operation].pos = kItemPos[Operation];
	m_itemInfo[ToTitle].item = ToTitle;
	m_itemInfo[ToTitle].pos = kItemPos[ToTitle];
	m_itemInfo[Close].item = Close;
	m_itemInfo[Close].pos = kItemPos[Close];
	m_itemInfo[BGM].item = BGM;
	m_itemInfo[BGM].pos = kItemPos[BGM];
	m_itemInfo[SE].item = SE;
	m_itemInfo[SE].pos = kItemPos[SE];
	m_itemInfo[FullScreen].item = FullScreen;
	m_itemInfo[FullScreen].pos = kItemPos[FullScreen];
	m_itemInfo[ReturnTitle].item = ReturnTitle;
	m_itemInfo[ReturnTitle].pos = kItemPos[ReturnTitle];

	//�J�[�\���͍ŏ��ɐݒ�̂Ƃ����
	m_cursol = m_itemInfo[Setting];

	m_BGMnum = m_manager.GetBGMvolume();
	m_SEnum = m_manager.GetSEvolume();
	m_isFullScreen = m_manager.GetisFull();

	//�c���̃T�C�Y
	m_afterImage.resize(kAfterImageNumMax);

	updateFunc_ = &PauseScene::DefaultUpdate;
	drawFunc_ = &PauseScene::SettingDraw;
}

PauseScene::~PauseScene()
{
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			delete m_afterImage[i];
			m_afterImage[i] = nullptr;
		}
	}
}

void PauseScene::Update(Input& input)
{
	m_frame++;
	m_angle += kRotationPlayerAngle;

	//�I�v�V������ʂ��J�����Ƃ����Ȃ�Ȃ��悤��
	if (m_cursolLog.pos != Vec2{})
	{
		//�J�[�\���ړ����ɉ����𗬂�
		if (m_cursol.item != m_cursolLog.item)
		{
			m_manager.m_sManager.PlaySE("moveSE");
		}
	}
	m_cursolLog = m_cursol;

	(this->*updateFunc_)(input);

	/*�J�[�\���̎c���֌W*/
	if (m_frame % kPlayerAfterImageIntervalFrame == 0)
	{
		CreateAfterImage(m_cursol.pos);
	}

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
}

void PauseScene::Draw()
{
	/*�g�̕`��*/
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DrawBox(50, 50, Game::kScreenWidth - 50, Game::kScreenHeight - 50, 0x222222, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(50, 50, Game::kScreenWidth - 50, Game::kScreenHeight - 50, 0xffffff, false);

	/*���ڂ̕`��*/
	DrawStringToHandle(static_cast<int>(m_itemInfo[Setting].pos.x) + 20, static_cast<int>(m_itemInfo[Setting].pos.y) - 24, "�ݒ�", 0xffffff, m_manager.ReturnFontHandle());
	DrawStringToHandle(static_cast<int>(m_itemInfo[Operation].pos.x) + 20, static_cast<int>(m_itemInfo[Operation].pos.y) - 24, "�������", 0xffffff, m_manager.ReturnFontHandle());
	DrawStringToHandle(static_cast<int>(m_itemInfo[ToTitle].pos.x) + 20, static_cast<int>(m_itemInfo[ToTitle].pos.y) - 24, "�^�C�g��", 0xffffff, m_manager.ReturnFontHandle());
	DrawStringToHandle(static_cast<int>(m_itemInfo[Close].pos.x) + 20, static_cast<int>(m_itemInfo[Close].pos.y) - 24, "����", 0xffffff, m_manager.ReturnFontHandle());

	/*�c���̕`��*/
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			m_afterImage[i]->Draw();
		}
	}

	/*�J�[�\���̕`��*/
	DrawRotaGraph(static_cast<int>(m_cursol.pos.x), static_cast<int>(m_cursol.pos.y), 0.8f, m_angle, m_pHandle, true);

	(this->*drawFunc_)();
}

void PauseScene::CreateAfterImage(Vec2 pos)
{
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (!m_afterImage[i])
		{
			m_afterImage[i] = new AfterImagePlayer;
			m_afterImage[i]->Init(pos, { 1,0 }, 4, false);
			return;
		}
	}
}

void PauseScene::DefaultUpdate(Input& input)
{
	/*���͊֌W*/
	if (input.IsTriggered("pause") || m_cursol.item == Close && input.IsTriggered("OK"))
	{
		m_manager.popScene();
	}

	if (input.IsTriggered("UP"))
	{
		if (m_cursol.item == Operation)
		{
			m_cursol = m_itemInfo[Setting];
		}

		if (m_cursol.item == ToTitle)
		{
			m_cursol = m_itemInfo[Operation];
		}

		if (m_cursol.item == Close)
		{
			m_cursol = m_itemInfo[ToTitle];
		}
	}

	if (input.IsTriggered("DOWN"))
	{
		if (m_cursol.item == ToTitle)
		{
			m_cursol = m_itemInfo[Close];
		}

		if (m_cursol.item == Operation)
		{
			m_cursol = m_itemInfo[ToTitle];
		}

		if (m_cursol.item == Setting)
		{
			m_cursol = m_itemInfo[Operation];
		}
	}

	/*��ԑJ�ڊ֌W*/
	//�ݒ�ɑJ��
	if (m_cursol.item == m_itemInfo[Setting].item)
	{
		drawFunc_ = &PauseScene::SettingDraw;
		if (input.IsTriggered("OK"))
		{
			m_cursol = m_itemInfo[BGM];
			updateFunc_ = &PauseScene::SettingUpdate;
		}
	}

	//��������ɑJ��
	if (m_cursol.item == m_itemInfo[Operation].item)
	{
		drawFunc_ = &PauseScene::OperationDraw;
	}

	//�^�C�g���ɖ߂�ɑJ��
	if (m_cursol.item == m_itemInfo[ToTitle].item)
	{
		drawFunc_ = &PauseScene::ReturnTitleDraw;
		if (input.IsTriggered("OK"))
		{
			m_isFromToTitle = true;
			m_cursol = m_itemInfo[ReturnTitle];
			updateFunc_ = &PauseScene::ReturnTitleUpdate;
		}
	}

	if (m_cursol.item == m_itemInfo[Close].item)
	{
		drawFunc_ = &PauseScene::CloseDraw;
	}
}

void PauseScene::SettingUpdate(Input& input)
{

	//�����ɃJ�[�\���ړ��Ƃ���
	//���ʐݒ�Ƃ����������悤�ɂ���

	if (input.IsTriggered("UP"))
	{
		if (m_cursol.item == m_itemInfo[SE].item)
		{
			m_cursol = m_itemInfo[BGM];
		}
		if (m_cursol.item == m_itemInfo[FullScreen].item)
		{
			m_cursol = m_itemInfo[SE];
		}
	}

	if (input.IsTriggered("DOWN"))
	{
		if (m_cursol.item == m_itemInfo[SE].item)
		{
			m_cursol = m_itemInfo[FullScreen];
		}
		if (m_cursol.item == m_itemInfo[BGM].item)
		{
			m_cursol = m_itemInfo[SE];
		}
	}

	if (m_cursol.item == m_itemInfo[BGM].item)
	{
		if (input.IsTriggered("RIGHT"))
		{
			if (m_BGMnum < 10)
			{
				m_BGMnum++;
			}
			m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		}
		if (input.IsTriggered("LEFT"))
		{
			if (m_BGMnum > 0)
			{
				m_BGMnum--;
			}
			m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		}
	}

	if (m_cursol.item == m_itemInfo[SE].item)
	{
		if (input.IsTriggered("RIGHT"))
		{
			if (m_SEnum < 10)
			{
				m_SEnum++;
			}
			m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		}
		if (input.IsTriggered("LEFT"))
		{
			if (m_SEnum > 0)
			{
				m_SEnum--;
			}
			m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		}
	}

	if (m_cursol.item == m_itemInfo[FullScreen].item)
	{
		if (input.IsTriggered("OK"))
		{
			m_isFullScreen = !m_isFullScreen;
			Application::GetInstance().ChangeScreenSize();
		}
	}

	if (input.IsTriggered("pause") || input.IsTriggered("CANCEL"))
	{
		m_cursol = m_itemInfo[Setting];
		m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		updateFunc_ = &PauseScene::DefaultUpdate;
	}
}

void PauseScene::ReturnTitleUpdate(Input& input)
{
	m_isReturn = true;
	if (input.IsTriggered("OK"))
	{
		//m_manager.popScene();
		m_manager.ChangeAndClearScene(std::make_shared<TitleScene>(m_manager));
	}

	if (input.IsTriggered("pause") || input.IsTriggered("CANCEL"))
	{
		m_isReturn = false;
		m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		m_cursol = m_itemInfo[ToTitle];
		updateFunc_ = &PauseScene::DefaultUpdate;
	}
}

void PauseScene::SettingDraw()
{
	DrawStringToHandle(400, 100, "BGM����", 0xffffff, m_manager.ReturnFontHandle2());
	for (int i = 0; i < 10; i++)
	{
		DrawBox(410 + (kVolumeEmptyBoxSize + 40) * i, 150, 410 + kVolumeEmptyBoxSize + (kVolumeEmptyBoxSize + 40) * i, 150 + kVolumeEmptyBoxSize, 0xffffff, false);
	}

	for (int i = 0; i < m_BGMnum; i++)
	{
		//TODO:�����ڂ�{�Ƃ݂����ɂ�����
		DrawBox(400 + 60 * i, 140, 440 + 60 * i, 180, 0xff00ff, true);
	}

	DrawStringToHandle(400, 200, "SE����", 0xffffff, m_manager.ReturnFontHandle2());
	for (int i = 0; i < 10; i++)
	{
		DrawBox(410 + (kVolumeEmptyBoxSize + 40) * i, 250, 410 + kVolumeEmptyBoxSize + (kVolumeEmptyBoxSize + 40) * i, 250 + kVolumeEmptyBoxSize, 0xffffff, false);
	}

	for (int i = 0; i < m_SEnum; i++)
	{
		//TODO:�����ڂ�{�Ƃ݂����ɂ�����
		DrawBox(400 + 60 * i, 240, 440 + 60 * i, 280, 0xff00ff, true);
	}

	DrawStringToHandle(400, 300, "�t���X�N���[��", 0xffffff, m_manager.ReturnFontHandle2());
	DrawBox(420, 340, 460, 380, 0xffffff, false);

	if (m_isFullScreen)
	{
		//DrawBox(420, 340, 460, 380, 0xff0000, true);
		DrawRotaGraph(440, 360, 0.08f, 0.0f, m_checkHandle, true);
	}
}

void PauseScene::OperationDraw()
{
	DrawStringToHandle(380, 100, "�L�[�{�[�h:", 0xffffff, m_manager.ReturnFontHandle2());
	m_manager.m_gManager.Draw(640, 260, 0.8f, 0.0f, "sousaKey");

	DrawStringToHandle(380, 400, "�R���g���[���[:", 0xffffff, m_manager.ReturnFontHandle2());
	m_manager.m_gManager.Draw(710, 540, 1.4f, 0.0f, "sousaCon");
}

void PauseScene::ReturnTitleDraw()
{
	DrawStringToHandle(400, 100, "�^�C�g���ɖ߂�H", 0xffffff, m_manager.ReturnFontHandle2());
	if (m_isReturn)
	{
		DrawStringToHandle(640, 300, "������x������", 0xffffff, m_manager.ReturnFontHandle2());
		DrawStringToHandle(740, 360, "�߂�܂�", 0xffffff, m_manager.ReturnFontHandle2());
	}
}

void PauseScene::CloseDraw()
{
	//���������Ȃ�(�\��)
}
