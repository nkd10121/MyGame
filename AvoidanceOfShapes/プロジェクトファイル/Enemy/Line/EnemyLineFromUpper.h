#pragma once
#include "EnemyLineBase.h"

/// <summary>
/// ã‚©‚ço‚Ä‚­‚éƒ‰ƒCƒ“Œn‚Ì“G
/// </summary>
class EnemyLineFromUpper	: public EnemyLineBase
{
public:
	EnemyLineFromUpper();
	~EnemyLineFromUpper();

	virtual void Init(Vec2 pos, int width, int firstFrame, int secondFrame) override;
	virtual void Update()override;
	virtual void Draw()override;
};

