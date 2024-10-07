#pragma once
#include "Scene.h"
#include "../Util/Vec2.h"

#include <vector>

/// <summary>
/// 左の項目
/// </summary>
enum Item
{
	Setting,
	Operation,
	ToTitle,
	Close,
	BGM,
	SE,
	FullScreen,
	ReturnTitle,
	ItemNum
};

struct Info
{
	Item item;
	Vec2 pos;
};

class AfterImagePlayer;

class PauseScene :public Scene
{
public:
	PauseScene(SceneManager& manager);
	~PauseScene();
	void Update(Input& input);
	void Draw();

private:
	int m_frame = 0;

	int m_pHandle = -1;
	int m_checkHandle = -1;

	//プレイヤーの角度
	float m_angle = 0;

	int m_BGMnum = 5;
	int m_SEnum = 5;
	bool m_isFullScreen = false;

	bool m_isFromToTitle = false;
	bool m_isReturn = false;

	//カーソルの情報
	Info m_cursol;
	Info m_cursolLog;

	//項目の選択肢の情報
	Info m_itemInfo[8];

	//残像
	std::vector<AfterImagePlayer*> m_afterImage;
	//残像生成の関数
	void CreateAfterImage(Vec2 pos);

	//更新メンバ関数ポインタ
	void (PauseScene::* updateFunc_)(Input& input);
	//描画メンバ関数ポインタ
	void (PauseScene::* drawFunc_)();

	//通常状態のUpdate
	void DefaultUpdate(Input& input);
	//設定状態のUpdate
	void SettingUpdate(Input& input);

	void ReturnTitleUpdate(Input& input);

	//設定状態のDraw
	void SettingDraw();
	//操作説明状態のDraw
	void OperationDraw();

	void ReturnTitleDraw();
	//閉じる状態のDraw
	void CloseDraw();
};