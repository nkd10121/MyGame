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

//ステージを解放していく形式にするかどうか
//false:最初から全ステージ解放、true:ステージをクリアして解放していく
#define isSTAGECLEARFORMAT true

namespace
{
	//フェードイン、フェードアウトにかかる時間
	constexpr int kFadeFrame = 30;

	//背景の四角の最大数
	constexpr int kBGnum = 10;

	//プレイヤーが動くスピード
	constexpr int kMoveSpeed = 20;
	//プレイヤーの基本座標
	constexpr float kPlayerPosX = 640.0f;
	constexpr float kPlayerPosY = 600.0f;
	//プレイヤーが着地するまでのフレーム
	constexpr int kPlayerLandingFrame = 66;
	//プレイヤーのジャンプ力
	constexpr int kPlayerJumpPower = 180;

	//トランジション用のboxのサイズ
	constexpr int kWidth = 80;
	constexpr int kHeight = 80;
	//トランジション用のBoxの数
	constexpr int kWidthNum = 16;
	constexpr int kHeightNum = 9;

	//ステージ詳細の基本値
	constexpr int kStageAlphaBase = 200;
	//ステージ詳細の明暗の遷移速度
	constexpr float kStageAlphaSpeed = 3.0f;

	constexpr int kDetailBasePosX = 640;
	constexpr int kDetailBasePosY = 360;

}


StageSelect::StageSelect(SceneManager& mgr) :
	Scene(mgr)
{
	//背景の四角の初期化
	m_parts.resize(kBGnum);
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i] = new DrawBGparts;
		m_parts[i]->Init(true);
	}

	//トランジションの準備
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

	//ゲーム画面描画先の生成
	//画面サイズと同じ大きさのグラフィックデータを生成する
	m_gameScreenHandle = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	//プレイヤーの基本座標を代入
	m_playerPos.x = kPlayerPosX;
	m_playerPos.y = kPlayerPosY;

	//最初はチュートリアルを選択している状態にする
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
	//ステージをクリアしてステージセレクトに戻ってきたとき、クリアしたステージを選択している状態にする処理
	if (m_isFirst)
	{
		//TODO:if文の中身をクリアフラグにする
		//ステージ1のアドレスと直前のシーンのアドレスが同じ場合
		if (m_manager.m_stage1 == m_manager.m_logScene)
		{
			//ステージ1を選択している状態にする
			m_next = nStage1;
			m_detailX = -kDetailBasePosX * m_next;
		}
		//ステージ2のアドレスと直前のシーンのアドレスが同じ場合
		if (m_manager.m_stage2 == m_manager.m_logScene)
		{
			//ステージ2を選択している状態にする
			m_next = nStage2;
			m_detailX = -kDetailBasePosX * m_next;
		}

		m_isFirst = false;
	}

	m_frame++;

	//背景の更新処理
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

	//動いているときのBGMの処理
	if (m_isMoving)
	{
		//BGMをフェードアウトさせる
		m_soundFadeFrame++;
		m_manager.m_sManager.FadeOutBGM("Tutorial", m_soundFadeFrame);
		m_manager.m_sManager.FadeOutBGM("stage1", m_soundFadeFrame);
		m_manager.m_sManager.FadeOutBGM("stage2", m_soundFadeFrame);
	}
	else
	{
		//動いていないときフェードアウト用変数を0にする
		m_soundFadeFrame = 0;
	}

	//動いているときのプレイヤーの処理
	if (m_isMoving)
	{
		
		m_angle -= DX_TWO_PI_F / kPlayerLandingFrame;

		//プレイヤーのy座標が基本座標のy座標を超えていたら基本座標に補正する
		if (m_playerPos.y > kPlayerPosY)
		{
			m_playerPos.y = kPlayerPosY;
			m_angle = 0;
		}

		//y座標の移動のための処理
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
	/*選択しているステージを明るく、
	選択していないステージを暗く描画するための処理*/
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
	/*選択しているステージを明るく、
	選択していないステージを暗く描画するための処理*/
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
	//描画先スクリーンをクリアする
	ClearDrawScreen();
	///////////////////////////////

	//背景の描画
	DrawBox(0, 0, 1280, 720, 0x0d1118, true);
	//背景のBoxの描画
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
	//バックバッファに書き込む設定に戻しておく
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
	//動いていないとき
	if (!m_isMoving)
	{
		m_nextLog = m_next;

		//選択しているステージのBGMを流す
		//PlayBGMの中で流れていたら何もしないようにしている
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

		//右を押したとき選択しているステージをずらす
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

		//左を押したとき選択しているステージをずらす
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
		//OKを押したときにフェードアウトする
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
		//OKを押したときにフェードアウトする
		if (input.IsTriggered("OK"))
		{
			updateFunc_ = &StageSelect::FadeOutUpdate;
			drawFunc_ = &StageSelect::FadeOutDraw;
		}
#endif

		//ポーズを押したとき
		if (input.IsTriggered("pause"))
		{
			//決定SEを流して
			m_manager.m_sManager.PlaySE("enterSE");

			//選択しているステージのBGMを止めて
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

			//ポーズ画面を表示する
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
		//選択しているステージに遷移する
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

	//フェードアウト用のカウンタを進めて
	m_fadeFrame++;

	//選択しているステージのBGMをフェードアウトさせる
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