//*************************************************************************************************************
//
// ゲーム処理 [game.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
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

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define LINK_GAMEUIINFO	("DATA/TEXT/UIInfo/gameUIInfo.txt")		// UI情報のあるテキストファイル
#define TIME_DEFAULT	(60)									// 最初のタイム

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CONST D3DXVECTOR3	CGame::m_InitPosPlayer				= D3DXVECTOR3(640.0f, 360.0f, 0.0f);
CONST D3DXVECTOR2	CGame::m_InitSizePlayer				= D3DXVECTOR2(60.0f, 60.0f);
D3DXVECTOR3			CGame::m_posUI[CGame::GAMEUI_MAX]	= {};
D3DXVECTOR2			CGame::m_sizeUI[CGame::GAMEUI_MAX]	= {};
C2DUi *				CGame::m_pGameUI[CGame::GAMEUI_MAX] = {};
int					CGame::m_nNumKill					= 0;
int					CGame::m_nCntTime					= TIME_DEFAULT;

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CGame * CGame::Create(void)
{
	CGame *pGame = new CGame;
	pGame->Init();
	return pGame;
}

//-------------------------------------------------------------------------------------------------------------
// UI情報のロード
//-------------------------------------------------------------------------------------------------------------
HRESULT CGame::LoadUIInfo(void)
{
	// 変数宣言
	FILE *pFile;
	char cReadText[128];
	char cHeadText[128] = "\0";

	// ファイルを開く
	pFile = fopen(LINK_GAMEUIINFO, "r");

#ifdef _DEBUG
	std::cout << "ゲームUIの情報を取得\n";
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
			if (strcmp(cHeadText, "Index") == 0)
				sscanf(cReadText, "Index = %d", &nIndex);
			// 座標
			if (strcmp(cHeadText, "pos") == 0)
				sscanf(cReadText, "pos = %f %f", &m_posUI[nIndex].x, &m_posUI[nIndex].y);
			// サイズ
			if (strcmp(cHeadText, "size") == 0)
				sscanf(cReadText, "size = %f %f", &m_sizeUI[nIndex].x, &m_sizeUI[nIndex].y);
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
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CGame::Init(void)
{
	m_nNumKill = 0;
	m_nCntFrame = 0;
	m_nCntTime = TIME_DEFAULT;

	// UI情報のロード
	LoadUIInfo();
	// ゲームUIの生成
	CreateGameUI();

	// 敵マネージャーの生成
	m_pEnemyMana = CEnemyMana::Create();
	m_mode = MODE_NONE;
	m_nCntMode = ML_INT_UNSET;

	m_pPlayer = CPlayer::Create(m_InitPosPlayer, m_InitSizePlayer);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
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
// 更新
//-------------------------------------------------------------------------------------------------------------
void CGame::Update(void)
{
	// タイマーの更新
	UpdateTimer();
	// モードの遷移
	ModeTrans();
	// マネージャーの更新
	m_pEnemyMana->Update();
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CGame::Draw(void)
{
	// マネージャーの描画
	m_pEnemyMana->Draw();
}

//-------------------------------------------------------------------------------------------------------------
// ゲームUIの生成
//-------------------------------------------------------------------------------------------------------------
void CGame::CreateGameUI(void)
{
	N2Dui_seting seting;
	seting.bDisp = true;
	seting.col = ML_D3DXCOR_SET;
	seting.fRotation = ML_FLOAT_UNSET;

	// ペア数
	seting.nTextureID = CTexture::NAME_PAIR;
	seting.pos = m_posUI[GAMEUI_PAIR];
	seting.size = m_sizeUI[GAMEUI_PAIR];
	m_pGameUI[GAMEUI_PAIR] = C2DUi::Create(seting, CScene::PRIORITY_BUI);

	// スコア
	seting.mask.unMask = N2Dui_mask::E_M_NUMBER;
	seting.nTextureID = CTexture::NAME_NUMBER;
	seting.nValue = 100;
	seting.pos = m_posUI[GAMEUI_SCORE];
	seting.size = m_sizeUI[GAMEUI_SCORE];
	m_pGameUI[GAMEUI_SCORE] = C2DUi::Create(seting, CScene::PRIORITY_BUI);
	m_pGameUI[GAMEUI_SCORE]->GetImage().pNumber->SetValue(0);
	m_pGameUI[GAMEUI_SCORE]->GetImage().pNumber->UpdateNumber();

	// タイマー
	seting.mask.unMask = N2Dui_mask::E_M_NUMBER;
	seting.nTextureID = CTexture::NAME_NUMBER;
	seting.nValue = m_nCntTime;
	seting.pos = m_posUI[GAMEUI_TIMER];
	seting.size = m_sizeUI[GAMEUI_TIMER];
	m_pGameUI[GAMEUI_TIMER] = C2DUi::Create(seting, CScene::PRIORITY_BUI);
}

//-------------------------------------------------------------------------------------------------------------
// モード遷移
//-------------------------------------------------------------------------------------------------------------
void CGame::ModeTrans(void)
{
	if (m_nCntTime <= 0)
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
// モードの設定
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
// タイマーの更新
//-------------------------------------------------------------------------------------------------------------
void CGame::UpdateTimer(void)
{
	m_nCntFrame++;

	// カラー更新
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
	
	// 1秒経過
	m_nCntFrame = 0;
	if (m_nCntTime > 0)
	{
		m_nCntTime--;
		// タイマーの数字を更新
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->SetValue(m_nCntTime);
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->UpdateNumber();
	}
}

//-------------------------------------------------------------------------------------------------------------
// タイムの加算
//-------------------------------------------------------------------------------------------------------------
void CGame::AddTime(int nValue)
{
	m_nCntTime += nValue;

	if (m_nCntTime >= 0)
	{
		// タイマーの数字を更新
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->SetValue(m_nCntTime);
		m_pGameUI[GAMEUI_TIMER]->GetImage().pNumber->UpdateNumber();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 撃破数の加算
//-------------------------------------------------------------------------------------------------------------
void CGame::AddNumKill(int nValue)
{
	m_nNumKill += nValue;

	m_pGameUI[GAMEUI_SCORE]->GetImage().pNumber->SetValue(m_nNumKill);
	m_pGameUI[GAMEUI_SCORE]->GetImage().pNumber->UpdateNumber();
}