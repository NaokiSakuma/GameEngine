//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Obj3d.h
//!
//! @brief  3d�I�u�W�F�N�g�𐶐�����w�b�_�[�t�@�C��
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <map>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>

#include "..\Camera\Camera.h"
class Obj3d
{
	//�ÓI
public:
	//3d�I�u�W�F�N�g��Initialize
	static void InitializeStatic(
		Microsoft::WRL::ComPtr<ID3D11Device>            d3dDevice,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>     d3dContext,
		Camera* camera);
private:
	//�f�o�C�X
	static Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
	//�f�o�C�X�R���e�L�X�g
	static Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
	//�J����
	static const Camera* m_camera;
	//�ėp�I�X�e�[�g�ݒ�
	static std::unique_ptr<DirectX::CommonStates> m_states;
	//�G�t�F�N�g�t�@�N�g���[
	static std::unique_ptr<DirectX::EffectFactory> m_factory;
	//���f���R���e�i
	static std::map<std::wstring, std::unique_ptr<DirectX::Model>> m_modelarray;

public:
	//�R���X�g���N�^
	Obj3d();
	//CMO���f���̃��[�h
	void LoadModel(const wchar_t* fileName);
	//�X�V
	virtual void Update();
	//�`��
	virtual void Render() const;

	//Setter
	//�X�P�[�����O
	void SetScale(const DirectX::SimpleMath::Vector3& scale);
	//��](xyz)
	void SetRot(const DirectX::SimpleMath::Vector3& rot);
	//��](�N�H�[�^�j�I��)
	void SetRotQ(const DirectX::SimpleMath::Quaternion& quaternion);
	//���s�ړ�
	void SetTrans(const DirectX::SimpleMath::Vector3& trans);
	//�e��3d�I�u�W�F�N�g
	void SetObjParent(Obj3d* objParent);
	//Getter
	//�X�P�[�����O
	const DirectX::SimpleMath::Vector3& GetScale() const;
	//��]
	const DirectX::SimpleMath::Vector3& GetRot() const;
	//���s�ړ�
	const DirectX::SimpleMath::Vector3& GetTrans() const;
	//���[���h�s��
	const DirectX::SimpleMath::Matrix& GetWorld() const;
	//�e��3d�I�u�W�F�N�g
	const bool GetObjParent() const;

protected:
	//���f��
	const DirectX::Model* m_model;
	//�X�P�[�����O
	DirectX::SimpleMath::Vector3 m_scale;
	//��](x,y,z / �I�C���[�p)
	DirectX::SimpleMath::Vector3 m_rotation;
	//��](�N�H�[�^�j�I��)
	DirectX::SimpleMath::Quaternion m_rotationQ;
	//���s�ړ�
	DirectX::SimpleMath::Vector3 m_translation;
	//���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	//�e��Obj3d�ւ̃|�C���^
	Obj3d* m_ObjParent;

	//��]���N�H�[�^�j�I���ň����t���O
	bool m_UseQuaternion;

};