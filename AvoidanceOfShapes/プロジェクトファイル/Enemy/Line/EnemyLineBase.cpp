#include "EnemyLineBase.h"
#include "DxLib.h"
#include "Game.h"

EnemyLineBase::EnemyLineBase() :
	m_startPos			(0.0f, 0.0f),
	m_endPos			(0.0f, 0.0f),
	m_frame				(0),
	m_waitFrame			(0),
	m_disappearFrame	(0),
	m_color				(0xff00ff),
	m_shakeFrame		(0),
	m_isShake			(false),
	m_isExist			(false)
{
}

EnemyLineBase::~EnemyLineBase()
{
}
