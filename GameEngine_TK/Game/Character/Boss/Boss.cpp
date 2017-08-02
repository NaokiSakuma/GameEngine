//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Boss.cpp
//!
//! @brief  ボスのソースファイル
//!
//! @date   2017/07/28
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Boss.h"
#include "..\..\Device\Device.h"
#include "..\..\DebugFont\DebugFont.h"
//名前空間
using namespace DirectX;
using namespace SimpleMath;

const int Boss::BULLETCOUNT = 30;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Boss::Boss()
	:m_timer(0)
	, m_DistAngle(0)
	,m_wallCount(0)
	,m_bulletCount(0)
	,m_hp(100)
{

}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
Boss::~Boss()
{

}

//----------------------------------------------------------------------
//! @brief 初期化
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Boss::Initialize()
{

	//要素数分にリサイズする
	m_obj3d.resize(BOSS_PARTS_NUM);
	//自機パーツの読み込み
	m_obj3d[BOSS].LoadModel(L"Resources/boss.cmo");

	//親子関係

	//子パーツの親からのオフセット(座標のズレ)をセット
	m_obj3d[BOSS].SetTrans(Vector3(10, 10, 10));

	//大きさ
	//for (int i = 0; i < BOSS_PARTS_NUM; i++)
	{
		m_obj3d[BOSS].SetScale(Vector3(30, 30, 30));
	}
	//m_obj3d[HEAD2].SetScale(Vector3(2, 4, 2));


	//当たり判定
	{
		m_collisionNode = new CollisionNode::SphereNode();
		m_collisionNode->Initialize();
		m_collisionNode->SetParent(&m_obj3d[BOSS]);
		m_collisionNode->SetTrans(Vector3(0, 0.15f, 0));
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
void Boss::Update()
{
	//動作の実行
	//m_state->Execute(this);
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;
		//目標角をランダムに抽選
		float rnd =  (float)rand() / RAND_MAX - 0.5f;
		//-90～90の乱数
		//rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_DistAngle += rnd;
	}
	{
		//自機の角度を回転
		Vector3 rot = GetRot();
		//今の角度
		float angle;
		if (rot.y < -3 )
			angle = m_DistAngle - rot.y;
		else
			angle = m_DistAngle - rot.y + XMConvertToRadians(180);
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}
		//補間
		rot.y += angle * 0.01f;
		SetRot(rot);
	}
	{//	機体の向いている方向に進む
		Vector3 trans = m_obj3d[BOSS].GetTrans();
		Vector3 moveV(0, 0, -0.1f);
		Vector3 rotv = m_obj3d[BOSS].GetRot();
		Matrix rotm = Matrix::CreateRotationY(rotv.y);
		moveV = Vector3::TransformNormal(moveV, rotm);
		trans += moveV;
		m_obj3d[BOSS].SetTrans(trans);
	}
	//行列の更新
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Update();
	}
	//当たり判定
	m_collisionNode->Update();
	{
		if (m_wallCount > 0)
		{
			m_wallCount--;
		}
	}
	{//テスト
		m_bulletCount++;
		//if (Device::m_keyboardState.Space)
		ParentFreed();
	}
	//弾の更新
	for (auto& itr : m_bullet)
		itr->Update();

	for (std::vector<std::unique_ptr<Bullet>>::iterator itr = m_bullet.begin(); itr != m_bullet.end();)
	{
		if ((*itr)->GetDeleteCount() > 300)
		{
			itr = m_bullet.erase(itr);
		}
		else
		{
			itr++;
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
void Boss::Render()
{
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Render();
	}
	//弾丸用の当たり判定の描画
	if (m_isRenderColl)
	{
		//m_collisionNode->Render();
	}
	//m_collisionNode->Render();
	for (auto& itr : m_bullet)
		itr->Render();
}

//----------------------------------------------------------------------
//! @brief クリエイト関数
//!
//! @param[in] なし
//!
//! @return Playerのポインタ
//----------------------------------------------------------------------
std::unique_ptr<Boss> Boss::Create()
{
	//std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>();
	std::unique_ptr<Boss> boss(new Boss());
	boss->Initialize();

	return boss;
}

const int Boss::GetBulletNum() const
{
	return static_cast<int>(m_bullet.size());
}

const std::unique_ptr<Bullet>& Boss::GetBullet(int i) const
{
	return std::move(m_bullet[i]);
}

void Boss::ParentFreed()
{
	if (m_bulletCount >= BULLETCOUNT)
	{
		m_bullet.push_back(std::move(Bullet::Create(this->GetTrans(), this->GetRot(), this->GetWorld(),false)));
		m_bulletCount = 0;
	}

}

void Boss::DeleteBullet(int i)
{
	m_bullet.erase(m_bullet.begin() + i);
}

void Boss::DecreaseHP()
{
	m_hp--;
}

//----------------------------------------------------------------------
//! @brief ステイトの変更
//!
//! @param[in] 変更先のステイト
//!
//! @return なし
//----------------------------------------------------------------------
void Boss::ChangeState(State* state)
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
void Boss::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_obj3d[BOSS].SetScale(scale);
}

//----------------------------------------------------------------------
//! @brief 回転のSetter
//!
//! @param[in] 回転
//!
//! @return なし
//----------------------------------------------------------------------
void Boss::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_obj3d[BOSS].SetRot(rot);
}

//----------------------------------------------------------------------
//! @brief 平行移動のSetter
//!
//! @param[in] 平行移動
//!
//! @return なし
//----------------------------------------------------------------------
void Boss::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	m_obj3d[BOSS].SetTrans(trans);
}

//----------------------------------------------------------------------
//! @brief スケーリングのGetter
//!
//! @param[in] なし
//!
//! @return スケーリング
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Boss::GetScale() const
{
	return m_obj3d[BOSS].GetScale();
}

//----------------------------------------------------------------------
//! @brief 回転のGetter
//!
//! @param[in] なし
//!
//! @return 回転
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Boss::GetRot() const
{
	return m_obj3d[BOSS].GetRot();
}

//----------------------------------------------------------------------
//! @brief 平行移動のGetter
//!
//! @param[in] なし
//!
//! @return 平行移動
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Boss::GetTrans() const
{
	return m_obj3d[BOSS].GetTrans();
}

//----------------------------------------------------------------------
//! @brief ワールド行列のGetter
//!
//! @param[in] なし
//!
//! @return ワールド行列
//----------------------------------------------------------------------
const DirectX::SimpleMath::Matrix& Boss::GetWorld() const
{
	return m_obj3d[BOSS].GetWorld();
}

//----------------------------------------------------------------------
//! @brief オブジェクトのGetter
//!
//! @param[in] オブジェクトの要素番号
//!
//! @return オブジェクトのポインタ
//----------------------------------------------------------------------
Obj3d* Boss::GetObj3d(int num)
{
	return &m_obj3d[num];
}
