//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Camera.cpp
//!
//! @brief  カメラクラスのソースファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
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
	//ビュー行列の生成
	//射影行列の生成
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
Camera::~Camera()
{

}

//----------------------------------------------------------------------
//! @brief 更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::Update()
{
	//ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);
	//射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_floV, m_aspect, m_nearclip, m_farclip);

}

//----------------------------------------------------------------------
//! @brief ビュー行列の取得
//!
//! @param[in] なし
//!
//! @return ビュー行列
//----------------------------------------------------------------------
DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
{
	return m_view;
}

//----------------------------------------------------------------------
//! @brief 射影行列の取得
//!
//! @param[in] なし
//!
//! @return 射影行列
//----------------------------------------------------------------------
DirectX::SimpleMath::Matrix Camera::GetProjectionMatrix()
{
	return m_proj;
}

//----------------------------------------------------------------------
//! @brief 視点座標のセット
//!
//! @param[in] 視点座標
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::SetEyePos(DirectX::SimpleMath::Vector3 eyepos)
{
	m_eyepos = eyepos;
}

//----------------------------------------------------------------------
//! @brief 目標座標のセット
//!
//! @param[in] 目標座標
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::SetRefPos(DirectX::SimpleMath::Vector3 refpos)
{
	m_refpos = refpos;
}

//----------------------------------------------------------------------
//! @brief 目標座標のセット
//!
//! @param[in] 目標座標
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::SetUpVec(DirectX::SimpleMath::Vector3 upvec)
{
	m_upvec = upvec;
}

//----------------------------------------------------------------------
//! @brief 垂直方向視野角のセット
//!
//! @param[in] 垂直方向視野角
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::SetFloV(float floV)
{
	m_floV = floV;
}

//----------------------------------------------------------------------
//! @brief アスペクト比のセット
//!
//! @param[in] アスペクト比
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

//----------------------------------------------------------------------
//! @brief ニアクリップのセット
//!
//! @param[in] ニアクリップ
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::SetNearClip(float nearclip)
{
	m_nearclip = nearclip;
}

//----------------------------------------------------------------------
//! @brief ファークリップのセット
//!
//! @param[in] ファークリップ
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::SetFarClip(float farclip)
{
	m_farclip = farclip;
}
