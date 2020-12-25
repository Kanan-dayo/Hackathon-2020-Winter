//*************************************************************************************************************
//
// エネミー処理[enemy.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "enemy.h"
#include "texture.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define ENEMY_SIZE D3DXVECTOR2(60,60)

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CONST D3DXVECTOR3 CEnemy::m_aDiffpos[UI_MAX] = {
	{ -40.0f,0.0,0.0f },
	{ 40.0f,0.0,0.0f },
};

CONST D3DXVECTOR2 CEnemy::m_aInitUiSize[UI_MAX] = {
	{ 50.0f,80.0f },
	{ 50.0f,80.0f },
};

D3DXCOLOR CEnemy::m_LifeCol[3] =
{
	{ 1.0f,1.0f,1.0f,1.0f },
	{ 1.0f,0.7f,0.7f,1.0f },
	{ 1.0f,0.0f,0.0f,1.0f },
};

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CEnemy * CEnemy::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEnemy* pEnemy = new CEnemy;
	pEnemy->InitImage(pos, type);
	pEnemy->Init();
	return pEnemy;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CEnemy::Init(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CEnemy::Uninit(void)
{
	m_pImage.clear();
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CEnemy::Update(void)
{
	switch (m_state)
	{
		MLK_CASE(STATE_NORMAL)NormalProc();
		MLK_CASE(STATE_DIE)DieProc();
		MLK_CASE(STATE_DAMAGE)DamageProc();
	default:break;
	}

	if (m_state != STATE_DIE)
	{
		D3DXVECTOR3* pPos = m_pImage[IMG_HEART]->GetPosition();

		if (pPos->y <= 0.0f)
		{
			pPos->y = 0.0f;
		}
		else if (pPos->y >= 720.0f)
		{
			pPos->y = 720.0f;
		}

		if (pPos->x <= 0.0f)
		{
			pPos->x = 0.0f;
		}
		else if (pPos->x >= 1280.0f)
		{
			pPos->x = 1280.0f;
		}

		m_pImage[IMG_HEART]->SetPosition(*pPos);
		m_pImage[IMG_CHAR1]->SetPosition(*pPos + m_aDiffpos[UI_CHAR1]);
		m_pImage[IMG_CHAR2]->SetPosition(*pPos + m_aDiffpos[UI_CHAR2]);

	}
	// 頂点情報の更新
	for (auto& itr : m_pImage) {
		itr->UpdateVertex();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CEnemy::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 画像の初期化
//-------------------------------------------------------------------------------------------------------------
void CEnemy::InitImage(D3DXVECTOR3 & pos, TYPE& type)
{
	m_type = type;
	m_move = ML_VEC3_UNSET;
	m_fSpeed = 2.0f;
	m_state = STATE_NONE;
	m_nCntState = ML_INT_UNSET;
	m_nCntAction = ML_INT_UNSET;
	m_Scatter[0] =
		m_Scatter[1] = ML_VEC3_UNSET;
	switch (type)
	{
		MLK_CASE(TYPE_COUPLE)m_nLife = 2;
		MLK_CASE(TYPE_NEWLYWED)m_nLife = 3;
		MLK_CASE(TYPE_STUDENT) m_nLife = 1;
	}

	m_pImage.resize(IMG_MAX);

	m_pImage[IMG_HEART] = CScene2D::Create(PRIORITY_CHARACTER, pos, m_LifeCol[m_nLife - 1], ORIGINVERTEXTYPE_ROTCENTER, ENEMY_SIZE);
	m_pImage[IMG_HEART]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_HEART));

	m_pImage[IMG_CHAR1] = CScene2D::Create(PRIORITY_CHARACTER, pos + m_aDiffpos[UI_CHAR1], ML_D3DXCOR_SET, ORIGINVERTEXTYPE_ROTCENTER, m_aInitUiSize[UI_CHAR1]);

	m_pImage[IMG_CHAR2] = CScene2D::Create(PRIORITY_CHARACTER, pos + m_aDiffpos[UI_CHAR2], ML_D3DXCOR_SET, ORIGINVERTEXTYPE_ROTCENTER, m_aInitUiSize[UI_CHAR2]);

	if (type == TYPE_STUDENT)
	{
		m_pImage[IMG_CHAR1]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_STUDENT_0));
		m_pImage[IMG_CHAR2]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_STUDENT_1));
	}
	else if (type == TYPE_COUPLE)
	{
		m_pImage[IMG_CHAR1]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_COUPLE_0));
		m_pImage[IMG_CHAR2]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_COUPLE_1));
	}
	else if (type == TYPE_NEWLYWED)
	{
		m_pImage[IMG_CHAR1]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_NEWLYWED_0));
		m_pImage[IMG_CHAR2]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_NEWLYWED_1));
	}
	SetState(STATE_NORMAL);

}

