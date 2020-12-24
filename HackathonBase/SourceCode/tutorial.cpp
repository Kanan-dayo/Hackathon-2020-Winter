//*************************************************************************************************************
//
// �^�C�g������ [title.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "tutorial.h"
#include "texture.h"
#include "2DUI.h"

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CTutorial * CTutorial::Create(void)
{
	CTutorial * pTutorial = new CTutorial;
	pTutorial->Init();
	return pTutorial;
}

//-------------------------------------------------------------------------------------------------------------
// ������
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
	pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	pC2dui->SetFadeAbility(N2Dui_fade(true, true, 60, -1));
	pC2dui->SetFlashingAbility(N2Dui_flashing(true, 3));
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Update(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Draw(void)
{
}