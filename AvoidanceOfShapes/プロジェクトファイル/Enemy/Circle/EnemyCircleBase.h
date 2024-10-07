#pragma once
#include "DxLib.h"
#include "../../Util/Vec2.h"
#include "../../Util/Col.h"

class EnemyCircleBase
{
public:
	EnemyCircleBase();
	~EnemyCircleBase();

	//純粋仮想関数(継承先でオーバーロードする)
	//EnemyCircle用スタート関数
	virtual void Init(float pos, int radius, bool isSmooth) {};
	//EnemyBurstedCircle用スタート関数
	virtual void Init(Vec2 pos, Vec2 vec, int speed, int first, int second) {};
	//EnemyCircleScaling用スタート関数
	virtual void Init(Vec2 pos,int rad, int time = 60, int draw = 120,int speed = 20) {};
	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// 当たり判定の座標を取得する
	/// </summary>
	/// <returns>当たり判定の座標</returns>
	Col GetColRect() const { return m_col; }

	/// <summary>
	/// 画面内に存在するかのフラグを返す
	/// </summary>
	/// <returns>存在しているかフラグ</returns>
	bool isExist() const { return m_isExist; }
protected:
	//座標
	Vec2 m_pos;

	// 始点座標
	Vec2 m_startPos;
	// 終点座標
	Vec2 m_endPos;

	// 当たり判定の座標
	Col m_col;

	//本体の色
	int m_color;

	//存在するかしないか
	bool m_isExist;
};