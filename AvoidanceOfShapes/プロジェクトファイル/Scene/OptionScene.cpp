#include "OptionScene.h"
#include "Input.h"
#include "Game.h"
#include "SceneManager.h"
#include <DxLib.h>

#include "Application.h"
#include "AfterImagePlayer.h"

namespace
{
	//残像の最大数
	constexpr int kAfterImageNumMax = 64;

	//項目別の座標
	const Vec2 kItemPos[ItemNum] = {
		{ 150.0f, 100.0f },
		{ 100.0f, 200.0f },
		{ 130.0f, 300.0f },
		{ 376.0f, 110.0f },
		{ 376.0f, 210.0f },
		{ 376.0f, 310.0f }
	};

	//音量設定の四角の数
	constexpr int kVolumeEmptyBoxNum = 10;
	//音量設定の四角のサイズ
	constexpr int kVolumeEmptyBoxSize = 20;

	//音量設定の四角同士の間隔
	constexpr int kVolumeEmptyBoxPosXInterval = 40;

	//音量設定のテキストのX座標
	constexpr int kSettingTextPosX = 400;
	//音量設定の四角の一番左のX座標
	constexpr int kVolumeEmptyBoxPosX = kSettingTextPosX + 10;

	//音量設定のBGMテキストのY座標
	constexpr int kBGMTextPosY = 100;
	//音量設定のBGMの四角のY座標
	constexpr int kBGMEmptyBoxPosY = kBGMTextPosY + 50;

	//音量設定のSEテキストのY座標
	constexpr int kSETextPosY = kBGMTextPosY + 100;
	//音量設定のSEの四角のY座標
	constexpr int kSEEmptyBoxPosY = kSETextPosY + 50;

	//音量設定の四角(ピンクのやつ)のX座標
	constexpr int kVolumeBoxPosX = 400;
	//音量設定の四角(ピンクのやつ)のサイズ
	constexpr int kVolumeBoxSize = 40;
	//音量設定の四角(ピンクのやつ)同士の間隔
	constexpr int kVolumePosXInterval = 20;

	//音量設定のBGMの四角(ピンクのやつ)のY座標
	constexpr int kBGMBoxPosY = kBGMTextPosY + 40;
	//音量設定のSEの四角(ピンクのやつ)のY座標
	constexpr int kSEBoxPosY = kSETextPosY + 40;

	constexpr int kBoxColor = 0xff00ff;
	const Vec2 kCursolOffset = { 24.0f,-24.0f };

	//フルスクリーンのテキスト描画Y座標
	constexpr int kFullScrennTextPosY = 300;
	//フルスクリーンのチェックボックスのX座標
	constexpr int kFullScreenCheckBoxPosX = 420;
	//フルスクリーンのチェックボックスのY座標
	constexpr int kFullScreenCheckBoxPosY = 340;
	//フルスクリーンのチェックボックスの大きさ
	constexpr int kFullScreenCheckBoxSize = 40;

	//フルスクリーンのチェックマークのX座標
	constexpr int kFullScreenCheckMarkPosX = 440;
	//フルスクリーンのチェックマークのY座標
	constexpr int kFullScreenCheckMarkPosY = 360;
	//フルスクリーンのチェックマークの拡大率
	constexpr float kFullScreenCheckMarkExrate = 0.08f;

	//操作方法のテキストの描画座標
	constexpr int kOperationTextPosX = 380;
	constexpr int kOperationKeyBoardTextPosY = 100;
	constexpr int kOperationControllerTextPosY = 400;
	//操作方法の画像の描画座標
	constexpr int kOperationKeyBoardGraphPosX = 640;
	constexpr int kOperationKeyBoardGraphPosY = 260;
	constexpr int kOperationControllerGraphPosX = 710;
	constexpr int kOperationControllerGraphPosY = 540;
	//操作方法の画像の拡大率
	constexpr float kOperationKeyBoardGraphExrate = 0.8f;
	constexpr float kOperationControllerGraphExrate = 1.4f;


