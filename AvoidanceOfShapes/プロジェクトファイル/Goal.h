#pragma once
#include "Vec2.h"
#include "Col.h"
#include "DrawRotationCirlce.h"

class Goal
{
public:
	Goal();
	~Goal();

	void Init();
	void Update();
	void Draw();

	void SetGraphHandle(int handle) { m_handle = handle; };

	void DrawQuilateralTriangle(Vec2 centerPos, int sideLength, unsigned int color);

	void Hit();

	Col GetColRect() const { return m_col; }

	/// <summary>
	/// 画面内に存在するかのフラグを返す
	/// </summary>
	/// <returns>存在しているかフラグ</returns>
	bool isExist() const { return m_isExist; }
private:
	int m_frame;

	int m_handle = 0;

	Vec2 m_center;
	Col m_col;
	bool m_isExist;

	DrawRotationCirlce m_inSide;
	DrawRotationCirlce m_outSide;
};