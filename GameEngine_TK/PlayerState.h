//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerState.h
//!
//! @brief  プレイヤーのステイトのヘッダーファイル
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
		//状態
		static State* m_state;
	public:
		//インスタンスの取得
		static State* GetInstance();
		//動作の実行
		void Execute(Character* character);
		//オブジェクトの破棄
		static void Dispose();
	};

	class Jump :public State
	{
	private:
		//状態
		static State* m_state;
	public:
		//インスタンスの取得
		static State* GetInstance();
		//動作の実行
		void Execute(Character* character);
		//オブジェクトの破棄
		static void Dispose();
	};

	class Move :public State
	{
	private:
		//状態
		static State* m_state;
	public:
		//インスタンスの取得
		static State* GetInstance();
		//動作の実行
		void Execute(Character* character);
		//オブジェクトの破棄
		static void Dispose();
	};
}