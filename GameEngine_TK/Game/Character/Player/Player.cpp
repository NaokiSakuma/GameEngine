//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.cpp
//!
//! @brief  �v���C���[�̃\�[�X�t�@�C��
//!
//! @date   2017/05/11 (06/04�X�V�FY.Yamada)
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Player.h"
#include "..\..\State\PlayerState\PlayerState.h"
#include "..\..\Device\Device.h"

//���O���
using namespace DirectX;
using namespace SimpleMath;

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Player::Player()
	:m_nextFireTimer(0)
{
}

//----------------------------------------------------------------------
//! @brief �f�X�g���N�^
//----------------------------------------------------------------------
Player::~Player()
{

}

//----------------------------------------------------------------------
//! @brief ������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::Initialize()
{
	//�e�̐���
	m_bullet = Bullet::Create();
	//�X�e�C�g���擾(����)
	ChangeState(PlayerState::Stand::GetInstance());
	//�v�f�����Ƀ��T�C�Y����
	m_obj3d.resize(PLAYER_PARTS_NUM);
	//���@�p�[�c�̓ǂݍ���
	m_obj3d[HEAD1].LoadModel(L"Resources/head.cmo");
	for (int i = 1; i < PLAYER_PARTS_NUM; i++)
	{
		m_obj3d[i].LoadModel(L"Resources/head2.cmo");
	}
	//�e�q�֌W
	//m_obj3d[HEAD9].SetObjParent(&m_obj3d[HEAD1]);
	m_bullet->SetObjParent(&m_obj3d[HEAD1]);
	//�q�p�[�c�̐e����̃I�t�Z�b�g(���W�̃Y��)���Z�b�g
	m_obj3d[HEAD2].SetTrans(Vector3(0, 0, 2));

	//�傫��
	m_obj3d[HEAD1].SetScale(Vector3(2, 2, 2));
	m_obj3d[HEAD2].SetScale(Vector3(2, 2, 2));


}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::Update()
{
	//����̎��s
	m_state->Execute(this);
	//�s��̍X�V
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Update();
	}
	//�e�̈ړ�
	m_bullet->Update();

	//�e�ɐe�����Ȃ��ꍇ
	if (!m_bullet->GetObjParent())
	{
		m_nextFireTimer++;
		if (m_nextFireTimer > 120)
			//���U
			ResetBullet();
	}
}

//----------------------------------------------------------------------
//! @brief �`��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Render();
	}
	//�e�̕`��
	m_bullet->Render();
}

//----------------------------------------------------------------------
//! @brief �e�ۂ𑕓U
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::ResetBullet()
{
	//
	m_bullet->SetObjParent(&m_obj3d[HEAD1]);
	m_bullet->SetScale(Vector3(2, 2, 2));
	m_bullet->SetRot(Vector3(0, 0, 0));
	m_bullet->SetTrans(Vector3(0, 1, 0));
	m_nextFireTimer = 0;
}

//----------------------------------------------------------------------
//! @brief �N���G�C�g�֐�
//!
//! @param[in] �Ȃ�
//!
//! @return Player�̃|�C���^
//----------------------------------------------------------------------
Player* Player::Create()
{
	Player* player = new Player;
	player->Initialize();

	return player;
}

void Player::ParentFreed()
{
	if (m_bullet->GetObjParent())
		m_bullet->SetObjParent(nullptr);
	else if (!m_bullet->GetObjParent())
		ResetBullet();
}

//----------------------------------------------------------------------
//! @brief �X�e�C�g�̕ύX
//!
//! @param[in] �ύX��̃X�e�C�g
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::ChangeState(State* state)
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
void Player::SetScale(const DirectX::SimpleMath::Vector3& scale)
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
void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
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
void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
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
const DirectX::SimpleMath::Vector3& Player::GetScale() const
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
const DirectX::SimpleMath::Vector3& Player::GetRot() const
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
const DirectX::SimpleMath::Vector3& Player::GetTrans() const
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
const DirectX::SimpleMath::Matrix& Player::GetWorld() const
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
Obj3d* Player::GetObj3d(int num)
{
	return &m_obj3d[num];
}
