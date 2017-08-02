//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Bullet.h
//!
//! @brief  弾のヘッダーファイル
//!
//! @date   2017/06/13
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Bullet.h"
#include "..\..\DebugFont\DebugFont.h"
#include <ctime>
//名前空間
using namespace DirectX;
using namespace DirectX::SimpleMath;

//----------------------------------------------------------------------
//! @brief イニシャライズ
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Bullet::Initialize(DirectX::SimpleMath::Vector3 trans, DirectX::SimpleMath::Vector3 rot ,DirectX::SimpleMath::Matrix world, bool isPlayer)
{
	this->LoadModel(L"Resources/head.cmo");

	{//弾本体
		this->SetTrans(trans);
		this->SetRot(rot);
		if(!isPlayer)
			this->SetScale(Vector3(10, 10, 10));
		Obj3d::Update();
		//Vector3 transVec = trans;
		m_translation = trans;
		srand(static_cast<unsigned int>(time(nullptr)));

		if (isPlayer)
			m_bulletVel = Vector3(0, 0, -1.5f);
		else
		{
			m_bulletVel = Vector3(x, y, z);
		}
		m_bulletVel = Vector3::TransformNormal(m_bulletVel, m_world);
	}
	{//弾の当たり判定
		m_collisionNodeBullet = new CollisionNode::SphereNode(); //std::make_unique<CollisionNode::SphereNode>();
		//弾丸用の当たり判定ノードの設定
		m_collisionNodeBullet->Initialize();
		//親パーツを設定
		m_collisionNodeBullet->SetParent(this);
		//スケールを変える
		m_collisionNodeBullet->SetLocalRadius(0.3f);
		//オフセット
		m_collisionNodeBullet->SetTrans(Vector3(0, 0.2f, 0));
	}
}

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Bullet::Bullet()
	:m_deleteCount(0)
	//,m_enemy(nullptr)
{
	m_ObjParent = nullptr;
	m_scale = Vector3(0.5f, 0.5f, 0.5f);
	m_translation = Vector3(0, 0, 0);
	m_UseQuaternion = false;
	x = (rand() % 21 - 10) / 1000.0f;
	y = (rand() % 21 - 10) / 1000.0f;
	z = (rand() % 21 - 10) / 1000.0f;
}

//----------------------------------------------------------------------
//! @brief 更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Bullet::Update()
{
	////抽出した情報をしまっておく変数
	//Vector3 scale;		//ワールドスケーリング
	//Quaternion rotation;//ワールド回転
	//Vector3 translation;//ワールド座標

	////ワールド座標から各要素を抽出
	//m_world.Decompose(scale, rotation, translation);

	//親がいない場合
	if (!m_ObjParent)
	{
		////親パーツから分離、独立させる
		//m_scale = scale;
		//this->SetRotQ(rotation);
		//m_translation = translation;

		////弾丸パーツの速度を設定
		//Vector3 BulletVel = Vector3(0, 0, -0.5f);
		////パーツの向きによって合わせて速度ベクトルを回転
		//BulletVel = Vector3::TransformNormal(BulletVel, m_world);
		{
			Vector3 pos = m_translation;
			m_translation = pos + m_bulletVel;
		}
	}
	Obj3d::Update();
	m_collisionNodeBullet->Update();
	m_deleteCount++;
}

void Bullet::Render()
{
	m_collisionNodeBullet->Render();
	DebugFont font;
}

//----------------------------------------------------------------------
//! @brief クリエイト関数
//!
//! @param[in] なし
//!
//! @return 弾のポインタ
//----------------------------------------------------------------------
std::unique_ptr<Bullet> Bullet::Create(DirectX::SimpleMath::Vector3 trans, DirectX::SimpleMath::Vector3 rot , DirectX::SimpleMath::Matrix world, bool isPlayer)
{
	std::unique_ptr<Bullet> bullet(new Bullet);
	bullet->Initialize(trans,rot, world, isPlayer);
	return bullet;
}
