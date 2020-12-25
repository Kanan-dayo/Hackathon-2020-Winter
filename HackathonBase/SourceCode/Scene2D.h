//*************************************************************************************************************
//
// シーン2D処理[scene2D.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CScene2D :public CScene
{
public:
	/* 列挙型定義 */
	enum
	{
		E_F_NONE = -1,
		E_F_POS,
		E_F_TEX,
		E_F_COL,
		E_F_MAX
	};

	enum
	{
		E_M_NONE = 0b00000000,
		E_M_POS = 0b00000001,
		E_M_TEX = 0b00000010,
		E_M_COL = 0b00000100,
		E_M_MAX = E_M_POS | E_M_TEX | E_M_COL,
	};

	/* 構造体定義 */
	typedef struct BITS
	{
		union {
			struct {
				UINT pos : 1;
				UINT tex : 1;
				UINT col : 1;
			};
			UINT mask;
		};

		inline bool IsitSet(void) { return mask != ML_INT_UNSET; }														// 設定されているか?
	}BITS;


	CScene2D(CScene::PRIORITY priority);											// コンストラクタ
	~CScene2D();																	// デストラクタ

	static CScene2D *Create(														// 生成
		CScene::PRIORITY priority,													// 描画優先順位
		D3DXVECTOR3      pos,														// 位置
		D3DXCOLOR        col,														// 色
		ORIGINVERTEXTYPE OriginType,												// 原点タイプ
		D3DXVECTOR2      size,														// 大きさ
		float            fRotation = 0.0f,											// 回転量
		TEXTUREUV        TexUVInfo = TEXTUREUV(0, 1, 0, 1));						// テクスチャUV設定

	void Init(void);																// 初期化
	void Uninit(void);																// 終了
	void Update(void);																// 更新
	void Draw(void);																// 描画

	void UpdateVertex(void);														// 頂点の更新
	void UpdateVertex(																// 頂点の更新
		bool bUpdatePos,															// 位置更新フラグ
		bool bUpdateCol = false,													// 色更新フラグ
		bool bUpdateUVTex = false);													// UV座標更新フラグ

	void                    BindTexture(LPDIRECT3DTEXTURE9 pTexture);				// テクスチャを関連付ける

	LPDIRECT3DVERTEXBUFFER9 GetpVtxBuff(void);										// 頂点バッファの取得

	void                    SetOriginType(ORIGINVERTEXTYPE OriginType);				// 原点タイプの設定
	ORIGINVERTEXTYPE*       GetOriginType(void);									// 原点タイプの取得

	void                    SetPosition(D3DXVECTOR3& pos);							// 位置の設定
	D3DXVECTOR3*            GetPosition(void);										// 位置の取得

	void                    SetColor(D3DXCOLOR& col);								// 色の設定
	void                    SetColorR(const float fR);									// 赤色の設定
	void                    SetColorG(const float fG);									// 緑色の設定
	void                    SetColorB(const float fB);									// 青色の設定
	void                    SetColorA(const float fA);									// アルファ値の設定

	D3DXCOLOR*              GetColor(void);											// 色の取得

	void                    SetSize(D3DXVECTOR2& size);								// 大きさの設定
	void                    SetSizeX(const float fSize);							// 大きさXの設定
	void                    SetSizeY(const float fSize);							// 大きさYの設定
	D3DXVECTOR2*            GetSize(void);											// 大きさの取得

	void                    SetSetingUV(TEXTUREUV& SetingUV);						// テクスチャのUV座標の設定用の情報の設定
	void                    SetSetingUVCountU(const int nCntU);						// テクスチャのUV座標の設定用の情報横のカウントの設定
	void                    SetSetingUVCountV(const int nCntV);						// テクスチャのUV座標の設定用の情報縦のカウントの設定
	void                    SetSetingUVNumU(const int nNumU);						// テクスチャのUV座標の設定用の情報横の個数の設定
	void                    SetSetingUVNumV(const int nNumV);						// テクスチャのUV座標の設定用の情報縦の個数の設定
	TEXTUREUV*              GetSetingUV(void);										// テクスチャのUV座標の設定用の情報の取得

	void                    SetAnimTexVariables(TEXTUREANIMEINFO& TextureAnimeInfo);// テクスチャアニメーション情報の設定
	void                    SetAnimTexVariFrame(const int nFrame);					// テクスチャアニメーションフレーム情報の設定
	void                    SetAnimTexVariFrameMax(const int nFrameMax);			// テクスチャアニメーションの最大フレーム情報の設定
	TEXTUREANIMEINFO*       GetAnimTexVariables(void);								// テクスチャアニメーション情報の取得

	void                    SetRotation(const float fRotation);						// 回転量の設定
	float                   GetRotation(void);										// 回転量の取得
	int                     CalculatVatexUVMap(bool bLoop, int nFrameMax, int &nFrame, int nNumWidht, int nNumHight, int &nCountWidht, int &nCountHight);
	void                    CalSelectionUVMap(bool bIteration, int nFrameMax, int &nFrame, int nNumWidht, int &nCountWidht, int nCountHight, int &m_nIteration);
	void                    CalTurnBackUVMap(int nFrameMax, int &nFrame, int nNumWidht, int &nCountWidht, int nCountHight, int &m_nIteration);
	inline void             SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	inline bool &           GetDisp(void) { return m_bDisp; }

	inline void             Setflagbits(UINT mask) { m_flagbits.mask = mask; }												// フラグビットの設定
	inline void             SetPosflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_POS); }										// 位置フラグを設定
	inline void             SetTexflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_TEX); }										// テクスチャフラグを設定
	inline void             SetColflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_COL); }										// カラーフラグを設定
	inline void             UnsetPosflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_POS); }									// 位置フラグを外す
	inline void             UnsetTexflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_TEX); }									// テクスチャフラグを外す
	inline void             UnsetColflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_COL); }									// カラーフラグを外す

	// スクロール処理
	void ScrollTex(float const & fTexY);
protected:
	/* -- メンバ関数 -- */
	HRESULT                 MakeVatex(LPDIRECT3DDEVICE9 pDevice);					// 頂点の作成
	void                    SetVatexPosition(CRenderer::VERTEX_2D *pVtx, ORIGINVERTEXTYPE *pType, D3DXVECTOR3 * pPos, D3DXVECTOR2 * pSize, float *pRotation);			// 頂点位置の設定
	void                    SetVatexPosition(CRenderer::VERTEX_2D *pVtx);			// 頂点位置の設定
	void                    SetVatexColor(CRenderer::VERTEX_2D *pVtx);				// 頂点カラーの設定
	void                    SetVatexTexture(CRenderer::VERTEX_2D *pVtx);			// 頂点テクスチャの設定
	/* -- メンバ変数 -- */
	static const float		m_fHalfPi;												// 円周率の半分の値
	static const float		m_fMinHalfPi;											// 円周率の半分の負の値
	LPDIRECT3DTEXTURE9      m_pTexture;												// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												// 頂点バッファへのポインタ
	ORIGINVERTEXTYPE        m_OriginType;											// 原点の種類
	D3DXVECTOR3             m_pos;													// 位置
	D3DXCOLOR               m_col;													// 色
	D3DXVECTOR2             m_size;													// 大きさ
	TEXTUREUV               m_TexUVInfo;											// テクスチャUV座標の情報
	float                   m_fRotation;											// 回転量
	bool                    m_bDisp;												// 描画フラグ
	BITS                    m_flagbits;												// フラグビット
	TEXTUREANIMEINFO        m_TexAnimInfo;											// テクスチャアニメーション情報
};

#endif