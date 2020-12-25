//*************************************************************************************************************
//
// �Q�[������ [game.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "game.h"
#include "texture.h"
#include "2DUI.h"
#include "player.h"
#include "keyboard.h"
#include "renderer.h"
#include "fade.h"
#include "enemyMana.h"
#include "NumericString.h"
#include "result.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define LINK_GAMEUIINFO	("DATA/TEXT/UIInfo/gameUIInfo.txt")		// UI���̂���e�L�X�g�t�@�C��
#define TIME_DEFAULT	(60)									// �ŏ��̃^�C��

#define TIME_READY		(40)
#define TIME_GO			(40)
#define TIME_FINISH		(40)

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
CONST D3DXVECTOR3	CGame::m_InitPosPlayer				= D3DXVECTOR3(640.0f, 360.0f, 0.0f);
CONST D3DXVECTOR2	CGame::m_InitSizePlayer				= D3DXVECTOR2(60.0f, 60.0f);
D3DXVECTOR3			CGame::m_posUI[CGame::GAMEUI_MAX]	= {};
D3DXVECTOR2			CGame::m_sizeUI[CGame::GAMEUI_MAX]	= {};
C2DUi *				CGame::m_pGameUI[CGame::GAMEUI_MAX] = {};
int					CGame::m_nNumKill					= 0;
int					CGame::m_nCntTime					= TIME_DEFAULT;

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CGame * CGame::Create(void)
{
	CGame *pGame = new CGame;
	pGame->Init();
	return pGame;
}

//-------------------------------------------------------------------------------------------------------------
// UI���̃��[�h
//-------------------------------------------------------------------------------------------------------------
HRESULT CGame::LoadUIInfo(void)
{
	// �ϐ��錾
	FILE *pFile;
	char cReadText[128];
	char cHeadText[128] = "\0";

	// �t�@�C�����J��
	pFile = fopen(LINK_GAMEUIINFO, "r");

#ifdef _DEBUG
	std::cout << "�Q�[��UI�̏����擾\n";
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

#ifdef _DEBUG
	std::cout << "�����L���OUI�̏��擾����\n";
#endif

	// ����
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CGame::Init(void)
{
	m_nNumKill = 0;
	m_nCntFrame = 0;
	m_nCntTime = TIME_DEFAULT;

	// UI���̃��[�h
	LoadUIInfo();
	// �Q�[��UI�̐���
	CreateGameUI();

	// �G�}�l�[�W���[�̐���
	m_pEnemyMana = CEnemyMana::Create();
	m_mode = MODE_BEGIN;
	m_nCntMode = ML_INT_UNSET;

	m_pPlayer = CPlayer::Create(m_InitPosPlayer, m_InitSizePlayer);
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CGame::Uninit(void)
{
	if (m_pEnemyMana)
	{
		m_pEnemyMana->Uninit();
		delete m_pEnemyMana;
		m_pEnemyMana = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CGame::Update(void)
{
	// ���[�h���̏���
	switch (m_mode)
	{
	case MODE_BEGIN:	ModeBegin();	break;
	case MODE_READY:	ModeReady();	break;
	case MODE_GO:		ModeGo();		break;
	case MODE_NORMAL:	ModeNormal();	break;
	case MODE_FINISH:	ModeFinish();	break;
	}
	// ���[�h�̑J��
	ModeTrans();
	// �}�l�[�W���[�̍X�V
	m_pEnemyMana->Update();
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CGame::Draw(void)
{
	// �}�l�[�W���[�̕`��
	m_pEnemyMana->Draw();
}

//-------------------------------------------------------------------------------------------------------------
// �Q�[��UI�̐���
//-------------------------------------------------------------------------------------------------------------
void CGame::CreateGameUI(void)
{
	N2Dui_seting seting;
	N2Dui_seting set;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;

	// �y�A��
	seting.nTextureID = CTexture::NAME_PAIR;
	seting.pos = m_posUI[GAMEUI_PAIR];
	seting.size = m_sizeUI[GAMEUI_PAIR];
	m_pGameUI[GAMEUI_PAIR] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	// �X�R�A
	seting.mask.unMask = N2Dui_mask::E_M_NUMBER;
	seting.nTextureID = CTexture::NAME_NUMBER;
	seting.nValue = 100;
	seting.pos = m_posUI[GAMEUI_SCORE];
	seting.size = m_sizeUI[GAMEUI_SCORE];
	m_pGameUI[GAMEUI_SCORE] = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_pGameUI[GAMEUI_SCORE]->GetImage().pNumber->SetValue(0);
	m_pGameUI[GAMEUI_SCORE]->GetImage().pNumber->UpdateNumber();

	// �^�C�}�[
	seting.mask.unMask = N2Dui_mask::E_M_NUMBER;
	seting.nTextureID = CTexture::NAME_NUMBER;
	seting.nValue = m_nCntTime;
	seting.pos = m_posUI[GAMEUI_TIMER];
	seting.size = m_sizeUI[GAMEUI_TIMER];
	m_pGameUI[GAMEUI_TIMER] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	set.bDisp = false;
	set.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	set.fRotation = ML_FLOAT_UNSET;

	// ���f�B
	set.nTextureID = CTexture::NAME_READY;
	set.pos = m_posUI[GAMEUI_READY];
	set.size = m_sizeUI[GAMEUI_READY];
	m_pGameUI[GAMEUI_READY] = C2DUi::Create(set, CScene::PRIORITY_BUI);

	// �S�[
	set.nTextureID = CTexture::NAME_GO;
	set.pos = m_posUI[GAMEUI_GO];
	set.size = m_sizeUI[GAMEUI_GO];
	m_pGameUI[GAMEUI_GO] = C2DUi::Create(set, CScene::PRIORITY_BUI);

	// �t�B�j�b�V��
	set.nTextureID = CTexture::NAME_FINISH;
	set.pos = m_posUI[GAMEUI_FINISH];
	set.size = m_sizeUI[GAMEUI_FINISH];
	m_pGameUI[GAMEUI_FINISH] = C2DUi::Create(set, CScene::PRIORITY_BUI);
}

//-------------------------------------------------------------------------------------------------------------
// ���[�h�J��
//-------------------------------------------------------------------------------------------------------------
void CGame::ModeTrans(void)
{
	if (m_mode == MODE_END)
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_RESULT);
		}
	}

#ifdef _DEBUG
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_RESULT);
		}
	}
	if (CManager::GetKeyboard().GetTrigger(DIK_1))
	{
		AddTime(1);
	}
	if (CManager::GetKeyboard().GetTrigger(DIK_2))
	{
		AddTime(-1);
	}
