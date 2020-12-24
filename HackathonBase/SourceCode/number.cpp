//*************************************************************************************************************
//
// 数字処理 [number.cpp]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "number.h"
#include "manager.h"
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
const float CNumber::m_fHalfPi    = D3DX_PI * ML_HALF_SIZE;		// 円周率の半分の値
const float CNumber::m_fMinHalfPi = D3DX_PI * -ML_HALF_SIZE;		// 円周率の半分の負の値

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumber::CNumber(int nNumOrder)
{

}

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumber::CNumber()
{

}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumber::~CNumber()
{

}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CNumber::Load(void)
{
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CNumber::Unload(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 作成
//-------------------------------------------------------------------------------------------------------------
CNumber * CNumber::Create(float fSizeX, float fSizeY, D3DXVECTOR3 pos, float fRot, int nNumber, D3DXCOLOR col)
{
	// ナンバーの生成
	CNumber *pNumber = new CNumber;
	// 初期化
	pNumber->Init(fSizeX, fSizeY, nNumber, fRot, pos ,col);
	return pNumber;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
HRESULT CNumber::Init(float fSizeX, float fSizeY, int nNumber, float fRot, D3DXVECTOR3 pos, D3DXCOLOR col)
{
	m_flagbits.mask = E_M_NONE;
	m_nNumber	= nNumber;		// 数字
	m_size.x	= fSizeX;		// 大きさX
	m_size.y	= fSizeY;		// 大きさY
	m_halfSize = m_size * ML_HALF_SIZE;
	m_fRot		= fRot;			// 回転量
	m_pos		= pos;			// 位置 
	m_col		= col;			// 色
	// 辺巣宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();	// デバイスのポインタ

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(CRenderer::VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点の設定
	pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
	pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
	pVtx[2].pos.x = m_pos.x + sinf(-m_fRot)*m_halfSize.y + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
	pVtx[2].pos.y = m_pos.y + cosf(-m_fRot)*m_halfSize.y + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
	pVtx[3].pos.x = m_pos.x + sinf(-m_fRot)*m_halfSize.y + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
	pVtx[3].pos.y = m_pos.y + cosf(-m_fRot)*m_halfSize.y + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
	pVtx[0].pos.z = ML_FLOAT_UNSET;
	pVtx[1].pos.z = ML_FLOAT_UNSET;
	pVtx[2].pos.z = ML_FLOAT_UNSET;
	pVtx[3].pos.z = ML_FLOAT_UNSET;
	// 同次座標の設定
	pVtx[0].rhw = 1.0f;		//  1.0fで固定
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// 頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CNumber::Unint(void)
{
	// 頂点バッファがNULLじゃない時
	if (m_pVtxBuff != NULL)
	{// 頂点バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CNumber::Update(void)
{
	// 設定されている時
	if (m_flagbits.IsitSet())
	{// 頂点情報の更新
		UpdateVertex();
		m_flagbits.mask = E_M_NONE;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CNumber::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// 頂点バッファをストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CRenderer::VERTEX_2D));

	// 頂点フォーマット設定
	pDevice->SetTexture(0, m_pTexture);

	// テクスチャの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の更新
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetVaetex(int * nNumber, D3DXVECTOR3 * pos, D3DXCOLOR * col)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 数字ポインタがNULLじゃない時
	if (nNumber != NULL)
	{// テクスチャの設定
		m_nNumber = *nNumber;
		pVtx[0].tex = D3DXVECTOR2((*nNumber*0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (*nNumber*0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((*nNumber*0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (*nNumber*0.1f), 1.0f);
	}
	// 位置ポインタがNULLじゃない時
	if (pos != NULL)
	{// 位置の設定
		m_pos = *pos;
		pVtx[0].pos.x = pos->x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[0].pos.y = pos->y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[1].pos.x = pos->x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[1].pos.y = pos->y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[2].pos.x = pos->x + sinf(-m_fRot)*m_halfSize.y          + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[2].pos.y = pos->y + cosf(-m_fRot)*m_halfSize.y          + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[3].pos.x = pos->x + sinf(-m_fRot)*m_halfSize.y          + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[3].pos.y = pos->y + cosf(-m_fRot)*m_halfSize.y          + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
	}
	// カラーポインタがNULLじゃない時
	if (col != NULL)
	{// 色の更新
		m_col = *col;
		pVtx[0].col = *col;
		pVtx[1].col = *col;
		pVtx[2].col = *col;
		pVtx[3].col = *col;
	}
	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点カラー設定
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetVeatexColor(void)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の更新
//-------------------------------------------------------------------------------------------------------------
void CNumber::UpdateVertex(void)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 位置フラグが立っている時
	if (Mlf_bit_comp(m_flagbits.mask, E_F_POS))
	{
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[2].pos.x = m_pos.x + sinf(-m_fRot)*m_halfSize.y          + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[2].pos.y = m_pos.y + cosf(-m_fRot)*m_halfSize.y          + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[3].pos.x = m_pos.x + sinf(-m_fRot)*m_halfSize.y          + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[3].pos.y = m_pos.y + cosf(-m_fRot)*m_halfSize.y          + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
	}
	// テクスチャフラグが立っている時
	if (Mlf_bit_comp(m_flagbits.mask, E_F_TEX))
	{
		float fTex = m_nNumber*0.1f;
		pVtx[0].tex.x = fTex;
		pVtx[1].tex.x = 0.1f + fTex;
		pVtx[2].tex.x = pVtx[0].tex.x;
		pVtx[3].tex.x = pVtx[1].tex.x;

	}
	// カラーフラグが立っている時
	if (Mlf_bit_comp(m_flagbits.mask, E_F_COL))
	{
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = m_col;
	}


	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}
