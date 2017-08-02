//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   LockonCamera.cpp
//!
//! @brief  ���b�N�I���J�����̃\�[�X�t�@�C��
//!
//! @date   2017/07/19
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "LockonCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

LockonCamera::LockonCamera(int width, int height)
	: Camera(width, height)
	, m_state(State::FOLLOW)
	, m_followCamera(std::make_unique<FollowCamera>(width, height))
	, m_shiftCount(0)
{
}

void LockonCamera::Update()
{
	//�Ǐ]�J�����̃^�[�Q�b�g�����@�ɐݒ�
	m_followCamera->SetTarget(m_lockon->GetPlayer());
	//�Ǐ]�J�����̍X�V
	//m_followCamera->Update();

	//���b�N�I�����Ă��邩�ǂ����̃t���O
	bool isLockon = m_lockon->IsLockOn();
	//�G
	Enemy* enemy = nullptr;

	//���b�N�I�����Ă�����
	if (isLockon)
	{
		enemy = m_lockon->SearchEnemy();
		//�G�����Ȃ�������
		if (!enemy)
			isLockon = false;
	}

	//�X�e�C�g�̐؂�ւ�
	switch (m_state)
	{
		//���b�N�I���J����
	case LOCKON:
		//���b�N�I�����Ă��Ȃ�������
		if (!isLockon)
			m_state = FOLLOW;
		break;
	case FOLLOW:
	default:		
		//���b�N�I�����Ă�����
		if (isLockon)
		{
			m_state = LOCKON;
			m_shiftCount = SHIFT_FRAME;
		}
		break;
	}

	//���b�N�I�����̏ꍇ
	if (isLockon)
	{
		//�O�̎��_���W
		Vector3 eyeposPre = m_eyepos;
		//�O�̖ڕW���W
		Vector3 refposPre = m_refpos;
		//�v���C���[
		Player* player = m_lockon->GetPlayer();
		//�G
		Enemy* enemy = m_lockon->SearchEnemy();
		//�G������ꍇ
		if (enemy)
		{
			//�v���C���[�̍��W
			const Vector3& playerTrans = player->GetTrans();
			//�G�̍��W
			const Vector3& enemyTrans = enemy->GetTrans();
			//���_���W
			Vector3 eyepos;
			//�ڕW���W
			Vector3 refpos;

			//�G���玩�@�̓���ւ̕����x�N�g��
			//���@�ƓG���߂Â��قǏォ�猩���낷��ԂɂȂ�
			Vector3 cameraV = playerTrans + Vector3(0, CAMERA_UPOFFSET, 0) - enemyTrans;
			cameraV.Normalize();
			cameraV *= CAMERA_DISTANCE;

			//���_
			eyepos = playerTrans + cameraV;
			//���ȏ�̍����ɂ���
			if (eyepos.y - playerTrans.y < CAMERA_UPOFFSET)
				eyepos.y = playerTrans.y + CAMERA_UPOFFSET;
			//�Q�Ɠ_
			refpos = enemyTrans;

			//�J�����̈ړ��̕⊮
			if (m_shiftCount > 0)
			{
				//���Ԃ̏I��肪�߂Â��ɂ�ĕ⊮�Ȃ���Ԃɋ߂Â���
				//���_���W�̔�
				float eyeposRatio = 0.95f * m_shiftCount / SHIFT_FRAME;
				//�ڕW���W�̔�
				float refposRatio = 0.80f * m_shiftCount / SHIFT_FRAME;
				eyepos = eyepos + (eyeposPre - eyepos) * eyeposRatio;
				refpos = refpos + (refposPre - refpos) * refposRatio;
				m_shiftCount--;
			}
			this->SetEyePos(eyepos);
			this->SetRefPos(refpos);
			this->SetUpVec(Vector3::UnitY);
		}
		//���b�N�I�����̓G�����݂��Ȃ��ꍇ
		else
			isLockon = false;
	}

	//���b�N�I�����Ă��Ȃ��ꍇ
	if (!isLockon)
	{
		////�O�̎��_���W
		//Vector3 eyeposPre = m_eyepos;
		////�O�̖ڕW���W
		//Vector3 refposPre = m_refpos;
		////���_���W
		//Vector3 eyepos = eyeposPre + ()
	}
	Camera::Update();
}

void LockonCamera::SetLockon(Lockon * lockon)
{
	m_lockon = lockon;
}
