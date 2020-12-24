//*************************************************************************************************************
//
// モード処理 [mode.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _MODE_H_
#define _MODE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "manager.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CMode
{
public:
	typedef enum
	{
		STATE_NONE = 0,		// 無し
		STATE_IN,			// 入り
		STATE_NORMAL,		// 通常
		STATE_OUT,			// 抜ける
		STATE_MAX			// 最大
	}STATE;

	CMode() {};															// コンストラクタ
	~CMode() {};														// デストラクタ

	virtual void Init(void) = 0;										// 初期化
	virtual void Uninit(void) = 0;										// 終了
	virtual void Update(void) = 0;										// 更新
	virtual void Draw(void) = 0;										// 描画

	void          Init(STATE State,int nMaxCnt);						// 初期化

	inline void   SetState(STATE rhs)      { m_State = rhs; }			// 状態の設定
	inline STATE& GetState(void)           { return m_State; }			// 状態の取得
	inline void   SetMaxCntState(UINT rhs) { m_nMaxCntState = rhs; }	// 切り替わるカウントの最大数の設定
	inline UINT&  GetMaxCntState(void)     { return m_nMaxCntState; }	// 切り替わるカウントの最大数の取得
	inline void   SetCntState(UINT rhs)    { m_nCntState = rhs; }		// 切り替わるカウントの設定
	inline UINT&  GetCntState(void)        { return m_nCntState; }		// 切り替わるカウントの取得

	bool          ChangeState(STATE State);			// 状態の切り替え

protected:
	STATE m_State;								// 状態
	UINT  m_nMaxCntState;						// 切り替わるカウントの最大数
	UINT  m_nCntState;							// 切り替わるカウント
};

#endif