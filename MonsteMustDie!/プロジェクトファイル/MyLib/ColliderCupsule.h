#pragma once
#include "ColliderBase.h"
#include "Vec3.h"

namespace MyLib
{
	/// <summary>
	/// �J�v�Z���^�̓����蔻��
	/// </summary>
	class ColliderCupsule : public ColliderBase
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="isTrigger">�g���K�[���ǂ���</param>
		ColliderCupsule(bool isTrigger);

	public:
		float m_size;	//�傫��
		float m_radius;	//���a
	};
}


