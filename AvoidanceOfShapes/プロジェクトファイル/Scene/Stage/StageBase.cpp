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
	//フェードイン、フェードアウトにかかる時間
	constexpr int kFadeFrame = 30;

	//画面を区切るBoxの高さ
	constexpr int kWidth = 80;
	//画面を区切るBoxの幅
	constexpr int kHeight = 80;

	//画面幅をkWidthで割った数
	constexpr int kWidthNum = Game::kScreenWidth / kWidth;
	//画面高さをkHeightで割った数
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

	//ゲーム画面描画先の生成
	//画面サイズと同じ大きさのグラフィックデータを生成する
	m_gameScreenHandle = MakeScreen(Game::kScreenWidth, Game::kScreenHeight, true);

	//フェードイン時のアニメーションに使う
	//四角のX座標を設定
	m_posX.resize(kWidthNum);
	for (int i = 0; i < m_posX.size(); i++)
	{
		m_posX[i] = 40 + 80 * i;
	}
	//フェードイン時のアニメーションに使う
	//四角のY座標を設定
	m_posY.resize(kHeightNum);
	for (int i = 0; i < m_posY.size(); i++)
	{
		m_posY[i] = 40 + 80 * i;
	}

	//プレイヤーの初期化
	m_player = new Player;
	m_player->Init(true);

	//プレイヤーの画像をm_playerに渡す
	int playerHandle[4];
	playerHandle[0] = m_manager.m_gManager.GetHandle("Player");
	playerHandle[1] = m_manager.m_gManager.GetHandle("Player1");
	playerHandle[2] = m_manager.m_gManager.GetHandle("Player2");
	playerHandle[3] = m_manager.m_gManager.GetHandle("Player3");

	//プレイヤーにハンドルを渡す
	m_player->SetPlayerHandle(playerHandle[0], playerHandle[1], playerHandle[2], playerHandle[3]);

	//ゴールの初期化
	m_goal = new Goal;
	m_goal->Init();
	m_goal->SetGraphHandle(m_manager.m_gManager.GetHandle("Goal"));

	//ゴールエフェクト
	m_goalEfect = new GoalEfect;

	updateFunc_ = &StageBase::FadeInUpdate;
	drawFunc_ = &StageBase::FadeInDraw;
	m_fadeFrame = 0;
}

StageBase::~StageBase()
{
	//プレイヤーのメモリ削除
	delete m_player;
	m_player = nullptr;

	//敵(Line系)のメモリ削除
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (m_eneLine[i] != nullptr)
		{
			delete m_eneLine[i];
			m_eneLine[i] = nullptr;
		}
	}

	//敵(Circle系)のメモリ削除
	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (m_eneCircle[i] != nullptr)
		{
			delete m_eneCircle[i];
			m_eneCircle[i] = nullptr;
		}
	}

	//ゴールのメモリ削除
	delete m_goal;
	m_goal = nullptr;

	DeleteGraph(m_gameScreenHandle);

}

