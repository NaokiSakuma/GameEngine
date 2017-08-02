//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Boss.h
//!
//! @brief  ボスのヘッダーファイル
//!
//! @date   2017/06/08
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once
#include "..\Character.h"
#include "..\..\Obj3d\Bullet\Bullet.h"

class Boss final : public Character
{
public:
	enum ENEMY_PARTS
	{
		BOSS,
		BOSS_PARTS_NUM,
	};

public:
	//デストラクタ
	~Boss();
	//初期化
	void Initialize()	override;
	//更新
	void Update()		override;
	//描画
	void Render()		override;

	//ステイトを変更
	void ChangeState(State* state) override;

	//Setter
	//スケーリング
	void SetScale(const DirectX::SimpleMath::Vector3& scale)	    override;
	//回転
	void SetRot(const DirectX::SimpleMath::Vector3& rot)		    override;
	//平行移動
	void SetTrans(const DirectX::SimpleMath::Vector3& trans)	    override;
	void SetWallCount(const int wallCount) { m_wallCount = wallCount; };
	//Getter
	//スケーリング
	const DirectX::SimpleMath::Vector3& GetScale()			const	override;
	//回転
	const DirectX::SimpleMath::Vector3& GetRot()			const	override;
	//平行移動
	const DirectX::SimpleMath::Vector3& GetTrans()			const	override;
	//ワールド行列
	const DirectX::SimpleMath::Matrix&	GetWorld()			const	override;
	//オブジェクト
	Obj3d*								GetObj3d(int num)			override;
	const int GetWallCount() const { return m_wallCount; };
	const int GetHp() const { return m_hp; };
	//クリエイト関数
	static std::unique_ptr<Boss> Create();

	//弾の個数
	const int                           GetBulletNum()        const;
	//弾
	const std::unique_ptr<Bullet>&      GetBullet(int i)     const;
	//弾を発射する
	void ParentFreed();
	//弾を消す
	void DeleteBullet(int i);
	//hpを減らす
	void DecreaseHP();
	//弾の数
	static const int BULLETNUM = 50;
	//弾の発射速度
	static const int BULLETCOUNT;

private:
	//コンストラクタ
	Boss();
	//弾
	std::vector<std::unique_ptr<Bullet>> m_bullet;
	//弾丸の速度ベクトル
	DirectX::SimpleMath::Vector3 m_BulletVel;
	//タイマー
	int m_timer;
	//目標角度
	float m_DistAngle;
	//壁との当たり判定のカウント
	int m_wallCount;
	//カウント
	int m_bulletCount;
	//HP
	int m_hp;
};