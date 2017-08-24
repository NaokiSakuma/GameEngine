//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Device.h
//!
//! @brief  デバイスクラスのヘッダーファイル
//!
//! @date   2017/06/04
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
#pragma once
#include <d3d11_1.h>
#include <Keyboard.h>
#include <Memory>

class Device
{
public:
	//キーボード
	static std::unique_ptr<DirectX::Keyboard>	   m_keyboard;
	//キーボードのステイト
	static DirectX::Keyboard::State	   m_keyboardState;
	//キーボードトラッカー
	static std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;
	//初期化
	static void Initialize();
	//更新
	static void Update();
};
