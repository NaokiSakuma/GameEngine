//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Command.h
//!
//! @brief  コマンドのヘッダファイル
//!
//! @date   2017/06/04
//!
//! @author Y.Yamada
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
#pragma once

class Character;

class Command
{
public:
	virtual ~Command() {};
	virtual void Execute(Character* character) = 0;
};