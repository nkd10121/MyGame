#include "SceneManager.h"
#include "Scene.h"
#include "Application.h"
#include "Game.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace
{
	//�w�i�̐F
	constexpr int kBGcolor = 0x0d1118;

	constexpr int kDefaultBGMvolume = 5;
	constexpr int kDefaultSEvolume = 3;

	constexpr int kFontSize = 24;
	constexpr int kFontDoubleSize = kFontSize * 2;
}

enum StageNo
{
	Tutorial,
	Stage1,
	Stage2
};

void SceneManager::Save()
{
	std::ofstream file("Data.txt");  // �����o���t�@�C���̃p�X���w��
	int info[5] = { m_BGMvolume,m_SEvolume,m_clearStageTutorial,m_clearStage1,m_clearStage2 };
	for (const auto test : info)
	{
		file << test << std::endl;  // �����o��
	}
}

void SceneManager::Load()
{
	std::ifstream data("Data.txt");  // �ǂݍ��ރt�@�C���̃p�X���w��
	std::string line;

	//Data.txt�����݂��Ȃ��Ƃ��A�����l�Ƃ���5��3������ďI���
	if (!data) {
		m_BGMvolume = kDefaultBGMvolume;
		m_SEvolume = kDefaultSEvolume;
		m_clearStageTutorial = false;
		m_clearStage1 = false;
		m_clearStage2 = false;
		return;
	}

	//�f�[�^�����[�h����
	std::getline(data, line);
	m_BGMvolume = stoi(line);

	std::getline(data, line);
	m_SEvolume = stoi(line);

	std::getline(data, line);
	m_clearStageTutorial = stoi(line);

	std::getline(data, line);
	m_clearStage1 = stoi(line);

	std::getline(data, line);
	m_clearStage2 = stoi(line);

}

SceneManager::SceneManager() :
	m_isFull(false)
{
	//�ŏ��Ƀ��[�h����
	Load();

	m_isFull = Application::GetInstance().GetIsFullScreen();

	//�g�p����t�H���g����������
	m_fontPath = "data/font/PixelMplus12-Bold.ttf"; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(m_fontPath, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// �t�H���g�Ǎ��G���[����
		MessageBox(NULL, "�t�H���g�Ǎ����s", "", MB_OK);
	}

	//�^�C�g���\���p�̃t�H���g
	m_fontHandle = CreateFontToHandle("PixelMplus12", kFontDoubleSize, -1);
	m_fontHandle2 = CreateFontToHandle("PixelMplus12", kFontSize, -1);

	/*UI�Ȃ�*/
	//�v���C���[�摜
	LoadG("Player", "data/player/player0.png");
	LoadG("Player1", "data/player/player1.png");
	LoadG("Player2", "data/player/player2.png");
	LoadG("Player3", "data/player/player3.png");

	//�w�i�̎l�p�̉摜
	LoadG("BG", "data/titleParts.png");
	//�^�C�g���̃��S
	LoadG("Title", "data/title.png");
	//�{�^��1
	LoadG("Button1", "data/Button1.png");
	//�{�^��2
	LoadG("Button2", "data/Button2.png");
	//�{�^��3
	LoadG("Button3", "data/Button3.png");
	//�I�v�V�����A�|�[�Y�̃t���X�N���[���̃`�F�b�N�}�[�N
	LoadG("Check", "data/UI_check.png");
	//�S�[���̎O�p�`
	LoadG("Goal", "data/goal.png");

	/*������@�֘A*/
	//�R���g���[���[�̑�������̉摜
	LoadG("sousaCon", "data/sousa.png");
	//�L�[�{�[�h�̑�������̉摜
	LoadG("sousaKey", "data/sousaKeyboard.png");

	/*�X�e�[�W�Z���N�g�̃X�e�[�W�̉摜*/
	//�`���[�g���A���̉摜
	LoadG("stageSelect0", "data/stageSelect0.png");
	//�Â��\�����邽�߂̐^���Âɂ����摜
	LoadG("stageSelect0B", "data/stageSelect0Black.png");
	//Stage1�̉摜
	LoadG("stageSelect1", "data/stageSelect1.png");
	//�Â��\�����邽�߂̐^���Âɂ����摜
	LoadG("stageSelect1B", "data/stageSelect1Black.png");
	//Stage2�̉摜
	LoadG("stageSelect2", "data/stageSelect2.png");
	//�Â��\�����邽�߂̐^���Âɂ����摜
	LoadG("stageSelect2B", "data/stageSelect2Black.png");

	/*�`���[�g���A���ŕ\�����镶��*/
	LoadG("Tutorial1", "data/text.png");
	LoadG("Tutorial2", "data/text2.png");
	LoadG("Tutorial3", "data/text3.png");

	/*movie�֘A*/
	//�^�C�g���ŗ�������
	LoadG("TitleMovie", "data/movie/title.mp4");
	LoadG("DemoText", "data/demoText.png");

	/*BMG�֘A*/
	m_sManager.Load("Title", "data/sound/Player.mp3",true);
	m_sManager.Load("Tutorial", "data/sound/Electric_Shine.mp3", true);
	m_sManager.Load("stage1", "data/sound/Kagura.mp3", true);
	m_sManager.Load("stage2", "data/sound/15 80's Graffiti .mp4", true);
	m_sManager.Load("GameOver", "data/sound/�s��_-_8bit.mp3", true);

	//�v���C���[��������������SE
	m_sManager.Load("hitSE", "data/se/�q�b�g�Q����3.wav", false);
	//���肵������SE
	m_sManager.Load("enterSE", "data/se/����Q������iSF�n�j.wav", false);
	//�J�[�\���𓮂������Ƃ���SE
	m_sManager.Load("moveSE", "data/se/�J�[�\���i�_�炩���j2.wav", false);
	//�S�[����������SE
	m_sManager.Load("GoalSE", "data/se/maou_se_magical27.mp3", false);
	//�Q�[���I�[�o�[���畜������Ƃ�
	m_sManager.Load("RevivalSE", "data/se/�Q�[�W��1.mp3", false);
	//�Q�[���I�[�o�[�ł�����߂��Ƃ�
	m_sManager.Load("deathSE", "data/se/�ˌ��Q�s�X�g��.wav", false);
}

