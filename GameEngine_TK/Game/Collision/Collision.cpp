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

//----------------------------------------------------------------------
//! @brief 3点から三角形を構築
//!
//! @param[in] 頂点p1、p2、p3、三角形(結果出力用)
//!
//! @return 当たっているかどうか
//----------------------------------------------------------------------
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Collision::Triangle* _triangle)
{
	//座標をコピー
	_triangle->P0 = _p0;
	_triangle->P1 = _p1; 
	_triangle->P2 = _p2;

	//法線ベクトルを計算
	Vector3 P0_P1 = _p1 - _p0;
	Vector3 P1_P2 = _p2 - _p1;
	
	//２ベクトルに垂直なベクトルを得る(法線ベクトル)
	_triangle->Normal = P0_P1.Cross(P1_P2);
	//正規化
	_triangle->Normal.Normalize();
}
