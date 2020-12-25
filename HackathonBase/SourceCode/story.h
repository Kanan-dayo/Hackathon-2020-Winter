//*************************************************************************************************************
//
// ストーリー処理 [story.h]
// Author:NISHIYAMA KOKI
//
//*************************************************************************************************************
#ifndef _STORY_H_
#define _STORY_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CStory : public CMode
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_STORYBG = 0,
		TYPE_STORY_1,
		TYPE_STORY_2,
		TYPE_STORY_SANTA,
		TYPE_MAX
	} TYPE;



	/* ストーリーとチュートリアルどっちでも使えるかも */
	// 拡大率情報
	typedef struct SCALING
	{
		SCALING()
		{
			OriginSize = D3DXVECTOR2(0.0f, 0.0f);
			fScal = 1.0f;
			fScalChange = 0.0f;
			fScalDiff = 0.0f;
			nCntTimeChange = 0;
		}
		// 設定
		void Set(
			D3DXVECTOR2 const & SouceSize,
			float const & fSouceScalChange
		);
		D3DXVECTOR2 OriginSize;			// 元のサイズ
		int nCntTimeChange;				// 切り替わる時間カウント
		float fScal;					// 現在の拡大率
		float fScalChange;				// 目標拡大率
		float fScalDiff;				// 拡大率上昇率
	} SCALING, *P_SCALING;

	typedef struct PERFORM2DUI
	{
		SCALING * pScal;
		C2DUi * pC2dui;
		bool	bMove;
	} PERFORM2DUI;
	// サイズ変更の更新処理
	static void Update_SizeChange(PERFORM2DUI * pPerfomUi);
	// 演出UIの初期化処理
	static void Init_PerfomUi(int const & nMaxUi, PERFORM2DUI * pPerfomUi);






	/* 関数 */
	CStory() {};						// コンストラクタ
	~CStory() {};						// デストラクタ

	static CStory *Create(void);		// 生成
	void Init(void);					// 初期化
	void Uninit(void);					// 終了
	void Update(void);					// 更新
	void Draw(void);					// 描画

private:
	// 紙芝居移動処理
	void Update_UiMove(int const & nUi);

	/* 変数 */
	PERFORM2DUI m_apPerfomUi[TYPE::TYPE_MAX];	// ２DUIポインタ
	int			m_nPushButton;					// 押したボタン回数
	int			m_nCntFram;						// フレームカウント
};

#endif