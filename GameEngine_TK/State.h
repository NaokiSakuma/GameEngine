//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   State.cpp
//!
//! @brief  �L�����N�^�[�X�e�C�g�̃w�b�_�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "Character.h"

class State
{
public:
	//�f�X�g���N�^
	virtual ~State(){}
	//����̎��s
	virtual void Execute(Character* character) = 0;
};