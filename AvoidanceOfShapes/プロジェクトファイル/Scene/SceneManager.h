#pragma once
#include <list> 
#include <memory>
#include <windows.h>
#include "../SoundManager.h"
#include "../GraphManager.h"

class Input;
class Scene;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	/// <summary>
	/// �����Ă���V�[����Update�֐����Ăяo��
	/// </summary>
	/// <param name="input">���̓N���X</param>
	void Update(Input& input);

	/// <summary>
	/// �����Ă���V�[����Draw�֐����Ăяo��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���s���̃V�[���������Ŏw�肵���V�[���ɐ؂�ւ���
	/// </summary>
	/// <param name="nextScene"></param>
	void ChangeScene(std::shared_ptr<Scene> nextScene);

	/// <summary>
	/// Scene���N���A���ăV�[����؂�ւ���
	/// </summary>
	/// <param name="nextScene"></param>
	void ChangeAndClearScene(std::shared_ptr<Scene> nextScene);

	/// <summary>
	/// ���ݐ擪�Ŏ��s���̃V�[���̏�ɃV�[�����������
	/// </summary>
	/// <param name="scene"></param>
	void PushScene(std::shared_ptr<Scene> scene);

	/// <summary>
	/// ���݂̐擪�V�[�����폜����
	/// </summary>
	void popScene(bool isToTitle = false);

	//TODO:�X�e�[�W���Ƃ̃A�h���X�Ƃ���ۑ�����GameoverScene����retry�Ō��̃X�e�[�W�ɖ߂��悤�ɂ���

	void GetPtr(int stageNum);

	void LoadG(std::string name, std::string path);

	void Draw(int x, int y, float exRate, float angle, std::string name);

	int ReturnFontHandle() { return m_fontHandle; }
	int ReturnFontHandle2() { return m_fontHandle2; }

	void SetStageClearFlag(int stageNum);

	/// <summary>
	/// �X�e�[�W�N���A�t���O��Ԃ�
	/// </summary>
	/// <param name="stageNum">1:stage1, 2:stage2</param>
	/// <returns>�N���A�t���O</returns>
	bool GetStageClearFlag(int stageNum);

	void SetInfo(int BGMvolume, int SEvolume, bool isFull);

	int GetBGMvolume() { return m_BGMvolume; }
	int GetSEvolume() { return m_SEvolume; }
	bool GetisFull() { return m_isFull; }

public:
	/// <summary>
	/// ���O�̃V�[�����c��
	/// </summary>
	std::shared_ptr<Scene> m_logScene;

	std::shared_ptr<Scene> m_StageTutorial;
	std::shared_ptr<Scene> m_stage1;
	std::shared_ptr<Scene> m_stage2;

	std::shared_ptr<Scene> m_pauseScene;
	//std::shared_ptr<Scene> stage3;

	SoundManager m_sManager;
	GraphManager m_gManager;

private:
	std::list < std::shared_ptr<Scene>> scenes;

	LPCSTR m_fontPath;
	int m_fontHandle;
	int m_fontHandle2;

	bool m_clearStageTutorial = false;
	bool m_clearStage1 = false;
	bool m_clearStage2 = false;

	int m_BGMvolume = 0;
	int m_SEvolume = 0;
	bool m_isFull = true;

	void SetBGMvolume(int volume) { m_BGMvolume = volume; }
	void SetSEvolume(int volume) { m_SEvolume = volume; }
	void SetIsFull(bool isFull) { m_isFull = isFull; }

	void Save();
	void Load();

};