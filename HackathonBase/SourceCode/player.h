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
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CScene2D;
class CPlayer : public CScene
{
public:

	typedef enum
	{
		IMG_NONE = -1,
		IMG_BODY,
		IMG_GAGE,
		IMG_FRAME,
		IMG_MAX
	}IMG;

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
	MyVector<CScene2D*>  m_pImage;
	D3DXVECTOR3          m_move;
};


#endif