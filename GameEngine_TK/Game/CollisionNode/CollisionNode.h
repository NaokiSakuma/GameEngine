//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   CollisionNode.h
//!
//! @brief  当たり判定ノード判定のヘッダーファイル
//!
//! @date   2017/06/19
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include "..\Obj3d\Obj3d.h"
#include "..\Collision\Collision.h"

namespace CollisionNode
{
	//当たり判定ノード
	class CollisionNode
	{
	public:
		//デバッグ表示のsetter
		static bool SetDebugVisible(bool visible) { m_debugVisible = visible; };
		//デバッグ表示のgetter
		static bool GetDebugVisible()			  { return m_debugVisible; };
	protected:
		//デバッグ表示
		static bool m_debugVisible;
	public:
		//初期化
		virtual void Initialize() = 0;
		//更新
		virtual void Update()	  = 0;
		//描画
		virtual void Render()     = 0;

		//setter
		//親子関係
		void SetParent(Obj3d* parent);
		//オフセット
		void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	protected:
		//デバッグ表示用オブジェクト
		Obj3d m_Obj;
		//親からのオフセット
		DirectX::SimpleMath::Vector3 m_trans;
	};

	//球の当たり判定ノード
	class SphereNode final: public CollisionNode ,public Collision::Sphere
	{
	public:
		//コンストラクタ
		SphereNode();
		//初期化
		void Initialize() override;
		//更新
		void Update()	  override;
		//描画
		void Render()	  override;
		//ローカルの半径
		void SetLocalRadius(float radius);
	private:
		//ローカルの半径
		float m_LocalRadius;
	};
}