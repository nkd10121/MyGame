﻿#include "MapManager.h"
#include <fstream>
#include <sstream>
#include <map>

#include "ModelManager.h"
#include "TrapManager.h"

MapManager* MapManager::m_instance = nullptr;

namespace
{
	float DegreeToRadian(float deg)
	{
		return deg * DX_PI_F / 180.0f;
	}

	const std::string kModelPathFront = "data/model/stage/";
	const std::string kModelPathBack = ".mv1";

	const std::string kStageDataPathFront = "data/stageData/";
	const std::string kStageDataPathBack = ".loc";

	const std::map<std::string,std::string> kId = 
	{
		{"floor","M_FLOOR"},
		{"wall","M_WALL"},
		{"doorFrame","M_FRAME"},
		{"stairs","M_STAIRS"},
		{"stairsWall","M_SMOOTHBLOCK"},
	};
}

/// <summary>
/// コンストラクタ
/// </summary>
MapManager::MapManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
MapManager::~MapManager()
{
}

/// <summary>
/// 初期化
/// </summary>
void MapManager::Init()
{
	//ロードが完了しているときにここが呼ばれるため、モデルマネージャーに頼んでおいたモデルを取得する
	for (auto& path : kId)
	{
		m_handles[path.first] = ModelManager::GetInstance().GetModelHandle(path.second);
	}
}

/// <summary>
/// ステージの生成に必要なモデルの削除
/// </summary>
void MapManager::DeleteModel()
{
	//モデルの削除
	for (auto& model : m_handles)
	{
		MV1DeleteModel(model.second);
	}
	m_handles.clear();

	//モデルとデータの削除
	for (auto& data : m_data)
	{
		MV1DeleteModel(data.handle);
	}
	m_data.clear();
}

/// <summary>
/// ステージ情報を読み込む
/// </summary>
void MapManager::Load(const char* stageName)
{
	//モデルのロード
	//MapManager::LoadModel();

	//開くファイルのハンドルを取得
	int handle = FileRead_open((kStageDataPathFront + stageName + kStageDataPathBack).c_str());

	//読み込むオブジェクト数が何個あるか取得
	int dataCnt = 0;
	FileRead_read(&dataCnt, sizeof(dataCnt), handle);
	//読み込むオブジェクト数分の配列に変更する
	m_data.resize(dataCnt);

	//配列の数分回す
	for (auto& loc : m_data)
	{
		//名前のバイト数を取得する
		byte nameCnt = 0;
		FileRead_read(&nameCnt, sizeof(nameCnt), handle);
		//名前のサイズを変更する
		loc.name.resize(nameCnt);
		//名前を取得する
		FileRead_read(loc.name.data(), sizeof(char) * static_cast<int>(loc.name.size()), handle);

		//タグのバイト数を取得する
		byte tagCnt = 0;
		FileRead_read(&tagCnt, sizeof(tagCnt), handle);
		//タグのサイズを変更する
		loc.tag.resize(tagCnt);
		//タグを取得する
		FileRead_read(loc.tag.data(), sizeof(char) * static_cast<int>(loc.tag.size()), handle);

		//座標を取得する
		FileRead_read(&loc.pos, sizeof(loc.pos), handle);
		//回転を取得する
		FileRead_read(&loc.rot, sizeof(loc.rot), handle);
		//大きさを取得する
		FileRead_read(&loc.scale, sizeof(loc.scale), handle);

		if (loc.tag == "trapPos")
		{
			TrapManager::GetInstance().AddTrapPos(loc.pos);
		}

		m_min = Vec3(min(m_min.x, loc.pos.x), min(m_min.y, loc.pos.y), min(m_min.z, loc.pos.z));
		m_max = Vec3(max(m_max.x, loc.pos.x), max(m_max.y, loc.pos.y), max(m_max.z, loc.pos.z));

	}
	FileRead_close(handle);

	for (auto& loc : m_data)
	{
		loc.rot.x = DegreeToRadian(loc.rot.x);
		loc.rot.y = DegreeToRadian(loc.rot.y);
		loc.rot.z = DegreeToRadian(loc.rot.z);

		loc.handle = MV1DuplicateModel(m_handles[loc.tag]);
		MV1SetPosition(loc.handle, loc.pos.ToVECTOR());
		MV1SetScale(loc.handle, loc.scale.ToVECTOR());
		if (loc.tag != "stairs")
		{
			MV1SetRotationXYZ(loc.handle, loc.rot.ToVECTOR());
		}
		else
		{
			loc.rot.y -= DX_PI_F;
			MV1SetRotationXYZ(loc.handle, loc.rot.ToVECTOR());
		}
	}
}

/// <summary>
/// 描画
/// </summary>
void MapManager::Draw()
{
	for (auto& loc : m_data)
	{
		if (loc.tag != "floor" && loc.tag != "wall" && loc.tag != "doorFrame" && loc.tag != "stairs" && loc.tag != "stairsWall")
		{
			continue;
		}
		MV1DrawModel(loc.handle);
	}
}
