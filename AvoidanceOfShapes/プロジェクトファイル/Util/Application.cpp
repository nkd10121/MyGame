#include "Application.h"
#include "SceneManager.h"
#include "TitleScene.h"
#include "Input.h"
#include "Game.h"

#include <DxLib.h>
#include <assert.h>

//デバッグ用
#include "StageTutorial.h"
#include "Stage1.h"
#include "Stage2.h"
#include "GameoverScene.h"
namespace
{
	constexpr int screen_width = Game::kScreenWidth;
	constexpr int screen_height = Game::kScreenHeight;
}

void Application::Terminate()
{
	DxLib_End();
}

bool Application::Init()
{
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	m_windowSize = { screen_width,screen_height };
	//ChangeWindowMode(m_isFullScreen);	//非スクリーン

	SetWindowIconID(111);
	//ChangeScreenSize();

	SetGraphMode(m_windowSize.w, m_windowSize.h, 32);
	//ウィンドウ名を変更
	SetWindowText("AvoidanceOfShapes");
	if (DxLib_Init() == -1) {	//初期化に失敗したらアプリを落とす
		return false;	//異常終了
	}
	SetDrawScreen(DX_SCREEN_BACK);
	return true;
}

void Application::Run()
{
	SceneManager manager;
	manager.ChangeScene(std::make_shared<TitleScene>(manager));
	//manager.ChangeScene(std::make_shared<StageTutorial>(manager));
	//manager.ChangeScene(std::make_shared<Stage2>(manager));
	//manager.ChangeScene(std::make_shared<Stage3>(manager));
	//manager.ChangeScene(std::make_shared<GameoverScene>(manager));

	Input input;

	while (ProcessMessage() != -1) {
		LONGLONG time = GetNowHiPerformanceCount();

		ClearDrawScreen();

		input.Update();
		manager.Update(input);
		manager.Draw();

		ScreenFlip();

		while (GetNowHiPerformanceCount() - time < 16667);
	}

	Terminate();
}

const Size& Application::GetWindowSize() const
{
	return m_windowSize;
}

void Application::ChangeScreenSize()
{
	ChangeWindowMode(m_isFullScreen);
	m_isFullScreen = !m_isFullScreen;
}

