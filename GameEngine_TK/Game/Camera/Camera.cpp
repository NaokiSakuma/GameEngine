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
using namespace DirectX::SimpleMath;

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
	//ビルボード行列を計算
	Billboard();
}

//----------------------------------------------------------------------
//! @brief ビュー行列の取得
//!
//! @param[in] なし
//!
//! @return ビュー行列
//----------------------------------------------------------------------
const Matrix& Camera::GetViewMatrix() const
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
//! @brief 視点座標のセット
//!
//! @param[in] 視点座標
//!
//! @return なし
//----------------------------------------------------------------------
void Camera::SetEyePos(const Vector3& eyepos)
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
void Camera::SetRefPos(const Vector3& refpos)
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
void Camera::SetUpVec(const Vector3& upvec)
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

void Camera::Billboard()
{
	//視線方向
	Vector3 eyeDir = m_refpos - m_eyepos;
	//Y軸
	Vector3 axisY = Vector3::UnitY;
	//X軸				視線方向の外積
	Vector3 axisX = axisY.Cross(eyeDir);
	axisX.Normalize();
	//Z軸
	Vector3 axisZ = axisX.Cross(axisY);
	axisZ.Normalize();
	//Y軸回りのビルボード行列
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
	//ビルボード行列
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
