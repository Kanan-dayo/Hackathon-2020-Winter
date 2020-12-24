//*************************************************************************************************************
//
// 2DUI処理[2DUI.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "2DUI.h"
#include "Scene2D.h"
#include "number.h"
#include "NumericString.h"
#include "texture.h"
#include "DebugProc.h"

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
C2DUi * C2DUi::Create(N2Dui_seting &seting, PRIORITY pri)
{
	C2DUi * pC2dui = new C2DUi;
	pC2dui->Init(seting, pri);
	return pC2dui;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Init(N2Dui_seting & seting, PRIORITY& pri)
{
	// マスクの設定
	m_mask = seting.mask;
	// テクスチャIDの初期化
	m_nTextureID = seting.nTextureID;
	// 描画フラグの設定
	m_bDisp = seting.bDisp;

	// サイズの変更
	m_Image.resize(N2Dui_img::E_I_MAX);

	// アビリティの生成
	if (Mlf_bit_comp(m_mask.unMask, N2Dui_flag::E_F_NUMBER))
	{
		m_Image[N2Dui_img::E_I_NUMBER].pNumber = CNumericString::Create(seting.pos, seting.col, seting.size, seting.fRotation, seting.nValue);
		m_Image[N2Dui_img::E_I_NUMBER].pNumber->BindTexture(CTexture::GetTextureInfo(seting.nTextureID));
		m_ImageType = N2Dui_img::E_I_NUMBER;
	}
	else
	{
		m_Image[N2Dui_img::E_I_IMAGE].pImage = CScene2D::Create(pri, seting.pos, seting.col, ORIGINVERTEXTYPE_ROTCENTER, seting.size, seting.fRotation, TEXTUREUV(0, 1, 0, 1));
		m_Image[N2Dui_img::E_I_IMAGE].pImage->BindTexture(CTexture::GetTextureInfo(seting.nTextureID));
		m_ImageType = N2Dui_img::E_I_IMAGE;
	}
	if (Mlf_bit_comp(m_mask.unMask, N2Dui_flag::E_F_FLASHING))
	{
		m_AbilityData.push_back();
		m_AbilityData.back().pFlashing = new N2Dui_flashing;
	}
	if (Mlf_bit_comp(m_mask.unMask, N2Dui_flag::E_F_FADE))
	{
		m_AbilityData.push_back();
		m_AbilityData.back().pFade = new N2Dui_fade;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Init(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Uninit(void)
{
	if (m_ImageType == N2Dui_img::E_I_NUMBER)
	{
		if (m_Image[N2Dui_img::E_I_NUMBER].pVoidptr)
		{
			delete m_Image[N2Dui_img::E_I_NUMBER].pNumber;
			m_Image[N2Dui_img::E_I_NUMBER].pNumber = nullptr;
		}
	}
	m_Image.clear();

	for (int nCntAbility = 0; nCntAbility < m_AbilityData.size(); nCntAbility++)
	{
		delete m_AbilityData[nCntAbility].pVoidptr;
		m_AbilityData[nCntAbility].pVoidptr = nullptr;
	}
	m_AbilityData.clear();
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Update(void)
{
	// アビリティの更新
	for (auto &itr : m_AbilityData)
	{
		itr.pBase->Update(this);
	}
	// 数字の時更新
	if (m_ImageType == N2Dui_img::E_I_NUMBER)
	{
		m_Image[m_ImageType].pNumber->Update();
	}
	else if (m_ImageType == N2Dui_img::E_I_IMAGE)
	{
		m_Image[m_ImageType].pImage->UpdateVertex();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Draw(void)
{
	if (m_bDisp && m_ImageType == N2Dui_img::E_I_NUMBER)
	{
		m_Image[m_ImageType].pNumber->Draw();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画フラグの切り替え
//-------------------------------------------------------------------------------------------------------------
void C2DUi::ChangeDispFlag(void)
{
	m_bDisp ^= 1;
	ReflectDispFlagInImage();
}

//-------------------------------------------------------------------------------------------------------------
// フェード能力の設定
//-------------------------------------------------------------------------------------------------------------
void C2DUi::SetFadeAbility(N2Dui_fade & fade)
{
	auto *pItr = m_AbilityData.LambdaSearch([](ABILITY &lhs) {return  lhs.pBase->type == N2Dui_ability::E_A_FADE; });
	if (pItr)
	{
		*pItr->pFade = fade;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 点滅の設定
//-------------------------------------------------------------------------------------------------------------
void C2DUi::SetFlashingAbility(N2Dui_flashing & flashing)
{
	auto *pItr = m_AbilityData.LambdaSearch([](ABILITY &lhs) {return  lhs.pBase->type == N2Dui_ability::E_A_FLASHING; });
	if (pItr)
	{
		*pItr->pFlashing = flashing;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 先頭のアビリティデータのポインタの取得
//-------------------------------------------------------------------------------------------------------------
C2DUi::ABILITY* C2DUi::GetAbilityDataTopPtr(void)
{
	return &m_AbilityData[0];
}

//-------------------------------------------------------------------------------------------------------------
// アビリティデータの取得
//-------------------------------------------------------------------------------------------------------------
C2DUi::ABILITY & C2DUi::GetAbilityData(N2Dui_ability type)
{
	if (Mlf_InRange(type, N2Dui_ability::E_A_MAX, N2Dui_ability::E_A_NONE))
	{
		return m_AbilityData[type];
	}
	return m_AbilityData[0];
}

//-------------------------------------------------------------------------------------------------------------
// 描画フラグを画像に反映
//-------------------------------------------------------------------------------------------------------------
void C2DUi::ReflectDispFlagInImage(void)
{
	if (m_ImageType == N2Dui_img::E_I_IMAGE)
	{
		m_Image[m_ImageType].pImage->SetDisp(m_bDisp);
	}
}


