#include "OptionScene.h"
#include "Input.h"
#include "Game.h"
#include "SceneManager.h"
#include <DxLib.h>

#include "Application.h"
#include "AfterImagePlayer.h"

namespace
{
	//�c���̍ő吔
	constexpr int kAfterImageNumMax = 64;

	//���ڕʂ̍��W
	const Vec2 kItemPos[ItemNum] = {
		{ 150.0f, 100.0f },
		{ 100.0f, 200.0f },
		{ 130.0f, 300.0f },
		{ 376.0f, 110.0f },
		{ 376.0f, 210.0f },
		{ 376.0f, 310.0f }
	};

	//���ʐݒ�̎l�p�̐�
	constexpr int kVolumeEmptyBoxNum = 10;
	//���ʐݒ�̎l�p�̃T�C�Y
	constexpr int kVolumeEmptyBoxSize = 20;

	//���ʐݒ�̎l�p���m�̊Ԋu
	constexpr int kVolumeEmptyBoxPosXInterval = 40;

	//���ʐݒ�̃e�L�X�g��X���W
	constexpr int kSettingTextPosX = 400;
	//���ʐݒ�̎l�p�̈�ԍ���X���W
	constexpr int kVolumeEmptyBoxPosX = kSettingTextPosX + 10;

	//���ʐݒ��BGM�e�L�X�g��Y���W
	constexpr int kBGMTextPosY = 100;
	//���ʐݒ��BGM�̎l�p��Y���W
	constexpr int kBGMEmptyBoxPosY = kBGMTextPosY + 50;

	//���ʐݒ��SE�e�L�X�g��Y���W
	constexpr int kSETextPosY = kBGMTextPosY + 100;
	//���ʐݒ��SE�̎l�p��Y���W
	constexpr int kSEEmptyBoxPosY = kSETextPosY + 50;

	//���ʐݒ�̎l�p(�s���N�̂��)��X���W
	constexpr int kVolumeBoxPosX = 400;
	//���ʐݒ�̎l�p(�s���N�̂��)�̃T�C�Y
	constexpr int kVolumeBoxSize = 40;
	//���ʐݒ�̎l�p(�s���N�̂��)���m�̊Ԋu
	constexpr int kVolumePosXInterval = 20;

	//���ʐݒ��BGM�̎l�p(�s���N�̂��)��Y���W
	constexpr int kBGMBoxPosY = kBGMTextPosY + 40;
	//���ʐݒ��SE�̎l�p(�s���N�̂��)��Y���W
	constexpr int kSEBoxPosY = kSETextPosY + 40;

	constexpr int kBoxColor = 0xff00ff;
	const Vec2 kCursolOffset = { 24.0f,-24.0f };

	//�t���X�N���[���̃e�L�X�g�`��Y���W
	constexpr int kFullScrennTextPosY = 300;
	//�t���X�N���[���̃`�F�b�N�{�b�N�X��X���W
	constexpr int kFullScreenCheckBoxPosX = 420;
	//�t���X�N���[���̃`�F�b�N�{�b�N�X��Y���W
	constexpr int kFullScreenCheckBoxPosY = 340;
	//�t���X�N���[���̃`�F�b�N�{�b�N�X�̑傫��
	constexpr int kFullScreenCheckBoxSize = 40;

	//�t���X�N���[���̃`�F�b�N�}�[�N��X���W
	constexpr int kFullScreenCheckMarkPosX = 440;
	//�t���X�N���[���̃`�F�b�N�}�[�N��Y���W
	constexpr int kFullScreenCheckMarkPosY = 360;
	//�t���X�N���[���̃`�F�b�N�}�[�N�̊g�嗦
	constexpr float kFullScreenCheckMarkExrate = 0.08f;

	//������@�̃e�L�X�g�̕`����W
	constexpr int kOperationTextPosX = 380;
	constexpr int kOperationKeyBoardTextPosY = 100;
	constexpr int kOperationControllerTextPosY = 400;
	//������@�̉摜�̕`����W
	constexpr int kOperationKeyBoardGraphPosX = 640;
	constexpr int kOperationKeyBoardGraphPosY = 260;
	constexpr int kOperationControllerGraphPosX = 710;
	constexpr int kOperationControllerGraphPosY = 540;
	//������@�̉摜�̊g�嗦
	constexpr float kOperationKeyBoardGraphExrate = 0.8f;
	constexpr float kOperationControllerGraphExrate = 1.4f;


	//�v���C���[�����t���[��������Ƃ���]���邽�߂�Angle
	constexpr float kRotationPlayerAngle = 0.08f;
	//�c���������t���[���Ԋu
	constexpr int kPlayerAfterImageIntervalFrame = 4;
	//��ʒ[����̋���
	constexpr int kRangeFromScreenEdge = 50;
	//�E�B���h�E�̔w�i�F
	constexpr int kWindowBackColor = 0x222222;
	//��(�E�B���h�E�̉��╶���̐F)
	constexpr int kWhite = 0xffffff;
	//�c�������ł�������
	const Vec2 kAfterImageDirection = { 1,0 };
	//�c���̃T�C�Y
	constexpr int kAfterImageSize = 4;
	//�v���C���[�̕`��{��
	constexpr float kPlayerExrate = 0.8f;
	//���ʂ̍ő�
	constexpr int kVolumeMax = 10;
}

