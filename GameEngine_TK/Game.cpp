//
// Game.cpp
//

//インクルード
#include "pch.h"
#include "Game.h"
#include <time.h>
extern void ExitGame();

//名前空間
using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;


Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */
	//独自の初期化はここに書く
	//unique_ptr...スコープを抜けたら自動的にdeleteしてくれるので、メモリリークの必要がない
	//smart_pointer.Get()で生のポインターを取得
	//四角形用
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());
	//三角形用
	//m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(m_d3dContext.Get());

	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());
	//Projection...射影行列
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		m_outputWidth, m_outputHeight, 0, 0, 1));
	//関数の呼び出しは、そのままで出来る
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());
	//汎用ステート設定を生成
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	//デバックカメラを生成
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);
	
	//エフェクトファクトリー生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//テクスチャのパスを指定(vcxprojから見てファイルの場所を指定)
	m_factory->SetDirectory(L"Resources");
	//モデルの生成(空)			デバイス			cmoファイルの場所を指定  エフェクトファクトリー
	m_modelSkydome = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/skydome.cmo", *m_factory);
	//モデルの生成(地面)			デバイス			cmoファイルの場所を指定  エフェクトファクトリー
	m_modelGround = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ground200m.cmo", *m_factory);
	//モデルの生成(球)				デバイス			cmoファイルの場所を指定  エフェクトファクトリー
	m_modelBall = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ball.cmo", *m_factory);
	//モデルの生成(ティーポット)
	m_modelTeapot = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/teapot.cmo", *m_factory);
	//カウント
	m_count = 0;
	//大きさ
	m_val = 0;
	//時間
	m_time = 0.0f;
	//乱数の初期化
	srand(static_cast<unsigned int>(time(nullptr)));
	for (int i = 0; i < 20; i++)
	{
		m_randAngle[i] = std::rand() % 360;		//角度
		m_randDistance[i] = std::rand() % 100;	//距離
	}
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	int count = 0.0f;
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;
	//毎フレーム処理を追加する
	m_debugCamera->Update();
	m_count++;

	m_time += 1.0f / 600.0f;
	if (m_time > 1.0f)
	{
		m_time = 1.0f;
	}
	//天球
	////ワールド行列を計算
	//for (int i = 0; i < 20; i++)
	//{
	//	//スケーリング
	//	Matrix scalemat = Matrix::CreateScale(1.0f);
	//	//ロール
	//	Matrix rotmatz;
	//	//内側の球
	//	if (i < 10)
	//		rotmatz = Matrix::CreateRotationZ(XMConvertToRadians(36.0f * i + m_count));
	//	//外側の球
	//	else 
	//		rotmatz = Matrix::CreateRotationZ(XMConvertToRadians(36.0f * i - m_count));
	//	//ピッチ(仰角)
	//	Matrix rotmatx = Matrix::CreateRotationX(0);
	//	//ヨー(方位角)
	//	Matrix rotmaty = Matrix::CreateRotationY(0);
	//	//回転行列の合成(順番を変えると描画が変わる)
	//	Matrix rotmat = rotmatz * rotmatx * rotmaty;
	//	//平行移動
	//	Matrix transmat = Matrix::CreateTranslation(20 + (i / 10 * 20), 0, 0);
	//	//ワールド行列の合成(順番大事)、よくSRTの順番で掛け算する
	//	m_worldBall[i] = scalemat * transmat * rotmat;
	//}

	//地面
	////ワールド行列を計算
	//for (int i = 0; i < 40000; i++)
	//{
	//	//ピッチ(仰角)
	//	Matrix rotmatx = Matrix::CreateRotationX(XMConvertToRadians(90));
	//	//平行移動
	//	Matrix transmat = Matrix::CreateTranslation( i / 200 - 100 , 0, i % 200 -100);
	//	//ワールド行列の合成(順番大事)、よくSRTの順番で掛け算する
	//	m_worldGround[i] = transmat * rotmatx;
	//}

	//ティーポットのワールド行列を計算
	for (int i = 0; i < 20; i++)
	{
		//スケーリング
		Matrix scalemat = Matrix::CreateScale((sinf(XMConvertToRadians(m_count))+1.0f)*2.0f + 1.0f);
		//ロール
		Matrix rotmatz = Matrix::CreateRotationZ(0);
		//ピッチ(仰角)
		Matrix rotmatx = Matrix::CreateRotationX(0);
		//ヨー(方位角)							回転させる
		Matrix rotmaty = Matrix::CreateRotationY(m_count/60.0f);
		//回転行列の合成
		Matrix rotmat = rotmatz * rotmatx * rotmaty;
		//平行移動
		m_trans = Lerp(Vector3(cosf(XMConvertToRadians(m_randAngle[i])) * m_randDistance[i], 0, sinf(XMConvertToRadians(m_randAngle[i])) * m_randDistance[i]),Vector3(0,0,0), m_time);
		Matrix transmat = Matrix::CreateTranslation(m_trans);
		//ワールド行列の合成			strだと先に距離を決めるので、全体的に回転してしまう
		m_worldTeapot[i] = scalemat * rotmat * transmat;
	}
}

