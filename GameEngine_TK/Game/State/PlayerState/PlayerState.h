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

#include "..\State.h"
#include "..\..\Command\PlayerCommand\PlayerCommands.h"

class Player;

namespace PlayerState
{
	class Stand :public State
	{
	private:
		//���
		static State* m_state;
		//�e�L�[�ɑΉ������R�}���h
		Command* m_putKeyA;
		Command* m_putKeyD;
		Command* m_putKeyQ;
		Command* m_putKeyE;
		Command* m_putKeySpace;
		Command* m_putKey1;
		Command* m_putKeyZ;
	public:
		//�f�X�g���N�^
		~Stand();
		//�C���X�^���X�̎擾
		static State* GetInstance();
		//������
		void Initialize() override;
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
		//�e�L�[�ɑΉ������R�}���h
		Command* m_putKeyW;
		Command* m_putKeyS;
		Command* m_putKeyA;
		Command* m_putKeyD;
		Command* m_putKeySpace;
		Command* m_putKeyZ;
	public:
		//�f�X�g���N�^
		~Move();
		//�C���X�^���X�̎擾
		static State* GetInstance();
		//������
		void Initialize() override;
		//����̎��s
		void Execute(Character* character) override;
		//�I�u�W�F�N�g�̔j��
		static void Dispose();
	};
	class Jump :public State
	{
	private:
		//���
		static State* m_state;
		//�e�L�[�ɑΉ������R�}���h
		Command* m_putKeyW;
		Command* m_putKeyS;
		Command* m_putKeyA;
		Command* m_putKeyD;
		Command* m_putKeySpace;
		Command* m_putKeyZ;
	public:
		//�f�X�g���N�^
		~Jump();
		//�C���X�^���X�̎擾
		static State* GetInstance();
		//������
		void Initialize() override;
		//����̎��s
		void Execute(Character* character) override;
		//�I�u�W�F�N�g�̔j��
		static void Dispose();
	};
	//class LockOn : public State
	//{
	//private:
	//	//���
	//	static State* m_state;
	//	//�e�L�[�ɑΉ������R�}���h
	//	Command* m_putKeyW;
	//	Command* m_putKeyS;
	//	Command* m_putKeyA;
	//	Command* m_putKeyD;
	//	Command* m_putKeySpace;
	//	Command* m_putKeyZ;
	//	Command* m_putKeyEnter;
	//public:
	//	//�f�X�g���N�^
	//	~LockOn();
	//	//�C���X�^���X�̎擾
	//	static State* GetInstance();
	//	//������
	//	void Initialize() override;
	//	//����̎��s
	//	void Execute(Character* character) override;
	//	//�I�u�W�F�N�g�̔j��
	//	static void Dispose();
	//};
}