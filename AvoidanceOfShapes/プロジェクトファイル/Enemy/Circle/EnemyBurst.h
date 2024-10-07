#pragma once
#include "EnemyCircleBase.h"
#include <vector>

//プロトタイプ宣言
class StageBase;

//破裂する敵
class EnemyBurst :
    public EnemyCircleBase
{
public:
    EnemyBurst(StageBase* scene);
    ~EnemyBurst();

    /// <summary>
    /// スタート関数
    /// </summary>
    /// <param name="pos">座標</param>
    /// <param name="vec">移動する方向</param>
    /// <param name="speed">移動するスピード</param>
    /// <param name="firstFrame">移動しながら大きくなる時間</param>
    /// <param name="secondFrame"></param>
    void Init(Vec2 pos, Vec2 vec, int speed,int firstFrame,int secondFrame)override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update()override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw()override;

    /// <summary>
    /// 破裂した後の敵を作成
    /// </summary>
    /// <param name="pos">座標</param>
    /// <param name="vec">移動する方向</param>
    void CreateBurstedCircle(Vec2 pos, Vec2 vec);

private:
    //破裂後の敵の処理をStageBaseに任せるためアドレスをもらう
    StageBase* m_pStage;

    //フレーム数
    int m_frame;

    //大きくなるフレーム数
    int m_bigFrame;
    //移動のフレーム数
    int m_moveFrame;

    //回転する四角の差分
    Vec2 m_offset;
    //回転する四角の回転角度
    float m_angle;
    //円の半径
    int m_radius;
    //移動速度
    float m_growSpeed;

    //回転する四角の中心から頂点までの距離
    float m_diagonal;
    //移動ベクトル
    Vec2 m_vec;

    //破裂するタイミングのフラグ
    bool m_isBurst;
    //破裂後の敵の座標
    Vec2 m_circlePos;
};