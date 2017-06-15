//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Enemy.cpp
//!
//! @brief  �G�̃\�[�X�t�@�C��
//!
//! @date   2017/06/08
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Enemy.h"
#include "..\..\State\EnemyState\EnemyState.h"

//���O���
using namespace DirectX;
using namespace SimpleMath;

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Enemy::Enemy()
	:m_timer(0)
	,m_DistAngle(0)
{

}

//----------------------------------------------------------------------
//! @brief �f�X�g���N�^
//----------------------------------------------------------------------
Enemy::~Enemy()
{

}

//----------------------------------------------------------------------
//! @brief ������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Enemy::Initialize()
{
	//�X�e�C�g���擾(����)
	ChangeState(EnemyState::Stand::GetInstance());

	//�v�f�����Ƀ��T�C�Y����
	m_obj3d.resize(ENEMY_PARTS_NUM);
	//���@�p�[�c�̓ǂݍ���
	m_obj3d[HEAD1].LoadModel(L"Resources/head.cmo");
	m_obj3d[HEAD2].LoadModel(L"Resources/head2.cmo");

	//�q�p�[�c�̐e����̃I�t�Z�b�g(���W�̃Y��)���Z�b�g
	m_obj3d[HEAD2].SetTrans(Vector3(0, 0, 10));

	//�傫��
	m_obj3d[HEAD1].SetScale(Vector3(4, 2, 2));
	m_obj3d[HEAD2].SetScale(Vector3(2, 4, 2));
}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Enemy::Update()
{
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;
		//�ڕW�p�������_���ɒ��I
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		//-90�`90�̗���
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_DistAngle += rnd;
	}
	{
		//���@�̊p�x����]
		Vector3 rot = GetRot();
		//���̊p�x
		float angle = m_DistAngle - rot.y;
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
	//����̎��s
	m_state->Execute(this);

	//�s��̍X�V
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Update();
	}


}

//----------------------------------------------------------------------
//! @brief �`��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Enemy::Render()
{
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Render();
	}
}

//----------------------------------------------------------------------
//! @brief �N���G�C�g�֐�
//!
//! @param[in] �Ȃ�
//!
//! @return Player�̃|�C���^
//----------------------------------------------------------------------
Enemy* Enemy::Create()
{
	Enemy* enemy = new Enemy;
	enemy->Initialize();

	return enemy;
}

//----------------------------------------------------------------------
//! @brief �X�e�C�g�̕ύX
//!
//! @param[in] �ύX��̃X�e�C�g
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Enemy::ChangeState(State* state)
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
void Enemy::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_obj3d[HEAD1].SetScale(scale);
}

//----------------------------------------------------------------------
//! @brief ��]��Setter
//!
//! @param[in] ��]
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_obj3d[HEAD1].SetRot(rot);
}

//----------------------------------------------------------------------
//! @brief ���s�ړ���Setter
//!
//! @param[in] ���s�ړ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	m_obj3d[HEAD1].SetTrans(trans);
}

//----------------------------------------------------------------------
//! @brief �X�P�[�����O��Getter
//!
//! @param[in] �Ȃ�
//!
//! @return �X�P�[�����O
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Enemy::GetScale() const
{
	return m_obj3d[HEAD1].GetScale();
}

//----------------------------------------------------------------------
//! @brief ��]��Getter
//!
//! @param[in] �Ȃ�
//!
//! @return ��]
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Enemy::GetRot() const
{
	return m_obj3d[HEAD1].GetRot();
}

//----------------------------------------------------------------------
//! @brief ���s�ړ���Getter
//!
//! @param[in] �Ȃ�
//!
//! @return ���s�ړ�
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Enemy::GetTrans() const
{
	return m_obj3d[HEAD1].GetTrans();
}

//----------------------------------------------------------------------
//! @brief ���[���h�s���Getter
//!
//! @param[in] �Ȃ�
//!
//! @return ���[���h�s��
//----------------------------------------------------------------------
const DirectX::SimpleMath::Matrix& Enemy::GetWorld() const
{
	return m_obj3d[HEAD1].GetWorld();
}

//----------------------------------------------------------------------
//! @brief �I�u�W�F�N�g��Getter
//!
//! @param[in] �I�u�W�F�N�g�̗v�f�ԍ�
//!
//! @return �I�u�W�F�N�g�̃|�C���^
//----------------------------------------------------------------------
Obj3d* Enemy::GetObj3d(int num)
{
	return &m_obj3d[num];
}
