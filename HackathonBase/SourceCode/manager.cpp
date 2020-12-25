//*************************************************************************************************************
//
// マネージャ処理[manager.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "manager.h"
#include "Scene.h"
#include "keyboard.h"
#include "mouse.h"
#include "sound.h"
#include "DebugProc.h"
#include "camera.h"
#include "Light.h"
#include "texture.h"
#include "gamepad.h"
#include "myhash.h"
#include "renderer.h"
#include "mode.h"
#include "title.h"
#include "result.h"
#include "game.h"
#include "ranking.h"
#include "tutorial.h"
#include "3Dparticle.h"
#include "XGamepad.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CRenderer		CManager::m_Renderer                       = ML_INITSTRUCT_WITHCONST;	// レンダラーポインタ
Ckeyboard		CManager::m_Keyboard                       = ML_INITSTRUCT_WITHCONST;	// キーボードのポインタ
CMouse			CManager::m_Mouse                          = ML_INITSTRUCT_WITHCONST;	// マウスのポインタ
CGamepad		CManager::m_Gamepad                        = ML_INITSTRUCT_WITHCONST;	// ゲームパッドのポインタ
CXGamepad		CManager::m_XGamePad                       = ML_INITSTRUCT_WITHCONST;
CSound			CManager::m_Sound                          = ML_INITSTRUCT_WITHCONST;	// サウンドのポインタ
CDebugProc		CManager::m_DebugProc                      = ML_INITSTRUCT_WITHCONST;	// デバッグ処理のポインタ
CCamera			CManager::m_Camera                         = ML_INITSTRUCT_WITHCONST;	// カメラのポインタ
CLight			CManager::m_Light                          = ML_INITSTRUCT_WITHCONST;	// ライトのポインタ
STRING			CManager::m_aFIleName[FILE_NAME::FILE_MAX] = ML_INITSTRUCT_WITHCONST;	// ファイル名(.iniから読みこんだファイル)
CManager::MODE	CManager::m_mode                           = CManager::MODE_NONE;			// モード
int				CManager::m_nMyScore                       = ML_INT_UNSET;				// スコア保存
CHash			CManager::m_Hash                           = ML_INITSTRUCT_WITHCONST;	// ハッシュポインタ
CMode*			CManager::m_pModeClass                     = nullptr;						// モードクラスのポインタ

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CManager::CManager()
{

}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CManager::~CManager()
{

}

//-------------------------------------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------------------------------------
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// 初期化処理（ウィンドウの生成後に行う）
	if (FAILED(m_Renderer.Init(hWnd, bWindow)))
	{
		return -1;
	}

	// ハッシュテーブルの作成
	m_Hash.MakeHashtable();
	// ハッシュの設定
	SetHash();

	// 初期化ファイルの読み込み
	LoadInitFile();

	// キーボードの初期化初期化
	m_Keyboard.Init(hInstance, hWnd);
	// マウスの初期化
	m_Mouse.Init(hInstance, hWnd);

	// ゲームパッドの初期化
	m_Gamepad.Init(hInstance, hWnd);
	m_XGamePad.Init(hInstance, hWnd);
	// サウンドの初期化
	m_Sound.InitSound(hWnd);
	// カメラの初期化
	m_Camera.Init();
	// ライトの生成
	m_Light.Init();

#ifdef _DEBUG
	// デバッグの生成
	m_DebugProc.Init();
#endif // _DEBUG

	// テクスチャの読み込み
	CTexture::Load();
	// パーティクルの読み込み
	C3DParticle::Load();
	// モデルの読み込み
	//CModel::Load();
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 終了処理
//------------------------------------------------------------------------------------------------------------
void CManager::Uninit(void)
{
	SceneAllReleaseAndSoundStop();
	// モデルの開放
	//CModel::Unload();
	// テクスチャの開放
	CTexture::Unload();
	// パーティクルの読み込み
	C3DParticle::Load();
#ifdef _DEBUG
	// デバッグの終了処理
	m_DebugProc.Uninit();
#endif // _DEBUG

	// ライトの終了処理
	m_Light.Uninit();
	// カメラの終了処理
	m_Camera.Uninit();
	// サウンドの終了処理
	m_Sound.UninitSound();
	m_XGamePad.Uninit();
	// ゲームパッドの終了処理
	m_Gamepad.Uninit();
	// マウスの終了処理
	m_Mouse.Uninit();
	// キーボードの終了処理
	m_Keyboard.Uninit();
	// ファイルの開放
	for (int nCntFiileName = 0; nCntFiileName < MANAGER_NUMLOAD; nCntFiileName++)
	{// ファイルの終了処理
		delete[] m_aFIleName[nCntFiileName];
		m_aFIleName[nCntFiileName] = nullptr;
	}
	// ハッシュテーブルの開放
	m_Hash.ReleaseHashtable();
	// レンダラーの終了処理
	m_Renderer.Uninit();
}

