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
const float FollowCamera::CAMERA_DISTANCE = 2.0f;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
FollowCamera::FollowCamera(int width, int height)
	:Camera(width, height)
	, m_targetPos(Vector3::Zero)
	, m_targetAngle(0.0f)
	, m_keyboard(nullptr)
	, m_isFPS(false)
	,m_target(nullptr)
{
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
FollowCamera::~FollowCamera()
{
	delete m_target;
	m_target = nullptr;
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
	//キーボードの状態を取得
	Keyboard::State keystate = m_keyboard->GetState();
	m_keyboardTracker.Update(keystate);
	//Cキーが押されたら
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keyboard::C))
	{
		m_isFPS = !m_isFPS;
	}
	//カメラ視点座標、参照点座標
	Vector3 eyepos, refpos;
	if (m_isFPS)
	{//FPSカメラの処理
		Vector3 position;
		//参照点座標を計算
		position = m_targetPos + Vector3(0, 0.2f, 0);
		//自機からカメラ座標への差分
		Vector3 cameraV(0, 0, -CAMERA_DISTANCE);
		//自機の後ろに回り込むための回転行列
		Matrix rotmat = Matrix::CreateRotationY(m_targetAngle.y);
		//カメラへのベクトルを計算
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//カメラ座標を計算
		//少し進んだ位置が視点座標
		eyepos = position + cameraV *0.1f;
		//たくさん進んだ位置が参照点座標
		refpos = position + cameraV;
	}
	else if (!m_isFPS)
	{//TPSカメラの処理
		//参照点座標を計算
		refpos = m_targetPos + Vector3(0, 2, 0);
		//自機からカメラ座標への差分
		Vector3 cameraV(0, 0, CAMERA_DISTANCE);
		//自機の後ろに回り込むための回転行列
		Matrix rotmatY = Matrix::CreateRotationY(m_targetAngle.y);
		Matrix rotmatX = Matrix::CreateRotationX(m_targetAngle.x);
		Matrix rotmat = rotmatX * rotmatY;
		//カメラへのベクトルを計算
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		//カメラ座標を計算
		eyepos = refpos + cameraV;
		//視点を現在位置から保管する
		eyepos = m_eyepos + (eyepos - m_eyepos) * 0.05f;
		//参照点を現在位置から保管する
		refpos = m_refpos + (refpos - m_refpos) * 0.2f;

	}
	//カメラに情報をセット
	SetEyePos(eyepos);
	SetRefPos(refpos);
	//基底クラスの更新
	Camera::Update();
}

//----------------------------------------------------------------------
//! @brief 追従対象のsetter
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void FollowCamera::SetTarget(Character * targetChara)
{
	m_target = targetChara;
}

//----------------------------------------------------------------------
//! @brief 追従対象の座標のsetter
//!
//! @param[in] ターゲットの座標
//!
//! @return なし
//----------------------------------------------------------------------
void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_targetPos = targetpos;
}

//----------------------------------------------------------------------
//! @brief 追従対象の回転角のsetter
//!
//! @param[in] ターゲットの回転角
//!
//! @return なし
//----------------------------------------------------------------------
void FollowCamera::SetTargetAngle(const DirectX::SimpleMath::Vector3 & targetangle)
{
	m_targetAngle = targetangle;
}

//----------------------------------------------------------------------
//! @brief キーボード
//!
//! @param[in] キーボード
//!
//! @return なし
//----------------------------------------------------------------------
void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}
