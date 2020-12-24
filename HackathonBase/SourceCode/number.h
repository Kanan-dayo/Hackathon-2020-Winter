//*************************************************************************************************************
//
// 数字の処理[number.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CNumber
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
		E_M_POS  = 0b00000001,
		E_M_TEX  = 0b00000010,
		E_M_COL  = 0b00000100,
		E_M_MAX  = E_M_POS | E_M_TEX | E_M_COL,
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


	CNumber(int nNumOrder);																								// コンストラクタ
	CNumber();																											// コンストラクタ
	~CNumber();																											// デストラクタ
	static HRESULT      Load(void);																						// 読み込み
	static void         Unload(void);																					// 開放
	static CNumber*     Create(float fSizeX, float fSizeY, D3DXVECTOR3 pos, float fRot, int nNumber, D3DXCOLOR col);	// 生成
	HRESULT             Init(float fSizeX, float fSizeY, int nNumber, float fRot, D3DXVECTOR3 pos, D3DXCOLOR col);		// 初期化
	void                Unint(void);																					// 終了
	void                Update(void);																					// 更新
	void                Draw(void);																						// 描画
	void                SetVaetex(int *nNumber = NULL, D3DXVECTOR3 *pos = NULL, D3DXCOLOR *col = NULL);					// 頂点の更新
	void                SetVeatexColor(void);																			// 頂点カラー設定

	inline void         SetPosition(D3DXVECTOR3 &pos) { if (m_pos != pos) { m_pos = pos; SetPosflag(); } }				// 位置の設定
	inline void         SetNumber(int nNumber)     { if (m_nNumber != nNumber) { m_nNumber = nNumber; SetTexflag(); } }	// 数字の設定
	inline void         SetSize(D3DXVECTOR2 &size) { if (m_size != size) { m_size = size; m_halfSize = size*ML_HALF_SIZE; SetPosflag(); } }				// 大きさの設定
	inline void         SetRotation(float fRot)    { if (m_fRot != fRot) { m_fRot = fRot; SetTexflag(); } }				// 回転量の設定
	inline void         SetColor(D3DXCOLOR &col)   { if (m_col != col) { m_col = col; SetColflag(); } }					// 回転量の設定
	inline void         BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }								// テクスチャ情報の設定

	void                Setflagbits(UINT mask) { m_flagbits.mask = mask; }												// フラグビットの設定
	void                SetPosflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_POS); }										// 位置フラグを設定
	void                SetTexflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_TEX); }										// テクスチャフラグを設定
	void                SetColflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_COL); }										// カラーフラグを設定
	void                UnsetPosflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_POS); }									// 位置フラグを外す
	void                UnsetTexflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_TEX); }									// テクスチャフラグを外す
	void                UnsetColflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_COL); }									// カラーフラグを外す

	inline D3DXCOLOR*   GetColorPtr(void) { return &m_col; }															// カラーポインタの取得
	inline D3DXVECTOR2* GetSizePtr(void) { return &m_size; }															// 大きさポインタの取得
	inline D3DXVECTOR3* GetPosPtr(void) { return &m_pos; }																// 位置ポインタの取得
	inline float*       GetRotPtr(void) { return &m_fRot; }																// 向きポインタの取得
	inline int*         GetNumberPtr(void) { return &m_nNumber; }														// 数字ポインタの取得
	inline D3DXCOLOR&   GetColor(void) { return m_col; }																// カラーポインタの取得
	inline D3DXVECTOR2& GetSize(void) { return m_size; }																// 大きさポインタの取得
	inline D3DXVECTOR3& GetPos(void) { return m_pos; }																	// 位置ポインタの取得
	inline float&       GetRot(void) { return m_fRot; }																	// 向きポインタの取得
	inline int&         GetNumber(void) { return m_nNumber; }															// 数字ポインタの取得

private:
	void                UpdateVertex(void);																				// 頂点情報の更新

	static const float		m_fHalfPi;												// 円周率の半分の値
	static const float		m_fMinHalfPi;											// 円周率の半分の負の値
	LPDIRECT3DTEXTURE9      m_pTexture;		// テクスチャ情報のポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// バッファ情報のポインタ
	int                     m_nNumber;		// 数字
	float                   m_fRot;			// 回転量
	D3DXVECTOR2             m_size;			// 大きさ
	D3DXVECTOR2             m_halfSize;		// 半分の大きさ
	D3DXVECTOR3             m_pos;			// 位置
	D3DXCOLOR               m_col;			// 色
	BITS                    m_flagbits;		// フラグビット
};

#endif
