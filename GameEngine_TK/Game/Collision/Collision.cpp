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

//----------------------------------------------------------------------
//! @brief 3�_����O�p�`���\�z
//!
//! @param[in] ���_p1�Ap2�Ap3�A�O�p�`(���ʏo�͗p)
//!
//! @return �������Ă��邩�ǂ���
//----------------------------------------------------------------------
void ComputeTriangle(const Vector3& _p0, const Vector3& _p1, const Vector3& _p2, Collision::Triangle* _triangle)
{
	//���W���R�s�[
	_triangle->P0 = _p0;
	_triangle->P1 = _p1; 
	_triangle->P2 = _p2;

	//�@���x�N�g�����v�Z
	Vector3 P0_P1 = _p1 - _p0;
	Vector3 P1_P2 = _p2 - _p1;
	
	//�Q�x�N�g���ɐ����ȃx�N�g���𓾂�(�@���x�N�g��)
	_triangle->Normal = P0_P1.Cross(P1_P2);
	//���K��
	_triangle->Normal.Normalize();
}
