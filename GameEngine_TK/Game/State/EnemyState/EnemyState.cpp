//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   EnemyState.cpp
//!
//! @brief  敵のステイトのソースファイル
//!
//! @date   2017/06/12
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "EnemyState.h"
#include "..\..\Character\Enemy\Enemy.h"

using namespace DirectX;
using namespace SimpleMath;

//初期化
State* EnemyState::Stand::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief インスタンスの取得
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
State* EnemyState::Stand::GetInstance()
{
	if (!m_state)
	{
		m_state = new Stand();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief 動作の実行
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void EnemyState::Stand::Execute(Character* character)
{
	//処理
	//Move状態へ移行
	character->ChangeState(EnemyState::Move::GetInstance());
}

//----------------------------------------------------------------------
//! @brief オブジェクトの破棄
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void EnemyState::Stand::Dispose()
{
	if (m_state)
		delete m_state;
}

//初期化
State* EnemyState::Move::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief インスタンスの取得
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
State* EnemyState::Move::GetInstance()
{
	if (!m_state)
	{
		m_state = new Move();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief 動作の実行
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void EnemyState::Move::Execute(Character* character)
{
	//処理
	float angle = character->GetRot().y;
	Matrix rotmat = Matrix::CreateRotationY(angle);
	Vector3 moveV(0, 0, -0.05f);
	moveV = Vector3::TransformNormal(moveV, rotmat);
	Vector3 pos = character->GetTrans();
	character->SetTrans(pos + moveV);
	for (int i = 1; i < Enemy::ENEMY_PARTS::ENEMY_PARTS_NUM; i++)
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

	//Stand状態へ移行
	//character->ChangeState(EnemyState::Stand::GetInstance());
}

//----------------------------------------------------------------------
//! @brief オブジェクトの破棄
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void EnemyState::Move::Dispose()
{
	if (m_state)
		delete m_state;
}