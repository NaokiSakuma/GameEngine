//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Device.h
//!
//! @brief  �f�o�C�X�N���X�̃\�[�X�t�@�C��
//!
//! @date   2017/06/04
//!
//! @author Y.Yamada
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Device.h"

using namespace DirectX;

//�ÓI�����o�̏�����
std::unique_ptr<DirectX::Keyboard> Device::m_keyboard;
DirectX::Keyboard::KeyboardStateTracker Device::m_keyboardTracker;
//----------------------------------------------------------------------
//! @brief ������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Device::Initialize()
{
	m_keyboard = std::make_unique<Keyboard>();
}
