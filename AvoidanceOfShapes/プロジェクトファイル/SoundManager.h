#pragma once
#include "DxLib.h"
#include <string>
#include <vector>

using namespace std;

struct Sound;

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	/// <summary>
	/// サウンドを読み込む
	/// </summary>
	/// <param name="name">サウンドネーム</param>
	/// <param name="path">サウンドパス</param>
	/// <param name="isBGM">true = BGM,false = SE</param>
	void Load(string name,string path,bool isBGM);

	/// <summary>
	/// 指定した名前のサウンドを流す
	/// </summary>
	/// <param name="name">流したいサウンドネーム</param>
	/// <param name="isFromStart">true = 最初から,false = 途中から</param>
	void PlayBGM(string name,bool isFromStart);

	void PlaySE(string name);

	void FadeOutBGM(string name,int fadeFrame);

	/// <summary>
	/// 指定した名前のサウンドを止める
	/// </summary>
	/// <param name="name">止めたいサウンドネーム</param>
	void StopBGM(string name);

	/// <summary>
	/// eternalフラグがfalseのサウンドを削除する
	/// </summary>
	void Delete();

	/// <summary>
	/// 指定したサウンドが流れているどうか
	/// </summary>
	/// <param name="name">サウンドネーム</param>
	/// <returns>true = 流れている,false = 流れていない</returns>
	bool isPlayingSound(string name);

	void ChangeBGMVolume(int volume);
	void ChangeSEVolume(int volume);

	

private:
	vector<Sound*> m_BGM;
	vector<Sound*> m_SE;

	float m_BGMvolume = 0.5f;
	float m_SEvolume = 0.5f;
	/// <summary>
	/// 指定したハンドルのサウンドが流れているかどうか
	/// </summary>
	/// <param name="handle">チェックしたいハンドル</param>
	/// <returns>true = 流れている,false = 流れていない</returns>
	bool CheckPlaying(int handle) { return CheckSoundMem(handle); }
};