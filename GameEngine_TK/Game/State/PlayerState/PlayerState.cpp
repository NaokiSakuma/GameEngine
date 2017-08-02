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
	m_putKeyZ     = new PlayerCommands::JumpCommand;
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

	//W�L�[��������Ă�����
	//S�L�[�������ꂽ��
	if (Device::m_keyboardState.W || Device::m_keyboardState.S)
	{
		//Move��Ԃֈڍs
		character->ChangeState(PlayerState::Move::GetInstance());
	}
	//A�L�[�������ꂽ��
	if (Device::m_keyboardState.A)
	{
		//��]
		m_putKeyA->Execute(character);
	}
	//D�L�[�������ꂽ��
	if (Device::m_keyboardState.D)
	{
		//��]
		m_putKeyD->Execute(character);
	}
	//Space�L�[�������ꂽ��
	if (Device::m_keyboardTracker->pressed.Space)
	{
		//�e����
		m_putKeySpace->Execute(character);
	}
	//Q�L�[�������ꂽ��
	else if (Device::m_keyboardState.Q)
	{
		//���U
		m_putKeyQ->Execute(character);
	}
	//E�L�[�������ꂽ��
	else if (Device::m_keyboardState.E)
	{
		//�W��
		m_putKeyE->Execute(character);
	}
	//�f�o�b�O�����\��
	if (Device::m_keyboardTracker->pressed.D1)
	{
		m_putKey1->Execute(character);
	}
	//�W�����v
	if (Device::m_keyboardState.Z)
	{
		//Jump��Ԃֈڍs
		character->ChangeState(PlayerState::Jump::GetInstance());
		//�W�����v������
		m_putKeyZ->Execute(character);
	}
	////���b�N�I��
	//if (Device::m_keyboardState.Enter)
	//{
	//	character->ChangeState(PlayerState::LockOn::GetInstance());
	//}
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
	delete m_putKeyZ;
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
	m_putKeyA     = new PlayerCommands::RotLeftCommand;
	m_putKeyD     = new PlayerCommands::RotRightCommand;
	m_putKeyS     = new PlayerCommands::BackCommand;
	m_putKeyW     = new PlayerCommands::AdvanceCommand;
	m_putKeySpace = new PlayerCommands::BulletCommand;
	m_putKeyZ     = new PlayerCommands::JumpCommand;
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
	//W�L�[��������Ă�����
	if (Device::m_keyboardState.W)
	{
		//�O�i
		m_putKeyW->Execute(character);
	}
	//S�L�[�������ꂽ��
	if (Device::m_keyboardState.S)
	{
		//���
		m_putKeyS->Execute(character);
	}
	//A�L�[�������ꂽ��
	if (Device::m_keyboardState.A)
	{
		//��]
		m_putKeyA->Execute(character);
	}
	//D�L�[�������ꂽ��
	if (Device::m_keyboardState.D)
	{
		//��]
		m_putKeyD->Execute(character);
	}
	//Space�L�[�������ꂽ��
	if (Device::m_keyboardTracker->pressed.Space)
	{
		//�e����
		m_putKeySpace->Execute(character);
	}
	//W��S��������Ă��Ȃ�������
	if (!Device::m_keyboardState.S && !Device::m_keyboardState.W && !Device::m_keyboardState.Z)
	{
		//Stand��Ԃֈڍs
		character->ChangeState(PlayerState::Stand::GetInstance());
	}
	//�W�����v
	if (Device::m_keyboardState.Z)
	{
		//Jump��Ԃֈڍs
		character->ChangeState(PlayerState::Jump::GetInstance());
		m_putKeyZ->Execute(character);
	}
	////���b�N�I��
	//if (Device::m_keyboardState.Enter)
	//{
	//	//Lockon��Ԃֈڍs
	//	character->ChangeState(PlayerState::LockOn::GetInstance());
	//}
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
	delete m_putKeyZ;
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

