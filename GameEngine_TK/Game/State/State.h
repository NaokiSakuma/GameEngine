//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   State.h
//!
//! @brief  キャラクターステイトのヘッダファイル
//!
//! @date   2017/05/11（06/04更新：Y.Yamada）
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

class Character;

class State
{
public:
	//デストラクタ
	virtual ~State() {}
	//初期化
	virtual void Initialize() = 0;
	//動作の実行
	virtual void Execute(Character* character) = 0;
};

