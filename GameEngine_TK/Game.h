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
		HEAD1,		//��
		HEAD2,		//��
		HEAD3,		//��
		HEAD4,		//��

		PLAYER_PARTS_NUM = 10,	//�p�[�c��
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

	//�v���~�e�B�u�o�b�`
	//std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;	//�O�p�`�p
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;	//�l�p�`�p
	//�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//CommonState...�`�悷��Ƃ��̔ėp�I�Ȑݒ�//
	//�ėp�I�X�e�[�g�ݒ�
	std::unique_ptr<DirectX::CommonStates> m_states;

	//Matrix�̓f�t�H���g�R���X�g���N�^�ŏ���������Ă���
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	//�v���W�F�N�V�����s��i�ˉe�s��j
	DirectX::SimpleMath::Matrix m_proj;

	//�f�o�b�O�J����
	std::unique_ptr<DebugCamera> m_debugCamera;

	//�G�t�F�N�g�t�@�N�g���[
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//�V�����f�� ���ɂ�����̂��ɕ`�悵�������悢�B
	Obj3d m_objSkydome;
	//�n�ʃ��f��
	std::unique_ptr<DirectX::Model> m_modelGround;
	//�����f��
	std::unique_ptr<DirectX::Model> m_modelBall;
	//�e�B�[�|�b�g���f��
	std::unique_ptr<DirectX::Model> m_modelTeapot;
	////���̃��f��
	//std::unique_ptr<DirectX::Model> m_modelHead;
	////���̃��[���h�s��
	//DirectX::SimpleMath::Matrix m_worldHead;
	////���̃��[���h�s��
	//DirectX::SimpleMath::Matrix m_worldHead2;
	//���̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldBall[20];
	//�e�B�[�|�b�g�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldTeapot[20];
	//�n�ʂ̃��[���h�s��
	DirectX::SimpleMath::Matrix m_worldGround[40000];
	
	//��]�p�̕ϐ�
	int m_count;
	//�p�x�����rand
	float m_randAngle[20];
	//���������rand;
	float m_randDistance[20];
	//�傫��
	float m_val;
	//����
	float m_time;
	//�e�X�g
	DirectX::SimpleMath::Vector3 m_trans;
	//���`���
	static DirectX::SimpleMath::Vector3 Lerp(DirectX::SimpleMath::Vector3 startPos, DirectX::SimpleMath::Vector3 targetPos, float t);

	//�L�[�{�[�h
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	//���̍��W
	DirectX::SimpleMath::Vector3 m_head_pos;
	//��]�p
	int m_rot;
	
	//�J����
	std::unique_ptr<FollowCamera> m_camera;
	//��
	std::vector<Obj3d> m_ObjPlayer;
	//�T�C���p�̊p�x
	float m_cycle;
	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	//������
	std::unique_ptr<DebugText> m_debugText;

	//�֐�
	void SetW();	//W�������ꂽ�Ƃ�
	void SetS();	//S�������ꂽ�Ƃ�
};