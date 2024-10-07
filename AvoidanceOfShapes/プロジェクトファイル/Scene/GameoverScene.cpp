#include <DxLib.h>
#include "Input.h"
#include "Game.h"
#include "GameoverScene.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "DrawBGparts.h"

#include "Efects.h"
#include "DashEfect.h"
#include "AfterImagePlayer.h"

#include "StageTutorial.h"
#include "Stage1.h"
#include "Stage2.h"

enum HitNum
{
	zeroHit,
	oneHit,
	twoHit,
	threeHit
};

namespace
{
	//フェードイン、フェードアウトにかかる時間
	constexpr int kFadeFrame = 30;

	//UIのボタンのデフォルトのX座標
	constexpr int kButtonDefaultPosX = 780;
	//UIのボタンの移動後のX座標
	constexpr int kButtonMovedPosX = 860;
	//UIのボタンの移動速度
	constexpr int kButtonMoveSpeed = 24;
	//リトライボタンのY座標
	constexpr int kRetryButtonY = 400;
	//戻るボタンのY座標
	constexpr int kReturnButtonY = kRetryButtonY + 280;

	//テキストを描画する座標(UIボタン座標との差で計算)
	constexpr int kTextOffsetX = 148;
	constexpr int kTextOffsetY = 180;
	//テキストの色
	constexpr int kTextColor = 0xffffff;

	//プレイヤーの基本座標
	constexpr int kPlayerPosX = 296;
	constexpr int kPlayerPosY = 360;
	//プレイヤーのサイズの半分(グラフサイズより)
	constexpr int kBoxSizeHalf = 16;
	constexpr int kPlayerBaseColor = 0x004646;

	//プレイヤーが移動仕様とし始めるフレーム
	constexpr int kStartChargePlayerFrame = 160;
	//プレイヤーが動き出すフレーム数
	constexpr int kMovePlayerFrame = 200;
	//プレイヤーの移動速度
	constexpr int kMovePlayerSpeed = 32;

	//プレイヤーが回復するフレーム数
	constexpr int kFirstHealFrame = 30;
	constexpr int kSecondHealFrame = kFirstHealFrame * 2;
	constexpr int kThirdHealFrame = kFirstHealFrame * 3;
	constexpr int kFourthHealFrame = kFirstHealFrame * 4;

	//背景の四角の数
	constexpr int kBGnum = 12;
	//プレイヤーのダッシュエフェクトの数
	constexpr int kEfectNum = 32;
	//敵の移動速度
	constexpr int kEnemyMoveSpeed = 128;
	//敵の横幅の半分
	constexpr int kEnemySizeHalf = 64;
	//敵の色
	constexpr int kEnemyColor = 0xff00ff;

	//フェードのアルファ値の最大
	constexpr float kAlphaMax = 255.0f;
	//m_fadeFrameの初期化
	constexpr int kInitFadeFrame = 60;

}

GameoverScene::GameoverScene(SceneManager& mgr) :
	Scene(mgr), 
	m_frame(0),
	m_retryButtonX(kButtonDefaultPosX),
	m_returnButtonX(kButtonMovedPosX),
	m_playerPos(kPlayerPosX, kPlayerPosY),
	m_isRevival(false),
	m_deathAnimationFlag(false)
{

	m_parts.resize(kBGnum);

	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i] = new DrawBGparts;
		m_parts[i]->Init(false);
	}

	m_afterImage.resize(kEfectNum);


	m_handle[zeroHit] = LoadGraph("data/player/player0.png");
	m_handle[oneHit] = LoadGraph("data/player/player1.png");
	m_handle[twoHit] = LoadGraph("data/player/player2.png");
	m_handle[threeHit] = LoadGraph("data/player/player3.png");

	updateFunc_ = &GameoverScene::FadeInUpdate;
	drawFunc_ = &GameoverScene::FadeOutDraw;

	m_fadeFrame = kInitFadeFrame;
}

GameoverScene::~GameoverScene()
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

