//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Character.h
//!
//! @brief  キャラクターのヘッダーファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include <vector>
#include "..\Obj3d\Obj3d.h"
#include "..\State\State.h"
#include "..\CollisionNode\CollisionNode.h"

class Character
{
public:
	//重力加速度
	const float GRAVITY_ACC = 0.03f;
	//ジャンプしたときの初速度
	const float JUMP_SPEED_FIRST = 0.5f;
	//ジャンプ速度制限
	const float JUMP_SPEED_MAX = 0.3f;

	//コンストラクタ
	Character() :m_isRenderColl(false),m_isJump(false) {};
	//デストラクタ
	virtual ~Character() {
		/*if (m_state) { delete m_state; m_state = nullptr; }*/
	}
	//初期化
	virtual void Initialize() = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Render() = 0;

	//ステイトの変更
	virtual void ChangeState(State* state) {};
	//当たり判定を描画するかどうかの変更
	void ChangeCollision() { m_isRenderColl = !m_isRenderColl; };
	//ジャンプ
	void StartJump() {
		if (!m_isJump)
		{
			//上方向の初速度を設定
			m_velocity.y = JUMP_SPEED_FIRST;
			m_isJump = true;
		}
	}
	//落下を開始する
	void StartFall() {
		//ジャンプ中ではないか
		if (!m_isJump)
		{
			//上方向の初速度を０に
			m_velocity.y = 0.f;
			//ジャンプフラグを立てる
			m_isJump = true;
		}
	}
	//落下を終える
	void StopFall() {
		m_isJump = false;
		m_velocity = DirectX::SimpleMath::Vector3::Zero;
	}
	//Setter
	//スケーリング
	virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) {};
	//回転
	virtual void SetRot(const DirectX::SimpleMath::Vector3& rot)     {};
	//平行移動
	virtual void SetTrans(const DirectX::SimpleMath::Vector3& trans) {};

	//Getter
	//スケーリング
	virtual const	DirectX::SimpleMath::Vector3&	GetScale()				const { return DirectX::SimpleMath::Vector3::Zero; };
	//回転
	virtual const	DirectX::SimpleMath::Vector3&	GetRot()				const { return DirectX::SimpleMath::Vector3::Zero; };
	//平行移動
	virtual const	DirectX::SimpleMath::Vector3&	GetTrans()				const { return DirectX::SimpleMath::Vector3::Zero;};
	//ワールド行列
	virtual const	DirectX::SimpleMath::Matrix&	GetWorld()				const { return DirectX::SimpleMath::Matrix::Identity; };
	//オブジェクト
	virtual			Obj3d*							GetObj3d(int obj_num)         { return nullptr; };
	//当たり判定
	virtual		    CollisionNode::CollisionNode*   GetCollisionNode()            { return m_collisionNode; };
	//速度
	virtual const   DirectX::SimpleMath::Vector3&   GetVelocity()           const { return DirectX::SimpleMath::Vector3::Zero; };
	//落下中のフラグ
		    const   bool                            GetIsJump()             const { return m_isJump; }
protected:
	//オブジェクト
	std::vector<Obj3d> m_obj3d;
	//ステイト
	State* m_state;
	//当たり判定
	CollisionNode::CollisionNode* m_collisionNode;
	//速度
	DirectX::SimpleMath::Vector3 m_velocity;
	//当たり判定を描画するフラグ
	bool m_isRenderColl;
	//落下中のフラグ
	bool m_isJump;

};