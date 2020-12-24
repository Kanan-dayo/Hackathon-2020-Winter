// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 2Dエフェクトの処理[2Deffect.cpp]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "2Deffect.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define ORIGIN_POS	(D3DXVECTOR3(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f))	// 初期位置

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9	C2DEffect::m_pVtxBuff = NULL;			// バッファ情報
CEffect::EFFECT			C2DEffect::m_aEffect[EFFECT_MAX] = {};	// エフェクト情報

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DEffect::C2DEffect() :CEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DEffect::~C2DEffect()
{
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;					// デバイスのポインタ
	pDevice = CManager::GetRenderer().GetDevice();	// デバイスの取得
	// エフェクトの初期化
	CEffect::Init();
	// 変数の初期化
	CEffect::InitValues(&m_aEffect[0]);
	// 頂点情報作成
	MakeVertex(pDevice);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Uninit(void)
{
	// バッファ情報のNULLチェック
	if (m_pVtxBuff != NULL)
	{
		// バッファ情報の開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Update(void)
{
	// 変数定義
	CRenderer::VERTEX_2D *pVtx;		// 頂点情報のポインタ
	C2DEffect::EFFECT *pEffect;		// エフェクトのポインタ
	// ポインタの初期化
	pEffect = &m_aEffect[0];

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// エフェクトループ
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++, pVtx+=4)
	{
		// 使用フラグがオフの時
		// ->ループスキップ
		if (!pEffect->bUse)
		{
			continue;
		}
		// ライフ更新
		// ->ループスキップ
		if (!CEffect::UpdateLife(pEffect))
		{
			continue;
		}
		// 移動の更新
		CEffect::UpdateMove(pEffect);
		// 頂点情報の更新
		CEffect::UpdateVetex(pEffect);
		// 頂点テクスチャー情報の更新
		CEffect::Updata_Animation(pEffect);
		// 位置更新
		pEffect->pos += pEffect->move;
		// 頂点サイズの設定
		SetVartexSize(pVtx, pEffect);
		// 頂点カラーの設定
		SetVetexColor(pVtx, pEffect);
		// 頂点テクスチャーの設定
		SetVetexTex(pVtx, pEffect);
	}
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;					// デバイスのポインタ
	C2DEffect::EFFECT *pEffect;						// エフェクトのポインタ

	pDevice = CManager::GetRenderer().GetDevice();	// デバイスの取得
	pEffect = &m_aEffect[0];						// ポインタの初期化


	// エフェクトループ
	for (int nCount = 0; nCount < EFFECT_MAX; nCount++, pEffect++)
	{
		if (pEffect->bUse == true)
		{
			// 合成演算設定
			CManager::GetRenderer().SetBlend(pEffect->BlendType);
			// 頂点バッファをデータストリームにバインド
			pDevice->SetStreamSource(
				0,
				m_pVtxBuff,
				0,
				sizeof(CRenderer::VERTEX_2D)
			);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			// テクスチャの設定
			pDevice->SetTexture(0, CTexture::GetTextureInfo(pEffect->nTexType));
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点作成
//	pDevice	: デバイス情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C2DEffect::MakeVertex(
	LPDIRECT3DDEVICE9 pDevice	// デバイス情報
)
{
	// 変数宣言
	CRenderer::VERTEX_2D *pVtx;							// 頂点情報

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(CRenderer::VERTEX_2D) * 4 * EFFECT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pVtx += 4)
	{
		// 頂点サイズ設定
		SetVartexSize(pVtx, NULL, &nCntEffect);

		// 頂点
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		// texture座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

	// 成功を返す
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点サイズ設定
//	pVtx	: 2D頂点情報
//	pEffect	: エフェクト情報
//	nIndex	: 番号情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::SetVartexSize(
	CRenderer::VERTEX_2D *pVtx,	// 2D頂点情報
	EFFECT *pEffect,	// エフェクト情報
	const int *nIndex	// 番号情報
)
{
	// 番号情報のNULLチェック
	if (nIndex != NULL)
	{
		// 変数宣言
		C2DEffect::EFFECT *pLocalEffect;					// エフェクトのポインタ
		pLocalEffect = &C2DEffect::m_aEffect[*nIndex];		// ポインタの初期化

		// 頂点の設定
		pVtx[0].pos = ORIGIN_POS + pLocalEffect->pos;
		pVtx[0].pos.x += -sinf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[0].pos.y += -cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[1].pos = ORIGIN_POS + pLocalEffect->pos;
		pVtx[1].pos.x += sinf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[1].pos.y += -cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[2].pos = ORIGIN_POS + pLocalEffect->pos;
		pVtx[2].pos.x += -sinf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[2].pos.y += cosf(BASEANGLE - pLocalEffect->fAngle) * pLocalEffect->size.y;
		pVtx[3].pos = ORIGIN_POS + pLocalEffect->pos;
		pVtx[3].pos.x += sinf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.x;
		pVtx[3].pos.y += cosf(BASEANGLE + pLocalEffect->fAngle) * pLocalEffect->size.y;
	}
	// エフェクト情報のNULLチェック
	else if (pEffect != NULL)
	{
		// 頂点の設定
		pVtx[0].pos = ORIGIN_POS + pEffect->pos;
		pVtx[0].pos.x += -sinf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[0].pos.y += -cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.y;
		pVtx[1].pos = ORIGIN_POS + pEffect->pos;
		pVtx[1].pos.x += sinf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[1].pos.y += -cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.y;
		pVtx[2].pos = ORIGIN_POS + pEffect->pos;
		pVtx[2].pos.x += -sinf(BASEANGLE - pEffect->fAngle) * pEffect->size.x;
		pVtx[2].pos.y += cosf(BASEANGLE - pEffect->fAngle) * pEffect->size.y;
		pVtx[3].pos = ORIGIN_POS + pEffect->pos;
		pVtx[3].pos.x += sinf(BASEANGLE + pEffect->fAngle) * pEffect->size.x;
		pVtx[3].pos.y += cosf(BASEANGLE + pEffect->fAngle) * pEffect->size.y;
	}
}


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点カラー設定
//	pVtx	: 2D頂点情報
//	pEffect	: エフェクト情報
//	nIndex	: 番号情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::SetVetexColor(
	CRenderer::VERTEX_2D *pVtx,	// 2D頂点情報
	EFFECT *pEffect,	// エフェクト情報
	const int *nIndex	// 番号情報
)
{
	// 番号情報のNULLチェック
	if (nIndex != NULL)
	{
		// 変数宣言
		C2DEffect::EFFECT *pLocalEffect;					// エフェクトのポインタ
		pLocalEffect = &C2DEffect::m_aEffect[*nIndex];		// ポインタの初期化

		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pLocalEffect->col;
	}
	// エフェクト情報のNULLチェック
	else if (pEffect != NULL)
	{
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = pEffect->col;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 頂点テクスチャー設定
//	pVtx	: 2D頂点情報
//	pEffect	: エフェクト情報
//	nIndex	: 番号情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::SetVetexTex(
	CRenderer::VERTEX_2D *pVtx,	// 2D頂点情報
	EFFECT *pEffect,	// エフェクト情報
	const int *nIndex 	// 番号情報
)
{
	if (nIndex != NULL)
	{
		// 変数宣言
		C2DEffect::EFFECT *pLocalEffect;					// エフェクトのポインタ
		pLocalEffect = &C2DEffect::m_aEffect[*nIndex];		// ポインタの初期化
															// アニメーション情報が存在しないなら
		if (!pLocalEffect->Animation.bUse &&
			!pLocalEffect->Animation.bTexUpdate) return;
		// 頂点テクスチャー
		pVtx[0].tex = D3DXVECTOR2(pLocalEffect->Animation.FirstPos.x, pLocalEffect->Animation.FirstPos.y);
		pVtx[1].tex = D3DXVECTOR2(pLocalEffect->Animation.EndPos.x, pLocalEffect->Animation.FirstPos.y);
		pVtx[2].tex = D3DXVECTOR2(pLocalEffect->Animation.FirstPos.x, pLocalEffect->Animation.EndPos.y);
		pVtx[3].tex = D3DXVECTOR2(pLocalEffect->Animation.EndPos.x, pLocalEffect->Animation.EndPos.y);
		// 頂点テクスチャー更新状態をfalseに
		pLocalEffect->Animation.bTexUpdate = false;
	}
	else if (pEffect != NULL)
	{
		// アニメーション情報が存在しないなら
		if (!pEffect->Animation.bUse &&
			!pEffect->Animation.bTexUpdate) return;
		// 頂点テクスチャー
		pVtx[0].tex = D3DXVECTOR2(pEffect->Animation.FirstPos.x, pEffect->Animation.FirstPos.y);
		pVtx[1].tex = D3DXVECTOR2(pEffect->Animation.EndPos.x, pEffect->Animation.FirstPos.y);
		pVtx[2].tex = D3DXVECTOR2(pEffect->Animation.FirstPos.x, pEffect->Animation.EndPos.y);
		pVtx[3].tex = D3DXVECTOR2(pEffect->Animation.EndPos.x, pEffect->Animation.EndPos.y);
		// 頂点テクスチャー更新状態をfalseに
		pEffect->Animation.bTexUpdate = false;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 位置上限処理
//	pEffect	: エフェクト情報
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Pos_Limit(
	EFFECT *pEffect		// エフェクト情報
)
{
	// 位置xの上限を超えたら
	if (pEffect->pos.x < -pEffect->size.x || pEffect->pos.x >(SCREEN_WIDTH + pEffect->size.x))
	{
		pEffect->bUse = false;
	}
	// 位置yの上限を超えたら
	if (pEffect->pos.y < -pEffect->size.y || pEffect->pos.y >(SCREEN_HEIGHT + pEffect->size.y))
	{
		pEffect->bUse = false;
	}
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 生成処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C2DEffect * C2DEffect::Create(void)
{
	// 変数宣言
	C2DEffect *p2DEffect = new C2DEffect;
	// 初期化
	p2DEffect->Init();
	return p2DEffect;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全体の設定
//	EffectType	: エフェクトタイプ
//	nTexType	: テクスチャータイプ
//	pos			: 位置
//	rot			: 回転
//	move		: 移動量
//	col			: 色
//	size		: サイズ
//	nLife		: ライフ
//	Blend		: ブレンドタイプ
//	sizeValue	: サイズ変化
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Set2DEffect(
	EFFECT_TYPE const &EffectType,					// エフェクトタイプ
	int const &nTexType,							// テクスチャータイプ
	D3DXVECTOR3 const &pos,							// 位置
	D3DXVECTOR3 const &rot,							// 回転
	D3DXVECTOR3 const &move,						// 移動量
	D3DXCOLOR const &col,							// 色
	D3DXVECTOR2 const &size,						// サイズ
	int const &nLife,								// ライフ
	ANIMATION_LOAD const & Anim,					// アニメーション情報読み込み
	CRenderer::BLEND const &Blend,					// ブレンドタイプ
	D3DXVECTOR2 const &sizeValue,					// サイズ変化値
	float const &fAlphaValue						// α変化値
)
{
	// 変数宣言
	C2DEffect::EFFECT *pEffect;			// エフェクトのポインタ
	pEffect = &C2DEffect::m_aEffect[0];	// ポインタの初期化

	// 最大数ループ
	for (int nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		// 使用フラグオフの個体の時
		if (pEffect->bUse == false)
		{
			// エフェクト情報の値初期化
			Init_OneValues(pEffect);
			// 変数定義
			CRenderer::VERTEX_2D *pVtx;		// 頂点ポインタ
			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点ポインタの更新
			pVtx += nCntEffect * 4;
			// ライフの設定
			pEffect->nLife = nLife;
			// テクスチャ種類の設定
			pEffect->nTexType = nTexType;
			// エフェクト種類の設定
			pEffect->EffectType = EffectType;
			// 移動量の設定
			pEffect->move = move;
			// 半径の設定
			pEffect->size = size;
			// 位置の設定
			pEffect->pos = pos;
			// 回転の設定
			pEffect->rot = rot;
			// ブレンド設定
			pEffect->BlendType = Blend;
			// 色の設定
			pEffect->col = col;
			// 使用フラグをオン
			pEffect->bUse = true;
			// アルファ変化値の設定
			pEffect->fAlphaValue = fAlphaValue;
			// サイズ変化値の設定
			pEffect->sizeValue = sizeValue;
			// アニメーション設定
			pEffect->Animation = Anim;
			// 始点位置
			pEffect->Animation.FirstPos = D3DXVECTOR2(
				pEffect->Animation.nHorizonAnim * pEffect->Animation.fHorizonSize,
				pEffect->Animation.nVirticalAnim * pEffect->Animation.fVirticalSize
			);
			// 終点位置
			pEffect->Animation.EndPos = D3DXVECTOR2(
				pEffect->Animation.nHorizonAnim * pEffect->Animation.fHorizonSize + pEffect->Animation.fHorizonSize,
				pEffect->Animation.nVirticalAnim * pEffect->Animation.fVirticalSize + pEffect->Animation.fVirticalSize
			);
			// 頂点位置の設定
			SetVartexSize(pVtx, pEffect);
			// 頂点カラーの設定
			SetVetexColor(pVtx, pEffect);
			// 頂点テクスチャーの設定
			SetVetexTex(pVtx, pEffect);

			//頂点データをアンロック
			m_pVtxBuff->Unlock();
			break;
		}
	}

}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全リソース情報の読み込み処理
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C2DEffect::Load(void)
{
	return S_OK;
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 全リソース情報の開放
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Unload(void)
{
}

#ifdef _DEBUG
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デバッグ表示
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C2DEffect::Debug(void)
{
}
#endif // _DEBUG
