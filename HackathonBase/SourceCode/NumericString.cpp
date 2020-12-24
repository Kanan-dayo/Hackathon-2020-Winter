//*************************************************************************************************************
//
// 数字文字列処理[NumericString.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "NumericString.h"
#include "number.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumericString::CNumericString()
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CNumericString::~CNumericString()
{
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CNumericString * CNumericString::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue, int nNumNumber)
{
	CNumericString *pNumericString = new CNumericString;
	pNumericString->Init(pos, col, size, fRotation, nValue, nNumNumber);
	return pNumericString;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Init(D3DXVECTOR3 & pos, D3DXCOLOR & col, D3DXVECTOR2 & size, float & fRotation, int & nValue, int& nNumNumber)
{
	m_col = col;				// 色
	m_pos = pos;				// 位置
	m_size = size;				// 大きさ
	m_nValue = nValue;			// 時間の値
	m_nAddSign = -1;			// 加算する符号
	m_fRotation = fRotation;	// 回転量

	// 桁数から使う個数を算出
	m_nDigits = 0;
	CMylibrary::GetDigits(m_nDigits, m_nValue);
	// 使う個数が-1の時
	if (nNumNumber == -1)
	{
		m_nNumNumber = m_nDigits;
	}

	// メモリを確保
	m_pNumber.resize(m_nNumNumber);

	// 使う数字分ループ
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; nCntNumber++)
	{// 桁の数字を算出
		int nDigits =
			m_nValue %
			(int)powf(10.0f, (float)m_nNumNumber - nCntNumber) /
			(int)powf(10.0f, (float)m_nNumNumber - 1.0f - nCntNumber);
		// 位置の設定
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.z = 0.0f;
		// 初期化
		m_pNumber[nCntNumber].Init(m_size.x, m_size.y, nDigits, m_fRotation, pos, m_col);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Uninit(void)
{
	for (auto itr : m_pNumber)
	{
		itr.Unint();
	}
	m_pNumber.clear();
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Update(void)
{
	for (auto itr : m_pNumber)
	{
		itr.Update();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Draw(void)
{
	for (auto itr : m_pNumber)
	{
		itr.Draw();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 値の加算
//-------------------------------------------------------------------------------------------------------------
void CNumericString::AddValue(void)
{
	// 開始値に符号を加算
	m_nValue += m_nAddSign;
}

//-------------------------------------------------------------------------------------------------------------
// 数字の更新
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdateNumber(void)
{
	// 使う数字分ループ
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; ++nCntNumber)
	{// 桁の数字を算出
		int nDigits = m_nValue %
			(int)powf(10.0f, (float)m_nNumNumber - nCntNumber) /
			(int)powf(10.0f, (float)m_nNumNumber - 1.0f - nCntNumber);
		// 初期化
		m_pNumber[nCntNumber].SetNumber(nDigits);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 位置の更新
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdatePos(void)
{
	// 使う数字分ループ
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; ++nCntNumber)
	{		// 位置の設定
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.z = 0.0f;
		m_pNumber[nCntNumber].SetPosition(pos);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 大きさの更新
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdateSize(void)
{
	// 使う数字分ループ
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; ++nCntNumber)
	{		// 位置の設定
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x * 0.75f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x * 0.75f));
		pos.z = 0.0f;
		m_pNumber[nCntNumber].SetSize(m_size);
		m_pNumber[nCntNumber].SetPosition(pos);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 回転の更新
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdateRotation(void)
{
	// 使う数字分ループ
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; ++nCntNumber)
	{		// 位置の設定
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.z = 0.0f;
		m_pNumber[nCntNumber].SetRotation(m_fRotation);
		m_pNumber[nCntNumber].SetPosition(pos);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 色の更新
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdateCol(void)
{
	for (auto &itr : m_pNumber)
	{
		itr.SetColor(m_col);
	}
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャの連結
//-------------------------------------------------------------------------------------------------------------
void CNumericString::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	for (auto &itr : m_pNumber)
	{
		itr.BindTexture(pTexture);
	}
}
