//#pragma once
//#include "Vec2.h"
////当たり判定を管理するクラス
//class Rect
//{
//public:
//	Rect();
//	virtual ~Rect();
//	
//	void Draw(unsigned int color, bool isFill);
//
//	void DrawRectCircle(unsigned int color, bool isFill);
//
//	void SetBox(float left, float top, float right, float bottom);
//
//	//左上座標と幅高さを指定
//	void SetLT(float left, float top, float width, float height);
//
//	//中心座標と幅高さを指定
//	void SetCenter(float x, float y, float width, float height);
//
//	void SetCircle(Vec2 center, int m_rad);
//
//	//ゲームによっては中心座標で指定したり足元の座標で指定する
//	//set関数があったほうが便利かもしれない
//
//	//必要な情報を取得する
//	float GetWidth() const;		//幅
//	float GetHeight() const;	//高さ
//	Vec2 GetCenter() const;		//中心座標
//
//	//矩形同士の当たり判定
//	bool IsCollision(const Rect& rect);
//
//	//矩形と円の当たり判定
//	bool IsCollisionCircle(const Rect& rect);
//
//private:
//	float m_left;
//	float m_top;
//	float m_right;
//	float m_bottom;
//
//	Vec2 m_center;
//	int m_rad;
//};
//
