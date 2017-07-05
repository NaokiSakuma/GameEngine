//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   CollisionNode.h
//!
//! @brief  �����蔻��m�[�h����̃w�b�_�[�t�@�C��
//!
//! @date   2017/06/19
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Obj3d\Obj3d.h"
#include "..\Collision\Collision.h"

namespace CollisionNode
{
	//�����蔻��m�[�h
	class CollisionNode
	{
	public:
		//�f�o�b�O�\����setter
		static bool SetDebugVisible(bool visible) { m_debugVisible = visible; };
		//�f�o�b�O�\����getter
		static bool GetDebugVisible()			  { return m_debugVisible; };
	protected:
		//�f�o�b�O�\��
		static bool m_debugVisible;
	public:
		//������
		virtual void Initialize() = 0;
		//�X�V
		virtual void Update()	  = 0;
		//�`��
		virtual void Render()     = 0;

		//setter
		//�e�q�֌W
		void SetParent(Obj3d* parent);
		//�I�t�Z�b�g
		void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	protected:
		//�f�o�b�O�\���p�I�u�W�F�N�g
		Obj3d m_Obj;
		//�e����̃I�t�Z�b�g
		DirectX::SimpleMath::Vector3 m_trans;
	};

	//���̓����蔻��m�[�h
	class SphereNode final: public CollisionNode ,public Collision::Sphere
	{
	public:
		//�R���X�g���N�^
		SphereNode();
		//������
		void Initialize() override;
		//�X�V
		void Update()	  override;
		//�`��
		void Render()	  override;
		//���[�J���̔��a
		void SetLocalRadius(float radius);
	private:
		//���[�J���̔��a
		float m_LocalRadius;
	};
}