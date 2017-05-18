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
#include "Camera.h"

class FollowCamera : public Camera
{
public:
	//カメラとの距離
	static const float CAMERA_DISTANCE;
	//コンストラクタ
	FollowCamera(int width, int height);
	//更新
	void Update() override;
	//追従対象の座標をセット
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);
	//追従対象の回転角をセット
	void SetTargetAngle(float targetangle);
	//キーボードをセット
	void SetKeyboard(DirectX::Keyboard* keyboard);
protected:
	//追従対象の座標
	DirectX::SimpleMath::Vector3 m_targetPos;
	//追従対象の回転角
	float m_targetAngle;
	//キーボード(シングルトンを使う)
	DirectX::Keyboard* m_keyboard;
	//キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//視点を切り替えるフラグ
	bool m_isFPS;
};