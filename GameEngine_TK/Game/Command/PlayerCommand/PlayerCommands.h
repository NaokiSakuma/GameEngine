//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerCommand.h
//!
//! @brief  �v���C���[�̃R�}���h�̃w�b�_�t�@�C��
//!
//! @date   2017/06/04
//!
//! @author Y.Yamada
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
#pragma once
#include "..\Command.h"

namespace PlayerCommands
{
	//�O�i
	class AdvanceCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void Advance(Character* character);
	};

	//���
	class BackCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void Back(Character* character);
	};

	//������
	class RotLeftCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void RotLeft(Character* character);
	};

	//�E����
	class RotRightCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void RotRight(Character* character);
	};
	//�e����
	class BulletCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void BulletsFire(Character* character);
	};
	//���U
	class BreakUpCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void BreakUp(Character* character);
	};

	//�W��
	class AssemblyCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void Assembly(Character* character);
	};
}