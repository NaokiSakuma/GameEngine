//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Follow.cpp
//!
//! @brief  自機に追従するカメラクラスのソースファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Follow.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 5.0f;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
	, m_targetPos(Vector3::Zero)
	, m_targetAngle(0.0f)
{
}

//----------------------------------------------------------------------
//! @brief 更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void FollowCamera::Update()
{
	//カメラ視点座標、参照点座標
	Vector3 eyepos, refpos;
	//参照点座標を計算
	refpos = m_targetPos + Vector3(0, 2, 0);
	//自機からカメラ座標への差分
	Vector3 cameraV(0, 0, CAMERA_DISTANCE);
	//自機の後ろに回り込むための回転行列
	Matrix rotmat = Matrix::CreateRotationY(m_targetAngle);
	//カメラへのベクトルを計算
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//カメラ座標を計算
	eyepos = refpos + cameraV;
	//カメラに情報をセット
	SetEyePos(eyepos);
	SetRefPos(refpos);
	//基底クラスの更新
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
