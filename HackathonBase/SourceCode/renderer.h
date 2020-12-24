//*************************************************************************************************************
//
// レンダラー処理[renderer.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "main.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CFade;
class CRenderer
{
public:
	typedef struct
	{
		D3DXVECTOR3 pos;						// 頂点の座標
		float		rhw;						// 1.0で固定同次座標
		D3DCOLOR	col;						// 頂点カラー
		D3DXVECTOR2	tex;						// テクスチャ座標
	} VERTEX_2D;

	// ３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
	typedef struct
	{
		D3DXVECTOR3 pos;	// 頂点座標
		D3DXVECTOR3 nor;	// 法線ベクトル
		D3DCOLOR	col;	// 頂点カラー
		D3DXVECTOR2 tex;	// テクスチャ座標
	} VERTEX_3D;

	// 合成演算
	typedef enum
	{
		BLEND_NORMAL = 0,		// 通常
		BLEND_TRANSLUCENT,		// 半透明
		BLEND_ADD,				// 加算合成
		BLEND_ADD_TRANSLUCENT,	// 加算半透明
		BLEND_SUBTRACTION,		// 減算
		BLEND_MULTIPUL1,		// 乗算1
		BLEND_MULTIPUL2,		// 乗算2
		BLEND_SCREEN,			// スクリーン
		BLEND_REVERSE,			// リバース
		BLEND_MAX				// 最大数
	} BLEND;


	/* -- メンバ関数 -- */
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, BOOL bWindow);		// 初期化処理
	void Uninit(void);							// 終了処理
	void UpDate(void);							// 更新処理
	void Draw(void);							// 描画処理

	// 合成処理
	//	blend	: 合成演算
	void SetBlend(
		BLEND const &blend = BLEND_TRANSLUCENT	// 合成演算
	);


	inline LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }			// デバイスの取得
	inline CFade *GetFade(void) { return m_pFade; }								// フェードの取得
#ifdef _DEBUG
	inline void SetFPS(int fps) { m_nCountFPS = fps; }
#endif

private:
	/* -- メンバ関数 -- */

	/* -- メンバ変数 -- */
	LPDIRECT3D9			m_pD3D = NULL;				// Direct3Dオブジェクトのポインタ
	LPDIRECT3DDEVICE9	m_pD3DDevice = NULL;		// DirectX3Dデバイスのポインタ
	static CFade		*m_pFade;					// フェード
#ifdef _DEBUG
	int			m_nCountFPS = 0;	// FPSカウンタ
#endif

protected:
};

#endif