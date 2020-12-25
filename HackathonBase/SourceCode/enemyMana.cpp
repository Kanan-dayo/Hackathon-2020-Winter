//*************************************************************************************************************
//
// 敵のマネージャー処理[EnemyMana.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "enemyMana.h"
#include "enemy.h"
#include "player.h"
#include "manager.h"
#include "game.h"
#include "Scene2D.h"
#include "sound.h"

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CEnemyMana::CEnemyMana()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CEnemyMana::~CEnemyMana()
{
}

//-------------------------------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------------------------------
CEnemyMana * CEnemyMana::Create(void)
{
	CEnemyMana * pEnemyMana = new CEnemyMana;
	pEnemyMana->Init();
	return pEnemyMana;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CEnemyMana::Init(void)
{
	m_pStudent.clear();
	m_pCouple.clear();
	m_pNewLywed.clear();

	m_pStudent.resize(ENEMYMANA_STUDENT_MAX);
	m_pCouple.resize(ENEMYMANA_COUPLE_MAX);
	m_pNewLywed.resize(ENEMYMANA_NEWLYWED_MAX);

	D3DXVECTOR3 InitPos;
	InitPos.z = 0.0f;

	for (int nCnt = 0; nCnt < (int)m_pStudent.size(); ++nCnt)
	{
		InitPos.x = rand() % 1000 + 140.0f;
		InitPos.y = rand() % 480 + 140.0f;
		m_pStudent[nCnt] = CEnemy::Create(InitPos, CEnemy::TYPE::TYPE_STUDENT);
	}

	for (int nCnt = 0; nCnt < (int)m_pCouple.size(); ++nCnt)
	{
		InitPos.x = rand() % 1000 + 140.0f;
		InitPos.y = rand() % 480 + 140.0f;
		m_pCouple[nCnt] = CEnemy::Create(InitPos, CEnemy::TYPE::TYPE_COUPLE);
	}

	for (int nCnt = 0; nCnt < (int)m_pNewLywed.size(); ++nCnt)
	{
		InitPos.x = rand() % 1000 + 140.0f;
		InitPos.y = rand() % 480 + 140.0f;
		m_pNewLywed[nCnt] = CEnemy::Create(InitPos, CEnemy::TYPE::TYPE_NEWLYWED);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CEnemyMana::Uninit(void)
{
	m_pStudent.clear();
	m_pCouple.clear();
	m_pNewLywed.clear();
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CEnemyMana::Update(void)
{
	StudentAppearance();
	// 衝突判定
	Collision();
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CEnemyMana::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 衝突
//-------------------------------------------------------------------------------------------------------------
void CEnemyMana::Collision(void)
{
	if (CManager::GetMode() != CManager::MODE_GAME)
	{
		return;
	}
	CGame *pGame  = (CGame*)CManager::GetModeClassPtr();
	if (pGame == nullptr)
	{
		return;
	}
	CPlayer*pPlaye = pGame->GetPlayer();
	if (pPlaye == nullptr)
	{
		return;
	}
	D3DXVECTOR3 *PlayerPos = pPlaye->GetImage(CPlayer::IMG_BODY)->GetPosition();

	for (int nCnt = 0; nCnt < (int)m_pStudent.size(); ++nCnt)
	{
		if (m_pStudent[nCnt] == nullptr)
		{
			continue;
		}
		D3DXVECTOR3 *EnemyPos = m_pStudent[nCnt]->GetImage(CEnemy::IMG_HEART)->GetPosition();
		VEC3 vec = *EnemyPos - *PlayerPos;
		if (vec.LengthSq() < 50.0f * 50.0f)
		{
			if (m_pStudent[nCnt]->GetState() == CEnemy::STATE_NORMAL)
			{
				float fRadian = atan2f(vec.x, vec.y);
				if (pPlaye->GetState() == CPlayer::STATE_NORMAL || pPlaye->GetState() == CPlayer::STATE_CHARGE)
				{
					EnemyPos->x = PlayerPos->x + sinf(fRadian) * 50.0f;
					EnemyPos->y = PlayerPos->y + cosf(fRadian) * 50.0f;
					m_pStudent[nCnt]->GetImage(CEnemy::IMG_HEART)->SetPosflag();
				}
				else if (pPlaye->GetState() == CPlayer::STATE_SMASH)
				{
					//
					if (m_pStudent[nCnt]->DamageProc(vec, PlayerPos))
					{
						CGame::AddNumKill(1);
					}
				}
			}
		}
	}

	for (int nCnt = 0; nCnt < (int)m_pCouple.size(); ++nCnt)
	{
		if (m_pCouple[nCnt] == nullptr)
		{
			continue;
		}
		D3DXVECTOR3 *EnemyPos = m_pCouple[nCnt]->GetImage(CEnemy::IMG_HEART)->GetPosition();
		VEC3 vec = *EnemyPos - *PlayerPos;
		if (vec.LengthSq() < 50.0f * 50.0f)
		{
			if (m_pCouple[nCnt]->GetState() == CEnemy::STATE_NORMAL)
			{
				float fRadian = atan2f(vec.x, vec.y);
				if (pPlaye->GetState() == CPlayer::STATE_NORMAL || pPlaye->GetState() == CPlayer::STATE_CHARGE)
				{
					EnemyPos->x = PlayerPos->x + sinf(fRadian) * 50.0f;
					EnemyPos->y = PlayerPos->y + cosf(fRadian) * 50.0f;
					m_pCouple[nCnt]->GetImage(CEnemy::IMG_HEART)->SetPosflag();
				}
				else if (pPlaye->GetState() == CPlayer::STATE_SMASH)
				{
					VEC3 move;
					move.z = 0.0f;
					move.x = sinf(fRadian) * 50.0f;
					move.y = cosf(fRadian) * 50.0f;
					if (m_pCouple[nCnt]->DamageProc(move, PlayerPos))
					{//
						CGame::AddNumKill(1);
						CGame::AddTime(2);
					}
				}
			}
		}

	}
	for (int nCnt = 0; nCnt < (int)m_pNewLywed.size(); ++nCnt)
	{
		if (m_pNewLywed[nCnt] == nullptr)
		{
			continue;
		}
		D3DXVECTOR3 *EnemyPos = m_pNewLywed[nCnt]->GetImage(CEnemy::IMG_HEART)->GetPosition();
		VEC3 vec = *EnemyPos - *PlayerPos;
		if (vec.LengthSq() < 50.0f * 50.0f)
		{
			if (m_pNewLywed[nCnt]->GetState() == CEnemy::STATE_NORMAL)
			{
				float fRadian = atan2f(vec.x, vec.y);
				if (pPlaye->GetState() == CPlayer::STATE_NORMAL || pPlaye->GetState() == CPlayer::STATE_CHARGE)
				{
					EnemyPos->x = PlayerPos->x + sinf(fRadian) * 50.0f;
					EnemyPos->y = PlayerPos->y + cosf(fRadian) * 50.0f;
					m_pNewLywed[nCnt]->GetImage(CEnemy::IMG_HEART)->SetPosflag();
				}
				else if (pPlaye->GetState() == CPlayer::STATE_SMASH)
				{//
					VEC3 move;
					move.z = 0.0f;
					move.x = sinf(fRadian) * 50.0f;
					move.y = cosf(fRadian) * 50.0f;
					if (m_pNewLywed[nCnt]->DamageProc(move, PlayerPos))
					{
						CGame::AddNumKill(1);
						CGame::AddTime(5);
					}
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 学生出現
//-------------------------------------------------------------------------------------------------------------
void CEnemyMana::StudentAppearance(void)
{
	if (CManager::GetMode() != CManager::MODE_GAME)
	{
		return;
	}
	CGame *pGame = (CGame*)CManager::GetModeClassPtr();
	if (pGame == nullptr)
	{
		return;
	}

	D3DXVECTOR3 InitPos;
	InitPos.z = 0.0f;

	for (int nCnt = 0; nCnt < (int)m_pStudent.size(); ++nCnt)
	{
		if (m_pStudent[nCnt]->GetState() == CEnemy::STATE_STANDBY)
		{
			InitPos.x = rand() % 1000 + 140.0f;
			InitPos.y = rand() % 480 + 140.0f;
			m_pStudent[nCnt]->SetAppearance(InitPos);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 新婚出現
//-------------------------------------------------------------------------------------------------------------
void CEnemyMana::NewlywedAppearance(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 夫婦出現
//-------------------------------------------------------------------------------------------------------------
void CEnemyMana::CoupleAppearance(void)
{
}
