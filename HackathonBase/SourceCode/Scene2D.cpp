//*************************************************************************************************************
//
// シーン2D処理[Scene2D.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene2D.h"
#include "manager.h"
#include "keyboard.h"
#include "DebugProc.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
const float      CScene2D::m_fHalfPi    = D3DX_PI * ML_HALF_SIZE;		// 円周率の半分の値
const float      CScene2D::m_fMinHalfPi = D3DX_PI * -ML_HALF_SIZE;		// 円周率の半分の負の値

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CScene2D::CScene2D(CScene::PRIORITY priority) : CScene(priority)
{
	m_pTexture = nullptr;
	m_TexAnimInfo = { 0,0,0,false,false };
	m_bTexUpScroll = false;
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CScene2D::~CScene2D()
{
}

//-------------------------------------------------------------------------------------------------------------
// 細かい作成
//-------------------------------------------------------------------------------------------------------------
CScene2D * CScene2D::Create(CScene::PRIORITY priority, D3DXVECTOR3 pos, D3DXCOLOR col, ORIGINVERTEXTYPE OriginType, D3DXVECTOR2 size, float m_fRotation, TEXTUREUV nTexVari)
{
	CScene2D *pScene2D = new CScene2D(priority);
	pScene2D->SetColor(col);
	pScene2D->SetOriginType(OriginType);
	pScene2D->SetPosition(pos);
	pScene2D->SetRotation(m_fRotation);
	pScene2D->SetSize(size);
	pScene2D->SetSetingUV(nTexVari);
	pScene2D->Init();
	return pScene2D;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CScene2D::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	m_bDisp = true;
	m_flagbits.mask = 0;

	// 頂点の作成
	this->MakeVatex(pDevice);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CScene2D::Uninit(void)
{
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::Update(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CScene2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = CManager::GetRenderer().GetDevice();

	// 頂点バッファをストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CRenderer::VERTEX_2D));

	// 頂点フォーマット設定
	pDevice->SetTexture(0, m_pTexture);

	// テクスチャの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	if (m_bDisp)
	{
		// ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::UpdateVertex(void)
{
	if (m_flagbits.IsitSet())
	{
		UpdateVertex(
			m_flagbits.pos == 1,
			m_flagbits.col == 1,
			m_flagbits.tex == 1);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の更新
//-------------------------------------------------------------------------------------------------------------
void CScene2D::UpdateVertex(bool bUpdatePos, bool bUpdateCol, bool bUpdateUVTex)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (bUpdatePos)
	{
		this->SetVatexPosition(pVtx);
	}
	if (bUpdateCol)
	{
		this->SetVatexColor(pVtx);
	}
	if (bUpdateUVTex)
	{
		this->SetVatexTexture(pVtx);
	}
	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// バインドテクスチャ
//-------------------------------------------------------------------------------------------------------------
void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//-------------------------------------------------------------------------------------------------------------
// バッファの取得
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetpVtxBuff(void)
{
	return m_pVtxBuff;
}

//-------------------------------------------------------------------------------------------------------------
// 原点種類の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetOriginType(ORIGINVERTEXTYPE OriginType)
{
	if (m_OriginType != OriginType)
	{
		m_OriginType = OriginType;
		// 位置フラグを設定
		SetPosflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 原点種類の取得
//-------------------------------------------------------------------------------------------------------------
ORIGINVERTEXTYPE *CScene2D::GetOriginType(void)
{
	return &m_OriginType;
}

//-------------------------------------------------------------------------------------------------------------
// 位置の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetPosition(D3DXVECTOR3& pos)
{
	if (m_pos != pos)
	{
		m_pos = pos;
		// 位置フラグを設定
		SetPosflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 位置の取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 *CScene2D::GetPosition(void)
{
	return &m_pos;
}

//-------------------------------------------------------------------------------------------------------------
// 色の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetColor(D3DXCOLOR& col)
{
	if (m_col != col)
	{
		m_col = col;
		// カラーフラグを設定
		SetColflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 赤色の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetColorR(const float fR)
{
	if (m_col.r != fR)
	{
		m_col.r = fR;
		// カラーフラグを設定
		SetColflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 緑色の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetColorG(const float fG)
{
	if (m_col.b != fG)
	{
		m_col.b = fG;
		// カラーフラグを設定
		SetColflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 青色の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetColorB(const float fB)
{
	if (m_col.b != fB)
	{
		m_col.b = fB;
		// カラーフラグを設定
		SetColflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// アルファ値の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetColorA(const float fA)
{
	if (m_col.a != fA)
	{
		m_col.a = fA;
		// カラーフラグを設定
		SetColflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 色の取得
//-------------------------------------------------------------------------------------------------------------
D3DXCOLOR *CScene2D::GetColor(void)
{
	return &m_col;
}

//-------------------------------------------------------------------------------------------------------------
// 大きさの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSize(D3DXVECTOR2& size)
{
	if (m_size != size)
	{
		m_size = size;
		// 位置フラグを設定
		SetPosflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 大きさXの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSizeX(const float size)
{
	if (m_size.x != size)
	{
		m_size.x = size;
		// 位置フラグを設定
		SetPosflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 大きさYの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSizeY(const float size)
{
	if (m_size.y != size)
	{
		m_size.y = size;
		// 位置フラグを設定
		SetPosflag();
	}

}

//-------------------------------------------------------------------------------------------------------------
// 大きさの取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR2 *CScene2D::GetSize(void)
{
	return &m_size;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSetingUV(TEXTUREUV& SetingUV)
{
	m_TexUVInfo = SetingUV;
}

//-------------------------------------------------------------------------------------------------------------
// // テクスチャのUV座標の設定用の情報横のカウントの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSetingUVCountU(const int nCntU)
{
	if (m_TexUVInfo.nCntU != nCntU)
	{
		m_TexUVInfo.nCntU = nCntU;
	}
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報縦のカウントの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSetingUVCountV(const int nCntV)
{
	if (m_TexUVInfo.nCntV != nCntV)
	{
		m_TexUVInfo.nCntV = nCntV;
	}
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報横の個数の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSetingUVNumU(const int nNumU)
{
	if (m_TexUVInfo.nNumU != nNumU)
	{
		m_TexUVInfo.nNumU = nNumU;
		m_TexUVInfo.fIntervalU = 1.0f / m_TexUVInfo.nNumU;
	}
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報縦の個数の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetSetingUVNumV(const int nNumV)
{
	if (m_TexUVInfo.nNumV != nNumV)
	{
		m_TexUVInfo.nNumV = nNumV;
		m_TexUVInfo.fIntervalV = 1.0f / m_TexUVInfo.nNumV;
	}
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャのUV座標の設定用の情報の取得
//-------------------------------------------------------------------------------------------------------------
TEXTUREUV *CScene2D::GetSetingUV(void)
{
	return &m_TexUVInfo;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャアニメーション用変数の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetAnimTexVariables(TEXTUREANIMEINFO& AnimTexVari)
{
	m_TexAnimInfo = AnimTexVari;
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャアニメーション用フレーム変数の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetAnimTexVariFrame(const int nFrame)
{
	if (m_TexAnimInfo.nFrame != nFrame)
	{
		m_TexAnimInfo.nFrame = nFrame;
	}
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャアニメーション用最大フレーム変数の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetAnimTexVariFrameMax(const int nFrameMax)
{
	if (m_TexAnimInfo.nFrameMax != nFrameMax)
	{
		m_TexAnimInfo.nFrameMax = nFrameMax;
	}
}
//-------------------------------------------------------------------------------------------------------------
// テクスチャアニメーション用変数の取得
//-------------------------------------------------------------------------------------------------------------
TEXTUREANIMEINFO * CScene2D::GetAnimTexVariables(void)
{
	return &m_TexAnimInfo;
}

//-------------------------------------------------------------------------------------------------------------
// 回転量の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetRotation(float fRotation)
{
	if (m_fRotation != fRotation)
	{
		m_fRotation = fRotation;
		// 位置フラグを設定
		SetPosflag();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 回転量の取得
//-------------------------------------------------------------------------------------------------------------
float CScene2D::GetRotation(void)
{
	return m_fRotation;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点テクスチャ座表計算
//-------------------------------------------------------------------------------------------------------------
int CScene2D::CalculatVatexUVMap(bool bLoop, int nFrameMax, int & nFrame, int nNumWidht, int nNumHight, int & nCountWidht, int & nCountHight)
{
	// 変数宣言
	int nState = -1;

	// フレーム数が最大値に到達した時
	if (nFrame >= nFrameMax)
	{	// フレームを初期化
		nFrame = 0;

		// x軸のカウントをクリアメント
		nCountWidht++;
		nState = true;

		// x軸のカウントが既定値に到達し時
		if (nNumWidht <= nCountWidht)
		{	// x軸のカウントを初期化
			nCountWidht = 0;

			// y軸のカウントをクリアメント
			nCountHight++;
			nState = true;

			// y軸のカウントが既定値に到達し時
			if (nNumHight <= nCountHight)
			{	// y軸のカウントを初期化
				nCountHight = 0;
				if (bLoop = false)
				{
					nState = false;
				}
			}
		}
	}
	else
	{	// フレーム数をクリアメント
		nFrame++;
	}

	return nState;
}

//-------------------------------------------------------------------------------------------------------------
// 選択可能な頂点テクスチャ座表計算
//-------------------------------------------------------------------------------------------------------------
void CScene2D::CalSelectionUVMap(bool bIteration, int nFrameMax, int & nFrame, int nNumWidht, int & nCountWidht, int nCountHight, int & m_nIteration)
{
	// フレーム数が最大値に到達した時
	if (nFrame >= nFrameMax)
	{
		// フレームを初期化
		nFrame = 0;

		// x軸のカウントをクリアメント
		nCountWidht += m_nIteration;

		// 反復する場合
		if (bIteration == true)
		{// x軸のカウントが既定値に到達し時
			if (nNumWidht - 1 <= nCountWidht)
			{
				m_nIteration = -1;
			}
			else if (0 >= nCountWidht)
			{
				m_nIteration = 1;
			}
		}
		else
		{
			if (nNumWidht - 1 <= nCountWidht)
			{
				nCountWidht = nNumWidht - 1;
			}
			else if (0 >= nCountWidht)
			{
				nCountWidht = 0;
			}
		}
	}
	else
	{	// フレーム数をクリアメント
		nFrame++;
	}

}

//-------------------------------------------------------------------------------------------------------------
// 反復頂点テクスチャ座表計算
//-------------------------------------------------------------------------------------------------------------
void CScene2D::CalTurnBackUVMap(int nFrameMax, int & nFrame, int nNumWidht, int & nCountWidht, int nCountHight, int & m_nIteration)
{
	// フレーム数が最大値に到達した時
	if (nFrame >= nFrameMax)
	{	// フレームを初期化
		nFrame = 0;
		// x軸のカウントが既定値に到達し時
		if (nNumWidht - 1 <= nCountWidht)
		{
			m_nIteration = -1;
		}
		else if (0 >= nCountWidht)
		{
			m_nIteration = 1;
		}
		// x軸のカウントをクリアメント
		nCountWidht += m_nIteration;
	}
	else
	{	// フレーム数をクリアメント
		nFrame++;
	}
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャースクロール処理
//-------------------------------------------------------------------------------------------------------------
void CScene2D::ScrollTex(float const & fTexY)
{
	// 頂点情報へのポインタ
	CRenderer::VERTEX_2D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex.y = fTexY;
	pVtx[1].tex.y = pVtx[0].tex.y;
	pVtx[2].tex.y = fTexY + 1.0f;
	pVtx[3].tex.y = pVtx[2].tex.y;

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT CScene2D::MakeVatex(LPDIRECT3DDEVICE9 pDevice)
{
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

	// 頂点位置の設定
	SetVatexPosition(pVtx);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラーの設定
	SetVatexColor(pVtx);

	// テクスチャ座標
	m_TexUVInfo.fIntervalU = (1.0f / m_TexUVInfo.nNumU);
	m_TexUVInfo.fIntervalV = (1.0f / m_TexUVInfo.nNumV);

	// テクスチャ座標の設定
	SetVatexTexture(pVtx);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetVatexPosition(CRenderer::VERTEX_2D *pVtx, ORIGINVERTEXTYPE *pType, D3DXVECTOR3 * pPos, D3DXVECTOR2 * pSize, float *pRotation)
{
	// 変数宣言
	D3DXVECTOR2 halfSize = *pSize * ML_HALF_SIZE;	// 半分の大きさ
	switch (*pType)
	{
	case ORIGINVERTEXTYPE_LOWERLEFT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x,            pPos->y - pSize->y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y - pSize->y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x,            pPos->y,            ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y,            ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_LOWERCENTER:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - halfSize.x, pPos->y - pSize->y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + halfSize.x, pPos->y - pSize->y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - halfSize.x, pPos->y, ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + halfSize.x, pPos->y, ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_LOWERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - pSize->y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x, pPos->y - pSize->y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y, ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x, pPos->y, ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTERLEFT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x, pPos->y - halfSize.y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y - halfSize.y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x, pPos->y + halfSize.y, ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + halfSize.y, ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTER:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - halfSize.x, pPos->y - halfSize.y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + halfSize.x, pPos->y - halfSize.y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - halfSize.x, pPos->y + halfSize.y, ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + halfSize.x, pPos->y + halfSize.y, ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - halfSize.y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x, pPos->y - halfSize.y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - halfSize.y, ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x, pPos->y - halfSize.y, ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERLEFT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x, pPos->y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x, pPos->y + pSize->y, ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + pSize->y, ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERCENTER:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y + pSize->y, ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + pSize->y, ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERRIGHT:
		pVtx[0].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y, ML_FLOAT_UNSET);
		pVtx[1].pos = D3DXVECTOR3(pPos->x, pPos->y, ML_FLOAT_UNSET);
		pVtx[2].pos = D3DXVECTOR3(pPos->x - pSize->x, pPos->y + pSize->y, ML_FLOAT_UNSET);
		pVtx[3].pos = D3DXVECTOR3(pPos->x, pPos->y + pSize->y, ML_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_ROTCENTER:
		pVtx[0].pos.x = pPos->x + sinf(D3DX_PI - *pRotation)*halfSize.y + sinf(m_fMinHalfPi - *pRotation)*halfSize.x;
		pVtx[0].pos.y = pPos->y + cosf(D3DX_PI - *pRotation)*halfSize.y + cosf(m_fMinHalfPi - *pRotation)*halfSize.x;
		pVtx[1].pos.x = pPos->x + sinf(D3DX_PI - *pRotation)*halfSize.y + sinf(m_fHalfPi - *pRotation)*halfSize.x;
		pVtx[1].pos.y = pPos->y + cosf(D3DX_PI - *pRotation)*halfSize.y + cosf(m_fHalfPi - *pRotation)*halfSize.x;
		pVtx[2].pos.x = pPos->x + sinf(-*pRotation)*halfSize.y + sinf(m_fMinHalfPi - *pRotation)*halfSize.x;
		pVtx[2].pos.y = pPos->y + cosf(-*pRotation)*halfSize.y + cosf(m_fMinHalfPi - *pRotation)*halfSize.x;
		pVtx[3].pos.x = pPos->x + sinf(-*pRotation)*halfSize.y + sinf(m_fHalfPi - *pRotation)*halfSize.x;
		pVtx[3].pos.y = pPos->y + cosf(-*pRotation)*halfSize.y + cosf(m_fHalfPi - *pRotation)*halfSize.x;
		pVtx[0].pos.z = ML_FLOAT_UNSET;
		pVtx[1].pos.z = ML_FLOAT_UNSET;
		pVtx[2].pos.z = ML_FLOAT_UNSET;
		pVtx[3].pos.z = ML_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTCENTERLEFT:
		pVtx[0].pos.x = pPos->x + sinf(D3DX_PI + *pRotation)*halfSize.y;
		pVtx[0].pos.y = pPos->y + cosf(D3DX_PI + *pRotation)*halfSize.y;
		pVtx[1].pos.x = pPos->x + sinf(D3DX_PI + *pRotation)*halfSize.y + sinf(m_fHalfPi + *pRotation)*(pSize->x);
		pVtx[1].pos.y = pPos->y + cosf(D3DX_PI + *pRotation)*halfSize.y + cosf(m_fHalfPi + *pRotation)*(pSize->x);
		pVtx[2].pos.x = pPos->x + sinf(*pRotation)*halfSize.y;
		pVtx[2].pos.y = pPos->y + cosf(*pRotation)*halfSize.y;
		pVtx[3].pos.x = pPos->x + sinf(*pRotation)*halfSize.y + sinf(m_fHalfPi + *pRotation)*(pSize->x);
		pVtx[3].pos.y = pPos->y + cosf(*pRotation)*halfSize.y + cosf(m_fHalfPi + *pRotation)*(pSize->x);
		pVtx[0].pos.z = ML_FLOAT_UNSET;
		pVtx[1].pos.z = ML_FLOAT_UNSET;
		pVtx[2].pos.z = ML_FLOAT_UNSET;
		pVtx[3].pos.z = ML_FLOAT_UNSET;
		break;

	case ORIGINVERTEXTYPE_ROTLOWERCENTER:
		pVtx[2].pos.x = pPos->x + sinf(m_fMinHalfPi + *pRotation)*halfSize.x;
		pVtx[2].pos.y = pPos->y + cosf(m_fMinHalfPi + *pRotation)*halfSize.x;
		pVtx[3].pos.x = pPos->x + sinf(m_fHalfPi + *pRotation)*halfSize.x;
		pVtx[3].pos.y = pPos->y + cosf(m_fHalfPi + *pRotation)*halfSize.x;
		pVtx[0].pos.x = pVtx[2].pos.x + sinf(D3DX_PI + *pRotation)*(pSize->y);
		pVtx[0].pos.y = pVtx[2].pos.y + cosf(D3DX_PI + *pRotation)*(pSize->y);
		pVtx[1].pos.x = pVtx[3].pos.x + sinf(D3DX_PI + *pRotation)*(pSize->y);
		pVtx[1].pos.y = pVtx[3].pos.y + cosf(D3DX_PI + *pRotation)*(pSize->y);

		pVtx[0].pos.z = ML_FLOAT_UNSET;
		pVtx[1].pos.z = ML_FLOAT_UNSET;
		pVtx[2].pos.z = ML_FLOAT_UNSET;
		pVtx[3].pos.z = ML_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTCENTERRIGHT:
		pVtx[0].pos.x = pPos->x + sinf(D3DX_PI + *pRotation)*halfSize.y + sinf(m_fMinHalfPi + *pRotation)*(pSize->x);
		pVtx[0].pos.y = pPos->y + cosf(D3DX_PI + *pRotation)*halfSize.y + cosf(m_fMinHalfPi + *pRotation)*(pSize->x);
		pVtx[1].pos.x = pPos->x + sinf(D3DX_PI + *pRotation)*halfSize.y;
		pVtx[1].pos.y = pPos->y + cosf(D3DX_PI + *pRotation)*halfSize.y;
		pVtx[2].pos.x = pPos->x + sinf(*pRotation)*halfSize.y + sinf(m_fMinHalfPi + *pRotation)*(pSize->x);
		pVtx[2].pos.y = pPos->y + cosf(*pRotation)*halfSize.y + cosf(m_fMinHalfPi + *pRotation)*(pSize->x);
		pVtx[3].pos.x = pPos->x + sinf(*pRotation)*halfSize.y;
		pVtx[3].pos.y = pPos->y + cosf(*pRotation)*halfSize.y;
		pVtx[0].pos.z = ML_FLOAT_UNSET;
		pVtx[1].pos.z = ML_FLOAT_UNSET;
		pVtx[2].pos.z = ML_FLOAT_UNSET;
		pVtx[3].pos.z = ML_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTUPPERCENTER:
		pVtx[0].pos.x = pPos->x + sinf(m_fMinHalfPi + *pRotation)*halfSize.x;
		pVtx[0].pos.y = pPos->y + cosf(m_fMinHalfPi + *pRotation)*halfSize.x;
		pVtx[1].pos.x = pPos->x + sinf(m_fHalfPi + *pRotation)*halfSize.x;
		pVtx[1].pos.y = pPos->y + cosf(m_fHalfPi + *pRotation)*halfSize.x;
		pVtx[2].pos.x = pVtx[0].pos.x + sinf(*pRotation)*(pSize->y);
		pVtx[2].pos.y = pVtx[0].pos.y + cosf(*pRotation)*(pSize->y);
		pVtx[3].pos.x = pVtx[1].pos.x + sinf(*pRotation)*(pSize->y);
		pVtx[3].pos.y = pVtx[1].pos.y + cosf(*pRotation)*(pSize->y);
		pVtx[0].pos.z = ML_FLOAT_UNSET;
		pVtx[1].pos.z = ML_FLOAT_UNSET;
		pVtx[2].pos.z = ML_FLOAT_UNSET;
		pVtx[3].pos.z = ML_FLOAT_UNSET;
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetVatexPosition(CRenderer::VERTEX_2D * pVtx)
{
	SetVatexPosition(pVtx, &m_OriginType, &m_pos, &m_size, &m_fRotation);
}

//-------------------------------------------------------------------------------------------------------------
// 頂点カラーの設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetVatexColor(CRenderer::VERTEX_2D * pVtx)
{
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = m_col;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点テクスチャ座標の設定
//-------------------------------------------------------------------------------------------------------------
void CScene2D::SetVatexTexture(CRenderer::VERTEX_2D * pVtx)
{
	pVtx[0].tex.x = m_TexUVInfo.fIntervalU  * m_TexUVInfo.nCntU;
	pVtx[1].tex.x = m_TexUVInfo.fIntervalU  + pVtx[0].tex.x;
	pVtx[2].tex.x = pVtx[0].tex.x;
	pVtx[3].tex.x = pVtx[1].tex.x;

	pVtx[0].tex.y = m_TexUVInfo.fIntervalV*m_TexUVInfo.nCntV;
	pVtx[1].tex.y = pVtx[0].tex.y;
	pVtx[2].tex.y = m_TexUVInfo.fIntervalV + pVtx[0].tex.y;
	pVtx[3].tex.y = pVtx[2].tex.y;
}
