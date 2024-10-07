#include "EnemyFast.h"
#include "CsvLoad.h"
#include "DxLib.h"

#include "HitBox.h"
#include "SearchObject.h"

#include "SoundManager.h"
#include "ModelManager.h"

namespace
{
	constexpr float kModelDefaultSize = 2.166f;

	//モデルサイズ
	constexpr float kModelSizeScale = 3.2f;
	constexpr float kModelOffsetY = 0.3f;

	constexpr float kWeaponModelSize = 0.01f;

	//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeFrame = 10.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//アニメーションブレンド率の最大
	constexpr float kAnimBlendRateMax = 1.0f;

	constexpr float kSearchingRadius = 30.0f;
}


EnemyFast::EnemyFast():
	EnemyBase(Collidable::Priority::Middle),
	m_attackWaitFrame(0),
	m_knockCount(0),
	m_isChase(false),
	m_isChasing(false)
{
	//当たり判定の設定
	auto collider = Collidable::AddCollider(MyLib::ColliderData::Kind::Sphere, false);
	auto sphereCol = dynamic_cast<MyLib::ColliderDataSphere*>(collider.get());
	sphereCol->m_radius = 1.4f;

	CsvLoad::GetInstance().AnimDataLoad("FastSkelton", m_animIdx);
	CsvLoad::GetInstance().StatusLoad(m_status, "FastSkelton");
	m_maxHp = m_status.hp;

	//モデルの読み込み
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("data/model/Skeleton_Minion.mv1");
	MV1SetupCollInfo(m_modelHandle);

	m_dropPoint = m_status.point;
}


EnemyFast::~EnemyFast()
{
	//メモリの解放
	MV1DeleteModel(m_modelHandle);
}

void EnemyFast::Init(std::shared_ptr<MyLib::Physics> physics, std::vector<MyLib::Vec3> route)
{
	m_pPhysics = physics;

	m_route = route;

	for (int i = 0; i < m_route.size(); i++)
	{
		m_route[i].y += kModelOffsetY * kModelSizeScale;
	}


	//最初の目的地を設定する
	m_routeNum = 1;
	m_destinationPos = m_route[m_routeNum];

	//存在している状態にする
	m_isExist = true;

	Collidable::Init(m_pPhysics);

	rigidbody.Init(true);
	rigidbody.SetPos(m_route[0]);
	rigidbody.SetNextPos(rigidbody.GetPos());
	m_moveVec = (m_destinationPos - rigidbody.GetPos()).Normalize();

	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConvertToVECTOR());

	//ヒットボックスの作成
	MyLib::Vec3 hitboxPos = rigidbody.GetPos();
	hitboxPos.y += kModelDefaultSize * kModelSizeScale / 2 * 0.7f;
	m_pHitbox = std::make_shared<HitBox>(3.0f);
	m_pHitbox->Init(m_pPhysics, hitboxPos, true);

	//中心座標の設定
	m_centerPos = rigidbody.GetPos();
	m_centerPos.y += kModelDefaultSize * kModelSizeScale * 0.5f;

	//モデルのサイズ設定
	MV1SetScale(m_modelHandle, VGet(kModelSizeScale, kModelSizeScale, kModelSizeScale));
	//m_weponAttachFrameNum = MV1SearchFrame(m_modelHandle, "handslot.r");


	//アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Move"]);
	m_nowAnimIdx = m_animIdx["Move"];


	//通常状態に設定しておく
	m_updateFunc = &EnemyFast::WalkUpdate;
}

void EnemyFast::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Finalize(physics);

	m_pHitbox->Finalize(m_pPhysics);
}

void EnemyFast::Update(MyLib::Vec3 playerPos, bool isChase)
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	//アニメーションの更新
	m_isAnimationFinish = UpdateAnim(m_currentAnimNo);

	MyLib::Vec3 centerPos = rigidbody.GetPos();
	centerPos.y += kModelDefaultSize / 2 * kModelSizeScale * 0.8f;
	m_pHitbox->Update(centerPos);

	//状態の更新
	(this->*m_updateFunc)(playerPos, isChase);

	if (m_pHitbox->GetIsHit() != 0)
	{
		//HPを減らす
		m_status.hp -= m_pHitbox->GetIsAttackNum() - m_status.def;

		m_lastHitObjectTag = m_pHitbox->GetHitObjectTag();

		//EnemyManagerがエフェクトを生成するためのフラグ
		m_isHit = true;

		SoundManager::GetInstance().PlaySE("EnemyHit");

		if (!m_isKnock && !m_isAttack)
		{
			m_isKnock = true;
			m_updateFunc = &EnemyFast::HitUpdate;
			m_nowAnimIdx = m_animIdx["Hit"];

			ChangeAnim(m_nowAnimIdx);
		}
	}

	//HPが以下になったら死亡する
	if (m_status.hp <= 0)
	{
		if (!m_isDead)
		{
			m_isDead = true;
			Finalize(m_pPhysics);

			m_updateFunc = &EnemyFast::DeathUpdate;

			auto deathAnimIdx = GetRand(2);
			auto deathAnimSpeed = 0.66f;

			switch (deathAnimIdx)
			{
			case 0:
				m_nowAnimIdx = m_animIdx["Death0"];
				deathAnimSpeed = 0.33f;
				break;
			case 1:
				m_nowAnimIdx = m_animIdx["Death1"];
				break;
			case 2:
				m_nowAnimIdx = m_animIdx["Death2"];
				break;
			}



			ChangeAnim(m_nowAnimIdx, deathAnimSpeed);
		}
	}

	//中心座標の設定
	m_centerPos = rigidbody.GetPos();
	m_centerPos.y += kModelDefaultSize * kModelSizeScale * 0.5f;

	//アニメーションの切り替え
	if (m_prevAnimNo != -1)
	{
		//フレームでアニメーションを切り替える
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= kAnimBlendRateMax)
		{
			m_animBlendRate = kAnimBlendRateMax;
		}

		//アニメーションのブレンド率を設定する
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, kAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_currentAnimNo, m_animBlendRate);
	}
}

