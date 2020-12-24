//*************************************************************************************************************
//
// ランキング処理 [ranking.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "ranking.h"
#include "texture.h"
#include "2DUI.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define LINK_RANKINGUIINFO	("DATA/TEXT/rankingUIInfo.txt")		// UI情報のあるテキストファイル
#define POS_RANLING_TITLE	(D3DXVECTOR3(640.0f, 120.0f, 0.0f))	// ランキングの座標
#define SIZE_RANLING_TITLE	(D3DXVECTOR2(400.0f, 200.0f))	// ランキングのサイズ

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CRanking::m_posUI[CRanking::RANK_MAX][CRanking::RANKING_MAX]	= {};
D3DXVECTOR2 CRanking::m_sizeUI[CRanking::RANK_MAX][CRanking::RANKING_MAX]	= {};
int			CRanking::m_nNumPair[CRanking::RANK_MAX]						= {};

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CRanking * CRanking::Create(void)
{
	CRanking *pRanking = new CRanking;
	pRanking->Init();
	return pRanking;
}

//-------------------------------------------------------------------------------------------------------------
// UI情報のロード
//-------------------------------------------------------------------------------------------------------------
HRESULT CRanking::LoadUIInfo(void)
{
	// 変数宣言
	FILE *pFile;
	char cReadText[128];
	char cHeadText[128] = "\0";

	// ファイルを開く
	pFile = fopen(LINK_RANKINGUIINFO, "r");

#ifdef _DEBUG
	std::cout << "ランキングUIの情報を取得\n";
#endif

	// 失敗
	if (!pFile)
	{
#ifdef _DEBUG
		std::cout << "ファイルオープンに失敗\n";
#endif
		return E_FAIL;
	}


	// スクリプトがくるまで繰り返す
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);
	}
	// スクリプトが来たら
	if (strcmp(cHeadText, "SCRIPT") == 0)
	{
		int nIndex = 0;
		int nRank = 0;

		// エンドスクリプトがくるまで繰り返す
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			strcpy(cHeadText, "");
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);

			// 改行
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// 番号
			if (strcmp(cHeadText, "Rank") == 0)
				sscanf(cReadText, "Rank = %d", &nRank);
			// 番号
			if (strcmp(cHeadText, "Index") == 0)
				sscanf(cReadText, "	Index = %d", &nIndex);
			// 座標
			if (strcmp(cHeadText, "pos") == 0)
				sscanf(cReadText, "	pos = %f %f", &m_posUI[nRank][nIndex].x, &m_posUI[nRank][nIndex].y);
			// サイズ
			if (strcmp(cHeadText, "size") == 0)
				sscanf(cReadText, "	size = %f %f", &m_sizeUI[nRank][nIndex].x, &m_sizeUI[nRank][nIndex].y);
		}
	}

	// ファイルを閉じる
	fclose(pFile);
	// 成功
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CRanking::Init(void)
{
	LoadUIInfo();

	// 初期設定
	N2Dui_seting setTitle;
	setTitle.bDisp = true;
	setTitle.col = ML_D3DXCOR_SET;
	setTitle.fRotation = ML_FLOAT_UNSET;

	// 順位
	setTitle.nTextureID = CTexture::NAME_RANKING;
	setTitle.pos = POS_RANLING_TITLE;
	setTitle.size = SIZE_RANLING_TITLE;
	m_pUIRaking = C2DUi::Create(setTitle, CScene::PRIORITY_BUI);

	for (int nRank = 0; nRank < RANK_MAX; nRank++)
	{
		// 初期設定
		N2Dui_seting seting;
		seting.bDisp = true;
		seting.col = ML_D3DXCOR_SET;
		seting.fRotation = ML_FLOAT_UNSET;

		// 順位
		seting.nTextureID = CTexture::NAME_RANKING_1st + nRank;
		seting.pos = m_posUI[nRank][RANKING_RANKED];
		seting.size = m_sizeUI[nRank][RANKING_RANKED];
		m_p2DUI[nRank][RANKING_RANKED] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

		// 〇ペア
		seting.nTextureID = CTexture::NAME_RESULT_NUMPAIR;
		seting.pos = m_posUI[nRank][RANKING_NUMPAIR];
		seting.size = m_sizeUI[nRank][RANKING_NUMPAIR];
		m_p2DUI[nRank][RANKING_NUMPAIR] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

		// ペア数
		seting.mask.unMask = N2Dui_mask::E_M_NUMBER;
		seting.nTextureID = CTexture::NAME_NUMBER;
		seting.nValue = m_nNumPair[nRank];
		seting.pos = m_posUI[nRank][RANKING_NUM];
		seting.size = m_sizeUI[nRank][RANKING_NUM];
		m_p2DUI[nRank][RANKING_NUM] = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CRanking::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CRanking::Update(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CRanking::Draw(void)
{
}
