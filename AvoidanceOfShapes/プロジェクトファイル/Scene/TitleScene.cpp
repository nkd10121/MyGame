#include <DxLib.h>
#include <cmath>
#include "Input.h"
#include "Application.h"
#include "Game.h"
#include "SceneManager.h"
#include "DrawBGparts.h"
#include "TitleScene.h"
#include "StageSelect.h"
#include "OptionScene.h"

namespace
{
	//フェードイン、フェードアウトにかかる時間
	constexpr int kFadeFrame = 30;

	//ボタンが左右に動く速さ
	constexpr int kButtonMoveSpeed = 24;
	//背景の四角の数
	constexpr int kBGnum= 10;

	//デモムービーが流れ始めるフレーム
	constexpr int kPlayMovieFrame = 600;

	//プレイボタンのy座標
	constexpr int kPlayButtonY = 380;
	//オプションボタンのy座標
	constexpr int kOptionButtonY = 560;
	//やめるボタンのy座標
	constexpr int kQuitButtonY = 740;
}

TitleScene::TitleScene(SceneManager& manager) :
	Scene(manager),
	m_playButtonPosX(680),
	m_optionButtonPosX(800),
	m_quitButtonPosX(800),
	m_movieHandle(0)
{
	//タイトルで流す動画のハンドルを取得
	m_movieHandle = m_manager.m_gManager.GetHandle("TitleMovie");

	//背景の四角の初期化処理
	m_parts.resize(kBGnum);
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i] = new DrawBGparts;
		m_parts[i]->Init(false);
	}

	m_fadeFrame = 60;
	updateFunc_ = &TitleScene::FadeInUpdate;
	drawFunc_ = &TitleScene::FadeOutDraw;
}

TitleScene::~TitleScene()
{

}

void TitleScene::Update(Input& input)
{
	//BGMを流す
	m_manager.m_sManager.PlayBGM("Title", true);

	(this->*updateFunc_)(input);
	m_frame++;

	//背景の四角の更新処理
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i]->Update();
	}

	//タイトルロゴがBGMと合わせて動くように
	if (m_frame == 70)
	{
		m_titleSize = m_titleSize * 1.08f;

	}
	else if (m_frame== 140)
	{
		m_titleSize = m_titleSize * 1.08f;
		m_frame = 0;
	}
	else
	{
		m_titleSize = 0.28f;
	}

	//playボタンを選択している場合
	if (m_nowButton == Play)
	{
		//playボタンを左に動かす
		if (m_playButtonPosX >= 680)
		{
			m_playButtonPosX -= kButtonMoveSpeed;
		}
	}
	//選択していない場合
	else
	{
		//右に動かす
		if (m_playButtonPosX <= 800)
		{
			m_playButtonPosX += kButtonMoveSpeed;
		}
	}

	if (m_nowButton == Option)
	{
		if (m_optionButtonPosX >= 680)
		{
			m_optionButtonPosX -= kButtonMoveSpeed;
		}
	}
	else
	{
		if (m_optionButtonPosX <= 800)
		{
			m_optionButtonPosX += kButtonMoveSpeed;
		}
	}

	if (m_nowButton == Quit)
	{
		if (m_quitButtonPosX >= 680)
		{
			m_quitButtonPosX -= kButtonMoveSpeed;
		}
	}
	else
	{
		if (m_quitButtonPosX <= 800)
		{
			m_quitButtonPosX += kButtonMoveSpeed;
		}
	}

	if (m_waitFrame == kPlayMovieFrame)
	{
		m_isPlayMovie = true;
	}

	if (m_isPlayMovie)
	{
		m_manager.m_sManager.FadeOutBGM("Title", m_waitFrame - kPlayMovieFrame);

		if (m_waitFrame > kPlayMovieFrame)
		{
			if (GetMovieStateToGraph(m_movieHandle) == 0)
			{
				m_waitFrame = 0;

				m_frame = 0;
				m_isPlayMovie = false;

				PauseMovieToGraph(m_movieHandle);
				SeekMovieToGraph(m_movieHandle, 0);
			}

			if (GetJoypadInputState(DX_INPUT_KEY_PAD1) || CheckHitKeyAll() != 0)
			{
				m_waitFrame = 0;

				m_frame = 0;
				m_isPlayMovie = false;

				PauseMovieToGraph(m_movieHandle);
				SeekMovieToGraph(m_movieHandle, 0);
			}
		}	
	}
}

