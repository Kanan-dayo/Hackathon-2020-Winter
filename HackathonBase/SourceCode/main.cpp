//*************************************************************************************************************
//
// ウィンドウ表示処理[man.icpp]
// Author:Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define CLASS_NAME		"WindowClass"								// ウィンドウクラスの名前
#define WINDOW_NAME		"CyoTotu Mousin"			// キャプション

//-------------------------------------------------------------------------------------------------------------
// プロトタイプ宣言
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void MeasurProcesTime(CManager *g_pManager, DWORD &dwCurrentTime, DWORD &dwFPSLastTime, DWORD &dwFrameCount);
void DispConsol();

//-------------------------------------------------------------------------------------------------------------
// グローバル変数
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
//
// メイン関数
//
//-------------------------------------------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	// randをシャッフル
	srand((unsigned int)time(NULL));

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtSetBreakAlloc(331);
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEXのメモリサイズの指定
		CS_CLASSDC,						// 表示するウィンドウのスタイルを設定
		WindowProc,						// ウィンドウプロシージャのアドレス（関数名）を指定
		0,								// 通常は使用は使用しないので0指定
		0,								// 通常は使用は使用しないので0指定
		hInstance,						// Windowsの引数の院スタンドハンドル指定
		LoadIcon(NULL,IDI_APPLICATION),	// タスクバーに使用するアイコン指定
		LoadCursor(NULL,IDC_ARROW),		// 使用するマウスカーソル指定
		(HBRUSH)(COLOR_WINDOW + 1),		// ウィンドウクライアント領域の色指定
		NULL,							// メニューを指定
		CLASS_NAME,						// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)	// 小さいアイコン指定
	};

	// コンソール画面の表示
	DispConsol();

	HWND hWnd;							// ウィンドウハンドル（識別子）
	MSG  msg;							// メッセージを格納する変数
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);


	// ウインドウを作成
	hWnd = CreateWindowEx(0,		// 拡張ウィンドウスタイル
		CLASS_NAME,					// ウィンドウクラス名
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウスタイル
		CW_USEDEFAULT,				// ウィンドウ左上X座標
		CW_USEDEFAULT,				// ウィンドウ左上Y座標
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューもしくは子ウィンドウのハンドル
		hInstance,					// インスタンスハンドル
		NULL);						// ウィンドウ作成データ

	DWORD dwCurrentTime = NULL;		// 現在時刻
	DWORD dwExecLastTime = NULL;	// 最後に処理した時刻
	DWORD dwFrameCount = NULL;		// 計測用時刻
	DWORD dwFPSLastTime = NULL;		// 保存用時刻

	CManager *pManager = NULL;
	pManager = new CManager;

	// 初期化処理（ウィンドウの生成後に行う）
	if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// ウィンドウの表示
	ShowWindow(hWnd, nShowCmd);			// 指定されたウィンドウの表示設定
	UpdateWindow(hWnd);					// ウィンドウのクライアントの更新

	// 分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// システム時刻をミリ秒単位で取得

	// メッセージループ（メッセージ”キューからメッセージ”を取得
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)// メッセージを取得した場合を返す
		{// Window処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージが送られたら終了
				break;
			}
			else
			{// メッセージの翻訳を送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
			// システム時間の取得処理
			dwCurrentTime = timeGetTime();

			// 処理時間の計測
			MeasurProcesTime(pManager, dwCurrentTime, dwFPSLastTime, dwFrameCount);

			// 処理速度の制限
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 処理した時刻を保存
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				pManager->Update();

				// 描画処理
				pManager->Draw();

				// フレームカウントの更新処理
				dwFrameCount++;
			}
		}
	}
	// 終了処理
	pManager->Uninit();
	delete pManager;
	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	_CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}

//-------------------------------------------------------------------------------------------------------------
// ウィンドウプロシージャ関数
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:						// ウィンドウ破棄メッセージ
		PostQuitMessage(0);					// メッセージを返す
		break;

	case WM_KEYDOWN:						//キーが押された際のメッセージ
		// [ESC]キーが押された
		if (wParam == VK_ESCAPE) {
			//ウィンドウの破棄（WM_DESTROYを発生）
			DestroyWindow(hWnd);
		}
		break;

	default:
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 既定の処理を提供
}

//-------------------------------------------------------------------------------------------------------------
// 処理速度の計測
//-------------------------------------------------------------------------------------------------------------
void MeasurProcesTime(CManager * g_pManager, DWORD & dwCurrentTime, DWORD & dwFPSLastTime, DWORD & dwFrameCount)
{
	// 処理時間の計測
	if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5秒ごとに実行
	{
#ifdef _DEBUG
		// FPSを測定
		g_pManager->SetFPS((dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime));
#endif
		dwFPSLastTime = dwCurrentTime;			// FPSを測定した時刻を保存
		dwFrameCount = 0;						// カウントをクリア
	}
}

//-------------------------------------------------------------------------------------------------------------
// コンソール画面の表示
//-------------------------------------------------------------------------------------------------------------
void DispConsol()
{
#ifdef _DEBUG
	AllocConsole();

	FILE *fp = NULL;

	freopen_s(&fp, "CONOUT$", "w", stdout);

	cout << fixed;
#endif
}
