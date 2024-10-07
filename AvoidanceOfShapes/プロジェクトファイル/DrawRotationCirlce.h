#pragma once
#include "Vec2.h"
#include <vector>

class DrawRotationCirlce
{
public:
	DrawRotationCirlce();
	~DrawRotationCirlce();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="Rr">中心からの半径</param>
	/// <param name="clockwise">true = 時計回り,false = 反時計回り</param>
	/// <param name="white">true = 色を白に,false = 色をシアンに</param>
	void Init(int Rr, bool clockwise = true, bool white = false);
	void Update(Vec2 pos);
	void Draw();
private:
	Vec2 m_center;

	Vec2 m_pos1;
	Vec2 m_pos2;
	int m_frameCount;
	float m_angle;
	int m_circleRadius;
	int m_rotateRadius;

	bool m_isWhite;

	//残像を表示するために過去の位置を覚えておく
	std::vector<Vec2> m_posLog1;
	std::vector<Vec2> m_posLog2;

	bool m_isClockwise;
};