//������
State* PlayerState::Jump::m_state = nullptr;

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
	{
		m_state = new Jump();
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
void PlayerState::Jump::Initialize()
{
	//�e�L�[�ɃR�}���h�����蓖��
	m_putKeyA     = new PlayerCommands::RotLeftCommand;
	m_putKeyD     = new PlayerCommands::RotRightCommand;
	m_putKeyS     = new PlayerCommands::BackCommand;
	m_putKeyW     = new PlayerCommands::AdvanceCommand;
	m_putKeySpace = new PlayerCommands::BulletCommand;
	m_putKeyZ     = new PlayerCommands::JumpCommand;
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
	//W�L�[��������Ă�����
	if (Device::m_keyboardState.W)
	{
		//�O�i
		m_putKeyW->Execute(character);
	}
	//S�L�[�������ꂽ��
	if (Device::m_keyboardState.S)
	{
		//���
		m_putKeyS->Execute(character);
	}
	//A�L�[�������ꂽ��
	if (Device::m_keyboardState.A)
	{
		//��]
		m_putKeyA->Execute(character);
	}
	//D�L�[�������ꂽ��
	if (Device::m_keyboardState.D)
	{
		//��]
		m_putKeyD->Execute(character);
	}
	//Space�L�[�������ꂽ��
	if (Device::m_keyboardTracker->pressed.Space)
	{
		//�e����
		m_putKeySpace->Execute(character);
	}
	//�W�����v���Ă��Ȃ�������stand��Ԃֈڍs
	if (!character->GetIsJump())
	{
		character->ChangeState(PlayerState::Stand::GetInstance());
	}
	//�W�����v
	//if (Device::m_keyboardTracker->pressed.Z)
	//{
		//Jump
		//m_putKeyZ->Execute(character);
	//}
	//W��S��Z��������Ă��Ȃ�������
	//else if (!Device::m_keyboardState.Z)
	//{
	//	int a = 0;
	//	a++;
	//}
	//{
	//	if(key.W || key.S)
	//		character->ChangeState(PlayerState::Move::GetInstance());
	//	//Stand��Ԃֈڍs
	//	if (!key.W && !key.S)
	//		character->ChangeState(PlayerState::Stand::GetInstance());
	//}
	//else //if(!Device::m_keyboardTracker.pressed.Z)
	//{
	//	//character->ChangeState(PlayerState::Stand::GetInstance());
	//}
	//���b�N�I��
	//if (Device::m_keyboardState.Enter)
	//{
	//	character->ChangeState(PlayerState::LockOn::GetInstance());
	//}
}

//----------------------------------------------------------------------
//! @brief �f�X�g���N�^
//----------------------------------------------------------------------
PlayerState::Jump::~Jump()
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
void PlayerState::Jump::Dispose()
{
	if (m_state)
		delete m_state;
}

////������
//State*  PlayerState::LockOn::m_state = nullptr;
//
////----------------------------------------------------------------------
////! @brief �f�X�g���N�^
////----------------------------------------------------------------------
//PlayerState::LockOn::~LockOn()
//{
//	delete m_putKeyA;
//	delete m_putKeyD;
//	delete m_putKeyW;
//	delete m_putKeyS;
//	delete m_putKeySpace;
//	delete m_putKeyZ;
//	delete m_putKeyEnter;
//
//}
//
////----------------------------------------------------------------------
////! @brief �C���X�^���X�̎擾
////!
////! @param[in] �Ȃ�
////!
////! @return �Ȃ�
////----------------------------------------------------------------------
//State * PlayerState::LockOn::GetInstance()
//{
//	if (!m_state)
//	{
//		m_state = new LockOn();
//		m_state->Initialize();
//	}
//	return m_state;
//}
//
////----------------------------------------------------------------------
////! @brief ������
////!
////! @param[in] �L�����N�^�[
////!
////! @return �Ȃ�
////----------------------------------------------------------------------
//void PlayerState::LockOn::Initialize()
//{
//	m_putKeyA = new PlayerCommands::RotLeftCommand;
//	m_putKeyD = new PlayerCommands::RotRightCommand;
//	m_putKeyS = new PlayerCommands::BackCommand;
//	m_putKeyW = new PlayerCommands::AdvanceCommand;
//	m_putKeySpace = new PlayerCommands::BulletCommand;
//	m_putKeyZ = new PlayerCommands::JumpCommand;
//	m_putKeyEnter = new PlayerCommands::LockonCommand;
//}
//
////----------------------------------------------------------------------
////! @brief ����̎��s
////!
////! @param[in] �L�����N�^�[
////!
////! @return �Ȃ�
////----------------------------------------------------------------------
//void PlayerState::LockOn::Execute(Character * character)
//{
//	//W�L�[��������Ă�����
//	if (Device::m_keyboardState.W)
//	{
//		//�O�i
//		m_putKeyW->Execute(character);
//	}
//	//S�L�[�������ꂽ��
//	if (Device::m_keyboardState.S)
//	{
//		//���
//		m_putKeyS->Execute(character);
//	}
//	//A�L�[�������ꂽ��
//	if (Device::m_keyboardState.A)
//	{
//		//��]
//		m_putKeyA->Execute(character);
//	}
//	//D�L�[�������ꂽ��
//	if (Device::m_keyboardState.D)
//	{
//		//��]
//		m_putKeyD->Execute(character);
//	}
//	//Space�L�[�������ꂽ��
//	if (Device::m_keyboardTracker->pressed.Space)
//	{
//		//�e����
//		m_putKeySpace->Execute(character);
//	}
//	//W��S��Z��������Ă��Ȃ�������
//	if (!Device::m_keyboardState.S && !Device::m_keyboardState.W && !Device::m_keyboardState.Z &&!Device::m_keyboardState.Enter)
//	{
//		//Stand��Ԃֈڍs
//		character->ChangeState(PlayerState::Stand::GetInstance());
//	}
//	//�W�����v
//	if (Device::m_keyboardTracker->pressed.Z)
//	{
//		//Jump
//		m_putKeyZ->Execute(character);
//	}
//	//���b�N�I��
//	if (Device::m_keyboardState.Enter)
//	{
//		m_putKeyEnter->Execute(character);
//	}
//	else
//
//}
//
////----------------------------------------------------------------------
////! @brief �I�u�W�F�N�g�̔j��
////!
////! @param[in] �Ȃ�
////!
////! @return �Ȃ�
////----------------------------------------------------------------------
//void PlayerState::LockOn::Dispose()
//{
//	if (m_state)
//		delete m_state;
//}
