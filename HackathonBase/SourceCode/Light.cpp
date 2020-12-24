//*************************************************************************************************************
//
// 入力処理[keyboard.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Light.h"
#include "manager.h"
#include "DebugProc.h"
#include "keyboard.h"
#include "renderer.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define LIGHT_0_SELECT_DIRECTION	(D3DXVECTOR3(0.0f,-0.6f,0.8f))			// 選択モード時のライト0の向き
#define LIGHT_1_SELECT_DIRECTION	(D3DXVECTOR3(0.96f,-0.0f,0.5f))			// 選択モード時のライト1の向き
#define LIGHT_2_SELECT_DIRECTION	(D3DXVECTOR3(-0.53f,-0.4f,0.74f))		// 選択モード時のライト2の向き

#define LIGHT_0_UNSET_DIRECTION		(D3DXVECTOR3(0.22f, -0.87f, 0.44f))		// 特に設定なしのライト0の向き
#define LIGHT_1_UNSET_DIRECTION		(D3DXVECTOR3(-0.18f, 0.88f, -0.44f))	// 特に設定なしのライト1の向き
#define LIGHT_2_UNSET_DIRECTION		(D3DXVECTOR3(0.89f, -0.11f, 0.44f))		// 特に設定なしのライト2の向き


#define LIGHT_0_UNSET_COLOR			(D3DXCOLOR( 1.0f,1.0f,1.0f,1.0f))		// 特に設定なしのライト0の色
#define LIGHT_1_UNSET_COLOR			(D3DXCOLOR(0.65f,0.65f,0.65f,0.65f))	// 特に設定なしのライト1の色
#define LIGHT_2_UNSET_COLOR			(D3DXCOLOR(0.15f,0.15f,0.15f,0.15f))	// 特に設定なしのライト2の色

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();	// デバイスの取得
	// 向きの設定
	D3DXVECTOR3 vecDir[LIGHT_MAX] = {
		{ LIGHT_0_UNSET_DIRECTION },
		{ LIGHT_1_UNSET_DIRECTION },
		{ LIGHT_2_UNSET_DIRECTION },
	};
	// 色の設定
	D3DXCOLOR col[LIGHT_MAX] = {
		{ LIGHT_0_UNSET_COLOR },
		{ LIGHT_1_UNSET_COLOR },
		{ LIGHT_2_UNSET_COLOR },
	};

	for (int nCount = 0; nCount < LIGHT_MAX; nCount++)
	{
		// ライトをクリアする
		ZeroMemory(&m_aLight[nCount], sizeof(D3DLIGHT9));

		// ライトの種類を設定		(_DIRECTIONAL | _SPOT | _POINT)
		m_aLight[nCount].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		m_aLight[nCount].Diffuse = col[nCount];
		// 正規化
		D3DXVec3Normalize(&vecDir[nCount], &vecDir[nCount]);
		// 向きを代入
		m_aLight[nCount].Direction = vecDir[nCount];
		// ライトを設定する
		pDevice->SetLight(nCount, &m_aLight[nCount]);

		// ライトを有効にする
		pDevice->LightEnable(nCount, TRUE);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CLight::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CLight::Update(void)
{

}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CLight * CLight::Create(void)
{
	// ライトの生成
	CLight *pLight = new CLight;
	// ライトの初期化
	pLight->Init();
	return pLight;
}

//-------------------------------------------------------------------------------------------------------------
// ライトの方向の設定
//-------------------------------------------------------------------------------------------------------------
void CLight::SetDirection(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();	// デバイスの取得

	// 方向ベクトル
	D3DXVECTOR3 vecDir[LIGHT_MAX] = {};
	vecDir[0] = LIGHT_0_SELECT_DIRECTION;
	vecDir[1] = LIGHT_1_SELECT_DIRECTION;
	vecDir[2] = LIGHT_2_SELECT_DIRECTION;
	// ライトの最大数分ループ
	for (int nCount = 0; nCount < LIGHT_MAX; nCount++)
	{// 正規化
		D3DXVec3Normalize(&vecDir[nCount], &vecDir[nCount]);
		// 向きを代入
		m_aLight[nCount].Direction = vecDir[nCount];
		// ライトを設定する
		pDevice->SetLight(nCount, &m_aLight[nCount]);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ライト情報の取得
//-------------------------------------------------------------------------------------------------------------
D3DLIGHT9 * CLight::GetLight(int nNumLight)
{
	return &m_aLight[nNumLight];
}
