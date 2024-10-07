#pragma once
#include "DxLib.h"
#include "MyLib.h"
#include "EnemyBase.h"
#include <map>

class EnemyFast : public EnemyBase
{
public:
	EnemyFast();
	virtual ~EnemyFast();

	void Init(std::shared_ptr<MyLib::Physics>physics, std::vector<MyLib::Vec3> route) override;
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;
	void Update(MyLib::Vec3 playerPos, bool isChase);
	void Draw();

private:
	void SetModelPos();
private:
	int m_attackWaitFrame;

	int m_knockCount;

	std::vector<MyLib::Vec3> m_route;
	int m_routeNum;
	MyLib::Vec3 m_destinationPos;

	//プレイヤーを追うかどうか
	bool m_isChase;
	//現在プレイヤーを追っているかどうか
	bool m_isChasing;

	//状態遷移のためのメンバ関数
	using UpdateFunc_t = void (EnemyFast::*)(MyLib::Vec3 playerPos, bool isChase);
	UpdateFunc_t m_updateFunc;

	//攻撃
	void AttackUpdate(MyLib::Vec3 playerPos, bool isChase);
	//歩き
	void WalkUpdate(MyLib::Vec3 playerPos, bool isChase);
	//攻撃ヒット
	void HitUpdate(MyLib::Vec3 playerPos, bool isChase);

	void DeathUpdate(MyLib::Vec3 playerPos, bool isChase);
};

