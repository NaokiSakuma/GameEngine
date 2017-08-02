//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Game.cpp
//!
//! @brief  ゲームのソースファイル
//!
//! @date   2017/06/04
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

//インクルード
#include "pch\pch.h"
#include "DebugFont\DebugFont.h"
#include <WICTextureLoader.h>
#include "Game.h"
#include <time.h>
#include "ADX2Le\ADX2Le.h"
#include "Basic\Basic.h"
#include "CueSheet\CueSheet_0.h"

extern void ExitGame();

//名前空間
using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

Game* Game::m_instance;

Game * Game::GetInstance()
{
	assert(m_instance);
	return m_instance;
}


Game::Game() :
	m_window(0),
	m_outputWidth(800),
	m_outputHeight(600),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
	assert(!m_instance);
	m_instance = this;
}

Game::~Game()
{
	m_instance = nullptr;
	ADX2Le::Finalize();
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
	//デバイスの生成
	Device::Initialize();
	//カメラの生成
	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	m_camera->SetKeyboard(Device::m_keyboard.get());
	//3dオブジェクトの静的メンバ変数を初期化
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	LandShapeCommonDef lscDef;
	lscDef.pDevice = m_d3dDevice.Get();
	lscDef.pDeviceContext = m_d3dContext.Get();
	lscDef.pCamera = m_camera.get();
	LandShape::InitializeCommon(lscDef);

	//unique_ptr...スコープを抜けたら自動的にdeleteしてくれるので、メモリリークの必要がない
	//smart_pointer.Get()で生のポインターを取得
	//四角形用
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());
	//Projection...射影行列
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight), 0, 0, 1.0f));
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
	//m_objSkydome.LoadModel(L"skydome.cmo");
	//m_objSkydome.DisableLighting();

	////モデルの生成(地面)			デバイス			cmoファイルの場所を指定  エフェクトファクトリー
	//m_modelGround.LoadModel(L"Resources/ground200m.cmo");
	m_landShapeobjSkydome.Initialize(L"skydome", L"skydome");
	m_landShapeobjSkydome.DisableLighting();
	//地形データの読み込み
	m_landShapeGround.Initialize(L"ground200m", L"ground200m");
	//m_landShapeGround.DisableLighting();
	//乱数の初期化
	srand(static_cast<unsigned int>(time(nullptr)));
	//プレイヤーの生成
	m_player = Player::Create();
	//敵の生成
	int enemyNum = 5;
	m_enemy.resize(enemyNum);
	for (int i = 0; i < enemyNum; i++)
	{
		m_enemy[i] = std::move(Enemy::Create());
	}
	//ロックオンの生成
	m_lockon = std::make_unique<Lockon>();
	m_lockon->SetPlayer(dynamic_cast<Player*>(m_player));
	m_lockon->SetEnemy(&m_enemy);
	// スプライトバッチを作成
	m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dContext.Get());
	DebugFont::InitializeStatic(m_d3dDevice, m_d3dContext);
	// デバッグテキストを作成
	m_debugText = std::make_unique<DebugText>(m_d3dDevice.Get(), m_spriteBatch.get());
	//初期化
	isDebug = false;
	//リソース情報
	ComPtr<ID3D11Resource> resource;
	//png用
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"Resources/hpgageGreen.png",
			resource.GetAddressOf(),
			m_HpGreentexture[0].ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"Resources/hpgageRed.png",
			resource.GetAddressOf(),
			m_HpRedtexture[0].ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"Resources/hpgageWhite.png",
			resource.GetAddressOf(),
			m_HpWhitetexture[0].ReleaseAndGetAddressOf()));

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"Resources/hpgageGreen2.png",
			resource.GetAddressOf(),
			m_HpGreentexture[1].ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"Resources/hpgageRed2.png",
			resource.GetAddressOf(),
			m_HpRedtexture[1].ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"Resources/hpgageWhite2.png",
			resource.GetAddressOf(),
			m_HpWhitetexture[1].ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"Resources/gameclear.png",
			resource.GetAddressOf(),
			m_clearTexture.ReleaseAndGetAddressOf()));
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(m_d3dDevice.Get(), L"Resources/gameover.png",
			resource.GetAddressOf(),
			m_overTexture.ReleaseAndGetAddressOf()));

	//ロックオンの対象の作成
	m_lockonTarget.resize(1);
	for (int i = 0; i < m_lockonTarget.size(); i++)
	{
		m_lockonTarget[i].LoadModel(L"Resources/Lockon.cmo");
		m_lockonTarget[i].EnbleAlpha();
		m_lockonTarget[i].DisableLighting();
		m_lockonTarget[i].SetScale(Vector3(2,2,2));
	}

	//テクスチャの原点を画像の中心にする
	m_origin.x = 0;// float(catDesc.Width);	//x座標
	m_origin.y = 0;//float(catDesc.Height / 2); //y座標
	//表示座標を画面の中央に指定
	m_screenPos.x = 50;// / 2.f;	//x座標
	m_screenPos.y = 400;// / 2.f;	//y座標
	m_count = 0;
	m_m_count = 0;

	m_bosshp = 0;
	m_bosshpOld = 0;
	m_timer1 = 0;
	m_timer2 = 0;
	m_bossFlag = false;
	m_clearFlag = false;
	m_playerExplosion = false;

	//acfファイルの読み込み
	ADX2Le::Initialize("Resources/Music/test.acf");
	//ACBとAWBを読み込む
	ADX2Le::LoadAcb("Resources/Music/CueSheet_0.acb");
	//音の再生
	ADX2Le::Play(CRI_CUESHEET_0_CHESS);

	//m_vertx[0] = { Vector3(-0.5f, 0.5f,0.0f),Vector2(0.0f,0.0f) };
	//m_vertx[1] = { Vector3(0.5f, 0.5f,0.0f),Vector2(1.0f,0.0f) };
	//m_vertx[2] = { Vector3(0.5f,-0.5f,0.0f),Vector2(1.0f,1.0f) };
	//m_vertx[3] = { Vector3(-0.5f,-0.5f,0.0f),Vector2(0.0f,1.0f) };
	//m_index[0] = 0;
	//m_index[1] = 1;
	//m_index[2] = 2;	
	//m_index[3] = 0;
	//m_index[4] = 2;
	//m_index[5] = 3;
	//CreateWICTextureFromFile(m_d3dDevice.Get(), L"Assets/Lockon.png", nullptr, m_texture.GetAddressOf());
	//m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(m_d3dContext.Get());
	//m_bassicEffect = std::make_unique<BasicEffect>(m_d3dDevice.Get());
	//m_bassicEffect->SetVertexColorEnabled(false);
	//m_bassicEffect->SetLightingEnabled(false);
	//m_bassicEffect->SetTextureEnabled(true);
	//m_bassicEffect->SetPerPixelLighting(false);
	////使用するシェーダー(プログラム)を取得
	//void const* shaderByteCode1;	//プログラムの先頭アドレス
	//size_t byteCodeLength1;		//プログラムのサイズ
	//m_bassicEffect->GetVertexShaderBytecode(&shaderByteCode1, &byteCodeLength1);

	//ComPtr<ID3D11InputLayout>inputLayout;
	//m_d3dDevice->CreateInputLayout(DirectX::VertexPositionTexture::InputElements, DirectX::VertexPositionTexture::InputElementCount, shaderByteCode1, byteCodeLength1, inputLayout.GetAddressOf());

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
	//m_debugText->AddText(Vector2(0,  0), ptr);
	//m_debugText->AddText(Vector2(0, 20), L"A,D     :Rotation");
	//m_debugText->AddText(Vector2(0, 40), L"Q        :Break-Up");
	//m_debugText->AddText(Vector2(0, 60), L"E         :Assembly");
	//m_debugText->AddText(Vector2(0, 80), L"C         :ChangeCamera");
	//m_debugText->AddText(Vector2(0, 100), L"Space :FireBullet");
	//m_debugText->AddText(Vector2(0, 120), L"1          :Collision");
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;
	//毎フレーム処理を追加する
	//m_debugCamera->Update();
	Device::Update();
	ADX2Le::Update();
	//デバッグ表示
	if (Device::m_keyboardTracker->pressed.D1)
	{
		m_player->ChangeCollision();
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
		{
			(*it)->ChangeCollision();
		}
	}

	//プレイヤーのアップデート
	if (m_count < 150)
		m_player->Update();
	if (m_count >= 150 && !m_playerExplosion)
	{
		m_playerExplosion = true;
		ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 30, m_player->GetTrans(), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(15.0f));
	}
	{//天球とプレイヤーの当たり判定
		Collision::Sphere* sphere = dynamic_cast<Collision::Sphere*>(m_player->GetCollisionNode());
		//自機のワールド座標
		Vector3 trans = m_player->GetTrans();
		//球からプレイヤーへのベクトル
		Vector3 sphrere2player = trans - sphere->Center;
		//めり込み排斥ベクトル
		Vector3 reject;
		if (m_landShapeobjSkydome.IntersectSphere(*sphere, &reject))
		{
			if (dynamic_cast<Player*>(m_player)->GetInvincible() <= 110)
			{
				//dynamic_cast<Player*>(m_player)->SetInvincible(120);
				//m_count += 15;
				Vector3 playerAngle = m_player->GetRot();
				m_player->SetRot(Vector3(playerAngle.x, playerAngle.y + XMConvertToRadians(180), playerAngle.z));
				dynamic_cast<Player*>(m_player)->SetInvincible(120);
			}
			//めり込みを解消するように球をズラす
			//sphere->Center += reject;
		}

	}
	//自機の地形へのめり込みを解消する
	{
		Collision::Sphere* sphere = dynamic_cast<Collision::Sphere*>(m_player->GetCollisionNode());
		//自機のワールド座標
		Vector3 trans = m_player->GetTrans();
		//球からプレイヤーへのベクトル
		Vector3 sphrere2player = trans - sphere->Center;
		//めり込み排斥ベクトル
		Vector3 reject;
		if (m_landShapeGround.IntersectSphere(*sphere, &reject))
		{
			if (dynamic_cast<Player*>(m_player)->GetInvincible() == 0)
			{
				dynamic_cast<Player*>(m_player)->SetInvincible(120);
				m_count += 15;
				ADX2Le::Play(CRI_CUESHEET_0_HIT);
			}
			//めり込みを解消するように球をズラす
			sphere->Center += reject;
		}
		//自機を移動
		m_player->SetTrans(sphere->Center + sphrere2player);
		//ワールド行列を更新
		//m_player->Update();
	}
	{//自機が地面に乗る処理
		const Vector3 vel = m_player->GetVelocity();
		if (vel.y <= 0.0f)
		{
			//自機の頭から足元への線分
			Collision::Segment player_segment;
			//自機のワールド座標
			Vector3 trans = m_player->GetTrans();
			//プレイヤーの頭
			player_segment.Start = trans + Vector3(0, 1, 0);
			//プレイヤーの足元+aして挙動を自然にする
			player_segment.End = trans + Vector3(0, 0.5f, 0);
			//交点座標
			Vector3 inter;
			//地形と線分の当たり判定(レイキャスト RayCasting)
			if (m_landShapeGround.IntersectSegment(player_segment, &inter))
			{
				//Y座標を交点に移動させる
				trans.y = inter.y;
				//落下を終了
				m_player->StopFall();
			}
			else
			{
				//落下を開始
				m_player->StartFall();
			}
			//自機を移動
			m_player->SetTrans(trans);
			//ワールド行列を更新
			//m_player->Update();
		}
	}

	//敵の更新
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
	{
		(*it)->Update();
	}
	//ボス
	if(m_boss)
		m_boss->Update();
	if(m_boss)
	{//天球とボス当たり判定
		Collision::Sphere* sphere = dynamic_cast<Collision::Sphere*>(m_boss->GetCollisionNode());
		//自機のワールド座標
		Vector3 trans = m_boss->GetTrans();
		//球からボスのベクトル
		Vector3 sphrere2boss = trans - sphere->Center;
		//めり込み排斥ベクトル
		Vector3 reject;
		if (m_landShapeobjSkydome.IntersectSphere(*sphere, &reject))
		{
			if (m_boss->GetWallCount() == 0)
			{
				//dynamic_cast<Player*>(m_player)->SetInvincible(120);
				//m_count += 15;
				Vector3 bossAngle = m_boss->GetRot();
				//float angle = 
				if (bossAngle.y < -5)
				{
					m_boss->SetRot(Vector3(bossAngle.x, bossAngle.y + XMConvertToRadians(180), bossAngle.z));
				}
				else
				{
					m_boss->SetRot(Vector3(bossAngle.x, bossAngle.y - XMConvertToRadians(180), bossAngle.z));
				}
				m_boss->SetWallCount(60);
			}
			//めり込みを解消するように球をズラす
			//sphere->Center += reject;
		}

	}


	//ロックオンの対象の更新
	for (int i = 0; i < m_lockonTarget.size(); i++)
	{
		m_lockonTarget[i].Update();
		if (m_lockon->SearchEnemy())
		{
			//ロックオンの対象の座標
			Vector3 player2enemy = m_lockon->SearchEnemy()->GetTrans() - m_player->GetTrans();
			player2enemy.Normalize();
			m_lockonTarget[i].SetTrans(m_lockon->SearchEnemy()->GetTrans() - player2enemy);
		}
	}
	{//弾丸と敵の当たり判定
		for (int i = 1; i <= dynamic_cast<Player*>(m_player)->GetBulletNum(); i++)
		{
			//弾の当たり判定
			CollisionNode::SphereNode bulletSphere = dynamic_cast<Player*>(m_player)->GetBullet(i - 1)->GetCollisionNodeBullet();
			//敵の数だけ処理する i++をなくすことで進みすぎないようにする
			for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin();
				it != m_enemy.end();)
			{
				Enemy* enemy = it->get();
				//敵の判定球を取得
				const CollisionNode::CollisionNode* enemySphereConst = enemy->GetCollisionNode();
				CollisionNode::CollisionNode* enemySphere = const_cast<CollisionNode::CollisionNode*>(enemySphereConst);
				//２つの弾が当たっていたら
				if (CheckSphere2Sphere(bulletSphere, *(dynamic_cast<Collision::Sphere*>(enemySphere))))
				{
					//エフェクトを発生させる
					ModelEffectManager::getInstance()->Entry(L"Resources/HitEffect.cmo", 10, dynamic_cast<Player*>(m_player)->GetBullet(i - 1)->GetTrans(), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(10.0f));
					//敵を消す
					//消した要素の次の要素を指すイテレータ
					it = m_enemy.erase(it);
					dynamic_cast<Player*>(m_player)->DeleteBullet(i - 1);
					ADX2Le::Play(CRI_CUESHEET_0_HIT);
				}
				else
				{
					//消さなかった場合にイテレータを進める
					it++;
				}
			}
			if (m_boss)
			{
				//弾とボスの当たり判定
				//ボスの判定球を取得
				CollisionNode::CollisionNode* bossSphere = m_boss->GetCollisionNode();
				if (CheckSphere2Sphere((bulletSphere), *(dynamic_cast<Collision::Sphere*>(bossSphere))))
				{
					//エフェクトを発生させる
					ModelEffectManager::getInstance()->Entry(L"Resources/HitEffect.cmo", 10, dynamic_cast<Player*>(m_player)->GetBullet(i - 1)->GetTrans(), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(10.0f));
					dynamic_cast<Player*>(m_player)->DeleteBullet(i - 1);
					m_boss->DecreaseHP();
					m_bosshp += 2;
					m_timer1 = 0;
					ADX2Le::Play(CRI_CUESHEET_0_HIT);
					if (m_boss->GetHp() == 0)
					{
						for (int i = 0; i < 5; i++)
						{
							ADX2Le::Play(CRI_CUESHEET_0_HIT);
						}
						m_clearFlag = true;
						Vector3 bossTrans = m_boss->GetTrans();
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 30, Vector3(bossTrans.x + 3.f, bossTrans.y + 6.f, bossTrans.z), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 35, Vector3(bossTrans.x - 3.f, bossTrans.y + 6.f, bossTrans.z), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 40, Vector3(bossTrans.x, bossTrans.y + 4.f, bossTrans.z + 3.f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 45, Vector3(bossTrans.x, bossTrans.y + 5.f, bossTrans.z - 3.f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 50, Vector3(bossTrans.x + 1.f, bossTrans.y + 4.f, bossTrans.z), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 55, Vector3(bossTrans.x + -1.f, bossTrans.y + 5.f, bossTrans.z + 1.f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 60, Vector3(bossTrans.x + 2.f, bossTrans.y + 6.f, bossTrans.z - 1.f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 65, Vector3(bossTrans.x - 2.f, bossTrans.y + 5.f, bossTrans.z + 2.f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 70, Vector3(bossTrans.x, bossTrans.y + 4.f, bossTrans.z - 2.f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(70.0f));
						m_boss.reset();
					}
				}
			}
		}
	}
	{//自機と敵の当たり判定
	 //弾の当たり判定
		if (dynamic_cast<Player*>(m_player)->GetInvincible() == 0)
		{
			CollisionNode::CollisionNode* playersphere = dynamic_cast<Player*>(m_player)->GetCollisionNode();
			//敵の数だけ処理する i++をなくすことで進みすぎないようにする
			for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin();
				it != m_enemy.end();)
			{
				Enemy* enemy = it->get();
				//敵の判定球を取得
				const CollisionNode::CollisionNode* enemySphereConst = enemy->GetCollisionNode();
				CollisionNode::CollisionNode* enemySphere = const_cast<CollisionNode::CollisionNode*>(enemySphereConst);
				//２つの弾が当たっていたら
				if (CheckSphere2Sphere(*(dynamic_cast<Collision::Sphere*>(playersphere)), *(dynamic_cast<Collision::Sphere*>(enemySphere))))
				{
					it++;
					//it = m_enemy.erase(it);
					ADX2Le::Play(CRI_CUESHEET_0_HIT);
					dynamic_cast<Player*>(m_player)->SetInvincible(120);
					m_count += 15;
				}
				else
				{
					//消さなかった場合にイテレータを進める
					it++;
				}
			}

		}
	}
	if (m_boss)
	{//自機とボスの当たり判定
	 //自機の当たり判定
		if (dynamic_cast<Player*>(m_player)->GetInvincible() == 0)
		{
			//プレイヤーの判定
			CollisionNode::CollisionNode* playersphere = dynamic_cast<Player*>(m_player)->GetCollisionNode();
			//ボスの判定球を取得
			CollisionNode::CollisionNode* bossSphere = m_boss->GetCollisionNode();
			//２つの弾が当たっていたら
			if (CheckSphere2Sphere(*(dynamic_cast<Collision::Sphere*>(playersphere)), *(dynamic_cast<Collision::Sphere*>(bossSphere))))
			{
				dynamic_cast<Player*>(m_player)->SetInvincible(120);
				m_count += 30;
				ADX2Le::Play(CRI_CUESHEET_0_HIT);
			}
			//自機とボスの弾との当たり判定
			for (int i = 1; i <= m_boss->GetBulletNum(); i++)
			{
				//ボスの弾の当たり判定の取得
				CollisionNode::SphereNode bossBulletSphere = m_boss->GetBullet(i - 1)->GetCollisionNodeBullet();
				if (CheckSphere2Sphere(*(dynamic_cast<Collision::Sphere*>(playersphere)), bossBulletSphere))
				{
					dynamic_cast<Player*>(m_player)->SetInvincible(120);
					m_count += 30;
					ADX2Le::Play(CRI_CUESHEET_0_HIT);

				}
			}
		}
	}

	//敵がいなくなったら
	if (m_enemy.empty() && !m_bossFlag)
	{
		m_boss = std::move(Boss::Create());
		m_bossFlag = true;
		//m_debugText->AddText(Vector2(350, 300), L"CLEAR");
	}
	//スカイドームの更新
	//m_objSkydome.Update();
	m_landShapeobjSkydome.Update();
	//地面の更新
	m_landShapeGround.Update();
	//モデルエフェクトの更新
	ModelEffectManager::getInstance()->Update();
	{//自機に追従するカメラ
	 //カメラの更新
		m_camera->SetTargetPos(m_player->GetTrans());
		m_camera->SetTargetAngle(m_player->GetRot());
		m_camera->Update();
		m_view = m_camera->GetViewMatrix();
		m_proj = m_camera->GetProjectionMatrix();
	}
	if (m_m_count != m_count)
	{
		m_timer2++;
		if (/*dynamic_cast<Player*>(m_player)->GetInvincible() < 60*/m_timer2 > 60)
		{
			m_m_count++;
		}
	}
	if (m_m_count == m_count)
		m_timer2 = 0;
	if (m_bosshp != m_bosshpOld)
	{
		m_timer1++;
		if (m_timer1 > 60)
		{
			m_bosshpOld++;
		}
	}
	if (m_bosshp == m_bosshpOld)
	{
		m_timer1 = 0;
	}
	//m_count++;
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


	//m_bassicEffect->SetView(m_view);
	//m_bassicEffect->SetProjection(m_proj);
	//m_bassicEffect->SetTexture(m_texture.Get());
	//m_bassicEffect->SetLightEnabled(0, true);
	//m_bassicEffect->SetLightEnabled(1, false);
	//m_bassicEffect->SetLightEnabled(2, false);
	//m_bassicEffect->SetLightDiffuseColor(0, Colors::White);
	////ライトの方向
	//static float angle = 0;
	//angle += 0.01f;
	//Vector3 lightDir(1, -1, 0);
	//lightDir.Normalize();	//長さを1にする
	//Matrix rot = Matrix::CreateRotationY(angle);
	//lightDir = Vector3::Transform(lightDir, rot);
	//m_bassicEffect->SetLightDirection(0, lightDir);
	//m_bassicEffect->Apply(m_d3dContext.Get());

	//渡す
	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);
	m_effect->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());
	//スカイドームの描画
	//m_objSkydome.Render();
	m_landShapeobjSkydome.Draw();
	//地面の描画
	m_landShapeGround.Draw();


	//敵の描画
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
	{
		(*it)->Render();
	}
	if(m_boss)
		m_boss->Render();
	//m_primitiveBatch->Begin();
	//m_primitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_index, 6, m_vertx, 4);
	//m_primitiveBatch->End();

	//プレイヤーの描画
	//if(m_count % 15 == 0)
	if(m_count < 150)
		m_player->Render();
	//ロックオンの対象の描画
	for (unsigned int i = 0; i < m_lockonTarget.size(); i++)
	{
		if (m_lockon->SearchEnemy())
			m_lockonTarget[i].DrawBillboard();
	}
	//モデルエフェクトの描画
	ModelEffectManager::getInstance()->Render();

	//prmitiveBatchの描画開始時に必須
	m_batch->Begin();
	//描画処理
	//四角形の描画
	m_batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		indices, 6, vertices, 4);
	////primitiveBatchの描画終了時に必須
	m_batch->End();
	CommonStates states(m_d3dDevice.Get());
	m_spriteBatch->Begin(SpriteSortMode_Deferred,states.NonPremultiplied());
	//テクスチャの切り取り矩形
	RECT rect{ 0,m_count,25,150 };
	RECT rect1{ 0,m_m_count,25,150 };
	m_spriteBatch->Draw(
		m_HpWhitetexture[0].Get(),			//テクスチャの情報 
		DirectX::SimpleMath::Vector2(m_screenPos.x, m_screenPos.y),//位置
		nullptr,					//画像の切り取り(AABB)
		Colors::White,				//色
		XMConvertToRadians(0),		//角度(ラジアン)、角度をラジアンにする関数
		m_origin,					//回転中心点
		1.0f);						//大きさの倍率(指定しないとデフォルトで1.0f)

	//スプライトの描画
	m_spriteBatch->Draw(
		m_HpRedtexture[0].Get(),			//テクスチャの情報 
		DirectX::SimpleMath::Vector2(m_screenPos.x, m_screenPos.y + m_m_count),//位置
		&rect1,					//画像の切り取り(AABB)
		Colors::White,				//色
		XMConvertToRadians(0),		//角度(ラジアン)、角度をラジアンにする関数
		m_origin,					//回転中心点
		1.0f);						//大きさの倍率(指定しないとデフォルトで1.0f)
	m_spriteBatch->Draw(
		m_HpGreentexture[0].Get(),			//テクスチャの情報 
		DirectX::SimpleMath::Vector2(m_screenPos.x, m_screenPos.y + m_count),//位置
		&rect,					//画像の切り取り(AABB)
		Colors::White,				//色
		XMConvertToRadians(0),		//角度(ラジアン)、角度をラジアンにする関数
		m_origin,					//回転中心点
		1.0f);						//大きさの倍率(指定しないとデフォルトで1.0f)

	if (m_boss)
	{
		RECT rect2{ 0,0,200 - m_bosshp,50 };
		RECT rect3{ 0,0,200 - m_bosshpOld,50 };

										//スプライトの描画

		m_spriteBatch->Draw(
			m_HpWhitetexture[1].Get(),			//テクスチャの情報 
			DirectX::SimpleMath::Vector2(300, 20),//位置
			nullptr,					//画像の切り取り(AABB)
			Colors::White,				//色
			XMConvertToRadians(0),		//角度(ラジアン)、角度をラジアンにする関数
			m_origin,					//回転中心点
			1.0f);						//大きさの倍率(指定しないとデフォルトで1.0f)

		m_spriteBatch->Draw(
			m_HpRedtexture[1].Get(),			//テクスチャの情報 
			DirectX::SimpleMath::Vector2(300, 20),//位置
			&rect3,					//画像の切り取り(AABB)
			Colors::White,				//色
			XMConvertToRadians(0),		//角度(ラジアン)、角度をラジアンにする関数
			m_origin,					//回転中心点
			1.0f);						//大きさの倍率(指定しないとデフォルトで1.0f)

		m_spriteBatch->Draw(
			m_HpGreentexture[1].Get(),			//テクスチャの情報 
			DirectX::SimpleMath::Vector2(300, 20),//位置
			&rect2,					//画像の切り取り(AABB)
			Colors::White,				//色
			XMConvertToRadians(0),		//角度(ラジアン)、角度をラジアンにする関数
			m_origin,					//回転中心点
			1.0f);						//大きさの倍率(指定しないとデフォルトで1.0f)

	}
	if (m_count >= 150 && !m_clearFlag)
	{
		m_spriteBatch->Draw(
			m_overTexture.Get(),			//テクスチャの情報 
			DirectX::SimpleMath::Vector2(400, 300),//位置
			nullptr,					//画像の切り取り(AABB)
			Colors::White,				//色
			XMConvertToRadians(0),		//角度(ラジアン)、角度をラジアンにする関数
			Vector2(331.f / 2.f,195.f /2.f),					//回転中心点
			1.0f);						//大きさの倍率(指定しないとデフォルトで1.0f)
	}
	if (m_clearFlag)
	{
		m_spriteBatch->Draw(
			m_clearTexture.Get(),			//テクスチャの情報 
			DirectX::SimpleMath::Vector2(400, 300),//位置
			nullptr,					//画像の切り取り(AABB)
			Colors::White,				//色
			XMConvertToRadians(0),		//角度(ラジアン)、角度をラジアンにする関数
			Vector2(400.f / 2.f, 74.f / 2.f),					//回転中心点
			1.0f);						//大きさの倍率(指定しないとデフォルトで1.0f)
	}

	m_debugText->Draw();
	m_spriteBatch->End();

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

	static const D3D_FEATURE_LEVEL featureLevels[] =
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
			D3D11_MESSAGE_ID hide[] =
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
	ID3D11RenderTargetView* nullViews[] = { nullptr };
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
	Vector3 lerpPos = Vector3(0, 0, 0);
	lerpPos = (1 - t)* startPos + t * targetPos;
	return lerpPos;
}

int Game::Lerp(int startPos, int targetPos, float t)
{
	int lerpPos = 0;
	lerpPos = (1 - t)* startPos + t * targetPos;
	return lerpPos;
}




