#include "StageBase.h"
#include "SceneManager.h"
#include "StageSelect.h"
#include "GameoverScene.h"
#include "PauseScene.h"
#include "Player.h"
#include "GoalEfect.h"
#include "Input.h"
#include "EnemyLineBase.h"
#include "EnemyBox.h"
#include "EnemyLineFromUpper.h"
#include "EnemyLineDown.h"
#include "EnemyLineFromLeft.h"
#include "EnemyLineFromRight.h"
#include "EnemyCircleBase.h"
#include "EnemyCircle.h"
#include "EnemyBurst.h"
#include "EnemyCircleScaling.h"
#include "Goal.h"

#include "StageTutorial.h"
namespace
{
	//�t�F�[�h�C���A�t�F�[�h�A�E�g�ɂ����鎞��
	constexpr int kFadeFrame = 30;

	//��ʂ���؂�Box�̍���
	constexpr int kWidth = 80;
	//��ʂ���؂�Box�̕�
	constexpr int kHeight = 80;

	//��ʕ���kWidth�Ŋ�������
	constexpr int kWidthNum = Game::kScreenWidth / kWidth;
	//��ʍ�����kHeight�Ŋ�������
	constexpr int kHeightNum = Game::kScreenHeight / kHeight;
}

StageBase::StageBase(SceneManager& mgr) :
	Scene(mgr),
	m_isFirst(true),
	m_isEndFadeIn(false),
	m_isHit(false),
	m_eneLinePos(0.0f, 0.0f),
	m_lineWidth(0),
	m_eneCirclePos(0, 0),
	m_eneCircleBurstVec(0, 0),
	m_eneCircleRadius(0),
	m_eneCircleBurstSpeed(0),
	m_frame(0),
	m_goalFlag(false),
	m_onGoal(false),
	m_progressSpeed(0.0f),
	m_progressPos(520.0f),
	m_screenX(0),
	m_screenY(0),
	m_shakeFrame(0),
	m_gameScreenHandle(0),
	m_amp(0),
	m_isStartGoalEfect(false),
	m_goalEfectFrame(0),
	m_isSoundFadeOut(false),
	m_soundFadeFrame(0),
	m_isHitFlash(false),
	m_alpha(0),
	m_isTutorial(false),
	m_mag(0.0f),
	m_fadeoutAngle(-10.2f),
	m_magSpeed(0.02f),
	m_fadeoutAngleSpeed(0.2f),
	m_fadeFrame(0)
{

	//�Q�[����ʕ`���̐���
	//��ʃT�C�Y�Ɠ����傫���̃O���t�B�b�N�f�[�^�𐶐�����
	m_gameScreenHandle = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	//�t�F�[�h�C�����̃A�j���[�V�����Ɏg��
	//�l�p��X���W��ݒ�
	m_posX.resize(kWidthNum);
	for (int i = 0; i < m_posX.size(); i++)
	{
		m_posX[i] = 40 + 80 * i;
	}
	//�t�F�[�h�C�����̃A�j���[�V�����Ɏg��
	//�l�p��Y���W��ݒ�
	m_posY.resize(kHeightNum);
	for (int i = 0; i < m_posY.size(); i++)
	{
		m_posY[i] = 40 + 80 * i;
	}

	//�v���C���[�̏�����
	m_player = new Player;
	m_player->Init(true);

	//�v���C���[�̉摜��m_player�ɓn��
	int playerHandle[4];
	playerHandle[0] = m_manager.m_gManager.GetHandle("Player");
	playerHandle[1] = m_manager.m_gManager.GetHandle("Player1");
	playerHandle[2] = m_manager.m_gManager.GetHandle("Player2");
	playerHandle[3] = m_manager.m_gManager.GetHandle("Player3");

	//�v���C���[�Ƀn���h����n��
	m_player->SetPlayerHandle(playerHandle[0], playerHandle[1], playerHandle[2], playerHandle[3]);

	//�S�[���̏�����
	m_goal = new Goal;
	m_goal->Init();
	m_goal->SetGraphHandle(m_manager.m_gManager.GetHandle("Goal"));

	//�S�[���G�t�F�N�g
	m_goalEfect = new GoalEfect;

	updateFunc_ = &StageBase::FadeInUpdate;
	drawFunc_ = &StageBase::FadeInDraw;
	m_fadeFrame = 0;
}

