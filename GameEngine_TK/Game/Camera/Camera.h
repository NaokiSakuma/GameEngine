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
	//setter
	//���_���W
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);
	//�ڕW���W
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);
	//�J�����̏�����x�N�g��
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);
	//������������p
	void SetFloV(float floV);
	//�A�X�y�N�g��
	void SetAspect(float aspect);
	//�j�A�N���b�v
	void SetNearClip(float nearclip);
	//�t�@�[�N���b�v
	void SetFarClip(float farclip);

	//getter
	//�r���[�s��̎擾
	const DirectX::SimpleMath::Matrix& Camera::GetViewMatrix() const;
	//�ˉe�s��̎擾
	const DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix() const;
	//�r���{�[�h�s��
	const DirectX::SimpleMath::Matrix& GetBillboard() const;
	//Y�����̃r���{�[�h�s��
	const DirectX::SimpleMath::Matrix& GetBillboardAxisY() const;
	//�r���{�[�h�s��̌v�Z
	void Billboard();
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
	//�r���{�[�h�s��
	DirectX::SimpleMath::Matrix m_billboard;
	//Y�����̃r���{�[�h�s��
	DirectX::SimpleMath::Matrix m_billboardAxisY;
};
