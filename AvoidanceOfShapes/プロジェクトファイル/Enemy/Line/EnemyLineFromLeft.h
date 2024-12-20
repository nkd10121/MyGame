#pragma once
#include "EnemyLineBase.h"

/// <summary>
/// 左から出てくるライン系の敵
/// </summary>
class EnemyLineFromLeft : public EnemyLineBase
{
public:
	EnemyLineFromLeft();
	~EnemyLineFromLeft();

	virtual void Init(Vec2 pos, int width, int firstFrame, int secondFrame) override;
	virtual void Update()override;
	virtual void Draw()override;
};

