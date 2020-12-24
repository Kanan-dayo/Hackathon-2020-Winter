//*************************************************************************************************************
//
// プレイヤー処理[player.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CScene2D;
class CPlayer : public CScene
{
public:

	typedef enum
	{
		STATE_NONE = -1,
		STATE_NORMAL,
		STATE_CHARGE,
		STATE_SMASH,
		STATE_MAX
	}STATE; 

	// 画像
	enum
	{
		IMG_NONE = -1,		// 無し
		IMG_BODY,			// 体
		IMG_GAGE,			// ゲージ
		IMG_FRAME,			// 枠
		IMG_MAX				// 最大
	};

	// UI
	enum
	{
		UI_NONE = -1,		// 無し
		UI_GAGE,			// ゲージ
		UI_FRAME,			// 枠
		UI_MAX				// 最大
	};

	CPlayer() : CScene(PRIORITY_CHARACTER) {}									// コンストラクタ
	~CPlayer() {}																// デストラクタ

	static CPlayer *     Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);				// 生成
	void                 Init(void);											// 初期化
	void                 Uninit(void);											// 終了
	void                 Update(void);											// 更新
	void                 Draw(void);											// 描画
	void                 InitImage(D3DXVECTOR3 &pos, D3DXVECTOR2 &size);		// 画像の初期化

	inline CScene2D*     GetImage(UINT nIndex) { return m_pImage[nIndex]; }		// 画像の取得

protected:
	void NormalProc(void);
	void SmashProc(void);
	bool GamepadOperationProc(void);											// ゲームパッド操作処理
	bool KeyboardOperationProc(void);											// キーボード操作処理
	void BodyAction(void);														// 体の行動処理
	static CONST D3DXVECTOR3 m_aDiffpos[UI_MAX];								// 位置の差
	static CONST D3DXCOLOR   m_aInitUiCol[UI_MAX];								// UIの初期化カラー
	static CONST D3DXVECTOR2 m_aInitUiSize[UI_MAX];								// UIの初期化大きさ

	MyVector<CScene2D*>  m_pImage;												// 画像ポインタ
	VEC3                 m_move;												// 移動量
	float                m_fRotDest;											// 回転の到達値
	float                m_fSpeed;												// 速度
	STATE                m_state;												// 状態
};


#endif