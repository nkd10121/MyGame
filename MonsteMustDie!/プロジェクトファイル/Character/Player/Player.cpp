﻿#include "Player.h"
#include "Input.h"
#include "MyLib.h"

#include "PlayerStateIdle.h"
#include "PlayerStateDeath.h"

#include "HealPortion.h"
#include "EnemyBase.h"
#include "Shot.h"

#include "LoadCSV.h"
#include "ModelManager.h"
#include "TrapManager.h"

#include "GameManager.h"
namespace
{
	/*デバッグ用のカプセル関係*/
	constexpr float kCollisionCapsuleSize = 4.0f;		//カプセルのサイズ
	constexpr float kCollisionCapsuleRadius = 2.0f;		//カプセルの半径
	constexpr int kCollisionCapsuleDivNum = 10;			//カプセルの分け数(?)	//TODO:いらんかったら消す

	/*アナログスティックによる移動関連*/
	constexpr float kMaxSpeed = 0.2f;			//プレイヤーの最大速度
	constexpr float kAnalogRangeMin = 0.1f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	/*モデル関係*/
	//キャラクターモデル
	constexpr float kModelScale = 0.065f;		//モデルのサイズ
	//constexpr float kModelScale = 0.05f;		//モデルのサイズ

	//武器モデル
	constexpr float kCrossbowModelScale = 0.64f;	//モデルのサイズ
	const char* kAttachFrameName = "mixamorig:RightHandThumb1";

	/*アニメーション関係*/
	constexpr float kAnimChangeFrame = 10.0f;							//アニメーションの切り替えにかかるフレーム数
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;		//1フレーム当たりのアニメーション切り替えが進む速さ
	constexpr float kAnimBlendRateMax = 1.0f;							//アニメーションブレンド率の最大


}

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player() :
	CharacterBase(Collidable::Priority::Middle, GameObjectTag::Player),
	m_pos(Vec3(0.0f, kCollisionCapsuleRadius + kCollisionCapsuleSize *8, 0.0f)),
	temp_moveVec(),
	m_cameraDirection(),
	m_rot(),
	m_crossbowPos(),
	m_cameraAngle(0.0f),
	m_angle(0.0f),
	m_attackButtonPushCount(0),
	m_isStartDeathAnimation(false),
	m_isDeath(false),
	m_slotNum(0)
{
	//当たり判定の生成
	auto collider = Collidable::AddCollider(MyLib::ColliderBase::Kind::Cupsule, false);
	auto sphereCol = dynamic_cast<MyLib::ColliderCupsule*>(collider.get());
	sphereCol->m_radius = kCollisionCapsuleRadius;
	sphereCol->m_size = kCollisionCapsuleSize;

	m_collisionRadius = kCollisionCapsuleRadius;
	m_collisionSize = kCollisionCapsuleSize;

	//キャラクター名を設定
	m_characterName = "Player";
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
	//基底クラスで呼んでいるが念のため
	MV1DeleteModel(m_modelHandle);
	MV1DeleteModel(m_crossbowHandle);
}

/// <summary>
/// 初期化
/// </summary>
void Player::Init()
{
	//当たり判定の初期化
	OnEntryPhysics();

	//ステートパターンの初期化
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()));
	m_pState->SetNextKind(StateBase::StateKind::Idle);
	m_pState->Init();

	//プレイヤーの初期位置設定
	rigidbody->Init(true);
	rigidbody->SetPos(m_pos);
	rigidbody->SetNextPos(rigidbody->GetPos());

	//プレイヤーモデルを取得
	m_modelHandle = ModelManager::GetInstance().GetModelHandle("M_PLAYER");
	//スケールの変更
	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));

	//待機アニメーションを設定
	m_currentAnimNo = MV1AttachAnim(m_modelHandle, LoadCSV::GetInstance().GetAnimIdx(m_characterName, "IDLE"));
	m_preAnimIdx = 0;
	m_nowAnimIdx = 0;

	//プレイヤーのステータス取得
	m_status = LoadCSV::GetInstance().LoadStatus(m_characterName.c_str());
	//最大HPを設定しておく
	m_hpMax = m_status.hp;

	//クロスボウのモデルハンドルを取得する
	m_crossbowHandle = ModelManager::GetInstance().GetModelHandle("M_CROSSBOW");
	//スケールの変更
	MV1SetScale(m_crossbowHandle, VGet(kCrossbowModelScale, kCrossbowModelScale, kCrossbowModelScale));
}

/// <summary>
/// 終了処理
/// </summary>
void Player::Finalize()
{
	Collidable::OnExistPhysics();
}

