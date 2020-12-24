//*************************************************************************************************************
//
// リザルト処理 [result.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CResult : public CMode
{
public:
	typedef enum
	{
		RESULT_TITLE = 0,	// リザルト
		RESULT_TEXT,		// 破壊したペア数
		RESULT_NUM,			// ペア数
		RESULT_NUMPAIR,		// 〇ペア
		RESULT_MAX
	} RESULT_UI;

	CResult() {};					// コンストラクタ
	~CResult() {};					// デストラクタ

	static CResult *Create(void);		// 生成
	static HRESULT LoadUIInfo(void);	// UIの情報のロード

	static void SetNumPair(int nPair) { m_nNumPair = nPair; }	// ペア数の設定

	void Init(void);					// 初期化
	void Uninit(void);					// 終了
	void Update(void);					// 更新
	void Draw(void);					// 描画

private:
	static D3DXVECTOR3 m_posUI[RESULT_MAX];		// UIの座標
	static D3DXVECTOR2 m_sizeUI[RESULT_MAX];	// UIのサイズ
	static int m_nNumPair;						// ペア数

	C2DUi *m_p2DUI[RESULT_MAX];					// ２DUIポインタ
};

#endif