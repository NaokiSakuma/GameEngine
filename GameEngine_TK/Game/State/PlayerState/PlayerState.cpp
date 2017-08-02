//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerState.cpp
//!
//! @brief  プレイヤーのステイトのソースファイル
//!
//! @date   2017/05/11 
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "PlayerState.h"
#include "..\..\Character\Player\Player.h"
#include "..\..\Device\Device.h"

using namespace DirectX;
using namespace SimpleMath;

//初期化
State*  PlayerState::Stand::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief インスタンスの取得
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
State* PlayerState::Stand::GetInstance()
{
	if (!m_state)
	{
		m_state = new Stand();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief 初期化
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Stand::Initialize()
{
	//各キーにコマンドを割り当て
	m_putKeyA     = new PlayerCommands::RotLeftCommand;
	m_putKeyD     = new PlayerCommands::RotRightCommand;
	m_putKeyE     = new PlayerCommands::AssemblyCommand;
	m_putKeyQ     = new PlayerCommands::BreakUpCommand;
	m_putKeySpace = new PlayerCommands::BulletCommand;
	m_putKey1	  = new PlayerCommands::CollisionCommand;
	m_putKeyZ     = new PlayerCommands::JumpCommand;
}

//----------------------------------------------------------------------
//! @brief 動作の実行
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Stand::Execute(Character * character)
{

	//Wキーが押されていたら
	//Sキーが押されたら
	if (Device::m_keyboardState.W || Device::m_keyboardState.S)
	{
		//Move状態へ移行
		character->ChangeState(PlayerState::Move::GetInstance());
	}
	//Aキーが押されたら
	if (Device::m_keyboardState.A)
	{
		//回転
		m_putKeyA->Execute(character);
	}
	//Dキーが押されたら
	if (Device::m_keyboardState.D)
	{
		//回転
		m_putKeyD->Execute(character);
	}
	//Spaceキーが押されたら
	if (Device::m_keyboardTracker->pressed.Space)
	{
		//弾発射
		m_putKeySpace->Execute(character);
	}
	//Qキーが押されたら
	else if (Device::m_keyboardState.Q)
	{
		//解散
		m_putKeyQ->Execute(character);
	}
	//Eキーが押されたら
	else if (Device::m_keyboardState.E)
	{
		//集合
		m_putKeyE->Execute(character);
	}
	//デバッグ文字表示
	if (Device::m_keyboardTracker->pressed.D1)
	{
		m_putKey1->Execute(character);
	}
	//ジャンプ
	if (Device::m_keyboardState.Z)
	{
		//Jump状態へ移行
		character->ChangeState(PlayerState::Jump::GetInstance());
		//ジャンプさせる
		m_putKeyZ->Execute(character);
	}
	////ロックオン
	//if (Device::m_keyboardState.Enter)
	//{
	//	character->ChangeState(PlayerState::LockOn::GetInstance());
	//}
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
PlayerState::Stand::~Stand()
{
	delete m_putKeyA;
	delete m_putKeyD;
	delete m_putKeyE;
	delete m_putKeyQ;
	delete m_putKeySpace;
	delete m_putKey1;
	delete m_putKeyZ;
}

//----------------------------------------------------------------------
//! @brief オブジェクトの破棄
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Stand::Dispose()
{
	if (m_state)
		delete m_state;
}

//初期化
State*  PlayerState::Move::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief インスタンスの取得
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
State * PlayerState::Move::GetInstance()
{
	if (!m_state)
	{
		m_state = new Move();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief 初期化
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Move::Initialize()
{
	//各キーにコマンドを割り当て
	m_putKeyA     = new PlayerCommands::RotLeftCommand;
	m_putKeyD     = new PlayerCommands::RotRightCommand;
	m_putKeyS     = new PlayerCommands::BackCommand;
	m_putKeyW     = new PlayerCommands::AdvanceCommand;
	m_putKeySpace = new PlayerCommands::BulletCommand;
	m_putKeyZ     = new PlayerCommands::JumpCommand;
}

//----------------------------------------------------------------------
//! @brief 動作の実行
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Move::Execute(Character * character)
{
	//Wキーが押されていたら
	if (Device::m_keyboardState.W)
	{
		//前進
		m_putKeyW->Execute(character);
	}
	//Sキーが押されたら
	if (Device::m_keyboardState.S)
	{
		//後退
		m_putKeyS->Execute(character);
	}
	//Aキーが押されたら
	if (Device::m_keyboardState.A)
	{
		//回転
		m_putKeyA->Execute(character);
	}
	//Dキーが押されたら
	if (Device::m_keyboardState.D)
	{
		//回転
		m_putKeyD->Execute(character);
	}
	//Spaceキーが押されたら
	if (Device::m_keyboardTracker->pressed.Space)
	{
		//弾発射
		m_putKeySpace->Execute(character);
	}
	//WもSも押されていなかったら
	if (!Device::m_keyboardState.S && !Device::m_keyboardState.W && !Device::m_keyboardState.Z)
	{
		//Stand状態へ移行
		character->ChangeState(PlayerState::Stand::GetInstance());
	}
	//ジャンプ
	if (Device::m_keyboardState.Z)
	{
		//Jump状態へ移行
		character->ChangeState(PlayerState::Jump::GetInstance());
		m_putKeyZ->Execute(character);
	}
	////ロックオン
	//if (Device::m_keyboardState.Enter)
	//{
	//	//Lockon状態へ移行
	//	character->ChangeState(PlayerState::LockOn::GetInstance());
	//}
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
PlayerState::Move::~Move()
{
	delete m_putKeyA;
	delete m_putKeyD;
	delete m_putKeyW;
	delete m_putKeyS;
	delete m_putKeySpace;
	delete m_putKeyZ;
}

//----------------------------------------------------------------------
//! @brief オブジェクトの破棄
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Move::Dispose()
{
	if (m_state)
		delete m_state;
}

//初期化
State* PlayerState::Jump::m_state = nullptr;

//----------------------------------------------------------------------
//! @brief インスタンスの取得
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
State * PlayerState::Jump::GetInstance()
{
	if (!m_state)
	{
		m_state = new Jump();
		m_state->Initialize();
	}
	return m_state;
}

//----------------------------------------------------------------------
//! @brief 初期化
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Jump::Initialize()
{
	//各キーにコマンドを割り当て
	m_putKeyA     = new PlayerCommands::RotLeftCommand;
	m_putKeyD     = new PlayerCommands::RotRightCommand;
	m_putKeyS     = new PlayerCommands::BackCommand;
	m_putKeyW     = new PlayerCommands::AdvanceCommand;
	m_putKeySpace = new PlayerCommands::BulletCommand;
	m_putKeyZ     = new PlayerCommands::JumpCommand;
}

//----------------------------------------------------------------------
//! @brief 動作の実行
//!
//! @param[in] キャラクター
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Jump::Execute(Character * character)
{
	//Wキーが押されていたら
	if (Device::m_keyboardState.W)
	{
		//前進
		m_putKeyW->Execute(character);
	}
	//Sキーが押されたら
	if (Device::m_keyboardState.S)
	{
		//後退
		m_putKeyS->Execute(character);
	}
	//Aキーが押されたら
	if (Device::m_keyboardState.A)
	{
		//回転
		m_putKeyA->Execute(character);
	}
	//Dキーが押されたら
	if (Device::m_keyboardState.D)
	{
		//回転
		m_putKeyD->Execute(character);
	}
	//Spaceキーが押されたら
	if (Device::m_keyboardTracker->pressed.Space)
	{
		//弾発射
		m_putKeySpace->Execute(character);
	}
	//ジャンプしていなかったらstand状態へ移行
	if (!character->GetIsJump())
	{
		character->ChangeState(PlayerState::Stand::GetInstance());
	}
	//ジャンプ
	//if (Device::m_keyboardTracker->pressed.Z)
	//{
		//Jump
		//m_putKeyZ->Execute(character);
	//}
	//WもSもZも押されていなかったら
	//else if (!Device::m_keyboardState.Z)
	//{
	//	int a = 0;
	//	a++;
	//}
	//{
	//	if(key.W || key.S)
	//		character->ChangeState(PlayerState::Move::GetInstance());
	//	//Stand状態へ移行
	//	if (!key.W && !key.S)
	//		character->ChangeState(PlayerState::Stand::GetInstance());
	//}
	//else //if(!Device::m_keyboardTracker.pressed.Z)
	//{
	//	//character->ChangeState(PlayerState::Stand::GetInstance());
	//}
	//ロックオン
	//if (Device::m_keyboardState.Enter)
	//{
	//	character->ChangeState(PlayerState::LockOn::GetInstance());
	//}
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
PlayerState::Jump::~Jump()
{
	delete m_putKeyA;
	delete m_putKeyD;
	delete m_putKeyW;
	delete m_putKeyS;
	delete m_putKeySpace;
}

//----------------------------------------------------------------------
//! @brief オブジェクトの破棄
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void PlayerState::Jump::Dispose()
{
	if (m_state)
		delete m_state;
}

////初期化
//State*  PlayerState::LockOn::m_state = nullptr;
//
////----------------------------------------------------------------------
////! @brief デストラクタ
////----------------------------------------------------------------------
//PlayerState::LockOn::~LockOn()
//{
//	delete m_putKeyA;
//	delete m_putKeyD;
//	delete m_putKeyW;
//	delete m_putKeyS;
//	delete m_putKeySpace;
//	delete m_putKeyZ;
//	delete m_putKeyEnter;
//
//}
//
////----------------------------------------------------------------------
////! @brief インスタンスの取得
////!
////! @param[in] なし
////!
////! @return なし
////----------------------------------------------------------------------
//State * PlayerState::LockOn::GetInstance()
//{
//	if (!m_state)
//	{
//		m_state = new LockOn();
//		m_state->Initialize();
//	}
//	return m_state;
//}
//
////----------------------------------------------------------------------
////! @brief 初期化
////!
////! @param[in] キャラクター
////!
////! @return なし
////----------------------------------------------------------------------
//void PlayerState::LockOn::Initialize()
//{
//	m_putKeyA = new PlayerCommands::RotLeftCommand;
//	m_putKeyD = new PlayerCommands::RotRightCommand;
//	m_putKeyS = new PlayerCommands::BackCommand;
//	m_putKeyW = new PlayerCommands::AdvanceCommand;
//	m_putKeySpace = new PlayerCommands::BulletCommand;
//	m_putKeyZ = new PlayerCommands::JumpCommand;
//	m_putKeyEnter = new PlayerCommands::LockonCommand;
//}
//
////----------------------------------------------------------------------
////! @brief 動作の実行
////!
////! @param[in] キャラクター
////!
////! @return なし
////----------------------------------------------------------------------
//void PlayerState::LockOn::Execute(Character * character)
//{
//	//Wキーが押されていたら
//	if (Device::m_keyboardState.W)
//	{
//		//前進
//		m_putKeyW->Execute(character);
//	}
//	//Sキーが押されたら
//	if (Device::m_keyboardState.S)
//	{
//		//後退
//		m_putKeyS->Execute(character);
//	}
//	//Aキーが押されたら
//	if (Device::m_keyboardState.A)
//	{
//		//回転
//		m_putKeyA->Execute(character);
//	}
//	//Dキーが押されたら
//	if (Device::m_keyboardState.D)
//	{
//		//回転
//		m_putKeyD->Execute(character);
//	}
//	//Spaceキーが押されたら
//	if (Device::m_keyboardTracker->pressed.Space)
//	{
//		//弾発射
//		m_putKeySpace->Execute(character);
//	}
//	//WもSもZも押されていなかったら
//	if (!Device::m_keyboardState.S && !Device::m_keyboardState.W && !Device::m_keyboardState.Z &&!Device::m_keyboardState.Enter)
//	{
//		//Stand状態へ移行
//		character->ChangeState(PlayerState::Stand::GetInstance());
//	}
//	//ジャンプ
//	if (Device::m_keyboardTracker->pressed.Z)
//	{
//		//Jump
//		m_putKeyZ->Execute(character);
//	}
//	//ロックオン
//	if (Device::m_keyboardState.Enter)
//	{
//		m_putKeyEnter->Execute(character);
//	}
//	else
//
//}
//
////----------------------------------------------------------------------
////! @brief オブジェクトの破棄
////!
////! @param[in] なし
////!
////! @return なし
////----------------------------------------------------------------------
//void PlayerState::LockOn::Dispose()
//{
//	if (m_state)
//		delete m_state;
//}
