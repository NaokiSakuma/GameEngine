//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Boss.cpp
//!
//! @brief  �{�X�̃\�[�X�t�@�C��
//!
//! @date   2017/07/28
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Boss.h"
#include "..\..\Device\Device.h"
#include "..\..\DebugFont\DebugFont.h"
//���O���
using namespace DirectX;
using namespace SimpleMath;

const int Boss::BULLETCOUNT = 30;

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Boss::Boss()
	:m_timer(0)
	, m_DistAngle(0)
	,m_wallCount(0)
	,m_bulletCount(0)
	,m_hp(100)
{

}

//----------------------------------------------------------------------
//! @brief �f�X�g���N�^
//----------------------------------------------------------------------
Boss::~Boss()
{

}

//----------------------------------------------------------------------
//! @brief ������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Boss::Initialize()
{

	//�v�f�����Ƀ��T�C�Y����
	m_obj3d.resize(BOSS_PARTS_NUM);
	//���@�p�[�c�̓ǂݍ���
	m_obj3d[BOSS].LoadModel(L"Resources/boss.cmo");

	//�e�q�֌W

	//�q�p�[�c�̐e����̃I�t�Z�b�g(���W�̃Y��)���Z�b�g
	m_obj3d[BOSS].SetTrans(Vector3(10, 10, 10));

	//�傫��
	//for (int i = 0; i < BOSS_PARTS_NUM; i++)
	{
		m_obj3d[BOSS].SetScale(Vector3(30, 30, 30));
	}
	//m_obj3d[HEAD2].SetScale(Vector3(2, 4, 2));


	//�����蔻��
	{
		m_collisionNode = new CollisionNode::SphereNode();
		m_collisionNode->Initialize();
		m_collisionNode->SetParent(&m_obj3d[BOSS]);
		m_collisionNode->SetTrans(Vector3(0, 0.15f, 0));
		dynamic_cast<CollisionNode::SphereNode*>(m_collisionNode)->SetLocalRadius(0.2f);
	}
	//�e
	{
		m_bullet.resize(0);
	}

}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Boss::Update()
{
	//����̎��s
	//m_state->Execute(this);
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;
		//�ڕW�p�������_���ɒ��I
		float rnd =  (float)rand() / RAND_MAX - 0.5f;
		//-90�`90�̗���
		//rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_DistAngle += rnd;
	}
	{
		//���@�̊p�x����]
		Vector3 rot = GetRot();
		//���̊p�x
		float angle;
		if (rot.y < -3 )
			angle = m_DistAngle - rot.y;
		else
			angle = m_DistAngle - rot.y + XMConvertToRadians(180);
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}
		//���
		rot.y += angle * 0.01f;
		SetRot(rot);
	}
	{//	�@�̂̌����Ă�������ɐi��
		Vector3 trans = m_obj3d[BOSS].GetTrans();
		Vector3 moveV(0, 0, -0.1f);
		Vector3 rotv = m_obj3d[BOSS].GetRot();
		Matrix rotm = Matrix::CreateRotationY(rotv.y);
		moveV = Vector3::TransformNormal(moveV, rotm);
		trans += moveV;
		m_obj3d[BOSS].SetTrans(trans);
	}
	//�s��̍X�V
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Update();
	}
	//�����蔻��
	m_collisionNode->Update();
	{
		if (m_wallCount > 0)
		{
			m_wallCount--;
		}
	}
	{//�e�X�g
		m_bulletCount++;
		//if (Device::m_keyboardState.Space)
		ParentFreed();
	}
	//�e�̍X�V
	for (auto& itr : m_bullet)
		itr->Update();

	for (std::vector<std::unique_ptr<Bullet>>::iterator itr = m_bullet.begin(); itr != m_bullet.end();)
	{
		if ((*itr)->GetDeleteCount() > 300)
		{
			itr = m_bullet.erase(itr);
		}
		else
		{
			itr++;
		}
	}

}

//----------------------------------------------------------------------
//! @brief �`��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Boss::Render()
{
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Render();
	}
	//�e�ۗp�̓����蔻��̕`��
	if (m_isRenderColl)
	{
		//m_collisionNode->Render();
	}
	//m_collisionNode->Render();
	for (auto& itr : m_bullet)
		itr->Render();
}

//----------------------------------------------------------------------
//! @brief �N���G�C�g�֐�
//!
//! @param[in] �Ȃ�
//!
//! @return Player�̃|�C���^
//----------------------------------------------------------------------
std::unique_ptr<Boss> Boss::Create()
{
	//std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	std::unique_ptr<Boss> boss(new Boss());
	boss->Initialize();

	return boss;
}

const int Boss::GetBulletNum() const
{
	return static_cast<int>(m_bullet.size());
}

const std::unique_ptr<Bullet>& Boss::GetBullet(int i) const
{
	return std::move(m_bullet[i]);
}

void Boss::ParentFreed()
{
	if (m_bulletCount >= BULLETCOUNT)
	{
		m_bullet.push_back(std::move(Bullet::Create(this->GetTrans(), this->GetRot(), this->GetWorld(),false)));
		m_bulletCount = 0;
	}

}

void Boss::DeleteBullet(int i)
{
	m_bullet.erase(m_bullet.begin() + i);
}

void Boss::DecreaseHP()
{
	m_hp--;
}

//----------------------------------------------------------------------
//! @brief �X�e�C�g�̕ύX
//!
//! @param[in] �ύX��̃X�e�C�g
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Boss::ChangeState(State* state)
{
	m_state = state;
}

//----------------------------------------------------------------------
//! @brief �X�P�[�����O��Setter
//!
//! @param[in] �X�P�[�����O
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Boss::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_obj3d[BOSS].SetScale(scale);
}

//----------------------------------------------------------------------
//! @brief ��]��Setter
//!
//! @param[in] ��]
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Boss::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_obj3d[BOSS].SetRot(rot);
}

//----------------------------------------------------------------------
//! @brief ���s�ړ���Setter
//!
//! @param[in] ���s�ړ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Boss::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	m_obj3d[BOSS].SetTrans(trans);
}

//----------------------------------------------------------------------
//! @brief �X�P�[�����O��Getter
//!
//! @param[in] �Ȃ�
//!
//! @return �X�P�[�����O
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Boss::GetScale() const
{
	return m_obj3d[BOSS].GetScale();
}

//----------------------------------------------------------------------
//! @brief ��]��Getter
//!
//! @param[in] �Ȃ�
//!
//! @return ��]
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Boss::GetRot() const
{
	return m_obj3d[BOSS].GetRot();
}

//----------------------------------------------------------------------
//! @brief ���s�ړ���Getter
//!
//! @param[in] �Ȃ�
//!
//! @return ���s�ړ�
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Boss::GetTrans() const
{
	return m_obj3d[BOSS].GetTrans();
}

//----------------------------------------------------------------------
//! @brief ���[���h�s���Getter
//!
//! @param[in] �Ȃ�
//!
//! @return ���[���h�s��
//----------------------------------------------------------------------
const DirectX::SimpleMath::Matrix& Boss::GetWorld() const
{
	return m_obj3d[BOSS].GetWorld();
}

//----------------------------------------------------------------------
//! @brief �I�u�W�F�N�g��Getter
//!
//! @param[in] �I�u�W�F�N�g�̗v�f�ԍ�
//!
//! @return �I�u�W�F�N�g�̃|�C���^
//----------------------------------------------------------------------
Obj3d* Boss::GetObj3d(int num)
{
	return &m_obj3d[num];
}
