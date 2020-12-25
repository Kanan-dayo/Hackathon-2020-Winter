//*************************************************************************************************************
//
// �����L���O���� [ranking.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "ranking.h"
#include "texture.h"
#include "2DUI.h"
#include "keyboard.h"
#include "renderer.h"
#include "fade.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define LINK_RANKINGUIINFO	("DATA/TEXT/UIInfo/rankingUIInfo.txt")		// UI���̂���e�L�X�g�t�@�C��
#define LINK_RANKING		("DATA/SAVEDATA/ranking.txt")		// �����L���O�̃t�@�C��
#define POS_RANLING_TITLE	(D3DXVECTOR3(640.0f, 120.0f, 0.0f))	// �����L���O�̍��W
#define SIZE_RANLING_TITLE	(D3DXVECTOR2(400.0f, 200.0f))		// �����L���O�̃T�C�Y

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CRanking::m_posUI[CRanking::RANK_MAX][CRanking::RANKING_MAX]	= {};
D3DXVECTOR2 CRanking::m_sizeUI[CRanking::RANK_MAX][CRanking::RANKING_MAX]	= {};
int			CRanking::m_nNumPair[CRanking::RANK_MAX]						= {};
int			CRanking::m_nPlayerScore										= 0;
int			CRanking::m_nPlayerRank											= CRanking::RANK_MAX;
bool		CRanking::m_bSort												= false;

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CRanking * CRanking::Create(void)
{
	CRanking *pRanking = new CRanking;
	pRanking->Init();
	return pRanking;
}

//-------------------------------------------------------------------------------------------------------------
// UI���̃��[�h
//-------------------------------------------------------------------------------------------------------------
HRESULT CRanking::LoadUIInfo(void)
{
	// �ϐ��錾
	FILE *pFile;
	char cReadText[128];
	char cHeadText[128] = "\0";

	// �t�@�C�����J��
	pFile = fopen(LINK_RANKINGUIINFO, "r");

#ifdef _DEBUG
	std::cout << "�����L���OUI�̏����擾\n";
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
		int nRank = 0;

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
			if (strcmp(cHeadText, "Rank") == 0)
				sscanf(cReadText, "Rank = %d", &nRank);
			// �ԍ�
			if (strcmp(cHeadText, "Index") == 0)
				sscanf(cReadText, "	Index = %d", &nIndex);
			// ���W
			if (strcmp(cHeadText, "pos") == 0)
				sscanf(cReadText, "	pos = %f %f", &m_posUI[nRank][nIndex].x, &m_posUI[nRank][nIndex].y);
			// �T�C�Y
			if (strcmp(cHeadText, "size") == 0)
				sscanf(cReadText, "	size = %f %f", &m_sizeUI[nRank][nIndex].x, &m_sizeUI[nRank][nIndex].y);
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
// �����L���O�̃��[�h
//-------------------------------------------------------------------------------------------------------------
HRESULT CRanking::LoadRank(void)
{
	// �ϐ��錾
	FILE *pFile;
	char cReadText[128];
	char cHeadText[128] = "\0";

	// �t�@�C�����J��
	pFile = fopen(LINK_RANKING, "r");

#ifdef _DEBUG
	std::cout << "�����L���O�̏����擾\n";
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
		// �G���h�X�N���v�g������܂ŌJ��Ԃ�
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			strcpy(cHeadText, "");
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);

			// ���s
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// �X�R�A
			if (strcmp(cHeadText, "score") == 0)
				sscanf(cReadText, "score = %d %d %d %d %d", 
					&m_nNumPair[0], &m_nNumPair[1], &m_nNumPair[2], &m_nNumPair[3], &m_nNumPair[4]);
		}
	}

	// �t�@�C�������
	fclose(pFile);

#ifdef _DEBUG
	std::cout << "�����L���O�̏��擾����\n";
