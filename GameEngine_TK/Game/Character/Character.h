//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Character.h
//!
//! @brief  キャラクターのヘッダーファイル
//!
//! @date   2017/05/11(06/04更新：Y.Yamada)
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "..\Obj3d\Obj3d.h"
#include "..\State\State.h"

class Character
{
public:
	//コンストラクタ
	Character() {};
	//デストラクタ
	virtual ~Character() { delete m_state; };
	//初期化
	virtual void Initialize() = 0;
	//更新
	virtual void Update() = 0;
	//描画
	virtual void Render() = 0;

	//テスト
	virtual void ParentFreed() {};

	//ステイトの変更
	virtual void ChangeState(State* state) {};
	//Setter
	//スケーリング
	virtual void SetScale(const DirectX::SimpleMath::Vector3& scale) {};
	//回転
	virtual void SetRot(const DirectX::SimpleMath::Vector3& rot) {};
	//平行移動
	virtual void SetTrans(const DirectX::SimpleMath::Vector3& trans) {};

	//Getter
	//スケーリング
	virtual const	DirectX::SimpleMath::Vector3&	GetScale()				const { return DirectX::SimpleMath::Vector3(0, 0, 0); };
	//回転
	virtual const	DirectX::SimpleMath::Vector3&	GetRot()				const { return DirectX::SimpleMath::Vector3(0, 0, 0); };
	//平行移動
	virtual const	DirectX::SimpleMath::Vector3&	GetTrans()				const { return DirectX::SimpleMath::Vector3(0, 0, 0); };
	//ワールド行列
	virtual const	DirectX::SimpleMath::Matrix&	GetWorld()				const { return DirectX::SimpleMath::Matrix::Identity; };
	//オブジェクト
	virtual			Obj3d*							GetObj3d(int obj_num) { return nullptr; };
protected:
	//オブジェクト
	std::vector<Obj3d> m_obj3d;
	//ステイト
	State* m_state;
};