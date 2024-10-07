#pragma once
#include "Scene.h"
#include "Vec2.h"

#include <vector>

/// <summary>
/// カーソル用
/// </summary>
enum Button
{
	Retry,
	Return
};

class DrawBGparts;
class Efects;
class AfterImagePlayer;

class GameoverScene : public Scene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="mgr"></param>
	GameoverScene(SceneManager& mgr);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameoverScene();

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="input">入力情報</param>
	void Update(Input& input);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	//フェード用フレーム
	int m_fadeFrame = 0;
	//フレームカウント用変数
	int m_frame = 0;

	//選択しているボタン
	int m_nowButton = Retry;

	//リトライボタンのX座標
	int m_retryButtonX;
	//戻るボタンのX座標
	int m_returnButtonX;

	//プレイヤーハンドル
	int m_handle[4];

	//プレイヤー座標
	Vec2 m_playerPos;
	//プレイヤーの座標差分
	Vec2 m_offset{};

	//背景のBox
	std::vector<DrawBGparts*> m_parts;

	//復活フラグ
	bool m_isRevival;
	//死亡フラグ
	bool m_deathAnimationFlag;

	//敵のY座標
	int enemyY = 0;

	//プレイヤーの残像
	std::vector<AfterImagePlayer*> m_afterImage;
	//プレイヤーのエフェクト
	std::vector<Efects*> m_efects;

	void CreateAfterImage(Vec2 pos);

	void CreateDashEfect(Vec2 pos, bool isLastHit = false);

	//状態遷移用メンバ関数ポインタの宣言
	using UpdateFunc_t = void (GameoverScene::*)(Input&);
	using DrawFunc_t = void (GameoverScene::*)();

	UpdateFunc_t updateFunc_ = nullptr;
	DrawFunc_t drawFunc_ = nullptr;

	//更新関数
	void FadeInUpdate(Input& input);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input& input);

	//描画関数
	void FadeOutDraw();
	void NormalDraw();
};