/// <summary>
/// 更新
/// </summary>
void Player::Update(GameManager* pGameManager,Vec3 cameraRayCastRet)
{
	//ステートの更新
	m_pState->Update();

	//アニメーションの更新
	m_isAnimationFinish = UpdateAnim(m_currentAnimNo);
	//アニメーションブレンド
	AnimationBlend();

	//死んでいなかったらカメラが向いている方向を正面とする
	if (!m_isStartDeathAnimation)
	{
		//カメラの座標からプレイヤーを回転させる方向を計算する
		m_angle = -atan2f(m_cameraDirection.z, m_cameraDirection.x) - DX_PI_F / 2;
		m_rot = Vec3(0.0f, m_angle, 0.0f);

		//プレイヤーを回転させる
		MV1SetRotationXYZ(m_modelHandle, m_rot.ToVECTOR());
	}


	//足元座標を計算
	auto bottomPos = m_pos;
	bottomPos.y -= kCollisionCapsuleSize + kCollisionCapsuleRadius;
	if (m_cameraDirection.y < 0.0f)
	{
		bottomPos.y += 1.0f;
	}
	else if (m_cameraDirection.y > 0.0f)
	{
		bottomPos.y -= 1.0f;
	}

	//足元座標を使ってトラップ設置位置の選択と設置
	TrapManager::GetInstance().SelectPoint(bottomPos, m_cameraDirection);

	//ZLボタンを押している
	if (Input::GetInstance().GetIsPushedTriggerButton(true))
	{
		if (m_slotNum == 0)
		{
			//一定間隔で向いている方向に弾を撃つ
			if (m_attackButtonPushCount % 20 == 0)
			{
				std::shared_ptr<Shot> shot = std::make_shared<Shot>(GameObjectTag::PlayerShot);
				shot->Init();
				shot->Set(m_crossbowPos, (cameraRayCastRet - m_crossbowPos).Normalize(), m_status.atk);

				//弾の管理をゲームシーンに任せる
				pGameManager->AddObject(shot);
			}
		}
		else if(m_slotNum == 1 && m_attackButtonPushCount == 0)
		{
			TrapManager::GetInstance().EstablishTrap(bottomPos, m_cameraDirection, m_slotNum);
		}


		//押しているカウントを更新
		m_attackButtonPushCount++;
	}
	else
	{
		//押していないときは0にする
		m_attackButtonPushCount = 0;
	}

	//スロットの選択
	if (Input::GetInstance().IsTriggered("RB"))
	{
		m_slotNum++;
		if (m_slotNum > 3)
		{
			m_slotNum = 3;
		}
	}
	if (Input::GetInstance().IsTriggered("LB"))
	{
		m_slotNum--;
		if (m_slotNum < 0)
		{
			m_slotNum = 0;
		}
	}



	//DEBUG:自決用
	if (Input::GetInstance().IsTriggered("X"))
	{
		m_status.hp -= 10;
	}

#ifdef _DEBUG
	printf("プレイヤーHP:%d\n", m_status.hp);
#endif
	//HPが0以下かつ死亡アニメーションを開始していなかったら
	if (m_status.hp <= 0 && !m_isStartDeathAnimation)
	{
		//当たり判定を削除して
		Collidable::OnExistPhysics();

		//死亡アニメーションを開始する
		m_isStartDeathAnimation = true;

		//現在のステートを強制的に死亡にする
		m_pState = std::make_shared<PlayerStateDeath>(std::dynamic_pointer_cast<Player>(shared_from_this()));
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
			m_isDeath = true;
		}
	}

	//バグなどで地面をすり抜けて落下時の対策
	if (m_pos.y <= -100.0f)
	{
		Collidable::OnExistPhysics();
		m_isDeath = true;
	}
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
	//プレイヤーモデルの描画
	MV1DrawModel(m_modelHandle);

	//クロスボウモデルの描画
	MV1DrawModel(m_crossbowHandle);

#ifdef _DEBUG	//デバッグ描画
	//向いてる方向の線
	auto target = m_pos + m_cameraDirection * 1000.0f;
	DrawLine3D(m_crossbowPos.ToVECTOR(), target.ToVECTOR(), 0x00ff00);

	//入力値の確認
	DrawFormatString(0, 16, 0xff0000, "入力値　: %.3f,%.3f,%.3f", temp_moveVec.x, temp_moveVec.y, temp_moveVec.z);
	auto pos = rigidbody->GetPos();
	DrawFormatString(280, 16, 0xff0000, "座標　: %.3f,%.3f,%.3f", pos.x, pos.y, pos.z);
	DrawFormatString(0, 80, 0xff0000, "%d", MV1GetAttachAnim(m_modelHandle, m_currentAnimNo));
	//ステートパターンの確認
	m_pState->DebugDrawState(0, 32);
#endif
}

