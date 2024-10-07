#pragma once
#include "CharacterBase.h"
#include  <string>

class WeaponBase;
class HitBox;
class SearchObject;

namespace
{
	constexpr float kDistance = 2.6f;
}

class EnemyBase : public CharacterBase
{
protected:
	enum WeaponKind
	{
		Blade
	};
public:
	EnemyBase(Priority priority);
	~EnemyBase();

	virtual void Init(std::shared_ptr<MyLib::Physics>physics, std::vector<MyLib::Vec3> route) {};
	virtual void Finalize(std::shared_ptr<MyLib::Physics> physics);
	virtual void Update(MyLib::Vec3 playerPos, bool isChase) {};
	virtual void Draw() {};

	// 衝突したとき
	void OnCollideEnter(const std::shared_ptr<Collidable>& colider)override;
	void OnCollideStay(const std::shared_ptr<Collidable>& colider)override;

	virtual  MyLib::Vec3 GetCenterPos()const { return m_centerPos; };
	virtual  MyLib::Vec3 GetPos()const { return rigidbody.GetPos(); };
	bool GetIsHit();

	int GetDropPoint();
	bool GetIsDropedPoint()const { return m_isDroped; };
	bool GetIsDead()const { return m_isDead; };

	const bool GetIsExist()const { return m_isExist; }
	const bool GetIsReach()const { return m_isReach; }

	const GameObjectTag GetLastHitObjectTag()const { return m_lastHitObjectTag; }
	
	//レイキャストをするためにモデルハンドルを取得
	const int GetModelHandle()const { return CharacterBase::m_modelHandle; }
	const int GetHp()const { return CharacterBase::m_status.hp; }

protected:
	//ほかのオブジェクトと押し出し判定をする当たり判定を作成
	void InitCollision(float radius);
	//モデルを読み込む
	void LoadModel(std::string path);
	//アニメーション情報や、ステータス情報を読み込む
	void LoadData(std::string name);
	//武器を生成する
	void CreateWeapon(WeaponKind kind, float modelSize);
	//ルートをモデルの高さに合うように調整する
	void AdjustmentRoute(float modelOffesetY,float modelSize);
	//物理クラスの初期化
	void InitRigidbody(bool isUseGravity = true);
	//モデルの中心座標を計算
	void CalculationCenterPos(float modeldefaultSize,float modelSize);
	//モデル座標を設定
	void SetModelPos(float offset);
	//ダメージ判定をする当たり判定を作成
	void InitHitBox(float radius);
	//索敵判定をする当たり判定を作成
	void InitSearch(float radius);
	//ダメージを受けたとき
	void OnDamage();
	//死亡した時
	void Death();
	//アニメーションブレンドの更新
	void UpdateAnimationBlend();

	//モデルの座標設定
	void SetDrawModelPos(float offset);
protected:
	//武器ポインタ
	std::shared_ptr<WeaponBase> m_pWeapon;
	std::shared_ptr<HitBox> m_pHitbox;
	std::shared_ptr<SearchObject> m_pSearch;

	bool m_isExist;	//存在するかどうか
	bool m_isReach;	//最終目的地に着いたかどうか

	bool m_isHit;

	bool m_isDead;		//死亡したかどうか
	int m_dropPoint;	//死亡時にドロップするポイント
	bool m_isDroped;	//ポイントをドロップしたかどうか

	bool m_isAttack;	
	bool m_isKnock;

	float m_searchRange;

	//移動ルート
	std::vector<MyLib::Vec3> m_route;
	//次の目的座標
	MyLib::Vec3 m_destinationPos;

	MyLib::Vec3 m_centerPos;

	GameObjectTag m_lastHitObjectTag;

	//////////////////////////////////////
	int m_attackWaitFrame;

	int m_knockCount;

	int m_routeNum;

	//プレイヤーを追うかどうか
	bool m_isChase;
	//現在プレイヤーを追っているかどうか
	bool m_isChasing;

	//状態遷移のためのメンバ関数
	using UpdateFunc_t = void (EnemyBase::*)(MyLib::Vec3 playerPos, bool isChase);
	UpdateFunc_t m_updateFunc;

	//攻撃
	void AttackUpdate(MyLib::Vec3 playerPos, bool isChase);
	//歩き
	void WalkUpdate(MyLib::Vec3 playerPos, bool isChase);
	//攻撃ヒット
	void HitUpdate(MyLib::Vec3 playerPos, bool isChase);

	void DeathUpdate(MyLib::Vec3 playerPos, bool isChase);
};

