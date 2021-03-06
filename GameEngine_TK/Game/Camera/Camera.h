//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Camera.h
//!
//! @brief  カメラクラスのヘッダーファイル
//!
//! @date   2017/05/11
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>

class Camera
{
public:
	//コンストラクタ
	Camera(int width, int height);
	//デストラクタ
	virtual ~Camera();
	//更新
	virtual void Update();
	//setter
	//視点座標
	void SetEyePos(const DirectX::SimpleMath::Vector3& eyepos);
	//目標座標
	void SetRefPos(const DirectX::SimpleMath::Vector3& refpos);
	//カメラの上方向ベクトル
	void SetUpVec(const DirectX::SimpleMath::Vector3& upvec);
	//垂直方向視野角
	void SetFloV(float floV);
	//アスペクト比
	void SetAspect(float aspect);
	//ニアクリップ
	void SetNearClip(float nearclip);
	//ファークリップ
	void SetFarClip(float farclip);

	//getter
	//ビュー行列の取得
	const DirectX::SimpleMath::Matrix& Camera::GetViewMatrix() const;
	//射影行列の取得
	const DirectX::SimpleMath::Matrix& Camera::GetProjectionMatrix() const;
	//ビルボード行列
	const DirectX::SimpleMath::Matrix& GetBillboard() const;
	//Y軸回りのビルボード行列
	const DirectX::SimpleMath::Matrix& GetBillboardAxisY() const;
	//ビルボード行列の計算
	void Billboard();
protected:
	//カメラの位置
	DirectX::SimpleMath::Vector3 m_eyepos;
	//カメラの見ている先
	DirectX::SimpleMath::Vector3 m_refpos;
	//カメラの上方向ベクトル
	DirectX::SimpleMath::Vector3 m_upvec;
	//ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	//垂直方向視野角
	float m_floV;
	//アスペクト比
	float m_aspect;
	//ニアクリップ
	float m_nearclip;
	//ファークリップ
	float m_farclip;
	//射影行列
	DirectX::SimpleMath::Matrix m_proj;
	//ビルボード行列
	DirectX::SimpleMath::Matrix m_billboard;
	//Y軸回りのビルボード行列
	DirectX::SimpleMath::Matrix m_billboardAxisY;
};
