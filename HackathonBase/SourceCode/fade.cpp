//*************************************************************************************************************
//
// フェードの処理[fade.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "fade.h"
#include "renderer.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CFade::m_pTexture = nullptr;
LPDIRECT3DVERTEXBUFFER9 CFade::m_pVtxBuff = nullptr;

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CFade::CFade()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CFade::~CFade()
{
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CFade::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// テクスチャの読み込み		
	if (FAILED(D3DXCreateTextureFromFile(pDevice, "", &m_pTexture)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CFade::Unload(void)
{
	// テクスチャの開放
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CFade::Init(CManager::MODE mode)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(CRenderer::VERTEX_2D) * 4,	// 必要な頂点数
		D3DUSAGE_WRITEONLY, FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	m_fadeState = FADE_OUT;
	m_modeNext = mode;
	m_col = { 0.0f,0.0f,0.0f,1.0f };
	m_pos = { 0.0f,0.0f,0.0f };

	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CFade::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CFade::Update(void)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;
	// 何かしている状態の時
	if (m_fadeState != FADE_NONE)
	{
		// フェードインしているとき
		if (m_fadeState == FADE_IN)
		{
			m_col.a -= 0.05f;
			if (m_col.a <= 0.0f)
			{
				m_fadeState = FADE_NONE;
				m_col.a = 0.0f;
			}
			//m_pos.x += 21.0f;
			//if (m_pos.x > 1280.0f)
			//{
			//	// フェード終了
			//	m_pos.x = 1280.0f;
			//	m_fadeState = FADE_NONE;
			//}
		}
		// フェードアウトしている時
		else if (m_fadeState == FADE_OUT)
		{
			m_col.a += 0.05f;
			if (m_col.a >= 1.0f)
			{
				m_col.a = 1.0f;
				m_fadeState = FADE_IN;
				CManager::SetMode(m_modeNext);
			}
			//m_pos.x += 20.0f;
			//if (m_pos.x > 0.0f)
			//{
			//	// フェード終了
			//	m_pos.x = 0.0f;
			//	m_fadeState = FADE_IN;
			//	CManager::SetMode(m_modeNext);
			//}
		}
		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点カラーの設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		//pVtx[0].pos = D3DXVECTOR3(m_pos.x, 0.0f, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(m_pos.x + SCREEN_WIDTH, 0.0f, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(m_pos.x, SCREEN_HEIGHT, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(m_pos.x + SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		// 頂点データをアンロックする
		m_pVtxBuff->Unlock();
	}

}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CFade::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();
	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CRenderer::VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CFade * CFade::Create(void)
{
	CFade *pFade = new CFade;
	return pFade;
}

//-------------------------------------------------------------------------------------------------------------
// フェード設定
//-------------------------------------------------------------------------------------------------------------
bool CFade::SetFade(CManager::MODE mode)
{
	if (m_fadeState == FADE_NONE)
	{
		m_fadeState = FADE_OUT;
		m_modeNext = mode;
		m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	// 黒い画面（透明)
		//m_pos = D3DXVECTOR3(-1280.0f, 0.0f, 0.0f);			// 位置
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// フェード状態の取得
//-------------------------------------------------------------------------------------------------------------
CFade::FADE CFade::GetFadeState(void)
{
	return m_fadeState;
}
