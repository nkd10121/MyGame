#include "EnemyBox.h"
#include "DxLib.h"

namespace
{
	//大きさを小さくするための定数
	constexpr int kDelSize = 8;

	constexpr unsigned int kDefaultColor = 0xff00ff;
	constexpr unsigned int kWhiteColor = 0xffffff;
	constexpr unsigned int kPreColor = 0x440044;
}

EnemyBox::EnemyBox() :
	m_pos			(0.0f, 0.0f),
	m_sizeMax		(0),
	m_size			(0),
	m_growSizeSpeed	(0),
	m_color			(0xff00ff),
	m_isMax			(false),
	m_isDrawPre		(false)
{
}

EnemyBox::~EnemyBox()
{
}

void EnemyBox::Init(Vec2 pos, int size,int speed, int firstFrame, int secondFrame,bool isPre)
{
	/*情報の設定*/
	//存在フラグをtrueにする
	m_isExist = true;

	//座標を設定する
	m_pos = pos;
	//最大サイズを設定する
	m_sizeMax = size;
	//待機フレーム数の設定
	m_waitFrame = firstFrame;
	//消え始めるフレーム数の設定
	m_disappearFrame = secondFrame;

	//大きくなるはやさの設定
	m_growSizeSpeed = speed;
	//予測線を描画するかどうかを設定
	m_isDrawPre = isPre;
}

void EnemyBox::Update()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		return;
	}

	//フレーム数を更新
	m_frame++;

	//最初に指定したフレームを過ぎると実体が出てくる
	if (m_frame >= m_waitFrame)
	{
		if (m_frame < m_disappearFrame)
		{
			//もし最大サイズになったら
			if (m_size > m_sizeMax + static_cast<int>(m_sizeMax * 0.1f))
			{
				//最大サイズになったかフラグをtrueにして
				m_isMax = true;
				//色を白にする
				m_color = kWhiteColor;
			}
			else
			{
				//それ以外の時はデフォルトカラーにする
				m_color = kDefaultColor;
			}

			//最大になっていないときは大きくなる
			if (!m_isMax)
			{
				m_size += m_growSizeSpeed;
			}
			//一瞬最大サイズより大きくなるため補正する
			else if (m_size > m_sizeMax)
			{
				m_size -= kDelSize;
			}
		}

		//実体の当たり判定の設定
		m_col.SetBox(m_pos.x - m_size, m_pos.y - m_size,
			m_pos.x + m_size, m_pos.y + m_size);
	}
	else
	{
		//実体の当たり判定を設定するまでの間、当たり判定が{0,0}にあって端に行くと当たってしまうため
		//そのバグ対策の処理
		m_col.SetBox(-1, -1, -1, -1);
	}

	//二番目に指定したフレームを過ぎると小さくなる
	if (m_frame >= m_disappearFrame)
	{
		m_size -= m_growSizeSpeed;
	}

	//サイズが0より小さくなると存在を消す
	if (m_size < 0)
	{
		m_isExist = false;
	}
}

void EnemyBox::Draw()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		return;
	}

	//予測線の描画フラグがtrueなら
	if (m_isDrawPre)
	{
		//予測の描画
		if (!m_isMax)
		{
			DrawBox(static_cast<int>(m_pos.x) - m_sizeMax, static_cast<int>(m_pos.y) - m_sizeMax,
				static_cast<int>(m_pos.x) + m_sizeMax, static_cast<int>(m_pos.y) + m_sizeMax, kPreColor, true);
		}
	}

	//本体の描画
	DrawBox(static_cast<int>(m_pos.x) - m_size, static_cast<int>(m_pos.y) - m_size,
		static_cast<int>(m_pos.x) + m_size, static_cast<int>(m_pos.y) + m_size, m_color, true);

#ifdef _DEBUG
	m_col.DrawColBox(0xffffff);
#endif
}
