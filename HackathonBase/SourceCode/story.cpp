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
#define ROTATIONSPEED	(0.02f)	// ��]���x
#define MOVESPEED		(15.0f)	// ���y�[�W�̑J�ڃX�s�[�h
/* �ʒu��� */


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
	m_nPushButton = -1;

	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;
	seting.nValue = 1000;
	seting.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	seting.size = D3DXVECTOR2(1280.0f, 720.0f);
	seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_FADE;


	// �X�g�[���[3
	seting.nTextureID = CTexture::NAME_RESULT;
	m_apPerfomUi[TYPE::TYPE_STORY_3].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);


	// �X�g�[���[2
	seting.nTextureID = CTexture::NAME_RANKING;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pC2dui = C2DUi::Create(seting,CScene::PRIORITY_BUI);


	// �X�g�[���[1
	seting.nTextureID = CTexture::NAME_TITLE_BG;
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
	Update_UiMove(m_nPushButton);
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		// �������񐔃A�b�v
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
// �`��
//-------------------------------------------------------------------------------------------------------------
void CStory::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V_UI
//-------------------------------------------------------------------------------------------------------------
void CStory::Update_UiMove(int const & nUi)
{
	if (nUi < 0 || nUi >= TYPE::TYPE_MAX) return;
	else if (!m_apPerfomUi[nUi].pC2dui) return;
	C2DUi * p2Dui = m_apPerfomUi[nUi].pC2dui;
	// ���_���W�̉�]����
	float fRotation = p2Dui->GetImage().pImage->GetRotation();
	fRotation += ROTATIONSPEED;
	p2Dui->GetImage().pImage->SetRotation(fRotation);

	// UI�̈ړ�����
	D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();
	pos->x += MOVESPEED;
	pos->y -= MOVESPEED;

	// ���_���W�̃t���O�ݒ�
	p2Dui->GetImage().pImage->SetPosflag();
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