	//プレイヤーが毎フレームちょっとずつ回転するためのAngle
	constexpr float kRotationPlayerAngle = 0.08f;
	//残像が作られるフレーム間隔
	constexpr int kPlayerAfterImageIntervalFrame = 4;
	//画面端からの距離
	constexpr int kRangeFromScreenEdge = 50;
	//ウィンドウの背景色
	constexpr int kWindowBackColor = 0x222222;
	//白(ウィンドウの縁や文字の色)
	constexpr int kWhite = 0xffffff;
	//残像が飛んでいく向き
	const Vec2 kAfterImageDirection = { 1,0 };
	//残像のサイズ
	constexpr int kAfterImageSize = 4;
	//プレイヤーの描画倍率
	constexpr float kPlayerExrate = 0.8f;
	//音量の最大
	constexpr int kVolumeMax = 10;
}

OptionScene::OptionScene(SceneManager& manager) :
	Scene(manager)
{
	//プレイヤーハンドルの取得
	m_pHandle = m_manager.m_gManager.GetHandle("Player");
	m_checkHandle = m_manager.m_gManager.GetHandle("Check");

	//項目の初期化
	//[0] = 設定
	m_itemInfo[Setting].item = Setting;
	m_itemInfo[Setting].pos = kItemPos[Setting];
	//[1] = 操作方法
	m_itemInfo[Operation].item = Operation;
	m_itemInfo[Operation].pos = kItemPos[Operation];
	//[2] = 閉じる
	m_itemInfo[Close].item = Close;
	m_itemInfo[Close].pos = kItemPos[Close];
	//[3] = BGMの音量調整
	m_itemInfo[BGM].item = BGM;
	m_itemInfo[BGM].pos = kItemPos[BGM];
	//[4] = SEの音量調整
	m_itemInfo[SE].item = SE;
	m_itemInfo[SE].pos = kItemPos[SE];
	//[5] = フルスクリーン
	m_itemInfo[FullScreen].item = FullScreen;
	m_itemInfo[FullScreen].pos = kItemPos[FullScreen];

	//カーソルは最初に設定のところに
	m_cursol = m_itemInfo[Setting];

	m_BGMnum = m_manager.GetBGMvolume();
	m_SEnum = m_manager.GetSEvolume();
	m_isFullScreen = m_manager.GetisFull();

	//残像のサイズ
	m_afterImage.resize(kAfterImageNumMax);

	updateFunc_ = &OptionScene::DefaultUpdate;
	drawFunc_ = &OptionScene::SettingDraw;
}

OptionScene::~OptionScene()
{
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			delete m_afterImage[i];
			m_afterImage[i] = nullptr;
		}
	}
}

void OptionScene::Update(Input& input)
{
	//オプション画面を開いたときもならないように
	if (m_cursolLog.pos != Vec2{})
	{
		//カーソル移動時に音声を流す
		if (m_cursol.item != m_cursolLog.item)
		{
			m_manager.m_sManager.PlaySE("moveSE");
		}
	}

	m_cursolLog = m_cursol;

	m_frame++;
	m_angle += kRotationPlayerAngle;

	(this->*updateFunc_)(input);

	/*カーソルの残像関係*/
	if (m_frame % kPlayerAfterImageIntervalFrame == 0)
	{
		CreateAfterImage(m_cursol.pos - kCursolOffset);
	}

	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			m_afterImage[i]->Update();

			//使用済みの敵キャラクタを削除する必要がある
			if (!m_afterImage[i]->isExist())
			{
				//メモリを解放する
				delete m_afterImage[i];
				m_afterImage[i] = nullptr;	//使っていないとわかるように
			}
		}
	}
}

void OptionScene::Draw()
{
	/*枠の描画*/
	SetDrawBlendMode(DX_BLENDMODE_MUL, 255);
	DrawBox(kRangeFromScreenEdge, kRangeFromScreenEdge, Game::kScreenWidth - kRangeFromScreenEdge, Game::kScreenHeight - kRangeFromScreenEdge, kWindowBackColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	DrawBox(kRangeFromScreenEdge, kRangeFromScreenEdge, Game::kScreenWidth - kRangeFromScreenEdge, Game::kScreenHeight - kRangeFromScreenEdge, kWhite, false);

	/*項目の描画*/
	DrawStringToHandle(static_cast<int>(m_itemInfo[Setting].pos.x), static_cast<int>(m_itemInfo[Setting].pos.y), "設定", kWhite, m_manager.ReturnFontHandle());
	DrawStringToHandle(static_cast<int>(m_itemInfo[Operation].pos.x), static_cast<int>(m_itemInfo[Operation].pos.y), "操作説明", kWhite, m_manager.ReturnFontHandle());
	DrawStringToHandle(static_cast<int>(m_itemInfo[Close].pos.x), static_cast<int>(m_itemInfo[Close].pos.y), "閉じる", kWhite, m_manager.ReturnFontHandle());

	/*残像の描画*/
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (m_afterImage[i])
		{
			m_afterImage[i]->Draw();
		}
	}

	/*カーソルの描画*/
	DrawRotaGraph(static_cast<int>(m_cursol.pos.x - kCursolOffset.x), static_cast<int>(m_cursol.pos.y - kCursolOffset.y), kPlayerExrate, m_angle, m_pHandle, true);

	(this->*drawFunc_)();
}

