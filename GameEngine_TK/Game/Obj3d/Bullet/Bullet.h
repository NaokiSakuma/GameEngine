//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Bullet.h
//!
//! @brief  �e�̃w�b�_�[�t�@�C��
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
	//�C�j�V�����C�Y
	void Initialize(DirectX::SimpleMath::Vector3 trans, DirectX::SimpleMath::Vector3 rot, DirectX::SimpleMath::Matrix world, bool isPlayer);
	//�f�X�g���N�^
	~Bullet() { };
	//�X�V
	void Update() override;
	//�`��
	void Render() override;
	//�N���G�C�g�֐�
	static std::unique_ptr<Bullet>Create(DirectX::SimpleMath::Vector3 trans, DirectX::SimpleMath::Vector3 rot , DirectX::SimpleMath::Matrix world, bool isPlayer);
	//getter
	//�e��������J�E���g�̃J�E���^�[
	const int GetDeleteCount() const{ return m_deleteCount; };
	//�e�̓����蔻��
	const CollisionNode::SphereNode GetCollisionNodeBullet() const { return *m_collisionNodeBullet; };
private:
	//�R���X�g���N�^
	Bullet();
	//�e�̓����蔻��
	CollisionNode::SphereNode* m_collisionNodeBullet;
	//�e�̑��x
	DirectX::SimpleMath::Vector3 m_bulletVel;
	//�e��������J�E���g
	int m_deleteCount;

	float x, y, z;
};