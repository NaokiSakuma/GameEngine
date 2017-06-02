//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   State.cpp
//!
//! @brief  キャラクターステイトのヘッダファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "Character.h"

class State
{
public:
	//デストラクタ
	virtual ~State(){}
	//動作の実行
	virtual void Execute(Character* character) = 0;
};