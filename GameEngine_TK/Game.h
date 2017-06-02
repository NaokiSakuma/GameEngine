//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include "DebugCamera.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include <Keyboard.h>
#include "Follow.h"
#include "Obj3d.h"
#include <vector>
#include "DebugText.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:
	enum  PLAYER_PARTS
	{
		HEAD1,		//頭
		HEAD2,		//頭
		HEAD3,		//頭
		HEAD4,		//頭

		PLAYER_PARTS_NUM = 10,	//パーツ数
	};
    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

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

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	//プリミティブバッチ
	//std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;	//三角形用
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;	//四角形用
	//エフェクト
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//CommonState...描画するときの汎用的な設定//
	//汎用的ステート設定
	std::unique_ptr<DirectX::CommonStates> m_states;

	//Matrixはデフォルトコンストラクタで初期化されている
	//ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	//プロジェクション行列（射影行列）
	DirectX::SimpleMath::Matrix m_proj;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugCamera;

	//エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//天球モデル 奥にあるものを先に描画した方がよい。
	Obj3d m_objSkydome;
	//地面モデル
	std::unique_ptr<DirectX::Model> m_modelGround;
	//球モデル
	std::unique_ptr<DirectX::Model> m_modelBall;
	//ティーポットモデル
	std::unique_ptr<DirectX::Model> m_modelTeapot;
	////頭のモデル
	//std::unique_ptr<DirectX::Model> m_modelHead;
	////頭のワールド行列
	//DirectX::SimpleMath::Matrix m_worldHead;
	////頭のワールド行列
	//DirectX::SimpleMath::Matrix m_worldHead2;
	//球のワールド行列
	DirectX::SimpleMath::Matrix m_worldBall[20];
	//ティーポットのワールド行列
	DirectX::SimpleMath::Matrix m_worldTeapot[20];
	//地面のワールド行列
	DirectX::SimpleMath::Matrix m_worldGround[40000];
	
	//回転用の変数
	int m_count;
	//角度を取るrand
	float m_randAngle[20];
	//距離を取るrand;
	float m_randDistance[20];
	//大きさ
	float m_val;
	//時間
	float m_time;
	//テスト
	DirectX::SimpleMath::Vector3 m_trans;
	//線形補間
	static DirectX::SimpleMath::Vector3 Lerp(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 targetPos, float t);

	//キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	//頭の座標
	DirectX::SimpleMath::Vector3 m_head_pos;
	//回転用
	int m_rot;
	
	//カメラ
	std::unique_ptr<FollowCamera> m_camera;
	//頭
	std::vector<Obj3d> m_ObjPlayer;
	//サイン用の角度
	float m_cycle;
	//スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//文字列
	std::unique_ptr<DebugText> m_debugText;

	//関数
	void SetW();	//Wが押されたとき
	void SetS();	//Sが押されたとき
};