#endif
}

//-------------------------------------------------------------------------------------------------------------
// ���[�h�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CGame::SetMode(MODE mode)
{
	if (m_mode != mode)
	{
		m_mode = mode;
		m_nCntMode = ML_INT_UNSET;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �^�C�}�[�̍X�V
//-------------------------------------------------------------------------------------------------------------
void CGame::UpdateTimer(void)
{
	m_nCntFrame++;

	// �J���[�X�V
	if (m_nCntTime <= 30 && m_nCntTime > 15)
	{
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->SetCol(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->UpdateCol();
	}
	else if (m_nCntTime <= 15)
	{
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->UpdateCol();
	}
	else
	{
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->UpdateCol();
	}

	if (m_nCntFrame <= 60)
	{
		return;
	}
	
	// 1�b�o��
	m_nCntFrame = 0;
	if (m_nCntTime > 0)
	{
		m_nCntTime--;
		// �^�C�}�[�̐������X�V
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->SetValue(m_nCntTime);
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->UpdateNumber();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �J�n��
//-------------------------------------------------------------------------------------------------------------
void CGame::ModeBegin(void)
{
	// ���Ԍo�߂Ŏ���
	m_nCntMode++;
	if (m_nCntMode >= 20)
	{
		m_nCntMode = 0;
		m_mode = MODE_READY;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ���f�B��
//-------------------------------------------------------------------------------------------------------------
void CGame::ModeReady(void)
{
	if (m_nCntMode == 0)
		m_pGameUI[GAMEUI_READY]->GetImage().pImage->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	// ���Ԍo�߂Ŏ���
	m_nCntMode++;
	if (m_nCntMode >= TIME_READY)
	{
		m_nCntMode = 0;
		m_mode = MODE_GO;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �S�[��
//-------------------------------------------------------------------------------------------------------------
void CGame::ModeGo(void)
{
	if (m_nCntMode == 0)
	{
		m_pGameUI[GAMEUI_READY]->GetImage().pImage->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_pGameUI[GAMEUI_GO]->GetImage().pImage->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// ���Ԍo�߂Ŏ���
	m_nCntMode++;
	if (m_nCntMode >= TIME_GO)
	{
		m_nCntMode = 0;
		m_mode = MODE_NORMAL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �ʏ펞
//-------------------------------------------------------------------------------------------------------------
void CGame::ModeNormal(void)
{
	// �ŏ�����UI������
	if (m_nCntMode == 0)
	{
		m_pGameUI[GAMEUI_GO]->GetImage().pImage->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		m_nCntMode++;
	}

	// �^�C�}�[�̍X�V
	UpdateTimer();

	// �J�E���g0�Ŏ���
	if (m_nCntTime == 0)
	{
		m_nCntMode = 0;
		m_mode = MODE_FINISH;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �t�B�j�b�V����
//-------------------------------------------------------------------------------------------------------------
void CGame::ModeFinish(void)
{
	if (m_nCntMode == 0)
	{
		m_pGameUI[GAMEUI_FINISH]->GetImage().pImage->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	// ���Ԍo�߂Ŏ���
	m_nCntMode++;
	if (m_nCntMode >= TIME_FINISH)
	{
		m_nCntMode = 0;
		m_mode = MODE_END;
		// ���j����ۑ�
		CResult::SetNumPair(m_nNumKill);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �^�C���̉��Z
//-------------------------------------------------------------------------------------------------------------
void CGame::AddTime(int nValue)
{
	m_nCntTime += nValue;

	if (m_nCntTime >= 0)
	{
		// �^�C�}�[�̐������X�V
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->SetValue(m_nCntTime);
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->UpdateNumber();
	}
}

//-------------------------------------------------------------------------------------------------------------
// ���j���̉��Z
//-------------------------------------------------------------------------------------------------------------
void CGame::AddNumKill(int nValue)
{
	m_nNumKill += nValue;

	m_pGameUI[GAMEUI_SCORE]->GetImage().pNumber->SetValue(m_nNumKill);
	m_pGameUI[GAMEUI_SCORE]->GetImage().pNumber->UpdateNumber();
}