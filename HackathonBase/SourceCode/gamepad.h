//*************************************************************************************************************
//
// ゲームパッド処理 [gamepad.h]
// Author : KOKI NISHIYAMA
//
//*************************************************************************************************************
#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#define _CRT_SECURE_NO_WARNINGS
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "input.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define MAX_GAMEPAD (1)			// ゲームパッドの最大数

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CGamepad : public CInput
{
public:
	/* 構造体番号(enum) */
	typedef enum
	{
		KEY_X = 0,
		KEY_Y,
		KEY_A,
		KEY_B,
		KEY_LEFT_1,
		KEY_RIGHT_1,
		KEY_LEFT_2,
		KEY_RIGHT_2,
		KEY_LEFT_STICK,
		KEY_RIGHT_STICK,
		KEY_BACK,
		KEY_START,
		KEY_LEFT,
		KEY_RIGHT,
		KEY_UP,
		KEY_DOWN,
		KEY_MAX,
	}KEY;

	/* 関数 */
	CGamepad();										// コンストラクタ
	~CGamepad();									// デストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	// 押している状態の取得
	bool GetPress(int nJoy, KEY key);
	// 押した時の状態の取得
	bool GetTrigger(int nJoy, KEY key);
	// 離した時の状態の取得
	bool GetRelease(int nJoy, KEY key);
	// リピート状態の取得
	bool GetRepeat(int nJoy, KEY key);
	// 押した時の左の取得
	int GetTriggerLeft(int nJoy, KEY key);
	// 押した時の右の取得
	int GetTriggerRight(int nJoy, KEY key);
	// 左スティックの取得
	void GetStickLeft(int nJoy, int *pValueH, int *pValueV);
	// 右スティックの取得
	void GetStickRight(int nJoy, int *pValueH, int *pValueV);

protected:
private:
	/* メンバ関数 */
	static BOOL CALLBACK EnumCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// 能力取得
	void SetState(int nJoy);																	// ボタン設定
	/* メンバ変数 */
	static LPDIRECTINPUTDEVICE8 m_pJoyDevice[MAX_GAMEPAD];	// 入力デバイスへのポインタ
	DIJOYSTATE2 m_aState[MAX_GAMEPAD];						// ゲームパッド状態を受け取るワーク
	D3DXVECTOR3 m_aStateAxis[MAX_GAMEPAD];					// スティックの位置情報
	int m_nCntWaitRepeat;									// カウントマックス
	int m_nIntervalRepeat;									// 往復の間
	static int m_nCntpad;									// カウント
	int m_aStateRepeatCnt[MAX_GAMEPAD][KEY_MAX];			// リピート回数
	bool m_abState[MAX_GAMEPAD][KEY_MAX];					// ゲームパッド状態の保存場所
	bool m_abStateTrigger[MAX_GAMEPAD][KEY_MAX];			// ゲームパッド状態のオン・オフ
	bool m_abStateRelease[MAX_GAMEPAD][KEY_MAX];			// ゲームパッド状態の返し
	bool m_abStateRepeat[MAX_GAMEPAD][KEY_MAX];				// ゲームパッド状態を繰り返し

};
#endif
