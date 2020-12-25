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


#include "2Deffect.h"
#include "3Dparticle.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define ROTATIONSPEED		(0.02f)	// ��]���x
#define MOVESPEED			(15.0f)	// ���y�[�W�̑J�ڃX�s�[�h
#define SCALTIME			(50)	// �g�嗦�̖ڕW�g�嗦�ɂ����鎞��
#define SCALPERSENT			(0.3f)	// �g�嗦

// �y�[�W�p
#define STORY_1_POS			(D3DXVECTOR3(640.0f, 360.0f, 0.0f))		// �X�g�[���[1�ʒu
#define STORY_2_POS			(D3DXVECTOR3(640.0f, 360.0f, 0.0f))		// �X�g�[���[2�ʒu

#define STORY_1_SIZE		(D3DXVECTOR2(1280.0f, 720.0f))	// �X�g�[���[1_�T�C�Y
#define STORY_2_SIZE		(D3DXVECTOR2(1280.0f, 720.0f))	// �X�g�[���[2_�T�C�Y

// �T���^�p
#define SANTA_IKARI_POS (D3DXVECTOR3(230.0f, 400.0f, 0.0f))
#define SANTA_IKARI_SIZE (D3DXVECTOR2(385.0f, 450.0f))

#define SANTA_NORMAL_POS (D3DXVECTOR3(210.0f, 400.0f, 0.0f))
#define SANTA_NORMAL_SIZE (D3DXVECTOR2(321.0f, 410.0f))

#define STORY_SANTA_MOVEMENT	(5)							// ���U��

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

	// �X�g�[���[2
	seting.pos = STORY_2_POS;
	seting.size = STORY_2_SIZE;
	seting.nTextureID = CTexture::NAME_STORY_SLID2;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pC2dui = C2DUi::Create(seting,CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_2].bMove = false;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pScal = new SCALING;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pScal->Set(seting.size, SCALPERSENT);


	// �T���^�{��
	seting.pos = SANTA_IKARI_POS;
	seting.size = SANTA_IKARI_SIZE;
	seting.nTextureID = CTexture::NAME_STORY_SANTAIKARI;
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA_IKARI].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA_IKARI].bMove = false;

	// �X�g�[���[1
	seting.pos = STORY_1_POS;
	seting.size = STORY_1_SIZE;
	seting.nTextureID = CTexture::NAME_STORY_SLID1;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_1].bMove = false;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pScal = new SCALING;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pScal->Set(seting.size, SCALPERSENT);

	// �T���^
	seting.pos = SANTA_NORMAL_POS;
	seting.size = SANTA_NORMAL_SIZE;
	seting.nTextureID = CTexture::NAME_STORY_SANTA;
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA_NORMAL].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA_NORMAL].bMove = false;
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
		m_apPerfomUi[nCntUi].pC2dui = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CStory::Update(void)
{
	// �T�C�Y�ύX�̍X�V����
	for (int nCntUi = TYPE::TYPE_STORY_1; nCntUi < TYPE::TYPE_MAX; nCntUi++)
	{
		Update_UiMove(nCntUi);
	}
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		// �������񐔃A�b�v
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
	if (!m_apPerfomUi[nUi].pC2dui) return;

	C2DUi * p2Dui = m_apPerfomUi[nUi].pC2dui;

	// �T���^�p
	if (nUi == TYPE::TYPE_STORY_SANTA_NORMAL)
	{
		// UI�̈ړ�����
		D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();

		pos->x = SANTA_NORMAL_POS.x - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;
		pos->y = SANTA_NORMAL_POS.y - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;

		// �t�F�[�h����
		if (m_apPerfomUi[nUi].bMove)
		{
			// �^�C�g�������t�F�[�h����
			m_apPerfomUi[nUi].pC2dui->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
		}

		// ���_���W�̃t���O�ݒ�
		p2Dui->GetImage().pImage->SetPosflag();
		// �t���[���J�E���g�A�b�v
		m_nCntFram++;
	}
	// �T���^�p
	else if (nUi == TYPE::TYPE_STORY_SANTA_IKARI)
	{
		// UI�̈ړ�����
		D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();

		pos->x = SANTA_IKARI_POS.x - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;
		pos->y = SANTA_IKARI_POS.y - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;

		// �t�F�[�h����
		if (m_apPerfomUi[nUi].bMove)
		{
			// �^�C�g�������t�F�[�h����
			m_apPerfomUi[nUi].pC2dui->SetFadeAbility(N2Dui_fade(true, false, 5, -1));
		}

		// ���_���W�̃t���O�ݒ�
		p2Dui->GetImage().pImage->SetPosflag();
		// �t���[���J�E���g�A�b�v
		m_nCntFram++;
	}

	// �y�[�W�p
	else
	{
		if (!m_apPerfomUi[nUi].bMove) return;
		// ���_���W�̉�]����
		float fRotation = p2Dui->GetImage().pImage->GetRotation();
		fRotation += ROTATIONSPEED;
		p2Dui->GetImage().pImage->SetRotation(fRotation);

		// UI�̈ړ�����
		D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();
		pos->x += MOVESPEED;
		pos->y -= MOVESPEED;

		// �g�嗦�̕ω�����
		Update_SizeChange(&m_apPerfomUi[nUi]);


		// ���_���W�̃t���O�ݒ�
		p2Dui->GetImage().pImage->SetPosflag();
		// ������݂���
		if (pos->x > 1500.0f ||
			pos->y < -500.0f)
		{
			m_apPerfomUi[nUi].pC2dui->Release();
			m_apPerfomUi[nUi].pC2dui = NULL;
		}
	}
}








//-------------------------------------------------------------------------------------------------------------
// �T�C�Y�ύX�̍X�V����
//-------------------------------------------------------------------------------------------------------------
void CStory::Update_SizeChange(PERFORM2DUI * pPerfomUi)
{
	if (!pPerfomUi->pScal) return;
	// �ϐ��錾
	SCALING * pScal = pPerfomUi->pScal;
	// �؂�ւ�OFF|�؂�ւ��ON/OFF
	if (pScal->nCntTimeChange >= SCALTIME)
	{
		return;
	}
	// UI�̎擾
	D3DXVECTOR2 * pSize;	// �T�C�Y���
	// �T�C�Y���̎擾
	pSize = pPerfomUi->pC2dui->GetImage().pImage->GetSize();
	// �g�嗦�̕ω�
	pScal->fScal += pScal->fScalDiff;
	// �T�C�Y�̕ω�
	*pSize = pScal->OriginSize * pScal->fScal;
	// �؂�ւ�鎞�ԉ��Z
	pScal->nCntTimeChange++;
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

//-------------------------------------------------------------------------------------------------------------
// �g�嗦�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CStory::SCALING::Set(D3DXVECTOR2 const & SouceSize, float const & fSouceScalChange)
{
	nCntTimeChange = 0;
	fScal = 1.0f;
	OriginSize = SouceSize;
	fScalChange = fSouceScalChange;
	fScalDiff = (fScalChange - fScal) / SCALTIME;
}
