//*************************************************************************************************************
//
// ランキング処理 [ranking.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CRanking : public CMode
{
public:
	typedef enum
	{	// ランキングのUI
		RANKING_RANKED,		// 順位
		RANKING_NUM,		// ペア数
		RANKING_NUMPAIR,	// 〇ペア
		RANKING_MAX
	} RANKING_UI;

	typedef enum
	{	// ランキングの順位
		RANK_1ST,			// 1
		RANK_2ND,			// 2
		RANK_3RD,			// 3
		RANK_4TH,			// 4
		RANK_5TH,			// 5
		RANK_MAX
	} RANKING_RANK;

	CRanking() {};					// コンストラクタ
	~CRanking() {};					// デストラクタ

	static CRanking *Create(void);		// 生成
	static HRESULT LoadUIInfo(void);	// UIの情報のロード
	static HRESULT LoadRank(void);		// ランキングのロード
	static HRESULT SaveRanking(void);		// ランキングのセーブ

	static void SetPlayerScore(int nScore) { m_nPlayerScore = nScore; m_bSort = true; }	// プレイヤースコアの設定

	void Init(void);				// 初期化
	void Uninit(void);				// 終了
	void Update(void);				// 更新
	void Draw(void);				// 描画

private:
	void SortScore(void);
	static D3DXVECTOR3 m_posUI[RANK_MAX][RANKING_MAX];		// UIの座標
	static D3DXVECTOR2 m_sizeUI[RANK_MAX][RANKING_MAX];		// UIのサイズ
	static int		   m_nNumPair[RANK_MAX];				// ペア数

	static int m_nPlayerScore;								// プレイヤーのスコア
	static int m_nPlayerRank;								// プレイヤーの順位
	static bool m_bSort;									// ソートするか

	C2DUi *m_p2DUI[RANK_MAX][RANKING_MAX];					// ２DUIポインタ
	C2DUi *m_pUIRaking;										// ランキングUI
	C2DUi *m_pUIBG;											// 背景UI
};

#endif