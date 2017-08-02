//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Lockon.h
//!
//! @brief  ���b�N�I���̃w�b�_�t�@�C��
//!
//! @date   2017/07/13
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Lockon.h"
#include "..\State\PlayerState\PlayerState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Lockon::Lockon()
	:m_player(nullptr)
	,m_enemy(nullptr)
	,m_lockOnSerial(0)
	,m_isLockOn(false)
{
}

//----------------------------------------------------------------------
//! @brief �f�X�g���N�^
//----------------------------------------------------------------------
Lockon::~Lockon()
{
	delete m_player;
	m_player = nullptr;
}

//----------------------------------------------------------------------
//! @brief ���b�N�I��
//!
//! @param[in] �Ȃ�
//!
//! @return ���b�N�I�����Ă��邩
//----------------------------------------------------------------------
bool Lockon::ToLockon()
{
	//���@���Z�b�g����Ă��Ȃ��ꍇ
	if (!m_player) return false;
	//�G���Z�b�g����Ă��Ȃ��ꍇ
	if (!m_enemy) return false;
	//�G���S��
	if (m_enemy->size() == 0) return false;
	//�ꎞ�I�ȏ��
	struct Temporary
	{
		//����
		float distance;
		//�z��̒��ł̏���
		int index;
		//�p�x
		float delta;
	};
	//���@�̍��W
	const Vector3& playerPos = m_player->GetTrans();

	Temporary tmp;
	//�\���ɑ傫������
	tmp.distance = 1.0e5f;
	tmp.index = -1;
	tmp.delta = 180;
	//���ׂĂ̓G�ɂ���
	int enemyNum = (int)m_enemy->size();
	for (int i = 0; i < enemyNum; i++)
	{
		{
			////�v���C���[�̌���
			//float playerRotY = m_player->GetRot().y;
			////���_�̐ݒ�
			//const Matrix& rotmat = Matrix::CreateRotationY(playerRotY);
			////�����Ă���x�N�g��
			//Vector3 faceVec(0, 0, -50.f);
			////�����x�N�g��
			//faceVec = Vector3::TransformNormal(faceVec, rotmat);
			//Enemy* enemy = (*m_enemy)[i].get();
			//const Vector3& enemyPos = enemy->GetTrans();
			////���@�ƓG�̋���
			//float distance = Vector3::Distance(faceVec, enemyPos);
			////��������ԋ߂�
			//if (distance < tmp.distance)
			//{
			//	tmp.distance = distance;
			//	tmp.index = i;
			//}

			//Enemy* enemy = (*m_enemy)[i].get();
			////�G�ƃv���C���[�Ƃ̊p�x
			//float player2enemyAngle = atan2f(playerPos.x - enemy->GetTrans().x, playerPos.z - enemy->GetTrans().z);
			////�����̌���
			//float playerRotY = m_player->GetRot().y;
			//if(player2enemyAngle - playerRotY)

			Enemy* enemy = (*m_enemy)[i].get();
			Vector3 enemyPos = enemy->GetTrans();
			//�����̌����̃x�N�g��
			float angle = m_player->GetRot().y;
			Matrix rotmatY = Matrix::CreateRotationY(angle);
			Vector3 moveV(0, 0, -0.1f);
			moveV = Vector3::TransformNormal(moveV, rotmatY);
			moveV.Normalize();
			//�G�ƃv���C���[�Ƃ̋����̃x�N�g��
			Vector3 player2enemyDistance = enemy->GetTrans() - m_player->GetTrans();
			player2enemyDistance.Normalize();
			//����
			float cosine = moveV.Dot(player2enemyDistance);
			//�p�x
			float delta = acosf(cosine);
			if (delta < XMConvertToRadians(20))
			{
				float distance = Vector3::Distance(playerPos, enemyPos);
				if (distance < 50.f)
				{
					if (delta < tmp.delta)
					{
						tmp.delta = delta;
						if (distance < tmp.distance)
						{
							tmp.distance = distance;
							tmp.index = i;
						}
					}
				}
			}
		}
	}
	//�z��̗v�f�O�A�N�Z�X�����
	if (!(0 <= tmp.index) || !(tmp.index < enemyNum))
		return false;

	//��ԋ߂��G�����b�N�I��
	m_lockOnSerial = (*m_enemy)[tmp.index]->GetSerialNum();
	m_isLockOn = true;
	return true;
}

//----------------------------------------------------------------------
//! @brief ���b�N�I������������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Lockon::ReleaseLockon()
{
	m_isLockOn = false;
	m_lockOnSerial = 0;
}

//----------------------------------------------------------------------
//! @brief �G��T��
//!
//! @param[in] �Ȃ�
//!
//! @return �G
//----------------------------------------------------------------------
Enemy * Lockon::SearchEnemy()
{
	Enemy* lockingEnemy = nullptr;
	//���b�N�I�������ǂ���
	if (IsLockOn())
	{
		//���b�N�I�����̓G�����݂��Ă��邩�m�F
		int enemyNum = (int)m_enemy->size();
		for (int i = 0; i < enemyNum; i++)
		{
			Enemy* enemy = (*m_enemy)[i].get();
			//�����������ǂ���
			if (enemy->GetSerialNum() == m_lockOnSerial)
			{
				lockingEnemy = enemy;
				break;
			}
		}
	}
	if (!lockingEnemy)
		ReleaseLockon();
	return lockingEnemy;
}

//----------------------------------------------------------------------
//! @brief �v���C���[��setter
//!
//! @param[in] �v���C���[
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Lockon::SetPlayer(Player * player)
{
	m_player = player;
}

//----------------------------------------------------------------------
//! @brief �G��setter
//!
//! @param[in] �G
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Lockon::SetEnemy(std::vector<std::unique_ptr<Enemy>>* enemy)
{
	m_enemy = enemy;
}

//----------------------------------------------------------------------
//! @brief �v���C���[��getter
//!
//! @param[in] �Ȃ�
//!
//! @return �v���C���[
//----------------------------------------------------------------------
Player * Lockon::GetPlayer()
{
	return m_player;
}

//----------------------------------------------------------------------
//! @brief ���b�N�I�����Ă��邩
//!
//! @param[in] �Ȃ�
//!
//! @return ���b�N�I�����Ă��邩
//----------------------------------------------------------------------
bool Lockon::IsLockOn()
{
	return m_isLockOn;
}
