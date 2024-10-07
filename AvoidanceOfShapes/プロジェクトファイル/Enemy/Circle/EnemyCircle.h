#pragma once
#include "EnemyCircleBase.h"

class EnemyCircle : public EnemyCircleBase
{
public:
	EnemyCircle();
	~EnemyCircle();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="radius">半径</param>
	/// <param name="isSmooth">形 true:円、false:とげとげ</param>
	void Init(float pos, int radius, bool isSmooth)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

private:
	//移動ベクトル
	Vec2 m_moveVec;

	//本体の半径
	int m_rad;

	//円かとげとげか
	//true:円、false:とげとげ
	bool m_isSmooth;

	//回転する四角の回転角度
	float m_angle;
	//回転する四角の中心から頂点までの距離
	int m_diagonal;

	//登場時のエフェクトの座標
	int m_efectCirclePos;
	//登場時のエフェクトの半径
	int m_effectCircleRad;
};