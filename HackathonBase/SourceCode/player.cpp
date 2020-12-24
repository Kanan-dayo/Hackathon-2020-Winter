//*************************************************************************************************************
//
// プレイヤーの処理[player.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "player.h"
#include "Scene2D.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "DebugProc.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define PLAYER_GAGEFRAME_SIZE_Y (40.0f)							// ゲージフレームのサイズY
#define PLAYER_GAGE_SIZE_X      (200.0f)						// ゲージのサイズX
#define PLAYER_FRAME_SIZE_X     (200.0f)						// 枠のサイズX
#define PLAYER_ROTVALUECOEFFI	(0.05f)							// 回転値係数
#define PLAYER_SPEEDLIMIT		(3.0f)							// 速度制限
#define PLAYER_CHARGE			(2.0f)							// 速度制限

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CONST D3DXVECTOR3 CPlayer::m_aDiffpos[CPlayer::UI_MAX] = {
	{ -PLAYER_FRAME_SIZE_X * ML_HALF_SIZE,-60.0f,0.0f },
	{ -PLAYER_FRAME_SIZE_X * ML_HALF_SIZE,-60.0f,0.0f },
};

CONST D3DXCOLOR   CPlayer::m_aInitUiCol[UI_MAX] = {
	{ 1.0f,1.0f,1.0f,1.0f },
	{ 0.5f,0.5f,0.5f,1.0f },
};

CONST D3DXVECTOR2 CPlayer::m_aInitUiSize[UI_MAX] = {
	{ 0.0f, PLAYER_GAGEFRAME_SIZE_Y },
	{ PLAYER_FRAME_SIZE_X, PLAYER_GAGEFRAME_SIZE_Y },
};

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CPlayer *pPlayer = new CPlayer;
	pPlayer->InitImage(pos, size);
	pPlayer->Init();
	return pPlayer;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	m_move = ML_VEC3_UNSET;
	m_fRotDest = ML_FLOAT_UNSET;
	m_fSpeed = ML_FLOAT_UNSET;
	m_state = STATE_NORMAL;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	// ゲームパッド操作処理
	if (GamepadOperationProc())
	{

	}
	// キーボード操作処理
	else if (KeyboardOperationProc())
	{
	}
	else
	{
	}

	// 体の行動処理
	BodyAction();

	// 頂点情報の更新
	for (auto& itr : m_pImage) {
		itr->UpdateVertex();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 画像の初期化
//-------------------------------------------------------------------------------------------------------------
void CPlayer::InitImage(D3DXVECTOR3 &pos, D3DXVECTOR2 &size)
{
	// サイズの再設定
	m_pImage.resize(IMG_MAX);
	// プレイヤーの体
	m_pImage[IMG_BODY] = CScene2D::Create(PRIORITY_CHARACTER, pos, ML_D3DXCOR_SET, ORIGINVERTEXTYPE_ROTCENTER, size);
	m_pImage[IMG_BODY]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_PLAYER));
	// ゲージ
	m_pImage[IMG_GAGE] = CScene2D::Create(PRIORITY_CHARACTER, pos + m_aDiffpos[UI_GAGE], m_aInitUiCol[UI_GAGE], ORIGINVERTEXTYPE_CENTERLEFT, m_aInitUiSize[UI_GAGE]);
	m_pImage[IMG_GAGE]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_GAGE));
	// 枠
	m_pImage[IMG_FRAME] = CScene2D::Create(PRIORITY_CHARACTER, pos + m_aDiffpos[UI_GAGE], m_aInitUiCol[UI_FRAME], ORIGINVERTEXTYPE_CENTERLEFT, m_aInitUiSize[UI_FRAME]);
	m_pImage[IMG_FRAME]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_FRAME));
}

//-------------------------------------------------------------------------------------------------------------
// ゲームパッド操作処理
//-------------------------------------------------------------------------------------------------------------
bool CPlayer::GamepadOperationProc(void)
{
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// キーボード操作処理
//-------------------------------------------------------------------------------------------------------------
bool CPlayer::KeyboardOperationProc(void)
{
	// マネージャーの取得
	Ckeyboard &Keyboard = CManager::GetKeyboard();
	VEC3 direNor = ML_VEC3_UNSET;
	bool bOperat = false;

	if (Keyboard.GetPress(DIK_SPACE))
	{
		float fSizeX = m_pImage[IMG_GAGE]->GetSize()->x + PLAYER_CHARGE;
		if (fSizeX > PLAYER_GAGE_SIZE_X)
		{
			fSizeX = PLAYER_GAGE_SIZE_X;
		}
		m_pImage[IMG_GAGE]->SetSizeX(fSizeX);
	}
	else if (Keyboard.GetRelease(DIK_SPACE))
	{
		m_pImage[IMG_GAGE]->SetSizeX(0.0f);
	}
	else
	{
		if (Keyboard.GetPress(DIK_A))
		{
			direNor.x = -1.0f;
			bOperat = true;
		}
		if (Keyboard.GetPress(DIK_D))
		{
			direNor.x = 1.0f;
			bOperat = true;
		}
		if (Keyboard.GetPress(DIK_S))
		{
			direNor.y = -1.0f;
			bOperat = true;
		}
		if (Keyboard.GetPress(DIK_W))
		{
			direNor.y = 1.0f;
			bOperat = true;
		}
	}


	if (bOperat)
	{
		m_fRotDest = atan2f(direNor.x, direNor.y);
		m_fSpeed += 0.05f;
	}
	else
	{
		m_fSpeed -= 0.05f;
	}

	return bOperat;
}

//-------------------------------------------------------------------------------------------------------------
// 体の行動処理
//-------------------------------------------------------------------------------------------------------------
void CPlayer::BodyAction(void)
{
	// 変数宣言
	float fRotation = m_pImage[IMG_BODY]->GetRotation();
	float fDirecRot = m_fRotDest - fRotation;
	if (fDirecRot != 0.0f)
	{
		CMylibrary::SetFixTheRotation(&fDirecRot);
		if (Mlf_OutIncRange(fDirecRot, PLAYER_ROTVALUECOEFFI * 2.0f, -PLAYER_ROTVALUECOEFFI * 2.0f))
		{
			fRotation += (fDirecRot / abs(fDirecRot)) * PLAYER_ROTVALUECOEFFI;
			m_pImage[IMG_BODY]->SetRotation(fRotation);
		}
		else
		{
			m_pImage[IMG_BODY]->SetRotation(m_fRotDest);
		}
	}

	D3DXVECTOR3* pPos = m_pImage[IMG_BODY]->GetPosition();

	if (m_fSpeed > PLAYER_SPEEDLIMIT)
	{
		m_fSpeed = PLAYER_SPEEDLIMIT;
	}
	else if(m_fSpeed < ML_FLOAT_UNSET)
	{
		m_fSpeed = ML_FLOAT_UNSET;
	}

	if (m_fSpeed != 0.0f)
	{
		m_move.x = sinf(fRotation);
		m_move.y = -cosf(fRotation);
		m_move.Norm();
		m_move *= m_fSpeed;
		*pPos += m_move;

		m_pImage[IMG_BODY]->SetPosition(*pPos);
		m_pImage[IMG_GAGE]->SetPosition(*pPos + m_aDiffpos[UI_GAGE]);
		m_pImage[IMG_FRAME]->SetPosition(*pPos + m_aDiffpos[UI_FRAME]);
	}
}
