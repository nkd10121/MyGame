#pragma once
#include "Scene.h"
#include "DxLib.h"

#include <vector>

class DrawBGparts;

/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	~TitleScene();
	void Update(Input& input);
	void Draw();

private:
	/// <summary>
	/// 選択しているボタン
	/// </summary>
	enum ButtonName : int
	{
		Play,
		Option,
		Quit
	};

	//選択しているボタン
	int m_nowButton = Play;
	int m_nowButtonLog = m_nowButton;

	int m_fadeFrame = 0;
	int m_frame = 3;

	//背景を振動させる倍率
	float m_mag = 1.0f;
	//タイトルロゴのサイズ
	float m_titleSize = 0.28f;
	//タイトルのY座標
	int m_titlePosY = 280;

	//プレイボタンのX座標
	int m_playButtonPosX;
	//オプションボタンのX座標
	int m_optionButtonPosX;
	//やめるボタンのX座標
	int m_quitButtonPosX;

	/*デモ動画関連*/
	int m_movieHandle;
	int m_waitFrame = 0;
	bool m_isPlayMovie = false;

	std::vector<DrawBGparts*> m_parts;

	//更新メンバ関数ポインタ
	void (TitleScene::* updateFunc_)(Input& input);

	//描画メンバ関数ポインタ
	using DrawFunc_t = void (TitleScene::*)();
	DrawFunc_t drawFunc_;

	//更新関数
	void FadeInUpdate(Input&);	//フェードイン状態
	void NormalUpdate(Input& input);	//通常状態
	void FadeOutUpdate(Input&);	//フェードアウト状態

	//描画関数
	void FadeOutDraw();
	void NormalDraw();
};