#pragma once

class GraphManager;

class DrawBGparts
{
public:
	DrawBGparts();
	~DrawBGparts();

	void Init(bool isMove);
	void Update();
	void Draw();

private:
	GraphManager m_gManager;

	int m_frame = 0;
	int m_handle = 0;
	bool m_isMove = false;

	//x���W
	int m_x = 0;
	//y���W
	int m_y = 0;
	//�䗦
	float m_exrate = 0.0f;
	//�p�x
	float m_angle = 0.0f;

	float m_rotateAngle = 0.0f;

	int m_rotateFrame = 0;
	//���S����̔��a
	int m_rad = 0;

	float m_offsetX = 0.0f;
	float m_offsetY = 0.0f;

	float m_rotateSpeed = 0;
	float m_mag = 1.0f;
};

