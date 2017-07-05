//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerState.cpp
//!
//! @brief  �v���C���[�̃X�e�C�g�̃\�[�X�t�@�C��
//!
//! @date   2017/05/11 (06/04�X�V�FY.Yamada)
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "PlayerState.h"
#include "..\..\Character\Player\Player.h"
#include "..\..\Device\Device.h"

using namespace DirectX;
using namespace SimpleMath;

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
	{
		m_state = new Stand();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief ������
//!
//! @param[in] �L�����N�^�[
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerState::Stand::Initialize()
{
	//�e�L�[�ɃR�}���h�����蓖��
	m_putKeyA     = new PlayerCommands::RotLeftCommand;
	m_putKeyD     = new PlayerCommands::RotRightCommand;
	m_putKeyE     = new PlayerCommands::AssemblyCommand;
	m_putKeyQ     = new PlayerCommands::BreakUpCommand;
	m_putKeySpace = new PlayerCommands::BulletCommand;
	m_putKey1	  = new PlayerCommands::CollisionCommand;
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
	//�L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State key = Device::m_keyboard->GetState();
	Device::m_keyboardTracker.Update(key);

	//W�L�[��������Ă�����
	//S�L�[�������ꂽ��
	if (key.W || key.S)
	{
		//Move��Ԃֈڍs
		character->ChangeState(PlayerState::Move::GetInstance());
	}
	//A�L�[�������ꂽ��
	if (key.A)
	{
		//��]
		m_putKeyA->Execute(character);
	}
	//D�L�[�������ꂽ��
	if (key.D)
	{
		//��]
		m_putKeyD->Execute(character);
	}
	//Space�L�[�������ꂽ��
	if (Device::m_keyboardTracker.pressed.Space)
	{
		//�e����
		m_putKeySpace->Execute(character);
	}
	//Q�L�[�������ꂽ��
	else if (key.Q)
	{
		//���U
		m_putKeyQ->Execute(character);
	}
	//E�L�[�������ꂽ��
	else if (key.E)
	{
		//�W��
		m_putKeyE->Execute(character);
	}
	if (Device::m_keyboardTracker.pressed.D1)
	{
		m_putKey1->Execute(character);
	}
}

//----------------------------------------------------------------------
//! @brief �f�X�g���N�^
//----------------------------------------------------------------------
PlayerState::Stand::~Stand()
{
	delete m_putKeyA;
	delete m_putKeyD;
	delete m_putKeyE;
	delete m_putKeyQ;
	delete m_putKeySpace;
	delete m_putKey1;
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
	{
		m_state = new Move();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief ������
//!
//! @param[in] �L�����N�^�[
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerState::Move::Initialize()
{
	//�e�L�[�ɃR�}���h�����蓖��
	m_putKeyA = new PlayerCommands::RotLeftCommand;
	m_putKeyD = new PlayerCommands::RotRightCommand;
	m_putKeyS = new PlayerCommands::BackCommand;
	m_putKeyW = new PlayerCommands::AdvanceCommand;
	m_putKeySpace = new PlayerCommands::BulletCommand;
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
	//�L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State key = Device::m_keyboard->GetState();
	Device::m_keyboardTracker.Update(key);

	//W�L�[��������Ă�����
	if (key.W)
	{
		//�O�i
		m_putKeyW->Execute(character);
	}
	//S�L�[�������ꂽ��
	if (key.S)
	{
		//���
		m_putKeyS->Execute(character);
	}
	//A�L�[�������ꂽ��
	if (key.A)
	{
		//��]
		m_putKeyA->Execute(character);
	}
	//D�L�[�������ꂽ��
	if (key.D)
	{
		//��]
		m_putKeyD->Execute(character);
	}
	//Space�L�[�������ꂽ��
	if (Device::m_keyboardTracker.pressed.Space)
	{
		//�e����
		m_putKeySpace->Execute(character);
	}
	//W��S��������Ă��Ȃ�������
	if (!key.S && !key.W)
	{
		//Stand��Ԃֈڍs
		character->ChangeState(PlayerState::Stand::GetInstance());
	}
}

//----------------------------------------------------------------------
//! @brief �f�X�g���N�^
//----------------------------------------------------------------------
PlayerState::Move::~Move()
{
	delete m_putKeyA;
	delete m_putKeyD;
	delete m_putKeyW;
	delete m_putKeyS;
	delete m_putKeySpace;
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
