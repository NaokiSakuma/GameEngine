//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Collision.cpp
//!
//! @brief  衝突判定のソースファイル
//!
//! @date   2017/06/15
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Collision.h"

using namespace DirectX::SimpleMath;

//----------------------------------------------------------------------
//! @brief 球と球との当たり判定
//!
//! @param[in] 球A、球B
//!
//! @return 当たっているかどうか
//----------------------------------------------------------------------
bool CheckSphere2Sphere(const Collision::Sphere& sphereA, const Collision::Sphere& sphereB)
{
	//球Aから球Bに向かうベクトル
	Vector3 sub = sphereB.Center - sphereA.Center;
	//球Aと球Bの距離を計算(三平方の定理)
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
	//半径の和を計算
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	//距離が半径の輪より大きければ当たっていない
	if (distanceSquare > radiusSquare)
		return false;
	//当たっている
	return true;
}