//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Enemy.cpp
//!
//! @brief  敵のソースファイル
//!
//! @date   2017/06/08
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Enemy.h"
#include "..\..\State\EnemyState\EnemyState.h"

//名前空間
using namespace DirectX;
using namespace SimpleMath;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Enemy::Enemy()
	:m_timer(0)
	,m_DistAngle(0)
{

}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
Enemy::~Enemy()
{

}

//----------------------------------------------------------------------
//! @brief 初期化
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Enemy::Initialize()
{
	//ステイトを取得(立ち)
	ChangeState(EnemyState::Stand::GetInstance());

	//要素数分にリサイズする
	m_obj3d.resize(ENEMY_PARTS_NUM);
	//自機パーツの読み込み
	m_obj3d[HEAD1].LoadModel(L"Resources/head.cmo");
	m_obj3d[HEAD2].LoadModel(L"Resources/head2.cmo");

	//子パーツの親からのオフセット(座標のズレ)をセット
	m_obj3d[HEAD2].SetTrans(Vector3(0, 0, 10));

	//大きさ
	m_obj3d[HEAD1].SetScale(Vector3(4, 2, 2));
	m_obj3d[HEAD2].SetScale(Vector3(2, 4, 2));
}

//----------------------------------------------------------------------
//! @brief 更新
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Enemy::Update()
{
	m_timer--;
	if (m_timer < 0)
	{
		m_timer = 60;
		//目標角をランダムに抽選
		float rnd = (float)rand() / RAND_MAX - 0.5f;
		//-90～90の乱数
		rnd *= 180.0f;
		rnd = XMConvertToRadians(rnd);
		m_DistAngle += rnd;
	}
	{
		//自機の角度を回転
		Vector3 rot = GetRot();
		//今の角度
		float angle = m_DistAngle - rot.y;
		if (angle > XM_PI)
		{
			angle -= XM_2PI;
		}
		if (angle < -XM_PI)
		{
			angle += XM_2PI;
		}
		//補間
		rot.y += angle * 0.01f;
		SetRot(rot);
	}
	//動作の実行
	m_state->Execute(this);

	//行列の更新
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Update();
	}


}

//----------------------------------------------------------------------
//! @brief 描画
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Enemy::Render()
{
	for (std::vector<Obj3d>::iterator it = m_obj3d.begin(); it != m_obj3d.end(); it++)
	{
		it->Render();
	}
}

//----------------------------------------------------------------------
//! @brief クリエイト関数
//!
//! @param[in] なし
//!
//! @return Playerのポインタ
//----------------------------------------------------------------------
Enemy* Enemy::Create()
{
	Enemy* enemy = new Enemy;
	enemy->Initialize();

	return enemy;
}

//----------------------------------------------------------------------
//! @brief ステイトの変更
//!
//! @param[in] 変更先のステイト
//!
//! @return なし
//----------------------------------------------------------------------
void Enemy::ChangeState(State* state)
{
	m_state = state;
}

//----------------------------------------------------------------------
//! @brief スケーリングのSetter
//!
//! @param[in] スケーリング
//!
//! @return なし
//----------------------------------------------------------------------
void Enemy::SetScale(const DirectX::SimpleMath::Vector3& scale)
{
	m_obj3d[HEAD1].SetScale(scale);
}

//----------------------------------------------------------------------
//! @brief 回転のSetter
//!
//! @param[in] 回転
//!
//! @return なし
//----------------------------------------------------------------------
void Enemy::SetRot(const DirectX::SimpleMath::Vector3& rot)
{
	m_obj3d[HEAD1].SetRot(rot);
}

//----------------------------------------------------------------------
//! @brief 平行移動のSetter
//!
//! @param[in] 平行移動
//!
//! @return なし
//----------------------------------------------------------------------
void Enemy::SetTrans(const DirectX::SimpleMath::Vector3& trans)
{
	m_obj3d[HEAD1].SetTrans(trans);
}

//----------------------------------------------------------------------
//! @brief スケーリングのGetter
//!
//! @param[in] なし
//!
//! @return スケーリング
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Enemy::GetScale() const
{
	return m_obj3d[HEAD1].GetScale();
}

//----------------------------------------------------------------------
//! @brief 回転のGetter
//!
//! @param[in] なし
//!
//! @return 回転
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Enemy::GetRot() const
{
	return m_obj3d[HEAD1].GetRot();
}

//----------------------------------------------------------------------
//! @brief 平行移動のGetter
//!
//! @param[in] なし
//!
//! @return 平行移動
//----------------------------------------------------------------------
const DirectX::SimpleMath::Vector3& Enemy::GetTrans() const
{
	return m_obj3d[HEAD1].GetTrans();
}

//----------------------------------------------------------------------
//! @brief ワールド行列のGetter
//!
//! @param[in] なし
//!
//! @return ワールド行列
//----------------------------------------------------------------------
const DirectX::SimpleMath::Matrix& Enemy::GetWorld() const
{
	return m_obj3d[HEAD1].GetWorld();
}

//----------------------------------------------------------------------
//! @brief オブジェクトのGetter
//!
//! @param[in] オブジェクトの要素番号
//!
//! @return オブジェクトのポインタ
//----------------------------------------------------------------------
Obj3d* Enemy::GetObj3d(int num)
{
	return &m_obj3d[num];
}
