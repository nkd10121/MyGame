#include "EnemyCircleBase.h"

EnemyCircleBase::EnemyCircleBase() :
	m_pos		(0.0f,0.0f),
	m_startPos	(0.0f,0.0f),
	m_endPos	(0.0f,0.0f),
	m_color		(0xff00ff),
	m_isExist	(false)
{
}

EnemyCircleBase::~EnemyCircleBase()
{
}
