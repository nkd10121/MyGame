﻿#include "EnemyNormal.h"
#include "EnemyStateIdle.h"
#include "EnemyStateDeath.h"

#include "Player.h"
#include "Shot.h"
#include "TrapBase.h"

#include "SoundManager.h"
#include "EffectManager.h"
#include "LoadCSV.h"
#include "ModelManager.h"

namespace
{
	constexpr float kCollisionCapsuleSize = 3.0f;	//カプセルの大きさ
	constexpr float kCollisionCapsuleRadius = 2.0f;	//カプセルの半径
	constexpr int kCollisionCapsuleDivNum = 10;		//カプセルの分割数
#ifdef _DEBUG	//デバッグ描画
#endif

	/*モデル関係*/
	constexpr float kModelScale = 0.018f;		//モデルサイズ

	/*当たり判定関係*/
	constexpr float kSearchCollisionRadius = kCollisionCapsuleRadius * 10;

	const char* kAttachFrameName = "Head";
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyNormal::EnemyNormal() :
	EnemyBase()
{
	//キャラクター名を設定
	m_characterName = "EnemyNormal";
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyNormal::~EnemyNormal()
{
}

/// <summary>
/// 初期化
/// </summary>
void EnemyNormal::Init()
{
	//当たり判定の初期化
	OnEntryPhysics();

	//ステートパターンの初期化
	m_pState = std::make_shared<EnemyStateIdle>(std::dynamic_pointer_cast<EnemyNormal>(shared_from_this()));
	m_pState->SetNextKind(StateBase::StateKind::Idle);
	m_pState->Init();

	//物理データの初期化
	rigidbody->Init(true);
	rigidbody->SetPos(m_drawPos);
	rigidbody->SetNextPos(rigidbody->GetPos());

	//モデルハンドルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("M_ENEMYNORMAL");
	//モデルのサイズを変更
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetPosition(m_modelHandle, m_drawPos.ToVECTOR());

	// モデル全体のコリジョン情報を構築
	MV1SetupCollInfo(m_modelHandle, -1, 8, 8, 8);

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, LoadCSV::GetInstance().GetAnimIdx(m_characterName, "IDLE"));
	m_preAnimIdx = 0;
	m_nowAnimIdx = 0;

	//プレイヤーのステータス取得
	m_status = LoadCSV::GetInstance().LoadStatus(m_characterName.c_str());
	//最大HPを設定しておく
	m_hpMax = m_status.hp;

	//当たり判定をとらないオブジェクトタグを設定
	AddThroughTag(GameObjectTag::Portion);		//ポーション
	AddThroughTag(GameObjectTag::SwarmEnemy);	//敵群れ

	{
		//通常の当たり判定の作成
		auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, false);	//追加
		auto capsuleCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());			//キャスト
		capsuleCol->m_radius = kCollisionCapsuleRadius;		//カプセルの半径
		capsuleCol->m_size = kCollisionCapsuleSize;			//カプセルの大きさ

	}

	{
		//ヘッドショット判定の作成
		auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Head);	//追加
		auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());			//キャスト
		sphereCol->m_radius = 2.0f;		//カプセルの半径

		auto attachFrameNum = MV1SearchFrame(m_modelHandle, kAttachFrameName);
		auto mat = MV1GetFrameLocalWorldMatrix(m_modelHandle, attachFrameNum);
		auto pos = Vec3(mat.m[3][0], mat.m[3][1], mat.m[3][2]);

		auto modelCenterPos = rigidbody->GetPos();

		auto vec = pos - modelCenterPos;
		vec.y *= 0.55f;

		sphereCol->SetOffsetPos(vec);
	}

	//存在フラグをtrueにする
	m_isExist = true;
}

/// <summary>
/// 終了
/// </summary>
void EnemyNormal::Finalize()
{
	//当たり判定の削除
	Collidable::OnExistPhysics();
	MV1TerminateCollInfo(m_modelHandle, -1);
}

/// <summary>
/// 更新
/// </summary>
void EnemyNormal::Update()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	MV1RefreshCollInfo(m_modelHandle, -1);

	//ステートの更新
	m_pState->Update();

	//アニメーションの更新
	m_isAnimationFinish = UpdateAnim(m_currentAnimNo);
	//アニメーションブレンド
	AnimationBlend();

	//HPが0になったら自身を削除する
	if (m_status.hp <= 0 && !m_isStartDeathAnimation)
	{
		//当たり判定を削除
		Finalize();

		//死亡アニメーションを開始する
		m_isStartDeathAnimation = true;

		//現在のステートを強制的に死亡にする
		m_pState = std::make_shared<EnemyStateDeath>(std::dynamic_pointer_cast<EnemyBase>(shared_from_this()));
		m_pState->SetNextKind(StateBase::StateKind::Death);
		m_pState->Init();
	}

	//死亡アニメーションが始まっていて
	if (m_isStartDeathAnimation)
	{
		//死亡アニメーションが終了したら
		if (GetAnimEnd())
		{
			//完全に死亡したものとする
			m_isExist = false;
		}
	}

#ifdef _DEBUG
	//何の当たり判定を持っているかをデバッグ描画
	for (auto& col : m_colliders)
	{
		switch (col->collideTag)
		{
		case MyLib::ColliderBase::CollisionTag::Normal:
			printf("通常の当たり判定\n");
			break;

		case MyLib::ColliderBase::CollisionTag::Attack:
			printf("攻撃の当たり判定\n");
			break;

		case MyLib::ColliderBase::CollisionTag::Search:
			printf("索敵の当たり判定\n");
			break;

		case MyLib::ColliderBase::CollisionTag::Head:
			printf("頭の当たり判定\n");
			break;
		default:
			break;
		}
	}
