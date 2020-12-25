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


#include "2Deffect.h"
#include "3Dparticle.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define ROTATIONSPEED		(0.02f)	// 回転速度
#define MOVESPEED			(15.0f)	// 次ページの遷移スピード
#define SCALTIME			(50)	// 拡大率の目標拡大率にかかる時間
#define SCALPERSENT			(0.3f)	// 拡大率

// ページ用
#define STORY_1_POS			(D3DXVECTOR3(640.0f, 360.0f, 0.0f))		// ストーリー1位置
#define STORY_2_POS			(D3DXVECTOR3(640.0f, 360.0f, 0.0f))		// ストーリー2位置

#define STORY_1_SIZE		(D3DXVECTOR2(1280.0f, 720.0f))	// ストーリー1_サイズ
#define STORY_2_SIZE		(D3DXVECTOR2(1280.0f, 720.0f))	// ストーリー2_サイズ

// サンタ用
#define SANTA_IKARI_POS (D3DXVECTOR3(230.0f, 400.0f, 0.0f))
#define SANTA_IKARI_SIZE (D3DXVECTOR2(385.0f, 450.0f))

#define SANTA_NORMAL_POS (D3DXVECTOR3(210.0f, 400.0f, 0.0f))
#define SANTA_NORMAL_SIZE (D3DXVECTOR2(321.0f, 410.0f))

#define STORY_SANTA_MOVEMENT	(5)							// 微振動

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
	for (int nCntUi = 0; nCntUi < TYPE::TYPE_MAX; nCntUi++)
	{
		m_apPerfomUi[nCntUi].pC2dui = NULL;
		m_apPerfomUi[nCntUi].pScal = NULL;
	}
	m_nCntFram = 0;


	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;
	seting.nValue = 1000;
	seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_FADE;

	// ストーリー2
	seting.pos = STORY_2_POS;
	seting.size = STORY_2_SIZE;
	seting.nTextureID = CTexture::NAME_STORY_SLID2;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pC2dui = C2DUi::Create(seting,CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_2].bMove = false;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pScal = new SCALING;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pScal->Set(seting.size, SCALPERSENT);


	// サンタ怒り
	seting.pos = SANTA_IKARI_POS;
	seting.size = SANTA_IKARI_SIZE;
	seting.nTextureID = CTexture::NAME_STORY_SANTAIKARI;
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA_IKARI].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA_IKARI].bMove = false;

	// ストーリー1
	seting.pos = STORY_1_POS;
	seting.size = STORY_1_SIZE;
	seting.nTextureID = CTexture::NAME_STORY_SLID1;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_1].bMove = false;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pScal = new SCALING;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pScal->Set(seting.size, SCALPERSENT);

	// サンタ
	seting.pos = SANTA_NORMAL_POS;
	seting.size = SANTA_NORMAL_SIZE;
	seting.nTextureID = CTexture::NAME_STORY_SANTA;
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA_NORMAL].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA_NORMAL].bMove = false;
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
		m_apPerfomUi[nCntUi].pC2dui = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CStory::Update(void)
{
	// サイズ変更の更新処理
	for (int nCntUi = TYPE::TYPE_STORY_1; nCntUi < TYPE::TYPE_MAX; nCntUi++)
	{
		Update_UiMove(nCntUi);
	}
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		// 押した回数アップ
		m_nPushButton++;
		if (m_nPushButton == 0)
		{
			m_apPerfomUi[TYPE::TYPE_STORY_1].bMove = true;
			m_apPerfomUi[TYPE::TYPE_STORY_SANTA_NORMAL].bMove = true;
		}
		else if(m_nPushButton == 1)
		{
			m_apPerfomUi[TYPE::TYPE_STORY_2].bMove = true;
			m_apPerfomUi[TYPE::TYPE_STORY_SANTA_IKARI].bMove = true;
		}
		if (m_nPushButton >= 1)
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
	if (!m_apPerfomUi[nUi].pC2dui) return;

	C2DUi * p2Dui = m_apPerfomUi[nUi].pC2dui;

	// サンタ用
	if (nUi == TYPE::TYPE_STORY_SANTA_NORMAL)
	{
		// UIの移動処理
		D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();

		pos->x = SANTA_NORMAL_POS.x - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;
		pos->y = SANTA_NORMAL_POS.y - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;

		// フェード処理
		if (m_apPerfomUi[nUi].bMove)
		{
			// タイトル名をフェードする
			m_apPerfomUi[nUi].pC2dui->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
		}

		// 頂点座標のフラグ設定
		p2Dui->GetImage().pImage->SetPosflag();
		// フレームカウントアップ
		m_nCntFram++;
	}
	// サンタ用
	else if (nUi == TYPE::TYPE_STORY_SANTA_IKARI)
	{
		// UIの移動処理
		D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();

		pos->x = SANTA_IKARI_POS.x - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;
		pos->y = SANTA_IKARI_POS.y - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;

		// フェード処理
		if (m_apPerfomUi[nUi].bMove)
		{
			// タイトル名をフェードする
			m_apPerfomUi[nUi].pC2dui->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
		}

		// 頂点座標のフラグ設定
		p2Dui->GetImage().pImage->SetPosflag();
		// フレームカウントアップ
		m_nCntFram++;
	}

	// ページ用
	else
	{
		if (!m_apPerfomUi[nUi].bMove) return;
		// 頂点座標の回転処理
		float fRotation = p2Dui->GetImage().pImage->GetRotation();
		fRotation += ROTATIONSPEED;
		p2Dui->GetImage().pImage->SetRotation(fRotation);

		// UIの移動処理
		D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();
		pos->x += MOVESPEED;
		pos->y -= MOVESPEED;

		// 拡大率の変化処理
		Update_SizeChange(&m_apPerfomUi[nUi]);


		// 頂点座標のフラグ設定
		p2Dui->GetImage().pImage->SetPosflag();
		// 制限を設ける
		if (pos->x > 1500.0f ||
			pos->y < -500.0f)
		{
			m_apPerfomUi[nUi].pC2dui->Release();
			m_apPerfomUi[nUi].pC2dui = NULL;
		}
	}
}








//-------------------------------------------------------------------------------------------------------------
// サイズ変更の更新処理
//-------------------------------------------------------------------------------------------------------------
void CStory::Update_SizeChange(PERFORM2DUI * pPerfomUi)
{
	if (!pPerfomUi->pScal) return;
	// 変数宣言
	SCALING * pScal = pPerfomUi->pScal;
	// 切り替えOFF|切り替わるON/OFF
	if (pScal->nCntTimeChange >= SCALTIME)
	{
		return;
	}
	// UIの取得
	D3DXVECTOR2 * pSize;	// サイズ情報
	// サイズ情報の取得
	pSize = pPerfomUi->pC2dui->GetImage().pImage->GetSize();
	// 拡大率の変化
	pScal->fScal += pScal->fScalDiff;
	// サイズの変化
	*pSize = pScal->OriginSize * pScal->fScal;
	// 切り替わる時間加算
	pScal->nCntTimeChange++;
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

//-------------------------------------------------------------------------------------------------------------
// 拡大率の設定
//-------------------------------------------------------------------------------------------------------------
void CStory::SCALING::Set(D3DXVECTOR2 const & SouceSize, float const & fSouceScalChange)
{
	nCntTimeChange = 0;
	fScal = 1.0f;
	OriginSize = SouceSize;
	fScalChange = fSouceScalChange;
	fScalDiff = (fScalChange - fScal) / SCALTIME;
}
