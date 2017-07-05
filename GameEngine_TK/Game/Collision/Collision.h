//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Collision.h
//!
//! @brief  衝突判定のヘッダーファイル
//!
//! @date   2017/06/15
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>

namespace Collision
{
	//球
	class Sphere
	{
	public:
		//中心座標
		DirectX::SimpleMath::Vector3 Center;
		//半径
		float Radius;
		//コンストラクタ
		Sphere():Radius(1.0f) {};
	};
	//線分
	class Segment
	{
	public:
		//視点座標
		DirectX::SimpleMath::Vector3 Start;
		//終点座標
		DirectX::SimpleMath::Vector3 End;
	};
	//三角形
	class Triangle
	{
	public:
		//頂点座標
		DirectX::SimpleMath::Vector3 P0;
		DirectX::SimpleMath::Vector3 P1;
		DirectX::SimpleMath::Vector3 P2;
		//法線ベクトル
		DirectX::SimpleMath::Vector3 Normal;
	};
}

//球と球との当たり判定
bool CheckSphere2Sphere(const Collision::Sphere & sphereA, const Collision::Sphere & sphereB);
//３点から三角形を構築
void ComputeTriangle(const DirectX::SimpleMath::Vector3 & _p0, const DirectX::SimpleMath::Vector3 & _p1, const DirectX::SimpleMath::Vector3 & _p2, Collision::Triangle * _triangle);
