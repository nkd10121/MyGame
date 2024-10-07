#pragma once
#include <vector>

#include "DxLib.h"
#include "../Scene.h"
#include "../../Util/Vec2.h"
#include "../../Util/Col.h"
#include "../../Util/Game.h"

class Player;
class GoalEfect;
class EnemyLineBase;
class EnemyCircleBase;
class Goal;

class StageBase : public Scene
{
public:
	StageBase(SceneManager& mgr);
	~StageBase();
	virtual void Update(Input& input);
	virtual void Draw();

	/// <summary>
	/// ���C���n�̓G�̍쐬�֐�
	/// </summary>
	/// <param name="pos">�E��̍��W</param>
	/// <param name="width">�傫��</param>
	/// <param name="first">�{�̂������t���[��</param>
	/// <param name="second">�{�̂������n�߂�t���[��</param>
	/// <param name="enemy">Line�̎��</param>
	void CreateEnemyLineFrom(Vec2 pos, int width,int first,int second, EnemyLineBase* enemy);

	/// <summary>
	/// Box�G�̍쐬�֐�
	/// </summary>
	/// <param name="pos">���S���W</param>
	/// <param name="size">��ӂ̒���</param>
	/// <param name="speed">�傫���Ȃ鑬��</param>
	/// <param name="firstFrame">�{�̂��o�Ă���t���[��</param>
	/// <param name="secondFrame">�{�̂������n�߂�t���[��</param>
	/// <param name="isPre">�\����������邩�ǂ���</param>
	void CreateEnemyBox(Vec2 pos, int size, int speed, int firstFrame, int secondFrame,bool isPre = true);

	void CreateEnemyCircle(float pos, int radius, bool isSmooth);

	void CreateEnemyBurst(Vec2 pos, Vec2 vec, int speed, int first, int second);

	void CreateEnemyCircleScaling(Vec2 pos, int rad,int first,int second,int speed = 20);

	void AddEnemy(EnemyLineBase* test);

protected:
	//Update�̍ŏ��̃t���[���̏����̂��߂�bool
	bool m_isFirst;

	//�t�F�[�h�C�����I��������ǂ���
	bool m_isEndFadeIn;

	/*�v���C���[�֘A*/
	//�v���C���[�{��
	Player* m_player;
	//�v���C���[�̓G�Ƃ̓����蔻��
	Col m_playerColEnemy;
	//�v���C���[�̃S�[���Ƃ̓����蔻��
	Col m_playerColGoal;

	//�����̓G�ɓ����ɓ�����ƈ�C��HP������̂�h�����߂�bool
	bool m_isHit;

	/*��`�̓����蔻������G�֌W*/
	std::vector<EnemyLineBase*> m_eneLine;
	Vec2 m_eneLinePos;
	int m_lineWidth;

	/*�~�̓����蔻������G�֌W*/
	std::vector<EnemyCircleBase*> m_eneCircle;
	Vec2 m_eneCirclePos;
	Vec2 m_eneCircleBurstVec;
	int m_eneCircleRadius;
	int m_eneCircleBurstSpeed;

	int m_frame;

	Goal* m_goal;
	bool m_goalFlag;
	bool m_onGoal;

	//��̃Q�[�W�̂��
	float m_progressSpeed;

	float m_progressPos;

	/*��ʗh�炷�֌W*/
	int m_screenX;
	int m_screenY;
	int m_shakeFrame;
	int m_gameScreenHandle;
	float m_amp;

	//�S�[���G�t�F�N�g
	bool m_isStartGoalEfect;
	int m_goalEfectFrame;
	GoalEfect* m_goalEfect;

	//�T�E���h�̃t�F�[�h�A�E�g
	bool m_isSoundFadeOut;
	int m_soundFadeFrame;

	//��ʓ_�ł�bool
	bool m_isHitFlash;
	int m_alpha;

	bool m_isTutorial;

	///////////////////////////////
	float m_mag;
	float m_fadeoutAngle;

	float m_magSpeed;
	float m_fadeoutAngleSpeed;

	std::vector<int> m_posX;
	std::vector<int> m_posY;
	///////////////////////////////


	//��ԑJ�ڗp�����o�֐��|�C���^�̐錾
	using UpdateFunc_t = void (StageBase::*)(Input&);
	using DrawFunc_t = void (StageBase::*)();

	UpdateFunc_t updateFunc_ = nullptr;
	DrawFunc_t drawFunc_ = nullptr;

	int m_fadeFrame;


	//�v���C���[���G�ɓ����������̏������܂Ƃ߂��֐�
	void PlayerHit();

	//�X�V�֐�
	virtual void FadeInUpdate(Input& input);
	virtual void NormalUpdate(Input& input);
	virtual void FadeOutUpdate(Input& input);

	//�`��֐�
	void FadeInDraw();
	void FadeOutDraw();
	void NormalDraw();
};

