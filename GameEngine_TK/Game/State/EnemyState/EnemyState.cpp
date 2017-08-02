//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   EnemyState.cpp
//!
//! @brief  �G�̃X�e�C�g�̃\�[�X�t�@�C��
//!
//! @date   2017/06/12
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "EnemyState.h"
#include "..\..\Character\Enemy\Enemy.h"

using namespace DirectX;
using namespace SimpleMath;

//������
State* EnemyState::Stand::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief �C���X�^���X�̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
State* EnemyState::Stand::GetInstance()
{
	if (!m_state)
	{
		m_state = new Stand();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief ����̎��s
//!
//! @param[in] �L�����N�^�[
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void EnemyState::Stand::Execute(Character* character)
{
	//����
	//Move��Ԃֈڍs
	character->ChangeState(EnemyState::Move::GetInstance());
}

//----------------------------------------------------------------------
//! @brief �I�u�W�F�N�g�̔j��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void EnemyState::Stand::Dispose()
{
	if (m_state)
		delete m_state;
}

//������
State* EnemyState::Move::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief �C���X�^���X�̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
State* EnemyState::Move::GetInstance()
{
	if (!m_state)
	{
		m_state = new Move();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief ����̎��s
//!
//! @param[in] �L�����N�^�[
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void EnemyState::Move::Execute(Character* character)
{
	//����
	float angle = character->GetRot().y;
	Matrix rotmat = Matrix::CreateRotationY(angle);
	Vector3 moveV(0, 0, -0.05f);
	moveV = Vector3::TransformNormal(moveV, rotmat);
	Vector3 pos = character->GetTrans();
	character->SetTrans(pos + moveV);
	for (int i = 1; i < Enemy::ENEMY_PARTS::ENEMY_PARTS_NUM; i++)
	{
		if (character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z > 1.0f ||
			character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z < -1.0f ||
			character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x > 1.0f ||
			character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x < -1.0f
			)
		{
			float angle = character->GetObj3d(i)->GetRot().y;
			Matrix rotmat = Matrix::CreateRotationY(angle);
			Vector3 moveV(0, 0, -0.05f);
			if (character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z > 1.5f ||
				character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z < -1.5f ||
				character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x > 1.5f ||
				character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x < -1.5f)
			{
				moveV = Vector3(0, 0, -0.1f);
			}
			moveV = Vector3::TransformNormal(moveV, rotmat);
			Vector3 pos = character->GetObj3d(i)->GetTrans();
			character->GetObj3d(i)->SetTrans(pos + moveV);
		}
		character->GetObj3d(i)->SetRot(Vector3(0, atan2f((character->GetObj3d(i)->GetTrans().x - character->GetObj3d(i - 1)->GetTrans().x),
			(character->GetObj3d(i)->GetTrans().z - character->GetObj3d(i - 1)->GetTrans().z)), 0));
	}

	//Stand��Ԃֈڍs
	//character->ChangeState(EnemyState::Stand::GetInstance());
}

//----------------------------------------------------------------------
//! @brief �I�u�W�F�N�g�̔j��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void EnemyState::Move::Dispose()
{
	if (m_state)
		delete m_state;
}