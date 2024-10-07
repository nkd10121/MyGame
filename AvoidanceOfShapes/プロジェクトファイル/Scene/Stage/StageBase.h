#pragma once
#include <vector>

#include "DxLib.h"
#include "../Scene.h"
#include "../../Util/Vec2.h"
#include "../../Util/Col.h"
#include "../../Util/Game.h"

class Player;
class GoalEfect;
class EnemyLineBase;
class EnemyCircleBase;
class Goal;

class StageBase : public Scene
{
public:
	StageBase(SceneManager& mgr);
	~StageBase();
	virtual void Update(Input& input);
	virtual void Draw();

	/// <summary>
	/// ライン系の敵の作成関数
	/// </summary>
	/// <param name="pos">右上の座標</param>
	/// <param name="width">大きさ</param>
	/// <param name="first">本体が動くフレーム</param>
	/// <param name="second">本体が消え始めるフレーム</param>
	/// <param name="enemy">Lineの種類</param>
	void CreateEnemyLineFrom(Vec2 pos, int width,int first,int second, EnemyLineBase* enemy);

	/// <summary>
	/// Box敵の作成関数
	/// </summary>
	/// <param name="pos">中心座標</param>
	/// <param name="size">一辺の長さ</param>
	/// <param name="speed">大きくなる速さ</param>
	/// <param name="firstFrame">本体が出てくるフレーム</param>
	/// <param name="secondFrame">本体が消え始めるフレーム</param>
	/// <param name="isPre">予備動作をするかどうか</param>
	void CreateEnemyBox(Vec2 pos, int size, int speed, int firstFrame, int secondFrame,bool isPre = true);

	void CreateEnemyCircle(float pos, int radius, bool isSmooth);

	void CreateEnemyBurst(Vec2 pos, Vec2 vec, int speed, int first, int second);

	void CreateEnemyCircleScaling(Vec2 pos, int rad,int first,int second,int speed = 20);

	void AddEnemy(EnemyLineBase* test);

protected:
	//Updateの最初のフレームの処理のためのbool
	bool m_isFirst;

	//フェードインが終わったかどうか
	bool m_isEndFadeIn;

	/*プレイヤー関連*/
	//プレイヤー本体
	Player* m_player;
	//プレイヤーの敵との当たり判定
	Col m_playerColEnemy;
	//プレイヤーのゴールとの当たり判定
	Col m_playerColGoal;

	//複数の敵に同時に当たると一気にHPが減るのを防ぐためのbool
	bool m_isHit;

	/*矩形の当たり判定を持つ敵関係*/
	std::vector<EnemyLineBase*> m_eneLine;
	Vec2 m_eneLinePos;
	int m_lineWidth;

	/*円の当たり判定を持つ敵関係*/
	std::vector<EnemyCircleBase*> m_eneCircle;
	Vec2 m_eneCirclePos;
	Vec2 m_eneCircleBurstVec;
	int m_eneCircleRadius;
	int m_eneCircleBurstSpeed;

	int m_frame;

	Goal* m_goal;
	bool m_goalFlag;
	bool m_onGoal;

	//上のゲージのやつ
	float m_progressSpeed;

	float m_progressPos;

	/*画面揺らす関係*/
	int m_screenX;
	int m_screenY;
	int m_shakeFrame;
	int m_gameScreenHandle;
	float m_amp;

	//ゴールエフェクト
	bool m_isStartGoalEfect;
	int m_goalEfectFrame;
	GoalEfect* m_goalEfect;

	//サウンドのフェードアウト
	bool m_isSoundFadeOut;
	int m_soundFadeFrame;

	//画面点滅のbool
	bool m_isHitFlash;
	int m_alpha;

	bool m_isTutorial;

	///////////////////////////////
	float m_mag;
	float m_fadeoutAngle;

	float m_magSpeed;
	float m_fadeoutAngleSpeed;

	std::vector<int> m_posX;
	std::vector<int> m_posY;
	///////////////////////////////


	//状態遷移用メンバ関数ポインタの宣言
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)();

	UpdateFunc_t updateFunc_ = nullptr;
	DrawFunc_t drawFunc_ = nullptr;

	int m_fadeFrame;


	//プレイヤーが敵に当たった時の処理をまとめた関数
	void PlayerHit();

	//更新関数
	virtual void FadeInUpdate(Input& input);
	virtual void NormalUpdate(Input& input);
	virtual void FadeOutUpdate(Input& input);

	//描画関数
	void FadeInDraw();
	void FadeOutDraw();
	void NormalDraw();
};

