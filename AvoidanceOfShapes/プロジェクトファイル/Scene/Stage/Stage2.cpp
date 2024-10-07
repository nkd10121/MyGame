#include "Stage2.h"
#include "SceneManager.h"
#include "EnemyLineDown.h"
#include "EnemyLineFromUpper.h"
#include "EnemyLineFromRight.h"
#include "EnemyLineFromLeft.h"

#include "Goal.h"

Stage2::Stage2(SceneManager& mgr) :
	StageBase(mgr)
{
	m_progressSpeed = 0.0481154f;
}

Stage2::~Stage2()
{

}

void Stage2::Update(Input& input)
{
	//最初にstage2のアドレスを取っておく
	//ゲームオーバーからstage2に戻ってこれるように
	if (m_isFirst)
	{
		m_manager.GetPtr(2);
		m_isFirst = false;
	}

	//一定の間をあけて音楽を再生する
	if (m_frame == 10)
	{
		m_manager.m_sManager.PlayBGM("stage2", true);
	}

	StageBase::Update(input);

	//敵の生成をする
	//参考
	//width = 1280,height = 720
	//中心 640,360

	if (m_frame == 360)
	{
		m_eneCirclePos = { 1000,0 };
		m_eneCircleBurstVec = { -1,4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 16, 32);
	}
	else if (m_frame == 410 - 2)
	{
		m_eneCirclePos = { 1000,720 };
		m_eneCircleBurstVec = { -1,-8 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 16, 32);


	}
	else if (m_frame == 460 - 4)
	{
		m_eneCirclePos = { 200,0 };
		m_eneCircleBurstVec = { 1,6 };
		m_eneCircleBurstSpeed = 30;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 16, 32);

	}
	else if (m_frame == 510 - 6)
	{
		m_eneCirclePos = { 450,720 };
		m_eneCircleBurstVec = { 8,-2 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 16, 32);

	}
	else if (m_frame == 560 - 8)
	{
		m_eneCirclePos = { 700,0 };
		m_eneCircleBurstVec = { 2,2 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 16, 32);

	}
	else if (m_frame == 610 - 10)
	{
		m_eneCirclePos = { 1280,360 };
		m_eneCircleBurstVec = { -4,0 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 16, 32);

	}

	//750
	if (m_frame == 660)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 560, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 70, 90, new EnemyLineFromUpper);
	}
	else if (m_frame == 660 + 2)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 680, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 68, 88, new EnemyLineDown);
	}
	else if (m_frame == 660 + 18)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 200, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 52, 72, new EnemyLineDown);
	}
	else if (m_frame == 660 + 20)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1040, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 50, 70, new EnemyLineFromUpper);
	}
	else if (m_frame == 660 + 36)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 280 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 34, 54, new EnemyLineFromLeft);
	}
	else if (m_frame == 660 + 38)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1280, 400 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 32, 52, new EnemyLineFromRight);
	}
	else if (m_frame == 660 + 54)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1280, 120 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 16, 36, new EnemyLineFromRight);
	}
	else if (m_frame == 660 + 56)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 560 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 14, 34, new EnemyLineFromLeft);
	}



	if (m_frame == 750)
	{
		m_eneCirclePos = { 280,0 };
		m_eneCircleBurstVec = { 2,1 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 800 - 2)
	{
		m_eneCirclePos = { 760,0 };
		m_eneCircleBurstVec = { -1,2 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 850 - 4)
	{
		m_eneCirclePos = { 1280,100 };
		m_eneCircleBurstVec = { -4,6 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 900 - 6)
	{
		m_eneCirclePos = { 1280,600 };
		m_eneCircleBurstVec = { -1,-2 };
		m_eneCircleBurstSpeed = 12;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 950 - 8)
	{
		m_eneCirclePos = { 600,720 };
		m_eneCircleBurstVec = { -1,-1 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1000 - 10)
	{
		m_eneCirclePos = { 80,720 };
		m_eneCircleBurstVec = { 4,-4 };
		m_eneCircleBurstSpeed = 24;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1050 - 12)
	{
		m_eneCirclePos = { 1000,0 };
		m_eneCircleBurstVec = { -2,1 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}


	if (m_frame == 1000)
	{
		m_eneCirclePos = { 320, 360 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 100, 120);
	}
	else if (m_frame == 1010)
	{
		m_eneCirclePos = { 960, 360 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 100, 120);
	}
	else if (m_frame == 1020)
	{
		m_eneCirclePos = { 640, 360 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 100, 120);
	}


	if (m_frame == 1130)
	{
		m_eneCirclePos = { 100,0 };
		m_eneCircleBurstVec = { 2,4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1178)
	{
		m_eneCirclePos = { 1020,0 };
		m_eneCircleBurstVec = { -4,6 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1226)
	{
		m_eneCirclePos = { 1280,400 };
		m_eneCircleBurstVec = { -2,-4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1274)
	{
		m_eneCirclePos = { 560,720 };
		m_eneCircleBurstVec = { -2,-8 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1322)
	{
		m_eneCirclePos = { 160,720 };
		m_eneCircleBurstVec = { 2,-4 };
		m_eneCircleBurstSpeed = 24;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1370)
	{
		m_eneCirclePos = { 0,180 };
		m_eneCircleBurstVec = { 4,1 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}

	if (m_frame == 1414)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 70, 90, new EnemyLineFromUpper);
	}
	else if (m_frame == 1416)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1240, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 68, 88, new EnemyLineDown);
	}
	else if (m_frame == 1432)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 600, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 52, 72, new EnemyLineDown);
	}
	else if (m_frame == 1434)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 640, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 50, 70, new EnemyLineFromUpper);
	}
	else if (m_frame == 1450)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 34, 54, new EnemyLineFromLeft);
	}
	else if (m_frame == 1452)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1280, 680 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 32, 52, new EnemyLineFromRight);
	}
	else if (m_frame == 1468)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1280, 320 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 16, 36, new EnemyLineFromRight);
	}
	else if (m_frame == 1470)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 360 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 14, 34, new EnemyLineFromLeft);
	}


	if (m_frame == 1510)//1
	{
		m_eneCirclePos = { 0,0 };
		m_eneCircleBurstVec = { 1,1 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1560 - 4)//2
	{
		m_eneCirclePos = { 560,0 };
		m_eneCircleBurstVec = { -2,4 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1610 - 8)//3
	{
		m_eneCirclePos = { 1280,660 };
		m_eneCircleBurstVec = { -2,-1 };
		m_eneCircleBurstSpeed = 24;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1660 - 12)//4
	{
		m_eneCirclePos = { 240,0 };
		m_eneCircleBurstVec = { 2,4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1710 - 16)//5
	{
		m_eneCirclePos = { 0,120 };
		m_eneCircleBurstVec = { 2,2 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1760 - 20)//6
	{
		m_eneCirclePos = { 1060,0 };
		m_eneCircleBurstVec = { -4,2 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1810 - 24)//7
	{
		m_eneCirclePos = { 340,0 };
		m_eneCircleBurstVec = { -1,2 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1860 - 28)//8
	{
		m_eneCirclePos = { 1280,60 };
		m_eneCircleBurstVec = { -2,4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1910 - 32)//9
	{
		m_eneCirclePos = { 360,0 };
		m_eneCircleBurstVec = { -2,6 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 1960 - 36)//10
	{
		m_eneCirclePos = { 1280,400 };
		m_eneCircleBurstVec = { -2,4 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2010 - 40)//11
	{
		m_eneCirclePos = { 0,600 };
		m_eneCircleBurstVec = { 4,-3 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2060 - 44)//12
	{
		m_eneCirclePos = { 1100,0 };
		m_eneCircleBurstVec = { -4,1 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2110 - 48)//13
	{
		m_eneCirclePos = { 280,720 };
		m_eneCircleBurstVec = { 2,-4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2160 - 52)//14
	{
		m_eneCirclePos = { 280,0 };
		m_eneCircleBurstVec = { -2,4 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}


	//2216
	if (m_frame == 2160)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 180 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 70, 90, new EnemyLineFromLeft);
	}
	else if (m_frame == 2160 + 2)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 920, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 68, 88, new EnemyLineFromUpper);
	}
	else if (m_frame == 2160 + 18)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 500 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 52, 72, new EnemyLineFromRight);
	}
	else if (m_frame == 2160 + 20)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 320, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 50, 70, new EnemyLineDown);
	}
	else if (m_frame == 2160 + 36)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 34, 54, new EnemyLineFromLeft);
	}
	else if (m_frame == 2160 + 38)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1240, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 32, 52, new EnemyLineFromUpper);
	}
	else if (m_frame == 2160 + 54)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 680 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 16, 36, new EnemyLineFromRight);
	}
	else if (m_frame == 2160 + 56)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 0, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 14, 34, new EnemyLineDown);
	}


	if (m_frame == 1530)
	{
		m_eneCirclePos = { 200, 520 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 90, 110);
	}
	else if (m_frame == 1900)
	{
		m_eneCirclePos = { 1080, 200 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 106, 126);
	}



	//2360 ~ 2948 Burst
	if (m_frame == 2360)//1
	{
		m_eneCirclePos = { 1280,200 };
		m_eneCircleBurstVec = { -2,2 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);

		//CreateEnemyBox(Vec2{ 280,160 }, 140, 24, 24, 48);

	}
	else if (m_frame == 2360 + 46)//2
	{
		m_eneCirclePos = { 240,720 };
		m_eneCircleBurstVec = { 2,-4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);

		//CreateEnemyBox(Vec2{ 280,720 - 160 }, 140, 24, 24, 48);

	}
	else if (m_frame == 2360 + 46 * 2)//3
	{
		m_eneCirclePos = { 100,0 };
		m_eneCircleBurstVec = { 6,4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 3)//4
	{
		m_eneCirclePos = { 1280,720 };
		m_eneCircleBurstVec = { -2,-4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 4)//5
	{
		m_eneCirclePos = { 820,0 };
		m_eneCircleBurstVec = { -4,2 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 5)//6
	{
		m_eneCirclePos = { 0,600 };
		m_eneCircleBurstVec = { 4,-2 };
		m_eneCircleBurstSpeed = 26;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 6)//7
	{
		m_eneCirclePos = { 80,0 };
		m_eneCircleBurstVec = { 2,4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 7)//8
	{
		m_eneCirclePos = { 1280,0 };
		m_eneCircleBurstVec = { -6,2 };
		m_eneCircleBurstSpeed = 26;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 8)//9
	{
		m_eneCirclePos = { 400,720 };
		m_eneCircleBurstVec = { 2,-2 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 9)//10
	{
		m_eneCirclePos = { 0,300 };
		m_eneCircleBurstVec = { 8,-4 };
		m_eneCircleBurstSpeed = 24;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 10)//11
	{
		m_eneCirclePos = { 1000,720 };
		m_eneCircleBurstVec = { -8,-4 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 11)//12
	{
		m_eneCirclePos = { 400,0 };
		m_eneCircleBurstVec = { 4,4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 12)//13
	{
		m_eneCirclePos = { 800,720 };
		m_eneCircleBurstVec = { 1,-4 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 2360 + 46 * 13)//14
	{
		m_eneCirclePos = { 580,0 };
		m_eneCircleBurstVec = { -1,2 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}


	//2300
	if (m_frame == 2360)
	{
		m_eneCirclePos = { 640, 180 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 84, 104);

		m_eneCirclePos = { 640, 540 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 92, 112);
	}


	//2720 ~ 2728
	if (m_frame == 2720)
	{
		m_eneCirclePos = { 280, 360 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 96, 116);
	}
	else if (m_frame == 2720 + 4)
	{
		m_eneCirclePos = { 1000, 360 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 92, 112);
	}
	else if (m_frame == 2720 + 8)
	{
		m_eneCirclePos = { 640, 360 };
		m_eneCircleRadius = 160;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 96, 116);
	}

	//3010 Line
	if (m_frame == 3010)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1180, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 70, 90, new EnemyLineFromUpper);
	}
	else if (m_frame == 3010 + 2)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1020, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 68, 88, new EnemyLineFromUpper);
	}
	else if (m_frame == 3010 + 18)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 860, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 52, 72, new EnemyLineFromUpper);
	}
	else if (m_frame == 3010 + 20)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 700, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 50, 70, new EnemyLineFromUpper);
	}
	else if (m_frame == 3010 + 36)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 540, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 34, 54, new EnemyLineFromUpper);
	}
	else if (m_frame == 3010 + 38)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 380, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 32, 52, new EnemyLineFromUpper);
	}
	else if (m_frame == 3010 + 54)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 220, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 16, 36, new EnemyLineFromUpper);
	}
	else if (m_frame == 3010 + 56)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 60, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 14, 34, new EnemyLineFromUpper);
	}


	//3110 ~  Burst
	if (m_frame == 3110)//1
	{
		m_eneCirclePos = { 200,0 };
		m_eneCircleBurstVec = { 4,4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);

		//CreateEnemyBox(Vec2{ 280,160 }, 120, 24, 28, 40);
	}
	else if (m_frame == 3110 + 46)//2
	{
		m_eneCirclePos = { 40,720 };
		m_eneCircleBurstVec = { 2,-4 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);

	}
	else if (m_frame == 3110 + 46 * 2)//3
	{
		m_eneCirclePos = { 1000,720 };
		m_eneCircleBurstVec = { -2,-4 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 3)//4
	{
		m_eneCirclePos = { 1000,0 };
		m_eneCircleBurstVec = { -4,4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 4)//5
	{
		m_eneCirclePos = { 600,720 };
		m_eneCircleBurstVec = { 2,-4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 5)//6
	{
		m_eneCirclePos = { 1280,400 };
		m_eneCircleBurstVec = { -6,-1 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 6)//7
	{
		m_eneCirclePos = { 0,0 };
		m_eneCircleBurstVec = { 4,4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 7)//8
	{
		m_eneCirclePos = { 280,720 };
		m_eneCircleBurstVec = { 2,-4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 8)//9
	{
		m_eneCirclePos = { 1280,600 };
		m_eneCircleBurstVec = { -2,-4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 9)//10
	{
		m_eneCirclePos = { 700,0 };
		m_eneCircleBurstVec = { -2,4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 10)//11
	{
		m_eneCirclePos = { 1140,720 };
		m_eneCircleBurstVec = { -2,-4 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 11)//12
	{
		m_eneCirclePos = { 280,0 };
		m_eneCircleBurstVec = { 2,1 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 12)//13
	{
		m_eneCirclePos = { 200,720 };
		m_eneCircleBurstVec = { 6,-4 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3110 + 46 * 13)//14
	{
		m_eneCirclePos = { 700,0 };
		m_eneCircleBurstVec = { -1,4 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}

	//2300
	if (m_frame == 3110)
	{
		m_eneCirclePos = { 640, 180 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 92, 112);

		m_eneCirclePos = { 640, 540 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 84, 104);
	}


	//3470
	if (m_frame == 3470)
	{
		m_eneCirclePos = { 280, 360 };
		m_eneCircleRadius = 160;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 96, 116);
	}
	else if (m_frame == 3470 + 4)
	{
		m_eneCirclePos = { 1000, 360 };
		m_eneCircleRadius = 160;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 92, 112);
	}
	else if (m_frame == 3470 + 8)
	{
		m_eneCirclePos = { 640, 360 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 96, 116);
	}

	//3760 Line
	if (m_frame == 3760)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 60, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 70, 90, new EnemyLineDown);
	}
	else if (m_frame == 3760 + 2)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 220, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 68, 88, new EnemyLineDown);
	}
	else if (m_frame == 3760 + 18)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 380, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 52, 72, new EnemyLineDown);
	}
	else if (m_frame == 3760 + 20)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 540, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 50, 70, new EnemyLineDown);
	}
	else if (m_frame == 3760 + 36)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 700, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 34, 54, new EnemyLineDown);
	}
	else if (m_frame == 3760 + 38)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 860, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 32, 52, new EnemyLineDown);
	}
	else if (m_frame == 3760 + 54)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1020, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 16, 36, new EnemyLineDown);
	}
	else if (m_frame == 3760 + 56)
	{
		m_lineWidth = 40;
		m_eneLinePos = { 1180, 0 };
		CreateEnemyLineFrom(m_eneLinePos, m_lineWidth, 14, 34, new EnemyLineDown);
	}




	if (m_frame == 3860)//1
	{
		m_eneCirclePos = { 980,0 };
		m_eneCircleBurstVec = { -2,4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46)//2
	{
		m_eneCirclePos = { 0,80 };
		m_eneCircleBurstVec = { 6,1 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 2)//3
	{
		m_eneCirclePos = { 1280,400 };
		m_eneCircleBurstVec = { -2,1 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 3)//4
	{
		m_eneCirclePos = { 400,720 };
		m_eneCircleBurstVec = { 2,-4 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 4)//5
	{
		m_eneCirclePos = { 300,0 };
		m_eneCircleBurstVec = { 6,4 };
		m_eneCircleBurstSpeed = 20;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 5)//6
	{
		m_eneCirclePos = { 280,720 };
		m_eneCircleBurstVec = { 2,-4 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 6)//7
	{
		m_eneCirclePos = { 600,0 };
		m_eneCircleBurstVec = { 2,4 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 7)//8
	{
		m_eneCirclePos = { 1280,700 };
		m_eneCircleBurstVec = { -2,-2 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 8)//9
	{
		m_eneCirclePos = { 200,720 };
		m_eneCircleBurstVec = { 1,-8 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 9)//10
	{
		m_eneCirclePos = { 0,160 };
		m_eneCircleBurstVec = { 10,2 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 10)//11
	{
		m_eneCirclePos = { 1000,0 };
		m_eneCircleBurstVec = { -2,4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 11)//12
	{
		m_eneCirclePos = { 1280,360 };
		m_eneCircleBurstVec = { -2,-2 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 12)//13
	{
		m_eneCirclePos = { 640,720 };
		m_eneCircleBurstVec = { 0,-4 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}
	else if (m_frame == 3860 + 46 * 13)//14
	{
		m_eneCirclePos = { 280,0 };
		m_eneCircleBurstVec = { -2,4 };
		m_eneCircleBurstSpeed = 14;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 28);
	}

	//3860
	if (m_frame == 3860)
	{
		m_eneCirclePos = { 320, 180 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 92, 112);

		m_eneCirclePos = { 960, 180 };
		m_eneCircleRadius = 120;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 84, 104);
	}


	//4220
	if (m_frame == 4220)
	{
		m_eneCirclePos = { 280, 540 };
		m_eneCircleRadius = 160;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 96, 116);
	}
	else if (m_frame == 4220 + 4)
	{
		m_eneCirclePos = { 1000, 540 };
		m_eneCircleRadius = 160;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 92, 112);
	}
	else if (m_frame == 4220 + 8)
	{
		m_eneCirclePos = { 640, 540 };
		m_eneCircleRadius = 140;
		CreateEnemyCircleScaling(m_eneCirclePos, m_eneCircleRadius, 96, 116);
	}

	//4500 ~ 4590 Last
	if (m_frame == 4500)
	{
		m_eneCirclePos = { 640,0 };
		m_eneCircleBurstVec = { 0,1 };
		m_eneCircleBurstSpeed = 12;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 90);
	}
	else if (m_frame == 4500 + 2)
	{
		m_eneCirclePos = { 1280,0 };
		m_eneCircleBurstVec = { -2,1 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 88);
	}
	else if (m_frame == 4500 + 18)
	{
		m_eneCirclePos = { 1280,360 };
		m_eneCircleBurstVec = { -1,0 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 72);
	}
	else if (m_frame == 4500 + 20)
	{
		m_eneCirclePos = { 1280,720 };
		m_eneCircleBurstVec = { -2,-1 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 70);
	}
	else if (m_frame == 4500 + 36)
	{
		m_eneCirclePos = { 640,720 };
		m_eneCircleBurstVec = { 0,-1 };
		m_eneCircleBurstSpeed = 12;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 54);
	}
	else if (m_frame == 4500 + 38)
	{
		m_eneCirclePos = { 0,720 };
		m_eneCircleBurstVec = { 2,-1 };
		m_eneCircleBurstSpeed = 18;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 52);
	}
	else if (m_frame == 4500 + 54)
	{
		m_eneCirclePos = { 0,360 };
		m_eneCircleBurstVec = { 1,0 };
		m_eneCircleBurstSpeed = 16;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 36);
	}
	else if (m_frame == 4500 + 56)
	{
		m_eneCirclePos = { 0,0 };
		m_eneCircleBurstVec = { 2,1 };
		m_eneCircleBurstSpeed = 22;
		CreateEnemyBurst(m_eneCirclePos, m_eneCircleBurstVec, m_eneCircleBurstSpeed, 14, 34);
	}


	//if (frame == 4988)
	//{
	//	m_manager.sManager.StopBGM("stage2");
	//}
	else if (m_frame == 5050)
	{
		m_goalFlag = true;
	}

	if (m_goalFlag)
	{
		m_goal->Update();

		Col goalCol = m_goal->GetColRect();

		//プレイヤーとゴールが当たった時の処理
		if (m_playerColGoal.IsCollision(goalCol))
		{
			m_goal->Hit();
			m_manager.SetStageClearFlag(1);
			m_onGoal = true;
		}

	}

	if (m_frame <= 4988)
	{
		m_progressPos += m_progressSpeed;
	}
}

void Stage2::Draw()
{
	StageBase::Draw();
}