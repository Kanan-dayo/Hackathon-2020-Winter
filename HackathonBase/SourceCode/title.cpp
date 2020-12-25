//*************************************************************************************************************
//
// タイトル処理 [title.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "title.h"
#include "texture.h"
#include "2DUI.h"
#include "fade.h"
#include "keyboard.h"
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define BGSCROLLUP	(0.01f)	// スクロール上昇値
/* 位置情報 */
#define TITLENAME_POS	(D3DXVECTOR3(640.0f, 200.0f, 0.0f))	// タイトル名位置
#define TITLEPRESS_POS	(D3DXVECTOR3(640.0f, 500.0f, 0.0f))	// プレススタート位置
/* サイズ情報 */
#define TITLENAME_SIZE	(D3DXVECTOR2(1104.0f, 204.0f))		// タイトル名サイズ
#define TITLEPRESS_SIZE	(D3DXVECTOR2(411.0f, 60.0f))		// プレススタートサイズ


//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CTitle * CTitle::Create(void)
{
	CTitle *pTitle = new CTitle;
	pTitle->Init();
	return pTitle;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CTitle::Init(void)
{
	// 初期化処理
	Init_PerfomUi(TYPE::TYPE_MAX, m_apPerfomUi);
	m_fBgTexY = 0.0f;

	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;
	seting.nValue = 1000;
	seting.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_FADE;


	// 背景
	seting.nTextureID = CTexture::NAME_TITLE_BG;
	seting.size = D3DXVECTOR2(1280.0f, 720.0f);
	m_apPerfomUi[TYPE::TYPE_BG].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);


	// タイトル名
	seting.pos = TITLENAME_POS;
	seting.size = TITLENAME_SIZE;
	seting.nTextureID = CTexture::NAME_TITLE_NAME;
	m_apPerfomUi[TYPE::TYPE_NAME].pC2dui = C2DUi::Create(seting,CScene::PRIORITY_BUI);
	// 拡大率設定
	m_apPerfomUi[TYPE::TYPE_NAME].pScal = new SCALING;
	m_apPerfomUi[TYPE::TYPE_NAME].pScal->Set(seting.size,1.2f,60);


	// キー入力ボタン
	seting.pos = TITLEPRESS_POS;
	seting.size = TITLEPRESS_SIZE;
	seting.nTextureID = CTexture::NAME_TITLE_PRESSSTART;
	m_apPerfomUi[TYPE::TYPE_KEYINFO].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_KEYINFO].pC2dui->SetFadeAbility(N2Dui_fade(true, true, 60, -1));
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CTitle::Uninit(void)
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
void CTitle::Update(void)
{
	// 背景スクロール処理
	Update_BGScroll();
	// サイズ変更の更新処理
	for (int nCntUi = 0; nCntUi < TYPE::TYPE_MAX; nCntUi++)
	{
		Update_SizeChange(&m_apPerfomUi[nCntUi]);
	}

	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			// タイトル名をフェードする
			m_apPerfomUi[TYPE::TYPE_NAME].pC2dui->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
			// キーボタンをフェードする
			m_apPerfomUi[TYPE::TYPE_KEYINFO].pC2dui->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
			// ストーリーモードに遷移
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_STORY);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 背景スクロール処理
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update_BGScroll(void)
{
	m_fBgTexY += BGSCROLLUP;
	C2DUi * p2DBG = m_apPerfomUi[TYPE::TYPE_BG].pC2dui;
	p2DBG->GetImage().pImage->ScrollTex(m_fBgTexY);
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CTitle::Draw(void)
{
}








//-------------------------------------------------------------------------------------------------------------
// サイズ変更の更新処理
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update_SizeChange(PERFORM2DUI * pPerfomUi)
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
void CTitle::Init_PerfomUi(int const & nMaxUi, PERFORM2DUI * pPerfomUi)
{
	// 初期化処理
	for (int nCntUi = 0; nCntUi < nMaxUi; nCntUi++,pPerfomUi++)
	{
		pPerfomUi->pScal = NULL;
		pPerfomUi->pC2dui = NULL;
	}
}
