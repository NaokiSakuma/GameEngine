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

#include "..\State.h"
#include "..\..\Command\PlayerCommand\PlayerCommands.h"

class Player;

namespace PlayerState
{
	class Stand :public State
	{
	private:
		//状態
		static State* m_state;
		//各キーに対応したコマンド
		Command* m_putKeyA;
		Command* m_putKeyD;
		Command* m_putKeyQ;
		Command* m_putKeyE;
		Command* m_putKeySpace;
		Command* m_putKey1;
		Command* m_putKeyZ;
	public:
		//デストラクタ
		~Stand();
		//インスタンスの取得
		static State* GetInstance();
		//初期化
		void Initialize() override;
		//動作の実行
		void Execute(Character* character) override;
		//オブジェクトの破棄
		static void Dispose();
	};

	class Move :public State
	{
	private:
		//状態
		static State* m_state;
		//各キーに対応したコマンド
		Command* m_putKeyW;
		Command* m_putKeyS;
		Command* m_putKeyA;
		Command* m_putKeyD;
		Command* m_putKeySpace;
		Command* m_putKeyZ;
	public:
		//デストラクタ
		~Move();
		//インスタンスの取得
		static State* GetInstance();
		//初期化
		void Initialize() override;
		//動作の実行
		void Execute(Character* character) override;
		//オブジェクトの破棄
		static void Dispose();
	};
	class Jump :public State
	{
	private:
		//状態
		static State* m_state;
		//各キーに対応したコマンド
		Command* m_putKeyW;
		Command* m_putKeyS;
		Command* m_putKeyA;
		Command* m_putKeyD;
		Command* m_putKeySpace;
		Command* m_putKeyZ;
	public:
		//デストラクタ
		~Jump();
		//インスタンスの取得
		static State* GetInstance();
		//初期化
		void Initialize() override;
		//動作の実行
		void Execute(Character* character) override;
		//オブジェクトの破棄
		static void Dispose();
	};
	//class LockOn : public State
	//{
	//private:
	//	//状態
	//	static State* m_state;
	//	//各キーに対応したコマンド
	//	Command* m_putKeyW;
	//	Command* m_putKeyS;
	//	Command* m_putKeyA;
	//	Command* m_putKeyD;
	//	Command* m_putKeySpace;
	//	Command* m_putKeyZ;
	//	Command* m_putKeyEnter;
	//public:
	//	//デストラクタ
	//	~LockOn();
	//	//インスタンスの取得
	//	static State* GetInstance();
	//	//初期化
	//	void Initialize() override;
	//	//動作の実行
	//	void Execute(Character* character) override;
	//	//オブジェクトの破棄
	//	static void Dispose();
	//};
}