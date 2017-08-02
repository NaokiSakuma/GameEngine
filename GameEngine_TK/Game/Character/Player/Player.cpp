//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.cpp
//!
//! @brief  プレイヤーのソースファイル
//!
//! @date   2017/05/11 
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Player.h"
#include "..\..\State\PlayerState\PlayerState.h"
#include "..\..\Device\Device.h"
#include "..\..\Game.h"
#include "..\..\DebugFont\DebugFont.h"
#include "..\..\ADX2Le\ADX2Le.h"
#include "..\..\CueSheet\CueSheet_0.h"

//名前空間
using namespace DirectX;
using namespace SimpleMath;

const int Player::BULLETCOUNT = 10;
//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Player::Player()
	:m_nextFireTimer(0)
	, m_bulletCount(0)
	,m_invincibleTime(0)
{
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
Player::~Player()
{

}

//----------------------------------------------------------------------
//! @brief 初期化
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::Initialize()
{
	//ステイトを取得(立ち)
	ChangeState(PlayerState::Stand::GetInstance());
	//要素数分にリサイズする
	m_obj3d.resize(PLAYER_PARTS_NUM);
	//自機パーツの読み込み
	m_obj3d[HEAD1].LoadModel(L"Resources/Fighter.cmo");
	//親子関係
	//m_obj3d[HEAD9].SetObjParent(&m_obj3d[HEAD1]);
	m_obj3d[HEAD1].SetTrans(Vector3(-10, 5, 0));
	//子パーツの親からのオフセット(座標のズレ)をセット

	//大きさ
	m_obj3d[HEAD1].SetScale(Vector3(2, 2, 2));

	//当たり判定
	{
		m_collisionNode = new CollisionNode::SphereNode();
		m_collisionNode->Initialize();
		m_collisionNode->SetParent(&m_obj3d[HEAD1]);
		dynamic_cast<CollisionNode::SphereNode*>(m_collisionNode)->SetLocalRadius(0.2f);
	}
	//弾
	{
		m_bullet.resize(0);
	}

}

//----------------------------------------------------------------------
//! @brief 更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::Update()
{
	//動作の実行
	m_state->Execute(this);
	//ロックオン切り替えボタン
	if (Device::m_keyboardTracker->pressed.Enter)
	{
		Lockon* lockon = Game::GetInstance()->GetLockon();
		//ロックオンをしている場合
		if (lockon->IsLockOn())
		{
			//ロックオン解除
			lockon->ReleaseLockon();
		}
		else
		{
			//ロックオン開始
			lockon->ToLockon();
		}
	}
	
	Lockon* lockon = Game::GetInstance()->GetLockon();
	//ロックオンしていたら
	if (lockon->IsLockOn())
	{
		ControlLockon();
	}

	//行列の更新
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Update();
	}
	//弾丸用の当たり判定の更新
	m_collisionNode->Update();

	{
		//float angle = GetRot().y;
		//Matrix rotmat = Matrix::CreateRotationY(angle);
		//Vector3 moveV(0, 0, -0.1f);
		//moveV = Vector3::TransformNormal(moveV, rotmat);
		//if (GetRot().x != 0)
		//{
		//	float angleX = GetRot().x;
		//	Matrix rotmatX = Matrix::CreateRotationX(angleX);
		//	Vector3 moveVec(0, -0.1f, 0);
		//	moveVec = Vector3::TransformNormal(moveVec, rotmat);
		//	Vector3 pos = GetTrans();
		//	SetTrans(pos + moveV * moveVec);
		//}
		//else
		//{
		//	Vector3 pos = GetTrans();
		//	SetTrans(pos + moveV);
		//}
		// 現在の座標・回転角を取得
		Vector3 trans = m_obj3d[HEAD1].GetTrans();
		// 移動ベクトル(Z座標前進)
		SimpleMath::Vector3 moveV(0, 0, -0.1f);
		// 移動ベクトルを回転する
		moveV = Vector3::TransformNormal(moveV, m_obj3d[HEAD1].GetWorld());
		// 移動
		trans += moveV;
		// 移動した座標を反映
		m_obj3d[HEAD1].SetTrans(trans);
	}
	////落下中であれば
	//if (m_isJump)
	//{
	//	//重力
	//	m_velocity.y -= GRAVITY_ACC;
	//	//重力の限界速度
	//	if (m_velocity.y < -JUMP_SPEED_MAX)
	//	{
	//		m_velocity.y = -JUMP_SPEED_MAX;
	//	}
	//}
	//{//速度による移動
	//	Vector3 trans = GetTrans();
	//	trans += m_velocity;
	//	SetTrans(trans);

	//}

	{//テスト
		m_bulletCount++;
		if (Device::m_keyboardState.Space)
			ParentFreed();
	}
	//弾の更新
	for (auto& itr : m_bullet)
		itr->Update();

	for (std::vector<std::unique_ptr<Bullet>>::iterator itr = m_bullet.begin(); itr != m_bullet.end();)
	{
		if ((*itr)->GetDeleteCount() > 120)
		{
			itr = m_bullet.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	if (!Device::m_keyboardState.A && !Device::m_keyboardState.D && !lockon->IsLockOn())
	{
		Vector3 angle = this->GetRot();
		if (angle.z > 0.01f)
		{
			this->SetRot(Vector3(angle.x,angle.y,angle.z - 0.03f));
		}
		else if (angle.z < -0.01f)
		{
			this->SetRot(Vector3(angle.x, angle.y, angle.z + 0.03f));
		}
	}
	if (!Device::m_keyboardState.W && !Device::m_keyboardState.S && !lockon->IsLockOn())
	{
		Vector3 angle = this->GetRot();
		if (angle.x > 0.01f)
		{
			this->SetRot(Vector3(angle.x - 0.01f, angle.y, angle.z ));
		}
		else if (angle.x < -0.01f)
		{
			this->SetRot(Vector3(angle.x + 0.01f, angle.y, angle.z));
		}
	}

	{//無敵時間用
		if (m_invincibleTime > 0)
		{
			lockon->ReleaseLockon();
			m_invincibleTime--;
		}
	}
}

//----------------------------------------------------------------------
//! @brief 描画
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::Render()
{
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		if (m_invincibleTime > 0)
		{
			if (m_invincibleTime % 15 == 0)
				it->Render();
		}
		else if(m_invincibleTime == 0)
			it->Render();
		
	}
	//弾の描画
	for (auto& itr : m_bullet)
		itr->Render();

}

//----------------------------------------------------------------------
//! @brief 弾丸を装填
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::ResetBullet()
{
	for (int i = 0; i < BULLETNUM; i++)
	{
		m_bullet[i]->SetObjParent(&m_obj3d[HEAD1]);
		m_bullet[i]->SetScale(Vector3(0.5f, 0.5f, 0.5f));
		m_bullet[i]->SetRot(Vector3(0, 0, 0));
		m_bullet[i]->SetTrans(Vector3(0, 0.1f, 0));
	}
	m_nextFireTimer = 0;
}

//----------------------------------------------------------------------
//! @brief クリエイト関数
//!
//! @param[in] なし
//!
//! @return Playerのポインタ
//----------------------------------------------------------------------
Player* Player::Create()
{
	Player* player = new Player;
	player->Initialize();

	return player;
}

//----------------------------------------------------------------------
//! @brief 弾を発射する
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Player::ParentFreed()
{

	if (m_bulletCount >= BULLETCOUNT)
	{
		m_bullet.push_back(std::move(Bullet::Create(this->GetTrans(),this->GetRot(),this->GetWorld(),true)));
		m_bulletCount = 0;
		ADX2Le::Play(CRI_CUESHEET_0_SHOT1);
	}
}

void Player::DeleteBullet(int i)
{
	m_bullet.erase(m_bullet.begin() + i);
}

void Player::ControlLockon()
{
	Lockon* lockon = Game::GetInstance()->GetLockon();
	assert(lockon);
	assert(lockon->IsLockOn());
	Enemy* enemy = lockon->SearchEnemy();
	if (!enemy) return;

	{
		Vector3 player2enemy = enemy->GetTrans() - this->GetTrans();
		float rotX = atan2f(player2enemy.y, player2enemy.x);
		float rotY = atan2f(-player2enemy.x, -player2enemy.z);
		if(rotX > -XM_PIDIV2)
			m_obj3d[HEAD1].SetRot(Vector3(rotX, rotY, 0));
		else if (rotX < -XM_PIDIV2)
			m_obj3d[HEAD1].SetRot(Vector3(-rotX - XMConvertToRadians(180), rotY, 0));

	}

	// 左右移動
	if (Device::m_keyboardState.A)
	{
		// 現在の座標・回転角を取得
		Vector3 trans = m_obj3d[HEAD1].GetTrans();
		float rot_y = m_obj3d[HEAD1].GetRot().y;
		// 移動ベクトル(Z座標前進)
		SimpleMath::Vector3 moveV(-0.1f, 0, 0);
		Matrix rotm = Matrix::CreateRotationY(rot_y);
		// 移動ベクトルを回転する
		moveV = Vector3::TransformNormal(moveV, rotm);
		// 移動
		trans += moveV;
		// 移動した座標を反映
		m_obj3d[HEAD1].SetTrans(trans);
	}
	if (Device::m_keyboardState.D)
	{
		// 現在の座標・回転角を取得
		Vector3 trans = m_obj3d[HEAD1].GetTrans();
		float rot_y = m_obj3d[HEAD1].GetRot().y;
		// 移動ベクトル(Z座標前進)
		SimpleMath::Vector3 moveV(0.1f, 0, 0);
		Matrix rotm = Matrix::CreateRotationY(rot_y);
		// 移動ベクトルを回転する
		moveV = Vector3::TransformNormal(moveV, rotm);
		// 移動
		trans += moveV;
		// 移動した座標を反映
		m_obj3d[HEAD1].SetTrans(trans);
	}

}


//----------------------------------------------------------------------
//! @brief ステイトの変更
//!
//! @param[in] 変更先のステイト
//!
//! @return なし
//----------------------------------------------------------------------
void Player::ChangeState(State* state)
{
	m_state = state;
}

//----------------------------------------------------------------------
//! @brief スケーリングのSetter
//!
//! @param[in] スケーリング
//!
//! @return なし
//----------------------------------------------------------------------
void Player::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_obj3d[HEAD1].SetScale(scale);
}

