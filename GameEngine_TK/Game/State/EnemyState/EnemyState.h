//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   EnemyState.h
//!
//! @brief  敵のステイトのヘッダーファイル
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
		//状態
		static State* m_state;
	public:
		//デストラクタ
		~Stand() {};
		//インスタンスの取得
		static State* GetInstance();
		//初期化
		void Initialize() override {};
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
	public:
		//デストラクタ
		~Move() {};
		//インスタンスの取得
		static State* GetInstance();
		//初期化
		void Initialize() override {};
		//動作の実行
		void Execute(Character* charcter) override;
		//オブジェクトの破棄
		static void Dispose();
	};
}