StageBase::~StageBase()
{
	//�v���C���[�̃������폜
	delete m_player;
	m_player = nullptr;

	//�G(Line�n)�̃������폜
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (m_eneLine[i] != nullptr)
		{
			delete m_eneLine[i];
			m_eneLine[i] = nullptr;
		}
	}

	//�G(Circle�n)�̃������폜
	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (m_eneCircle[i] != nullptr)
		{
			delete m_eneCircle[i];
			m_eneCircle[i] = nullptr;
		}
	}

	//�S�[���̃������폜
	delete m_goal;
	m_goal = nullptr;

	DeleteGraph(m_gameScreenHandle);

}

void StageBase::Update(Input& input)
{
	//�t���[�������J�E���g
	m_frame++;

	//�X�V
	if (m_isEndFadeIn)
	{
		m_player->Update(input);
	}
	//enemy->Update();

	//�����蔻��̐ݒ�
	m_playerColEnemy = m_player->GetColRect();
	m_playerColGoal = m_player->GetColRectGoal();

	//enemyLine�̍X�V
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (m_eneLine[i])
		{
			m_eneLine[i]->Update();

			//�g�p�ς݂̓G�L�����N�^���폜����K�v������
			if (!m_eneLine[i]->isExist())
			{
				//���������������
				delete m_eneLine[i];
				m_eneLine[i] = nullptr;
			}
			//���݂���G�̏ꍇ
			else
			{
				//�������Ă��Ȃ���Ԃ̎�
				if (!m_isHit)
				{
					//��ʂ��h��Ă��Ȃ����
					if (m_shakeFrame == 0)
					{
						//�G����ʒp�ɂ������̐U�����擾
						m_shakeFrame = m_eneLine[i]->GetShakeFrame();
						//�h�ꕝ�������l�ɖ߂�
						m_amp = 20.0f;
					}

					//�G�̓����蔻����󂯎����
					Col enemyLineCol = m_eneLine[i]->GetColRect();
					//�v���C���[�ƓG���������Ă�����
					if (m_playerColEnemy.IsCollision(enemyLineCol))
					{
						//�����������̏���������
						PlayerHit();
						m_isHit = true;
					}
				}
			}
		}
	}
	
	//�s�v�ɂȂ����G�������ō폜��������
	auto lIt = remove_if(m_eneLine.begin(), m_eneLine.end(), [](auto& v) {
		return v == nullptr;
	});
	m_eneLine.erase(lIt, m_eneLine.end());

	//eneCircle�̍X�V����
	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (m_eneCircle[i])
		{
			m_eneCircle[i]->Update();

			//�g�p�ς݂̓G�L�����N�^���폜����K�v������
			if (!m_eneCircle[i]->isExist())
			{
				//���������������
				delete m_eneCircle[i];
				m_eneCircle[i] = nullptr;
			}
			else
			{
				if (!m_isHit)
				{
					//�G�̓����蔻����󂯎����
					Col enemyCircleCol = m_eneCircle[i]->GetColRect();
					//�v���C���[�ƓG���������Ă�����
					if (m_playerColEnemy.IsCollisionCircle(enemyCircleCol))
					{
						//�����������̏���������
						PlayerHit();
						m_isHit = true;
					}
				}
			}
		}
	}

	//�����G�ɓ������Ă�����
	if (m_isHit)
	{
		m_isHit = false;
	}

	//�s�v�ɂȂ����G�������ō폜��������
	auto cIt = remove_if(m_eneCircle.begin(), m_eneCircle.end(), [](auto& v) {
		return v == nullptr;
		});
	m_eneCircle.erase(cIt, m_eneCircle.end());

	//��ʗh��t���[���̃J�E���g�_�E��
	if (m_shakeFrame > 0)
	{
		//��ʂ�h�炷�t���[�������炷
		m_shakeFrame--;
		//��ʂ��h��镝�̔{�������������炷
		m_amp *= 0.88f;
		//������x�A�l���������Ȃ��0�ɕ␳����
		if (m_amp < 0.01f)
		{
			m_amp = 0.0f;
		}
	}

#ifdef _DEBUG
	if (input.IsTriggered("Goal"))
	{
		m_goalFlag = true;
	}
