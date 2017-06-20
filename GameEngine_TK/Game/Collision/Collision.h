//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Collision.h
//!
//! @brief  �Փ˔���̃w�b�_�[�t�@�C��
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
	//��
	class Sphere
	{
	public:
		//���S���W
		DirectX::SimpleMath::Vector3 Center;
		//���a
		float Radius;
		//�R���X�g���N�^
		Sphere():Radius(1.0f) {};
	};
	//����
	class Segment
	{
	public:
		//���_���W
		DirectX::SimpleMath::Vector3 Start;
		//�I�_���W
		DirectX::SimpleMath::Vector3 End;
	};
}

//���Ƌ��Ƃ̓����蔻��
bool CheckSphere2Sphere(const Collision::Sphere & sphereA, const Collision::Sphere & sphereB);
