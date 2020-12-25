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
#include "fade.h"
#include "keyboard.h"
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
/* �ʒu��� */
#define TUTORIALSKIP_POS	(D3DXVECTOR3(1100.0f, 650.0f, 0.0f))	// �`���[�g���A���X�L�b�v�ʒu

/* �T�C�Y��� */
#define TUTORIALSKIP_SIZE	(D3DXVECTOR2(300.0f, 150.0f))		// �`���[�g���A���X�L�b�v�T�C�Y


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
	// ����������
	Init_PerfomUi(TYPE::TYPE_MAX, m_apPerfomUi);


	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;
	seting.nValue = 1000;
	seting.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_FADE;


	// �^�C�g����
	seting.size = D3DXVECTOR2(1280.0f, 720.0f);
	seting.nTextureID = CTexture::NAME_TUTORIAL_BG;
	m_apPerfomUi[TYPE::TYPE_NAME].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	// �L�[���̓{�^��
	seting.pos = TUTORIALSKIP_POS;
	seting.size = TUTORIALSKIP_SIZE;
	seting.nTextureID = CTexture::NAME_TUTORIAL_TUTORIALSKIP;
	m_apPerfomUi[TYPE::TYPE_KEYINFO].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_KEYINFO].pC2dui->SetFadeAbility(N2Dui_fade(true, true, 60, -1));
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Uninit(void)
{
	for (int nCntUi = 0; nCntUi < TYPE::TYPE_MAX; nCntUi++)
	{
		// �g�嗦�̊J��
		if (m_apPerfomUi[nCntUi].pScal)
		{
			delete m_apPerfomUi[nCntUi].pScal;
			m_apPerfomUi[nCntUi].pScal = NULL;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Update(void)
{
	// �T�C�Y�ύX�̍X�V����
	for (int nCntUi = 0; nCntUi < TYPE::TYPE_MAX; nCntUi++)
	{
		Update_SizeChange(&m_apPerfomUi[nCntUi]);
	}
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{

			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_GAME);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Draw(void)
{
}


//-------------------------------------------------------------------------------------------------------------
// �T�C�Y�ύX�̍X�V����
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Update_SizeChange(PERFORM2DUI * pPerfomUi)
{
	if (!pPerfomUi->pScal) return;
	// �ϐ��錾
	SCALING * pScal = pPerfomUi->pScal;
	// UI�̎擾
	D3DXVECTOR2 * pSize;	// �T�C�Y���
	// �T�C�Y���̎擾
	pSize = pPerfomUi->pC2dui->GetImage().pImage->GetSize();
	// �؂�ւ�OFF|�؂�ւ��ON/OFF
	if (pScal->nCntTimeChange == pScal->nTimeChange)
	{
		// �؂�ւ��ON/OFF
		pScal->bChange = !pScal->bChange;
		// �؂�ւ��OFF��
		if (!pScal->bChange)
		{
			// �g�嗦�̍���
			pScal->fScalDiff = (pScal->fScalChange - pScal->fScal) / pScal->nTimeChange;
			// �ڕW�̊g�嗦�ɋ����ω�
			pScal->fScal = 1.0f;
		}
		// �؂�ւ��ON��
		else
		{
			// �ڕW�̊g�嗦�ɋ����ω�
			pScal->fScal = pScal->fScalChange;
			// �g�嗦�̍���
			pScal->fScalDiff = (1.0f - pScal->fScalChange) / pScal->nTimeChange;
		}
		// �J�E���g������
		pScal->nCntTimeChange = 0;
		// �֐��𔲂���
		return;
	}
	// �؂�ւ�鎞�ԉ��Z
	pScal->nCntTimeChange++;
	// �g�嗦�̕ω�
	pScal->fScal += pScal->fScalDiff;
	// �T�C�Y�̕ω�
	*pSize = pScal->OriginSize * pScal->fScal;
	// ���_�J���[�̐ݒ�
	pPerfomUi->pC2dui->GetImage().pImage->SetPosflag();
}

//-------------------------------------------------------------------------------------------------------------
// ���oUI�̏���������
//-------------------------------------------------------------------------------------------------------------
void CTutorial::Init_PerfomUi(int const & nMaxUi, PERFORM2DUI * pPerfomUi)
{
	// ����������
	for (int nCntUi = 0; nCntUi < nMaxUi; nCntUi++, pPerfomUi++)
	{
		pPerfomUi->pScal = NULL;
		pPerfomUi->pC2dui = NULL;
	}
}