void EnemyFast::Draw()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConvertToVECTOR());
	//モデルの描画
	MV1DrawModel(m_modelHandle);
	//MV1DrawModel(m_weponHandle);
}

void EnemyFast::SetModelPos()
{
	m_modelPos = m_collisionPos;
	m_modelPos.y -= kModelOffsetY * kModelSizeScale;
}

void EnemyFast::AttackUpdate(MyLib::Vec3 playerPos, bool isChase)
{
	rigidbody.SetVelocity(MyLib::Vec3());
	////アニメーションの更新
	//m_isAnimationFinish = UpdateAnim(m_currentAnimNo);

	if (m_attackWaitFrame >= 20)
	{
		if (m_attackWaitFrame == 20)
		{
			m_nowAnimIdx = m_animIdx["Attack"];
			ChangeAnim(m_nowAnimIdx, 0.3f);
		}


		//アニメーションが終了したら歩き状態に戻す
		if (m_isAnimationFinish)
		{
			m_attackWaitFrame = 0;

			m_isAttack = false;

			m_updateFunc = &EnemyFast::WalkUpdate;
			ChangeAnim(m_animIdx["Move"]);
		}

	}

	m_attackWaitFrame++;
}

void EnemyFast::WalkUpdate(MyLib::Vec3 playerPos, bool isChase)
{
	////攻撃フラグがtrueになっていたら攻撃をする
	//if (m_isAttack)
	//{
	//	m_moveVec = (playerPos - m_collisionPos).Normalize();

	//	m_updateFunc = &EnemyFast::AttackUpdate;

	//	m_nowAnimIdx = m_animIdx["Idle"];
	//	ChangeAnim(m_nowAnimIdx);
	//	return;
	//}

	if (m_isKnock)
	{
		m_knockCount++;

		if (m_knockCount > 90)
		{
			m_isKnock = false;
			m_knockCount = 0;
		}

	}

	auto temp = (rigidbody.GetPos() - m_route[m_routeNum]).Size();
	if (temp < kDistance)
	{
		m_routeNum++;
		if (m_routeNum == m_route.size())
		{
			Finalize(m_pPhysics);
			m_isExist = false;
			m_isReach = true;
		}
		else
		{
			m_destinationPos = m_route[m_routeNum];
			//m_moveVec = (m_destinationPos - rigidbody.GetPos()).Normalize();
		}
	}

	//バグを治すためにいったんここで様子見
	m_moveVec = (m_destinationPos - rigidbody.GetPos()).Normalize();

	//プレイヤーを追跡する機能持ちなら
	if (m_isChase && isChase)
	{
		//直前まで追っていたかどうか保存
		bool isChesed = m_isChasing;

		//プレイヤーの座標と現在位置から距離を計算する
		auto pPos = playerPos;
		pPos.y = m_collisionPos.y;
		auto len = pPos - m_collisionPos;
		auto dis = len.Size();

		//一定範囲より近ければ追跡する
		if (dis < kSearchingRadius)
		{
			m_isChasing = true;
			m_moveVec = len.Normalize();
		}
		//
		else
		{
			m_isChasing = false;
			//追跡を終えたとき
			if (isChesed && !m_isChasing)
			{
				//ルートを見て、一番近い座標に向かう
				float distanceMin = 1000.0f;
				int retRouteNum = 0;

				for (int i = 0; i < m_route.size(); i++)
				{
					//現在座標からの距離を見る
					auto dis = (m_route[i] - rigidbody.GetPos()).Size();

					if (distanceMin > dis)
					{
						distanceMin = dis;
						retRouteNum = i;
					}
				}

				if (retRouteNum != m_routeNum - 1)
				{
					m_routeNum = retRouteNum;

				}

				//
				m_destinationPos = m_route[m_routeNum];
				m_moveVec = (m_destinationPos - rigidbody.GetPos()).Normalize();
			}
		}
	}


	//移動速度をここで設定できるっぽい？
	rigidbody.SetVelocity(m_moveVec, m_status.speed);

	//atan2を使用して向いている角度を取得
	auto angle = atan2(m_moveVec.x, m_moveVec.z);
	auto rotation = VGet(0.0f, angle + DX_PI_F, 0.0f);
	//移動方向に体を回転させる
	MV1SetRotationXYZ(m_modelHandle, rotation);
}

void EnemyFast::HitUpdate(MyLib::Vec3 playerPos, bool isChase)
{
	rigidbody.SetVelocity(MyLib::Vec3());
	//アニメーションの更新
	m_isAnimationFinish = UpdateAnim(m_currentAnimNo);

	//アニメーションが終了したら歩き状態に戻す
	if (m_isAnimationFinish)
	{

		m_updateFunc = &EnemyFast::WalkUpdate;
		ChangeAnim(m_animIdx["Move"]);
	}
}

void EnemyFast::DeathUpdate(MyLib::Vec3 playerPos, bool isChase)
{
	if (m_isAnimationFinish)
	{
		m_isExist = false;
	}
}
