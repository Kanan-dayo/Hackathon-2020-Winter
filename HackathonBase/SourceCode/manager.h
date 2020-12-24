//*************************************************************************************************************
//
// マネージャ処理[manager.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#ifndef TRANSITIONKEYBOARD
#define TRANSITIONKEYBOARD (DIK_RETURN)
#endif

#define MANAGER_NUMLOAD		(CManager::FILE_NAME::FILE_MAX)		// 読み込むファイル数個数

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CSound;
class CMouse;
class Ckeyboard;
class CDebugProc;
class CRenderer;
class CCamera;
class CLight;
class CGamepad;
class CHash;
class CMode;
class CManager
{
public:
	typedef enum
	{
		FILE_NONE = -1,		// 無し
		FILE_SOUND,			// サウンド
		FILE_TITLEUI,		// タイトルUI
		FILE_SELECTUI,		// 選択モードUI
		FILE_TUTORIALUI,	// チュートリアルUI
		FILE_GAMEUI,		// ゲームUI
		FILE_RESULTUI,		// リザルトUI
		FILE_RankingUI,		// ランキングUI
		FILE_TEXTURE,		// テクスチャ
		FILE_MAX			// 最大数
	} FILE_NAME;

	typedef enum
	{
		MODE_NONE = 0,		// 無し
		MODE_TITLE,			// タイトル
		MODE_TUTORIAL,		// チュートリアル
		MODE_GAME,			// ゲーム
		MODE_RESULT,		// リザルト
		MODE_RANKING,		// ランキング
		MODE_MAX			// 最大
	} MODE;

	/* -- メンバ関数 -- */
	CManager();																								// コンストラクタ
	~CManager();																							// デストラクタ
	static HRESULT Init(HINSTANCE hInstance ,HWND hWnd, BOOL bWindow);										// 初期化
	static void Uninit(void);																				// 終了
	static void Update(void);																				// 更新
	static void Draw(void);																					// 描画
	static void SetHash(void);																				// ハッシュを設定
	static void LoadInitFile(void);																			// 初期化ファイルの読み込み
	static void ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);	// 1行から情報を読み取る
	static STRING GetFIleName(CManager::FILE_NAME FileIndex);												// ファイル名の取得
	static void SceneAllReleaseAndSoundStop(void);															// シーンの開放処理とサウンドの停止
	static void SetMode(MODE mode);																			// モードの設定

	/*　インライン関数 */
	static void       SetTotalScore(int nMyScore) { m_nMyScore = nMyScore; }	// トータルスコアの設定
	static MODE&      GetMode(void)               { return m_mode; }			// モードの取得
	static CRenderer& GetRenderer(void)           { return m_Renderer; }		// レンダラーの取得
	static Ckeyboard& GetKeyboard(void)           { return m_Keyboard; }		// キーボードの取得
	static CGamepad&  GetGamepad(void)            { return m_Gamepad; }			// ゲームパッドの取得
	static CMouse&    GetMouse(void)              { return m_Mouse; }			// マウスの取得
	static CCamera&   GetCamera(void)             { return m_Camera; }			// カメラの取得
	static int&       GetTotalScore(void)         { return m_nMyScore; }		// トータルスコアの取得
	static CLight&    GetLight(void)              { return m_Light; }			// ライトの取得
	static CSound&    GetSound(void)              { return m_Sound; }			// サウンドの取得
	static CMode&     GetModeClass(void)          {return *m_pModeClass; }		// モードクラスの取得
	static CMode*     GetModeClassPtr(void)       { return m_pModeClass; }				// モードクラスのポインタの取得
#ifdef _DEBUG
	void SetFPS(int nFps);												// FPSの設定
#endif
private:
	/* -- メンバ変数 -- */
	static CRenderer	m_Renderer;						// レンダラーポインタ
	static Ckeyboard	m_Keyboard;						// キーボードのポインタ
	static CMouse		m_Mouse;							// マウスのポインタ
	static CGamepad		m_Gamepad;						// ゲームパッドのポインタ
	static CSound		m_Sound;							// サウンドのポインタ
	static CDebugProc	m_DebugProc;						// デバッグ処理のポインタ
	static CCamera		m_Camera;							// カメラのポインタ
	static CLight		m_Light;							// ライトのポインタ
	static STRING		m_aFIleName[FILE_NAME::FILE_MAX];	// ファイル名(.iniから読みこんだファイル)
	static MODE			m_mode;								// モード
	static int			m_nMyScore;							// スコア保存
	static CHash		m_Hash;							// ハッシュポインタ
	static CMode*		m_pModeClass;					// モードクラスのポインタ
protected:
};

#endif