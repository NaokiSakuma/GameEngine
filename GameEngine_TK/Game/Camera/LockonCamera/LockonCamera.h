//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   LockonCamera.h
//!
//! @brief  ���b�N�I���J�����̃w�b�_�t�@�C��
//!
//! @date   2017/07/19
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Camera.h"
#include "..\..\Lockon\Lockon.h"
#include "..\FollowCamera\Follow.h"

class LockonCamera : public Camera
{
	//�X�e�C�g
	enum State
	{
		LOCKON,		//���b�N�I���J����
		FOLLOW,		//�Ǐ]�J����
		NUM,		//�J�����̐�
	};
public:
	//����
	const float CAMERA_DISTANCE = 5.f;
	//�؂�ւ�����
	const int SHIFT_FRAME = 120;
	//�J�����̃I�t�Z�b�g
	const float CAMERA_UPOFFSET = 2.f;
	//�R���X�g���N�^
	LockonCamera(int width, int height);
	//�X�V
	void Update() override;
	//setter
	void SetLockon(Lockon* lockon);
protected:
	//�X�e�C�g
	State m_state;
	//���b�N�I��
	Lockon* m_lockon;
	//�Ǐ]�J����
	std::unique_ptr<FollowCamera> m_followCamera;
	//�V�t�g����܂ł̎���
	int m_shiftCount;
};