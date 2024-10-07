#pragma once
#include "Scene.h"
#include "../Util/Vec2.h"

#include <vector>

/// <summary>
/// ���̍���
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

	//�v���C���[�̊p�x
	float m_angle = 0;

	int m_BGMnum = 5;
	int m_SEnum = 5;
	bool m_isFullScreen = false;

	bool m_isFromToTitle = false;
	bool m_isReturn = false;

	//�J�[�\���̏��
	Info m_cursol;
	Info m_cursolLog;

	//���ڂ̑I�����̏��
	Info m_itemInfo[8];

	//�c��
	std::vector<AfterImagePlayer*> m_afterImage;
	//�c�������̊֐�
	void CreateAfterImage(Vec2 pos);

	//�X�V�����o�֐��|�C���^
	void (PauseScene::* updateFunc_)(Input& input);
	//�`�惁���o�֐��|�C���^
	void (PauseScene::* drawFunc_)();

	//�ʏ��Ԃ�Update
	void DefaultUpdate(Input& input);
	//�ݒ��Ԃ�Update
	void SettingUpdate(Input& input);

	void ReturnTitleUpdate(Input& input);

	//�ݒ��Ԃ�Draw
	void SettingDraw();
	//���������Ԃ�Draw
	void OperationDraw();

	void ReturnTitleDraw();
	//�����Ԃ�Draw
	void CloseDraw();
};