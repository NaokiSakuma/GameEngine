//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.h
//!
//! @brief  �v���C���[�̃w�b�_�[�t�@�C��
//!
//! @date   2017/05/11 
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
		PLAYER_PARTS_NUM,	//�p�[�c��
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
	void SetInvincible(int invicible) { m_invincibleTime = invicible; };
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
	//�e�̌�
	const int                           GetBulletNum()        const;
	//�e
	const std::unique_ptr<Bullet>&      GetBullet (int i)     const;
	//���x���擾����
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	const int GetInvincible() { return m_invincibleTime; }
	//�N���G�C�g�֐�
	static Player* Create();

	//�e�𔭎˂���
	void ParentFreed();
	//�e������
	void DeleteBullet(int i);

	//���b�N�I�����̑���
	void ControlLockon();

	//�e�̐�
	static const int BULLETNUM = 50;
	//�e�̔��ˑ��x
	static const int BULLETCOUNT;
private:
	//�R���X�g���N�^
	Player();
	std::vector<std::unique_ptr<Bullet>> m_bullet;
	//�e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;

	//�Ĕ��˂��邽�߂̎���
	int m_nextFireTimer;
	//�J�E���g
	int m_bulletCount;
	//���G����
	int m_invincibleTime;
	
};

