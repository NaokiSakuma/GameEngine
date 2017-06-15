//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Enemy.h
//!
//! @brief  �G�̃w�b�_�[�t�@�C��
//!
//! @date   2017/06/08
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Character.h"

class Enemy final : public Character
{
public:
	enum ENEMY_PARTS
	{
		HEAD1,
		HEAD2,
		ENEMY_PARTS_NUM,
	};

public:
	//�f�X�g���N�^
	~Enemy();
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
	void SetScale(const DirectX::SimpleMath::Vector3& scale)	override;
	//��]
	void SetRot(const DirectX::SimpleMath::Vector3& rot)		override;
	//���s�ړ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans)	override;

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

	//�N���G�C�g�֐�
	static Enemy* Create();

private:
	//�R���X�g���N�^
	Enemy();
	//�^�C�}�[
	int m_timer;
	//�ڕW�p�x
	float m_DistAngle;
};