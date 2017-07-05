//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.h
//!
//! @brief  �v���C���[�̃w�b�_�[�t�@�C��
//!
//! @date   2017/05/11 (06/04�X�V�FY.Yamada)
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
#pragma once

#include "..\Character.h"
#include "..\..\Obj3d\Bullet\Bullet.h"

class Player final :public Character
{
public:
	enum  PLAYER_PARTS
	{
		HEAD1,		//��
		HEAD2,		//��
		HEAD3,		//��
		HEAD4,		//��
		HEAD9 = 9,
		PLAYER_PARTS_NUM = 10,	//�p�[�c��
	};

public:

	//�f�X�g���N�^
	~Player();
	//������
	void Initialize()	override;
	//�X�V
	void Update()		override;
	//�`��
	void Render()		override;
	//�e�ۂ𑕓U
	void ResetBullet();
	//�X�e�C�g��ύX
	void ChangeState(State* state) override;

	//Setter
	//�X�P�[�����O
	void SetScale(const DirectX::SimpleMath::Vector3& scale)	    override;
	//��]
	void SetRot(const DirectX::SimpleMath::Vector3& rot)		    override;
	//���s�ړ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans)	    override;
	//Getter
	//�X�P�[�����O
	const DirectX::SimpleMath::Vector3& GetScale()			  const	override;
	//��]
	const DirectX::SimpleMath::Vector3& GetRot()			  const	override;
	//���s�ړ�
	const DirectX::SimpleMath::Vector3& GetTrans()			  const	override;
	//���[���h�s��
	const DirectX::SimpleMath::Matrix&	GetWorld()			  const	override;
	//�I�u�W�F�N�g
	Obj3d*								GetObj3d(int num)			override;
	//�e�ۗp�̓����蔻��
	const CollisionNode::SphereNode& GetCollisionNodeBullet() const;
	//�e
	Bullet* m_bullet;
	//�N���G�C�g�֐�
	static Player* Create();

	//�e�𔭎˂���
	void ParentFreed();

private:
	//�R���X�g���N�^
	Player();
	//�e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;

	//�Ĕ��˂��邽�߂̎���
	int m_nextFireTimer;

	//�e�ۗp�̓����蔻��
	CollisionNode::SphereNode m_collisionNodeBullet;
};

