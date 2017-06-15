//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Enemy.h
//!
//! @brief  敵のヘッダーファイル
//!
//! @date   2017/06/08
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Character.h"

class Enemy final : public Character
{
public:
	enum ENEMY_PARTS
	{
		HEAD1,
		HEAD2,
		ENEMY_PARTS_NUM,
	};

public:
	//デストラクタ
	~Enemy();
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
	void SetScale(const DirectX::SimpleMath::Vector3& scale)	override;
	//回転
	void SetRot(const DirectX::SimpleMath::Vector3& rot)		override;
	//平行移動
	void SetTrans(const DirectX::SimpleMath::Vector3& trans)	override;

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

	//クリエイト関数
	static Enemy* Create();

private:
	//コンストラクタ
	Enemy();
	//タイマー
	int m_timer;
	//目標角度
	float m_DistAngle;
};