//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Obj3d.h
//!
//! @brief  3dオブジェクトを生成するヘッダーファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <map>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "..\Camera\Camera.h"
class Obj3d
{
	//静的
public:
	//3dオブジェクトのInitialize
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
private:
	//デバイス
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	//デバイスコンテキスト
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	//カメラ
	static const Camera* m_camera;
	//汎用的ステート設定
	static std::unique_ptr<DirectX::CommonStates> m_states;
	//エフェクトファクトリー
	static std::unique_ptr<DirectX::EffectFactory> m_factory;
	//モデルコンテナ
	static std::map<std::wstring, std::unique_ptr<DirectX::Model>> m_modelarray;

public:
	//コンストラクタ
	Obj3d();
	//CMOモデルのロード
	void LoadModel(const wchar_t* fileName);
	//更新
	virtual void Update();
	//描画
	virtual void Render() const;

	//Setter
	//スケーリング
	void SetScale(const DirectX::SimpleMath::Vector3& scale);
	//回転(xyz)
	void SetRot(const DirectX::SimpleMath::Vector3& rot);
	//回転(クォータニオン)
	void SetRotQ(const DirectX::SimpleMath::Quaternion& quaternion);
	//平行移動
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	//親の3dオブジェクト
	void SetObjParent(Obj3d* objParent);
	//Getter
	//スケーリング
	const DirectX::SimpleMath::Vector3& GetScale() const;
	//回転
	const DirectX::SimpleMath::Vector3& GetRot() const;
	//平行移動
	const DirectX::SimpleMath::Vector3& GetTrans() const;
	//ワールド行列
	const DirectX::SimpleMath::Matrix& GetWorld() const;
	//親の3dオブジェクト
	const bool GetObjParent() const;

protected:
	//モデル
	const DirectX::Model* m_model;
	//スケーリング
	DirectX::SimpleMath::Vector3 m_scale;
	//回転(x,y,z / オイラー角)
	DirectX::SimpleMath::Vector3 m_rotation;
	//回転(クォータニオン)
	DirectX::SimpleMath::Quaternion m_rotationQ;
	//平行移動
	DirectX::SimpleMath::Vector3 m_translation;
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//親のObj3dへのポインタ
	Obj3d* m_ObjParent;

	//回転をクォータニオンで扱うフラグ
	bool m_UseQuaternion;

};