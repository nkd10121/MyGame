#include "Stage1.h"
#include "SceneManager.h"
#include "Goal.h"
#include "Col.h"

#include "EnemyLineFromRight.h"
#include "EnemyLineFromLeft.h"
#include "EnemyLineDown.h"

namespace
{
	//1回目の花火のフレーム
	constexpr int kFirstBurstFrame = 960;
	//2回目の花火のフレーム
	constexpr int kSecondBurstFrame = 1840;
	//3回目の花火のフレーム
	constexpr int kThirdBurstFrame = 2720;
	//花火のインターバル
	constexpr int kInterval = 55;
}

Stage1::Stage1(SceneManager& mgr) :
	StageBase(mgr)
{
	m_progressSpeed = 0.0538116f;
}

Stage1::~Stage1()
{
	m_manager.m_sManager.StopBGM("stage1");

}

void Stage1::Update(Input& input)
{
	if (m_isFirst)
	{
		m_manager.GetPtr(1);
		m_isFirst = false;
	}

	if (m_frame == 60)
	{
		m_manager.m_sManager.PlayBGM("stage1", true);
	}

	if (m_frame == 250)
	{
		m_eneCirclePos = { 320, 720 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 250 + 20)
	{
		m_eneCirclePos = { 960, 720 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 410)
	{
		m_eneCirclePos = { 320, 0 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 410 + 20)
	{
		m_eneCirclePos = { 960, 0 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 410 + 60)
	{
		m_eneCirclePos = { 640,0 };
		m_eneCircleBurstVec = { 0,1 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 8, 30);
	}

	if (m_frame == 680)
	{
		m_eneCirclePos = { 320, 720 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 680 + 20)
	{
		m_eneCirclePos = { 960, 720 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 840)
	{
		m_eneCirclePos = { 320, 0 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 840 + 20)
	{
		m_eneCirclePos = { 960, 0 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 840 + 60)
	{
		m_eneCirclePos = { 640,0 };
		m_eneCircleBurstVec = { 0,1 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 8, 30);
	}

	if (m_frame == kFirstBurstFrame)
	{
		CreateFireFlower(true, 28);
	}
	if (m_frame == kFirstBurstFrame + kInterval)
	{
		CreateFireFlower(false, 30);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 2)
	{
		CreateFireFlower(true, 24);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 3)
	{
		CreateFireFlower(false, 26);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 4)
	{
		CreateFireFlower(true, 28);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 5)
	{
		CreateFireFlower(false, 22);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 6)
	{
		CreateFireFlower(true, 26);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 7)
	{
		CreateFireFlower(false, 28);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 8)
	{
		CreateFireFlower(true, 30);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 9)
	{
		CreateFireFlower(false, 28);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 10)
	{
		CreateFireFlower(true, 26);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 11)
	{
		CreateFireFlower(false, 22);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 12)
	{
		CreateFireFlower(true, 24);
	}
	if (m_frame == kFirstBurstFrame + kInterval * 13)
	{
		CreateFireFlower(false, 26);
	}

	if (m_frame == 1748)
	{
		m_eneCirclePos = { 640, 160 };
		m_eneCircleRadius = 40;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 80, 80);
	}
	if (m_frame == 1748 + 14 * 1)
	{
		m_eneCirclePos = { 640, 160 };
		m_eneCircleRadius = 80;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 80 - 14 * 1, 80 - 14 * 1);
	}
	if (m_frame == 1748 + 14 * 2)
	{
		m_eneCirclePos = { 640, 160 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 80 - 14 * 2, 80 - 14 * 2);
	}
	if (m_frame == 1748 + 14 * 3)
	{
		m_eneCirclePos = { 640, 160 };
		m_eneCircleRadius = 160;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius,60,80);
	}


	if (m_frame == kSecondBurstFrame + kInterval * 0)
	{
		CreateFireFlower(true, 30);
		CreateUnderLine(false);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 1)
	{
		CreateFireFlower(false, 22);
		CreateUnderLine(true);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 2)
	{
		CreateFireFlower(true, 24);
		CreateUnderLine(false);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 3)
	{
		CreateFireFlower(false, 28);
		CreateUnderLine(true);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 4)
	{
		CreateFireFlower(true, 32);
		CreateUnderLine(false);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 5)
	{
		CreateFireFlower(false, 26);
		CreateUnderLine(true);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 6)
	{
		CreateFireFlower(true, 30);
		CreateUnderLine(false);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 7)
	{
		CreateFireFlower(false, 22);
		CreateUnderLine(true);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 8)
	{
		CreateFireFlower(true, 24);
		CreateUnderLine(false);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 9)
	{
		CreateFireFlower(false, 26);
		CreateUnderLine(true);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 10)
	{
		CreateFireFlower(true, 28);
		CreateUnderLine(false);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 11)
	{
		CreateFireFlower(false, 26);
		CreateUnderLine(true);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 12)
	{
		CreateFireFlower(true, 30);
		CreateUnderLine(false);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 13)
	{
		CreateFireFlower(false, 24);
		CreateUnderLine(true);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 14)
	{
		CreateFireFlower(true, 26);
		CreateUnderLine(false);
	}
	if (m_frame == kSecondBurstFrame + kInterval * 15)
	{
		CreateFireFlower(false, 28);
		CreateUnderLine(true);
	}

	if (m_frame == 2680)
	{
		m_eneCirclePos = { 640, 160 };
		m_eneCircleRadius = 160;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 40, 60);
	}

	if (m_frame == kThirdBurstFrame + kInterval * 0)
	{
		CreateFireFlower(false, 34);
		CreateFireFlower(true, 34);
		CreateUnderLine(false);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 1)
	{
		CreateFireFlower(true, 28);
		CreateFireFlower(false, 28);
		CreateUnderLine(true);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 2)
	{
		CreateFireFlower(false, 22);
		CreateFireFlower(true, 22);
		CreateUnderLine(false);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 3)
	{
		CreateFireFlower(true, 32);
		CreateFireFlower(false, 32);
		CreateUnderLine(true);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 4)
	{
		CreateFireFlower(false, 28);
		CreateFireFlower(true, 28);
		CreateUnderLine(false);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 5)
	{
		CreateFireFlower(true, 26);
		CreateFireFlower(false, 26);
		CreateUnderLine(true);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 6)
	{
		CreateFireFlower(false, 24);
		CreateFireFlower(true, 24);
		CreateUnderLine(false);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 7)
	{
		CreateFireFlower(true, 30);
		CreateFireFlower(false, 30);
		CreateUnderLine(true);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 8)
	{
		CreateFireFlower(false, 28);
		CreateFireFlower(true, 28);
		CreateUnderLine(false);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 9)
	{
		CreateFireFlower(true, 32);
		CreateFireFlower(false, 32);
		CreateUnderLine(true);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 10)
	{
		CreateFireFlower(false, 24);
		CreateFireFlower(true, 24);
		CreateUnderLine(false);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 11)
	{
		CreateFireFlower(true, 22);
		CreateFireFlower(false, 22);
		CreateUnderLine(true);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 12)
	{
		CreateFireFlower(false, 28);
		CreateFireFlower(true, 28);
		CreateUnderLine(false);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 13)
	{
		CreateFireFlower(true, 24);
		CreateFireFlower(false, 24);
		CreateUnderLine(true);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 14)
	{
		CreateFireFlower(false, 28);
		CreateFireFlower(true, 28);
		CreateUnderLine(false);
	}
	if (m_frame == kThirdBurstFrame + kInterval * 15)
	{
		CreateFireFlower(true, 26);
		CreateFireFlower(false, 26);
		CreateUnderLine(true);
	}

	if (m_frame == 3280)
	{
		m_lineWidth = 100;
		m_eneLinePos = { 590, static_cast<float>(Game::kScreenHeight - m_lineWidth) - 180 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 100, 100, new EnemyLineDown);
	}

	if (m_frame == 3380)
	{
		m_eneCirclePos = { 640,720 };
		m_eneCircleBurstVec = { 0,-1 };
		m_eneCircleBurstSpeed = 28;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 12, 197);
	}
	if (m_frame == 3380 + 30)
	{
		m_eneCirclePos = { 640,720 };
		m_eneCircleBurstVec = { 0,-1 };
		m_eneCircleBurstSpeed = 28;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 12, 168);
	}
	if (m_frame == 3380 + 30 * 2)
	{
		m_eneCirclePos = { 640,720 };
		m_eneCircleBurstVec = { 0,-1 };
		m_eneCircleBurstSpeed = 28;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 12, 139);
	}
	if (m_frame == 3380 + 30 * 3)
	{
		m_eneCirclePos = { 640,720 };
		m_eneCircleBurstVec = { 0,-1 };
		m_eneCircleBurstSpeed = 28;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 12, 110);
	}

	if (m_frame == 3770)
	{
		m_eneCirclePos = { 320, 720 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 3770 + 20)
	{
		m_eneCirclePos = { 960, 720 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}

	if (m_frame == 3930)
	{
		m_eneCirclePos = { 320, 0 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 3930 + 20)
	{
		m_eneCirclePos = { 960, 0 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 3930 + 60)
	{
		m_eneCirclePos = { 640,0 };
		m_eneCircleBurstVec = { 0,1 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 8, 30);
	}

	if (m_frame == 4200)
	{
		m_eneCirclePos = { 320, 720 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 4200 + 20)
	{
		m_eneCirclePos = { 960, 720 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}

	if (m_frame == 4360)
	{
		m_eneCirclePos = { 320, 0 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 4360 + 20)
	{
		m_eneCirclePos = { 960, 0 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 10, 40);
	}
	if (m_frame == 4360 + 60)
	{
		m_eneCirclePos = { 640,0 };
		m_eneCircleBurstVec = { 0,1 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 8, 30);
	}

	if (m_frame == 4460)
	{
		m_goalFlag = true;
		m_isSoundFadeOut = true;
	}

	if (m_frame <= 4460)
	{
		m_progressPos += m_progressSpeed;
	}

	if (m_isSoundFadeOut)
	{
		m_manager.m_sManager.FadeOutBGM("stage1", m_soundFadeFrame);
	}

	StageBase::Update(input);

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
				m_manager.m_sManager.StopBGM("stage1");

				m_goal->Hit();
				m_manager.SetStageClearFlag(1);
				m_onGoal = true;
			}
		}
	}
}

void Stage1::Draw()
{
	StageBase::Draw();
}

void Stage1::CreateFireFlower(bool isRight, int speed)
{

	if (isRight)
	{
		m_eneCirclePos = { 1040,720 };
		m_eneCircleBurstVec = { 0,-1 };
		m_eneCircleBurstSpeed = speed;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 8, 28);
	}
	else
	{
		m_eneCirclePos = { 280,720 };
		m_eneCircleBurstVec = { 0,-1 };
		m_eneCircleBurstSpeed = speed;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 8, 28);
	}

}

void Stage1::CreateUnderLine(bool isRight)
{
	if (isRight)
	{
		m_lineWidth = 80;
		m_eneLinePos = { 1280, static_cast<float>(Game::kScreenHeight - m_lineWidth) };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 28, 60, new EnemyLineFromRight);
	}
	else
	{
		m_lineWidth = 80;
		m_eneLinePos = { 0, static_cast<float>(Game::kScreenHeight - m_lineWidth) };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 28, 60, new EnemyLineFromLeft);
	}
}
