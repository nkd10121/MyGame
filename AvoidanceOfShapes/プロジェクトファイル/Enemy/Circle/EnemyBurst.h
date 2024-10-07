#pragma once
#include "EnemyCircleBase.h"
#include <vector>

//�v���g�^�C�v�錾
class StageBase;

//�j�􂷂�G
class EnemyBurst :
    public EnemyCircleBase
{
public:
    EnemyBurst(StageBase* scene);
    ~EnemyBurst();

    /// <summary>
    /// �X�^�[�g�֐�
    /// </summary>
    /// <param name="pos">���W</param>
    /// <param name="vec">�ړ��������</param>
    /// <param name="speed">�ړ�����X�s�[�h</param>
    /// <param name="firstFrame">�ړ����Ȃ���傫���Ȃ鎞��</param>
    /// <param name="secondFrame"></param>
    void Init(Vec2 pos, Vec2 vec, int speed,int firstFrame,int secondFrame)override;

    /// <summary>
    /// �X�V����
    /// </summary>
    void Update()override;

    /// <summary>
    /// �`�揈��
    /// </summary>
    void Draw()override;

    /// <summary>
    /// �j�􂵂���̓G���쐬
    /// </summary>
    /// <param name="pos">���W</param>
    /// <param name="vec">�ړ��������</param>
    void CreateBurstedCircle(Vec2 pos, Vec2 vec);

private:
    //�j���̓G�̏�����StageBase�ɔC���邽�߃A�h���X�����炤
    StageBase* m_pStage;

    //�t���[����
    int m_frame;

    //�傫���Ȃ�t���[����
    int m_bigFrame;
    //�ړ��̃t���[����
    int m_moveFrame;

    //��]����l�p�̍���
    Vec2 m_offset;
    //��]����l�p�̉�]�p�x
    float m_angle;
    //�~�̔��a
    int m_radius;
    //�ړ����x
    float m_growSpeed;

    //��]����l�p�̒��S���璸�_�܂ł̋���
    float m_diagonal;
    //�ړ��x�N�g��
    Vec2 m_vec;

    //�j�􂷂�^�C�~���O�̃t���O
    bool m_isBurst;
    //�j���̓G�̍��W
    Vec2 m_circlePos;
};