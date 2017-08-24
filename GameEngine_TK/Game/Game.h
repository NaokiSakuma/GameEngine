//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Game.h
//!
//! @brief  ゲームのヘッダファイル
//!
//! @date   2017/06/04
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/


#pragma once


#include <windows.h>

#include <wrl/client.h>

#include <d3d11_1.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>

#include "StepTimer.h"
#include "Camera\DebugCamera\DebugCamera.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include "Device\Device.h"
#include "Camera\FollowCamera\Follow.h"
#include "Obj3d\Obj3d.h"
#include <vector>
#include "Text\DebugText.h"
#include "Character\Player\Player.h"
#include "Character\Enemy\Enemy.h"
#include "Character\Boss\Boss.h"
#include "ModelEffect\ModelEffect.h"
#include "LandShape\LandShape.h"
#include "Lockon\Lockon.h"


#pragma comment(lib,"d3dCompiler.lib")

//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define WINDOW_WIDTH 640 //ウィンドウ幅
#define WINDOW_HEIGHT 480 //ウィンドウ高さ

namespace DX
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch DirectX API errors
			throw std::exception();
		}
	}
}

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:
	Game();
	~Game();
	// Initialization and management
	HRESULT Initialize(HWND window, int width, int height);

	// Basic game loop
	void Tick();

	// Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	// Properties
	void GetDefaultSize(int& width, int& height) const;

private:

	void Update(DX::StepTimer const& timer);
	void Render();

	void Clear();
	void Present();

	void CreateDevice();
	void CreateResources();

	void OnDeviceLost();

	// Device resources.
	HWND                                            m_window;
	int                                             m_outputWidth;
	int                                             m_outputHeight;

	D3D_FEATURE_LEVEL                               m_featureLevel;
	Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

	Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
	Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

	ID3D11Texture2D* m_backBuffer_DSTex;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11Buffer* m_constantBuffer;
	ID3D11Buffer* m_vertexBuffer;
	// Rendering loop timer.
	DX::StepTimer                                   m_timer;

	//プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;	//四角形用
	//エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//CommonState...描画するときの汎用的な設定//
	//汎用的ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;

	//Matrixはデフォルトコンストラクタで初期化されている
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//プロジェクション行列（射影行列）
	DirectX::SimpleMath::Matrix m_proj;
	//デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugCamera;
	//天球モデル 奥にあるものを先に描画した方がよい。
	//Obj3d m_objSkydome;
	LandShape m_landShapeobjSkydome;
	////地面モデル
	//Obj3d m_modelGround;
	LandShape m_landShapeGround;
	//線形補間
	static DirectX::SimpleMath::Vector3 Lerp(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 targetPos, float t);

	//線形補間
	int Lerp(int startPos, int targetPos, float t);

	//プレイヤー
	Character* m_player;
	//敵
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	//ボス
	std::unique_ptr<Boss> m_boss;
	//カメラ
	std::unique_ptr<FollowCamera> m_camera;
	//ロックオンの対象
	std::vector<Obj3d> m_lockonTarget;
	//スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//文字列
	std::unique_ptr<DebugText> m_debugText;
	//ロックオン
	std::unique_ptr<Lockon> m_lockon;
	//テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_HpGreentexture[2];
	//テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_HpRedtexture[2];
	//テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_HpWhitetexture[2];
	//ゲームクリアテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_clearTexture;
	//ゲームオーバーテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_overTexture;
	//スプライトを表示するスクリーン座標
	DirectX::SimpleMath::Vector2 m_screenPos;
	//スプライトの原点
	DirectX::SimpleMath::Vector2 m_origin;

	int m_count;
	int m_m_count;
	
	int m_bosshp;
	int m_bosshpOld;

	int m_timer1;
	int m_timer2;
	bool m_bossFlag;
	bool m_clearFlag;
	bool m_playerExplosion;
	////Textureハンドル
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	////四角形の頂点データ
	//DirectX::VertexPositionTexture m_vertx[4];
	////インデックスデータ
	//uint16_t m_index[6];
	////プリミティブバッチ
	//std::unique_ptr < DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;
	////ベーシックエフェクト
	//std::unique_ptr<DirectX::BasicEffect> m_bassicEffect;
public:
	//デバッグ表示
	bool isDebug;
	//ゲームのインスタンス
	static Game* GetInstance();
	//ロックオンのgetter
	Lockon* GetLockon() { return m_lockon.get(); }
private: 
	//ゲームのインスタンス
	static Game* m_instance;

	//必携
public:
	HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
	HRESULT InitModel();
	HRESULT InitPolygon();
	HRESULT InitShader();
	//頂点の構造体
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos; //位置
	};
	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;
		D3DXVECTOR4 vColor;
	};
};