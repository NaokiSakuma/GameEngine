//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Game.h
//!
//! @brief  �Q�[���̃w�b�_�t�@�C��
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

//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define WINDOW_WIDTH 640 //�E�B���h�E��
#define WINDOW_HEIGHT 480 //�E�B���h�E����

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

	//�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;	//�l�p�`�p
	//�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//CommonState...�`�悷��Ƃ��̔ėp�I�Ȑݒ�//
	//�ėp�I�X�e�[�g�ݒ�
	std::unique_ptr<DirectX::CommonStates> m_states;

	//Matrix�̓f�t�H���g�R���X�g���N�^�ŏ���������Ă���
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//�v���W�F�N�V�����s��i�ˉe�s��j
	DirectX::SimpleMath::Matrix m_proj;
	//�f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_debugCamera;
	//�V�����f�� ���ɂ�����̂��ɕ`�悵�������悢�B
	//Obj3d m_objSkydome;
	LandShape m_landShapeobjSkydome;
	////�n�ʃ��f��
	//Obj3d m_modelGround;
	LandShape m_landShapeGround;
	//���`���
	static DirectX::SimpleMath::Vector3 Lerp(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 targetPos, float t);

	//���`���
	int Lerp(int startPos, int targetPos, float t);

	//�v���C���[
	Character* m_player;
	//�G
	std::vector<std::unique_ptr<Enemy>> m_enemy;
	//�{�X
	std::unique_ptr<Boss> m_boss;
	//�J����
	std::unique_ptr<FollowCamera> m_camera;
	//���b�N�I���̑Ώ�
	std::vector<Obj3d> m_lockonTarget;
	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//������
	std::unique_ptr<DebugText> m_debugText;
	//���b�N�I��
	std::unique_ptr<Lockon> m_lockon;
	//�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_HpGreentexture[2];
	//�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_HpRedtexture[2];
	//�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_HpWhitetexture[2];
	//�Q�[���N���A�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_clearTexture;
	//�Q�[���I�[�o�[�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_overTexture;
	//�X�v���C�g��\������X�N���[�����W
	DirectX::SimpleMath::Vector2 m_screenPos;
	//�X�v���C�g�̌��_
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
	////Texture�n���h��
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	////�l�p�`�̒��_�f�[�^
	//DirectX::VertexPositionTexture m_vertx[4];
	////�C���f�b�N�X�f�[�^
	//uint16_t m_index[6];
	////�v���~�e�B�u�o�b�`
	//std::unique_ptr < DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;
	////�x�[�V�b�N�G�t�F�N�g
	//std::unique_ptr<DirectX::BasicEffect> m_bassicEffect;
public:
	//�f�o�b�O�\��
	bool isDebug;
	//�Q�[���̃C���X�^���X
	static Game* GetInstance();
	//���b�N�I����getter
	Lockon* GetLockon() { return m_lockon.get(); }
private: 
	//�Q�[���̃C���X�^���X
	static Game* m_instance;

	//�K�g
public:
	HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
	HRESULT InitModel();
	HRESULT InitPolygon();
	HRESULT InitShader();
	//���_�̍\����
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
	};
	//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;
		D3DXVECTOR4 vColor;
	};
};