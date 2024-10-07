#pragma once
#include "../Line/EnemyLineBase.h"
#include "DxLib.h"

class EnemyBurstedCircle : public EnemyLineBase
{
public:
	EnemyBurstedCircle();
	~EnemyBurstedCircle();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="vec">移動方向</param>
	void Init(Vec2 pos,Vec2 vec);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	//現在座標
	Vec2 m_pos;

	//移動方向
	Vec2 m_vec;

};