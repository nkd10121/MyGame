#pragma once
#include "Scene.h"
#include "Vec2.h"

#include <vector>

/// <summary>
/// ���̍���
/// </summary>
enum Item
{
	//�ݒ�
	Setting,
	//�������
	Operation,
	//����
	Close,
	//BGM
	BGM,
	//SE
	SE,
	//�t���X�N���[��
	FullScreen,
	//����enum�̐�
	ItemNum
};

class AfterImagePlayer;

class OptionScene :  public Scene
{
	/// <summary>
	/// ���ږ��ƍ��W������
	/// </summary>
	struct Info
	{
		//���ږ�
		Item item;
		//���W
		Vec2 pos;
	};

public:
	OptionScene(SceneManager& manager);
	~OptionScene();
	void Update(Input& input);
	void Draw();

private:
	int m_frame = 0;

	int m_pHandle = -1;
	int m_checkHandle = -1;

	//�v���C���[�̊p�x
	float m_angle = 0;

	int m_BGMnum = 0;
	int m_SEnum = 0;
	bool m_isFullScreen = false;

	//�J�[�\���̏��
	Info m_cursol;
	Info m_cursolLog;

	//���ڂ̑I�����̏��
	Info m_itemInfo[7];

	//�c��
	std::vector<AfterImagePlayer*> m_afterImage;
	//�c�������̊֐�
	void CreateAfterImage(Vec2 pos);

	//�X�V�����o�֐��|�C���^
	void (OptionScene::* updateFunc_)(Input& input);
	//�`�惁���o�֐��|�C���^
	void (OptionScene::* drawFunc_)();

	//�ʏ��Ԃ�Update
	void DefaultUpdate(Input& input);
	//�ݒ��Ԃ�Update
	void SettingUpdate(Input& input);

	//�ݒ��Ԃ�Draw
	void SettingDraw();
	//���������Ԃ�Draw
	void OperationDraw();
	//�����\�L��Ԃ�Draw
	void CreditDraw();
	//�����Ԃ�Draw
	void CloseDraw();
};