//-------------------------------------------------------------------------------------------------------------
// 状態の設定
//-------------------------------------------------------------------------------------------------------------
void CEnemy::SetState(CONST STATE state)
{
	if(m_state != state)
	{
		m_state = state;
		m_nCntState = ML_INT_UNSET;

		if (m_state == STATE_STANDBY)
		{
			for (auto &itr : m_pImage)
			{
				itr->SetDisp(false);
			}
		}
		else if (m_state == STATE_NORMAL)
		{
			for (auto &itr : m_pImage)
			{
				itr->SetDisp(true);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// ダメージ処理
//-------------------------------------------------------------------------------------------------------------
bool CEnemy::DamageProc(VEC3& move, D3DXVECTOR3 *pPos)
{
	m_nLife--;

	if (m_nLife <= 0)
	{
		VEC3 vec = *m_pImage[IMG_CHAR1]->GetPosition() - *pPos;
		float fRadian = atan2f(vec.x, vec.y);
		m_Scatter[UI_CHAR1].x = sinf(fRadian) * 100.0f;
		m_Scatter[UI_CHAR1].y = cosf(fRadian) * 100.0f;

		vec = *m_pImage[IMG_CHAR2]->GetPosition() - *pPos;
		fRadian = atan2f(vec.x, vec.y);
		m_Scatter[UI_CHAR2].x = sinf(fRadian) * 100.0f;
		m_Scatter[UI_CHAR2].y = cosf(fRadian) * 100.0f;

		m_pImage[IMG_HEART]->SetDisp(false);

		SetState(STATE_DIE);
		return true;
	}
	else
	{
		m_pImage[IMG_HEART]->SetColor(m_LifeCol[m_nLife - 1]);
		m_move = move;
		SetState(STATE_DAMAGE);
		return false;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 出現の設定
//-------------------------------------------------------------------------------------------------------------
void CEnemy::SetAppearance(D3DXVECTOR3 & pos)
{
	SetState(STATE_NORMAL);
	m_pImage[IMG_HEART]->SetPosition(pos);
	m_move = ML_VEC3_UNSET;
	m_fSpeed = 2.0f;
	m_nCntAction = ML_INT_UNSET;
	m_Scatter[0] =
		m_Scatter[1] = ML_VEC3_UNSET;
	switch (m_type)
	{
		MLK_CASE(TYPE_COUPLE)m_nLife = 2;
		MLK_CASE(TYPE_NEWLYWED)m_nLife = 3;
		MLK_CASE(TYPE_STUDENT) m_nLife = 1;
	}
	m_pImage[IMG_HEART]->SetColor(m_LifeCol[m_nLife - 1]);
}

//-------------------------------------------------------------------------------------------------------------
// 通常処理
//-------------------------------------------------------------------------------------------------------------
void CEnemy::NormalProc(void)
{
	if (m_nCntAction >= 90)
	{
		m_nCntAction = 0;
		int nIDX = rand() % 3;
		int nIDY = rand() % 3;

		m_move.x = 0.0f;
		m_move.y = 0.0f;

		if (nIDX == 0)
		{
			m_move.x = 0.0f;
		}
		if (nIDX == 1)
		{
			m_move.x = 1.0f;
		}
		if (nIDX == 2)
		{
			m_move.x = -1.0f;
		}
		if (nIDY == 0)
		{
			m_move.y = 0.0f;
		}
		if (nIDY == 1)
		{
			m_move.y = 1.0f;
		}
		if (nIDY == 2)
		{
			m_move.y = -1.0f;
		}
	}
	else
	{
		m_nCntAction += 1;
	}

	D3DXVECTOR3* pPos = m_pImage[IMG_HEART]->GetPosition();

	m_move.Norm();
	m_move *= m_fSpeed;

	*pPos += m_move;

	m_pImage[IMG_HEART]->SetPosflag();
	m_pImage[IMG_CHAR1]->SetPosition(*pPos + m_aDiffpos[UI_CHAR1]);
	m_pImage[IMG_CHAR2]->SetPosition(*pPos + m_aDiffpos[UI_CHAR2]);
}

//-------------------------------------------------------------------------------------------------------------
// 死の処理
//-------------------------------------------------------------------------------------------------------------
void CEnemy::DieProc(void)
{
	for (int nCnt = IMG_CHAR1; nCnt < IMG_MAX; nCnt++)
	{
		D3DXVECTOR3* pPos = m_pImage[nCnt]->GetPosition();

		if (pPos->y <= 0.0f)
		{
			m_pImage[nCnt]->SetDisp(false);
			SetState(STATE_STANDBY);
		}
		else if (pPos->y >= 720.0f)
		{
			m_pImage[nCnt]->SetDisp(false);
			SetState(STATE_STANDBY);
		}
		else if (pPos->x <= 0.0f)
		{
			m_pImage[nCnt]->SetDisp(false);
			SetState(STATE_STANDBY);
		}
		else if (pPos->x >= 1280.0f)
		{
			m_pImage[nCnt]->SetDisp(false);
			SetState(STATE_STANDBY);
		}
		*pPos += m_Scatter[nCnt - 1];
		m_pImage[nCnt]->SetPosition(*pPos);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ダメージの処理
//-------------------------------------------------------------------------------------------------------------
void CEnemy::DamageProc(void)
{
	m_nCntState++;
	if (m_nCntState > 20)
	{
		for (auto & itr : m_pImage)
		{
			itr->SetDisp(true);
		}
		SetState(STATE_NORMAL);
	}
	for (auto & itr : m_pImage)
	{
		itr->SetDisp(itr->GetDisp() ^ 1);
	}
}
