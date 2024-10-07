#pragma once
#include <iostream>

struct Size
{
	int w = 0;
	int h = 0;
};

/// <summary>
/// アプリケーション全体を管理するクラス
/// シングルトンクラス
/// </summary>
class Application
{
private:
	Size m_windowSize;

	bool m_isFullScreen = true;
	bool m_isPush = true;

	//シングルトンのためにprivateに
	Application() {};

	//コピー＆代入を禁止
	Application(const Application& app) = delete;	//コピーコンストラクタの禁止
	void operator=(const Application& app) = delete;	//代入演算子もついでに廃止


public:
	/// <summary>
	/// Applicationのシングルトンインスタンスの参照を返す
	/// </summary>
	/// <returns>Applicationインスタンス</returns>
	static Application& GetInstance() {
		static Application instance;	//自分自身の静的オブジェクトを作る
		return instance;
	}

	bool Init();
	void Run();
	/// <summary>
	/// 後処理を行う
	/// </summary>
	void Terminate();

	const Size& GetWindowSize() const;

	bool GetIsFullScreen()const { return m_isFullScreen; };

	void ChangeScreenSize();
};

