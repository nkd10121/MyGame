#pragma once
#include "EnemyLineBase.h"

/// <summary>
/// ¶‚©‚ço‚Ä‚­‚éƒ‰ƒCƒ“Œn‚Ì“G
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

