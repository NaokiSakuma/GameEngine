//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.h
//!
//! @brief  プレイヤーのヘッダーファイル
//!
//! @date   2017/05/11 (06/04更新：Y.Yamada)
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
#pragma once

#include "..\Character.h"
#include "..\..\Obj3d\Bullet\Bullet.h"

class Player final :public Character
{
public:
	enum  PLAYER_PARTS
	{
		HEAD1,		//頭
		HEAD2,		//頭
		HEAD3,		//頭
		HEAD4,		//頭
		HEAD9 = 9,
		PLAYER_PARTS_NUM = 10,	//パーツ数
	};

public:

	//デストラクタ
	~Player();
	//初期化
	void Initialize()	override;
	//更新
	void Update()		override;
	//描画
	void Render()		override;
	//弾丸を装填
	void ResetBullet();
	//ステイトを変更
	void ChangeState(State* state) override;

	//Setter
	//スケーリング
	void SetScale(const DirectX::SimpleMath::Vector3& scale)	    override;
	//回転
	void SetRot(const DirectX::SimpleMath::Vector3& rot)		    override;
	//平行移動
	void SetTrans(const DirectX::SimpleMath::Vector3& trans)	    override;
	//Getter
	//スケーリング
	const DirectX::SimpleMath::Vector3& GetScale()			  const	override;
	//回転
	const DirectX::SimpleMath::Vector3& GetRot()			  const	override;
	//平行移動
	const DirectX::SimpleMath::Vector3& GetTrans()			  const	override;
	//ワールド行列
	const DirectX::SimpleMath::Matrix&	GetWorld()			  const	override;
	//オブジェクト
	Obj3d*								GetObj3d(int num)			override;
	//弾丸用の当たり判定
	const CollisionNode::SphereNode& GetCollisionNodeBullet() const;
	//弾
	Bullet* m_bullet;
	//クリエイト関数
	static Player* Create();

	//弾を発射する
	void ParentFreed();

private:
	//コンストラクタ
	Player();
	//弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;

	//再発射するための時間
	int m_nextFireTimer;

	//弾丸用の当たり判定
	CollisionNode::SphereNode m_collisionNodeBullet;
};

