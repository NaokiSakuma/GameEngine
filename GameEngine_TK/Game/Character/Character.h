//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Character.h
//!
//! @brief  �L�����N�^�[�̃w�b�_�[�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <vector>
#include "..\Obj3d\Obj3d.h"
#include "..\State\State.h"
#include "..\CollisionNode\CollisionNode.h"

class Character
{
public:
	//�d�͉����x
	const float GRAVITY_ACC = 0.03f;
	//�W�����v�����Ƃ��̏����x
	const float JUMP_SPEED_FIRST = 0.5f;
	//�W�����v���x����
	const float JUMP_SPEED_MAX = 0.3f;

	//�R���X�g���N�^
	Character() :m_isRenderColl(false),m_isJump(false) {};
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
	//�W�����v
	void StartJump() {
		if (!m_isJump)
		{
			//������̏����x��ݒ�
			m_velocity.y = JUMP_SPEED_FIRST;
			m_isJump = true;
		}
	}
	//�������J�n����
	void StartFall() {
		//�W�����v���ł͂Ȃ���
		if (!m_isJump)
		{
			//������̏����x���O��
			m_velocity.y = 0.f;
			//�W�����v�t���O�𗧂Ă�
			m_isJump = true;
		}
	}
	//�������I����
	void StopFall() {
		m_isJump = false;
		m_velocity = DirectX::SimpleMath::Vector3::Zero;
	}
	//Setter
	//�X�P�[�����O
	virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) {};
	//��]
	virtual void SetRot(const DirectX::SimpleMath::Vector3& rot)     {};
	//���s�ړ�
	virtual void SetTrans(const DirectX::SimpleMath::Vector3& trans) {};

	//Getter
	//�X�P�[�����O
	virtual const	DirectX::SimpleMath::Vector3&	GetScale()				const { return DirectX::SimpleMath::Vector3::Zero; };
	//��]
	virtual const	DirectX::SimpleMath::Vector3&	GetRot()				const { return DirectX::SimpleMath::Vector3::Zero; };
	//���s�ړ�
	virtual const	DirectX::SimpleMath::Vector3&	GetTrans()				const { return DirectX::SimpleMath::Vector3::Zero;};
	//���[���h�s��
	virtual const	DirectX::SimpleMath::Matrix&	GetWorld()				const { return DirectX::SimpleMath::Matrix::Identity; };
	//�I�u�W�F�N�g
	virtual			Obj3d*							GetObj3d(int obj_num)         { return nullptr; };
	//�����蔻��
	virtual		    CollisionNode::CollisionNode*   GetCollisionNode()            { return m_collisionNode; };
	//���x
	virtual const   DirectX::SimpleMath::Vector3&   GetVelocity()           const { return DirectX::SimpleMath::Vector3::Zero; };
	//�������̃t���O
		    const   bool                            GetIsJump()             const { return m_isJump; }
protected:
	//�I�u�W�F�N�g
	std::vector<Obj3d> m_obj3d;
	//�X�e�C�g
	State* m_state;
	//�����蔻��
	CollisionNode::CollisionNode* m_collisionNode;
	//���x
	DirectX::SimpleMath::Vector3 m_velocity;
	//�����蔻���`�悷��t���O
	bool m_isRenderColl;
	//�������̃t���O
	bool m_isJump;

};