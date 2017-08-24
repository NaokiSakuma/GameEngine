//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Device.h
//!
//! @brief  �f�o�C�X�N���X�̃w�b�_�[�t�@�C��
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
	//�L�[�{�[�h
	static std::unique_ptr<DirectX::Keyboard>	   m_keyboard;
	//�L�[�{�[�h�̃X�e�C�g
	static DirectX::Keyboard::State	   m_keyboardState;
	//�L�[�{�[�h�g���b�J�[
	static std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_keyboardTracker;
	//������
	static void Initialize();
	//�X�V
	static void Update();
};
