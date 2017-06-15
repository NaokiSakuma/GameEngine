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
const float FollowCamera::CAMERA_DISTANCE = 10.0f;

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
	, m_targetPos(Vector3::Zero)
	, m_targetAngle(0.0f)
	, m_keyboard(nullptr)
	, m_isFPS(false)
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
	//�L�[�{�[�h�̏�Ԃ��擾
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTracker.Update(keystate);
	//C�L�[�������ꂽ��
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		m_isFPS = !m_isFPS;
	}
	//�J�������_���W�A�Q�Ɠ_���W
	Vector3 eyepos, refpos;
	if (m_isFPS)
	{//FPS�J�����̏���
		Vector3 position;
		//�Q�Ɠ_���W���v�Z
		position = m_targetPos + Vector3(0, 0.2f, 0);
		//���@����J�������W�ւ̍���
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		//���@�̌��ɉ�荞�ނ��߂̉�]�s��
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
		//�J�����ւ̃x�N�g�����v�Z
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//�J�������W���v�Z
		//�����i�񂾈ʒu�����_���W
		eyepos = position + cameraV *0.1f;
		//��������i�񂾈ʒu���Q�Ɠ_���W
		refpos = position + cameraV;
	}
	else if (!m_isFPS)
	{//TPS�J�����̏���
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
		//���_�����݈ʒu����ۊǂ���
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
		//�Q�Ɠ_�����݈ʒu����ۊǂ���
		refpos = m_refpos + (refpos - m_refpos) * 0.2f;

	}
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

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}
