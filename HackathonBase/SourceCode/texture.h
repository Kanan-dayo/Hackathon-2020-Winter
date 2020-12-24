//*************************************************************************************************************
//
// テクスチャ処理[texture.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "manager.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CTexture
{
public:
	enum
	{
		NAME_NONE = -1,					// 無し
		NAME_FRAME,
		NAME_NUMBER,
		NAME_PleasePressButton000,
		NAME_MAX						// 最大数
	};
	enum
	{
		TEXTURE_TOPID_NONE     = -1,						// 無し
		TEXTURE_TOPID_MAX      = NAME_MAX,					// 最大数
	};
	CTexture();																												// コンストラクタ
	~CTexture();																											// デストラクタ
	static void               Load(void);																					// 読み込み
	static void               Unload(void);																					// 開放
	static void               CreateTexture(void);																			// テクスチャの生成
	static LPDIRECT3DTEXTURE9 GetTextureInfo(int nIndex);																	// テクスチャの取得
	static int                GetNumTextureMax(void);																		// テクスチャ数の最大
	static void               GetFileName(const int nIndex, STRING output);													// ファイル名の取得
	static CONST_STRING       GetFileName(const int nIndex);																// ファイル名の取得
private:
	/* メンバ関数 */
	static void               GetResource(void);																			// 資源の調達
	static void               ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);		// 1行から情報を読み取る
	static void               NumLoad(CONST_STRING cnpLine);																// 個数の読み込み
	static void               FileNameLoad(CONST_STRING cnpLine);															// ファイル名の読み込み
	/* メンバ変数 */
	static MyVector<LPDIRECT3DTEXTURE9> m_pTexture;																			// テクスチャ情報のポインタ
	static MyVector<CString>            m_pFileName;																		// ファイル名のポインタ
	static int                          m_nNumTextureMax;																	// 最大テクスチャ数
};
#endif
