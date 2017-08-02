//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   LockonCamera.h
//!
//! @brief  ロックオンカメラのヘッダファイル
//!
//! @date   2017/07/19
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Camera.h"
#include "..\..\Lockon\Lockon.h"
#include "..\FollowCamera\Follow.h"

class LockonCamera : public Camera
{
	//ステイト
	enum State
	{
		LOCKON,		//ロックオンカメラ
		FOLLOW,		//追従カメラ
		NUM,		//カメラの数
	};
public:
	//距離
	const float CAMERA_DISTANCE = 5.f;
	//切り替え時間
	const int SHIFT_FRAME = 120;
	//カメラのオフセット
	const float CAMERA_UPOFFSET = 2.f;
	//コンストラクタ
	LockonCamera(int width, int height);
	//更新
	void Update() override;
	//setter
	void SetLockon(Lockon* lockon);
protected:
	//ステイト
	State m_state;
	//ロックオン
	Lockon* m_lockon;
	//追従カメラ
	std::unique_ptr<FollowCamera> m_followCamera;
	//シフトするまでの時間
	int m_shiftCount;
};