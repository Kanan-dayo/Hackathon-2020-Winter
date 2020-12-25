//*************************************************************************************************************
//
// �X�g�[���[���� [story.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "story.h"
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
#define STORYNAME_POS	(D3DXVECTOR3(640.0f, 200.0f, 0.0f))	// �X�g�[���[���ʒu
#define STORYPRESS_POS	(D3DXVECTOR3(640.0f, 500.0f, 0.0f))	// �v���X�X�^�[�g�ʒu
/* �T�C�Y��� */
#define STORYNAME_SIZE	(D3DXVECTOR2(1104.0f, 204.0f))		// �X�g�[���[���T�C�Y
#define STORYPRESS_SIZE	(D3DXVECTOR2(411.0f, 60.0f))		// �v���X�X�^�[�g�T�C�Y


//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CStory * CStory::Create(void)
{
	CStory *pStory = new CStory;
	pStory->Init();
	return pStory;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CStory::Init(void)
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


	// �X�g�[���[1
	seting.nTextureID = CTexture::NAME_TITLE_BG;
	seting.size = D3DXVECTOR2(1280.0f, 720.0f);
	m_apPerfomUi[TYPE::TYPE_STORY_1].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);


	// �X�g�[���[2
	seting.pos = STORYNAME_POS;
	seting.size = STORYNAME_SIZE;
	seting.nTextureID = CTexture::NAME_RANKING;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pC2dui = C2DUi::Create(seting,CScene::PRIORITY_BUI);


	// �X�g�[���[3
	seting.pos = STORYPRESS_POS;
	seting.size = STORYPRESS_SIZE;
	seting.nTextureID = CTexture::NAME_RESULT;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CStory::Uninit(void)
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
void CStory::Update(void)
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
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_TUTORIAL);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CStory::Draw(void)
{
}








//-------------------------------------------------------------------------------------------------------------
// �T�C�Y�ύX�̍X�V����
//-------------------------------------------------------------------------------------------------------------
void CStory::Update_SizeChange(PERFORM2DUI * pPerfomUi)
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
void CStory::Init_PerfomUi(int const & nMaxUi, PERFORM2DUI * pPerfomUi)
{
	// ����������
	for (int nCntUi = 0; nCntUi < nMaxUi; nCntUi++,pPerfomUi++)
	{
		pPerfomUi->pScal = NULL;
		pPerfomUi->pC2dui = NULL;
	}
}
