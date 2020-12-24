//*************************************************************************************************************
//
// 2DUI����[2DUI.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "2DUI.h"
#include "Scene2D.h"
#include "number.h"
#include "NumericString.h"
#include "texture.h"
#include "DebugProc.h"

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
C2DUi * C2DUi::Create(N2Dui_seting &seting, PRIORITY pri)
{
	C2DUi * pC2dui = new C2DUi;
	pC2dui->Init(seting, pri);
	return pC2dui;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Init(N2Dui_seting & seting, PRIORITY& pri)
{
	// �}�X�N�̐ݒ�
	m_mask = seting.mask;
	// �e�N�X�`��ID�̏�����
	m_nTextureID = seting.nTextureID;
	// �`��t���O�̐ݒ�
	m_bDisp = seting.bDisp;

	// �T�C�Y�̕ύX
	m_Image.resize(N2Dui_img::E_I_MAX);

	// �A�r���e�B�̐���
	if (Mlf_bit_comp(m_mask.unMask, N2Dui_flag::E_F_NUMBER))
	{
		m_Image[N2Dui_img::E_I_NUMBER].pNumber = CNumericString::Create(seting.pos, seting.col, seting.size, seting.fRotation, seting.nValue);
		m_Image[N2Dui_img::E_I_NUMBER].pNumber->BindTexture(CTexture::GetTextureInfo(seting.nTextureID));
		m_ImageType = N2Dui_img::E_I_NUMBER;
	}
	else
	{
		m_Image[N2Dui_img::E_I_IMAGE].pImage = CScene2D::Create(pri, seting.pos, seting.col, ORIGINVERTEXTYPE_ROTCENTER, seting.size, seting.fRotation, TEXTUREUV(0, 1, 0, 1));
		m_Image[N2Dui_img::E_I_IMAGE].pImage->BindTexture(CTexture::GetTextureInfo(seting.nTextureID));
		m_ImageType = N2Dui_img::E_I_IMAGE;
	}
	if (Mlf_bit_comp(m_mask.unMask, N2Dui_flag::E_F_FLASHING))
	{
		m_AbilityData.push_back();
		m_AbilityData.back().pFlashing = new N2Dui_flashing;
	}
	if (Mlf_bit_comp(m_mask.unMask, N2Dui_flag::E_F_FADE))
	{
		m_AbilityData.push_back();
		m_AbilityData.back().pFade = new N2Dui_fade;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Init(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Uninit(void)
{
	if (m_ImageType == N2Dui_img::E_I_NUMBER)
	{
		if (m_Image[N2Dui_img::E_I_NUMBER].pVoidptr)
		{
			delete m_Image[N2Dui_img::E_I_NUMBER].pNumber;
			m_Image[N2Dui_img::E_I_NUMBER].pNumber = nullptr;
		}
	}
	m_Image.clear();

	for (int nCntAbility = 0; nCntAbility < m_AbilityData.size(); nCntAbility++)
	{
		delete m_AbilityData[nCntAbility].pVoidptr;
		m_AbilityData[nCntAbility].pVoidptr = nullptr;
	}
	m_AbilityData.clear();
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Update(void)
{
	// �A�r���e�B�̍X�V
	for (auto &itr : m_AbilityData)
	{
		itr.pBase->Update(this);
	}
	// �����̎��X�V
	if (m_ImageType == N2Dui_img::E_I_NUMBER)
	{
		m_Image[m_ImageType].pNumber->Update();
	}
	else if (m_ImageType == N2Dui_img::E_I_IMAGE)
	{
		m_Image[m_ImageType].pImage->UpdateVertex();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void C2DUi::Draw(void)
{
	if (m_bDisp && m_ImageType == N2Dui_img::E_I_NUMBER)
	{
		m_Image[m_ImageType].pNumber->Draw();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��t���O�̐؂�ւ�
//-------------------------------------------------------------------------------------------------------------
void C2DUi::ChangeDispFlag(void)
{
	m_bDisp ^= 1;
	ReflectDispFlagInImage();
}

//-------------------------------------------------------------------------------------------------------------
// �t�F�[�h�\�͂̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C2DUi::SetFadeAbility(N2Dui_fade & fade)
{
	auto *pItr = m_AbilityData.LambdaSearch([](ABILITY &lhs) {return  lhs.pBase->type == N2Dui_ability::E_A_FADE; });
	if (pItr)
	{
		*pItr->pFade = fade;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �_�ł̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C2DUi::SetFlashingAbility(N2Dui_flashing & flashing)
{
	auto *pItr = m_AbilityData.LambdaSearch([](ABILITY &lhs) {return  lhs.pBase->type == N2Dui_ability::E_A_FLASHING; });
	if (pItr)
	{
		*pItr->pFlashing = flashing;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �擪�̃A�r���e�B�f�[�^�̃|�C���^�̎擾
//-------------------------------------------------------------------------------------------------------------
C2DUi::ABILITY* C2DUi::GetAbilityDataTopPtr(void)
{
	return &m_AbilityData[0];
}

//-------------------------------------------------------------------------------------------------------------
// �A�r���e�B�f�[�^�̎擾
//-------------------------------------------------------------------------------------------------------------
C2DUi::ABILITY & C2DUi::GetAbilityData(N2Dui_ability type)
{
	if (Mlf_InRange(type, N2Dui_ability::E_A_MAX, N2Dui_ability::E_A_NONE))
	{
		return m_AbilityData[type];
	}
	return m_AbilityData[0];
}

//-------------------------------------------------------------------------------------------------------------
// �`��t���O���摜�ɔ��f
//-------------------------------------------------------------------------------------------------------------
void C2DUi::ReflectDispFlagInImage(void)
{
	if (m_ImageType == N2Dui_img::E_I_IMAGE)
	{
		m_Image[m_ImageType].pImage->SetDisp(m_bDisp);
	}
}