void GameoverScene::Update(Input& input)
{
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i]->Update();
	}

	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			m_afterImage[i]->Update();

			//使用済みの敵キャラクタを削除する必要がある
			if (!m_afterImage[i]->isExist())
			{
				//メモリを解放する
				delete m_afterImage[i];
				m_afterImage[i] = nullptr;	//使っていないとわかるように
			}
		}
	}

	if (m_isRevival)
	{
		m_frame++;

		//200フレーム(ダッシュする前)までは位置を補正
		if (m_frame <= kMovePlayerFrame)
		{
			m_playerPos = { kPlayerPosX,kPlayerPosY };
		}

		//回復中は震えるように
		if (m_frame >= kFirstHealFrame)
		{
			if (m_frame < kFourthHealFrame)
			{
				m_offset = { static_cast<float>(GetRand(4) - 2),static_cast<float>(GetRand(4) - 2) };
				m_playerPos += m_offset;
			}
		}

		//ダッシュ前の溜め中も震えるように
		if (m_frame >= kStartChargePlayerFrame)
		{
			if (m_frame < kMovePlayerFrame)
			{
				m_offset = { static_cast<float>(GetRand(4) - 2),static_cast<float>(GetRand(4) - 2) };
				m_playerPos += m_offset;
			}
		}

		//ダッシュし始める
		if (m_frame >= kMovePlayerFrame)
		{
			m_playerPos.x += kMovePlayerSpeed;
		}

		//回復を始めたときにSEがなるように
		if (m_frame == kFirstHealFrame)
		{
			m_manager.m_sManager.PlaySE("RevivalSE");
		}
	}

	if (m_frame >= kMovePlayerFrame)
	{
		updateFunc_ = &GameoverScene::FadeOutUpdate;
		drawFunc_ = &GameoverScene::FadeOutDraw;
	}

	if (m_frame == kMovePlayerFrame)
	{
		for (int i = 0; i < m_afterImage.size(); i++)
		{
			CreateAfterImage(m_playerPos);
		}
	}

	(this->*updateFunc_)(input);
}

void GameoverScene::Draw()
{
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i]->Draw();
	}

	//残像の描画
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			m_afterImage[i]->Draw();
		}
	}

	m_manager.Draw(m_retryButtonX, kRetryButtonY, 1.0f, 0.0f, "Button1");
	DrawStringToHandle(m_retryButtonX + kTextOffsetX, kRetryButtonY - kTextOffsetY, "リトライ", kTextColor, m_manager.ReturnFontHandle());
	m_manager.Draw(m_returnButtonX, kReturnButtonY, 1.0f, 0.0f, "Button3");
	DrawStringToHandle(m_returnButtonX + kTextOffsetX, kReturnButtonY - kTextOffsetY, "タイトルへ", kTextColor, m_manager.ReturnFontHandle());

	DrawBox(static_cast<int>(m_playerPos.x) - kBoxSizeHalf, static_cast<int>(m_playerPos.y) - kBoxSizeHalf,
		static_cast<int>(m_playerPos.x) + kBoxSizeHalf, static_cast<int>(m_playerPos.y) + kBoxSizeHalf, kPlayerBaseColor, true);

	if (m_isRevival)
	{
		if (m_frame >= kFourthHealFrame)
		{
			DrawRotaGraph(static_cast<int>(m_playerPos.x), static_cast<int>(m_playerPos.y), 1.0f, 0.0f, m_handle[zeroHit], true);
		}
		else if (m_frame >= kThirdHealFrame)
		{
			DrawRotaGraph(static_cast<int>(m_playerPos.x), static_cast<int>(m_playerPos.y), 1.0f, 0.0f, m_handle[oneHit], true);
		}
		else if (m_frame >= kSecondHealFrame)
		{
			DrawRotaGraph(static_cast<int>(m_playerPos.x), static_cast<int>(m_playerPos.y), 1.0f, 0.0f, m_handle[twoHit], true);
		}
		else if (m_frame >= kFirstHealFrame)
		{
			DrawRotaGraph(static_cast<int>(m_playerPos.x), static_cast<int>(m_playerPos.y), 1.0f, 0.0f, m_handle[threeHit], true);
		}

		m_manager.m_sManager.FadeOutBGM("GameOver", m_frame);
	}

	if (m_deathAnimationFlag == true)
	{
		m_frame++;
		enemyY = m_frame * kEnemyMoveSpeed;

		DrawBox(static_cast<int>(m_playerPos.x) - kEnemySizeHalf, 0, static_cast<int>(m_playerPos.x) + kEnemySizeHalf, enemyY, kEnemyColor, true);

		if (enemyY / 2 >= Game::kScreenHeight)
		{
			updateFunc_ = &GameoverScene::FadeOutUpdate;
			drawFunc_ = &GameoverScene::FadeOutDraw;
		}
	}

	(this->*drawFunc_)();
}

