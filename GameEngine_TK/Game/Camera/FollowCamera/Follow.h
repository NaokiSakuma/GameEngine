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

#include <Windows.h>

#include <Keyboard.h>
#include "..\Camera.h"
#include "..\..\Character\Character.h"

class FollowCamera : public Camera
{
public:
	//�J�����Ƃ̋���
	static const float CAMERA_DISTANCE;
	//�R���X�g���N�^
	FollowCamera(int width, int height);
	//�f�X�g���N�^
	~FollowCamera();
	//�X�V
	void Update() override;
	//setter
	//�Ǐ]�Ώ�
	void SetTarget(Character* targetChara);
	//�Ǐ]�Ώۂ̍��W
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);
	//�Ǐ]�Ώۂ̉�]�p
	void SetTargetAngle(const DirectX::SimpleMath::Vector3& targetangle);
	//�L�[�{�[�h
	void SetKeyboard(DirectX::Keyboard* keyboard);
protected:
	//�Ǐ]�Ώ�
	Character* m_target;
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_targetPos;
	//�Ǐ]�Ώۂ̉�]�p
	DirectX::SimpleMath::Vector3 m_targetAngle;
	//�L�[�{�[�h(�V���O���g�����g��)
	DirectX::Keyboard* m_keyboard;
	//�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//���_��؂�ւ���t���O
	bool m_isFPS;
};