#pragma once

//当たり判定を管理するクラス
class Col
{
public:
	Col();
	virtual ~Col();

	/// <summary>
	/// 四角の当たり判定の範囲を描画
	/// </summary>
	/// <param name="color">色</param>
	void DrawColBox(unsigned int color);

	/// <summary>
	/// 円の当たり判定の範囲を描画
	/// </summary>
	/// <param name="color">色</param>
	void DrawColCircle(unsigned int color);

	/// <summary>
	/// 四角の当たり判定の設定
	/// </summary>
	/// <param name="left">左のx座標</param>
	/// <param name="top">上のy座標</param>
	/// <param name="right">右のx座標</param>
	/// <param name="bottom">下のy座標</param>
	void SetBox(float left, float top, float right, float bottom);

	/// <summary>
	/// 四角の当たり判定の設定
	/// </summary>
	/// <param name="left">左のx座標</param>
	/// <param name="top">上のy座標</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	void SetLT(float left, float top, float width, float height);

	/// <summary>
	/// 四角の当たり判定の設定
	/// </summary>
	/// <param name="x">中心のx座標</param>
	/// <param name="y">中心のy座標</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	void SetCenter(float x, float y, float width, float height);

	/// <summary>
	/// 円の当たり判定の設定
	/// </summary>
	/// <param name="center"></param>
	/// <param name="m_rad"></param>
	void SetCircle(float x,float y, int m_rad);

	//ゲームによっては中心座標で指定したり足元の座標で指定する
	//set関数があったほうが便利かもしれない

	//必要な情報を取得する
	float GetWidth() const;		//幅
	float GetHeight() const;	//高さ

	/// <summary>
	/// 矩形同士の当たり判定
	/// </summary>
	/// <param name="col">: 矩形の当たり判定</param>
	/// <returns></returns>
	bool IsCollision(const Col& col);
	
	/// <summary>
	/// 矩形と円の当たり判定
	/// 注意:矩形側から呼び出す必要あり
	/// </summary>
	/// <param name="col">: 円の当たり判定</param>
	/// <returns></returns>
	bool IsCollisionCircle(const Col& col);

private:
	float m_left;
	float m_top;
	float m_right;
	float m_bottom;

	float m_centerX;
	float m_centerY;
	int m_rad;
};