void GameoverScene::CreateAfterImage(Vec2 pos )
{
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (!m_afterImage[i])
		{
			m_afterImage[i] = new AfterImagePlayer;
			m_afterImage[i]->Init(pos, { 1,0 }, GetRand(4) + 4, true);
			return;
		}
	}
}

void GameoverScene::CreateDashEfect(Vec2 pos, bool isLastHit)
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

void GameoverScene::FadeInUpdate(Input& input)
{
	m_fadeFrame--;
	if (m_fadeFrame <= 0)
	{
		updateFunc_ = &GameoverScene::NormalUpdate;
		drawFunc_ = &GameoverScene::NormalDraw;
	}
}

void GameoverScene::NormalUpdate(Input& input)
{
	if (m_isRevival)
		return;

	m_manager.m_sManager.PlayBGM("GameOver", true);

	if (m_nowButton == Retry)
	{
		if (m_retryButtonX > kButtonDefaultPosX)
		{
			m_retryButtonX -= kButtonMoveSpeed;
		}
	}
	else
	{
		//右に動かす
		if (m_retryButtonX < kButtonMovedPosX)
		{
			m_retryButtonX += kButtonMoveSpeed;
		}
	}

	if (m_nowButton == Return)
	{
		if (m_returnButtonX > kButtonDefaultPosX)
		{
			m_returnButtonX -= kButtonMoveSpeed;
		}
	}
	else
	{
		//右に動かす
		if (m_returnButtonX < kButtonMovedPosX)
		{
			m_returnButtonX += kButtonMoveSpeed;
		}
	}

	if (input.IsTriggered("UP"))
	{
		if (m_nowButton == Return)
		{
			m_nowButton = Retry;
		}
	}

	if (input.IsTriggered("DOWN"))
	{
		if (m_nowButton == Retry)
		{
			m_nowButton = Return;
		}
	}

	if (input.IsTriggered("OK") && m_nowButton == Retry)
	{
		m_isRevival = true;
		m_manager.m_sManager.FadeOutBGM("GameOver", m_fadeFrame);
	}

	if (input.IsTriggered("OK") && m_nowButton == Return)
	{
		m_deathAnimationFlag = true;
		m_manager.m_sManager.PlaySE("deathSE");

	}
}

void GameoverScene::FadeOutUpdate(Input& input)
{
	m_fadeFrame++;
	m_manager.m_sManager.FadeOutBGM("GameOver", m_fadeFrame);
	if (m_fadeFrame >= kFadeFrame)
	{
		if (m_nowButton == Retry)
		{
			if (m_manager.m_logScene == m_manager.m_stage1)
			{
				m_manager.ChangeScene(std::make_shared<Stage1>(m_manager));
			}
			else if (m_manager.m_logScene == m_manager.m_stage2)
			{
				m_manager.ChangeScene(std::make_shared<Stage2>(m_manager));
			}
			else if (m_manager.m_logScene == m_manager.m_StageTutorial)
			{
				m_manager.ChangeScene(std::make_shared<StageTutorial>(m_manager));
			}
		}
		else if (m_nowButton == Return)
		{
			m_manager.ChangeScene(std::make_shared<TitleScene>(m_manager));
		}
	}
}

void GameoverScene::FadeOutDraw()
{
#ifdef _DEBUG
	DrawString(0, 0, "GameoverScene", 0xffffff);
#endif

	float rate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);
	float alpha = kAlphaMax * rate;
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(alpha));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameoverScene::NormalDraw()
{
#ifdef _DEBUG
	DrawString(0, 0, "GameoverScene", 0xffffff);
#endif
}
