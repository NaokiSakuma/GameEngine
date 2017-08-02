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
using namespace DirectX::SimpleMath;

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
	//�r���{�[�h�s����v�Z
	Billboard();
}

//----------------------------------------------------------------------
//! @brief �r���[�s��̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �r���[�s��
//----------------------------------------------------------------------
const Matrix& Camera::GetViewMatrix() const
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
const Matrix& Camera::GetProjectionMatrix() const
{
	return m_proj;
}

const DirectX::SimpleMath::Matrix & Camera::GetBillboard() const
{
	return m_billboard;
}

const DirectX::SimpleMath::Matrix & Camera::GetBillboardAxisY() const
{
	return m_billboardAxisY;
}

//----------------------------------------------------------------------
//! @brief ���_���W�̃Z�b�g
//!
//! @param[in] ���_���W
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Camera::SetEyePos(const Vector3& eyepos)
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
void Camera::SetRefPos(const Vector3& refpos)
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
void Camera::SetUpVec(const Vector3& upvec)
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

void Camera::Billboard()
{
	//��������
	Vector3 eyeDir = m_refpos - m_eyepos;
	//Y��
	Vector3 axisY = Vector3::UnitY;
	//X��				���������̊O��
	Vector3 axisX = axisY.Cross(eyeDir);
	axisX.Normalize();
	//Z��
	Vector3 axisZ = axisX.Cross(axisY);
	axisZ.Normalize();
	//Y�����̃r���{�[�h�s��
	m_billboardAxisY = Matrix::Identity;
	m_billboardAxisY.m[0][0] = axisX.x;
	m_billboardAxisY.m[0][1] = axisX.y;
	m_billboardAxisY.m[0][2] = axisX.z;
	m_billboardAxisY.m[1][0] = axisY.x;
	m_billboardAxisY.m[1][1] = axisY.y;
	m_billboardAxisY.m[1][2] = axisY.z;
	m_billboardAxisY.m[2][0] = axisZ.x;
	m_billboardAxisY.m[2][1] = axisZ.y;
	m_billboardAxisY.m[2][2] = axisZ.z;

	axisY = eyeDir.Cross(axisX);
	axisY.Normalize();
	eyeDir.Normalize();
	//�r���{�[�h�s��
	m_billboard = Matrix::Identity;
	m_billboard.m[0][0] = axisX.x;
	m_billboard.m[0][1] = axisX.y;
	m_billboard.m[0][2] = axisX.z;
	m_billboard.m[1][0] = axisY.x;
	m_billboard.m[1][1] = axisY.y;
	m_billboard.m[1][2] = axisY.z;
	m_billboard.m[2][0] = eyeDir.x;
	m_billboard.m[2][1] = eyeDir.y;	
	m_billboard.m[2][2] = eyeDir.z;
}
