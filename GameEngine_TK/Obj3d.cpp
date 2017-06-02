//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Obj3d.cpp
//!
//! @brief  3d�I�u�W�F�N�g�𐶐�����\�[�X�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Obj3d.h"

//���O���
using namespace DirectX;
using namespace SimpleMath;

//�ÓI�����o�ϐ��̒�`
//�f�o�C�X
Microsoft::WRL::ComPtr<ID3D11Device>            Obj3d::m_d3dDevice;
//�f�o�C�X�R���e�L�X�g
Microsoft::WRL::ComPtr<ID3D11DeviceContext>     Obj3d::m_d3dContext;
//�J����
const Camera* Obj3d::m_camera;
//�ėp�I�X�e�[�g�ݒ�
std::unique_ptr<DirectX::CommonStates> Obj3d::m_states;
//�G�t�F�N�g�t�@�N�g���[
std::unique_ptr<DirectX::EffectFactory> Obj3d::m_factory;

//----------------------------------------------------------------------
//! @brief 3d�I�u�W�F�N�g��Initialize
//!
//! @param[in] �f�o�C�X�A�R���e�L�X�g�A�J����
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::InitializeStatic(Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice, Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3dContext, Camera * camera)
{
	m_d3dDevice = d3dDevice;
	m_d3dContext = d3dContext;
	m_camera = camera;

	m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());
	//�G�t�F�N�g�t�@�N�g���[����
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//�e�N�X�`���̃p�X���w��(vcxproj���猩�ăt�@�C���̏ꏊ���w��)
	m_factory->SetDirectory(L"Resources");

}

//----------------------------------------------------------------------
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Obj3d::Obj3d()
	:m_ObjParent(nullptr)
	, m_scale(1, 1, 1)
{
}

//----------------------------------------------------------------------
//! @brief CMO���f���̃��[�h
//!
//! @param[in] �t�@�C���̖��O
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::LoadModel(const wchar_t * fileName)
{
	m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
}

//----------------------------------------------------------------------
//! @brief �X�V
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::Update()
{
	//�I�u�W�F�N�g�̃��[���h�s����v�Z
	Matrix scalemat = Matrix::CreateScale(m_scale);				//�X�P�[�����O
	Matrix rotmatZ  = Matrix::CreateRotationZ(m_rotation.z);	//z���̉�]
	Matrix rotmatX  = Matrix::CreateRotationX(m_rotation.x);	//x���̉�]
	Matrix rotmatY  = Matrix::CreateRotationY(m_rotation.y);	//y���̉�]
	Matrix rotmat   = rotmatZ * rotmatX * rotmatY;				//��]
	Matrix transmat = Matrix::CreateTranslation(m_translation);	//���s�ړ�
	m_world = scalemat * rotmat * transmat;						//���[���h�s��
	//�e�I�u�W�F�N�g�����݂���ꍇ
	if(m_ObjParent)
		m_world *= m_ObjParent->GetWorld();	//�e�̍s����|����
}

//----------------------------------------------------------------------
//! @brief �`��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::Render() const
{
	//�I�u�W�F�N�g�����݂��Ă�����
	if (m_model)
	{
		m_model->Draw(						//�`�悷��	
			m_d3dContext.Get(),				//�R���e�L�X�g
			*m_states,						//�X�e�C�g
			m_world,						//���[���h�s��
			m_camera->GetViewMatrix(),		//view�s��
			m_camera->GetProjectionMatrix() //�v���W�F�N�V�����s��
		);
	}
}

//----------------------------------------------------------------------
//! @brief �X�P�[�����O��Setter
//!
//! @param[in] �X�P�[�����O
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_scale = scale;
}

//----------------------------------------------------------------------
//! @brief ��]��Setter
//!
//! @param[in] ��]
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_rotation = rot;
}

//----------------------------------------------------------------------
//! @brief ���s�ړ���Setter
//!
//! @param[in] ���s�ړ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	m_translation = trans;
}

//----------------------------------------------------------------------
//! @brief �e��3d�I�u�W�F�N�g��Setter
//!
//! @param[in] �e��3d�I�u�W�F�N�g
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::SetObjParent(Obj3d * objParent)
{
	m_ObjParent = objParent;
}

//----------------------------------------------------------------------
//! @brief �X�P�[�����O��Getter
//!
//! @param[in] �X�P�[�����O
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Obj3d::GetScale() const
{
	return m_scale;
}

//----------------------------------------------------------------------
//! @brief ��]��Getter
//!
//! @param[in] ��]
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Obj3d::GetRot() const
{
	return m_rotation;
}

//----------------------------------------------------------------------
//! @brief ���s�ړ���Getter
//!
//! @param[in] ���s�ړ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Obj3d::GetTrans() const
{
	return m_translation;
}

//----------------------------------------------------------------------
//! @brief ���[���h�s���Getter
//!
//! @param[in] ���[���h�s��
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
const DirectX::SimpleMath::Matrix& Obj3d::GetWorld() const
{
	return m_world;
}