OptionScene::OptionScene(SceneManager& manager) :
	Scene(manager)
{
	//�v���C���[�n���h���̎擾
	m_pHandle = m_manager.m_gManager.GetHandle("Player");
	m_checkHandle = m_manager.m_gManager.GetHandle("Check");

	//���ڂ̏�����
	//[0] = �ݒ�
	m_itemInfo[Setting].item = Setting;
	m_itemInfo[Setting].pos = kItemPos[Setting];
	//[1] = ������@
	m_itemInfo[Operation].item = Operation;
	m_itemInfo[Operation].pos = kItemPos[Operation];
	//[2] = ����
	m_itemInfo[Close].item = Close;
	m_itemInfo[Close].pos = kItemPos[Close];
	//[3] = BGM�̉��ʒ���
	m_itemInfo[BGM].item = BGM;
	m_itemInfo[BGM].pos = kItemPos[BGM];
	//[4] = SE�̉��ʒ���
	m_itemInfo[SE].item = SE;
	m_itemInfo[SE].pos = kItemPos[SE];
	//[5] = �t���X�N���[��
	m_itemInfo[FullScreen].item = FullScreen;
	m_itemInfo[FullScreen].pos = kItemPos[FullScreen];

	//�J�[�\���͍ŏ��ɐݒ�̂Ƃ����
	m_cursol = m_itemInfo[Setting];

	m_BGMnum = m_manager.GetBGMvolume();
	m_SEnum = m_manager.GetSEvolume();
	m_isFullScreen = m_manager.GetisFull();

	//�c���̃T�C�Y
	m_afterImage.resize(kAfterImageNumMax);

	updateFunc_ = &OptionScene::DefaultUpdate;
	drawFunc_ = &OptionScene::SettingDraw;
}

OptionScene::~OptionScene()
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

void OptionScene::Update(Input& input)
{
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

	m_frame++;
	m_angle += kRotationPlayerAngle;

	(this->*updateFunc_)(input);

	/*�J�[�\���̎c���֌W*/
	if (m_frame % kPlayerAfterImageIntervalFrame == 0)
	{
		CreateAfterImage(m_cursol.pos - kCursolOffset);
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

void OptionScene::Draw()
{
	/*�g�̕`��*/
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DrawBox(kRangeFromScreenEdge, kRangeFromScreenEdge, Game::kScreenWidth - kRangeFromScreenEdge, Game::kScreenHeight - kRangeFromScreenEdge, kWindowBackColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(kRangeFromScreenEdge, kRangeFromScreenEdge, Game::kScreenWidth - kRangeFromScreenEdge, Game::kScreenHeight - kRangeFromScreenEdge, kWhite, false);

	/*���ڂ̕`��*/
	DrawStringToHandle(static_cast<int>(m_itemInfo[Setting].pos.x), static_cast<int>(m_itemInfo[Setting].pos.y), "�ݒ�", kWhite, m_manager.ReturnFontHandle());
	DrawStringToHandle(static_cast<int>(m_itemInfo[Operation].pos.x), static_cast<int>(m_itemInfo[Operation].pos.y), "�������", kWhite, m_manager.ReturnFontHandle());
	DrawStringToHandle(static_cast<int>(m_itemInfo[Close].pos.x), static_cast<int>(m_itemInfo[Close].pos.y), "����", kWhite, m_manager.ReturnFontHandle());

	/*�c���̕`��*/
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			m_afterImage[i]->Draw();
		}
	}

	/*�J�[�\���̕`��*/
	DrawRotaGraph(static_cast<int>(m_cursol.pos.x - kCursolOffset.x), static_cast<int>(m_cursol.pos.y - kCursolOffset.y), kPlayerExrate, m_angle, m_pHandle, true);

	(this->*drawFunc_)();
}

void OptionScene::CreateAfterImage(Vec2 pos)
{
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (!m_afterImage[i])
		{
			m_afterImage[i] = new AfterImagePlayer;
			m_afterImage[i]->Init(pos, kAfterImageDirection, kAfterImageSize, false);
			return;
		}
	}
}

