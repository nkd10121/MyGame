#pragma once
#include "Scene.h"
#include "Vec2.h"

#include <vector>

/// <summary>
/// �J�[�\���p
/// </summary>
enum Button
{
	Retry,
	Return
};

class DrawBGparts;
class Efects;
class AfterImagePlayer;

class GameoverScene : public Scene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="mgr"></param>
	GameoverScene(SceneManager& mgr);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameoverScene();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="input">���͏��</param>
	void Update(Input& input);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private:
	//�t�F�[�h�p�t���[��
	int m_fadeFrame = 0;
	//�t���[���J�E���g�p�ϐ�
	int m_frame = 0;

	//�I�����Ă���{�^��
	int m_nowButton = Retry;

	//���g���C�{�^����X���W
	int m_retryButtonX;
	//�߂�{�^����X���W
	int m_returnButtonX;

	//�v���C���[�n���h��
	int m_handle[4];

	//�v���C���[���W
	Vec2 m_playerPos;
	//�v���C���[�̍��W����
	Vec2 m_offset{};

	//�w�i��Box
	std::vector<DrawBGparts*> m_parts;

	//�����t���O
	bool m_isRevival;
	//���S�t���O
	bool m_deathAnimationFlag;

	//�G��Y���W
	int enemyY = 0;

	//�v���C���[�̎c��
	std::vector<AfterImagePlayer*> m_afterImage;
	//�v���C���[�̃G�t�F�N�g
	std::vector<Efects*> m_efects;

	void CreateAfterImage(Vec2 pos);

	void CreateDashEfect(Vec2 pos, bool isLastHit = false);

	//��ԑJ�ڗp�����o�֐��|�C���^�̐錾
	using UpdateFunc_t = void (GameoverScene::*)(Input&);
	using DrawFunc_t = void (GameoverScene::*)();

	UpdateFunc_t updateFunc_ = nullptr;
	DrawFunc_t drawFunc_ = nullptr;

	//�X�V�֐�
	void FadeInUpdate(Input& input);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input& input);

	//�`��֐�
	void FadeOutDraw();
	void NormalDraw();
};

