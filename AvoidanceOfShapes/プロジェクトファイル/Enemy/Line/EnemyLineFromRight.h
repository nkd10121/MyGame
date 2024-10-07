#pragma once
#include "EnemyLineBase.h"

/// <summary>
/// ‰E‚©‚ço‚Ä‚­‚éƒ‰ƒCƒ“Œn‚Ì“G
/// </summary>
class EnemyLineFromRight :	public EnemyLineBase
{
public:
	EnemyLineFromRight();
	~EnemyLineFromRight();

	virtual void Init(Vec2 pos, int width, int firstFrame, int secondFrame) override;
	virtual void Update()override;
	virtual void Draw()override;
};