void OptionScene::DefaultUpdate(Input& input)
{
	/*���͊֌W*/
	if (input.IsTriggered("pause") || m_cursol.item == Close && input.IsTriggered("OK")/*|| input.IsTriggered("CANCEL")*/)
	{
		m_manager.popScene();
	}

	if (input.IsTriggered("UP"))
	{
		if (m_cursol.item == Operation)
		{
			m_cursol = m_itemInfo[Setting];
		}

		if (m_cursol.item == Close)
		{
			m_cursol = m_itemInfo[Operation];
		}
	}

	if (input.IsTriggered("DOWN"))
	{


		if (m_cursol.item == Operation)
		{
			m_cursol = m_itemInfo[Close];
		}

		if (m_cursol.item == Setting)
		{
			m_cursol = m_itemInfo[Operation];
		}
	}

	/*��ԑJ�ڊ֌W*/
	if (m_cursol.item == m_itemInfo[Setting].item)
	{
		drawFunc_ = &OptionScene::SettingDraw;
		if (input.IsTriggered("OK"))
		{
			m_cursol = m_itemInfo[BGM];
			updateFunc_ = &OptionScene::SettingUpdate;
		}
	}

	if (m_cursol.item == m_itemInfo[Operation].item)
	{
		drawFunc_ = &OptionScene::OperationDraw;
	}

	if (m_cursol.item == m_itemInfo[Close].item)
	{
		drawFunc_ = &OptionScene::CloseDraw;
	}

}

void OptionScene::SettingUpdate(Input& input)
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
			if (m_BGMnum < kVolumeMax)
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
			if (m_SEnum < kVolumeMax)
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
		m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		m_cursol = m_itemInfo[Setting];
		updateFunc_ = &OptionScene::DefaultUpdate;
	}
}

void OptionScene::SettingDraw()
{
	DrawStringToHandle(kSettingTextPosX, kBGMTextPosY, "BGM����", kWhite, m_manager.ReturnFontHandle2());
	for (int i = 0; i < kVolumeEmptyBoxNum; i++)
	{
		DrawBox(kVolumeEmptyBoxPosX + (kVolumeEmptyBoxSize + kVolumeEmptyBoxPosXInterval) * i, kBGMEmptyBoxPosY, kVolumeEmptyBoxPosX + kVolumeEmptyBoxSize + (kVolumeEmptyBoxSize + kVolumeEmptyBoxPosXInterval) * i, kBGMEmptyBoxPosY + kVolumeEmptyBoxSize, kWhite, false);
	}

	for (int i = 0; i < m_BGMnum; i++)
	{
		DrawBox(kVolumeBoxPosX + (kVolumeBoxSize + kVolumePosXInterval) * i, kBGMBoxPosY, kVolumeBoxPosX + kVolumeBoxSize + (kVolumeBoxSize + kVolumePosXInterval) * i, kBGMBoxPosY + kVolumeBoxSize, kBoxColor, true);
	}

	DrawStringToHandle(kSettingTextPosX, kSETextPosY, "SE����", kWhite, m_manager.ReturnFontHandle2());
	for (int i = 0; i < kVolumeEmptyBoxNum; i++)
	{
		DrawBox(kVolumeEmptyBoxPosX + (kVolumeEmptyBoxSize + kVolumeEmptyBoxPosXInterval) * i, kSEEmptyBoxPosY, kVolumeEmptyBoxPosX + kVolumeEmptyBoxSize + (kVolumeEmptyBoxSize + kVolumeEmptyBoxPosXInterval) * i, kSEEmptyBoxPosY + kVolumeEmptyBoxSize, kWhite, false);
	}

	for (int i = 0; i < m_SEnum; i++)
	{
		DrawBox(kVolumeBoxPosX + (kVolumeBoxSize + kVolumePosXInterval) * i, kSEBoxPosY, kVolumeBoxPosX + kVolumeBoxSize + (kVolumeBoxSize + kVolumePosXInterval) * i, kSEBoxPosY + kVolumeBoxSize, kBoxColor, true);
	}

	DrawStringToHandle(kSettingTextPosX, kFullScrennTextPosY, "�t���X�N���[��", kWhite, m_manager.ReturnFontHandle2());
	DrawBox(kFullScreenCheckBoxPosX, kFullScreenCheckBoxPosY, kFullScreenCheckBoxPosX + kFullScreenCheckBoxSize, kFullScreenCheckBoxPosY + kFullScreenCheckBoxSize, kWhite, false);

	if (m_isFullScreen)
	{
		DrawRotaGraph(kFullScreenCheckMarkPosX, kFullScreenCheckMarkPosY, kFullScreenCheckMarkExrate, 0.0f, m_checkHandle, true);
	}

}

void OptionScene::OperationDraw()
{
	DrawStringToHandle(kOperationTextPosX, kOperationKeyBoardTextPosY, "�L�[�{�[�h:", kWhite, m_manager.ReturnFontHandle2());
	m_manager.m_gManager.Draw(kOperationKeyBoardGraphPosX, kOperationKeyBoardGraphPosY, kOperationKeyBoardGraphExrate, 0.0f, "sousaKey");

	DrawStringToHandle(kOperationTextPosX, kOperationControllerTextPosY, "�R���g���[���[:", kWhite, m_manager.ReturnFontHandle2());
	m_manager.m_gManager.Draw(kOperationControllerGraphPosX, kOperationControllerGraphPosY, kOperationControllerGraphExrate, 0.0f, "sousaCon");

}

void OptionScene::CloseDraw()
{
	//���������Ȃ�
}
