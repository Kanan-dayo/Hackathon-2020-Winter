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
	static CONST D3DXVECTOR2 m_aDiffpos[UI_MAX];
	static CONST D3DXCOLOR   m_aInitUiCol[UI_MAX];
	static CONST D3DXVECTOR2 m_aInitUiSize[UI_MAX];

	MyVector<CScene2D*>  m_pImage;
	D3DXVECTOR3          m_move;
};


#endif