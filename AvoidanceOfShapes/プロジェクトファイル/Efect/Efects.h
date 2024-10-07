#pragma once
#include "../Util/Vec2.h"

class Efects
{
public:
	Efects(){};
	virtual ~Efects(){};

	bool isExist() { return m_isExist; }

	virtual void Init(Vec2 pos,bool isLastHit){};
	virtual void Update(){};
	virtual void Draw(){};

	void SetGraphHandle(int handle) { m_handle = handle; };

protected:
	bool m_isExist = false;

	int m_frame = 0;
	Vec2 m_pos;
	int m_rad = 20;
	float m_lineThick = 20.0f;

	//dashEfect
	int m_alpha = 160;

	//goalEfect
	float m_angle = 0;

	//hit
	int m_handle = -1;


};

