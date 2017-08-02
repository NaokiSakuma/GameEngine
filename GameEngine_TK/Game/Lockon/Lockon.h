//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Lockon.cpp
//!
//! @brief  ロックオンのソースファイル
//!
//! @date   2017/07/13
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Character\Player\Player.h"
#include "..\Character\Enemy\Enemy.h"

class Lockon
{
public:
	//コンストラクタ
	Lockon();
	//デストラクタ
	~Lockon();
	//ロックオンする
	bool ToLockon();
	//ロックオンを解除する
	void ReleaseLockon();
	//敵を探す
	Enemy* SearchEnemy();

	//setter
	//プレイヤー
	void SetPlayer(Player* player);
	//敵
	void SetEnemy(std::vector<std::unique_ptr<Enemy>>* enemy);

	//getter
	//プレイヤー
	Player* GetPlayer();
	//敵
	bool IsLockOn();

protected:
	//プレイヤー
	Player* m_player;
	//敵
	std::vector<std::unique_ptr<Enemy>>* m_enemy;
	//ロックオン対象のシリアルナンバー
	unsigned int m_lockOnSerial;
	//ロックオン中
	bool m_isLockOn;
};