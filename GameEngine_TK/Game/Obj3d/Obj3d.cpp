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
//#include <d3d9types.h>
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
//���f���R���e�i
std::map<std::wstring, std::unique_ptr<DirectX::Model>> Obj3d::m_modelarray;
//�u�����h�X�e�C�g
ID3D11BlendState* Obj3d::m_BlendStateSubtract;
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
//! @brief �R���X�g���N�^
//!
//! @param[in] �Ȃ�
//----------------------------------------------------------------------
Obj3d::Obj3d()
	: m_ObjParent(nullptr)
	, m_scale(1, 1, 1)
	, m_UseQuaternion(false)
	, m_model(nullptr)
{
}

//----------------------------------------------------------------------
//! @brief �R�s�[�R���X�g���N�^
//!
//! @param[in] �I�u�W�F�N�g
//----------------------------------------------------------------------
//Obj3d::Obj3d(const Obj3d & obj)
//{
//	this->m_model = std::move(obj.m_model);
//
//
//}

//----------------------------------------------------------------------
//! @brief CMO���f���̃��[�h
//!
//! @param[in] �t�@�C���̖��O
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::LoadModel(const wchar_t * fileName)
{
	//�G�t�F�N�g�t�@�N�g���[�������Ă��Ȃ��ꍇ���Ƃ�
	assert(m_factory);
	//�������O�̃��f����ǂݍ��ݍς݂łȂ��Ȃ�hit
	if (m_modelarray.count(fileName) == 0)
		m_modelarray[fileName] = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
	m_model = m_modelarray[fileName].get();
	//m_model = Model::CreateFromCMO(m_d3dDevice.Get(), fileName, *m_factory);
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
	Matrix rotmat;
	if (m_UseQuaternion)
	{	//�N�H�[�^�j�I���ŉ�]���v�Z
		rotmat = Matrix::CreateFromQuaternion(m_rotationQ);
	}
	else {	//�I�C���[�p�ŉ�]���v�Z
		Matrix rotmatZ = Matrix::CreateRotationZ(m_rotation.z);	//z���̉�]
		Matrix rotmatX = Matrix::CreateRotationX(m_rotation.x);	//x���̉�]
		Matrix rotmatY = Matrix::CreateRotationY(m_rotation.y);	//y���̉�]
		rotmat = rotmatZ * rotmatX * rotmatY;		    //��]
	}
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
void Obj3d::Render()
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
		//���f�����̑S���b�V������
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
				//���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect> ieff = meshpart->effect;
				BasicEffect* eff = dynamic_cast<BasicEffect*>(ieff.get());
				if (eff)
				{
					//���Ȕ������ő�l��
					eff->SetEmissiveColor(Vector3(1, 1, 1));
					//�G�t�F�N�g�Ɋ܂ނ��ׂĂ̕��s�������ɂ��ď�������
					for (int i = 0; i < BasicEffect::MaxDirectionalLights; i++)
					{
						//���C�g�𖳌��ɂ���
						eff->SetLightEnabled(i, false);
					}
				}
			}
		}
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
//! @brief ��]��Setter(xyz)
//!
//! @param[in] ��]
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_rotation = rot;
	m_UseQuaternion = false;
}

//----------------------------------------------------------------------
//! @brief ��]��Setter(�N�H�[�^�j�I��)
//!
//! @param[in] ��]
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::SetRotQ(const DirectX::SimpleMath::Quaternion & quaternion)
{
	m_rotationQ = quaternion;
	m_UseQuaternion = true;
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
//! @brief ���[���h�s���Setter
//!
//! @param[in] ���[���h�s��
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void Obj3d::SetWorld(const DirectX::SimpleMath::Matrix & world)
{
	m_world = world;
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
		//�r���{�[�h�s������[���h�s��ɍ���
		Matrix world = m_camera->GetBillboard()* m_world;
		assert(m_d3dContext);
		assert(m_states);
		//���Z�`��p�̐ݒ�֐���n���ĕ`��
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
		//�r���{�[�h�s������[���h�s��ɍ���
		Matrix world = m_camera->GetBillboardAxisY()*m_world;
		assert(m_d3dContext);
		assert(m_states);
		//���Z�p�̐ݒ�֐���n���ĕ`��
		m_model->Draw(m_d3dContext.Get(), *m_states, world, view, projection);
	}
}

void Obj3d::EnbleAlpha()
{
	if (m_model)
	{
		//���f�����̑S���b�V������
		ModelMesh::Collection::const_iterator it_mesh = m_model->meshes.begin();
		for (; it_mesh != m_model->meshes.end(); it_mesh++)
		{
			ModelMesh* modelmesh = it_mesh->get();
			assert(modelmesh);
			//���b�V�����̑S���b�V���p�[�c����
			std::vector<std::unique_ptr<ModelMeshPart>>::iterator it_meshpart = modelmesh->meshParts.begin();
			for (; it_meshpart != modelmesh->meshParts.end(); it_meshpart++)
			{
				ModelMeshPart* meshpart = it_meshpart->get();
				assert(meshpart);
				//���b�V���p�[�c�ɃZ�b�g���ꂽ�G�t�F�N�g��BasicEffect�Ƃ��Ď擾
				std::shared_ptr<IEffect>& ieff = meshpart->effect;
				meshpart->ModifyEffect(m_d3dDevice.Get(), ieff, true);
			}
		}
	}
}

