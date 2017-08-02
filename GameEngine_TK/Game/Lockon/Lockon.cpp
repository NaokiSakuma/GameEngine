//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Lockon.h
//!
//! @brief  ロックオンのヘッダファイル
//!
//! @date   2017/07/13
//!
//! @author N.Sakuma
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

#include "Lockon.h"
#include "..\State\PlayerState\PlayerState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

//----------------------------------------------------------------------
//! @brief コンストラクタ
//!
//! @param[in] なし
//----------------------------------------------------------------------
Lockon::Lockon()
	:m_player(nullptr)
	,m_enemy(nullptr)
	,m_lockOnSerial(0)
	,m_isLockOn(false)
{
}

//----------------------------------------------------------------------
//! @brief デストラクタ
//----------------------------------------------------------------------
Lockon::~Lockon()
{
	delete m_player;
	m_player = nullptr;
}

//----------------------------------------------------------------------
//! @brief ロックオン
//!
//! @param[in] なｓ
//!
//! @return ロックオンしているか
//----------------------------------------------------------------------
bool Lockon::ToLockon()
{
	//自機がセットされていない場合
	if (!m_player) return false;
	//敵がセットされていない場合
	if (!m_enemy) return false;
	//敵が全滅
	if (m_enemy->size() == 0) return false;
	//一時的な情報
	struct Temporary
	{
		//距離
		float distance;
		//配列の中での順番
		int index;
		//角度
		float delta;
	};
	//自機の座標
	const Vector3& playerPos = m_player->GetTrans();

	Temporary tmp;
	//十分に大きい距離
	tmp.distance = 1.0e5f;
	tmp.index = -1;
	tmp.delta = 180;
	//すべての敵について
	int enemyNum = (int)m_enemy->size();
	for (int i = 0; i < enemyNum; i++)
	{
		{
			////プレイヤーの向き
			//float playerRotY = m_player->GetRot().y;
			////頂点の設定
			//const Matrix& rotmat = Matrix::CreateRotationY(playerRotY);
			////向いているベクトル
			//Vector3 faceVec(0, 0, -50.f);
			////方向ベクトル
			//faceVec = Vector3::TransformNormal(faceVec, rotmat);
			//Enemy* enemy = (*m_enemy)[i].get();
			//const Vector3& enemyPos = enemy->GetTrans();
			////自機と敵の距離
			//float distance = Vector3::Distance(faceVec, enemyPos);
			////距離が一番近い
			//if (distance < tmp.distance)
			//{
			//	tmp.distance = distance;
			//	tmp.index = i;
			//}

			//Enemy* enemy = (*m_enemy)[i].get();
			////敵とプレイヤーとの角度
			//float player2enemyAngle = atan2f(playerPos.x - enemy->GetTrans().x, playerPos.z - enemy->GetTrans().z);
			////自分の向き
			//float playerRotY = m_player->GetRot().y;
			//if(player2enemyAngle - playerRotY)

			Enemy* enemy = (*m_enemy)[i].get();
			Vector3 enemyPos = enemy->GetTrans();
			//自分の向きのベクトル
			float angle = m_player->GetRot().y;
			Matrix rotmatY = Matrix::CreateRotationY(angle);
			Vector3 moveV(0, 0, -0.1f);
			moveV = Vector3::TransformNormal(moveV, rotmatY);
			moveV.Normalize();
			//敵とプレイヤーとの距離のベクトル
			Vector3 player2enemyDistance = enemy->GetTrans() - m_player->GetTrans();
			player2enemyDistance.Normalize();
			//内積
			float cosine = moveV.Dot(player2enemyDistance);
			//角度
			float delta = acosf(cosine);
			if (delta < XMConvertToRadians(20))
			{
				float distance = Vector3::Distance(playerPos, enemyPos);
				if (distance < 50.f)
				{
					if (delta < tmp.delta)
					{
						tmp.delta = delta;
						if (distance < tmp.distance)
						{
							tmp.distance = distance;
							tmp.index = i;
						}
					}
				}
			}
		}
	}
	//配列の要素外アクセスを回避
	if (!(0 <= tmp.index) || !(tmp.index < enemyNum))
		return false;

	//一番近い敵をロックオン
	m_lockOnSerial = (*m_enemy)[tmp.index]->GetSerialNum();
	m_isLockOn = true;
	return true;
}

//----------------------------------------------------------------------
//! @brief ロックオンを解除する
//!
//! @param[in] なし
//!
//! @return なし
//----------------------------------------------------------------------
void Lockon::ReleaseLockon()
{
	m_isLockOn = false;
	m_lockOnSerial = 0;
}

//----------------------------------------------------------------------
//! @brief 敵を探す
//!
//! @param[in] なし
//!
//! @return 敵
//----------------------------------------------------------------------
Enemy * Lockon::SearchEnemy()
{
	Enemy* lockingEnemy = nullptr;
	//ロックオン中かどうか
	if (IsLockOn())
	{
		//ロックオン中の敵が存在しているか確認
		int enemyNum = (int)m_enemy->size();
		for (int i = 0; i < enemyNum; i++)
		{
			Enemy* enemy = (*m_enemy)[i].get();
			//発見したかどうか
			if (enemy->GetSerialNum() == m_lockOnSerial)
			{
				lockingEnemy = enemy;
				break;
			}
		}
	}
	if (!lockingEnemy)
		ReleaseLockon();
	return lockingEnemy;
}

//----------------------------------------------------------------------
//! @brief プレイヤーのsetter
//!
//! @param[in] プレイヤー
//!
//! @return なし
//----------------------------------------------------------------------
void Lockon::SetPlayer(Player * player)
{
	m_player = player;
}

//----------------------------------------------------------------------
//! @brief 敵のsetter
//!
//! @param[in] 敵
//!
//! @return なし
//----------------------------------------------------------------------
void Lockon::SetEnemy(std::vector<std::unique_ptr<Enemy>>* enemy)
{
	m_enemy = enemy;
}

//----------------------------------------------------------------------
//! @brief プレイヤーのgetter
//!
//! @param[in] なし
//!
//! @return プレイヤー
//----------------------------------------------------------------------
Player * Lockon::GetPlayer()
{
	return m_player;
}

//----------------------------------------------------------------------
//! @brief ロックオンしているか
//!
//! @param[in] なし
//!
//! @return ロックオンしているか
//----------------------------------------------------------------------
bool Lockon::IsLockOn()
{
	return m_isLockOn;
}
