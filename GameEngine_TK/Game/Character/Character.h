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
#include "..\CollisionNode\CollisionNode.h"

class Character
{
public:
	//�R���X�g���N�^
	Character() :m_isRenderColl(false) {};
	//�f�X�g���N�^
	virtual ~Character() {
		/*if (m_state) { delete m_state; m_state = nullptr; }*/
	}
	//������
	virtual void Initialize() = 0;
	//�X�V
	virtual void Update() = 0;
	//�`��
	virtual void Render() = 0;

	//�X�e�C�g�̕ύX
	virtual void ChangeState(State* state) {};
	//�����蔻���`�悷�邩�ǂ����̕ύX
	void ChangeCollision() { m_isRenderColl = !m_isRenderColl; };
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
	virtual			Obj3d*							GetObj3d(int obj_num)         { return nullptr; };
	//�����蔻��
	virtual		    CollisionNode::CollisionNode*   GetCollisionNode()            { return m_collisionNode; };
protected:
	//�I�u�W�F�N�g
	std::vector<Obj3d> m_obj3d;
	//�X�e�C�g
	State* m_state;
	//�����蔻��
	CollisionNode::CollisionNode* m_collisionNode;
	//�����蔻���`�悷��t���O
	bool m_isRenderColl;
};