#endif

	// ����
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CRanking::Init(void)
{
	// UI�̃��[�h
	LoadUIInfo();
	// �����L���O�̃��[�h
	LoadRank();

	// �����ݒ�
	N2Dui_seting setTitle;
	setTitle.bDisp = true;
	setTitle.col = ML_D3DXCOR_SET;
	setTitle.fRotation = ML_FLOAT_UNSET;

	// ����
	setTitle.nTextureID = CTexture::NAME_RANKING;
	setTitle.pos = POS_RANLING_TITLE;
	setTitle.size = SIZE_RANLING_TITLE;
	m_pUIRaking = C2DUi::Create(setTitle, CScene::PRIORITY_BUI);

	// �X�R�A���\�[�g
	SortScore();

	for (int nRank = 0; nRank < RANK_MAX; nRank++)
	{
		// �����ݒ�
		N2Dui_seting seting;
		seting.bDisp = true;
		seting.col = ML_D3DXCOR_SET;
		seting.fRotation = ML_FLOAT_UNSET;

		// ����
		seting.nTextureID = CTexture::NAME_RANKING_1st + nRank;
		seting.pos = m_posUI[nRank][RANKING_RANKED];
		seting.size = m_sizeUI[nRank][RANKING_RANKED];
		m_p2DUI[nRank][RANKING_RANKED] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

		// �Z�y�A
		seting.nTextureID = CTexture::NAME_RESULT_NUMPAIR;
		seting.pos = m_posUI[nRank][RANKING_NUMPAIR];
		seting.size = m_sizeUI[nRank][RANKING_NUMPAIR];
		m_p2DUI[nRank][RANKING_NUMPAIR] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

		// �y�A��
		(m_nPlayerRank == nRank && m_bSort) ?
			seting.mask.unMask = N2Dui_mask::E_M_FLASHING | N2Dui_mask::E_M_NUMBER :
			seting.mask.unMask = N2Dui_mask::E_M_NUMBER;
		seting.nTextureID = CTexture::NAME_NUMBER;
		seting.nValue = m_nNumPair[nRank];
		seting.pos = m_posUI[nRank][RANKING_NUM];
		seting.size = m_sizeUI[nRank][RANKING_NUM];
		m_p2DUI[nRank][RANKING_NUM] = C2DUi::Create(seting, CScene::PRIORITY_BUI);
		m_p2DUI[nRank][RANKING_NUM]->SetFlashingAbility(N2Dui_flashing(true, 2));
	}
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CRanking::Uninit(void)
{
	// �����N�C�����Ă���΁A�����L���O�̃Z�[�u
	if (m_nPlayerRank < CRanking::RANK_MAX)
		SaveRanking();

	m_bSort = false;
}

//-------------------------------------------------------------------------------------------------------------
// �����L���O�̃Z�[�u
//-------------------------------------------------------------------------------------------------------------
HRESULT CRanking::SaveRanking(void)
{
	// �e�L�X�g�t�@�C�������₷������R�����g
	char cComment0[8] = "\n";
	char cWriteText[128];

	// �ϐ��錾
	FILE *pFile = fopen(LINK_RANKING, "w");

#ifdef _DEBUG
	std::cout << "�����L���O�̏���ۑ�\n";
#endif

	// ���s
	if (!pFile)
	{
#ifdef _DEBUG
		std::cout << "�t�@�C���I�[�v���Ɏ��s\n";
#endif
		return E_FAIL;
	}

	// �R�����g�ݒ�
	strcpy(cWriteText, "SCRIPT\n");
	fputs(cWriteText, pFile);															// SCRIPT
	fputs(cComment0, pFile);															// \n

	// �R�����g�ݒ�
	sprintf(cWriteText, "score = %d %d %d %d %d\n",
		m_nNumPair[0], m_nNumPair[1], m_nNumPair[2], m_nNumPair[3], m_nNumPair[4]);		// score = 
	fputs(cWriteText, pFile);

	fputs(cComment0, pFile);															// \n

	// �R�����g�ݒ�
	strcpy(cWriteText, "END_SCRIPT\n");
	fputs(cWriteText, pFile);															// END_SCRIPT

	// �t�@�C�������
	fclose(pFile);

	// ����
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CRanking::Update(void)
{
	if (CManager::GetKeyboard().GetTrigger(DIK_RETURN))
	{
		if (CManager::GetRenderer().GetFade()->GetFadeState() == CFade::FADE_NONE)
		{
			CManager::GetRenderer().GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CRanking::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�R�A�̃\�[�g
//-------------------------------------------------------------------------------------------------------------
void CRanking::SortScore(void)
{
	// �v���C���[�X�R�A�����O�Ȃ�A�������Ȃ�
	if (m_nNumPair[4] > m_nPlayerScore || 
		!m_bSort)
		return;

	// �Œ�X�R�A���v���C���[�̂��̂ɏ�������
	m_nNumPair[4] = m_nPlayerScore;

	// �����L���O�̓���ւ�
	for (int nCount = 0; nCount < CRanking::RANK_MAX - 1; nCount++)
	{
		for (int nCount2 = nCount + 1; nCount2 < CRanking::RANK_MAX; nCount2++)
		{
			// �X�R�A�������Ă���
			if (m_nNumPair[nCount] < m_nNumPair[nCount2])
			{
				// ���ʂ̓���ւ�
				int nData = m_nNumPair[nCount2];
				m_nNumPair[nCount2] = m_nNumPair[nCount];
				m_nNumPair[nCount] = nData;
			}
		}
	}

	// �v���C���[�����N��ݒ�
	for (int nScore = 0; nScore < CRanking::RANK_MAX; nScore++)
	{
		if (m_nNumPair[nScore] == m_nPlayerScore)
			m_nPlayerRank = nScore;
	}
}
