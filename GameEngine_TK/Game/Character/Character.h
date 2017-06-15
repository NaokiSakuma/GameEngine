//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Character.h
//!
//! @brief  �L�����N�^�[�̃w�b�_�[�t�@�C��
//!
//! @date   2017/05/11(06/04�X�V�FY.Yamada)
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "..\Obj3d\Obj3d.h"
#include "..\State\State.h"

class Character
{
public:
	//�R���X�g���N�^
	Character() {};
	//�f�X�g���N�^
	virtual ~Character() { delete m_state; };
	//������
	virtual void Initialize() = 0;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Render() = 0;

	//�e�X�g
	virtual void ParentFreed() {};

	//�X�e�C�g�̕ύX
	virtual void ChangeState(State* state) {};
	//Setter
	//�X�P�[�����O
	virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) {};
	//��]
	virtual void SetRot(const DirectX::SimpleMath::Vector3& rot) {};
	//���s�ړ�
	virtual void SetTrans(const DirectX::SimpleMath::Vector3& trans) {};

	//Getter
	//�X�P�[�����O
	virtual const	DirectX::SimpleMath::Vector3&	GetScale()				const { return DirectX::SimpleMath::Vector3(0, 0, 0); };
	//��]
	virtual const	DirectX::SimpleMath::Vector3&	GetRot()				const { return DirectX::SimpleMath::Vector3(0, 0, 0); };
	//���s�ړ�
	virtual const	DirectX::SimpleMath::Vector3&	GetTrans()				const { return DirectX::SimpleMath::Vector3(0, 0, 0); };
	//���[���h�s��
	virtual const	DirectX::SimpleMath::Matrix&	GetWorld()				const { return DirectX::SimpleMath::Matrix::Identity; };
	//�I�u�W�F�N�g
	virtual			Obj3d*							GetObj3d(int obj_num) { return nullptr; };
protected:
	//�I�u�W�F�N�g
	std::vector<Obj3d> m_obj3d;
	//�X�e�C�g
	State* m_state;
};