//*************************************************************************************************************
//
// �v���C���[�̏���[player.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "player.h"
#include "Scene2D.h"
#include "texture.h"
#include "manager.h"
#include "keyboard.h"
#include "DebugProc.h"
#include "XGamepad.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define PLAYER_GAGEFRAME_SIZE_Y (40.0f)							// �Q�[�W�t���[���̃T�C�YY
#define PLAYER_GAGE_SIZE_X      (200.0f)						// �Q�[�W�̃T�C�YX
#define PLAYER_FRAME_SIZE_X     (200.0f)						// �g�̃T�C�YX
#define PLAYER_ROTVALUECOEFFI	(0.1f)							// ��]�l�W��
#define PLAYER_SPEEDLIMIT		(3.0f)							// ���x����
#define PLAYER_CHARGE			(2.0f)							// ���x����
#define PLAYER_SMASHSPEED		(250.0f)						// �X�}�b�V�����x
//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
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
// ����
//-------------------------------------------------------------------------------------------------------------
CPlayer * CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CPlayer *pPlayer = new CPlayer;
	pPlayer->InitImage(pos, size);
	pPlayer->Init();
	return pPlayer;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Init(void)
{
	m_move = ML_VEC3_UNSET;
	m_fRotDest = ML_FLOAT_UNSET;
	m_fSpeed = ML_FLOAT_UNSET;
	m_state = STATE_NORMAL;
	m_nCntState = ML_INT_UNSET;
	m_bGamePadcharge = false;
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Update(void)
{
	switch (m_state)
	{
		MLK_CASE(STATE_NORMAL)MLK_CASES(STATE_CHARGE)NormalProc();
		MLK_CASE(STATE_SMASH)SmashProc();
		MLK_CASE(STATE_RECOIL)RecoilProc();
	MLK_DEFAULT:break;
	}






	// ��ʏ���

	D3DXVECTOR3* pPos = m_pImage[IMG_BODY]->GetPosition();

	if (pPos->y <= 0.0f)
	{
		pPos->y = 0.0f;
	}
	else if (pPos->y >= 720.0f)
	{
		pPos->y = 720.0f;
	}

	if (pPos->x <= 0.0f)
	{
		pPos->x = 0.0f;
	}
	else if (pPos->x >= 1280.0f)
	{
		pPos->x = 1280.0f;
	}

	m_pImage[IMG_BODY]->SetPosition(*pPos);
	m_pImage[IMG_GAGE]->SetPosition(*pPos + m_aDiffpos[UI_GAGE]);
	m_pImage[IMG_FRAME]->SetPosition(*pPos + m_aDiffpos[UI_FRAME]);


	// ���_���̍X�V
	for (auto& itr : m_pImage) {
		itr->UpdateVertex();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CPlayer::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �摜�̏�����
//-------------------------------------------------------------------------------------------------------------
void CPlayer::InitImage(D3DXVECTOR3 &pos, D3DXVECTOR2 &size)
{
	// �T�C�Y�̍Đݒ�
	m_pImage.resize(IMG_MAX);
	// �v���C���[�̑�
	m_pImage[IMG_BODY] = CScene2D::Create(PRIORITY_CHARACTER, pos, ML_D3DXCOR_SET, ORIGINVERTEXTYPE_ROTCENTER, size);
	m_pImage[IMG_BODY]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_PLAYER));
	// �Q�[�W
	m_pImage[IMG_GAGE] = CScene2D::Create(PRIORITY_CHARACTER, pos + m_aDiffpos[UI_GAGE], m_aInitUiCol[UI_GAGE], ORIGINVERTEXTYPE_CENTERLEFT, m_aInitUiSize[UI_GAGE]);
	m_pImage[IMG_GAGE]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_GAGE));
	// �g
	m_pImage[IMG_FRAME] = CScene2D::Create(PRIORITY_CHARACTER, pos + m_aDiffpos[UI_GAGE], m_aInitUiCol[UI_FRAME], ORIGINVERTEXTYPE_CENTERLEFT, m_aInitUiSize[UI_FRAME]);
	m_pImage[IMG_FRAME]->BindTexture(CTexture::GetTextureInfo(CTexture::NAME_FRAME));
}

