//*************************************************************************************************************
//
// ゲーム処理 [game.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _GAME_H_
#define _GAME_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CPlayer;
class CGame : public CMode
{
public:

	typedef enum
	{
		MODE_NONE = -1,
		MODE_NORMAL,
		MODE_START,
		MODE_FINISH,
	} MODE;

	typedef enum
	{	// ゲームUIの種類
		GAMEUI_TIMER,	// タイマーUI
		GAMEUI_SCORE,	// スコアUI
		GAMEUI_PAIR,	// 〇ペアUI
		GAMEUI_MAX
	} GAMEUI;

	CGame() {};					// コンストラクタ
	~CGame() {};					// デストラクタ

	static CGame *Create(void);	// 生成
	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

	C2DUi *Get2dui(void) { return m_pC2dui; }
	CPlayer *GetPlayer(void) { return m_pPlayer; }

private:
	void CreateGameUI(void);		// ゲームUIの生成
	void ModeTrans(void);			// モード遷移
	void SetMode(MODE mode);		// モードの設定

	static CONST D3DXVECTOR3 m_InitPosPlayer;
	static CONST D3DXVECTOR2 m_InitSizePlayer;
	C2DUi*   m_pC2dui;				// ２DUIポインタ
	C2DUi*	 m_pGameUI[GAMEUI_MAX];	// ゲームUIポインタ
	CPlayer* m_pPlayer;				// プレイヤーのポインタ[
	MODE     m_mode;				// モード
	UINT     m_nCntMode;			// モードカウント
};

#endif