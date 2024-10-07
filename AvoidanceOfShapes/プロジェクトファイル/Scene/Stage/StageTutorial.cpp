#include "StageTutorial.h"
#include "SceneManager.h"
#include "EnemyLineDown.h"
#include "EnemyLineFromUpper.h"
#include "EnemyLineFromRight.h"
#include "EnemyLineFromLeft.h"

#include "Goal.h"

StageTutorial::StageTutorial(SceneManager& mgr) :
	StageBase(mgr)
{
	m_progressSpeed = 0.0863309f;
}

StageTutorial::~StageTutorial()
{
}

void StageTutorial::Update(Input& input)
{
	if (m_isFirst)
	{
		m_manager.GetPtr(0);
		m_isTutorial = true;
		m_isFirst = false;
	}


	//一定の間をあけて音楽を再生する
	if (m_frame == 60)
	{
		m_manager.m_sManager.PlayBGM("Tutorial", true);
	}

	StageBase::Update(input);

	if (m_frame == 60)
	{
		m_eneCirclePos = { 1000, 0 };
		m_eneCircleRadius = 200;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 110, 200,40);

		m_eneCirclePos = { 1000, 720 };
		m_eneCircleRadius = 200;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 110, 200, 40);
	}

	if (m_frame == 280)
	{
		CreateEnemyBox(Vec2{ 280,160}, 120, 24, 110, 200);
		CreateEnemyBox(Vec2{ 280,720 - 160 }, 120, 24, 110, 200);
	}

	if (m_frame == 500)
	{
		m_lineWidth = 160;
		m_eneLinePos = { 560, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 90, 140, new EnemyLineFromUpper);
	}

	if (m_frame == 700)
	{
		m_lineWidth = 160;
		m_eneLinePos = { 0, 280 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 120, 180, new EnemyLineFromRight);
	}

	if (m_frame == 940)
	{
		m_lineWidth = 160;
		m_eneLinePos = { 560, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 90, 140, new EnemyLineDown);
	}

	if (m_frame == 1140)
	{
		m_lineWidth = 160;
		m_eneLinePos = { 0, 280 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 120, 180, new EnemyLineFromLeft);
	}

	if (m_frame == 1380)
	{
		m_eneCirclePos = { 1000,0 };
		m_eneCircleBurstVec = { 0,1 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 16, 90);
	}

	if (m_frame == 1610)
	{
		m_eneCirclePos = { 1000,720 };
		m_eneCircleBurstVec = { 0,-1 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 16, 90);
	}

	if (m_frame == 1800)
	{
		m_eneCircleRadius = 160;
		m_eneCirclePos = { 1280,200 };
		CreateEnemyCircle(m_eneCirclePos.y, m_eneCircleRadius, false);
	}

	if (m_frame == 2020)
	{
		m_eneCircleRadius = 160;
		m_eneCirclePos = { 1280,520 };
		CreateEnemyCircle(m_eneCirclePos.y, m_eneCircleRadius, false);
	}

	if (m_frame == 2240)
	{
		m_lineWidth = 80;
		m_eneLinePos = { 0, 320 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 120, 400, new EnemyLineFromRight);

		m_eneCircleRadius = 120;
		m_eneCirclePos = { 1280,160 };
		CreateEnemyCircle(m_eneCirclePos.y, m_eneCircleRadius, false);
	}
	
	if (m_frame == 2440)
	{
		m_eneCircleRadius = 120;
		m_eneCirclePos = { 1280,560 };
		CreateEnemyCircle(m_eneCirclePos.y, m_eneCircleRadius, false);
	}

	if (m_frame == 2780)
	{
		m_goalFlag = true;
	}

	if (m_frame <= 2780)
	{
		m_progressPos += m_progressSpeed;
	}


	if (m_goalFlag)
	{
		if (!m_goal->isExist())
		{
			delete m_goal;
			m_goal = nullptr;
		}
		else
		{
			m_goal->Update();

			Col goalCol = m_goal->GetColRect();

			//プレイヤーとゴールが当たった時の処理
			if (m_playerColGoal.IsCollision(goalCol))
			{
				m_goal->Hit();
				m_manager.SetStageClearFlag(0);
				m_onGoal = true;
			}
		}
	}
}

void StageTutorial::Draw()
{

	StageBase::Draw();
}
