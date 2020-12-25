//*************************************************************************************************************
//
// �^�C�g������ [title.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "title.h"
#include "texture.h"
#include "2DUI.h"
#include "fade.h"
#include "keyboard.h"
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define BGSCROLLUP	(0.01f)	// �X�N���[���㏸�l
/* �ʒu��� */
#define TITLENAME_POS	(D3DXVECTOR3(640.0f, 200.0f, 0.0f))	// �^�C�g�����ʒu
#define TITLEPRESS_POS	(D3DXVECTOR3(640.0f, 500.0f, 0.0f))	// �v���X�X�^�[�g�ʒu
/* �T�C�Y��� */
#define TITLENAME_SIZE	(D3DXVECTOR2(1104.0f, 204.0f))		// �^�C�g�����T�C�Y
#define TITLEPRESS_SIZE	(D3DXVECTOR2(411.0f, 60.0f))		// �v���X�X�^�[�g�T�C�Y


//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CTitle * CTitle::Create(void)
{
	CTitle *pTitle = new CTitle;
	pTitle->Init();
	return pTitle;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CTitle::Init(void)
{
	// ����������
	Init_PerfomUi(TYPE::TYPE_MAX, m_apPerfomUi);
	m_fBgTexY = 0.0f;

	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;
	seting.nValue = 1000;
	seting.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_FADE;


	// �w�i
	seting.nTextureID = CTexture::NAME_TITLE_BG;
	seting.size = D3DXVECTOR2(1280.0f, 720.0f);
	m_apPerfomUi[TYPE::TYPE_BG].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);


	// �^�C�g����
	seting.pos = TITLENAME_POS;
	seting.size = TITLENAME_SIZE;
	seting.nTextureID = CTexture::NAME_TITLE_NAME;
	m_apPerfomUi[TYPE::TYPE_NAME].pC2dui = C2DUi::Create(seting,CScene::PRIORITY_BUI);
	// �g�嗦�ݒ�
	m_apPerfomUi[TYPE::TYPE_NAME].pScal = new SCALING;
	m_apPerfomUi[TYPE::TYPE_NAME].pScal->Set(seting.size,1.2f,60);


	// �L�[���̓{�^��
	seting.pos = TITLEPRESS_POS;
	seting.size = TITLEPRESS_SIZE;
	seting.nTextureID = CTexture::NAME_TITLE_PRESSSTART;
	m_apPerfomUi[TYPE::TYPE_KEYINFO].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_KEYINFO].pC2dui->SetFadeAbility(N2Dui_fade(true, true, 60, -1));
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CTitle::Uninit(void)
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
void CTitle::Update(void)
{
	// �w�i�X�N���[������
	Update_BGScroll();
	// �T�C�Y�ύX�̍X�V����
	for (int nCntUi = 0; nCntUi < TYPE::TYPE_MAX; nCntUi++)
	{
		Update_SizeChange(&m_apPerfomUi[nCntUi]);
	}

	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			// �^�C�g�������t�F�[�h����
			m_apPerfomUi[TYPE::TYPE_NAME].pC2dui->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
			// �L�[�{�^�����t�F�[�h����
			m_apPerfomUi[TYPE::TYPE_KEYINFO].pC2dui->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
			// �X�g�[���[���[�h�ɑJ��
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_STORY);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �w�i�X�N���[������
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update_BGScroll(void)
{
	m_fBgTexY += BGSCROLLUP;
	C2DUi * p2DBG = m_apPerfomUi[TYPE::TYPE_BG].pC2dui;
	p2DBG->GetImage().pImage->ScrollTex(m_fBgTexY);
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CTitle::Draw(void)
{
}








//-------------------------------------------------------------------------------------------------------------
// �T�C�Y�ύX�̍X�V����
//-------------------------------------------------------------------------------------------------------------
void CTitle::Update_SizeChange(PERFORM2DUI * pPerfomUi)
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
void CTitle::Init_PerfomUi(int const & nMaxUi, PERFORM2DUI * pPerfomUi)
{
	// ����������
	for (int nCntUi = 0; nCntUi < nMaxUi; nCntUi++,pPerfomUi++)
	{
		pPerfomUi->pScal = NULL;
		pPerfomUi->pC2dui = NULL;
	}
}
