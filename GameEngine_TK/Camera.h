//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Camera.h
//!
//! @brief  �J�����N���X�̃w�b�_�[�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

class Camera
{
public:
	//�R���X�g���N�^
	Camera(int width, int height);
	//�f�X�g���N�^
	virtual ~Camera();
	//�X�V
	virtual void Update();
	//�r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& Camera::GetViewMatrix();
	//�ˉe�s��̎擾
	const DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix();
	//���_���W�̃Z�b�g
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);
	//�ڕW���W�̃Z�b�g
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);
	//�J�����̏�����x�N�g��
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);
	//������������p�̃Z�b�g
	void SetFloV(float floV);
	//�A�X�y�N�g��̃Z�b�g
	void SetAspect(float aspect);
	//�j�A�N���b�v�̃Z�b�g
	void SetNearClip(float nearclip);
	//�t�@�[�N���b�v�̃Z�b�g
	void SetFarClip(float farclip);
protected:
	//�J�����̈ʒu
	DirectX::SimpleMath::Vector3 m_eyepos;
	//�J�����̌��Ă����
	DirectX::SimpleMath::Vector3 m_refpos;
	//�J�����̏�����x�N�g��
	DirectX::SimpleMath::Vector3 m_upvec;
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	//������������p
	float m_floV;
	//�A�X�y�N�g��
	float m_aspect;
	//�j�A�N���b�v
	float m_nearclip;
	//�t�@�[�N���b�v
	float m_farclip;
	//�ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;
};
