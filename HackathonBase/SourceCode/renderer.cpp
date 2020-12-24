//*************************************************************************************************************
//
// レンダラー処理[renderer.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "renderer.h"
#include "Scene.h"
#include "fade.h"
#include "manager.h"
#include "DebugProc.h"
#include "camera.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CFade *CRenderer::m_pFade = nullptr;			// フェード

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CRenderer::CRenderer()
{

}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CRenderer::~CRenderer()
{

}

//-------------------------------------------------------------------------------------------------------------
// 初期化処理
//-------------------------------------------------------------------------------------------------------------
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// 変数宣言
	D3DDISPLAYMODE			d3ddm;				// ディスプレイモード
	D3DPRESENT_PARAMETERS	d3dpp;				// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL) {
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;							// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							// バックバッファの形式
	d3dpp.BackBufferCount = 1;										// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							// デプスバッファ(Ｚバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;										// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート(現在の速度に合わせる)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		// インターバル(VSyncを待って描画)
																	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	// クライアント領域を直ちに更新する


	// Direct3Dデバイスの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,				// ディスプレイアダプタ
									D3DDEVTYPE_HAL,												// デバイスタイプ
									hWnd,														// フォーカスするウインドウへのハンドル
									D3DCREATE_HARDWARE_VERTEXPROCESSING,						// デバイス作成制御の組み合わせ
									&d3dpp,														// デバイスのプレゼンテーションパラメータ
									&m_pD3DDevice)))											// デバイスインターフェースへのポインタ
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp,
										&m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp,
											&m_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートパラメータの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定


	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

	CFade::Load();
	m_pFade = CFade::Create();
	m_pFade->Init(CManager::MODE_TITLE);
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 終了処理
//-------------------------------------------------------------------------------------------------------------
void CRenderer::Uninit(void)
{
	CScene::ReleaseAll();

	CFade::Unload();
	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	if (m_pD3DDevice != NULL)
	{// Direct3Dデバイスの開放
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	if (m_pD3D != NULL)
	{
		// Direct3Dオブジェクトの開放
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------------------------------------
void CRenderer::UpDate(void)
{
#ifdef _DEBUG
	CDebugProc::print("FPS:%d\n", m_nCountFPS);
	int nNumAll = CScene::GetNumAll();
	CDebugProc::print("NumAll:%d\n", nNumAll);
#endif // DEBUG

	m_pFade->Update();
	CScene::UpdataAll();
}

//-------------------------------------------------------------------------------------------------------------
// 描画処理
//-------------------------------------------------------------------------------------------------------------
void CRenderer::Draw(void)
{
	// バックバッファ＆Zバッファのクリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER)
		, D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// DirectX3Dによる描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		CManager::GetCamera().Set();
		CScene::DrawAll();

		m_pFade->Draw();
#ifdef _DEBUG
		CDebugProc::Draw();
#endif // DEBUG
		// Direct3Dによる描画終了
		m_pD3DDevice->EndScene();
	}

	// バッフバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}
