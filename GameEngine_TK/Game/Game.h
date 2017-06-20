//
// Game.h
//

#pragma once

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

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:
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
	Obj3d m_objSkydome;
	//�n�ʃ��f��
	Obj3d m_modelGround;
	//���`���
	static DirectX::SimpleMath::Vector3 Lerp(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 targetPos, float t);
	//�v���C���[
	Character* m_player;
	//�G
	std::vector<Character*> m_enemy;
	//�J����
	std::unique_ptr<FollowCamera> m_camera;
	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//������
	std::unique_ptr<DebugText> m_debugText;
};