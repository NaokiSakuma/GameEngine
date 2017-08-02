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
//#include <d3d9types.h>
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
//モデルコンテナ
std::map<std::wstring, std::unique_ptr<DirectX::Model>> Obj3d::m_modelarray;
//ブレンドステイト
ID3D11BlendState* Obj3d::m_BlendStateSubtract;
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

	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable                 = false;
	desc.IndependentBlendEnable                = false;
	desc.RenderTarget[0].BlendEnable           = true;
	desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_REV_SUBTRACT;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT ret                                = m_d3dDevice->CreateBlendState(&desc, &m_BlendStateSubtract);

	//D3DPRESENT_PARAMETERS
}

void Obj3d::SetSubtractive()
{
	m_d3dContext->OMSetBlendState(m_BlendStateSubtract, nullptr, 0xffffff);
}

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Obj3d::Obj3d()
	: m_ObjParent(nullptr)
	, m_scale(1, 1, 1)
	, m_UseQuaternion(false)
	, m_model(nullptr)
{
}

//----------------------------------------------------------------------
//! @brief コピーコンストラクタ
//!
//! @param[in] オブジェクト
//----------------------------------------------------------------------
//Obj3d::Obj3d(const Obj3d & obj)
//{
//	this->m_model = std::move(obj.m_model);
//
//
//}

//----------------------------------------------------------------------
//! @brief CMOモデルのロード
//!
//! @param[in] ファイルの名前
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::LoadModel(const wchar_t * fileName)
{
	//エフェクトファクトリーが入っていない場合落とす
	assert(m_factory);
	//同じ名前のモデルを読み込み済みでないならhit
	if (m_modelarray.count(fileName) == 0)
		m_modelarray[fileName] = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
	m_model = m_modelarray[fileName].get();
	//m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
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
	Matrix rotmat;
	if (m_UseQuaternion)
	{	//クォータニオンで回転を計算
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else {	//オイラー角で回転を計算
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);	//z軸の回転
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);	//x軸の回転
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);	//y軸の回転
		rotmat = rotmatZ * rotmatX * rotmatY;		    //回転
	}
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
void Obj3d::Render()
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

void Obj3d::DrawSubtractive()
{
	if (m_model)
	{
		assert(m_camera);
		const Matrix& view = m_camera->GetViewMatrix();
		const Matrix& projection = m_camera->GetProjectionMatrix();
		assert(m_d3dContext);
		assert(m_states);
		m_model->Draw(m_d3dContext.Get(), *m_states, m_world, view, projection, false, Obj3d::SetSubtractive);
	}
}

void Obj3d::DisableLighting()
{
	if (m_model)
	{
		//モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);
				//メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff)
				{
					//自己発光を最大値に
					eff->SetEmissiveColor(Vector3(1, 1, 1));
					//エフェクトに含むすべての平行光源分について処理する
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						//ライトを無効にする
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
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
//! @brief 回転のSetter(xyz)
//!
//! @param[in] 回転
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_rotation = rot;
	m_UseQuaternion = false;
}

//----------------------------------------------------------------------
//! @brief 回転のSetter(クォータニオン)
//!
//! @param[in] 回転
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::SetRotQ(const DirectX::SimpleMath::Quaternion & quaternion)
{
	m_rotationQ = quaternion;
	m_UseQuaternion = true;
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
//! @brief ワールド行列のSetter
//!
//! @param[in] ワールド行列
//!
//! @return なし
//----------------------------------------------------------------------
void Obj3d::SetWorld(const DirectX::SimpleMath::Matrix & world)
{
	m_world = world;
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

const bool Obj3d::GetObjParent() const
{
	bool areThereParent;
	if (m_ObjParent)
		areThereParent = true;
	else if(!m_ObjParent)
		areThereParent = false;
	return areThereParent;
}

void Obj3d::DrawBillboard()
{
	if (m_model)
	{
		assert(m_camera);
		const Matrix& view = m_camera->GetViewMatrix();
		const Matrix& projection = m_camera->GetProjectionMatrix();
		//ビルボード行列をワールド行列に合成
		Matrix world = m_camera->GetBillboard()* m_world;
		assert(m_d3dContext);
		assert(m_states);
		//減算描画用の設定関数を渡して描画
		m_model->Draw(m_d3dContext.Get(), *m_states, world, view, projection);
	}
}

void Obj3d::DrawBillboardaxisY()
{
	if (m_model)
	{
		assert(m_camera);
		const Matrix& view = m_camera->GetViewMatrix();
		const Matrix& projection = m_camera->GetProjectionMatrix();
		//ビルボード行列をワールド行列に合成
		Matrix world = m_camera->GetBillboardAxisY()*m_world;
		assert(m_d3dContext);
		assert(m_states);
		//減算用の設定関数を渡して描画
		m_model->Draw(m_d3dContext.Get(), *m_states, world, view, projection);
	}
}

void Obj3d::EnbleAlpha()
{
	if (m_model)
	{
		//モデル内の全メッシュ分回す
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);
			//メッシュ内の全メッシュパーツ分回す
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);
				//メッシュパーツにセットされたエフェクトをBasicEffectとして取得
				std::shared_ptr<IEffect>& ieff = meshpart->effect;
				meshpart->ModifyEffect(m_d3dDevice.Get(), ieff, true);
			}
		}
	}
}

