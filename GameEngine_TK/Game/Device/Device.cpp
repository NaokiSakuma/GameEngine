//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Device.h
//!
//! @brief  デバイスクラスのソースファイル
//!
//! @date   2017/06/04
//!
//! @author Y.Yamada
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Device.h"

using namespace DirectX;

//静的メンバの初期化
std::unique_ptr<DirectX::Keyboard> Device::m_keyboard;
DirectX::Keyboard::KeyboardStateTracker Device::m_keyboardTracker;
//----------------------------------------------------------------------
//! @brief 初期化
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Device::Initialize()
{
	m_keyboard = std::make_unique<Keyboard>();
}
