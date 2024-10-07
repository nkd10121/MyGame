#pragma once
#include "Vec2.h"
#include "Col.h"
#include <vector>

class Input;
class AfterImagePlayer;
class Efects;

class Player
{
public:
	Player();
	~Player();
	
	void Init(bool isInStage);
	void Update(Input& input);
	void Draw();

	void SetPlayerHandle(int handle0, int handle1, int handle2, int handle3);

	/// <summary>
	/// プレイヤーの当たり判定を取得する
	/// </summary>
	/// <returns>プレイヤーの当たり判定の座標</returns>
	Col GetColRect() const { return m_col; }

	Col GetColRectGoal() const { return m_colOfGoal; }


	/// <summary>
	/// プレイヤーの右側のX座標を取得する
	/// </summary>
	/// <returns>プレイヤーの右側のX座標</returns>
	Vec2 GetPlayerPos()const {return m_pos;}

	/// <summary>
	/// プレイヤーが敵に当たった時の処理
	/// </summary>
	void HitEnemy();

	/// <summary>
	/// 敵に当たった回数を返す
	/// </summary>
	/// <returns>敵に当たった回数</returns>
	int ReturnHitEnemyNum()const { return m_hitCount; }

	void CreateAfterImage(Vec2 pos,Vec2 vec, int size, bool isDash);

	void CreateDashEfect(Vec2 pos, bool isLastHit = false);

	void CreateHitEfect(Vec2 pos, bool isLastHit = false);
private:
	//プレイヤーのグラフィックハンドル
	int m_playerHandle[4];

	bool m_isInStage;

	//プレイヤーの位置
	Vec2 m_pos;
	//プレイヤーの当たり判定の位置
	Vec2 m_colPos;

	//画面サイズ Game::に直す
	int m_ScreenSizeW;
	int m_ScreenSizeH;

	int m_frame = 0;

	//敵との当たり判定用の矩形
	Col m_col;

	//ゴールとの当たり判定用の矩形
	Col m_colOfGoal;



	//Padのスティックを傾けた向きと大きさを入れるよう
	int m_padStickX;
	int m_padStickY;

	//スティック入力のおおきさをfloatにする用
	float m_powerX;
	float m_powerY;

	float m_angle;

	//プレイヤーのスピード
	float m_growSpeed;

	//ダッシュを押したときのカウント
	int m_dashCount;
	//ダッシュしているか
	bool m_isDash;

	//敵に当たった回数
	int m_hitCount;

	//ノックバックした時のスピード
	int m_knockBackSpeed;
	//Vec2 m_knockBackSpeed;

	//敵にヒットした時プレイヤーが点滅するように
	int m_damageDrawFrame;

	//敵にヒットした時に無敵時間
	int m_playerInvincibleTime;

	std::vector<AfterImagePlayer*> m_afterImage;
	Vec2 m_posLog;

	std::vector<Efects*> m_efects;
};

