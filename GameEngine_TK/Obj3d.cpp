//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Obj3d.cpp
//!
//! @brief  3dオブジェクトを生成するソースファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Obj3d.h"

//名前空間
using namespace DirectX;
using namespace SimpleMath;

//静的メンバ変数の定義
//デバイス
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
//デバイスコンテキスト
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
//カメラ
const Camera* Obj3d::m_camera;
//汎用的ステート設定
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
//エフェクトファクトリー
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

//----------------------------------------------------------------------
//! @brief 3dオブジェクトのInitialize
//!
//! @param[in] デバイス、コンテキスト、カメラ
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	//エフェクトファクトリー生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//テクスチャのパスを指定(vcxprojから見てファイルの場所を指定)
	m_factory->SetDirectory(L"Resources");

}

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Obj3d::Obj3d()
	:m_ObjParent(nullptr)
	, m_scale(1, 1, 1)
{
}

//----------------------------------------------------------------------
//! @brief CMOモデルのロード
//!
//! @param[in] ファイルの名前
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::LoadModel(const wchar_t * fileName)
{
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
}

//----------------------------------------------------------------------
//! @brief 更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::Update()
{
	//オブジェクトのワールド行列を計算
	Matrix scalemat = Matrix::CreateScale(m_scale);				//スケーリング
	Matrix rotmatZ  = Matrix::CreateRotationZ(m_rotation.z);	//z軸の回転
	Matrix rotmatX  = Matrix::CreateRotationX(m_rotation.x);	//x軸の回転
	Matrix rotmatY  = Matrix::CreateRotationY(m_rotation.y);	//y軸の回転
	Matrix rotmat   = rotmatZ * rotmatX * rotmatY;				//回転
	Matrix transmat = Matrix::CreateTranslation(m_translation);	//平行移動
	m_world = scalemat * rotmat * transmat;						//ワールド行列
	//親オブジェクトが存在する場合
	if(m_ObjParent)
		m_world *= m_ObjParent->GetWorld();	//親の行列を掛ける
}

//----------------------------------------------------------------------
//! @brief 描画
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::Render() const
{
	//オブジェクトが存在していたら
	if (m_model)
	{
		m_model->Draw(						//描画する	
			m_d3dContext.Get(),				//コンテキスト
			*m_states,						//ステイト
			m_world,						//ワールド行列
			m_camera->GetViewMatrix(),		//view行列
			m_camera->GetProjectionMatrix() //プロジェクション行列
		);
	}
}

//----------------------------------------------------------------------
//! @brief スケーリングのSetter
//!
//! @param[in] スケーリング
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_scale = scale;
}

//----------------------------------------------------------------------
//! @brief 回転のSetter
//!
//! @param[in] 回転
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_rotation = rot;
}

//----------------------------------------------------------------------
//! @brief 平行移動のSetter
//!
//! @param[in] 平行移動
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	m_translation = trans;
}

//----------------------------------------------------------------------
//! @brief 親の3dオブジェクトのSetter
//!
//! @param[in] 親の3dオブジェクト
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::SetObjParent(Obj3d * objParent)
{
	m_ObjParent = objParent;
}

//----------------------------------------------------------------------
//! @brief スケーリングのGetter
//!
//! @param[in] スケーリング
//!
//! @return なし
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Obj3d::GetScale() const
{
	return m_scale;
}

//----------------------------------------------------------------------
//! @brief 回転のGetter
//!
//! @param[in] 回転
//!
//! @return なし
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Obj3d::GetRot() const
{
	return m_rotation;
}

//----------------------------------------------------------------------
//! @brief 平行移動のGetter
//!
//! @param[in] 平行移動
//!
//! @return なし
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Obj3d::GetTrans() const
{
	return m_translation;
}

//----------------------------------------------------------------------
//! @brief ワールド行列のGetter
//!
//! @param[in] ワールド行列
//!
//! @return なし
//----------------------------------------------------------------------
const DirectX::SimpleMath::Matrix& Obj3d::GetWorld() const
{
	return m_world;
}
