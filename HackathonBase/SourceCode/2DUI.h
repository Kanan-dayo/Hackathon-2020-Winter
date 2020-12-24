//*************************************************************************************************************
//
// 2DUI処理[2DUI.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _2DUI_H_
#define _2DUI_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "2DUITypes.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CNumericString;
class CScene2D;
class C2DUi : public CScene
{
public:


	typedef union
	{
		void*           pVoidptr;		// nullcheck
		N2Dui_abibase*  pBase;			// ベース
		N2Dui_flashing* pFlashing;		// 点滅のポインタ
		N2Dui_fade*     pFade;			// フェードのポインタ
	} ABILITY;	

	typedef union IMAGE
	{
		IMAGE() { pVoidptr = nullptr; }
		void*           pVoidptr;		// nullcheck
		CNumericString* pNumber;		// 数字のポインタ
		CScene2D*       pImage;			// 画像のポインタ
	}IMAGE;

	C2DUi() : CScene(PRIORITY_BUI) {}																							// コンストラクタ
	~C2DUi() {}																													// デストラクタ

	static C2DUi * Create(N2Dui_seting &seting, PRIORITY pri);																					// 生成
	void Init(N2Dui_seting &seting, PRIORITY& pri);																								// 初期化
	void Init(void);																											// 初期化
	void Uninit(void);																											// 終了
	void Update(void);																											// 更新
	void Draw(void);																											// 描画

	void          ChangeDispFlag(void);																							// 描画フラグの切り替え

	inline void   SetTextureID(int nTextureID) { m_nTextureID = nTextureID; }													// テクスチャIDの設定
	inline int&   GetTextureID(void) { return m_nTextureID; }																	// テクスチャIDの取得
	inline void   SetDisp(bool bDisp) { m_bDisp = bDisp; ReflectDispFlagInImage(); }											// 描画フラグの設定
	inline bool&  GetDisp(void) { return m_bDisp; }																				// 描画フラグの取得

	void SetFadeAbility(N2Dui_fade &fade);
	void SetFlashingAbility(N2Dui_flashing &flashing);

	ABILITY *GetAbilityDataTopPtr(void);																						// 先頭のアビリティデータのポインタの取得
	ABILITY &GetAbilityData(N2Dui_ability type);																				// アビリティデータの取得

	UINT &GetMask(void) { return m_mask.unMask; }																				// マスクの取得
	IMAGE &GetImage(void) { return m_Image[m_ImageType]; }																		// 画像の取得(数字も)
	IMAGE *GetImagePtr(void) { return &m_Image[m_ImageType]; }																	// 画像のポインタの取得(数字も)
protected:
	void ReflectDispFlagInImage(void);																							// 描画フラグを画像に反映
	int                    m_nTextureID;														// テクスチャID
	bool                   m_bDisp;																// 描画フラグ
	MyVector<ABILITY>      m_AbilityData;														// アビリティデータ
	MyVector<IMAGE>        m_Image;																// 画像データ
	N2Dui_bits             m_mask;																// ビットフラグ
	N2Dui_img              m_ImageType;

};


#endif