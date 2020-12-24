//*************************************************************************************************************
//
// 2DUIの構造体などの処理[2DUITypes.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _2DUITYPES_H_
#define _2DUITYPES_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// 前方宣言
//-------------------------------------------------------------------------------------------------------------
class CNumericString;
class CScene2D;
class C2DUi;

//-------------------------------------------------------------------------------------------------------------
// 名前空間定義
//-------------------------------------------------------------------------------------------------------------
namespace N2Duitypes
{
	//-------------------------------------------------------------------------------------------------------------
	// 列挙型定義
	//-------------------------------------------------------------------------------------------------------------
	// マスク
	enum MASK
	{
		E_M_NUMBER   = 0b00000001,	// 数字
		E_M_FLASHING = 0b00000010,	// 点滅
		E_M_FADE     = 0b00000100,	// フェード
		E_M_MAX      = 0b00000111,	// 最大
	};
	// フラグ
	enum FLAG
	{
		E_F_NONE = -1,				// 無し
		E_F_NUMBER,					// 数字
		E_F_FLASHING,				// 点滅
		E_F_FADE,					// フェード
		E_F_MAX						// 最大
	};
	// 画像タイプ
	typedef enum IMGTYPE
	{
		E_I_NONE = -1,				// 無し
		E_I_IMAGE,					// 画像
		E_I_NUMBER,					// 数字
		E_I_MAX						// 最大
	}IMGTYPE;
	// 能力タイプ
	typedef enum ABILITYTYPE
	{
		E_A_NONE = -1,
		E_A_FLASHING,				// 点滅
		E_A_FADE,					// フェード
		E_A_MAX						// 最大
	} ABILITYTYPE;

	//-------------------------------------------------------------------------------------------------------------
	// 構造体定義
	//-------------------------------------------------------------------------------------------------------------
	// ビットフラグ
	typedef struct BITS
	{
		BITS() : unMask(0) {}				// コンストラクタ
		BITS(UINT unMask);					// コンストラクタ

		union {
			struct {
				UINT bNumber : 1;			// 個数
				UINT bFlashing : 1;			// 点滅
				UINT bFade : 1;				// フェード
			};
			UINT unMask;					// マスク値
		};
	}BITS;

	// アビリティベース
	typedef struct ABILITYBASE
	{
		ABILITYBASE() {}								// コンストラクタ
		ABILITYBASE(ABILITYTYPE type) : type(type) {}	// コンストラクタ

		virtual void Update(C2DUi *pThis) = 0;			// 更新

		ABILITYTYPE type;								// タイプ
	}ABILITYBASE;

	// 点滅能力
	typedef struct FLASHING : public ABILITYBASE
	{
		FLASHING();																// コンストラクタ
		FLASHING(bool bUpdate, UINT nTiming);									// コンストラクタ

		inline bool IsTimeEqualTiming(void) { return (nTiming == nTime); }		// 時間とタイミングが同じだった時
		void        Update(C2DUi *pThis);										// 更新処理

		bool bUpdate;															// 更新フラグ
		UINT nTime;																// 時間
		UINT nTiming;															// 点滅
	} FLASHING;

	// フェード能力
	typedef struct FADE : public ABILITYBASE
	{
		FADE();																										// コンストラクタ
		FADE(bool bUpdate, bool bLoop, int nTiming, int nAddSign);													// コンストラクタ

		inline void   SetChangeValue(float fChangeValue) { this->fChangeValue = fChangeValue; }						// 1フレの変更値の設定
		inline float& GetChangeValue(void) { return fChangeValue; }													// 1フレの変更値の取得
		inline void   SetAddSign(int nAddSign) { if (nAddSign != 0) { this->nAddSign = (nAddSign > 0) ? 1 : -1; } }	// 加算する符号の設定
		inline int&   GetAddSign(void) { return nAddSign; }															// 加算する符号の取得

		void          Update(C2DUi *pThis);																			// 更新
		void          UpdateImage(CScene2D *pImage);																// 画像の更新
		void          UpdateImage(CNumericString *m_pNumber);														// 画像の更新

		bool  bUpdate;																								// 更新フラグ
		bool  bLoop;																								// ループ
		float fChangeValue;																							// アルファ値の1フレの変更値
		int   nTiming;																								// タイミング
		int   nAddSign;																								// 加算する符号
	} FADE;

	// 設定用
	typedef struct SETING
	{
		SETING() {}
		SETING(BITS mask, int  nTextureID, bool bDisp, D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue);

		BITS        mask;			// マスク
		int         nTextureID;		// テクスチャID
		bool        bDisp;			// 描画フラグ
		D3DXVECTOR3 pos;			// 位置
		D3DXCOLOR   col;			// 色
		D3DXVECTOR2 size;			// サイズ
		float       fRotation;		// 回転量
		int         nValue;			// 値
	} SETING;
}

//-------------------------------------------------------------------------------------------------------------
// エイリアス宣言
//-------------------------------------------------------------------------------------------------------------
// 列挙型
using N2Dui_flag     = N2Duitypes::FLAG;			// フラグ
using N2Dui_mask     = N2Duitypes::MASK;			// マスク
using N2Dui_img      = N2Duitypes::IMGTYPE;			// 画像タイプ
using N2Dui_ability  = N2Duitypes::ABILITYTYPE;		// 能力タイプ

// 構造体
using N2Dui_abibase  = N2Duitypes::ABILITYBASE;		// アビリティベース
using N2Dui_fade     = N2Duitypes::FADE;			// フェード能力
using N2Dui_flashing = N2Duitypes::FLASHING;		// 点滅能力
using N2Dui_bits     = N2Duitypes::BITS;			// ビットフラグ
using N2Dui_seting   = N2Duitypes::SETING;			// 設定用

#endif