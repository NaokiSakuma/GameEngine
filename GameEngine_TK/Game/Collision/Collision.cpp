//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Collision.cpp
//!
//! @brief  �Փ˔���̃\�[�X�t�@�C��
//!
//! @date   2017/06/15
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Collision.h"

using namespace DirectX::SimpleMath;

//----------------------------------------------------------------------
//! @brief ���Ƌ��Ƃ̓����蔻��
//!
//! @param[in] ��A�A��B
//!
//! @return �������Ă��邩�ǂ���
//----------------------------------------------------------------------
bool CheckSphere2Sphere(const Collision::Sphere& sphereA, const Collision::Sphere& sphereB)
{
	//��A���狅B�Ɍ������x�N�g��
	Vector3 sub = sphereB.Center - sphereA.Center;
	//��A�Ƌ�B�̋������v�Z(�O�����̒藝)
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
	//���a�̘a���v�Z
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	//���������a�̗ւ��傫����Γ������Ă��Ȃ�
	if (distanceSquare > radiusSquare)
		return false;
	//�������Ă���
	return true;
}