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
