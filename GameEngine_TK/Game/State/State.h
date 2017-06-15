//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   State.h
//!
//! @brief  �L�����N�^�[�X�e�C�g�̃w�b�_�t�@�C��
//!
//! @date   2017/05/11�i06/04�X�V�FY.Yamada�j
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

class Character;

class State
{
public:
	//�f�X�g���N�^
	virtual ~State() {}
	//������
	virtual void Initialize() = 0;
	//����̎��s
	virtual void Execute(Character* character) = 0;
};

