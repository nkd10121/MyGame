#pragma once
#include "Scene.h"
#include "DxLib.h"

#include <vector>

class DrawBGparts;

/// <summary>
/// �^�C�g���V�[���N���X
/// </summary>
class TitleScene : public Scene
{
public:
	TitleScene(SceneManager& manager);
	~TitleScene();
	void Update(Input& input);
	void Draw();

private:
	/// <summary>
	/// �I�����Ă���{�^��
	/// </summary>
	enum ButtonName : int
	{
		Play,
		Option,
		Quit
	};

	//�I�����Ă���{�^��
	int m_nowButton = Play;
	int m_nowButtonLog = m_nowButton;

	int m_fadeFrame = 0;
	int m_frame = 3;

	//�w�i��U��������{��
	float m_mag = 1.0f;
	//�^�C�g�����S�̃T�C�Y
	float m_titleSize = 0.28f;
	//�^�C�g����Y���W
	int m_titlePosY = 280;

	//�v���C�{�^����X���W
	int m_playButtonPosX;
	//�I�v�V�����{�^����X���W
	int m_optionButtonPosX;
	//��߂�{�^����X���W
	int m_quitButtonPosX;

	/*�f������֘A*/
	int m_movieHandle;
	int m_waitFrame = 0;
	bool m_isPlayMovie = false;

	std::vector<DrawBGparts*> m_parts;

	//�X�V�����o�֐��|�C���^
	void (TitleScene::* updateFunc_)(Input& input);

	//�`�惁���o�֐��|�C���^
	using DrawFunc_t = void (TitleScene::*)();
	DrawFunc_t drawFunc_;

	//�X�V�֐�
	void FadeInUpdate(Input&);	//�t�F�[�h�C�����
	void NormalUpdate(Input& input);	//�ʏ���
	void FadeOutUpdate(Input&);	//�t�F�[�h�A�E�g���

	//�`��֐�
	void FadeOutDraw();
	void NormalDraw();
};