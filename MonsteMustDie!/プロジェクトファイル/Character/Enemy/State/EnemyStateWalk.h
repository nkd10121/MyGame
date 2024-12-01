﻿#pragma once
#include "StateBase.h"

/// <summary>
/// 敵の歩き状態
/// </summary>
class EnemyStateWalk : public StateBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="own">持ち主のポインタ</param>
	EnemyStateWalk(std::shared_ptr<CharacterBase> own);
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

private:
};

