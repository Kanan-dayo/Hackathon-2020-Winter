//*************************************************************************************************************
//
// ポリゴン処理[Polygon.cpp]
// Author:Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
int CScene::m_nNumAll = 0;
CScene *CScene::m_pTop[PRIORITY::PRIORITY_MAX] = {};		// 先頭へのオブジェクトポインタ
CScene *CScene::m_pCur[PRIORITY::PRIORITY_MAX] = {};		// 現在(最後尾)オブジェクトへのポインタ


//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CScene::CScene(PRIORITY order)
{
	this->m_nPriority = order;		//描画優先順位の番号
	m_priority = order;

	CScene *pScene = m_pTop[m_nPriority];

	if (m_pTop[m_nPriority] == nullptr&&
		m_pCur[m_nPriority] == nullptr)
	{
		m_pTop[m_nPriority] = this;
	}

	m_pNext = nullptr;

	m_pPrev = m_pCur[m_nPriority];

	if (m_pPrev != nullptr)
	{
		m_pPrev->m_pNext = this;
	}

	m_pCur[m_nPriority] = this;
	m_nNumAll++;

}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CScene::~CScene()
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------------------------------------
void CScene::UpdataAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY::PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{
			CScene * pScene = m_pTop[nCntPriority];
			while (pScene != nullptr)
			{
				CScene * pSceneNext = pScene->m_pNext;
				pScene->Update();
				pScene = pSceneNext;
			}
		}
	}

	for (int nCntPriority = 0; nCntPriority < PRIORITY::PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{
			CScene * pScene = m_pTop[nCntPriority];
			while (pScene != nullptr)
			{
				CScene * pSceneNext = pScene->m_pNext;
				if (pScene->m_bDie)
				{
					pScene->ListRelease();
				}
				pScene = pSceneNext;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------------------------------------------------
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY::PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{
			CScene * pScene = m_pTop[nCntPriority];
			while (pScene != nullptr)
			{
				CScene * pSceneNext = pScene->m_pNext;
				pScene->Draw();
				pScene = pSceneNext;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 開放処理
//-------------------------------------------------------------------------------------------------------------
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY::PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{
			CScene * pScene = m_pTop[nCntPriority];
			while (pScene != nullptr)
			{
				CScene * pSceneNext = pScene->m_pNext;
				pScene->ListRelease();
				pScene = pSceneNext;
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// プレイヤーのシーンを返す
//-------------------------------------------------------------------------------------------------------------
CScene * CScene::GetPlayerScene(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY::PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{
			CScene * pScene = m_pTop[nCntPriority];
			while (pScene != nullptr)
			{
				CScene * pSceneNext = pScene->m_pNext;
				if (pScene->GetObjectTyoe() == TYPE::TYPE_PLAYER)
				{
					return pScene;
				}
				pScene = pSceneNext;
			}
		}
	}
	return nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// シーンの取得
//-------------------------------------------------------------------------------------------------------------
CScene * CScene::GetScene(TYPE type)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY::PRIORITY_MAX; nCntPriority++)
	{
		if (m_pTop[nCntPriority] != nullptr)
		{
			CScene * pScene = m_pTop[nCntPriority];
			while (pScene != nullptr)
			{
				CScene * pSceneNext = pScene->m_pNext;
				if (pScene->GetObjectTyoe() == type)
				{
					return pScene;
				}
				pScene = pSceneNext;
			}
		}
	}
	return nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// 先頭のポインタの取得
//-------------------------------------------------------------------------------------------------------------
CScene * CScene::GetTop(int nPriority)
{
	return m_pTop[nPriority];
}

//-------------------------------------------------------------------------------------------------------------
// 次のポインタ
//-------------------------------------------------------------------------------------------------------------
CScene * CScene::GetNext(void)
{
	return m_pNext;
}

//-------------------------------------------------------------------------------------------------------------
// 開放処理
//-------------------------------------------------------------------------------------------------------------
void CScene::Release(void)
{
	if (m_bDie == false)
	{
		m_bDie = true;
	}
}

//-------------------------------------------------------------------------------------------------------------
// リストからの開放処理
//-------------------------------------------------------------------------------------------------------------
void CScene::ListRelease(void)
{
	// 終了処理
	this->Uninit();
	if (this == m_pCur[this->m_nPriority] && this == m_pTop[this->m_nPriority])
	{	//最後の一つ
		m_pCur[this->m_nPriority] = nullptr;
		m_pTop[this->m_nPriority] = nullptr;
	}
	else if (this == m_pTop[this->m_nPriority])
	{	//Topの時
		m_pTop[m_nPriority] = this->m_pNext;
		this->m_pNext->m_pPrev = nullptr;
	}
	else if (this == m_pCur[this->m_nPriority])
	{	//Curの時
		m_pCur[m_nPriority] = this->m_pPrev;
		this->m_pPrev->m_pNext = nullptr;
	}
	else
	{	//中間
		this->m_pNext->m_pPrev = this->m_pPrev;
		this->m_pPrev->m_pNext = this->m_pNext;
	}
	//フラグがったったら消す
	delete this;
	m_nNumAll--;
}
