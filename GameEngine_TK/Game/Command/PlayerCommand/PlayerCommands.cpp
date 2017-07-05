//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerCommand.cpp
//!
//! @brief  �v���C���[�̃R�}���h�̃\�[�X�t�@�C��
//!
//! @date   2017/06/04(06/20�X�V : N.Sakuma)
//!
//! @author Y.Yamada
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "PlayerCommands.h"
#include "..\..\Character\Player\Player.h"

using namespace DirectX;
using namespace SimpleMath;

//----------------------------------------------------------------------
//! @brief �R�}���h�̎��s
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::AdvanceCommand::Execute(Character* character)	{ Advance(character); }
void PlayerCommands::BackCommand::Execute(Character* character)		{ Back(character); }
void PlayerCommands::RotLeftCommand::Execute(Character* character)	{ RotLeft(character); }
void PlayerCommands::RotRightCommand::Execute(Character* character) { RotRight(character); }
void PlayerCommands::BreakUpCommand::Execute(Character* character)	{ BreakUp(character); }
void PlayerCommands::AssemblyCommand::Execute(Character* character) { Assembly(character); }
void PlayerCommands::BulletCommand::Execute(Character* character)   { BulletsFire(character); }
void PlayerCommands::CollisionCommand::Execute(Character* charcter) { CollisionChange(charcter); }
//----------------------------------------------------------------------
//! @brief �O�i�R�}���h
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::AdvanceCommand::Advance(Character* character)
{
	float angle = character->GetRot().y;
	Matrix rotmat = Matrix::CreateRotationY(angle);
	Vector3 moveV(0, 0, -0.1f);
	moveV = Vector3::TransformNormal(moveV, rotmat);
	Vector3 pos = character->GetTrans();
	character->SetTrans(pos + moveV);
	for (int i = 1; i < Player::PLAYER_PARTS::PLAYER_PARTS_NUM; i++)
	{
		if (character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z > 1.0f ||
			character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z < -1.0f ||
			character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x > 1.0f ||
			character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x < -1.0f
			)
		{
			float angle = character->GetObj3d(i)->GetRot().y;
			Matrix rotmat = Matrix::CreateRotationY(angle);
			Vector3 moveV(0, 0, -0.05f);
			if (character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z > 1.5f ||
				character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z < -1.5f ||
				character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x > 1.5f ||
				character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x < -1.5f)
			{
				moveV = Vector3(0, 0, -0.1f);
			}
			moveV = Vector3::TransformNormal(moveV, rotmat);
			Vector3 pos = character->GetObj3d(i)->GetTrans();
			character->GetObj3d(i)->SetTrans(pos + moveV);
		}
		character->GetObj3d(i)->SetRot(Vector3(0, atan2f((character->GetObj3d(i)->GetTrans().x - character->GetObj3d(i - 1)->GetTrans().x),
			(character->GetObj3d(i)->GetTrans().z - character->GetObj3d(i - 1)->GetTrans().z)), 0));
	}
}

//----------------------------------------------------------------------
//! @brief ��ރR�}���h
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::BackCommand::Back(Character* character)
{
	for (int i = 0; i < Player::PLAYER_PARTS::PLAYER_PARTS_NUM; i++)
	{
		float angle = character->GetObj3d(i)->GetRot().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		Vector3 moveV(0, 0, 0.1f);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		Vector3 pos = character->GetObj3d(i)->GetTrans();
		character->GetObj3d(i)->SetTrans(pos + moveV);
	}
	for (int i = 1; i < Player::PLAYER_PARTS::PLAYER_PARTS_NUM; i++)
	{
		character->GetObj3d(i)->SetRot(Vector3(0, atan2f((character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x),
			(character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z)), 0));
	}
}

//----------------------------------------------------------------------
//! @brief ����]�R�}���h
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::RotLeftCommand::RotLeft(Character* character)
{
	//��]
	float angle = character->GetRot().y;
	character->SetRot(Vector3(0, angle + 0.03f, 0));
}

//----------------------------------------------------------------------
//! @brief �E��]�R�}���h
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::RotRightCommand::RotRight(Character* character)
{
	//��]
	float angle = character->GetRot().y;
	character->SetRot(Vector3(0, angle - 0.03f, 0));
}

//----------------------------------------------------------------------
//! @brief �e���˃R�}���h
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::BulletCommand::BulletsFire(Character* character)
{
	dynamic_cast<Player*>(character)->ParentFreed();
}
//----------------------------------------------------------------------
//! @brief ���U�R�}���h
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::BreakUpCommand::BreakUp(Character* character)
{
	Command* advance = new AdvanceCommand();
	Command* back = new BackCommand();

	advance->Execute(character);
	back->Execute(character);

	delete(advance);
	delete(back);
}

//----------------------------------------------------------------------
//! @brief �W���R�}���h
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::AssemblyCommand::Assembly(Character* character)
{
	float angle = character->GetRot().y;
	Matrix rotmat = Matrix::CreateRotationY(angle);
	Vector3 moveV(0, 0, -0.1f);
	moveV = Vector3::TransformNormal(moveV, rotmat);
	Vector3 pos = character->GetTrans();
	character->SetTrans(pos + moveV);
	for (int i = 1; i < Player::PLAYER_PARTS::PLAYER_PARTS_NUM; i++)
	{
		if (character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z > 1.0f ||
			character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z < -1.0f ||
			character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x > 1.0f ||
			character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x < -1.0f
			)
		{
			float angle = character->GetObj3d(i)->GetRot().y;
			Matrix rotmat = Matrix::CreateRotationY(angle);
			Vector3 moveV(0, 0, -0.05f);
			if (character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z > 1.5f ||
				character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z < -1.5f ||
				character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x > 1.5f ||
				character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x < -1.5f)
			{
				moveV = Vector3(0, 0, -0.1f);
			}
			moveV = Vector3::TransformNormal(moveV, rotmat);
			Vector3 pos = character->GetObj3d(i)->GetTrans();
			character->GetObj3d(i)->SetTrans(pos + moveV);
		}
		character->GetObj3d(i)->SetRot(Vector3(0, atan2f((character->GetObj3d(i - 1)->GetTrans().x - character->GetObj3d(i)->GetTrans().x),
			(character->GetObj3d(i - 1)->GetTrans().z - character->GetObj3d(i)->GetTrans().z)), 0));
	}

	for (int i = 0; i < Player::PLAYER_PARTS::PLAYER_PARTS_NUM; i++)
	{
		float angle = character->GetObj3d(i)->GetRot().y;
		Matrix rotmat = Matrix::CreateRotationY(angle);
		Vector3 moveV(0, 0, 0.1f);
		moveV = Vector3::TransformNormal(moveV, rotmat);
		Vector3 pos = character->GetObj3d(i)->GetTrans();
		character->GetObj3d(i)->SetTrans(pos + moveV);
	}
	for (int i = 1; i < Player::PLAYER_PARTS::PLAYER_PARTS_NUM; i++)
	{
		character->GetObj3d(i)->SetRot(Vector3(0, atan2f((character->GetObj3d(i)->GetTrans().x - character->GetObj3d(i - 1)->GetTrans().x),
			(character->GetObj3d(i)->GetTrans().z - character->GetObj3d(i - 1)->GetTrans().z)), 0));
	}
}

//----------------------------------------------------------------------
//! @brief �����蔻��R�}���h
//!
//! @param[in] Character�̃|�C���^
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void PlayerCommands::CollisionCommand::CollisionChange(Character * character)
{
	//character->ChangeCollision();
}
