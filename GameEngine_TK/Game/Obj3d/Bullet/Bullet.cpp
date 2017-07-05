//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Bullet.h
//!
//! @brief  �e�̃w�b�_�[�t�@�C��
//!
//! @date   2017/06/13
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Bullet.h"

//���O���
using namespace DirectX;
using namespace DirectX::SimpleMath;

//----------------------------------------------------------------------
//! @brief �C�j�V�����C�Y
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Bullet::Initialize()
{
	this->LoadModel(L"Resources/head.cmo");
}

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Bullet::Bullet()
{
	m_ObjParent = nullptr;
	m_scale = Vector3(2, 2, 2);
	m_translation = Vector3(0, 0, 0);
	m_UseQuaternion = false;

}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Bullet::Update()
{
	//���o�����������܂��Ă����ϐ�
	Vector3 scale;		//���[���h�X�P�[�����O
	Quaternion rotation;//���[���h��]
	Vector3 translation;//���[���h���W

	//���[���h���W����e�v�f�𒊏o
	m_world.Decompose(scale, rotation, translation);

	//�e�����Ȃ��ꍇ
	if (!m_ObjParent)
	{
		//�e�p�[�c���番���A�Ɨ�������
		m_scale = scale;
		this->SetRotQ(rotation);
		m_translation = translation;

		//�e�ۃp�[�c�̑��x��ݒ�
		Vector3 BulletVel = Vector3(0, 0, -0.5f);
		//�p�[�c�̌����ɂ���č��킹�đ��x�x�N�g������]
		BulletVel = Vector3::Transform(BulletVel, rotation);
		{
			Vector3 pos = m_translation;
			m_translation = pos + BulletVel;
		}
	}

	Obj3d::Update();

}

//----------------------------------------------------------------------
//! @brief �N���G�C�g�֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �e�̃|�C���^
//----------------------------------------------------------------------
Bullet * Bullet::Create()
{
	Bullet* bullet = new Bullet;
	bullet->Initialize();
	return bullet;
}
