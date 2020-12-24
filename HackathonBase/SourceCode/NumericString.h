//*************************************************************************************************************
//
// 数字文字列処理[NumericString.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _NUMERICSTRING_H_
#define _NUMERICSTRING_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CNumber;
class CNumericString
{
public:
	CNumericString();				// コンストラクタ
	~CNumericString();				// デストラクタ

	static CNumericString *Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue, int nNumNumber = -1);

	void Init(D3DXVECTOR3 &pos, D3DXCOLOR &col, D3DXVECTOR2 &size, float &fRotation, int &nValue, int &nNumNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddValue(void);

	// 更新
	void UpdateNumber(void);
	void UpdatePos(void);
	void UpdateSize(void);
	void UpdateRotation(void);
	void UpdateCol(void);

	// 設定
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);									// テクスチャを関連付ける
	inline void SetPos(D3DXVECTOR3 &pos) { m_pos = pos; }
	inline void SetCol(D3DXCOLOR &col) { m_col = col; }
	inline void SetSize(D3DXVECTOR2 &size) { m_size = size; }
	inline void SetRotation(float fRotation) { m_fRotation = fRotation; }
	inline void SetValue(int nValue) { m_nValue = nValue; }

	// 取得
	inline int &GetValue(void) { return m_nValue; }				// 値の取得
	inline int &GetDigits(void) { return m_nDigits; }			// 桁数(描画数)の取得
	inline int &GetNumNumber(void) { return m_nNumNumber; }		// 使う個数の取得
	inline int &GetAddSign(void) { return m_nAddSign; }			// 加算する符号の取得
	inline D3DXCOLOR & GetCol(void) { return m_col; }			// 色の取得
	inline D3DXVECTOR3 & GetPos(void) { return m_pos; }			// 位置の取得
	inline D3DXVECTOR2 GetSize(void) { return m_size; }			// 大きさの取得
	inline float &GetRotation(void) { return m_fRotation; }		// 回転量の取得

	MyVector<CNumber>			m_pNumber;			// 数字情報
private:
	int							m_nValue;			// 値
	int							m_nDigits;			// 桁数(描画数)
	int							m_nNumNumber;		// 使う個数
	int							m_nAddSign;			// 加算する符号
	D3DXCOLOR					m_col;				// 色
	D3DXVECTOR3					m_pos;				// 位置
	D3DXVECTOR2					m_size;				// 大きさ
	float						m_fRotation;		// 回転量
};

#endif