void Player::UpdateModelPos()
{
	//キャラクターモデル描画座標の更新
	rigidbody->SetPos(rigidbody->GetNextPos());
	m_pos = rigidbody->GetPos();

	auto drawPos = m_pos;
	drawPos.y -= kCollisionCapsuleRadius + kCollisionCapsuleSize;

	MV1SetPosition(m_modelHandle, drawPos.ToVECTOR());

	//クロスボウモデル描画座標の更新
	auto weaponAttachFrameNum = MV1SearchFrame(m_modelHandle, kAttachFrameName);
	auto weaponFrameMat = MV1GetFrameLocalWorldMatrix(m_modelHandle, weaponAttachFrameNum);

	//結果収納用変数
	Vec3 nowLocalPos;
	Vec3 nowLocalRot;
	Vec3 preLocalPos;
	Vec3 preLocalRot;

	//直前のアニメーションと現在のアニメーションのクロスボウのローカル座標とローカル回転を取得
	LoadCSV::GetInstance().GetCrossbowLocationData(m_nowAnimIdx, nowLocalPos, nowLocalRot);
	LoadCSV::GetInstance().GetCrossbowLocationData(m_preAnimIdx, preLocalPos, preLocalRot);

	//直前のアニメーションと現在のアニメーションとアニメーションブレンド率からローカル回転を計算する
	auto weaponRot = preLocalRot + (nowLocalRot - preLocalRot) * m_animBlendRate;

	//floatからそれぞれの軸回転行列を取得する
	auto xMat = MGetRotX(weaponRot.x);
	auto yMat = MGetRotY(weaponRot.y);
	auto zMat = MGetRotZ(weaponRot.z);

	//ローカル座標を座標行列にする
	auto posM = MGetTranslate(nowLocalPos.ToVECTOR());
	//アタッチフレームの座標にローカル座標を足してクロスボウのワールド座標を計算
	posM = MMult(posM, weaponFrameMat);
	m_crossbowPos = Vec3(posM.m[3][0], posM.m[3][1], posM.m[3][2]);

	//回転行列を計算
	auto rotM = MMult(MMult(xMat, yMat), zMat);
	//回転行列と座標行列を合わせる
	auto setM = MMult(rotM, posM);

	//拡大行列を計算
	auto scaM = MGetScale(VGet(kCrossbowModelScale, kCrossbowModelScale, kCrossbowModelScale));
	//回転行列と座標行列を合わせた行列に拡大行列を掛けて最終的なクロスボウに設定する行列を計算する
	setM = MMult(scaM, setM);

	//クロスボウモデルに行列を設定する
	MV1SetMatrix(m_crossbowHandle, setM);
}

/// <summary>
/// 押し出し処理を行うオブジェクトと衝突したとき
/// </summary>
void Player::OnCollideEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	//#ifdef _DEBUG	//デバッグ描画
	//	std::string message = "プレイヤーが";
	//#endif
	//	auto tag = colider->GetTag();
	//	switch (tag)
	//	{
	//	case GameObjectTag::Enemy:
	//#ifdef _DEBUG	//デバッグ描画
	//		message += "敵";
	//#endif
	//		break;
	//	case GameObjectTag::Portion:
	//#ifdef _DEBUG	//デバッグ描画
	//		message += "ポーション";
	//#endif
	//		break;
	//	}
	//#ifdef _DEBUG	//デバッグ描画
	//	message += "と当たった！\n";
	//	printf(message.c_str());
	//#endif
}

/// <summary>
/// 押し出し処理を行わないオブジェクトと衝突したとき
/// </summary>
void Player::OnTriggerEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)
{
	if (ownCol->collideTag == MyLib::ColliderBase::CollisionTag::Normal)
	{
		if (send->GetTag() == GameObjectTag::Portion)
		{
			//ポーションはそもそも一つのみの当たり判定の予定だから検索しない
			
			//HPが減っているときのみ回復処理を行う
			if (m_hpMax > m_status.hp)
			{
				//HPを満タンまで回復させる
				m_status.hp = m_hpMax;

				//ポーションを削除する
				HealPortion* col = dynamic_cast<HealPortion*>(send.get());
				col->End();

				//回復エフェクトを生成する
				//auto pos = rigidbody->GetPos();
				//EffectManager::GetInstance().CreateEffect("Heal", pos);

				//回復SEを流す
				//SoundManager::GetInstance().PlaySE("heal");
			}
		}
		//敵
		else if (send->GetTag() == GameObjectTag::Enemy)
		{
			//敵の攻撃判定に当たったらダメージを受ける
			if (sendCol->collideTag == MyLib::ColliderBase::CollisionTag::Attack)
			{
				//相手の攻撃力を取得
				EnemyBase* col = dynamic_cast<EnemyBase*>(send.get());
				//自身の防御力と敵の攻撃力からダメージを計算する
				auto atk = col->GetAttackPower() - m_status.def;
				//もし攻撃力が0以下なら何もせずに終える
				if (atk <= 0) return;

				//HPからダメージ分引く
				m_status.hp -= atk;
			}
		}
	}
}