#endif

}

/// <summary>
/// 描画
/// </summary>
void EnemyNormal::Draw()
{
	//存在していない状態なら何もさせない
	if (!m_isExist)return;

	//モデルを描画
	MV1DrawModel(m_modelHandle);

#ifdef _DEBUG	//デバッグ描画
	//ステートパターンの確認
	m_pState->DebugDrawState(128, 32);
#endif
}

/// <summary>
/// 索敵判定を生成する
/// </summary>
void EnemyNormal::CreateSearchCollision()
{
	//当たり判定の作成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Search);
	auto sphereCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());
	sphereCol->m_radius = kSearchCollisionRadius;
}

/// <summary>
/// 索敵判定を削除する
/// </summary>
void EnemyNormal::DeleteSearchCollision()
{
	Collidable::DeleteRequestCollider(Collidable::GetCollider(MyLib::ColliderBase::CollisionTag::Search));
}

/// <summary>
/// 3Dモデルの座標更新
/// </summary>
void EnemyNormal::UpdateModelPos()
{
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_drawPos = rigidbody->GetPos();
	m_drawPos.y -= kCollisionCapsuleRadius + kCollisionCapsuleSize;
	MV1SetPosition(m_modelHandle, m_drawPos.ToVECTOR());
}

/// <summary>
/// 索敵判定の半径を取得
/// </summary>
const float EnemyNormal::GetSearchCollisionRadius() const
{
	return  kSearchCollisionRadius;
}

/// <summary>
/// 押し出し処理を行わないオブジェクトと衝突したとき
/// </summary>
void EnemyNormal::OnTriggerEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	//当たったオブジェクトのタグを取得する
	m_hitObjectTag = send->GetTag();

	if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Normal)
	{
		//当たったオブジェクトがプレイヤーが撃った弾なら
		if (m_hitObjectTag == GameObjectTag::PlayerShot)
		{
			//弾の攻撃力分自身のHPを減らす(防御力と調整しながら)
			Shot* col = dynamic_cast<Shot*>(send.get());
			auto damage = col->GetAtk() - m_status.def;
			if (damage > 0)
			{
				m_status.hp -= damage;
			}

			//敵ヒットSEを流す
			SoundManager::GetInstance().PlaySE("S_ENEMYHIT");
			//敵ヒットエフェクトを出す
			EffectManager::GetInstance().CreateEffect("E_ENEMYHIT", rigidbody->GetPos());
			//当たった弾の終了処理を呼ぶ
			col->End();
		}
		//当たったオブジェクトがトラップなら
		else if (m_hitObjectTag == GameObjectTag::Trap)
		{
			//そのトラップの攻撃力分HPを減らす
			TrapBase* col = dynamic_cast<TrapBase*>(send.get());
			auto damage = col->GetAtk() - m_status.def;
			if (damage > 0)
			{
				m_status.hp -= damage;
			}
		}
	}
	else if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Search)
	{
		//当たったオブジェクトがプレイヤーがなら
		if (m_hitObjectTag == GameObjectTag::Player)
		{
			m_isSearchInPlayer = true;
			if (m_pState->GetKind() != StateBase::StateKind::Attack)
			{
				m_pState->SetNextKind(StateBase::StateKind::Walk);
			}
		}
	}
	else if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Head)
	{
		//当たったオブジェクトがプレイヤーが撃った弾なら
		if (m_hitObjectTag == GameObjectTag::PlayerShot)
		{
			{
				//弾の攻撃力分自身のHPを減らす(防御力と調整しながら)
				Shot* col = dynamic_cast<Shot*>(send.get());
				auto damage = col->GetAtk() - m_status.def;
				if (damage > 0)
				{
					m_status.hp -= damage * 2;
				}

				//敵ヒットSEを流す
				SoundManager::GetInstance().PlaySE("S_ENEMYHIT");
				//敵ヒットエフェクトを出す
				EffectManager::GetInstance().CreateEffect("E_ENEMYCRITICALHIT", rigidbody->GetPos());
				//当たった弾の終了処理を呼ぶ
				col->End();
#ifdef _DEBUG	//デバッグ描画

#endif
			}
		}
	}
}

/// <summary>
/// 押し出し処理を行わないオブジェクトと衝突しているとき
/// </summary>
void EnemyNormal::OnTriggerStay(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	//当たったオブジェクトのタグを取得する
	m_hitObjectTag = send->GetTag();

	//当たったオブジェクトがプレイヤーのとき
	if (m_hitObjectTag == GameObjectTag::Player)
	{
		//当たったコリジョンが索敵の時
		if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Search)
		{
			if (m_isSearchInPlayer)
			{
				Player* col = dynamic_cast<Player*>(send.get());
				m_playerPos = col->GetRigidbody()->GetPos();
			}
		}
	}
}

/// <summary>
/// 押し出し処理を行わないオブジェクトと衝突しなくなった時
/// </summary>
void EnemyNormal::OnTriggerExit(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	//当たったオブジェクトのタグを取得する
	m_hitObjectTag = send->GetTag();

	//当たったオブジェクトがプレイヤーのとき
	if (m_hitObjectTag == GameObjectTag::Player)
	{
		//当たったコリジョンが索敵の時
		if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Search)
		{
			m_isSearchInPlayer = false;
			if (m_pState->GetKind() != StateBase::StateKind::Attack)
			{
				m_pState->SetNextKind(StateBase::StateKind::Idle);
			}
		}
	}
}