//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   EnemyState.h
//!
//! @brief  �G�̃X�e�C�g�̃w�b�_�[�t�@�C��
//!
//! @date   2017/06/12
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\State.h"

namespace EnemyState
{
	class Stand :public State
	{
	private:
		//���
		static State* m_state;
	public:
		//�f�X�g���N�^
		~Stand() {};
		//�C���X�^���X�̎擾
		static State* GetInstance();
		//������
		void Initialize() override {};
		//����̎��s
		void Execute(Character* character) override;
		//�I�u�W�F�N�g�̔j��
		static void Dispose();
	};

	class Move :public State
	{
	private:
		//���
		static State* m_state;
	public:
		//�f�X�g���N�^
		~Move() {};
		//�C���X�^���X�̎擾
		static State* GetInstance();
		//������
		void Initialize() override {};
		//����̎��s
		void Execute(Character* charcter) override;
		//�I�u�W�F�N�g�̔j��
		static void Dispose();
	};
}