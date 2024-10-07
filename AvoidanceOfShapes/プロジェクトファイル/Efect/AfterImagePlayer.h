#pragma once
#include "../Util/Vec2.h"
#include "../Util/Game.h"

class AfterImagePlayer
{
public:
	AfterImagePlayer();
	~AfterImagePlayer();

	void Init(Vec2 pos, Vec2 vec, int size, bool isDash);
	void Update();
	void Draw();

	void SetSpeed(int speed) { m_growSpeed = speed; };

	bool isExist() { return m_isExsit; }
private:
	bool m_isExsit;

	Vec2 m_pos;
	Vec2 m_vec;
	int m_size;
	int m_growSpeed;

	int m_frame;
	int m_fadeFrame;
	int m_alpha;
};

