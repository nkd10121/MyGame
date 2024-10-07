#pragma once
#include "DxLib.h"
#include "MyLib.h"
#include "EnemyBase.h"
#include <map>


class Enemy : public EnemyBase
{
public:
	Enemy();
	virtual ~Enemy();

	void Init(std::shared_ptr<MyLib::Physics>physics, std::vector<MyLib::Vec3> route) override;
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;
	void Update(MyLib::Vec3 playerPos, bool isChase);
	void Draw();
};