void StageBase::Update(Input& input)
{
	//フレーム数をカウント
	m_frame++;

	//更新
	if (m_isEndFadeIn)
	{
		m_player->Update(input);
	}
	//enemy->Update();

	//当たり判定の設定
	m_playerColEnemy = m_player->GetColRect();
	m_playerColGoal = m_player->GetColRectGoal();

	//enemyLineの更新
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (m_eneLine[i])
		{
			m_eneLine[i]->Update();

			//使用済みの敵キャラクタを削除する必要がある
			if (!m_eneLine[i]->isExist())
			{
				//メモリを解放する
				delete m_eneLine[i];
				m_eneLine[i] = nullptr;
			}
			//存在する敵の場合
			else
			{
				//当たっていない状態の時
				if (!m_isHit)
				{
					//画面が揺れていなければ
					if (m_shakeFrame == 0)
					{
						//敵が画面恥についた時の振動を取得
						m_shakeFrame = m_eneLine[i]->GetShakeFrame();
						//揺れ幅を初期値に戻す
						m_amp = 20.0f;
					}

					//敵の当たり判定を受け取って
					Col enemyLineCol = m_eneLine[i]->GetColRect();
					//プレイヤーと敵が当たっていたら
					if (m_playerColEnemy.IsCollision(enemyLineCol))
					{
						//当たった時の処理をする
						PlayerHit();
						m_isHit = true;
					}
				}
			}
		}
	}
	
	//不要になった敵をここで削除処理する
	auto lIt = remove_if(m_eneLine.begin(), m_eneLine.end(), [](auto& v) {
		return v == nullptr;
	});
	m_eneLine.erase(lIt, m_eneLine.end());

	//eneCircleの更新処理
	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (m_eneCircle[i])
		{
			m_eneCircle[i]->Update();

			//使用済みの敵キャラクタを削除する必要がある
			if (!m_eneCircle[i]->isExist())
			{
				//メモリを解放する
				delete m_eneCircle[i];
				m_eneCircle[i] = nullptr;
			}
			else
			{
				if (!m_isHit)
				{
					//敵の当たり判定を受け取って
					Col enemyCircleCol = m_eneCircle[i]->GetColRect();
					//プレイヤーと敵が当たっていたら
					if (m_playerColEnemy.IsCollisionCircle(enemyCircleCol))
					{
						//当たった時の処理をする
						PlayerHit();
						m_isHit = true;
					}
				}
			}
		}
	}

	//もし敵に当たっていたら
	if (m_isHit)
	{
		m_isHit = false;
	}

	//不要になった敵をここで削除処理する
	auto cIt = remove_if(m_eneCircle.begin(), m_eneCircle.end(), [](auto& v) {
		return v == nullptr;
		});
	m_eneCircle.erase(cIt, m_eneCircle.end());

	//画面揺れフレームのカウントダウン
	if (m_shakeFrame > 0)
	{
		//画面を揺らすフレームを減らす
		m_shakeFrame--;
		//画面が揺れる幅の倍率を少しずつ減らす
		m_amp *= 0.88f;
		//ある程度、値が小さくなると0に補正する
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

	//ゴールした時
	if (m_onGoal)
	{
		//ゴールエフェクト開始フラグをtrueにする
		m_isStartGoalEfect = true;
		//ゴールエフェクトの初期化
		m_goalEfect->Init(m_player->GetPlayerPos());
		//ゴール時のSEの流す
		m_manager.m_sManager.PlaySE("GoalSE");
		//ゴールしたフラグをfalseにする
		m_onGoal = false;
	}

	//ゴールエフェクト開始フラグがtrueになったとき
	if (m_isStartGoalEfect)
	{
		//ゴールエフェクトが始まってからのフレームを更新する
		m_goalEfectFrame++;
		//ゴールエフェクトの更新処理をする
		m_goalEfect->Update();
	}

	//BGMをフェードアウトするタイミングが来たら
	if (m_isSoundFadeOut)
	{
		//サウンドのフェードアウト開始からのフレームを更新する
		m_soundFadeFrame++;
	}

	//敵に当たった時画面をピンク色で点滅させるための処理
	if (m_isHitFlash)
	{
		//アルファ値を増やす
		m_alpha += 100;
		if (m_alpha > 200)
		{
			m_isHitFlash = false;
		}
	}
	else //敵に当たっていないときでalphaが0以上の時は0にする
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
	//描画先スクリーンをクリアする
	ClearDrawScreen();

	//背景の描画
	DrawBox(0, 0, 1280, 720, 0x0d1118, true);

	//チュートリアルステージの時はテキストを表示する
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


	//進捗の灰色の部分の描画
	DrawBox(520, 20, 760, 30, 0x222222, true);
	//進捗の水色の部分の描画
	DrawBox(520, 20, static_cast<int>(m_progressPos), 30, 0x00ffff, true);

	//ゴールエフェクトの描画
	if (m_isStartGoalEfect)
	{
		m_goalEfect->Draw();
	}

	//Line系の敵の描画
	for (int i = 0; i < m_eneLine.size(); i++)
	{
		if (m_eneLine[i])
		{
			m_eneLine[i]->Draw();
		}
	}

	//Circle系の敵の描画
	for (int i = 0; i < m_eneCircle.size(); i++)
	{
		if (m_eneCircle[i])
		{
			m_eneCircle[i]->Draw();
		}
	}

	//プレイヤーの描画
	m_player->Draw();

	//ゴールの描画
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

	//敵に当たった時の画面の点滅を描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x660066, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	//デバッグ用
	//DrawFormatString(640, 0, 0xffffff, "%.2f", nowTime * 0.000001f);
	DrawFormatString(640, 16, 0xff0000, "%d", m_frame);
#endif

	//ゲーム画面をバックバッファに描画する
	//バックバッファに書き込む設定に戻しておく
	SetDrawScreen(DX_SCREEN_BACK);
	int screenX = 0;
	int screenY = 0;

	if (m_shakeFrame > 0)
	{
		//画面揺れ
		screenX = static_cast<int>(m_amp) * (GetRand(2) - 1);
		screenY = static_cast<int>(m_amp) * (GetRand(2) - 1);
	}


	//フェードインの描画
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
