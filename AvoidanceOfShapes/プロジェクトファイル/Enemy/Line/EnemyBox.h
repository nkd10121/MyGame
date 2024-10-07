#pragma once
#include "EnemyLineBase.h"
class EnemyBox : public EnemyLineBase
{
public:
	EnemyBox();
	~EnemyBox();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="size">最大の大きさ</param>
	/// <param name="speed">大きくなる速さ</param>
	/// <param name="drawFrame">本体が描画されるまでのフレーム</param>
	/// <param name="smallFrame">本体が小さくなり始めるフレーム</param>
	/// <param name="isPre">予測線の有無　true:予測線あり,false:予測線なし</param>
	void Init(Vec2 pos, int size, int speed, int drawFrame, int smallFrame,bool isPre)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

private:
	//現在の中心座標
	Vec2 m_pos;
	//大きさの最大値
	int m_sizeMax;
	//現在の大きさ
	int m_size;
	//大きくなる速さ
	int m_growSizeSpeed;
	//本体の色
	unsigned int m_color;

	//現在の大きさが大きさの最大値と同じかそれ以上になったかどうか
	bool m_isMax;
	//予測線の描画の有無
	bool m_isDrawPre;
};

