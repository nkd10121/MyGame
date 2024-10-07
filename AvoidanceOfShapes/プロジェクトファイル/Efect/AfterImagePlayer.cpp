#include "AfterImagePlayer.h"
#include "DxLib.h"

namespace
{
	constexpr int kFadeFrame = 20;
}

AfterImagePlayer::AfterImagePlayer() :
	m_isExsit(false),
	m_pos(0.0f, 0.0f),
	m_vec(0.0f, 0.0f),
	m_size(0),
	m_growSpeed(0),
	m_frame(0),
	m_fadeFrame(0),
	m_alpha(160)
{
}

AfterImagePlayer::~AfterImagePlayer()
{
}

void AfterImagePlayer::Init(Vec2 pos, Vec2 vec,int size,bool isDash)
{
	m_pos = pos;

	m_size = size;
	m_alpha = GetRand(25) + 225;

	if (isDash)
	{
		m_pos.x += GetRand(12) - 6;
		m_pos.y += GetRand(12) - 6;

		if (vec.x >= 0)
		{
			vec.x += GetRand(4) - 2;
		}
		else
		{
			vec.x -= GetRand(4) - 2;
		}

		if (vec.y >= 0)
		{
			vec.y += GetRand(4) - 2;
		}
		else
		{
			vec.y -= GetRand(4) - 2;
		}

		m_growSpeed = GetRand(6) + 2;
	}
	else
	{
		m_pos.x += GetRand(28) - 14;
		m_pos.y += GetRand(28) - 14;
		m_growSpeed = 3;
	}

	//ベクトルを受け取る
	m_vec = -vec.getNormalize();

	m_isExsit = true;
}

void AfterImagePlayer::Update()
{
	if (!m_isExsit)
		return;
	m_alpha -= 8;

	m_pos += m_vec * static_cast<float>(m_growSpeed);

	if (m_alpha <= 0)
	{
		m_isExsit = false;
	}

}

void AfterImagePlayer::Draw()
{
	if (!m_isExsit)
		return;

	//本体の描画

	//フェードアウトして消えていくように
	//alpha = 255 * static_cast<float>(m_fadeFrame) / (float)kFadeFrame;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alpha);
	DrawBox(static_cast<int>(m_pos.x) - m_size, static_cast<int>(m_pos.y) - m_size, static_cast<int>(m_pos.x) + m_size, static_cast<int>(m_pos.y) + m_size, 0x1e90ff, true);
	//DrawBox(static_cast<int>(m_pos.x) - m_size, static_cast<int>(m_pos.y) - m_size, static_cast<int>(m_pos.x) + m_size, static_cast<int>(m_pos.y) + m_size, 0x0d1118, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