//----------------------------------------------------------------------
//! @brief 回転のSetter
//!
//! @param[in] 回転
//!
//! @return なし
//----------------------------------------------------------------------
void Player::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_obj3d[HEAD1].SetRot(rot);
}

//----------------------------------------------------------------------
//! @brief 平行移動のSetter
//!
//! @param[in] 平行移動
//!
//! @return なし
//----------------------------------------------------------------------
void Player::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	m_obj3d[HEAD1].SetTrans(trans);
}

//----------------------------------------------------------------------
//! @brief スケーリングのGetter
//!
//! @param[in] なし
//!
//! @return スケーリング
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Player::GetScale() const
{
	return m_obj3d[HEAD1].GetScale();
}

//----------------------------------------------------------------------
//! @brief 回転のGetter
//!
//! @param[in] なし
//!
//! @return 回転
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Player::GetRot() const
{
	return m_obj3d[HEAD1].GetRot();
}

//----------------------------------------------------------------------
//! @brief 平行移動のGetter
//!
//! @param[in] なし
//!
//! @return 平行移動
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Player::GetTrans() const
{
	return m_obj3d[HEAD1].GetTrans();
}

//----------------------------------------------------------------------
//! @brief ワールド行列のGetter
//!
//! @param[in] なし
//!
//! @return ワールド行列
//----------------------------------------------------------------------
const DirectX::SimpleMath::Matrix& Player::GetWorld() const
{
	return m_obj3d[HEAD1].GetWorld();
}

//----------------------------------------------------------------------
//! @brief オブジェクトのGetter
//!
//! @param[in] オブジェクトの要素番号
//!
//! @return オブジェクトのポインタ
//----------------------------------------------------------------------
Obj3d* Player::GetObj3d(int num)
{
	return &m_obj3d[num];
}


const int Player::GetBulletNum() const
{
	return static_cast<int>(m_bullet.size());
}

const std::unique_ptr<Bullet>& Player::GetBullet(int i) const
{
	return std::move(m_bullet[i]);
}