void TitleScene::Draw()
{
	//BG
	for (int i = 0; i < m_parts.size(); i++)
	{
		m_parts[i]->Draw();
	}

	//ロゴ
	m_manager.Draw(340, m_titlePosY, m_titleSize, 0, "Title");

	//ボタン
	m_manager.Draw(m_playButtonPosX, kPlayButtonY, 1.0, 0, "Button1");
	DrawStringToHandle(m_playButtonPosX + 148, kPlayButtonY - 180, "スタート", 0xffffff, m_manager.ReturnFontHandle());

	m_manager.Draw(m_optionButtonPosX, kOptionButtonY, 1.0, 0, "Button2");
	DrawStringToHandle(m_optionButtonPosX + 148, kOptionButtonY - 180, "オプション", 0xffffff, m_manager.ReturnFontHandle());

	m_manager.Draw(m_quitButtonPosX, kQuitButtonY, 1.0, 0, "Button3");
	DrawStringToHandle(m_quitButtonPosX + 148, kQuitButtonY - 180, "やめる", 0xffffff, m_manager.ReturnFontHandle());

#ifdef _DEBUG
	DrawFormatString(0, 16, 0xff0000, "%d", m_waitFrame);
#endif
	(this->*drawFunc_)();

	if (m_isPlayMovie)
	{
		PlayMovieToGraph(m_movieHandle);
		DrawGraph(0, 0, m_movieHandle, false);

		m_manager.m_gManager.Draw(640, 360, 1.0f, 0.0f, "DemoText");
	}
}

void TitleScene::FadeInUpdate(Input&)
{
	m_fadeFrame--;
	if (m_fadeFrame <= 0)
	{
		//フェードインが終わったら状態を遷移する
		//次の遷移先
		updateFunc_ = &TitleScene::NormalUpdate;
		drawFunc_ = &TitleScene::NormalDraw;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	//ログを残す
	m_nowButtonLog = m_nowButton;

	//ムービー中はシーン遷移をさせない
	if (!m_isPlayMovie)
	{
		//上方向を押したとき
		if (input.IsTriggered("UP"))
		{
			//プレイボタンを選択している場合
			if (m_nowButton == Play)
			{
				//下に戻す？
				/*nowButton = Quit;*/
			}
			//オプションボタンを選択している場合
			else if (m_nowButton == Option)
			{
				m_manager.m_sManager.PlaySE("moveSE");
				//プレイボタンを選択している状態にする
				m_nowButton = Play;
			}
			//やめるボタンを選択している場合
			else if (m_nowButton == Quit)
			{
				m_manager.m_sManager.PlaySE("moveSE");
				//オプションボタンを選択している状態にする
				m_nowButton = Option;
			}
		}

		//下方向を押したとき
		if (input.IsTriggered("DOWN"))
		{
			if (m_nowButton == Play)
			{
				m_manager.m_sManager.PlaySE("moveSE");
				//オプションボタンを選択している状態にする
				m_nowButton = Option;
			}
			else if (m_nowButton == Option)
			{
				m_manager.m_sManager.PlaySE("moveSE");
				//やめるボタンを選択している状態にする
				m_nowButton = Quit;
			}
			else if (m_nowButton == Quit)
			{
				//now = Play;
			}
		}

		//OKボタンを押したとき
		if (input.IsTriggered("OK"))
		{
			//プレイボタンを選択しているとフェードアウトを始める
			if (m_nowButton == Play)
			{
				m_manager.m_sManager.PlaySE("enterSE");

				updateFunc_ = &TitleScene::FadeOutUpdate;
				drawFunc_ = &TitleScene::FadeOutDraw;
				m_fadeFrame = 0;
			}

			//オプションボタンでOKを押したとき
			if (m_nowButton == Option/* || input.IsTriggered("pause")*/)
			{
				m_manager.m_sManager.PlaySE("enterSE");

				//オプション画面を表示する
				m_manager.PushScene(std::make_shared<OptionScene>(m_manager));
			}

			//やめるボタンでOKを押したとき
			if (m_nowButton == Quit)
			{
				//プロジェクトを終了する
				Application::GetInstance().Terminate();
			}
		}
	}

	//カーソルの操作がないとき、ムービーが流れるまでのカウントを進める
	if (m_nowButton == m_nowButtonLog)
	{
		m_waitFrame++;
	}
	else
	{
		m_waitFrame = 0;
	}
}

void TitleScene::FadeOutUpdate(Input&)
{
	m_fadeFrame++;
	m_manager.m_sManager.FadeOutBGM("Title", m_fadeFrame);
	if (m_fadeFrame >= kFadeFrame)
	{
		//フェードアウトが終わったらSceneを変える
		if (m_nowButton == Play)
		{
			m_manager.ChangeScene(std::make_shared<StageSelect>(m_manager));
		}
	}
}

void TitleScene::FadeOutDraw()
{
#ifdef _DEBUG
	DrawString(0, 0, "TitleScene", 0xffffff);
#endif

	float rate = static_cast<float>(m_fadeFrame) / static_cast<float>(kFadeFrame);
	float alpha = 255.0f * rate;
	SetDrawBlendMode(DX_BLENDMODE_MULA, static_cast<int>(alpha));
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
#ifdef _DEBUG
	DrawString(0, 0, "TitleScene", 0xffffff);
#endif
}