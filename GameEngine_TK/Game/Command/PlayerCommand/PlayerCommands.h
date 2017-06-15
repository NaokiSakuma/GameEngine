//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerCommand.h
//!
//! @brief  プレイヤーのコマンドのヘッダファイル
//!
//! @date   2017/06/04
//!
//! @author Y.Yamada
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
#pragma once
#include "..\Command.h"

namespace PlayerCommands
{
	//前進
	class AdvanceCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void Advance(Character* character);
	};

	//後退
	class BackCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void Back(Character* character);
	};

	//左旋回
	class RotLeftCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void RotLeft(Character* character);
	};

	//右旋回
	class RotRightCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void RotRight(Character* character);
	};
	//弾発射
	class BulletCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void BulletsFire(Character* character);
	};
	//解散
	class BreakUpCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void BreakUp(Character* character);
	};

	//集合
	class AssemblyCommand : public Command
	{
	public:
		void Execute(Character* character) override;
	private:
		void Assembly(Character* character);
	};
}