#endif

	//�S�[��������
	if (m_onGoal)
	{
		//�S�[���G�t�F�N�g�J�n�t���O��true�ɂ���
		m_isStartGoalEfect = true;
		//�S�[���G�t�F�N�g�̏�����
		m_goalEfect->Init(m_player->GetPlayerPos());
		//�S�[������SE�̗���
		m_manager.m_sManager.PlaySE("GoalSE");
		//�S�[�������t���O��false�ɂ���
		m_onGoal = false;
	}

	//�S�[���G�t�F�N�g�J�n�t���O��true�ɂȂ����Ƃ�
	if (m_isStartGoalEfect)
	{
		//�S�[���G�t�F�N�g���n�܂��Ă���̃t���[�����X�V����
		m_goalEfectFrame++;
		//�S�[���G�t�F�N�g�̍X�V����������
		m_goalEfect->Update();
	}

	//BGM���t�F�[�h�A�E�g����^�C�~���O��������
	if (m_isSoundFadeOut)
	{
		//�T�E���h�̃t�F�[�h�A�E�g�J�n����̃t���[�����X�V����
		m_soundFadeFrame++;
	}

	//�G�ɓ�����������ʂ��s���N�F�œ_�ł����邽�߂̏���
	if (m_isHitFlash)
	{
		//�A���t�@�l�𑝂₷
		m_alpha += 100;
		if (m_alpha > 200)
		{
			m_isHitFlash = false;
		}
	}
	else //�G�ɓ������Ă��Ȃ��Ƃ���alpha��0�ȏ�̎���0�ɂ���
	{
		if (m_alpha > 0)
		{
			m_alpha -= 40;
		}
	}

	(this->*updateFunc_)(input);
}

void StageBase::Draw()
{
	SetDrawScreen(m_gameScreenHandle);
	//�`���X�N���[�����N���A����
	ClearDrawScreen();

	//�w�i�̕`��
	DrawBox(0, 0, 1280, 720, 0x0d1118, true);

	//�`���[�g���A���X�e�[�W�̎��̓e�L�X�g��\������
	if (m_isTutorial)
	{
		if (m_frame <= 1900)
		{
			m_manager.m_gManager.Draw(640, 360, 1.0f, 0.0f, "Tutorial1");
		}
		else if (m_frame <= 2780)
		{
			m_manager.m_gManager.Draw(640, 360, 1.0f, 0.0f, "Tutorial2");
		}
		else
		{
			m_manager.m_gManager.Draw(640, 360, 1.0f, 0.0f, "Tutorial3");
		}
	}


	//�i���̊D�F�̕����̕`��
	DrawBox(520, 20, 760, 30, 0x222222, true);
	//�i���̐��F�̕����̕`��
	DrawBox(520, 20, static_cast<int>(m_progressPos), 30, 0x00ffff, true);

	//�S�[���G�t�F�N�g�̕`��
	if (m_isStartGoalEfect)
	{
		m_goalEfect->Draw();
	}

	//Line�n�̓G�̕`��
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (m_eneLine[i])
		{
			m_eneLine[i]->Draw();
		}
	}

	//Circle�n�̓G�̕`��
	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (m_eneCircle[i])
		{
			m_eneCircle[i]->Draw();
		}
	}

	//�v���C���[�̕`��
	m_player->Draw();

	//�S�[���̕`��
	if (m_goalFlag)
	{
		if (!m_goal->isExist())
		{
			m_goalFlag = false;
			delete m_goal;
			m_goal = nullptr;
		}
		else
		{
			m_goal->Draw();
		}
	}

	//�G�ɓ����������̉�ʂ̓_�ł�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x660066, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	//�f�o�b�O�p
	//DrawFormatString(640, 0, 0xffffff, "%.2f", nowTime * 0.000001f);
	DrawFormatString(640, 16, 0xff0000, "%d", m_frame);
#endif

	//�Q�[����ʂ��o�b�N�o�b�t�@�ɕ`�悷��
	//�o�b�N�o�b�t�@�ɏ������ސݒ�ɖ߂��Ă���
	SetDrawScreen(DX_SCREEN_BACK);
	int screenX = 0;
	int screenY = 0;

	if (m_shakeFrame > 0)
	{
		//��ʗh��
		screenX = static_cast<int>(m_amp) * (GetRand(2) - 1);
		screenY = static_cast<int>(m_amp) * (GetRand(2) - 1);
	}


	//�t�F�[�h�C���̕`��
	for (int y = 0; y < m_posY.size(); y++)
	{
		for (int x = 0; x < m_posX.size(); x++)
		{
			DrawRectRotaGraph(m_posX[x] + screenX, m_posY[y] + screenY, 80 * x, 80 * y, kWidth, kHeight, m_mag, m_fadeoutAngle, m_gameScreenHandle, false);
		}
	}

	(this->*drawFunc_)();

}

