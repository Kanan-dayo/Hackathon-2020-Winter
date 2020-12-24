//*************************************************************************************************************
//
// タイトル処理 [title.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "tutorial.h"
#include "texture.h"
#include "2DUI.h"
#include "fade.h"
#include "keyboard.h"

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CTutorial * CTutorial::Create(void)
{
	CTutorial * pTutorial = new CTutorial;
	pTutorial->Init();
	return pTutorial;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Init(void)
{
	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;
	seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_FADE | N2Dui_mask::E_M_NUMBER;
	seting.nTextureID = CTexture::NAME_NUMBER;
	seting.nValue = 1000;
	seting.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	seting.size = D3DXVECTOR2(200.0f, 200.0f);
	// タイトル名
	m_pC2dui[TYPE::TYPE_NAME] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	seting.pos = D3DXVECTOR3(640.0f, 500.0f, 0.0f);
	seting.size = D3DXVECTOR2(400.0f, 200.0f);
	seting.nTextureID = CTexture::NAME_PleasePressButton000;

	// キー入力ボタン
	m_pC2dui[TYPE::TYPE_KEYINFO] = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_pC2dui[TYPE::TYPE_KEYINFO]->SetFadeAbility(N2Dui_fade(true, true, 60, -1));
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Update(void)
{
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			// チュートリアルをフェードする
			m_pC2dui[TYPE::TYPE_NAME]->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
			// キーボタンをフェードする
			m_pC2dui[TYPE::TYPE_KEYINFO]->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_GAME);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Draw(void)
{
}
