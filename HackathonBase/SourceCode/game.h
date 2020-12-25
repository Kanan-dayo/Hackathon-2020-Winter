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
class CEnemyMana;
class CGame : public CMode
{
public:

	typedef enum
	{
		MODE_NONE = -1,	// 何もない
		MODE_BEGIN,		// 開始時
		MODE_READY,		// レディ
		MODE_GO,		// ゴー
		MODE_NORMAL,	// 通常
		MODE_START,		//
		MODE_FINISH,	// フィニッシュ
		MODE_END,		// ゲーム終了
	} MODE;

	typedef enum
	{	// ゲームUIの種類
		GAMEUI_TIMER,	// タイマーUI
		GAMEUI_SCORE,	// スコアUI
		GAMEUI_PAIR,	// 〇ペアUI
		GAMEUI_READY,	// レディ
		GAMEUI_GO,		// ゴー
		GAMEUI_FINISH,	// フィニッシュ
		GAMEUI_MAX
	} GAMEUI;

	CGame() {};					// コンストラクタ
	~CGame() {};					// デストラクタ

	static CGame *Create(void);	// 生成
	static HRESULT LoadUIInfo(void);	// UI情報のロード
	static void AddNumKill(int nValue);	// 撃破数を加算
	static void AddTime(int nValue);	// タイムを加算
	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画
	MODE GetGameMode(void) { return m_mode; }
	C2DUi *GetGameUI(UINT nIndex) { return m_pGameUI[nIndex]; }
	CPlayer *GetPlayer(void) { return m_pPlayer; }
	CEnemyMana * GetEnemyMana(void) { return m_pEnemyMana; }
private:
	void CreateGameUI(void);		// ゲームUIの生成
	void ModeTrans(void);			// モード遷移
	void SetMode(MODE mode);		// モードの設定
	void UpdateTimer(void);			// タイマーの更新
	void ModeBegin(void);			// 開始時
	void ModeReady(void);			// レディ時
	void ModeGo(void);				// ゴー時
	void ModeNormal(void);			// 通常時
	void ModeFinish(void);			// フィニッシュ時

	static CONST D3DXVECTOR3 m_InitPosPlayer;
	static CONST D3DXVECTOR2 m_InitSizePlayer;
	static D3DXVECTOR3 m_posUI[GAMEUI_MAX];		// UI座標
	static D3DXVECTOR2 m_sizeUI[GAMEUI_MAX];	// UIサイズ
	static int m_nNumKill;						// 倒した敵数
	static int m_nCntTime;						// 残りタイム
	static C2DUi* m_pGameUI[GAMEUI_MAX];		// ゲームUIポインタ
	CEnemyMana * m_pEnemyMana;					// 敵マネージャーのポインタ
	CPlayer* m_pPlayer;							// プレイヤーのポインタ[
	MODE     m_mode;							// モード
	UINT     m_nCntMode;						// モードカウント
	int		 m_nCntFrame;						// フレームカウント
};

#endif