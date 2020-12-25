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
#include "keyboard.h"
#include "renderer.h"
#include "fade.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define LINK_RANKINGUIINFO	("DATA/TEXT/UIInfo/rankingUIInfo.txt")		// UI情報のあるテキストファイル
#define LINK_RANKING		("DATA/SAVEDATA/ranking.txt")		// ランキングのファイル
#define POS_RANLING_TITLE	(D3DXVECTOR3(640.0f, 120.0f, 0.0f))	// ランキングの座標
#define SIZE_RANLING_TITLE	(D3DXVECTOR2(400.0f, 200.0f))		// ランキングのサイズ

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CRanking::m_posUI[CRanking::RANK_MAX][CRanking::RANKING_MAX]	= {};
D3DXVECTOR2 CRanking::m_sizeUI[CRanking::RANK_MAX][CRanking::RANKING_MAX]	= {};
int			CRanking::m_nNumPair[CRanking::RANK_MAX]						= {};
int			CRanking::m_nPlayerScore										= 0;
int			CRanking::m_nPlayerRank											= CRanking::RANK_MAX;
bool		CRanking::m_bSort												= false;

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

#ifdef _DEBUG
	std::cout << "ランキングUIの情報取得完了\n";
#endif

	// 成功
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// ランキングのロード
//-------------------------------------------------------------------------------------------------------------
HRESULT CRanking::LoadRank(void)
{
	// 変数宣言
	FILE *pFile;
	char cReadText[128];
	char cHeadText[128] = "\0";

	// ファイルを開く
	pFile = fopen(LINK_RANKING, "r");

#ifdef _DEBUG
	std::cout << "ランキングの情報を取得\n";
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
		// エンドスクリプトがくるまで繰り返す
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			strcpy(cHeadText, "");
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);

			// 改行
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// スコア
			if (strcmp(cHeadText, "score") == 0)
				sscanf(cReadText, "score = %d %d %d %d %d", 
					&m_nNumPair[0], &m_nNumPair[1], &m_nNumPair[2], &m_nNumPair[3], &m_nNumPair[4]);
		}
	}

	// ファイルを閉じる
	fclose(pFile);

#ifdef _DEBUG
	std::cout << "ランキングの情報取得完了\n";
#endif

	// 成功
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CRanking::Init(void)
{
	// UIのロード
	LoadUIInfo();
	// ランキングのロード
	LoadRank();

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

	// スコアをソート
	SortScore();

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
// 終了
//-------------------------------------------------------------------------------------------------------------
void CRanking::Uninit(void)
{
	// ランクインしていれば、ランキングのセーブ
	if (m_nPlayerRank < CRanking::RANK_MAX)
		SaveRanking();

	m_bSort = false;
}

//-------------------------------------------------------------------------------------------------------------
// ランキングのセーブ
//-------------------------------------------------------------------------------------------------------------
HRESULT CRanking::SaveRanking(void)
{
	// テキストファイルを見やすくするコメント
	char cComment0[8] = "\n";
	char cWriteText[128];

	// 変数宣言
	FILE *pFile = fopen(LINK_RANKING, "w");

#ifdef _DEBUG
	std::cout << "ランキングの情報を保存\n";
#endif

	// 失敗
	if (!pFile)
	{
#ifdef _DEBUG
		std::cout << "ファイルオープンに失敗\n";
#endif
		return E_FAIL;
	}

	// コメント設定
	strcpy(cWriteText, "SCRIPT\n");
	fputs(cWriteText, pFile);															// SCRIPT
	fputs(cComment0, pFile);															// \n

	// コメント設定
	sprintf(cWriteText, "score = %d %d %d %d %d\n",
		m_nNumPair[0], m_nNumPair[1], m_nNumPair[2], m_nNumPair[3], m_nNumPair[4]);		// score = 
	fputs(cWriteText, pFile);

	fputs(cComment0, pFile);															// \n

	// コメント設定
	strcpy(cWriteText, "END_SCRIPT\n");
	fputs(cWriteText, pFile);															// END_SCRIPT

	// ファイルを閉じる
	fclose(pFile);

	// 成功
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 更新
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
// 描画
//-------------------------------------------------------------------------------------------------------------
void CRanking::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// スコアのソート
//-------------------------------------------------------------------------------------------------------------
void CRanking::SortScore(void)
{
	// プレイヤースコアが圏外なら、処理しない
	if (m_nNumPair[4] > m_nPlayerScore || 
		!m_bSort)
		return;

	// 最低スコアをプレイヤーのものに書き換え
	m_nNumPair[4] = m_nPlayerScore;

	// ランキングの入れ替え
	for (int nCount = 0; nCount < CRanking::RANK_MAX - 1; nCount++)
	{
		for (int nCount2 = nCount + 1; nCount2 < CRanking::RANK_MAX; nCount2++)
		{
			// スコアが上回っている
			if (m_nNumPair[nCount] < m_nNumPair[nCount2])
			{
				// 順位の入れ替え
				int nData = m_nNumPair[nCount2];
				m_nNumPair[nCount2] = m_nNumPair[nCount];
				m_nNumPair[nCount] = nData;
			}
		}
	}

	// プレイヤーランクを設定
	for (int nScore = 0; nScore < CRanking::RANK_MAX; nScore++)
	{
		if (m_nNumPair[nScore] == m_nPlayerScore)
			m_nPlayerRank = nScore;
	}
}
