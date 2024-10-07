#pragma once
#include "EnemyCircleBase.h"

class EnemyCircleScaling : public EnemyCircleBase
{
public:
	EnemyCircleScaling();
	~EnemyCircleScaling();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rad">大きさ</param>
	/// <param name="wait">待機時間</param>
	/// <param name="draw">描画時間</param>
	/// <param name="speed">小さくなる速さ</param>
	void Init(Vec2 pos, int rad,int wait,int draw,int speed)override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	//フレーム数
	int m_frame;
	//予測線の回転角度
	float m_angle;
	//予測線の中心からの距離
	Vec2 m_offset;
	
	//本体が描画されるまでのフレーム数
	int m_waitFrame;
	//本体が描画されて消え始めるまでのフレーム数
	int m_drawFrame;

	//予測線の半径
	int m_radius;
	//本体の半径
	int m_mainCircleRadius;

	//本体が小さくなる速さ
	int m_radiusSmallSpeed;

	//通常の色(ピンク)
	unsigned int m_defaultColor;
	//通常の色に加算する色(白)
	unsigned int m_addColor;
};

