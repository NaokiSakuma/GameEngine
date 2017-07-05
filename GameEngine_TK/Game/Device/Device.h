//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Device.h
//!
//! @brief  �f�o�C�X�N���X�̃w�b�_�[�t�@�C��
//!
//! @date   2017/06/04(06/20�X�V : N.Sakuma)
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
	//�L�[�{�[�h
	static std::unique_ptr<DirectX::Keyboard>	   m_keyboard;
	//�L�[�{�[�h�g���b�J�[
	static DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//������
	static void Initialize();
};
