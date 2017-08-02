//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.cpp
//!
//! @brief  �v���C���[�̃\�[�X�t�@�C��
//!
//! @date   2017/05/11 
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Player.h"
#include "..\..\State\PlayerState\PlayerState.h"
#include "..\..\Device\Device.h"
#include "..\..\Game.h"
#include "..\..\DebugFont\DebugFont.h"
#include "..\..\ADX2Le\ADX2Le.h"
#include "..\..\CueSheet\CueSheet_0.h"

//���O���
using namespace DirectX;
using namespace SimpleMath;

const int Player::BULLETCOUNT = 10;
//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Player::Player()
	:m_nextFireTimer(0)
	, m_bulletCount(0)
	,m_invincibleTime(0)
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
	//�X�e�C�g���擾(����)
	ChangeState(PlayerState::Stand::GetInstance());
	//�v�f�����Ƀ��T�C�Y����
	m_obj3d.resize(PLAYER_PARTS_NUM);
	//���@�p�[�c�̓ǂݍ���
	m_obj3d[HEAD1].LoadModel(L"Resources/Fighter.cmo");
	//�e�q�֌W
	//m_obj3d[HEAD9].SetObjParent(&m_obj3d[HEAD1]);
	m_obj3d[HEAD1].SetTrans(Vector3(-10, 5, 0));
	//�q�p�[�c�̐e����̃I�t�Z�b�g(���W�̃Y��)���Z�b�g

	//�傫��
	m_obj3d[HEAD1].SetScale(Vector3(2, 2, 2));

	//�����蔻��
	{
		m_collisionNode = new CollisionNode::SphereNode();
		m_collisionNode->Initialize();
		m_collisionNode->SetParent(&m_obj3d[HEAD1]);
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
void Player::Update()
{
	//����̎��s
	m_state->Execute(this);
	//���b�N�I���؂�ւ��{�^��
	if (Device::m_keyboardTracker->pressed.Enter)
	{
		Lockon* lockon = Game::GetInstance()->GetLockon();
		//���b�N�I�������Ă���ꍇ
		if (lockon->IsLockOn())
		{
			//���b�N�I������
			lockon->ReleaseLockon();
		}
		else
		{
			//���b�N�I���J�n
			lockon->ToLockon();
		}
	}
	
	Lockon* lockon = Game::GetInstance()->GetLockon();
	//���b�N�I�����Ă�����
	if (lockon->IsLockOn())
	{
		ControlLockon();
	}

	//�s��̍X�V
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Update();
	}
	//�e�ۗp�̓����蔻��̍X�V
	m_collisionNode->Update();

	{
		//float angle = GetRot().y;
		//Matrix rotmat = Matrix::CreateRotationY(angle);
		//Vector3 moveV(0, 0, -0.1f);
		//moveV = Vector3::TransformNormal(moveV, rotmat);
		//if (GetRot().x != 0)
		//{
		//	float angleX = GetRot().x;
		//	Matrix rotmatX = Matrix::CreateRotationX(angleX);
		//	Vector3 moveVec(0, -0.1f, 0);
		//	moveVec = Vector3::TransformNormal(moveVec, rotmat);
		//	Vector3 pos = GetTrans();
		//	SetTrans(pos + moveV * moveVec);
		//}
		//else
		//{
		//	Vector3 pos = GetTrans();
		//	SetTrans(pos + moveV);
		//}
		// ���݂̍��W�E��]�p���擾
		Vector3 trans = m_obj3d[HEAD1].GetTrans();
		// �ړ��x�N�g��(Z���W�O�i)
		SimpleMath::Vector3 moveV(0, 0, -0.1f);
		// �ړ��x�N�g������]����
		moveV = Vector3::TransformNormal(moveV, m_obj3d[HEAD1].GetWorld());
		// �ړ�
		trans += moveV;
		// �ړ��������W�𔽉f
		m_obj3d[HEAD1].SetTrans(trans);
	}
	////�������ł����
	//if (m_isJump)
	//{
	//	//�d��
	//	m_velocity.y -= GRAVITY_ACC;
	//	//�d�͂̌��E���x
	//	if (m_velocity.y < -JUMP_SPEED_MAX)
	//	{
	//		m_velocity.y = -JUMP_SPEED_MAX;
	//	}
	//}
	//{//���x�ɂ��ړ�
	//	Vector3 trans = GetTrans();
	//	trans += m_velocity;
	//	SetTrans(trans);

	//}

	{//�e�X�g
		m_bulletCount++;
		if (Device::m_keyboardState.Space)
			ParentFreed();
	}
	//�e�̍X�V
	for (auto& itr : m_bullet)
		itr->Update();

	for (std::vector<std::unique_ptr<Bullet>>::iterator itr = m_bullet.begin(); itr != m_bullet.end();)
	{
		if ((*itr)->GetDeleteCount() > 120)
		{
			itr = m_bullet.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	if (!Device::m_keyboardState.A && !Device::m_keyboardState.D && !lockon->IsLockOn())
	{
		Vector3 angle = this->GetRot();
		if (angle.z > 0.01f)
		{
			this->SetRot(Vector3(angle.x,angle.y,angle.z - 0.03f));
		}
		else if (angle.z < -0.01f)
		{
			this->SetRot(Vector3(angle.x, angle.y, angle.z + 0.03f));
		}
	}
	if (!Device::m_keyboardState.W && !Device::m_keyboardState.S && !lockon->IsLockOn())
	{
		Vector3 angle = this->GetRot();
		if (angle.x > 0.01f)
		{
			this->SetRot(Vector3(angle.x - 0.01f, angle.y, angle.z ));
		}
		else if (angle.x < -0.01f)
		{
			this->SetRot(Vector3(angle.x + 0.01f, angle.y, angle.z));
		}
	}

	{//���G���ԗp
		if (m_invincibleTime > 0)
		{
			lockon->ReleaseLockon();
			m_invincibleTime--;
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
void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		if (m_invincibleTime > 0)
		{
			if (m_invincibleTime % 15 == 0)
				it->Render();
		}
		else if(m_invincibleTime == 0)
			it->Render();
		
	}
	//�e�̕`��
	for (auto& itr : m_bullet)
		itr->Render();

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
	for (int i = 0; i < BULLETNUM; i++)
	{
		m_bullet[i]->SetObjParent(&m_obj3d[HEAD1]);
		m_bullet[i]->SetScale(Vector3(0.5f, 0.5f, 0.5f));
		m_bullet[i]->SetRot(Vector3(0, 0, 0));
		m_bullet[i]->SetTrans(Vector3(0, 0.1f, 0));
	}
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

//----------------------------------------------------------------------
//! @brief �e�𔭎˂���
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Player::ParentFreed()
{

	if (m_bulletCount >= BULLETCOUNT)
	{
		m_bullet.push_back(std::move(Bullet::Create(this->GetTrans(),this->GetRot(),this->GetWorld(),true)));
		m_bulletCount = 0;
		ADX2Le::Play(CRI_CUESHEET_0_SHOT1);
	}
}

void Player::DeleteBullet(int i)
{
	m_bullet.erase(m_bullet.begin() + i);
}

void Player::ControlLockon()
{
	Lockon* lockon = Game::GetInstance()->GetLockon();
	assert(lockon);
	assert(lockon->IsLockOn());
	Enemy* enemy = lockon->SearchEnemy();
	if (!enemy) return;

	{
		Vector3 player2enemy = enemy->GetTrans() - this->GetTrans();
		float rotX = atan2f(player2enemy.y, player2enemy.x);
		float rotY = atan2f(-player2enemy.x, -player2enemy.z);
		if(rotX > -XM_PIDIV2)
			m_obj3d[HEAD1].SetRot(Vector3(rotX, rotY, 0));
		else if (rotX < -XM_PIDIV2)
			m_obj3d[HEAD1].SetRot(Vector3(-rotX - XMConvertToRadians(180), rotY, 0));

	}

	// ���E�ړ�
	if (Device::m_keyboardState.A)
	{
		// ���݂̍��W�E��]�p���擾
		Vector3 trans = m_obj3d[HEAD1].GetTrans();
		float rot_y = m_obj3d[HEAD1].GetRot().y;
		// �ړ��x�N�g��(Z���W�O�i)
		SimpleMath::Vector3 moveV(-0.1f, 0, 0);
		Matrix rotm = Matrix::CreateRotationY(rot_y);
		// �ړ��x�N�g������]����
		moveV = Vector3::TransformNormal(moveV, rotm);
		// �ړ�
		trans += moveV;
		// �ړ��������W�𔽉f
		m_obj3d[HEAD1].SetTrans(trans);
	}
	if (Device::m_keyboardState.D)
	{
		// ���݂̍��W�E��]�p���擾
		Vector3 trans = m_obj3d[HEAD1].GetTrans();
		float rot_y = m_obj3d[HEAD1].GetRot().y;
		// �ړ��x�N�g��(Z���W�O�i)
		SimpleMath::Vector3 moveV(0.1f, 0, 0);
		Matrix rotm = Matrix::CreateRotationY(rot_y);
		// �ړ��x�N�g������]����
		moveV = Vector3::TransformNormal(moveV, rotm);
		// �ړ�
		trans += moveV;
		// �ړ��������W�𔽉f
		m_obj3d[HEAD1].SetTrans(trans);
	}

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


const int Player::GetBulletNum() const
{
	return static_cast<int>(m_bullet.size());
}

const std::unique_ptr<Bullet>& Player::GetBullet(int i) const
{
	return std::move(m_bullet[i]);
}