void OptionScene::CreateAfterImage(Vec2 pos)
{
	for (int i = 0; i < m_afterImage.size(); i++)
	{
		if (!m_afterImage[i])
		{
			m_afterImage[i] = new AfterImagePlayer;
			m_afterImage[i]->Init(pos, kAfterImageDirection, kAfterImageSize, false);
			return;
		}
	}
}

void OptionScene::DefaultUpdate(Input& input)
{
	/*入力関係*/
	if (input.IsTriggered("pause") || m_cursol.item == Close && input.IsTriggered("OK")/*|| input.IsTriggered("CANCEL")*/)
	{
		m_manager.popScene();
	}

	if (input.IsTriggered("UP"))
	{
		if (m_cursol.item == Operation)
		{
			m_cursol = m_itemInfo[Setting];
		}

		if (m_cursol.item == Close)
		{
			m_cursol = m_itemInfo[Operation];
		}
	}

	if (input.IsTriggered("DOWN"))
	{


		if (m_cursol.item == Operation)
		{
			m_cursol = m_itemInfo[Close];
		}

		if (m_cursol.item == Setting)
		{
			m_cursol = m_itemInfo[Operation];
		}
	}

	/*状態遷移関係*/
	if (m_cursol.item == m_itemInfo[Setting].item)
	{
		drawFunc_ = &OptionScene::SettingDraw;
		if (input.IsTriggered("OK"))
		{
			m_cursol = m_itemInfo[BGM];
			updateFunc_ = &OptionScene::SettingUpdate;
		}
	}

	if (m_cursol.item == m_itemInfo[Operation].item)
	{
		drawFunc_ = &OptionScene::OperationDraw;
	}

	if (m_cursol.item == m_itemInfo[Close].item)
	{
		drawFunc_ = &OptionScene::CloseDraw;
	}

}

void OptionScene::SettingUpdate(Input& input)
{
	//ここにカーソル移動とかと
	//音量設定とかをいじれるようにする

	if (input.IsTriggered("UP"))
	{
		if (m_cursol.item == m_itemInfo[SE].item)
		{
			m_cursol = m_itemInfo[BGM];
		}
		if (m_cursol.item == m_itemInfo[FullScreen].item)
		{
			m_cursol = m_itemInfo[SE];
		}
	}

	if (input.IsTriggered("DOWN"))
	{
		if (m_cursol.item == m_itemInfo[SE].item)
		{
			m_cursol = m_itemInfo[FullScreen];
		}
		if (m_cursol.item == m_itemInfo[BGM].item)
		{
			m_cursol = m_itemInfo[SE];
		}
	}

	if (m_cursol.item == m_itemInfo[BGM].item)
	{
		if (input.IsTriggered("RIGHT"))
		{
			if (m_BGMnum < kVolumeMax)
			{
				m_BGMnum++;
			}
			m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		}
		if (input.IsTriggered("LEFT"))
		{
			if (m_BGMnum > 0)
			{
				m_BGMnum--;
			}
			m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		}
	}

	if (m_cursol.item == m_itemInfo[SE].item)
	{
		if (input.IsTriggered("RIGHT"))
		{
			if (m_SEnum < kVolumeMax)
			{
				m_SEnum++;
			}
			m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		}
		if (input.IsTriggered("LEFT"))
		{
			if (m_SEnum > 0)
			{
				m_SEnum--;
			}
			m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		}
	}

	if (m_cursol.item == m_itemInfo[FullScreen].item)
	{
		if (input.IsTriggered("OK"))
		{
			m_isFullScreen = !m_isFullScreen;
			Application::GetInstance().ChangeScreenSize();
		}
	}

	if (input.IsTriggered("pause") || input.IsTriggered("CANCEL"))
	{
		m_manager.SetInfo(m_BGMnum, m_SEnum, m_isFullScreen);
		m_cursol = m_itemInfo[Setting];
		updateFunc_ = &OptionScene::DefaultUpdate;
	}
}

