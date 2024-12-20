﻿#pragma once
#include "CharacterBase.h"
#include "TrapBase.h"
#include "EnemyManager.h"

#include <string>
#include <memory>

namespace LoadData
{
	//ステータス情報の並び列挙型
	enum eStatusOrder : int
	{
		name,	//キャラクター名
		hp,		//体力
		atk,	//攻撃力
		def,	//防御力
		speed,	//移動速度
		point	//ドロップする罠ポイント
	};

	//敵生成情報の並び列挙型
	enum eStageOrder : int
	{
		ID,						//敵ID
		phase,					//出現フェーズ
		frame,					//出現フレーム
		AppearanceLocation,		//出現場所
	};

	enum class eTrapStatusOrder
	{
		name,			//トラップ名
		atk,			//攻撃力
		searchRange,	//索敵範囲
		attackRange,	//攻撃範囲
		coolTime,		//クールタイム
		cost			//設置コスト
	};
}

/// <summary>
/// CSVを読み込むクラス
/// </summary>
class CsvLoad
{
private:
	// シングルトンパターンなのでコンストラクタはprivateに置く
	CsvLoad() {};

public:
	virtual ~CsvLoad() {};

	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	CsvLoad(const CsvLoad&) = delete;
	CsvLoad& operator=(const CsvLoad&) = delete;
	CsvLoad(CsvLoad&&) = delete;
	CsvLoad& operator= (const CsvLoad&&) = delete;

	/// <summary>
	/// CsvLoadはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static CsvLoad& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new CsvLoad;
		}

		return *m_instance;
	}

	//これをし忘れると普通にメモリリーク
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	//ステータス情報ロード
	void StatusLoad(CharacterBase::Status& data, const char* characterName);

	//ステージ情報ロード
	void StageEnenyDataLoad(const char* stageName, std::list<std::shared_ptr<EnemyManager::EnemyGenerateInfo>>& pGenerateInfo);

	//アニメーション情報ロード
	void AnimDataLoad(std::string characterName, std::map<std::string, int>& anim);

	//罠ステータス情報ロード
	TrapBase::Status TrapStatusLoad(const char* trapName);
private:
	//staticにすることで
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static CsvLoad* m_instance;

};

