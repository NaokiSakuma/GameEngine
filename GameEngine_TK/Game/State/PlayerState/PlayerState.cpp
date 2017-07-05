//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   PlayerState.cpp
//!
//! @brief  プレイヤーのステイトのソースファイル
//!
//! @date   2017/05/11 (06/04更新：Y.Yamada)
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
	//キーボードの状態を取得
	Keyboard::State key = Device::m_keyboard->GetState();
	Device::m_keyboardTracker.Update(key);

	//Wキーが押されていたら
	//Sキーが押されたら
	if (key.W || key.S)
	{
		//Move状態へ移行
		character->ChangeState(PlayerState::Move::GetInstance());
	}
	//Aキーが押されたら
	if (key.A)
	{
		//回転
		m_putKeyA->Execute(character);
	}
	//Dキーが押されたら
	if (key.D)
	{
		//回転
		m_putKeyD->Execute(character);
	}
	//Spaceキーが押されたら
	if (Device::m_keyboardTracker.pressed.Space)
	{
		//弾発射
		m_putKeySpace->Execute(character);
	}
	//Qキーが押されたら
	else if (key.Q)
	{
		//解散
		m_putKeyQ->Execute(character);
	}
	//Eキーが押されたら
	else if (key.E)
	{
		//集合
		m_putKeyE->Execute(character);
	}
	if (Device::m_keyboardTracker.pressed.D1)
	{
		m_putKey1->Execute(character);
	}
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
	m_putKeyA = new PlayerCommands::RotLeftCommand;
	m_putKeyD = new PlayerCommands::RotRightCommand;
	m_putKeyS = new PlayerCommands::BackCommand;
	m_putKeyW = new PlayerCommands::AdvanceCommand;
	m_putKeySpace = new PlayerCommands::BulletCommand;
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
	//キーボードの状態を取得
	Keyboard::State key = Device::m_keyboard->GetState();
	Device::m_keyboardTracker.Update(key);

	//Wキーが押されていたら
	if (key.W)
	{
		//前進
		m_putKeyW->Execute(character);
	}
	//Sキーが押されたら
	if (key.S)
	{
		//後退
		m_putKeyS->Execute(character);
	}
	//Aキーが押されたら
	if (key.A)
	{
		//回転
		m_putKeyA->Execute(character);
	}
	//Dキーが押されたら
	if (key.D)
	{
		//回転
		m_putKeyD->Execute(character);
	}
	//Spaceキーが押されたら
	if (Device::m_keyboardTracker.pressed.Space)
	{
		//弾発射
		m_putKeySpace->Execute(character);
	}
	//WもSも押されていなかったら
	if (!key.S && !key.W)
	{
		//Stand状態へ移行
		character->ChangeState(PlayerState::Stand::GetInstance());
	}
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
