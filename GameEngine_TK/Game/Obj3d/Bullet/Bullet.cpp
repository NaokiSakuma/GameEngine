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
void Bullet::Initialize()
{
	this->LoadModel(L"Resources/head.cmo");
}

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Bullet::Bullet()
{
	m_ObjParent = nullptr;
	m_scale = Vector3(2, 2, 2);
	m_translation = Vector3(0, 0, 0);
	m_UseQuaternion = false;

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
	//抽出した情報をしまっておく変数
	Vector3 scale;		//ワールドスケーリング
	Quaternion rotation;//ワールド回転
	Vector3 translation;//ワールド座標

	//ワールド座標から各要素を抽出
	m_world.Decompose(scale, rotation, translation);

	//親がいない場合
	if (!m_ObjParent)
	{
		//親パーツから分離、独立させる
		m_scale = scale;
		this->SetRotQ(rotation);
		m_translation = translation;

		//弾丸パーツの速度を設定
		Vector3 BulletVel = Vector3(0, 0, -0.5f);
		//パーツの向きによって合わせて速度ベクトルを回転
		BulletVel = Vector3::Transform(BulletVel, rotation);
		{
			Vector3 pos = m_translation;
			m_translation = pos + BulletVel;
		}
	}

	Obj3d::Update();

}

//----------------------------------------------------------------------
//! @brief クリエイト関数
//!
//! @param[in] なし
//!
//! @return 弾のポインタ
//----------------------------------------------------------------------
Bullet * Bullet::Create()
{
	Bullet* bullet = new Bullet;
	bullet->Initialize();
	return bullet;
}
