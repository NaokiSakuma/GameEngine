//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   CollisionNode.cpp
//!
//! @brief  当たり判定ノード判定のソースファイル
//!
//! @date   2017/06/19
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

bool CollisionNode::CollisionNode::m_debugVisible = true;
//----------------------------------------------------------------------
//! @brief 親子関係のsetter
//!
//! @param[in] 親
//!
//! @return なし
//----------------------------------------------------------------------
void CollisionNode::CollisionNode::SetParent(Obj3d * parent)
{
	m_Obj.SetObjParent(parent);
}

//----------------------------------------------------------------------
//! @brief オフセットのsetter
//!
//! @param[in] オフセット
//!
//! @return なし
//----------------------------------------------------------------------
void CollisionNode::CollisionNode::SetTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	m_trans = trans;
}

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
CollisionNode::SphereNode::SphereNode()
	:m_LocalRadius(1)
{
}

//----------------------------------------------------------------------
//! @brief イニシャライズ
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void CollisionNode::SphereNode::Initialize()
{
	m_Obj.LoadModel(L"Resources/SphereNode.cmo");
}

//----------------------------------------------------------------------
//! @brief 更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void CollisionNode::SphereNode::Update()
{
	m_Obj.SetTrans(m_trans);
	m_Obj.SetScale(Vector3(m_LocalRadius));
	m_Obj.Update();
	{//判定球の要素計算
		const Matrix& worldm = m_Obj.GetWorld();
		//モデル座標系での中心点
		Vector3 center(0, 0, 0);
		//モデル座標系での右端の点
		Vector3 right(1, 0, 0);
		//ワールド座標系に変換
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);
		//判定級の要素を代入
		Collision::Sphere::Center = center;
		Collision::Sphere::Radius = Vector3::Distance(center, right);
	}
}

//----------------------------------------------------------------------
//! @brief 描画
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void CollisionNode::SphereNode::Render()
{
	m_Obj.Render();
}

//----------------------------------------------------------------------
//! @brief ローカルの半径のsetter
//!
//! @param[in] ローカルの半径
//!
//! @return なし
//----------------------------------------------------------------------
void CollisionNode::SphereNode::SetLocalRadius(float radius)
{
	m_LocalRadius = radius;
}
