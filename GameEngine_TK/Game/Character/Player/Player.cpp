//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.cpp
//!
//! @brief  プレイヤーのソースファイル
//!
//! @date   2017/05/11 (06/04更新：Y.Yamada)
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Player.h"
#include "..\..\State\PlayerState\PlayerState.h"
#include "..\..\Device\Device.h"

//名前空間
using namespace DirectX;
using namespace SimpleMath;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Player::Player()
	:m_nextFireTimer(0)
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
	//弾の生成
	m_bullet = Bullet::Create();
	//ステイトを取得(立ち)
	ChangeState(PlayerState::Stand::GetInstance());
	//要素数分にリサイズする
	m_obj3d.resize(PLAYER_PARTS_NUM);
	//自機パーツの読み込み
	m_obj3d[HEAD1].LoadModel(L"Resources/head.cmo");
	for (int i = 1; i < PLAYER_PARTS_NUM; i++)
	{
		m_obj3d[i].LoadModel(L"Resources/head2.cmo");
	}
	//親子関係
	//m_obj3d[HEAD9].SetObjParent(&m_obj3d[HEAD1]);
	m_bullet->SetObjParent(&m_obj3d[HEAD1]);
	//子パーツの親からのオフセット(座標のズレ)をセット
	m_obj3d[HEAD2].SetTrans(Vector3(0, 0, 2));

	//大きさ
	m_obj3d[HEAD1].SetScale(Vector3(2, 2, 2));
	m_obj3d[HEAD2].SetScale(Vector3(2, 2, 2));


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
	//行列の更新
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Update();
	}
	//弾の移動
	m_bullet->Update();

	//弾に親がいない場合
	if (!m_bullet->GetObjParent())
	{
		m_nextFireTimer++;
		if (m_nextFireTimer > 120)
			//装填
			ResetBullet();
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
		it->Render();
	}
	//弾の描画
	m_bullet->Render();
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
	//
	m_bullet->SetObjParent(&m_obj3d[HEAD1]);
	m_bullet->SetScale(Vector3(2, 2, 2));
	m_bullet->SetRot(Vector3(0, 0, 0));
	m_bullet->SetTrans(Vector3(0, 1, 0));
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

void Player::ParentFreed()
{
	if (m_bullet->GetObjParent())
		m_bullet->SetObjParent(nullptr);
	else if (!m_bullet->GetObjParent())
		ResetBullet();
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
