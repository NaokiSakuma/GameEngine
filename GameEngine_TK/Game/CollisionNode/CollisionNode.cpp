//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   CollisionNode.cpp
//!
//! @brief  �����蔻��m�[�h����̃\�[�X�t�@�C��
//!
//! @date   2017/06/19
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "CollisionNode.h"

using namespace DirectX::SimpleMath;

bool CollisionNode::CollisionNode::m_debugVisible = true;
//----------------------------------------------------------------------
//! @brief �e�q�֌W��setter
//!
//! @param[in] �e
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void CollisionNode::CollisionNode::SetParent(Obj3d * parent)
{
	m_Obj.SetObjParent(parent);
}

//----------------------------------------------------------------------
//! @brief �I�t�Z�b�g��setter
//!
//! @param[in] �I�t�Z�b�g
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void CollisionNode::CollisionNode::SetTrans(const DirectX::SimpleMath::Vector3 & trans)
{
	m_trans = trans;
}

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
CollisionNode::SphereNode::SphereNode()
	:m_LocalRadius(1)
{
}

//----------------------------------------------------------------------
//! @brief �C�j�V�����C�Y
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void CollisionNode::SphereNode::Initialize()
{
	m_Obj.LoadModel(L"Resources/SphereNode.cmo");
}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void CollisionNode::SphereNode::Update()
{
	m_Obj.SetTrans(m_trans);
	m_Obj.SetScale(Vector3(m_LocalRadius));
	m_Obj.Update();
	{//���苅�̗v�f�v�Z
		const Matrix& worldm = m_Obj.GetWorld();
		//���f�����W�n�ł̒��S�_
		Vector3 center(0, 0, 0);
		//���f�����W�n�ł̉E�[�̓_
		Vector3 right(1, 0, 0);
		//���[���h���W�n�ɕϊ�
		center = Vector3::Transform(center, worldm);
		right = Vector3::Transform(right, worldm);
		//���苉�̗v�f����
		Collision::Sphere::Center = center;
		Collision::Sphere::Radius = Vector3::Distance(center, right);
	}
}

//----------------------------------------------------------------------
//! @brief �`��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void CollisionNode::SphereNode::Render()
{
	m_Obj.Render();
}

//----------------------------------------------------------------------
//! @brief ���[�J���̔��a��setter
//!
//! @param[in] ���[�J���̔��a
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void CollisionNode::SphereNode::SetLocalRadius(float radius)
{
	m_LocalRadius = radius;
}
