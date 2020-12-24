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

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define LINK_RANKINGUIINFO	("DATA/TEXT/rankingUIInfo.txt")		// UI���̂���e�L�X�g�t�@�C��
#define POS_RANLING_TITLE	(D3DXVECTOR3(640.0f, 120.0f, 0.0f))	// �����L���O�̍��W
#define SIZE_RANLING_TITLE	(D3DXVECTOR2(400.0f, 200.0f))	// �����L���O�̃T�C�Y

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CRanking::m_posUI[CRanking::RANK_MAX][CRanking::RANKING_MAX]	= {};
D3DXVECTOR2 CRanking::m_sizeUI[CRanking::RANK_MAX][CRanking::RANKING_MAX]	= {};
int			CRanking::m_nNumPair[CRanking::RANK_MAX]						= {};

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
	// ����
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CRanking::Init(void)
{
	LoadUIInfo();

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
		seting.mask.unMask = N2Dui_mask::E_M_NUMBER;
		seting.nTextureID = CTexture::NAME_NUMBER;
		seting.nValue = m_nNumPair[nRank];
		seting.pos = m_posUI[nRank][RANKING_NUM];
		seting.size = m_sizeUI[nRank][RANKING_NUM];
		m_p2DUI[nRank][RANKING_NUM] = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CRanking::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CRanking::Update(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CRanking::Draw(void)
{
}
