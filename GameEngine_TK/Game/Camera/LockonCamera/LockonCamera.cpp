//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   LockonCamera.cpp
//!
//! @brief  ロックオンカメラのソースファイル
//!
//! @date   2017/07/19
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "LockonCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

LockonCamera::LockonCamera(int width, int height)
	: Camera(width, height)
	, m_state(State::FOLLOW)
	, m_followCamera(std::make_unique<FollowCamera>(width, height))
	, m_shiftCount(0)
{
}

void LockonCamera::Update()
{
	//追従カメラのターゲットを自機に設定
	m_followCamera->SetTarget(m_lockon->GetPlayer());
	//追従カメラの更新
	//m_followCamera->Update();

	//ロックオンしているかどうかのフラグ
	bool isLockon = m_lockon->IsLockOn();
	//敵
	Enemy* enemy = nullptr;

	//ロックオンしていたら
	if (isLockon)
	{
		enemy = m_lockon->SearchEnemy();
		//敵がいなかったら
		if (!enemy)
			isLockon = false;
	}

	//ステイトの切り替え
	switch (m_state)
	{
		//ロックオンカメラ
	case LOCKON:
		//ロックオンしていなかったら
		if (!isLockon)
			m_state = FOLLOW;
		break;
	case FOLLOW:
	default:		
		//ロックオンしていたら
		if (isLockon)
		{
			m_state = LOCKON;
			m_shiftCount = SHIFT_FRAME;
		}
		break;
	}

	//ロックオン中の場合
	if (isLockon)
	{
		//前の視点座標
		Vector3 eyeposPre = m_eyepos;
		//前の目標座標
		Vector3 refposPre = m_refpos;
		//プレイヤー
		Player* player = m_lockon->GetPlayer();
		//敵
		Enemy* enemy = m_lockon->SearchEnemy();
		//敵がいる場合
		if (enemy)
		{
			//プレイヤーの座標
			const Vector3& playerTrans = player->GetTrans();
			//敵の座標
			const Vector3& enemyTrans = enemy->GetTrans();
			//視点座標
			Vector3 eyepos;
			//目標座標
			Vector3 refpos;

			//敵から自機の頭上への方向ベクトル
			//自機と敵が近づくほど上から見下ろす状態になる
			Vector3 cameraV = playerTrans + Vector3(0, CAMERA_UPOFFSET, 0) - enemyTrans;
			cameraV.Normalize();
			cameraV *= CAMERA_DISTANCE;

			//視点
			eyepos = playerTrans + cameraV;
			//一定以上の高さにする
			if (eyepos.y - playerTrans.y < CAMERA_UPOFFSET)
				eyepos.y = playerTrans.y + CAMERA_UPOFFSET;
			//参照点
			refpos = enemyTrans;

			//カメラの移動の補完
			if (m_shiftCount > 0)
			{
				//時間の終わりが近づくにつれて補完なし状態に近づける
				//視点座標の比
				float eyeposRatio = 0.95f * m_shiftCount / SHIFT_FRAME;
				//目標座標の比
				float refposRatio = 0.80f * m_shiftCount / SHIFT_FRAME;
				eyepos = eyepos + (eyeposPre - eyepos) * eyeposRatio;
				refpos = refpos + (refposPre - refpos) * refposRatio;
				m_shiftCount--;
			}
			this->SetEyePos(eyepos);
			this->SetRefPos(refpos);
			this->SetUpVec(Vector3::UnitY);
		}
		//ロックオン中の敵が存在しない場合
		else
			isLockon = false;
	}

	//ロックオンしていない場合
	if (!isLockon)
	{
		////前の視点座標
		//Vector3 eyeposPre = m_eyepos;
		////前の目標座標
		//Vector3 refposPre = m_refpos;
		////視点座標
		//Vector3 eyepos = eyeposPre + ()
	}
	Camera::Update();
}

void LockonCamera::SetLockon(Lockon * lockon)
{
	m_lockon = lockon;
}
