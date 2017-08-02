//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Player.h
//!
//! @brief  プレイヤーのヘッダーファイル
//!
//! @date   2017/05/11 
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
		PLAYER_PARTS_NUM,	//パーツ数
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
	void SetInvincible(int invicible) { m_invincibleTime = invicible; };
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
	//弾の個数
	const int                           GetBulletNum()        const;
	//弾
	const std::unique_ptr<Bullet>&      GetBullet (int i)     const;
	//速度を取得する
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }
	const int GetInvincible() { return m_invincibleTime; }
	//クリエイト関数
	static Player* Create();

	//弾を発射する
	void ParentFreed();
	//弾を消す
	void DeleteBullet(int i);

	//ロックオン時の操作
	void ControlLockon();

	//弾の数
	static const int BULLETNUM = 50;
	//弾の発射速度
	static const int BULLETCOUNT;
private:
	//コンストラクタ
	Player();
	std::vector<std::unique_ptr<Bullet>> m_bullet;
	//弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;

	//再発射するための時間
	int m_nextFireTimer;
	//カウント
	int m_bulletCount;
	//無敵時間
	int m_invincibleTime;
	
};

