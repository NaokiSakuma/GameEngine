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

class Obj3d;

class Character
{
public:
	//�R���X�g���N�^
	Character();
	//�f�X�g���N�^
	virtual ~Character();
	//������
	//void Initialize();
	//�X�V
	virtual void Update();
protected:
	//�I�u�W�F�N�g
	Obj3d *obj3d;
};