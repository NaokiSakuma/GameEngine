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
#include "..\..\DebugFont\DebugFont.h"
#include <ctime>
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
void Bullet::Initialize(DirectX::SimpleMath::Vector3 trans, DirectX::SimpleMath::Vector3 rot ,DirectX::SimpleMath::Matrix world, bool isPlayer)
{
	this->LoadModel(L"Resources/head.cmo");

	{//�e�{��
		this->SetTrans(trans);
		this->SetRot(rot);
		if(!isPlayer)
			this->SetScale(Vector3(10, 10, 10));
		Obj3d::Update();
		//Vector3 transVec = trans;
		m_translation = trans;
		srand(static_cast<unsigned int>(time(nullptr)));

		if (isPlayer)
			m_bulletVel = Vector3(0, 0, -1.5f);
		else
		{
			m_bulletVel = Vector3(x, y, z);
		}
		m_bulletVel = Vector3::TransformNormal(m_bulletVel, m_world);
	}
	{//�e�̓����蔻��
		m_collisionNodeBullet = new CollisionNode::SphereNode(); //std::make_unique<CollisionNode::SphereNode>();
		//�e�ۗp�̓����蔻��m�[�h�̐ݒ�
		m_collisionNodeBullet->Initialize();
		//�e�p�[�c��ݒ�
		m_collisionNodeBullet->SetParent(this);
		//�X�P�[����ς���
		m_collisionNodeBullet->SetLocalRadius(0.3f);
		//�I�t�Z�b�g
		m_collisionNodeBullet->SetTrans(Vector3(0, 0.2f, 0));
	}
}

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Bullet::Bullet()
	:m_deleteCount(0)
	//,m_enemy(nullptr)
{
	m_ObjParent = nullptr;
	m_scale = Vector3(0.5f, 0.5f, 0.5f);
	m_translation = Vector3(0, 0, 0);
	m_UseQuaternion = false;
	x = (rand() % 21 - 10) / 1000.0f;
	y = (rand() % 21 - 10) / 1000.0f;
	z = (rand() % 21 - 10) / 1000.0f;
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
	////���o�����������܂��Ă����ϐ�
	//Vector3 scale;		//���[���h�X�P�[�����O
	//Quaternion rotation;//���[���h��]
	//Vector3 translation;//���[���h���W

	////���[���h���W����e�v�f�𒊏o
	//m_world.Decompose(scale, rotation, translation);

	//�e�����Ȃ��ꍇ
	if (!m_ObjParent)
	{
		////�e�p�[�c���番���A�Ɨ�������
		//m_scale = scale;
		//this->SetRotQ(rotation);
		//m_translation = translation;

		////�e�ۃp�[�c�̑��x��ݒ�
		//Vector3 BulletVel = Vector3(0, 0, -0.5f);
		////�p�[�c�̌����ɂ���č��킹�đ��x�x�N�g������]
		//BulletVel = Vector3::TransformNormal(BulletVel, m_world);
		{
			Vector3 pos = m_translation;
			m_translation = pos + m_bulletVel;
		}
	}
	Obj3d::Update();
	m_collisionNodeBullet->Update();
	m_deleteCount++;
}

void Bullet::Render()
{
	m_collisionNodeBullet->Render();
	DebugFont font;
}

//----------------------------------------------------------------------
//! @brief �N���G�C�g�֐�
//!
//! @param[in] �Ȃ�
//!
//! @return �e�̃|�C���^
//----------------------------------------------------------------------
std::unique_ptr<Bullet> Bullet::Create(DirectX::SimpleMath::Vector3 trans, DirectX::SimpleMath::Vector3 rot , DirectX::SimpleMath::Matrix world, bool isPlayer)
{
	std::unique_ptr<Bullet> bullet(new Bullet);
	bullet->Initialize(trans,rot, world, isPlayer);
	return bullet;
}
