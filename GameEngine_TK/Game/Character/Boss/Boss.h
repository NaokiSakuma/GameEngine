//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Boss.h
//!
//! @brief  �{�X�̃w�b�_�[�t�@�C��
//!
//! @date   2017/06/08
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once
#include "..\Character.h"
#include "..\..\Obj3d\Bullet\Bullet.h"

class Boss final : public Character
{
public:
	enum ENEMY_PARTS
	{
		BOSS,
		BOSS_PARTS_NUM,
	};

public:
	//�f�X�g���N�^
	~Boss();
	//������
	void Initialize()	override;
	//�X�V
	void Update()		override;
	//�`��
	void Render()		override;

	//�X�e�C�g��ύX
	void ChangeState(State* state) override;

	//Setter
	//�X�P�[�����O
	void SetScale(const DirectX::SimpleMath::Vector3& scale)	    override;
	//��]
	void SetRot(const DirectX::SimpleMath::Vector3& rot)		    override;
	//���s�ړ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans)	    override;
	void SetWallCount(const int wallCount) { m_wallCount = wallCount; };
	//Getter
	//�X�P�[�����O
	const DirectX::SimpleMath::Vector3& GetScale()			const	override;
	//��]
	const DirectX::SimpleMath::Vector3& GetRot()			const	override;
	//���s�ړ�
	const DirectX::SimpleMath::Vector3& GetTrans()			const	override;
	//���[���h�s��
	const DirectX::SimpleMath::Matrix&	GetWorld()			const	override;
	//�I�u�W�F�N�g
	Obj3d*								GetObj3d(int num)			override;
	const int GetWallCount() const { return m_wallCount; };
	const int GetHp() const { return m_hp; };
	//�N���G�C�g�֐�
	static std::unique_ptr<Boss> Create();

	//�e�̌�
	const int                           GetBulletNum()        const;
	//�e
	const std::unique_ptr<Bullet>&      GetBullet(int i)     const;
	//�e�𔭎˂���
	void ParentFreed();
	//�e������
	void DeleteBullet(int i);
	//hp�����炷
	void DecreaseHP();
	//�e�̐�
	static const int BULLETNUM = 50;
	//�e�̔��ˑ��x
	static const int BULLETCOUNT;

private:
	//�R���X�g���N�^
	Boss();
	//�e
	std::vector<std::unique_ptr<Bullet>> m_bullet;
	//�e�ۂ̑��x�x�N�g��
	DirectX::SimpleMath::Vector3 m_BulletVel;
	//�^�C�}�[
	int m_timer;
	//�ڕW�p�x
	float m_DistAngle;
	//�ǂƂ̓����蔻��̃J�E���g
	int m_wallCount;
	//�J�E���g
	int m_bulletCount;
	//HP
	int m_hp;
};