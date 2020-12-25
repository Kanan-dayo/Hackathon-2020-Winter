//*************************************************************************************************************
//
// エネミー処理[enemy.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
class CEnemy : public CScene
{
public:

	typedef enum
	{
		STATE_NONE = -1,		// 無し
		STATE_STANDBY,			// スタンバイ
		STATE_NORMAL,			// 通常
		STATE_DAMAGE,			// ダメージ
		STATE_DIE,				// 死
		STATE_MAX				// 最大数
	}STATE;

	typedef enum
	{
		TYPE_NONE = -1,		// 無し
		TYPE_COUPLE,		// 夫婦
		TYPE_NEWLYWED,		// 新婚
		TYPE_STUDENT,		// 学生
		TYPE_MAX			// 最大数
	}TYPE;

	// 画像
	enum
	{
		IMG_NONE = -1,		// 無し
		IMG_HEART,			// ハート
		IMG_CHAR1,			// キャラ1
		IMG_CHAR2,			// キャラ2
		IMG_MAX				// 最大
	};

	// UI
	enum
	{
		UI_NONE = -1,		// 無し
		UI_CHAR1,			// キャラ1
		UI_CHAR2,			// キャラ2
		UI_MAX				// 最大
	};

	CEnemy() : CScene(PRIORITY_CHARACTER) {}									// コンストラクタ
	~CEnemy() {}																// デストラクタ

	static CEnemy *      Create(D3DXVECTOR3 pos, TYPE type);					// 生成
	void                 Init(void);											// 初期化
	void                 Uninit(void);											// 終了
	void                 Update(void);											// 更新
	void                 Draw(void);											// 描画
	void                 InitImage(D3DXVECTOR3 &pos, TYPE& type);				// 画像の初期化
	STATE                GetState(void) { return m_state; }
	void                 SetState(CONST STATE state);							// 状態の設定
	bool                 DamageProc(VEC3& move,D3DXVECTOR3 *pPos);									// ダメージ処理
	inline CScene2D*     GetImage(UINT nIndex) { return m_pImage[nIndex]; }		// 画像の取得
	void                 SetAppearance(D3DXVECTOR3 &pos);
protected:
	void NormalProc(void);														// 通常処理
	void DieProc(void);															// 死ぬ処理
	void DamageProc(void);														// ダメージ処理
	static D3DXCOLOR m_LifeCol[3];												// ライフの色
	static CONST D3DXVECTOR3 m_aDiffpos[UI_MAX];								// 位置の差
	static CONST D3DXVECTOR2 m_aInitUiSize[UI_MAX];								// UIの初期化大きさ
	VEC3                 m_Scatter[UI_MAX];										// 散る
	TYPE                 m_type;												// 種類
	vector<CScene2D*>    m_pImage;												// 画像ポインタ
	VEC3                 m_move;												// 移動量
	float                m_fSpeed;												// 速度
	STATE                m_state;												// 状態
	UINT                 m_nCntState;											// 状態カウント
	int                  m_nLife;												// ライフ
	int                  m_nCntAction;											// 行動カウント
};


#endif