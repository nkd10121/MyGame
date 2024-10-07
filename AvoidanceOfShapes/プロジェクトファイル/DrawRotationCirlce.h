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
	/// <param name="Rr">���S����̔��a</param>
	/// <param name="clockwise">true = ���v���,false = �����v���</param>
	/// <param name="white">true = �F�𔒂�,false = �F���V�A����</param>
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

	//�c����\�����邽�߂ɉߋ��̈ʒu���o���Ă���
	std::vector<Vec2> m_posLog1;
	std::vector<Vec2> m_posLog2;

	bool m_isClockwise;
};