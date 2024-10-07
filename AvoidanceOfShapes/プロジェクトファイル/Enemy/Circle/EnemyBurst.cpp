#include "EnemyBurst.h"
#include "EnemyBurstedCircle.h"
#include "StageBase.h"

namespace
{
	//破裂した後の球の数
	constexpr int kBurstedCircleNum = 8;

	//上方向
	const Vec2 kUp = { 0.0f,1.0f };
	//右方向
	const Vec2 kRight = { 1.0f,0.0f };
	//下方向
	const Vec2 kDown = { 0.0f,-1.0f };
	//左方向
	const Vec2 kLeft = { -1.0f,0.0f };
	//回転させるために毎フレームangleに足す定数
	constexpr float kAddAngle = 0.14f;
	//点滅する間隔のフレーム
	constexpr int kChangeColorIntervalFrame = 4;
	//点滅をし始めるフレーム
	constexpr int kStartChangeColerFrame = 24;
	//radに足す数
	constexpr int kAddRadius = 2;
	//Boxのサイズを出すときに4分の1にするための定数
	constexpr float kQuarter = 0.25f;
	//speedから毎フレーム引く数
	constexpr float kDelSpeed = 0.8f;
	//当たり判定の差分
	constexpr int kColOffset = 4;
}

EnemyBurst::EnemyBurst(StageBase* scene):
	m_pStage	(scene),
	m_frame		(0),
	m_bigFrame	(0),
	m_moveFrame	(0),
	m_offset	(0.0f,0.0f),
	m_angle		(0.0f),
	m_radius	(4),
	m_growSpeed		(0.0f),
	m_diagonal	(0.0f),
	m_vec		(static_cast<float>(GetRand(1280) - 640), static_cast<float>(GetRand(960) - 480)),
	m_isBurst	(false),
	m_circlePos	(0.0f,0.0f)
{
}

EnemyBurst::~EnemyBurst()
{
}

void EnemyBurst::Init(Vec2 pos, Vec2 vec, int speed, int firstFrame, int secondFrame)
{
	/*敵の情報の設定*/
	//座標の設定
	m_pos = pos;
	//移動方向の設定
	m_vec = vec;
	//移動速度の設定
	m_growSpeed = static_cast<float>(speed);

	//大きくなるフレーム数の設定
	m_bigFrame = firstFrame;
	//移動するフレーム数の設定
	m_moveFrame = secondFrame;

	//存在フラグをtrueにする
	m_isExist = true;
	
}

void EnemyBurst::Update()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		//早期リターン
		return;
	}

	//フレーム数を測る
	m_frame++;

	//回転させるためにangleを増やし続ける
	m_angle += kAddAngle;

	//破裂するタイミングがわかるように色を点滅させる
	if (m_frame % kChangeColorIntervalFrame == 0 && m_frame > m_moveFrame - kStartChangeColerFrame)
	{
		//一瞬だけ白にする
		m_color = 0xffffff;
	}
	else
	{
		//そのほかの時は通常の色にする
		m_color = 0xff00ff;
	}

	//最初に設定したフレームを超えるまで
	if (m_frame <= m_bigFrame)
	{
		//大きくなり
		m_radius += kAddRadius;
		//回転する
		m_diagonal = static_cast<float>(m_radius) + (static_cast<float>(m_radius) * kQuarter);
	}

	//移動するフレームを超えるまで移動する
	if (m_frame <= m_moveFrame)
	{
		//移動させる
		m_pos += m_vec.getNormalize() * m_growSpeed;

		//当たり判定の設定
		m_col.SetCircle(m_pos.x,m_pos.y, m_radius);

		//四角のxyそれぞれの座標を出すための計算
		m_offset.x = cosf(m_angle) * m_diagonal;
		m_offset.y = sinf(m_angle) * m_diagonal;
	}
	//上の条件以外のタイミングでまだ破裂していないとき
	else if (!m_isBurst)
	{
		//破裂フラグをtrueにする
		m_isBurst = true;

		//破裂後の敵に座標を渡して
		m_circlePos = m_pos;

		//8方向に移動する敵を生成する
		CreateBurstedCircle(m_circlePos, kUp);
		CreateBurstedCircle(m_circlePos, kUp + kRight);
		CreateBurstedCircle(m_circlePos, kRight);
		CreateBurstedCircle(m_circlePos, kRight + kDown);
		CreateBurstedCircle(m_circlePos, kDown);
		CreateBurstedCircle(m_circlePos, kDown + kLeft);
		CreateBurstedCircle(m_circlePos, kLeft);
		CreateBurstedCircle(m_circlePos, kLeft + kUp);
	}

	//ちょっとずつ移動スピードを下げる
	m_growSpeed -= kDelSpeed;
	//0より小さくなったら0に補正する
	if (m_growSpeed < 0)
	{
		m_growSpeed = 0.0f;
	}

	//指定したフレームを過ぎると存在フラグをfalseにする
	if (m_frame > m_moveFrame)
	{
		m_isExist = false;
	}

	//当たり判定の設定
	m_col.SetCircle(m_pos.x, m_pos.y, m_radius + kColOffset);
}

void EnemyBurst::Draw()
{
	//存在フラグがfalseのときは処理をしない
	if (!m_isExist)
	{
		return;
	}

	//移動フレームを超えるまでは本体を描画する
	if (m_frame <= m_moveFrame)
	{
		//本体の四角の部分の描画
		DrawQuadrangle(
			static_cast<int>(m_pos.x - m_offset.x), static_cast<int>(m_pos.y - m_offset.y),
			static_cast<int>(m_pos.x + m_offset.y), static_cast<int>(m_pos.y - m_offset.x),
			static_cast<int>(m_pos.x + m_offset.x), static_cast<int>(m_pos.y + m_offset.y),
			static_cast<int>(m_pos.x - m_offset.y), static_cast<int>(m_pos.y + m_offset.x),
			m_color,
			true
		);

		//本体の円の部分の描画
		DrawCircle(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y), m_radius, m_color, true);

#ifdef _DEBUG
		//m_colRect.DrawRectCircle(0xffffff, false);
		m_col.DrawColCircle(0xffffff);
#endif
	}
}

void EnemyBurst::CreateBurstedCircle(Vec2 pos, Vec2 vec)
{
	//ここで作るだけ作ってUpdateやDrawはGamePlayingSceneに渡してしまう
	EnemyBurstedCircle* enemy = new EnemyBurstedCircle;
	enemy->Init(pos, vec);
	m_pStage->AddEnemy(enemy);
}
