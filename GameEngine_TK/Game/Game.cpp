//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Game.cpp
//!
//! @brief  �Q�[���̃\�[�X�t�@�C��
//!
//! @date   2017/06/04
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

//�C���N���[�h
//#include "pch\pch.h"
#include "DebugFont\DebugFont.h"
#include <WICTextureLoader.h>
#include "Game.h"
#include <time.h>
#include "ADX2Le\ADX2Le.h"
#include "Basic\Basic.h"
#include "CueSheet\CueSheet_0.h"

extern void ExitGame();

//���O���
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
HRESULT Game::Initialize(HWND window, int width, int height)
{
	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	CreateDevice();

	CreateResources();

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
	//�Ǝ��̏������͂����ɏ���

	//�f�o�C�X�̐���
	Device::Initialize();
	//�J�����̐���
	m_camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	m_camera->SetKeyboard(Device::m_keyboard.get());
	//3d�I�u�W�F�N�g�̐ÓI�����o�ϐ���������
	Obj3d::InitializeStatic(m_d3dDevice, m_d3dContext, m_camera.get());

	LandShapeCommonDef lscDef;
	lscDef.pDevice = m_d3dDevice.Get();
	lscDef.pDeviceContext = m_d3dContext.Get();
	lscDef.pCamera = m_camera.get();
	LandShape::InitializeCommon(lscDef);

	//unique_ptr...�X�R�[�v�𔲂����玩���I��delete���Ă����̂ŁA���������[�N�̕K�v���Ȃ�
	//smart_pointer.Get()�Ő��̃|�C���^�[���擾
	//�l�p�`�p
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());
	m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());
	//Projection...�ˉe�s��
	m_effect->SetProjection(XMMatrixOrthographicOffCenterRH(0,
		static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight), 0, 0, 1.0f));
	//�֐��̌Ăяo���́A���̂܂܂ŏo����
	m_effect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());
	//�ėp�X�e�[�g�ݒ�𐶐�
	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	//�f�o�b�N�J�����𐶐�
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);
	//m_objSkydome.LoadModel(L"skydome.cmo");
	//m_objSkydome.DisableLighting();

	////���f���̐���(�n��)			�f�o�C�X			cmo�t�@�C���̏ꏊ���w��  �G�t�F�N�g�t�@�N�g���[
	//m_modelGround.LoadModel(L"Resources/ground200m.cmo");
	m_landShapeobjSkydome.Initialize(L"skydome", L"skydome");
	m_landShapeobjSkydome.DisableLighting();
	//�n�`�f�[�^�̓ǂݍ���
	m_landShapeGround.Initialize(L"ground200m", L"ground200m");
	//m_landShapeGround.DisableLighting();
	//�����̏�����
	srand(static_cast<unsigned int>(time(nullptr)));
	//�v���C���[�̐���
	m_player = Player::Create();
	//�G�̐���
	int enemyNum = 5;
	m_enemy.resize(enemyNum);
	for (int i = 0; i < enemyNum; i++)
	{
		m_enemy[i] = std::move(Enemy::Create());
	}
	//���b�N�I���̐���
	m_lockon = std::make_unique<Lockon>();
	m_lockon->SetPlayer(dynamic_cast<Player*>(m_player));
	m_lockon->SetEnemy(&m_enemy);
	// �X�v���C�g�o�b�`���쐬
	m_spriteBatch = std::make_unique<SpriteBatch>(m_d3dContext.Get());
	DebugFont::InitializeStatic(m_d3dDevice, m_d3dContext);
	// �f�o�b�O�e�L�X�g���쐬
	m_debugText = std::make_unique<DebugText>(m_d3dDevice.Get(), m_spriteBatch.get());
	//������
	isDebug = false;
	//���\�[�X���
	ComPtr<ID3D11Resource> resource;
	//png�p
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

	//���b�N�I���̑Ώۂ̍쐬
	m_lockonTarget.resize(1);
	for (int i = 0; i < m_lockonTarget.size(); i++)
	{
		m_lockonTarget[i].LoadModel(L"Resources/Lockon.cmo");
		m_lockonTarget[i].EnbleAlpha();
		m_lockonTarget[i].DisableLighting();
		m_lockonTarget[i].SetScale(Vector3(2,2,2));
	}

	//�e�N�X�`���̌��_���摜�̒��S�ɂ���
	m_origin.x = 0;// float(catDesc.Width);	//x���W
	m_origin.y = 0;//float(catDesc.Height / 2); //y���W
	//�\�����W����ʂ̒����Ɏw��
	m_screenPos.x = 50;// / 2.f;	//x���W
	m_screenPos.y = 400;// / 2.f;	//y���W
	m_count = 0;
	m_m_count = 0;

	m_bosshp = 0;
	m_bosshpOld = 0;
	m_timer1 = 0;
	m_timer2 = 0;
	m_bossFlag = false;
	m_clearFlag = false;
	m_playerExplosion = false;

	//acf�t�@�C���̓ǂݍ���
	ADX2Le::Initialize("Resources/Music/test.acf");
	//ACB��AWB��ǂݍ���
	ADX2Le::LoadAcb("Resources/Music/CueSheet_0.acb");
	//���̍Đ�
	ADX2Le::Play(CRI_CUESHEET_0_CHESS);
	
	//hlsl�t�@�C���ǂݍ��݁@�u���u�쐬
	ID3DBlob* pCompiledShader = NULL;
	//�o�[�e�b�N�X�V�F�[�_�[
	if (FAILED(MakeShader("Point.hlsl", "VS", "vs_5_0", (void**)&m_vertexShader, &pCompiledShader))) return E_FAIL;
	//���_�C���v�b�g���C�A�E�g���`
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_d3dDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_inputLayout)))
	{
		return FALSE;
	}
	SAFE_RELEASE(pCompiledShader);
	//�s�N�Z���V�F�[�_�[�쐬
	if (FAILED(MakeShader("Point.hlsl", "PS", "ps_5_0", (void**)&m_pixelShader, &pCompiledShader))) return E_FAIL;
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_d3dDevice->CreateBuffer(&cb, NULL, &m_constantBuffer)))
	{
		return E_FAIL;
	}
	//�_�Ƃ��Ẵo�[�e�b�N�X�o�b�t�@�[�쐬
	if (FAILED(InitModel()))
	{
		return E_FAIL;
	}

	//�V�F�[�_�[������
	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	//�|���S���쐬
	if (FAILED(InitPolygon()))
	{
		return E_FAIL;
	}

	return S_OK;
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
	//���t���[��������ǉ�����
	//m_debugCamera->Update();
	Device::Update();
	ADX2Le::Update();
	//�f�o�b�O�\��
	if (Device::m_keyboardTracker->pressed.D1)
	{
		m_player->ChangeCollision();
		for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
		{
			(*it)->ChangeCollision();
		}
	}

	//�v���C���[�̃A�b�v�f�[�g
	if (m_count < 150)
		m_player->Update();
	if (m_count >= 150 && !m_playerExplosion)
	{
		m_playerExplosion = true;
		ModelEffectManager::getInstance()->Entry(L"Resources/explosion.cmo", 30, m_player->GetTrans(), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(15.0f));
	}
	//{//�V���ƃv���C���[�̓����蔻��
	//	Collision::Sphere* sphere = dynamic_cast<Collision::Sphere*>(m_player->GetCollisionNode());
	//	//���@�̃��[���h���W
	//	Vector3 trans = m_player->GetTrans();
	//	//������v���C���[�ւ̃x�N�g��
	//	Vector3 sphrere2player = trans - sphere->Center;
	//	//�߂荞�ݔr�˃x�N�g��
	//	Vector3 reject;
	//	if (m_landShapeobjSkydome.IntersectSphere(*sphere, &reject))
	//	{
	//		if (dynamic_cast<Player*>(m_player)->GetInvincible() <= 110)
	//		{
	//			//dynamic_cast<Player*>(m_player)->SetInvincible(120);
	//			//m_count += 15;
	//			Vector3 playerAngle = m_player->GetRot();
	//			m_player->SetRot(Vector3(playerAngle.x, playerAngle.y + XMConvertToRadians(180), playerAngle.z));
	//			dynamic_cast<Player*>(m_player)->SetInvincible(120);
	//		}
	//		//�߂荞�݂���������悤�ɋ����Y����
	//		//sphere->Center += reject;
	//	}

	//}
	//���@�̒n�`�ւ̂߂荞�݂���������
	{
		Collision::Sphere* sphere = dynamic_cast<Collision::Sphere*>(m_player->GetCollisionNode());
		//���@�̃��[���h���W
		Vector3 trans = m_player->GetTrans();
		//������v���C���[�ւ̃x�N�g��
		Vector3 sphrere2player = trans - sphere->Center;
		//�߂荞�ݔr�˃x�N�g��
		Vector3 reject;
		if (m_landShapeGround.IntersectSphere(*sphere, &reject))
		{
			if (dynamic_cast<Player*>(m_player)->GetInvincible() == 0)
			{
				dynamic_cast<Player*>(m_player)->SetInvincible(120);
				m_count += 15;
				ADX2Le::Play(CRI_CUESHEET_0_HIT);
			}
			//�߂荞�݂���������悤�ɋ����Y����
			sphere->Center += reject;
		}
		//���@���ړ�
		m_player->SetTrans(sphere->Center + sphrere2player);
		//���[���h�s����X�V
		//m_player->Update();
	}
	{//���@���n�ʂɏ�鏈��
		const Vector3 vel = m_player->GetVelocity();
		if (vel.y <= 0.0f)
		{
			//���@�̓����瑫���ւ̐���
			Collision::Segment player_segment;
			//���@�̃��[���h���W
			Vector3 trans = m_player->GetTrans();
			//�v���C���[�̓�
			player_segment.Start = trans + Vector3(0, 1, 0);
			//�v���C���[�̑���+a���ċ��������R�ɂ���
			player_segment.End = trans + Vector3(0, 0.5f, 0);
			//��_���W
			Vector3 inter;
			//�n�`�Ɛ����̓����蔻��(���C�L���X�g RayCasting)
			if (m_landShapeGround.IntersectSegment(player_segment, &inter))
			{
				//Y���W����_�Ɉړ�������
				trans.y = inter.y;
				//�������I��
				m_player->StopFall();
			}
			else
			{
				//�������J�n
				m_player->StartFall();
			}
			//���@���ړ�
			m_player->SetTrans(trans);
			//���[���h�s����X�V
			//m_player->Update();
		}
	}

	//�G�̍X�V
	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
	{
		(*it)->Update();
	}
	//�{�X
	if(m_boss)
		m_boss->Update();
	if(m_boss)
	{//�V���ƃ{�X�����蔻��
		Collision::Sphere* sphere = dynamic_cast<Collision::Sphere*>(m_boss->GetCollisionNode());
		//���@�̃��[���h���W
		Vector3 trans = m_boss->GetTrans();
		//������{�X�̃x�N�g��
		Vector3 sphrere2boss = trans - sphere->Center;
		//�߂荞�ݔr�˃x�N�g��
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
			//�߂荞�݂���������悤�ɋ����Y����
			//sphere->Center += reject;
		}

	}


	//���b�N�I���̑Ώۂ̍X�V
	for (int i = 0; i < m_lockonTarget.size(); i++)
	{
		m_lockonTarget[i].Update();
		if (m_lockon->SearchEnemy())
		{
			//���b�N�I���̑Ώۂ̍��W
			Vector3 player2enemy = m_lockon->SearchEnemy()->GetTrans() - m_player->GetTrans();
			player2enemy.Normalize();
			m_lockonTarget[i].SetTrans(m_lockon->SearchEnemy()->GetTrans() - player2enemy);
		}
	}
	{//�e�ۂƓG�̓����蔻��
		for (int i = 1; i <= dynamic_cast<Player*>(m_player)->GetBulletNum(); i++)
		{
			//�e�̓����蔻��
			CollisionNode::SphereNode bulletSphere = dynamic_cast<Player*>(m_player)->GetBullet(i - 1)->GetCollisionNodeBullet();
			//�G�̐������������� i++���Ȃ������ƂŐi�݂����Ȃ��悤�ɂ���
			for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin();
				it != m_enemy.end();)
			{
				Enemy* enemy = it->get();
				//�G�̔��苅���擾
				const CollisionNode::CollisionNode* enemySphereConst = enemy->GetCollisionNode();
				CollisionNode::CollisionNode* enemySphere = const_cast<CollisionNode::CollisionNode*>(enemySphereConst);
				//�Q�̒e���������Ă�����
				if (CheckSphere2Sphere(bulletSphere, *(dynamic_cast<Collision::Sphere*>(enemySphere))))
				{
					//�G�t�F�N�g�𔭐�������
					ModelEffectManager::getInstance()->Entry(L"Resources/HitEffect.cmo", 10, dynamic_cast<Player*>(m_player)->GetBullet(i - 1)->GetTrans(), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(0.0f), Vector3(10.0f));
					//�G������
					//�������v�f�̎��̗v�f���w���C�e���[�^
					it = m_enemy.erase(it);
					dynamic_cast<Player*>(m_player)->DeleteBullet(i - 1);
					ADX2Le::Play(CRI_CUESHEET_0_HIT);
				}
				else
				{
					//�����Ȃ������ꍇ�ɃC�e���[�^��i�߂�
					it++;
				}
			}
			if (m_boss)
			{
				//�e�ƃ{�X�̓����蔻��
				//�{�X�̔��苅���擾
				CollisionNode::CollisionNode* bossSphere = m_boss->GetCollisionNode();
				if (CheckSphere2Sphere((bulletSphere), *(dynamic_cast<Collision::Sphere*>(bossSphere))))
				{
					//�G�t�F�N�g�𔭐�������
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
	{//���@�ƓG�̓����蔻��
	 //�e�̓����蔻��
		if (dynamic_cast<Player*>(m_player)->GetInvincible() == 0)
		{
			CollisionNode::CollisionNode* playersphere = dynamic_cast<Player*>(m_player)->GetCollisionNode();
			//�G�̐������������� i++���Ȃ������ƂŐi�݂����Ȃ��悤�ɂ���
			for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin();
				it != m_enemy.end();)
			{
				Enemy* enemy = it->get();
				//�G�̔��苅���擾
				const CollisionNode::CollisionNode* enemySphereConst = enemy->GetCollisionNode();
				CollisionNode::CollisionNode* enemySphere = const_cast<CollisionNode::CollisionNode*>(enemySphereConst);
				//�Q�̒e���������Ă�����
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
					//�����Ȃ������ꍇ�ɃC�e���[�^��i�߂�
					it++;
				}
			}

		}
	}
	if (m_boss)
	{//���@�ƃ{�X�̓����蔻��
	 //���@�̓����蔻��
		if (dynamic_cast<Player*>(m_player)->GetInvincible() == 0)
		{
			//�v���C���[�̔���
			CollisionNode::CollisionNode* playersphere = dynamic_cast<Player*>(m_player)->GetCollisionNode();
			//�{�X�̔��苅���擾
			CollisionNode::CollisionNode* bossSphere = m_boss->GetCollisionNode();
			//�Q�̒e���������Ă�����
			if (CheckSphere2Sphere(*(dynamic_cast<Collision::Sphere*>(playersphere)), *(dynamic_cast<Collision::Sphere*>(bossSphere))))
			{
				dynamic_cast<Player*>(m_player)->SetInvincible(120);
				m_count += 30;
				ADX2Le::Play(CRI_CUESHEET_0_HIT);
			}
			//���@�ƃ{�X�̒e�Ƃ̓����蔻��
			for (int i = 1; i <= m_boss->GetBulletNum(); i++)
			{
				//�{�X�̒e�̓����蔻��̎擾
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

	//�G�����Ȃ��Ȃ�����
	if (m_enemy.empty() && !m_bossFlag)
	{
		m_boss = std::move(Boss::Create());
		m_bossFlag = true;
		//m_debugText->AddText(Vector2(350, 300), L"CLEAR");
	}
	//�X�J�C�h�[���̍X�V
	//m_objSkydome.Update();
	m_landShapeobjSkydome.Update();
	//�n�ʂ̍X�V
	m_landShapeGround.Update();
	//���f���G�t�F�N�g�̍X�V
	ModelEffectManager::getInstance()->Update();
	{//���@�ɒǏ]����J����
	 //�J�����̍X�V
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
	//���_�C���f�b�N�X
	uint16_t indices[] =
	{
		0,1,2,
		2,1,3
	};
	//���_���W
	VertexPositionNormal vertices[] =
	{//			���W						�@���x�N�g��
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

#pragma region test
	//// TODO: Add your rendering code here.
	////�|�C���^�[�Ȃ̂ŁA�A���[���Z�q//
	////Opaque...�s����
	//m_d3dContext->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	////Depth...���s���A�`�悷��Ƃ��̑O��֌W
	//m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	////Cull...�w�i�J�����O��L���ɂ��邩�ǂ���
	//m_d3dContext->RSSetState(m_states->CullNone());

	////�n��
	//m_effect->SetView(m_view);
	//m_effect->SetProjection(m_proj);
	//m_effect->Apply(m_d3dContext.Get());
	//m_d3dContext->IASetInputLayout(m_inputLayout.Get());
	////�X�J�C�h�[���̕`��
	////m_objSkydome.Render();
	m_landShapeobjSkydome.Draw();
	////�n�ʂ̕`��
	////m_landShapeGround.Draw();


	////�G�̕`��
	//for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_enemy.begin(); it != m_enemy.end(); it++)
	//{
	//	(*it)->Render();
	//}
	//if(m_boss)
	//	m_boss->Render();
	////m_primitiveBatch->Begin();
	////m_primitiveBatch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, m_index, 6, m_vertx, 4);
	////m_primitiveBatch->End();

	////�v���C���[�̕`��
	////if(m_count % 15 == 0)
	//if(m_count < 150)
	//	m_player->Render();
	////���b�N�I���̑Ώۂ̕`��
	//for (unsigned int i = 0; i < m_lockonTarget.size(); i++)
	//{
	//	if (m_lockon->SearchEnemy())
	//		m_lockonTarget[i].DrawBillboard();
	//}
	////���f���G�t�F�N�g�̕`��
	//ModelEffectManager::getInstance()->Render();

	////prmitiveBatch�̕`��J�n���ɕK�{
	//m_batch->Begin();
	////�`�揈��
	////�l�p�`�̕`��
	//m_batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	//	indices, 6, vertices, 4);
	//////primitiveBatch�̕`��I�����ɕK�{
	//m_batch->End();
	//CommonStates states(m_d3dDevice.Get());
	//m_spriteBatch->Begin(SpriteSortMode_Deferred,states.NonPremultiplied());
	////�e�N�X�`���̐؂����`
	//RECT rect{ 0,m_count,25,150 };
	//RECT rect1{ 0,m_m_count,25,150 };
	//m_spriteBatch->Draw(
	//	m_HpWhitetexture[0].Get(),			//�e�N�X�`���̏�� 
	//	DirectX::SimpleMath::Vector2(m_screenPos.x, m_screenPos.y),//�ʒu
	//	nullptr,					//�摜�̐؂���(AABB)
	//	Colors::White,				//�F
	//	XMConvertToRadians(0),		//�p�x(���W�A��)�A�p�x�����W�A���ɂ���֐�
	//	m_origin,					//��]���S�_
	//	1.0f);						//�傫���̔{��(�w�肵�Ȃ��ƃf�t�H���g��1.0f)

	////�X�v���C�g�̕`��
	//m_spriteBatch->Draw(
	//	m_HpRedtexture[0].Get(),			//�e�N�X�`���̏�� 
	//	DirectX::SimpleMath::Vector2(m_screenPos.x, m_screenPos.y + m_m_count),//�ʒu
	//	&rect1,					//�摜�̐؂���(AABB)
	//	Colors::White,				//�F
	//	XMConvertToRadians(0),		//�p�x(���W�A��)�A�p�x�����W�A���ɂ���֐�
	//	m_origin,					//��]���S�_
	//	1.0f);						//�傫���̔{��(�w�肵�Ȃ��ƃf�t�H���g��1.0f)
	//m_spriteBatch->Draw(
	//	m_HpGreentexture[0].Get(),			//�e�N�X�`���̏�� 
	//	DirectX::SimpleMath::Vector2(m_screenPos.x, m_screenPos.y + m_count),//�ʒu
	//	&rect,					//�摜�̐؂���(AABB)
	//	Colors::White,				//�F
	//	XMConvertToRadians(0),		//�p�x(���W�A��)�A�p�x�����W�A���ɂ���֐�
	//	m_origin,					//��]���S�_
	//	1.0f);						//�傫���̔{��(�w�肵�Ȃ��ƃf�t�H���g��1.0f)

	//if (m_boss)
	//{
	//	RECT rect2{ 0,0,200 - m_bosshp,50 };
	//	RECT rect3{ 0,0,200 - m_bosshpOld,50 };

	//									//�X�v���C�g�̕`��

	//	m_spriteBatch->Draw(
	//		m_HpWhitetexture[1].Get(),			//�e�N�X�`���̏�� 
	//		DirectX::SimpleMath::Vector2(300, 20),//�ʒu
	//		nullptr,					//�摜�̐؂���(AABB)
	//		Colors::White,				//�F
	//		XMConvertToRadians(0),		//�p�x(���W�A��)�A�p�x�����W�A���ɂ���֐�
	//		m_origin,					//��]���S�_
	//		1.0f);						//�傫���̔{��(�w�肵�Ȃ��ƃf�t�H���g��1.0f)

	//	m_spriteBatch->Draw(
	//		m_HpRedtexture[1].Get(),			//�e�N�X�`���̏�� 
	//		DirectX::SimpleMath::Vector2(300, 20),//�ʒu
	//		&rect3,					//�摜�̐؂���(AABB)
	//		Colors::White,				//�F
	//		XMConvertToRadians(0),		//�p�x(���W�A��)�A�p�x�����W�A���ɂ���֐�
	//		m_origin,					//��]���S�_
	//		1.0f);						//�傫���̔{��(�w�肵�Ȃ��ƃf�t�H���g��1.0f)

	//	m_spriteBatch->Draw(
	//		m_HpGreentexture[1].Get(),			//�e�N�X�`���̏�� 
	//		DirectX::SimpleMath::Vector2(300, 20),//�ʒu
	//		&rect2,					//�摜�̐؂���(AABB)
	//		Colors::White,				//�F
	//		XMConvertToRadians(0),		//�p�x(���W�A��)�A�p�x�����W�A���ɂ���֐�
	//		m_origin,					//��]���S�_
	//		1.0f);						//�傫���̔{��(�w�肵�Ȃ��ƃf�t�H���g��1.0f)

	//}
	//if (m_count >= 150 && !m_clearFlag)
	//{
	//	m_spriteBatch->Draw(
	//		m_overTexture.Get(),			//�e�N�X�`���̏�� 
	//		DirectX::SimpleMath::Vector2(400, 300),//�ʒu
	//		nullptr,					//�摜�̐؂���(AABB)
	//		Colors::White,				//�F
	//		XMConvertToRadians(0),		//�p�x(���W�A��)�A�p�x�����W�A���ɂ���֐�
	//		Vector2(331.f / 2.f,195.f /2.f),					//��]���S�_
	//		1.0f);						//�傫���̔{��(�w�肵�Ȃ��ƃf�t�H���g��1.0f)
	//}
	//if (m_clearFlag)
	//{
	//	m_spriteBatch->Draw(
	//		m_clearTexture.Get(),			//�e�N�X�`���̏�� 
	//		DirectX::SimpleMath::Vector2(400, 300),//�ʒu
	//		nullptr,					//�摜�̐؂���(AABB)
	//		Colors::White,				//�F
	//		XMConvertToRadians(0),		//�p�x(���W�A��)�A�p�x�����W�A���ɂ���֐�
	//		Vector2(400.f / 2.f, 74.f / 2.f),					//��]���S�_
	//		1.0f);						//�傫���̔{��(�w�肵�Ȃ��ƃf�t�H���g��1.0f)
	//}
#pragma endregion comment 
	D3DXMATRIX World;
	D3DXMATRIX View;
	D3DXMATRIX Proj;
	////���[���h�g�����X�t�H�[��
	//static float x = 0;
	//x += 0.00001;
	//D3DXMATRIX Tran;
	//D3DXMatrixTranslation(&Tran,x,0,0);
	//World = Tran;
	//���[���h�g�����X�t�H�[���i��΍��W�ϊ��j
	D3DXMatrixRotationY(&World, timeGetTime() / 110.0f);//�P����yaw��]������
	//�r���[�g�����X�t�H�[��
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -2.0f);	//���_�ʒu
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);	//����ʒu
	D3DXMatrixLookAtRH(&View, &vEyePt, &vLookatPt, &vUpVec);
	//�v���W�F�N�V�����g�����X�t�H�[��
	D3DXMatrixPerspectiveFovRH(&Proj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);
	//�g�p����V�F�[�_�[�̃Z�b�g
	m_d3dContext->VSSetShader(m_vertexShader, NULL, 0);
	m_d3dContext->GSSetShader(NULL, NULL, 0);
	m_d3dContext->PSSetShader(m_pixelShader, NULL, 0);
	//shader�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pDate;
	SIMPLESHADER_CONSTANT_BUFFER cb;
	if (SUCCEEDED(m_d3dContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pDate)))
	{
		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = World * View * Proj;
		D3DXMatrixTranspose(&m, &m);
		cb.mWVP = m;
		//�J���[��ω�������
		static D3DXVECTOR4 vColor(1,1,0,1);
		vColor.x-=0.01f;
		vColor.y+=0.01f;
		if(vColor.x<=0)
		{
			vColor.x=1;
			vColor.y=0;
		}
		cb.vColor=vColor;
		memcpy_s(pDate.pData, pDate.RowPitch, (void*)(&cb), sizeof(cb));
		m_d3dContext->Unmap(m_constantBuffer, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	m_d3dContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_d3dContext->PSSetConstantBuffers(0, 1, &m_constantBuffer);
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_d3dContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//m_d3dContext->OMSetDepthStencilState(m_states->DepthNone(), 0);
	//m_d3dContext->OMSetBlendState(m_states->Opaque(), NULL, 0xffffffff);
	m_d3dContext->RSSetState(m_states->CullNone());
	//�v���~�e�B�u�������_�����O
	m_d3dContext->Draw(4, 0);
	
	//m_debugText->Draw();
	//m_spriteBatch->End();

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
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

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
//! @brief ���`���
//!
//! @param[in] �ŏ��̏ꏊ�A�ڕW�̏ꏊ�A����(�O�`�P)
//!
//! @return ���̏ꏊ
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

//hlsl�t�@�C����ǂݍ��݃V�F�[�_�[���쐬����
HRESULT Game::MakeShader(LPSTR szFileName, LPSTR szFuncName, LPSTR szProfileName, void** ppShader, ID3DBlob** ppBlob)
{
	ID3DBlob *pErrors = NULL;
	if (FAILED(D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
	{
		char*p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	char szProfile[3] = { 0 };
	memcpy(szProfile, szProfileName, 2);
	if (strcmp(szProfile, "vs") == 0)//Vertex Shader
	{
		if (FAILED(m_d3dDevice->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "ps") == 0)//Pixel Shader
	{
		if (FAILED(m_d3dDevice->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "gs") == 0)//Geometry Shader
	{
		if (FAILED(m_d3dDevice->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "hs") == 0)//Hull Shader
	{
		if (FAILED(m_d3dDevice->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "ds") == 0)//Domain Shader
	{
		if (FAILED(m_d3dDevice->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader))) return E_FAIL;
	}
	if (strcmp(szProfile, "cs") == 0)//Compute Shader
	{
		if (FAILED(m_d3dDevice->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader))) return E_FAIL;
	}
	return S_OK;
}

//�|���S���A���b�V���Ȃǂ̃W�I���g���֘A��������
HRESULT Game::InitModel()
{
	//�o�[�e�b�N�X�o�b�t�@�[�쐬
	SimpleVertex vertices[]=
	{
		D3DXVECTOR3(-0.2f, 0.7f, 0.0f),
		D3DXVECTOR3(-0.5f,0.5f,0.0f),
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 1;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_d3dDevice->CreateBuffer(&bd, &InitData, &m_vertexBuffer)))
	{
		return E_FAIL;
	}
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_d3dContext->IAGetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	return S_OK;
}

HRESULT Game::InitPolygon()
{
	//���_���`
	SimpleVertex vertices[] =
	{
		D3DXVECTOR3(-0.5,-0.5,0),//���_1	
		D3DXVECTOR3(-0.5,0.5,0), //���_2
		D3DXVECTOR3(0.5,-0.5,0),  //���_3
		D3DXVECTOR3(0.5,0.5,0), //���_4	
	};
	//��̒��_�Ńo�[�e�b�N�X�o�b�t�@�[�쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(m_d3dDevice->CreateBuffer(&bd, &InitData, &m_vertexBuffer)))
	{
		return E_FAIL;
	}
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	m_d3dContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	return S_OK;
}

HRESULT Game::InitShader()
{
	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"Point.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(m_d3dDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_vertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(m_d3dDevice->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_inputLayout)))
	{
		return FALSE;
	}
	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile(L"Point.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, L"hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(m_d3dDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, L"�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(D3DMATRIX);
	cb.ByteWidth = sizeof(SIMPLESHADER_CONSTANT_BUFFER);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(m_d3dDevice->CreateBuffer(&cb, NULL, &m_constantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}



