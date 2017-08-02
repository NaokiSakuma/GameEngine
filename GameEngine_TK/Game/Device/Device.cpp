//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Device.cpp
//!
//! @brief  デバイスクラスのソースファイル
//!
//! @date   2017/06/04
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Device.h"

using namespace DirectX;

//静的メンバの初期化
std::unique_ptr<DirectX::Keyboard>Device::m_keyboard = nullptr;
Keyboard::State Device::m_keyboardState;
std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>Device::m_keyboardTracker = nullptr;
//----------------------------------------------------------------------
//! @brief 初期化
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Device::Initialize()
{
	if(!m_keyboard)
		m_keyboard = std::make_unique<Keyboard>();
	if (!m_keyboardTracker)
		m_keyboardTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
}

//----------------------------------------------------------------------
//! @brief 更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Device::Update()
{
	m_keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(m_keyboardState);
}
