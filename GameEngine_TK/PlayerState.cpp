//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerState.cpp
//!
//! @brief  �v���C���[�̃X�e�C�g�̃\�[�X�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "PlayerState.h"

//������
State*  PlayerState::Stand::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief �C���X�^���X�̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
State* PlayerState::Stand::GetInstance()
{
	if (!m_state)
		m_state = new Stand();
	return m_state;
}

//----------------------------------------------------------------------
//! @brief ����̎��s
//!
//! @param[in] �L�����N�^�[
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerState::Stand::Execute(Character * character)
{
}

//----------------------------------------------------------------------
//! @brief �I�u�W�F�N�g�̔j��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerState::Stand::Dispose()
{
	if (m_state)
		delete m_state;
}

//������
State*  PlayerState::Jump::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief �C���X�^���X�̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
State * PlayerState::Jump::GetInstance()
{
	if (!m_state)
		m_state = new Jump();
	return m_state;
}

//----------------------------------------------------------------------
//! @brief ����̎��s
//!
//! @param[in] �L�����N�^�[
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerState::Jump::Execute(Character * character)
{
}

//----------------------------------------------------------------------
//! @brief �I�u�W�F�N�g�̔j��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerState::Jump::Dispose()
{
	if (m_state)
		delete m_state;
}

//������
State*  PlayerState::Move::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief �C���X�^���X�̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
State * PlayerState::Move::GetInstance()
{
	if (!m_state)
		m_state = new Stand();
	return m_state;
}

//----------------------------------------------------------------------
//! @brief ����̎��s
//!
//! @param[in] �L�����N�^�[
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerState::Move::Execute(Character * character)
{
}

//----------------------------------------------------------------------
//! @brief �I�u�W�F�N�g�̔j��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerState::Move::Dispose()
{
	if (m_state)
		delete m_state;
}