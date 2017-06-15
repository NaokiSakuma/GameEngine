//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Bullet.h
//!
//! @brief  �e�̃w�b�_�[�t�@�C��
//!
//! @date   2017/06/13
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Obj3d.h"

class Bullet : public Obj3d
{
public:
	//�C�j�V�����C�Y
	void Initialize();
	//�f�X�g���N�^
	~Bullet() {};
	//�X�V
	void Update() override;
	//�N���G�C�g�֐�
	static Bullet* Create();
private:
	//�R���X�g���N�^
	Bullet();
};