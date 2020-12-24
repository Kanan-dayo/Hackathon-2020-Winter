//*************************************************************************************************************
//
// チュートリアル処理 [tutorial.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CTutorial : public CMode
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_NAME = 0,
		TYPE_KEYINFO,
		TYPE_MAX
	} TYPE;

	/* タイトルとチュートリアルどっちでも使えるかも */
	// 拡大率情報
	typedef struct SCALING
	{
		SCALING()
		{
			OriginSize = D3DXVECTOR2(0.0f, 0.0f);
			fScal = 1.0f;
			fScalChange = 0.0f;
			fScalDiff = 0.0f;
			nTimeChange = 0;
			nCntTimeChange = 0;
			bChange = false;
		}
		// 設定
		void Set(
			D3DXVECTOR2 const & SouceSize,
			float const & fSouceScalChange,
			int const & nSouceTimeChange
		)
		{
			OriginSize = SouceSize;
			fScalChange = fSouceScalChange;
			nTimeChange = nSouceTimeChange;
			fScalDiff = (fScalChange - fScal) / nTimeChange;
		}
		D3DXVECTOR2 OriginSize;			// 元のサイズ
		float fScal;					// 拡大率
		float fScalChange;				// 目標拡大率
		float fScalDiff;				// 拡大率上昇率
		int nTimeChange;				// 切り替わる時間
		int nCntTimeChange;				// 切り替わる時間カウント
		bool bChange;					// 切り替わり中
	} SCALING, *P_SCALING;

	typedef struct PERFORM2DUI
	{
		SCALING * pScal;
		C2DUi * pC2dui;
	} PERFORM2DUI;
	// サイズ変更の更新処理
	static void Update_SizeChange(PERFORM2DUI * pPerfomUi);
	// 演出UIの初期化処理
	static void Init_PerfomUi(int const & nMaxUi, PERFORM2DUI * pPerfomUi);


	/* 関数 */
	CTutorial() {};					// コンストラクタ
	~CTutorial() {};					// デストラクタ

	static CTutorial *Create(void);	// 生成
	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

private:
	PERFORM2DUI m_apPerfomUi[TYPE::TYPE_MAX];	// ２DUIポインタ
};

#endif