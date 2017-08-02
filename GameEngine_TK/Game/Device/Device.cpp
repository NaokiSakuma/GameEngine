//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Device.cpp
//!
//! @brief  �f�o�C�X�N���X�̃\�[�X�t�@�C��
//!
//! @date   2017/06/04
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Device.h"

using namespace DirectX;

//�ÓI�����o�̏�����
std::unique_ptr<DirectX::Keyboard>Device::m_keyboard = nullptr;
Keyboard::State Device::m_keyboardState;
std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker>Device::m_keyboardTracker = nullptr;
//----------------------------------------------------------------------
//! @brief ������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Device::Initialize()
{
	if(!m_keyboard)
		m_keyboard = std::make_unique<Keyboard>();
	if (!m_keyboardTracker)
		m_keyboardTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Device::Update()
{
	m_keyboardState = m_keyboard->GetState();
	m_keyboardTracker->Update(m_keyboardState);
}
