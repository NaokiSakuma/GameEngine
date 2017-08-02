//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Follow.h
//!
//! @brief  自機に追従するカメラクラスのヘッダーファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <Windows.h>

#include <Keyboard.h>
#include "..\Camera.h"
#include "..\..\Character\Character.h"

class FollowCamera : public Camera
{
public:
	//カメラとの距離
	static const float CAMERA_DISTANCE;
	//コンストラクタ
	FollowCamera(int width, int height);
	//デストラクタ
	~FollowCamera();
	//更新
	void Update() override;
	//setter
	//追従対象
	void SetTarget(Character* targetChara);
	//追従対象の座標
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);
	//追従対象の回転角
	void SetTargetAngle(const DirectX::SimpleMath::Vector3& targetangle);
	//キーボード
	void SetKeyboard(DirectX::Keyboard* keyboard);
protected:
	//追従対象
	Character* m_target;
	//追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	//追従対象の回転角
	DirectX::SimpleMath::Vector3 m_targetAngle;
	//キーボード(シングルトンを使う)
	DirectX::Keyboard* m_keyboard;
	//キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//視点を切り替えるフラグ
	bool m_isFPS;
};