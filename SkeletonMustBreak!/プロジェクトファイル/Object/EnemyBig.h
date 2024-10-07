#pragma once
#include "EnemyBase.h"

class WeaponBase;

class EnemyBig : public EnemyBase
{
public:
	EnemyBig();
	virtual ~EnemyBig();

	void Init(std::shared_ptr<MyLib::Physics>physics, std::vector<MyLib::Vec3> route) override;
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;
	void Update(MyLib::Vec3 playerPos, bool isChase);
	void Draw();
};

