//*************************************************************************************************************
//
// 2DUI�̍\���̂Ȃǂ̏���[2DUITypes.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "2DUITypes.h"
#include "2DUI.h"
#include "Scene2D.h"
#include "NumericString.h"
#include "number.h"

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::BITS::BITS(UINT unMask)
{
	this->unMask = unMask;
}

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::FLASHING::FLASHING() : ABILITYBASE(E_A_FLASHING)
{
	this->bUpdate = true;
	this->nTime   = ML_INT_UNSET;
	this->nTiming = ML_INT_UNSET;
}

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
N2Duitypes::FLASHING::FLASHING(bool bUpdate, UINT nTiming) : ABILITYBASE(E_A_FLASHING)
{
	this->bUpdate = bUpdate;
	this->nTime   = ML_INT_UNSET;
	this->nTiming = nTiming;
}

//-------------------------------------------------------------------------------------------------------------
// �_�ł̍X�V
//-------------------------------------------------------------------------------------------------------------
void N2Duitypes::FLASHING::Update(C2DUi *pThis)
{
	// �X�V�t���O���I�t�̎�
	if (!this->bUpdate)
	{// �����𔲂���
		return;
	}

	// ���Ԃ��X�V
	this->nTime += 1;

	// �����l�̎�
	if (this->IsTimeEqualTiming())
	{// �`��t���O��؂�ւ���
		pThis->ChangeDispFlag();
		// ���Ԃ�����������
		this->nTime = ML_INT_UNSET;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
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
// �R���X�g���N�^
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
// �t�F�[�h�X�V
//-------------------------------------------------------------------------------------------------------------
void N2Duitypes::FADE::Update(C2DUi * pThis)
{
	// �X�V�t���O���I�t�̎�
	if (!this->bUpdate)
	{// �����𔲂���
		return;
	}

	// �摜�f�[�^�̎擾
	auto &itr = pThis->GetImage();

	// �����t���O�������Ă�����
	if (Mlf_bit_comp(pThis->GetMask(), N2Dui_flag::E_F_NUMBER))
	{// �摜�̍X�V
		this->UpdateImage(itr.pNumber);
	}
	// ����ȊO
	else
	{// �摜�̍X�V
		this->UpdateImage(itr.pImage);
	}

}

//-------------------------------------------------------------------------------------------------------------
// �t�F�[�h�摜�̍X�V
//-------------------------------------------------------------------------------------------------------------
void N2Duitypes::FADE::UpdateImage(CScene2D * pImage)
{
	// �F�̎擾
	D3DXCOLOR *pCol = pImage->GetColor();

	// �F�̕ω�
	pCol->a += this->fChangeValue * this->nAddSign;

	// �A���t�@�l��͈͓��ɐ�������
	if (CMylibrary::ClampCol(*pCol) == true)
	{// �����ꂽ�Ƃ�
		this->nAddSign *= -1;
	}

	// ���_�J���[���X�V����
	pImage->SetColflag();
}

//-------------------------------------------------------------------------------------------------------------
// �t�F�[�h��������̍X�V
//-------------------------------------------------------------------------------------------------------------
void N2Duitypes::FADE::UpdateImage(CNumericString * pNumber)
{
	// �ϐ��錾
	float fChgAlpha = fChangeValue * nAddSign;	// �A���t�@�l�ύX�l
	bool  bInver    = false;					// ���]�t���O

	// �͈̓��[�v
	for (auto &itr : pNumber->m_pNumber)
	{
		// �F�̎擾
		D3DXCOLOR *pCol = itr.GetColorPtr();

		// �F�̕ω�
		pCol->a += fChgAlpha;

		// �A���t�@�l��͈͓��ɐ�������
		if (CMylibrary::ClampCol(*pCol) && !bInver)
		{// �����ꂽ�Ƃ����]�t���O�I��
			bInver = true;
		}

		// �J���[�t���O��ݒ�
		itr.SetColflag();
	}

	// ���]�t���O�`�F�b�N
	if (bInver)
	{// �����𔽓]
		nAddSign *= -1;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
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