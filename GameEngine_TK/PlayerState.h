//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerState.h
//!
//! @brief  �v���C���[�̃X�e�C�g�̃w�b�_�[�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "State.h"

namespace PlayerState
{
	class Stand :public State
	{
	private:
		//���
		static State* m_state;
	public:
		//�C���X�^���X�̎擾
		static State* GetInstance();
		//����̎��s
		void Execute(Character* character);
		//�I�u�W�F�N�g�̔j��
		static void Dispose();
	};

	class Jump :public State
	{
	private:
		//���
		static State* m_state;
	public:
		//�C���X�^���X�̎擾
		static State* GetInstance();
		//����̎��s
		void Execute(Character* character);
		//�I�u�W�F�N�g�̔j��
		static void Dispose();
	};

	class Move :public State
	{
	private:
		//���
		static State* m_state;
	public:
		//�C���X�^���X�̎擾
		static State* GetInstance();
		//����̎��s
		void Execute(Character* character);
		//�I�u�W�F�N�g�̔j��
		static void Dispose();
	};
}