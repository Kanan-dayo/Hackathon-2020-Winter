//*************************************************************************************************************
//
// 2DUIの構造体などの処理[2DUITypes.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "2DUITypes.h"
#include "2DUI.h"
#include "Scene2D.h"
#include "NumericString.h"
#include "number.h"

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::BITS::BITS(UINT unMask)
{
	this->unMask = unMask;
}

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::FLASHING::FLASHING() : ABILITYBASE(E_A_FLASHING)
{
	this->bUpdate = true;
	this->nTime   = ML_INT_UNSET;
	this->nTiming = ML_INT_UNSET;
}

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::FLASHING::FLASHING(bool bUpdate, UINT nTiming) : ABILITYBASE(E_A_FLASHING)
{
	this->bUpdate = bUpdate;
	this->nTime   = ML_INT_UNSET;
	this->nTiming = nTiming;
}

//-------------------------------------------------------------------------------------------------------------
// 点滅の更新
//-------------------------------------------------------------------------------------------------------------
void N2Duitypes::FLASHING::Update(C2DUi *pThis)
{
	// 更新フラグがオフの時
	if (!this->bUpdate)
	{// 処理を抜ける
		return;
	}

	// 時間を更新
	this->nTime += 1;

	// 同じ値の時
	if (this->IsTimeEqualTiming())
	{// 描画フラグを切り替える
		pThis->ChangeDispFlag();
		// 時間を初期化する
		this->nTime = ML_INT_UNSET;
	}
}

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::FADE::FADE() : ABILITYBASE(E_A_FADE), bUpdate(true), bLoop(true), fChangeValue(0.0f), nTiming(0), nAddSign(0)
{
	this->bUpdate      = true;
	this->bLoop        = true;
	this->fChangeValue = ML_FLOAT_UNSET;
	this->nTiming      = ML_INT_UNSET;
	this->nAddSign     = ML_INT_UNSET;
}

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::FADE::FADE(bool bUpdate, bool bLoop, int nTiming, int nAddSign) :ABILITYBASE(E_A_FADE), bUpdate(bUpdate), bLoop(bLoop), nTiming(nTiming)
{
	this->bUpdate      = bUpdate;
	this->bLoop        = bLoop;
	this->nTiming      = nTiming;
	this->fChangeValue = 1.0f / nTiming;
	this->SetAddSign(nAddSign);
}

//-------------------------------------------------------------------------------------------------------------
// フェード更新
//-------------------------------------------------------------------------------------------------------------
void N2Duitypes::FADE::Update(C2DUi * pThis)
{
	// 更新フラグがオフの時
	if (!this->bUpdate)
	{// 処理を抜ける
		return;
	}

	// 画像データの取得
	auto &itr = pThis->GetImage();

	// 数字フラグが立っていた時
	if (Mlf_bit_comp(pThis->GetMask(), N2Dui_flag::E_F_NUMBER))
	{// 画像の更新
		this->UpdateImage(itr.pNumber);
	}
	// それ以外
	else
	{// 画像の更新
		this->UpdateImage(itr.pImage);
	}

}

//-------------------------------------------------------------------------------------------------------------
// フェード画像の更新
//-------------------------------------------------------------------------------------------------------------
void N2Duitypes::FADE::UpdateImage(CScene2D * pImage)
{
	// 色の取得
	D3DXCOLOR *pCol = pImage->GetColor();

	// 色の変化
	pCol->a += this->fChangeValue * this->nAddSign;

	// アルファ値を範囲内に制限する
	if (CMylibrary::ClampCol(*pCol) == true)
	{// 直されたとき
		this->nAddSign *= -1;
	}

	// 頂点カラーを更新する
	pImage->SetColflag();
}

//-------------------------------------------------------------------------------------------------------------
// フェード文字数列の更新
//-------------------------------------------------------------------------------------------------------------
void N2Duitypes::FADE::UpdateImage(CNumericString * pNumber)
{
	// 変数宣言
	float fChgAlpha = fChangeValue * nAddSign;	// アルファ値変更値
	bool  bInver    = false;					// 反転フラグ

	// 範囲ループ
	for (auto &itr : pNumber->m_pNumber)
	{
		// 色の取得
		D3DXCOLOR *pCol = itr.GetColorPtr();

		// 色の変化
		pCol->a += fChgAlpha;

		// アルファ値を範囲内に制限する
		if (CMylibrary::ClampCol(*pCol) && !bInver)
		{// 直されたとき反転フラグオン
			bInver = true;
		}

		// カラーフラグを設定
		itr.SetColflag();
	}

	// 反転フラグチェック
	if (bInver)
	{// 符号を反転
		nAddSign *= -1;
	}
}

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::SETING::SETING(BITS mask, int nTextureID, bool bDisp, D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue)
{
	this->mask       = mask;
	this->nTextureID = nTextureID;
	this->bDisp      = bDisp;
	this->pos        = pos;
	this->col        = col;
	this->size       = size;
	this->fRotation  = fRotation;
	this->nValue     = nValue;
}