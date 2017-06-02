//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Character.h
//!
//! @brief  キャラクターのヘッダーファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

class Obj3d;

class Character
{
public:
	//コンストラクタ
	Character();
	//デストラクタ
	virtual ~Character();
	//初期化
	//void Initialize();
	//更新
	virtual void Update();
protected:
	//オブジェクト
	Obj3d *obj3d;
};