//*************************************************************************************************************
//
// ストーリー処理 [story.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "story.h"
#include "texture.h"
#include "2DUI.h"
#include "fade.h"
#include "keyboard.h"
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define ROTATIONSPEED	(0.02f)	// 回転速度
#define MOVESPEED		(15.0f)	// 次ページの遷移スピード
/* 位置情報 */


//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CStory * CStory::Create(void)
{
	CStory *pStory = new CStory;
	pStory->Init();
	return pStory;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CStory::Init(void)
{
	// 初期化処理
	Init_PerfomUi(TYPE::TYPE_MAX, m_apPerfomUi);
	m_nPushButton = -1;

	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;
	seting.nValue = 1000;
	seting.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	seting.size = D3DXVECTOR2(1280.0f, 720.0f);
	seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_FADE;


	// ストーリー3
	seting.nTextureID = CTexture::NAME_RESULT;
	m_apPerfomUi[TYPE::TYPE_STORY_3].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);


	// ストーリー2
	seting.nTextureID = CTexture::NAME_RANKING;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pC2dui = C2DUi::Create(seting,CScene::PRIORITY_BUI);


	// ストーリー1
	seting.nTextureID = CTexture::NAME_TITLE_BG;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CStory::Uninit(void)
{
	for (int nCntUi = 0; nCntUi < TYPE::TYPE_MAX; nCntUi++)
	{
		// 拡大率の開放
		if (m_apPerfomUi[nCntUi].pScal)
		{
			delete m_apPerfomUi[nCntUi].pScal;
			m_apPerfomUi[nCntUi].pScal = NULL;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CStory::Update(void)
{
	// サイズ変更の更新処理
	Update_UiMove(m_nPushButton);
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		// 押した回数アップ
		m_nPushButton++;
		if (m_nPushButton >= TYPE::TYPE_MAX)
		{
			if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
			{
				CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_TUTORIAL);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CStory::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新_UI
//-------------------------------------------------------------------------------------------------------------
void CStory::Update_UiMove(int const & nUi)
{
	if (nUi < 0 || nUi >= TYPE::TYPE_MAX) return;
	else if (!m_apPerfomUi[nUi].pC2dui) return;
	C2DUi * p2Dui = m_apPerfomUi[nUi].pC2dui;
	// 頂点座標の回転処理
	float fRotation = p2Dui->GetImage().pImage->GetRotation();
	fRotation += ROTATIONSPEED;
	p2Dui->GetImage().pImage->SetRotation(fRotation);

	// UIの移動処理
	D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();
	pos->x += MOVESPEED;
	pos->y -= MOVESPEED;

	// 頂点座標のフラグ設定
	p2Dui->GetImage().pImage->SetPosflag();
}








//-------------------------------------------------------------------------------------------------------------
// サイズ変更の更新処理
//-------------------------------------------------------------------------------------------------------------
void CStory::Update_SizeChange(PERFORM2DUI * pPerfomUi)
{
	if (!pPerfomUi->pScal) return;
	// 変数宣言
	SCALING * pScal = pPerfomUi->pScal;
	// UIの取得
	D3DXVECTOR2 * pSize;	// サイズ情報
	// サイズ情報の取得
	pSize = pPerfomUi->pC2dui->GetImage().pImage->GetSize();
	// 切り替えOFF|切り替わるON/OFF
	if (pScal->nCntTimeChange == pScal->nTimeChange)
	{
		// 切り替わるON/OFF
		pScal->bChange = !pScal->bChange;
		// 切り替わるOFF時
		if (!pScal->bChange)
		{
			// 拡大率の差分
			pScal->fScalDiff = (pScal->fScalChange - pScal->fScal) / pScal->nTimeChange;
			// 目標の拡大率に強制変化
			pScal->fScal = 1.0f;
		}
		// 切り替わるON時
		else
		{
			// 目標の拡大率に強制変化
			pScal->fScal = pScal->fScalChange;
			// 拡大率の差分
			pScal->fScalDiff = (1.0f - pScal->fScalChange) / pScal->nTimeChange;
		}
		// カウント初期化
		pScal->nCntTimeChange = 0;
		// 関数を抜ける
		return;
	}
	// 切り替わる時間加算
	pScal->nCntTimeChange++;
	// 拡大率の変化
	pScal->fScal += pScal->fScalDiff;
	// サイズの変化
	*pSize = pScal->OriginSize * pScal->fScal;
	// 頂点カラーの設定
	pPerfomUi->pC2dui->GetImage().pImage->SetPosflag();
}

//-------------------------------------------------------------------------------------------------------------
// 演出UIの初期化処理
//-------------------------------------------------------------------------------------------------------------
void CStory::Init_PerfomUi(int const & nMaxUi, PERFORM2DUI * pPerfomUi)
{
	// 初期化処理
	for (int nCntUi = 0; nCntUi < nMaxUi; nCntUi++,pPerfomUi++)
	{
		pPerfomUi->pScal = NULL;
		pPerfomUi->pC2dui = NULL;
	}
}
