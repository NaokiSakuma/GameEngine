//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Follow.cpp
//!
//! @brief  ���@�ɒǏ]����J�����N���X�̃\�[�X�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Follow.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//�ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
	, m_targetPos(Vector3::Zero)
	, m_targetAngle(0.0f)
{
}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FollowCamera::Update()
{
	//�J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;
	//�Q�Ɠ_���W���v�Z
	refpos = m_targetPos + Vector3(0, 2, 0);
	//���@����J�������W�ւ̍���
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	//���@�̌��ɉ�荞�ނ��߂̉�]�s��
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	//�J�����ւ̃x�N�g�����v�Z
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//�J�������W���v�Z
	eyepos = refpos + cameraV;
	//�J�����ɏ����Z�b�g
	SetEyePos(eyepos);
	SetRefPos(refpos);
	//���N���X�̍X�V
	Camera::Update();
}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_targetAngle = targetangle;
}
