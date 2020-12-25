//*************************************************************************************************************
//
// ���U���g���� [result.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "result.h"
#include "texture.h"
#include "2DUI.h"
#include "keyboard.h"
#include "renderer.h"
#include "fade.h"
#include "ranking.h"
#include "XGamepad.h"
#include "sound.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define LINK_RESULTUIINFO	("DATA/TEXT/UIInfo/resultUIInfo.txt")	// UI��񂪂���e�L�X�g�t�@�C��

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CResult::m_posUI[CResult::RESULT_MAX]	= {};
D3DXVECTOR2 CResult::m_sizeUI[CResult::RESULT_MAX]	= {};
int			CResult::m_nNumPair						= 0;

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CResult * CResult::Create(void)
{
	CResult *pResult = new CResult;
	pResult->Init();
	return pResult;
}

// ===================================================================
// UI���̃��[�h
// ===================================================================
HRESULT CResult::LoadUIInfo(void)
{
	// �ϐ��錾
	FILE *pFile;
	char cReadText[128];
	char cHeadText[128] = "\0";

	// �t�@�C�����J��
	pFile = fopen(LINK_RESULTUIINFO, "r");

#ifdef _DEBUG
	std::cout << "���U���gUI�̏����擾\n";
#endif

	// ���s
	if (!pFile)
	{
#ifdef _DEBUG
		std::cout << "�t�@�C���I�[�v���Ɏ��s\n";
#endif
		return E_FAIL;
	}


	// �X�N���v�g������܂ŌJ��Ԃ�
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);
	}
	// �X�N���v�g��������
	if (strcmp(cHeadText, "SCRIPT") == 0)
	{
		int nIndex = 0;

		// �G���h�X�N���v�g������܂ŌJ��Ԃ�
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			strcpy(cHeadText, "");
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);

			// ���s
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// �ԍ�
			if (strcmp(cHeadText, "Index") == 0)
				sscanf(cReadText, "Index = %d", &nIndex);
			// ���W
			if (strcmp(cHeadText, "pos") == 0)
				sscanf(cReadText, "pos = %f %f", &m_posUI[nIndex].x, &m_posUI[nIndex].y);
			// �T�C�Y
			if (strcmp(cHeadText, "size") == 0)
				sscanf(cReadText, "size = %f %f", &m_sizeUI[nIndex].x, &m_sizeUI[nIndex].y);
		}
	}

	// �t�@�C�������
	fclose(pFile);
	// ����
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CResult::Init(void)
{
	LoadUIInfo();

	// �����ݒ�
	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;
	//seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_FADE | N2Dui_mask::E_M_NUMBER;
	//pC2dui->SetFadeAbility(N2Dui_fade(true, true, 60, -1));
	//pC2dui->SetFlashingAbility(N2Dui_flashing(true, 3));

	seting.nTextureID = CTexture::NAME_RESULT_BG;
	seting.pos = m_posUI[RESULT_BG];
	seting.size = m_sizeUI[RESULT_BG];
	m_p2DUI[RESULT_BG] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	seting.nTextureID = CTexture::NAME_RESULT;
	seting.pos = m_posUI[RESULT_TITLE];
	seting.size = m_sizeUI[RESULT_TITLE];
	m_p2DUI[RESULT_TITLE] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	seting.nTextureID = CTexture::NAME_RESULT_TEXT;
	seting.pos = m_posUI[RESULT_TEXT];
	seting.size = m_sizeUI[RESULT_TEXT];
	m_p2DUI[RESULT_TEXT] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	seting.nTextureID = CTexture::NAME_RESULT_NUMPAIR;
	seting.pos = m_posUI[RESULT_NUMPAIR];
	seting.size = m_sizeUI[RESULT_NUMPAIR];
	m_p2DUI[RESULT_NUMPAIR] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	seting.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_NUMBER;
	seting.nTextureID = CTexture::NAME_NUMBER;
	seting.nValue = m_nNumPair;
	seting.pos = m_posUI[RESULT_NUM];
	seting.size = m_sizeUI[RESULT_NUM];
	m_p2DUI[RESULT_NUM] = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_p2DUI[RESULT_NUM]->SetFlashingAbility(N2Dui_flashing(true, 2));
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CResult::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CResult::Update(void)
{
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN) || CManager::GetXGamepad().GetTrigger(CXGamepad::JOYPADKEY_A))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			CManager::GetSound().PlaySoundA(CSound::SOUND_LABEL_SE_DECIDE);
			CRanking::SetPlayerScore(m_nNumPair);
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CResult::Draw(void)
{
}