void StageBase::CreateEnemyLineFrom(Vec2 pos, int width, int first, int second, EnemyLineBase* enemy)
{
	m_eneLine.resize(m_eneLine.size() + 1);
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (!m_eneLine[i])
		{
			m_eneLine[i] = enemy;
			m_eneLine[i]->Init(pos, width,first,second);
			return;
		}
	}
	delete enemy;
}

void StageBase::CreateEnemyBox(Vec2 pos, int size, int speed, int firstFrame, int secondFrame,bool isPre)
{
	m_eneLine.resize(m_eneLine.size() + 1);
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (!m_eneLine[i])
		{
			m_eneLine[i] = new EnemyBox;
			m_eneLine[i]->Init(pos, size, speed, firstFrame, secondFrame,isPre);
			return;
		}
	}
}

void StageBase::CreateEnemyCircle(float pos, int radius, bool isSmooth)
{
	m_eneCircle.resize(m_eneCircle.size() + 1);

	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (!m_eneCircle[i])
		{
			m_eneCircle[i] = new EnemyCircle;
			m_eneCircle[i]->Init(pos, radius, isSmooth);
			return;
		}
	}
}

void StageBase::CreateEnemyBurst(Vec2 pos, Vec2 vec, int speed, int first, int second)
{
	m_eneCircle.resize(m_eneCircle.size() + 1);

	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (!m_eneCircle[i])
		{
			m_eneCircle[i] = new EnemyBurst(this);
			m_eneCircle[i]->Init(pos, vec, speed,first,second);
			return;
		}
	}
}

void StageBase::CreateEnemyCircleScaling(Vec2 pos, int rad, int first, int second, int speed)
{
	m_eneCircle.resize(m_eneCircle.size() + 1);

	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (!m_eneCircle[i])
		{
			m_eneCircle[i] = new EnemyCircleScaling;
			m_eneCircle[i]->Init(pos, rad, first, second,speed);
			return;
		}
	}
}

void StageBase::AddEnemy(EnemyLineBase* test)
{
	m_eneLine.resize(m_eneLine.size() + 1);
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (!m_eneLine[i])
		{
			m_eneLine[i] = test;
			m_eneLine[i]->Init();
			return;
		}
	}
}

void StageBase::PlayerHit()
{
	m_player->HitEnemy();
	m_shakeFrame = 16;
	m_amp = 20;
	m_manager.m_sManager.PlaySE("hitSE");
	m_isHitFlash = true;
}

void StageBase::FadeInUpdate(Input& input)
{
	if (m_mag < 1)
	{
		m_mag += m_magSpeed;
		m_fadeoutAngle += m_fadeoutAngleSpeed;
	}
	else
	{
		m_mag = 1;
	}

	if (m_mag == 1)
	{
		updateFunc_ = &StageBase::NormalUpdate;
		drawFunc_ = &StageBase::NormalDraw;
	}
}

void StageBase::NormalUpdate(Input& input)
{
	if (!m_isEndFadeIn)
	{
		m_isEndFadeIn = true;
	}

	if (input.IsTriggered("end"))
	{
		updateFunc_ = &StageBase::FadeOutUpdate;
		drawFunc_ = &StageBase::FadeOutDraw;
	}

	if (input.IsTriggered("pause"))
	{
		m_shakeFrame = 0;

		m_manager.m_sManager.StopBGM("stage1");
		m_manager.m_sManager.StopBGM("stage2");
		m_manager.m_sManager.StopBGM("Tutorial");
		m_manager.PushScene(std::make_shared<PauseScene>(m_manager));
	}

	if (m_player->ReturnHitEnemyNum() >= 4 || m_goalEfectFrame >= 120)
	{
		updateFunc_ = &StageBase::FadeOutUpdate;
		drawFunc_ = &StageBase::FadeOutDraw;
	}
}

void StageBase::FadeOutUpdate(Input& input)
{
	m_fadeFrame++;
	if (m_fadeFrame >= kFadeFrame)
	{
		m_manager.m_sManager.StopBGM("stage1");
		m_manager.m_sManager.StopBGM("stage2");
		m_manager.m_sManager.StopBGM("Tutorial");

		if (m_player->ReturnHitEnemyNum() >= 4)
		{
			m_manager.ChangeScene(std::make_shared<GameoverScene>(m_manager));
		}
		else
		{
			m_manager.ChangeScene(std::make_shared<StageSelect>(m_manager));
		}
	}
}

void StageBase::FadeInDraw()
{

}

void StageBase::FadeOutDraw()
{
	float rate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);
	float alpha = 255.0f * rate;
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(alpha));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void StageBase::NormalDraw()
{
}
