#pragma once
#include "../../Util/Vec2.h"
#include "../../Util/Col.h"

/// <summary>
/// EnemyLine系の基底クラス
/// </summary>
/// 
class EnemyLineBase
{
public:
	EnemyLineBase();
	virtual ~EnemyLineBase();

	//純粋仮想関数(継承先でオーバーロードする)
	
	/// <summary>
	/// Line用の初期化関数
	/// </summary>
	/// <param name="pos">表示座標</param>
	/// <param name="width">敵の幅(大きさ)</param>
	virtual void Init(Vec2 pos, int width, int firstFrame, int secondFrame) {};

	/// <summary>
	/// Box用の初期化関数
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="speed">大きくなる速さ</param>
	/// <param name="firstFrame">本体が描画されるまでのフレーム</param>
	/// <param name="secondFrame">本体が小さくなり始めるフレーム</param>
	/// <param name="isPre">予測線の有無　true:予測線あり,false:予測線なし</param>
	virtual void Init(Vec2 pos, int size, int speed, int firstFrame, int secondFrame,bool isPre) {};
	void Init() {};
	virtual void Update() = 0;
	virtual void Draw() = 0;

	/// <summary>
	/// 当たり判定の座標を取得する
	/// </summary>
	/// <returns>当たり判定の座標</returns>
	Col GetColRect() const { return m_col; }

	int GetShakeFrame() { return m_shakeFrame; }

	/// <summary>
	/// 画面内に存在するかのフラグを返す
	/// </summary>
	/// <returns>存在しているかフラグ</returns>
	bool isExist() const { return m_isExist; }

protected:
	//右上の座標x,y
	Vec2 m_startPos;
	//左下の座標x,y
	Vec2 m_endPos;

	//当たり判定の座標
	Col m_col;

	int m_frame;
	int m_waitFrame;
	int m_disappearFrame;

	int m_color;

	int m_shakeFrame;
	bool m_isShake;

	bool m_isExist;
};

