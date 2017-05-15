//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Follow.h
//!
//! @brief  ���@�ɒǏ]����J�����N���X�̃w�b�_�[�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "Camera.h"

class FollowCamera : public Camera
{
public:
	//�J�����Ƃ̋���
	static const float CAMERA_DISTANCE;
	//�R���X�g���N�^
	FollowCamera(int width, int height);
	//�X�V
	void Update() override;
	//�Ǐ]�Ώۂ̍��W���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);
	//�Ǐ]�Ώۂ̉�]�p���Z�b�g
	void SetTargetAngle(float targetangle);
protected:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	//�Ǐ]�Ώۂ̉�]�p
	float m_targetAngle;
};