SceneManager::~SceneManager()
{
	// ********** �t�H���g�̃A�����[�h **********
	if (RemoveFontResourceEx(m_fontPath, FR_PRIVATE, NULL)) {
	}
	//�쐬�����t�H���g������������������
	DeleteFontToHandle(m_fontHandle);

	//�Ō�Ƀ��[�h����
	Save();
}

void SceneManager::Update(Input& input)
{
	//BG
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBGcolor, true);

	m_sManager.ChangeBGMVolume(m_BGMvolume);
	m_sManager.ChangeSEVolume(m_SEvolume);

	//�����̂ݎ��s
	scenes.back()->Update(input);
}

void SceneManager::Draw()
{
	//�擪���珇�ɕ`��
	//�Ō�ɐς񂾂��̂��Ō�ɕ`�悳���
	for (auto& scenes : scenes)
	{
		scenes->Draw();
	}
}

void SceneManager::ChangeScene(std::shared_ptr<Scene> nextScene)
{
	if (scenes.empty())//���X�g������ۂ����������ւ�����
	{
		scenes.push_back(nextScene);//�����ɒǉ�
	}
	else
	{
		m_logScene = *(scenes.begin());
		scenes.back() = nextScene;//���Ɉ�ȏ゠��Ζ�����
	}
}

void SceneManager::ChangeAndClearScene(std::shared_ptr<Scene> nextScene)
{
	scenes.clear();

	scenes.push_back(nextScene);//�����ɒǉ�
	
}

void SceneManager::PushScene(std::shared_ptr<Scene> scene)
{
	scenes.push_back(scene);
}

void SceneManager::popScene(bool isToTitle)
{
	//�V�[���̃��O���X�V����
	m_logScene = *(scenes.begin());
	if (!isToTitle)
	{
		//�������O�̃V�[����stage1��������
		//stage1��BGM��r������Đ�����
		if (m_logScene == m_stage1)
		{
			m_sManager.PlayBGM("stage1", false);
		}
		//�������O�̃V�[����stage2��������
		//stage2��BGM��r������Đ�����
		else if (m_logScene == m_stage2)
		{
			m_sManager.PlayBGM("stage2", false);
		}
		else if (m_logScene == m_StageTutorial)
		{
			m_sManager.PlayBGM("Tutorial", false);
		}
	}

	scenes.pop_back();
}

void SceneManager::GetPtr(int stageNum)
{
	switch (stageNum)
	{
	case Tutorial:
		{
			m_StageTutorial = *(scenes.begin());
		}
		break;
	case Stage1:
		{
			m_stage1 = *(scenes.begin());
		}
		break;
	case Stage2:
		{
			m_stage2 = *(scenes.begin());
		}
		break;
	case 3:
		m_pauseScene = *(scenes.begin());
		break;
	}
}

void SceneManager::LoadG(std::string name, std::string path)
{
	m_gManager.Load(name, path);
}

void SceneManager::Draw(int x, int y, float exRate, float angle, std::string name)
{
	m_gManager.Draw(x, y, exRate, angle, name);
}


void SceneManager::SetStageClearFlag(int stageNum)
{
	switch (stageNum)
	{
	case Tutorial:
		m_clearStageTutorial = true;
		return;
	case Stage1:
		m_clearStage1 = true;
		return;
	case Stage2:
		m_clearStage2 = true;
		return;
	default:
		return;
	}
}

bool SceneManager::GetStageClearFlag(int stageNum)
{
	switch (stageNum)
	{
	case Tutorial:
		return m_clearStageTutorial;
	case Stage1:
		return m_clearStage1;
	case Stage2:
		return m_clearStage2;
	default:
		return false;
	}
}

void SceneManager::SetInfo(int BGMvolume, int SEvolume, bool isFull)
{
	SetBGMvolume(BGMvolume);
	SetSEvolume(SEvolume);
	SetIsFull(isFull);
}
