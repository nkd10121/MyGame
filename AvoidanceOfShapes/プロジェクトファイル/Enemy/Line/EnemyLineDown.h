#pragma once
#include "EnemyLineBase.h"

/// <summary>
/// ‰º‚©‚ço‚Ä‚­‚éƒ‰ƒCƒ“Œn‚Ì“G
/// </summary>
class EnemyLineDown : public EnemyLineBase
{
public:
	EnemyLineDown();
	~EnemyLineDown();

	virtual void Init(Vec2 pos, int width, int firstFrame, int secondFrame) override;
	virtual void Update()override;
	virtual void Draw()override;

};