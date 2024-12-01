﻿#pragma once
#include "ObjectBase.h"
class Crystal : public ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="hp">体力</param>
	Crystal(int hp);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Crystal();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="physics">物理クラスポインタ</param>
	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 生成座標を設定
	/// </summary>
	/// <param name="pos">生成座標</param>
	void Set(const Vec3& pos);

	/// <summary>
	/// ほかのオブジェクトと衝突したときに呼ばれる
	/// </summary>
	/// <param name="colider">当たったオブジェクト</param>
	virtual void OnTriggerEnter(const std::shared_ptr<Collide>& ownCol, const std::shared_ptr<Collidable>& send, const std::shared_ptr<Collide>& sendCol)override;

	/// <summary>
	/// 破壊されたかどうかを取得する
	/// </summary>
	/// <returns>破壊フラグ</returns>
	const bool GetIsBreak()const { return m_isBreak; }
	/// <summary>
	/// クリスタルの現在HPを取得する
	/// </summary>
	/// <returns>クリスタルの現在HP</returns>
	const int GetHp()const { return m_hp; }

private:
	Vec3 m_pos;	//座標

	int m_hp;	//体力

	bool m_isBreak;		//破壊されたかどうか
	
	int m_crystalStandHandle;	//クリスタルスタンドのモデルハンドル

	float m_angle;	//クリスタルを動かすための角度
};

