//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Bullet.h
//!
//! @brief  弾のヘッダーファイル
//!
//! @date   2017/06/13
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Obj3d.h"

class Bullet : public Obj3d
{
public:
	//イニシャライズ
	void Initialize();
	//デストラクタ
	~Bullet() {};
	//更新
	void Update() override;
	//クリエイト関数
	static Bullet* Create();
private:
	//コンストラクタ
	Bullet();
};