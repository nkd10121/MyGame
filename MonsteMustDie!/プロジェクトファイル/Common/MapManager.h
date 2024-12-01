﻿#pragma once
#include "Vec3.h"
#include <string>
#include <vector>
#include <unordered_map>

/// <summary>
/// マップを管理するクラス
/// </summary>
class MapManager
{
private:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	MapManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~MapManager();

	static MapManager* m_instance;	//インスタンス

	/// <summary>
	/// 配置情報データ
	/// </summary>
	struct LocationData
	{
		std::string name;	//名前
		std::string tag;	//タグ
		Vec3 pos;	//座標
		Vec3 rot;	//回転
		Vec3 scale;	//大きさ
		int handle;
	};

public:
	//コピーコンストラクタから実体の生成ができてしまうため
	//コピーコンストラクタを禁止する
	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;
	MapManager(MapManager&&) = delete;
	MapManager& operator= (const MapManager&&) = delete;

	/// <summary>
	/// インスタンスを取得
	/// MapManagerはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns>インスタンス</returns>
	static MapManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new MapManager;
		}

		return *m_instance;
	}

	/// <summary>
	/// 削除
	/// これをし忘れると普通にメモリリーク
	/// </summary>
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// ステージの生成に必要なモデルの削除
	/// </summary>
	void DeleteModel();

	/// <summary>
	/// ステージ情報を読み込む
	/// </summary>
	/// <param name="stageName">ステージ名</param>
	void Load(const char* stageName);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	std::vector<LocationData> m_data;	//マップ情報
	std::unordered_map<std::string, int> m_handles;	//モデルハンドル

	Vec3 m_min;
	Vec3 m_max;
};