// Draws the scene.
void Game::Render()
{
	//頂点インデックス
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3
	};
	//頂点座標
	VertexPositionNormal vertices[] =
	{//			座標						法線ベクトル
		{ Vector3(-1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(-1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,+1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },
		{ Vector3(+1.0f,-1.0f,0.0f),Vector3(0.0f,0.0f,+1.0f) },

	};
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.
	//ポインターなので、アロー演算子//
	//Opaque...不透明
	m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	//Depth...奥行き、描画するときの前後関係
	m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//Cull...背景カリングを有効にするかどうか
	m_d3dContext->RSSetState(m_states->CullNone());
	//ビュー行列を生成
	//m_view = Matrix::CreateLookAt(
	//	Vector3(2.f, 2.f, 2.f),//カメラ視点、z座標とカメラ参照点が同じだとエラーになる
	//	Vector3::Zero,	//カメラ参照点
	//	Vector3::UnitY);//上方向ベクトル、カメラがローリングする演出によく使う、変えることは少ない
	//デバッグカメラからビュー行列を取得
	m_view = m_debugCamera->GetCameraMatrix();
	//プロジェクション行列を生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(
		XM_PI / 4.f,	//視野角(上下方向)
		float(m_outputWidth) / float(m_outputHeight),	//アスペクト比(画面の横幅と縦幅の比率)
		0.1f,	//ニアクリップ(指定した数値よりも近いと描画しない)
		500.0f);	//ファークリップ（指定した数値より遠いと描画しない）
	//渡す
	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());
	//モデルの描画   デバイス     コモンステイト ワールド行列　ビュー行列　射影行列
	m_modelSkydome->Draw(m_d3dContext.Get(), *m_states, m_world, m_view, m_proj);
	//モデルの描画   デバイス     コモンステイト ワールド行列　ビュー行列　射影行列
	m_modelGround->Draw(m_d3dContext.Get(), *m_states, Matrix::Identity, m_view, m_proj);
	//ティーポットモデルの描画
	for (int i = 0; i < 20; i++)
	{
		m_modelTeapot->Draw(m_d3dContext.Get(), *m_states, m_worldTeapot[i], m_view, m_proj);
	}
	//モデルの描画   デバイス     コモンステイト ワールド行列　ビュー行列　射影行列
	//for (int i = 0; i < 20; i++)
	//{
	//	m_modelBall->Draw(m_d3dContext.Get(), *m_states, m_worldBall[i], m_view, m_proj);
	//}
	//prmitiveBatchの描画開始時に必須
	m_batch->Begin();
	//描画処理
	//四角形の描画
	m_batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indices, 6, vertices, 4);
	//////線を描く
	////m_batch->DrawLine(
	////	//										座標							色
	////	VertexPositionColor(SimpleMath::Vector3(  0,   0, 0), SimpleMath::Color(1, 1, 1)),
	////	VertexPositionColor(SimpleMath::Vector3(800, 600, 0), SimpleMath::Color(1, 0, 0))
	////);

	////三角形の描画
	//VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	////VertexPositionColor v1(Vector3(400.f, 150.f, 0.f), Colors::Red);
	////VertexPositionColor v2(Vector3(600.f, 450.f, 0.f), Colors::Blue);
	////VertexPositionColor v3(Vector3(200.f, 450.f, 0.f), Colors::Green);
	//m_batch->DrawTriangle(v1, v2, v3);

	////primitiveBatchの描画終了時に必須
	m_batch->End();

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}
//----------------------------------------------------------------------
//! @brief 線形補間
//!
//! @param[in] 最初の場所、目標の場所、時間(０～１)
//!
//! @return 今の場所
//----------------------------------------------------------------------
Vector3 Game::Lerp(Vector3 startPos, Vector3 targetPos, float t)
{
	Vector3 lerpPos = Vector3(0,0,0);
	lerpPos = (1 - t)* startPos + t * targetPos;
	return lerpPos;
}
