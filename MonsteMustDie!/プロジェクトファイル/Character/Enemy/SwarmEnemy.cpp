﻿#include "SwarmEnemy.h"

namespace
{
	//索敵半径の差分倍率
	constexpr float kCollisionRadiusMag = 0.3f;
}

/// <summary>
/// コンストラクタ
/// </summary>
SwarmEnemy::SwarmEnemy(unsigned int color) :
	Collidable(Priority::Low, GameObjectTag::SwarmEnemy),
	m_swarm(),
	m_isExistMember(false),
	m_swarmCenterPos(),
	m_swarmRadius(0.0f),
	m_maxSearchCollisionRadius(0.0f),
	m_isInPlayer(false),
	m_memberColor(color)
{
	//物理データの初期化
	rigidbody->Init();

	//当たり判定を無視するオブジェクトタグを設定
	//TODO:このオブジェクトはプレイヤーとプレイヤーが撃った矢以外のすべてを無視するようにする
	AddThroughTag(GameObjectTag::Enemy);
	AddThroughTag(GameObjectTag::SwarmEnemy);
	AddThroughTag(GameObjectTag::Crystal);
	AddThroughTag(GameObjectTag::Portion);
	AddThroughTag(GameObjectTag::Trap);
}

/// <summary>
/// デストラクタ
/// </summary>
SwarmEnemy::~SwarmEnemy()
{

}

/// <summary>
/// 終了
/// </summary>
void SwarmEnemy::Finalize()
{
	for (auto& enemy : m_swarm)
	{
		enemy->Finalize();
	}

	m_swarm.clear();
}

/// <summary>
/// 更新
/// </summary>
void SwarmEnemy::Update()
{
	std::list<Vec3> pos;
	//構成メンバーの更新
	for (auto& enemy : m_swarm)
	{
		enemy->Update();
		pos.emplace_back(enemy->GetRigidbody()->GetPos());
	}

	//isExistがfalseのオブジェクトを削除
	{
		auto it = std::remove_if(m_swarm.begin(), m_swarm.end(), [](auto& v)
			{
				return v->GetIsExist() == false;
			});
		m_swarm.erase(it, m_swarm.end());
	}

	//もし、構成メンバーが1人もいなかったらメンバーが存在しないことにする
	if (m_swarm.size() == 0)
	{
		m_isExistMember = false;
		Collidable::OnExistPhysics();
		return;
	}

	Vec3 addPos;
	for (auto& p : pos)
	{
		addPos += p;
	}
	//中心座標の決定
	m_swarmCenterPos = addPos / static_cast<float>(m_swarm.size());
	rigidbody->SetPos(m_swarmCenterPos);

	float maxLength = 0.0f;

	for (auto& p : pos)
	{
		auto length = (p - m_swarmCenterPos).Length();

		if (maxLength < length)
		{
			maxLength = length;
		}
	}
	if (m_swarmRadius != maxLength)
	{
		m_swarmRadius = maxLength + m_maxSearchCollisionRadius + m_maxSearchCollisionRadius * 0.2f;
		auto cupsuleCol = dynamic_cast<MyLib::ColliderSphere*>(Collidable::m_colliders.back()->collide.get());			//キャスト
		cupsuleCol->SetRadius(m_swarmRadius);
	}
}

/// <summary>
/// 描画
/// </summary>
void SwarmEnemy::Draw()
{
	//構成メンバーの描画
	for (auto& enemy : m_swarm)
	{
		enemy->Draw();

#ifdef _DEBUG
		auto pos = enemy->GetRigidbody()->GetPosVECTOR();
		pos.y += 6.0f;
		DrawSphere3D(pos, 1, 8, m_memberColor, m_memberColor, true);

		DrawSphere3D(m_swarmCenterPos.ToVECTOR(), 4, 12, m_memberColor, m_memberColor, true);
#endif
	}
}

/// <summary>
/// 群れメンバーのモデル座標を更新
/// </summary>
void SwarmEnemy::UpdateModelPos()
{
	//構成メンバーの描画
	for (auto& enemy : m_swarm)
	{
		enemy->UpdateModelPos();
	}
}

/// <summary>
/// 群れメンバーを追加しきった後に呼ぶ関数
/// </summary>
void SwarmEnemy::SetUp()
{
	//群れ全体の中心座標と半径を求めて当たり判定を生成する
	Vec3 addPos;
	for (auto& enemy : m_swarm)
	{
		addPos += enemy->GetRigidbody()->GetPos();

		if (m_maxSearchCollisionRadius < enemy->GetSearchCollisionRadius())
		{
			m_maxSearchCollisionRadius = enemy->GetSearchCollisionRadius();
		}
	}
	//中心座標の決定
	m_swarmCenterPos = addPos / static_cast<float>(m_swarm.size());

	float maxLength = 0.0f;

	for (auto& enemy : m_swarm)
	{
		auto length = (enemy->GetRigidbody()->GetPos() - m_swarmCenterPos).Length();

		if (maxLength < length)
		{
			maxLength = length;
		}
	}
	m_swarmRadius = maxLength + m_maxSearchCollisionRadius + m_maxSearchCollisionRadius * 0.2f;

	{
		//当たり判定の作成
		auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Sphere, true, MyLib::ColliderBase::CollisionTag::Search);	//追加
		auto cupsuleCol = dynamic_cast<MyLib::ColliderSphere*>(collider.get());			//キャスト
		cupsuleCol->m_radius = m_swarmRadius;		//カプセルの半径

		Collidable::OnEntryPhysics();

		rigidbody->SetPos(m_swarmCenterPos);
		rigidbody->SetNextPos(rigidbody->GetPos());
	}
}

/// <summary>
/// 構成メンバーを追加
/// </summary>
void SwarmEnemy::AddSwarm(std::shared_ptr<EnemyBase> add)
{
	//構成メンバーが存在しないことになっていたら存在するようにする
	if (!m_isExistMember)m_isExistMember = true;

	m_swarm.emplace_back(add);
}

/// <summary>
/// メンバーの人数を取得
/// </summary>
const int SwarmEnemy::CheckMemberNum() const
{
	return m_swarm.size();
}

/// <summary>
/// 押し出し処理を行わないオブジェクトと衝突したとき
/// </summary>
void SwarmEnemy::OnTriggerEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	auto tag = send->GetTag();
	if (tag == GameObjectTag::Player)
	{
		if (!m_isInPlayer)
		{
			//プレイヤーがいる判定にする
			m_isInPlayer = true;

			for (auto& enemy : m_swarm)
			{
				enemy->CreateSearchCollision();
			}
		}
	}
}

/// <summary>
/// 押し出し処理を行わないオブジェクトと衝突しなくなった時
/// </summary>
void SwarmEnemy::OnTriggerExit(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	auto tag = send->GetTag();
	if (tag == GameObjectTag::Player)
	{
		//プレイヤーがいる判定にする
		m_isInPlayer = false;

		for (auto& enemy : m_swarm)
		{
			enemy->DeleteSearchCollision();
		}
	}
}

const std::list<int> SwarmEnemy::GetModelHandles() const
{
	std::list<int> ret;

	for (auto& enemy : m_swarm)
	{
		ret.push_back(enemy->GetModelHandle());
	}

	return ret;
}
