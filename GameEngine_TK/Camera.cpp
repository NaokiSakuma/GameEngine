//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Camera.cpp
//!
//! @brief  �J�����N���X�̃\�[�X�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Camera::Camera(int width, int height)
	:m_eyepos(0,0,5)
	,m_refpos(0,0,0)
	,m_upvec(0,1,0)
	,m_floV(XMConvertToRadians(60.0f))
	,m_aspect(static_cast<float>(width) / height)
	,m_nearclip(0.1f)
	,m_farclip(1000.0f)
	,m_view(Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec))
	,m_proj(Matrix::CreatePerspectiveFieldOfView(m_floV,m_aspect,m_nearclip,m_farclip))
{
	//�r���[�s��̐���
	//�ˉe�s��̐���
}

//----------------------------------------------------------------------
//! @brief �f�X�g���N�^
//----------------------------------------------------------------------
Camera::~Camera()
{

}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::Update()
{
	//�r���[�s��̐���
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//�ˉe�s��̐���
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_floV, m_aspect, m_nearclip, m_farclip);

}

//----------------------------------------------------------------------
//! @brief �r���[�s��̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �r���[�s��
//----------------------------------------------------------------------
DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
{
	return m_view;
}

//----------------------------------------------------------------------
//! @brief �ˉe�s��̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �ˉe�s��
//----------------------------------------------------------------------
DirectX::SimpleMath::Matrix Camera::GetProjectionMatrix()
{
	return m_proj;
}

//----------------------------------------------------------------------
//! @brief ���_���W�̃Z�b�g
//!
//! @param[in] ���_���W
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetEyePos(DirectX::SimpleMath::Vector3 eyepos)
{
	m_eyepos = eyepos;
}

//----------------------------------------------------------------------
//! @brief �ڕW���W�̃Z�b�g
//!
//! @param[in] �ڕW���W
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetRefPos(DirectX::SimpleMath::Vector3 refpos)
{
	m_refpos = refpos;
}

//----------------------------------------------------------------------
//! @brief �ڕW���W�̃Z�b�g
//!
//! @param[in] �ڕW���W
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetUpVec(DirectX::SimpleMath::Vector3 upvec)
{
	m_upvec = upvec;
}

//----------------------------------------------------------------------
//! @brief ������������p�̃Z�b�g
//!
//! @param[in] ������������p
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetFloV(float floV)
{
	m_floV = floV;
}

//----------------------------------------------------------------------
//! @brief �A�X�y�N�g��̃Z�b�g
//!
//! @param[in] �A�X�y�N�g��
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

//----------------------------------------------------------------------
//! @brief �j�A�N���b�v�̃Z�b�g
//!
//! @param[in] �j�A�N���b�v
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetNearClip(float nearclip)
{
	m_nearclip = nearclip;
}

//----------------------------------------------------------------------
//! @brief �t�@�[�N���b�v�̃Z�b�g
//!
//! @param[in] �t�@�[�N���b�v
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetFarClip(float farclip)
{
	m_farclip = farclip;
}
