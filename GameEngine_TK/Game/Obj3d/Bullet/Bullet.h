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
#include "..\..\CollisionNode\CollisionNode.h"
class Bullet : public Obj3d
{
public:
	//イニシャライズ
	void Initialize(DirectX::SimpleMath::Vector3 trans, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Matrix world, bool isPlayer);
	//デストラクタ
	~Bullet() { };
	//更新
	void Update() override;
	//描画
	void Render() override;
	//クリエイト関数
	static std::unique_ptr<Bullet>Create(DirectX::SimpleMath::Vector3 trans, DirectX::SimpleMath::Vector3 rot , DirectX::SimpleMath::Matrix world, bool isPlayer);
	//getter
	//弾が消えるカウントのカウンター
	const int GetDeleteCount() const{ return m_deleteCount; };
	//弾の当たり判定
	const CollisionNode::SphereNode GetCollisionNodeBullet() const { return *m_collisionNodeBullet; };
private:
	//コンストラクタ
	Bullet();
	//弾の当たり判定
	CollisionNode::SphereNode* m_collisionNodeBullet;
	//弾の速度
	DirectX::SimpleMath::Vector3 m_bulletVel;
	//弾が消えるカウント
	int m_deleteCount;

	float x, y, z;
};