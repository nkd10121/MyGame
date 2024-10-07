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
	//背景の色
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
	std::ofstream file("Data.txt");  // 書き出すファイルのパスを指定
	int info[5] = { m_BGMvolume,m_SEvolume,m_clearStageTutorial,m_clearStage1,m_clearStage2 };
	for (const auto test : info)
	{
		file << test << std::endl;  // 書き出し
	}
}

void SceneManager::Load()
{
	std::ifstream data("Data.txt");  // 読み込むファイルのパスを指定
	std::string line;

	//Data.txtが存在しないとき、初期値として5と3をいれて終わる
	if (!data) {
		m_BGMvolume = kDefaultBGMvolume;
		m_SEvolume = kDefaultSEvolume;
		m_clearStageTutorial = false;
		m_clearStage1 = false;
		m_clearStage2 = false;
		return;
	}

	//データをロードする
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
	//最初にロードする
	Load();

	m_isFull = Application::GetInstance().GetIsFullScreen();

	//使用するフォントを準備する
	m_fontPath = "data/font/PixelMplus12-Bold.ttf"; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(m_fontPath, FR_PRIVATE, NULL) > 0) {
	}
	else {
		// フォント読込エラー処理
		MessageBox(NULL, "フォント読込失敗", "", MB_OK);
	}

	//タイトル表示用のフォント
	m_fontHandle = CreateFontToHandle("PixelMplus12", kFontDoubleSize, -1);
	m_fontHandle2 = CreateFontToHandle("PixelMplus12", kFontSize, -1);

	/*UIなど*/
	//プレイヤー画像
	LoadG("Player", "data/player/player0.png");
	LoadG("Player1", "data/player/player1.png");
	LoadG("Player2", "data/player/player2.png");
	LoadG("Player3", "data/player/player3.png");

	//背景の四角の画像
	LoadG("BG", "data/titleParts.png");
	//タイトルのロゴ
	LoadG("Title", "data/title.png");
	//ボタン1
	LoadG("Button1", "data/Button1.png");
	//ボタン2
	LoadG("Button2", "data/Button2.png");
	//ボタン3
	LoadG("Button3", "data/Button3.png");
	//オプション、ポーズのフルスクリーンのチェックマーク
	LoadG("Check", "data/UI_check.png");
	//ゴールの三角形
	LoadG("Goal", "data/goal.png");

	/*操作方法関連*/
	//コントローラーの操作説明の画像
	LoadG("sousaCon", "data/sousa.png");
	//キーボードの操作説明の画像
	LoadG("sousaKey", "data/sousaKeyboard.png");

	/*ステージセレクトのステージの画像*/
	//チュートリアルの画像
	LoadG("stageSelect0", "data/stageSelect0.png");
	//暗く表示するための真っ暗にした画像
	LoadG("stageSelect0B", "data/stageSelect0Black.png");
	//Stage1の画像
	LoadG("stageSelect1", "data/stageSelect1.png");
	//暗く表示するための真っ暗にした画像
	LoadG("stageSelect1B", "data/stageSelect1Black.png");
	//Stage2の画像
	LoadG("stageSelect2", "data/stageSelect2.png");
	//暗く表示するための真っ暗にした画像
	LoadG("stageSelect2B", "data/stageSelect2Black.png");

	/*チュートリアルで表示する文字*/
	LoadG("Tutorial1", "data/text.png");
	LoadG("Tutorial2", "data/text2.png");
	LoadG("Tutorial3", "data/text3.png");

	/*movie関連*/
	//タイトルで流す動画
	LoadG("TitleMovie", "data/movie/title.mp4");
	LoadG("DemoText", "data/demoText.png");

	/*BMG関連*/
	m_sManager.Load("Title", "data/sound/Player.mp3",true);
	m_sManager.Load("Tutorial", "data/sound/Electric_Shine.mp3", true);
	m_sManager.Load("stage1", "data/sound/Kagura.mp3", true);
	m_sManager.Load("stage2", "data/sound/15 80's Graffiti .mp4", true);
	m_sManager.Load("GameOver", "data/sound/不穏_-_8bit.mp3", true);

	//プレイヤーが当たった時のSE
	m_sManager.Load("hitSE", "data/se/ヒット＿強打3.wav", false);
	//決定した時のSE
	m_sManager.Load("enterSE", "data/se/決定＿小決定（SF系）.wav", false);
	//カーソルを動かしたときのSE
	m_sManager.Load("moveSE", "data/se/カーソル（柔らかい）2.wav", false);
	//ゴールした時のSE
	m_sManager.Load("GoalSE", "data/se/maou_se_magical27.mp3", false);
	//ゲームオーバーから復活するとき
	m_sManager.Load("RevivalSE", "data/se/ゲージ回復1.mp3", false);
	//ゲームオーバーであきらめたとき
	m_sManager.Load("deathSE", "data/se/射撃＿ピストル.wav", false);
}

SceneManager::~SceneManager()
{
	// ********** フォントのアンロード **********
	if (RemoveFontResourceEx(m_fontPath, FR_PRIVATE, NULL)) {
	}
	//作成したフォントをメモリから解放する
	DeleteFontToHandle(m_fontHandle);

	//最後にロードする
	Save();
}

void SceneManager::Update(Input& input)
{
	//BG
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kBGcolor, true);

	m_sManager.ChangeBGMVolume(m_BGMvolume);
	m_sManager.ChangeSEVolume(m_SEvolume);

	//末尾のみ実行
	scenes.back()->Update(input);
}

void SceneManager::Draw()
{
	//先頭から順に描画
	//最後に積んだものが最後に描画される
	for (auto& scenes : scenes)
	{
		scenes->Draw();
	}
}

void SceneManager::ChangeScene(std::shared_ptr<Scene> nextScene)
{
	if (scenes.empty())//リストが空っぽだったら入れ替えずに
	{
		scenes.push_back(nextScene);//末尾に追加
	}
	else
	{
		m_logScene = *(scenes.begin());
		scenes.back() = nextScene;//既に一つ以上あれば末尾を
	}
}

void SceneManager::ChangeAndClearScene(std::shared_ptr<Scene> nextScene)
{
	scenes.clear();

	scenes.push_back(nextScene);//末尾に追加
	
}

void SceneManager::PushScene(std::shared_ptr<Scene> scene)
{
	scenes.push_back(scene);
}

void SceneManager::popScene(bool isToTitle)
{
	//シーンのログを更新して
	m_logScene = *(scenes.begin());
	if (!isToTitle)
	{
		//もし直前のシーンがstage1だったら
		//stage1のBGMを途中から再生する
		if (m_logScene == m_stage1)
		{
			m_sManager.PlayBGM("stage1", false);
		}
		//もし直前のシーンがstage2だったら
		//stage2のBGMを途中から再生する
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
