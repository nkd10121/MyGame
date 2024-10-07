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
	/// ��ʓ��ɑ��݂��邩�̃t���O��Ԃ�
	/// </summary>
	/// <returns>���݂��Ă��邩�t���O</returns>
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