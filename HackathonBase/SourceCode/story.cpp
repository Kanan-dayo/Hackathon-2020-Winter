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
#define ROTATIONSPEED		(0.02f)	// ��]���x
#define MOVESPEED			(15.0f)	// ���y�[�W�̑J�ڃX�s�[�h
#define SCALTIME			(50)	// �g�嗦�̖ڕW�g�嗦�ɂ����鎞��
#define SCALPERSENT			(0.3f)	// �g�嗦

// �y�[�W�p
#define STORY_1_POS			(D3DXVECTOR3(800.0f, 300.0f, 0.0f))		// �X�g�[���[1�ʒu
#define STORY_2_POS			(D3DXVECTOR3(800.0f, 300.0f, 0.0f))		// �X�g�[���[2�ʒu

#define STORY_1_SIZE		(D3DXVECTOR2(800.0f, 300.0f))	// �X�g�[���[1_�T�C�Y
#define STORY_2_SIZE		(D3DXVECTOR2(800.0f, 300.0f))	// �X�g�[���[2_�T�C�Y

// �T���^�p
#define STORY_SANTA_POS			(D3DXVECTOR3(800.0f, 600.0f, 0.0f))	// �X�g�[���[�v���X�ʒu

#define STORY_SANTA_SIZE		(D3DXVECTOR2(100.0f, 100.0f))	// �X�g�[���[�v���X_�T�C�Y

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
	m_nPushButton = TYPE::TYPE_STORYBG;
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


	// �X�g�[���[�w�i
	seting.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	seting.size = D3DXVECTOR2(1280.0f, 720.0f);
	seting.nTextureID = CTexture::NAME_TITLE_BG;
	m_apPerfomUi[TYPE::TYPE_STORYBG].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORYBG].bMove = false;


	// �X�g�[���[2
	seting.pos = STORY_2_POS;
	seting.size = STORY_2_SIZE;
	seting.nTextureID = CTexture::NAME_RANKING;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pC2dui = C2DUi::Create(seting,CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_2].bMove = false;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pScal = new SCALING;
	m_apPerfomUi[TYPE::TYPE_STORY_2].pScal->Set(seting.size, SCALPERSENT);

	// �X�g�[���[1
	seting.pos = STORY_1_POS;
	seting.size = STORY_1_SIZE;
	seting.nTextureID = CTexture::NAME_TITLE_BG;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_1].bMove = false;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pScal = new SCALING;
	m_apPerfomUi[TYPE::TYPE_STORY_1].pScal->Set(seting.size, SCALPERSENT);


	// �X�g�[���[�v���X
	seting.pos = STORY_SANTA_POS;
	seting.size = STORY_SANTA_SIZE;
	seting.nTextureID = CTexture::NAME_PLAYER;
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA].pC2dui = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_apPerfomUi[TYPE::TYPE_STORY_SANTA].bMove = true;
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
		if (m_nPushButton >= TYPE::TYPE_STORY_1 && m_nPushButton <= TYPE::TYPE_STORY_2)
		{
			m_apPerfomUi[m_nPushButton].bMove = true;
		}

		if (m_nPushButton > TYPE::TYPE_STORY_2)
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
	if (!m_apPerfomUi[nUi].pC2dui ||
		!m_apPerfomUi[nUi].bMove) return;

	C2DUi * p2Dui = m_apPerfomUi[nUi].pC2dui;

	// �T���^�p
	if (nUi == TYPE::TYPE_STORY_SANTA)
	{
		// UI�̈ړ�����
		D3DXVECTOR3 * pos = p2Dui->GetImage().pImage->GetPosition();

		pos->x = STORY_SANTA_POS.x - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;
		pos->y = STORY_SANTA_POS.y - STORY_SANTA_MOVEMENT + rand() % STORY_SANTA_MOVEMENT * 2;
		// ���_���W�̃t���O�ݒ�
		p2Dui->GetImage().pImage->SetPosflag();
		// �t���[���J�E���g�A�b�v
		m_nCntFram++;
	}
	// �y�[�W�p
	else
	{
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
