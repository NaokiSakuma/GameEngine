//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Device.h
//!
//! @brief  デバイスクラスのヘッダーファイル
//!
//! @date   2017/06/04(06/20更新 : N.Sakuma)
//!
//! @author Y.Yamada
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
#pragma once
#include <d3d11.h>
#include <Keyboard.h>
#include <Memory>

class Device
{
public:
	//キーボード
	static std::unique_ptr<DirectX::Keyboard>	   m_keyboard;
	//キーボードトラッカー
	static DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//初期化
	static void Initialize();
};
