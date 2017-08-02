//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Lockon.cpp
//!
//! @brief  ���b�N�I���̃\�[�X�t�@�C��
//!
//! @date   2017/07/13
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Character\Player\Player.h"
#include "..\Character\Enemy\Enemy.h"

class Lockon
{
public:
	//�R���X�g���N�^
	Lockon();
	//�f�X�g���N�^
	~Lockon();
	//���b�N�I������
	bool ToLockon();
	//���b�N�I������������
	void ReleaseLockon();
	//�G��T��
	Enemy* SearchEnemy();

	//setter
	//�v���C���[
	void SetPlayer(Player* player);
	//�G
	void SetEnemy(std::vector<std::unique_ptr<Enemy>>* enemy);

	//getter
	//�v���C���[
	Player* GetPlayer();
	//�G
	bool IsLockOn();

protected:
	//�v���C���[
	Player* m_player;
	//�G
	std::vector<std::unique_ptr<Enemy>>* m_enemy;
	//���b�N�I���Ώۂ̃V���A���i���o�[
	unsigned int m_lockOnSerial;
	//���b�N�I����
	bool m_isLockOn;
};