void OptionScene::SettingDraw()
{
	DrawStringToHandle(kSettingTextPosX, kBGMTextPosY, "BGM音量", kWhite, m_manager.ReturnFontHandle2());
	for (int i = 0; i < kVolumeEmptyBoxNum; i++)
	{
		DrawBox(kVolumeEmptyBoxPosX + (kVolumeEmptyBoxSize + kVolumeEmptyBoxPosXInterval) * i, kBGMEmptyBoxPosY, kVolumeEmptyBoxPosX + kVolumeEmptyBoxSize + (kVolumeEmptyBoxSize + kVolumeEmptyBoxPosXInterval) * i, kBGMEmptyBoxPosY + kVolumeEmptyBoxSize, kWhite, false);
	}

	for (int i = 0; i < m_BGMnum; i++)
	{
		DrawBox(kVolumeBoxPosX + (kVolumeBoxSize + kVolumePosXInterval) * i, kBGMBoxPosY, kVolumeBoxPosX + kVolumeBoxSize + (kVolumeBoxSize + kVolumePosXInterval) * i, kBGMBoxPosY + kVolumeBoxSize, kBoxColor, true);
	}

	DrawStringToHandle(kSettingTextPosX, kSETextPosY, "SE音量", kWhite, m_manager.ReturnFontHandle2());
	for (int i = 0; i < kVolumeEmptyBoxNum; i++)
	{
		DrawBox(kVolumeEmptyBoxPosX + (kVolumeEmptyBoxSize + kVolumeEmptyBoxPosXInterval) * i, kSEEmptyBoxPosY, kVolumeEmptyBoxPosX + kVolumeEmptyBoxSize + (kVolumeEmptyBoxSize + kVolumeEmptyBoxPosXInterval) * i, kSEEmptyBoxPosY + kVolumeEmptyBoxSize, kWhite, false);
	}

	for (int i = 0; i < m_SEnum; i++)
	{
		DrawBox(kVolumeBoxPosX + (kVolumeBoxSize + kVolumePosXInterval) * i, kSEBoxPosY, kVolumeBoxPosX + kVolumeBoxSize + (kVolumeBoxSize + kVolumePosXInterval) * i, kSEBoxPosY + kVolumeBoxSize, kBoxColor, true);
	}

	DrawStringToHandle(kSettingTextPosX, kFullScrennTextPosY, "フルスクリーン", kWhite, m_manager.ReturnFontHandle2());
	DrawBox(kFullScreenCheckBoxPosX, kFullScreenCheckBoxPosY, kFullScreenCheckBoxPosX + kFullScreenCheckBoxSize, kFullScreenCheckBoxPosY + kFullScreenCheckBoxSize, kWhite, false);

	if (m_isFullScreen)
	{
		DrawRotaGraph(kFullScreenCheckMarkPosX, kFullScreenCheckMarkPosY, kFullScreenCheckMarkExrate, 0.0f, m_checkHandle, true);
	}

}

void OptionScene::OperationDraw()
{
	DrawStringToHandle(kOperationTextPosX, kOperationKeyBoardTextPosY, "キーボード:", kWhite, m_manager.ReturnFontHandle2());
	m_manager.m_gManager.Draw(kOperationKeyBoardGraphPosX, kOperationKeyBoardGraphPosY, kOperationKeyBoardGraphExrate, 0.0f, "sousaKey");

	DrawStringToHandle(kOperationTextPosX, kOperationControllerTextPosY, "コントローラー:", kWhite, m_manager.ReturnFontHandle2());
	m_manager.m_gManager.Draw(kOperationControllerGraphPosX, kOperationControllerGraphPosY, kOperationControllerGraphExrate, 0.0f, "sousaCon");

}

void OptionScene::CloseDraw()
{
	//何も書かない
}