//-------------------------------------------------------------------------------------------------------------
// ��Ԃ̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CPlayer::SetState(CONST STATE state)
{
	if (m_state != state)
	{
		m_state = state;
		m_nCntState = ML_INT_UNSET;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �ʏ폈��
//-------------------------------------------------------------------------------------------------------------
void CPlayer::NormalProc(void)
{
	// �Q�[���p�b�h���쏈��
	if (GamepadOperationProc())
	{
	}
	// �L�[�{�[�h���쏈��
	else if (KeyboardOperationProc())
	{
	}
	else
	{
	}

	// �̂̍s������
	BodyAction();
}

//-------------------------------------------------------------------------------------------------------------
// �X�}�b�V������
//-------------------------------------------------------------------------------------------------------------
void CPlayer::SmashProc(void)
{
	D3DXVECTOR3* pPos = m_pImage[IMG_BODY]->GetPosition();

	CMylibrary::SlowingMove(&m_fSpeed, 0.2f);

	if (m_fSpeed < PLAYER_SPEEDLIMIT)
	{
		m_fSpeed = PLAYER_SPEEDLIMIT;
		SetState(STATE_RECOIL);
	}

	if (m_fSpeed != 0.0f)
	{
		float fRotation = m_pImage[IMG_BODY]->GetRotation();

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

//-------------------------------------------------------------------------------------------------------------
// ��������
//-------------------------------------------------------------------------------------------------------------
void CPlayer::RecoilProc(void)
{
	if (m_nCntState >= 0)
	{
		SetState(STATE_NORMAL);
	}
	else
	{
		++m_nCntState;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �Q�[���p�b�h���쏈��
//-------------------------------------------------------------------------------------------------------------
bool CPlayer::GamepadOperationProc(void)
{
	CXGamepad &Gamepad = CManager::GetXGamepad();
	VEC3 direNor;
	bool bOperat = false;
	
	direNor.z = 0.0f;
	Gamepad.GetStickLeft(&direNor.x, &direNor.y);

	if (direNor.LengthSq() != 0.0f)
	{
		bOperat = true;
	}

	if (Gamepad.GetPress(CXGamepad::JOYPADKEY_R2))
	{
		float fSizeX = m_pImage[IMG_GAGE]->GetSize()->x + PLAYER_CHARGE;
		if (fSizeX > PLAYER_GAGE_SIZE_X)
		{
			fSizeX = PLAYER_GAGE_SIZE_X;
		}
		m_pImage[IMG_GAGE]->SetSizeX(fSizeX);
		m_bGamePadcharge = true;
	}
	else if(m_bGamePadcharge == true)
	{
		m_fRotDest = m_pImage[IMG_BODY]->GetRotation();
		m_fSpeed = (m_pImage[IMG_GAGE]->GetSize()->x / PLAYER_GAGE_SIZE_X) * PLAYER_SMASHSPEED;
		m_pImage[IMG_GAGE]->SetSizeX(0.0f);
		SetState(STATE_SMASH);
		m_bGamePadcharge = false;
		return bOperat;
	}

	if (bOperat)
	{
		m_fRotDest = atan2f(direNor.x, direNor.y);
		if (!m_bGamePadcharge) {
			m_fSpeed += 0.05f;
		}
		else {
			CMylibrary::SlowingMove(&m_fSpeed, 0.05f);

		}
	}
	else
	{
		CMylibrary::SlowingMove(&m_fSpeed, 0.05f);
	}
	return bOperat;
}

//-------------------------------------------------------------------------------------------------------------
// �L�[�{�[�h���쏈��
//-------------------------------------------------------------------------------------------------------------
bool CPlayer::KeyboardOperationProc(void)
{
	// �}�l�[�W���[�̎擾
	Ckeyboard &Keyboard = CManager::GetKeyboard();
	VEC3 direNor = ML_VEC3_UNSET;
	bool bOperat = false;
	bool bCharge = false;
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

	if (Keyboard.GetPress(DIK_SPACE))
	{
		float fSizeX = m_pImage[IMG_GAGE]->GetSize()->x + PLAYER_CHARGE;
		if (fSizeX > PLAYER_GAGE_SIZE_X)
		{
			fSizeX = PLAYER_GAGE_SIZE_X;
		}
		m_pImage[IMG_GAGE]->SetSizeX(fSizeX);

		bCharge = true;
	}
	else if (Keyboard.GetRelease(DIK_SPACE))
	{
		m_fRotDest = m_pImage[IMG_BODY]->GetRotation();
		m_fSpeed =  (m_pImage[IMG_GAGE]->GetSize()->x / PLAYER_GAGE_SIZE_X) * PLAYER_SMASHSPEED;
		m_pImage[IMG_GAGE]->SetSizeX(0.0f);
		SetState(STATE_SMASH);
		return bOperat;
	}

	if (bOperat)
	{
		m_fRotDest = atan2f(direNor.x, direNor.y);
		if (!bCharge) {
			m_fSpeed += 0.05f;
		}
		else {
			CMylibrary::SlowingMove(&m_fSpeed, 0.05f);

		}
	}
	else
	{
		CMylibrary::SlowingMove(&m_fSpeed, 0.05f);
	}


	return bOperat;
}

//-------------------------------------------------------------------------------------------------------------
// �̂̍s������
//-------------------------------------------------------------------------------------------------------------
void CPlayer::BodyAction(void)
{
	// �ϐ��錾
	float fRotation = m_pImage[IMG_BODY]->GetRotation();
	float fDirecRot = m_fRotDest - fRotation;

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

	D3DXVECTOR3* pPos = m_pImage[IMG_BODY]->GetPosition();

	if (m_state != STATE_SMASH)
	{
		if (m_fSpeed > PLAYER_SPEEDLIMIT)
		{
			m_fSpeed = PLAYER_SPEEDLIMIT;
		}
		else if (m_fSpeed < ML_FLOAT_UNSET)
		{
			m_fSpeed = ML_FLOAT_UNSET;
		}
	}

	if (m_fSpeed != 0.0f)
	{
		m_move.x = sinf(fRotation);
		m_move.y = -cosf(fRotation);
		m_move.Norm();
		m_move *= m_fSpeed;

		*pPos += m_move;

		m_pImage[IMG_BODY]->SetPosflag();
		m_pImage[IMG_GAGE]->SetPosition(*pPos + m_aDiffpos[UI_GAGE]);
		m_pImage[IMG_FRAME]->SetPosition(*pPos + m_aDiffpos[UI_FRAME]);
	}
}
