#pragma once
#include "Scene.h"
#include "../Util/Col.h"
#include "../Util/Vec2.h"
#include <vector>

class Player;
class DrawBGparts;
class AfterImagePlayer;

enum NextScene
{
	nTutorial,
	nStage1,
	nStage2,
	StageNum
};

class StageSelect : public Scene
{
public:
	StageSelect(SceneManager& mgr);
	~StageSelect();
	void Update(Input& input);
	void Draw();

private:
	//�t�F�[�h���̃t���[��
	int m_fadeFrame = 0;
	//�ʏ펞�̃t���[��
	int m_frame = 0;

	///////////////////////////////
	//�t�F�[�h�A�E�g�̎��֘A
	int m_gameScreenHandle = 0;

	float m_mag = 1.0f;
	float m_fadeoutAngle = 0.0f;

	float m_magSpeed = 0.02f;
	float m_fadeoutAngleSpeed = 0.2f;

	std::vector<int> m_posX;
	std::vector<int> m_posY;
	///////////////////////////////

	//�v���C���[�̍��W
	Vec2 m_playerPos{};

	float m_angle = 0;
	float m_offsetY = 0;;

	float m_playerRotationAngle = 0;

	int m_detailX = 0;

	int m_stageAlpha[StageNum] = { 200,200,200 };
	int m_stage0Alpha = 200;
	int m_stage1Alpha = 200;
	int m_stage2Alpha = 200;

	bool m_isMoving = false;

	bool m_isFirst = true;

	NextScene m_next = nTutorial;
	NextScene m_nextLog = m_next;

	//�w�i�̎l�p
	std::vector<DrawBGparts*> m_parts;

	int m_soundFadeFrame = 0;

	//��ԑJ�ڗp�����o�֐��|�C���^�̐錾
	using UpdateFunc_t = void (StageSelect::*)(Input&);
	using DrawFunc_t = void (StageSelect::*)();

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