//-------------------------------------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------------------------------------
void CManager::Update(void)
{
	// キーボードの更新処理
	m_Keyboard.Update();
	// マウスの更新処理
	m_Mouse.Update();
	// ゲームパッドの更新処理
	m_Gamepad.Update();
	m_XGamePad.Update();
	// カメラの更新処理
	m_Camera.Update();
	// ライトの更新処理
	m_Light.Update();

	if (m_mode != MODE_NONE)
	{
		// モードクラスの更新
		m_pModeClass->Update();
	}

	// レンダラーの更新処理
	m_Renderer.UpDate();
}

//-------------------------------------------------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------------------------------------------------
void CManager::Draw(void)
{
	// モードクラスの更新
	m_pModeClass->Draw();
	// レンダラーの描画処理
	m_Renderer.Draw();
}

//-------------------------------------------------------------------------------------------------------------
// モードの切り替え
//-------------------------------------------------------------------------------------------------------------
void CManager::SetMode(MODE mode)
{
	// カメラの初期化
	m_Camera.Init();
	// ライトの方向を設定
	m_Light.SetDirection();

	// シーンの開放処理とサウンドの停止
	SceneAllReleaseAndSoundStop();

	m_mode = mode;
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pModeClass = CTitle::Create();
		break;
	case MODE_TUTORIAL:
		m_pModeClass = CTutorial::Create();
		break;
	case MODE_GAME:
		m_pModeClass = CGame::Create();
		break;
	case MODE_RESULT:
		m_pModeClass = CResult::Create();
		break;
	case MODE_RANKING:
		m_pModeClass = CRanking::Create();
		break;
	}
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// FPSの設定
//-------------------------------------------------------------------------------------------------------------
void CManager::SetFPS(int fps)
{
	m_Renderer.SetFPS(fps);
}
#endif


//-------------------------------------------------------------------------------------------------------------
// ハッシュの設定
//-------------------------------------------------------------------------------------------------------------
void CManager::SetHash(void)
{
	// 設定用キー
	char SetingKey[FILE_MAX][16] =
	{
		{ "SOUND" },
		{ "TITLEUI" },
		{ "SELECT" },
		{ "TUTORIALUI" },
		{ "GAMEUI" },
		{ "RESULTUI" },
		{ "RANKINGUI" },
		{ "TEXTURE" },
	};
	// 設定用データ
	char SetingData[FILE_MAX][4] =
	{
		{ "0" },
		{ "1" },
		{ "2" },
		{ "3" },
		{ "4" },
		{ "5" },
		{ "6" },
		{ "7" },
	};
	// ファイル最大数分ループ
	for (int nCntHash = 0; nCntHash < FILE_MAX; nCntHash++)
	{// ハッシュに登録する
		m_Hash.Insert(SetingKey[nCntHash], SetingData[nCntHash]);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 初期化ファイルの読み込み
//-------------------------------------------------------------------------------------------------------------
void CManager::LoadInitFile(void)
{
	// ファイルから初期設定を読みこむクラス宣言
	CLoadFile Load;

	// 読み込みファイルの情報を作成
	Load.LoadFileIntoString("DATA/TEXT/GameManager.ini");

	// 文字列から初期設定を読み込む
	Load.SetingfromString(ReadFromLine);

	// ファイルデータの破棄
	Load.DeleteFileData();
}

//-------------------------------------------------------------------------------------------------------------
// 1行から情報を読み取る
//-------------------------------------------------------------------------------------------------------------
void CManager::ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData)
{
	// 変数宣言
	char SetingFileName[ML_STRINGSIZE];	// 設定用ファイル名
	SetingFileName[0] = ML_CHAR_UNSET;

	if (sscanf(cnpLine, "FILENAME = %s", &SetingFileName[0]) == 1)
	{
		// 配列番号の取得
		int nIndex = CMylibrary::GetConvertStoI(m_Hash.Search((char*)cnpEntryData));
		// 文字列の確保と設定
		m_aFIleName[nIndex] = CMylibrary::SetStringAlloc(SetingFileName);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ファイル名
//-------------------------------------------------------------------------------------------------------------
STRING CManager::GetFIleName(CManager::FILE_NAME FileIndex)
{
	return m_aFIleName[FileIndex];
}

//-------------------------------------------------------------------------------------------------------------
// シーンの開放処理とサウンドの停止
//-------------------------------------------------------------------------------------------------------------
void CManager::SceneAllReleaseAndSoundStop(void)
{
	if (m_pModeClass != nullptr)
	{
		m_pModeClass->Uninit();
		delete m_pModeClass;
		m_pModeClass = nullptr;
	}
	// サウンドを停止
	m_Sound.StopSound();
	// 全ての開放処理
	CScene::ReleaseAll();
}
