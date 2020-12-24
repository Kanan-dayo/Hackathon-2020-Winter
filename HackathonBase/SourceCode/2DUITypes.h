//*************************************************************************************************************
//
// 2DUI�̍\���̂Ȃǂ̏���[2DUITypes.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _2DUITYPES_H_
#define _2DUITYPES_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// �O���錾
//-------------------------------------------------------------------------------------------------------------
class CNumericString;
class CScene2D;
class C2DUi;

//-------------------------------------------------------------------------------------------------------------
// ���O��Ԓ�`
//-------------------------------------------------------------------------------------------------------------
namespace N2Duitypes
{
	//-------------------------------------------------------------------------------------------------------------
	// �񋓌^��`
	//-------------------------------------------------------------------------------------------------------------
	// �}�X�N
	enum MASK
	{
		E_M_NUMBER   = 0b00000001,	// ����
		E_M_FLASHING = 0b00000010,	// �_��
		E_M_FADE     = 0b00000100,	// �t�F�[�h
		E_M_MAX      = 0b00000111,	// �ő�
	};
	// �t���O
	enum FLAG
	{
		E_F_NONE = -1,				// ����
		E_F_NUMBER,					// ����
		E_F_FLASHING,				// �_��
		E_F_FADE,					// �t�F�[�h
		E_F_MAX						// �ő�
	};
	// �摜�^�C�v
	typedef enum IMGTYPE
	{
		E_I_NONE = -1,				// ����
		E_I_IMAGE,					// �摜
		E_I_NUMBER,					// ����
		E_I_MAX						// �ő�
	}IMGTYPE;
	// �\�̓^�C�v
	typedef enum ABILITYTYPE
	{
		E_A_NONE = -1,
		E_A_FLASHING,				// �_��
		E_A_FADE,					// �t�F�[�h
		E_A_MAX						// �ő�
	} ABILITYTYPE;

	//-------------------------------------------------------------------------------------------------------------
	// �\���̒�`
	//-------------------------------------------------------------------------------------------------------------
	// �r�b�g�t���O
	typedef struct BITS
	{
		BITS() : unMask(0) {}				// �R���X�g���N�^
		BITS(UINT unMask);					// �R���X�g���N�^

		union {
			struct {
				UINT bNumber : 1;			// ��
				UINT bFlashing : 1;			// �_��
				UINT bFade : 1;				// �t�F�[�h
			};
			UINT unMask;					// �}�X�N�l
		};
	}BITS;

	// �A�r���e�B�x�[�X
	typedef struct ABILITYBASE
	{
		ABILITYBASE() {}								// �R���X�g���N�^
		ABILITYBASE(ABILITYTYPE type) : type(type) {}	// �R���X�g���N�^

		virtual void Update(C2DUi *pThis) = 0;			// �X�V

		ABILITYTYPE type;								// �^�C�v
	}ABILITYBASE;

	// �_�Ŕ\��
	typedef struct FLASHING : public ABILITYBASE
	{
		FLASHING();																// �R���X�g���N�^
		FLASHING(bool bUpdate, UINT nTiming);									// �R���X�g���N�^

		inline bool IsTimeEqualTiming(void) { return (nTiming == nTime); }		// ���Ԃƃ^�C�~���O��������������
		void        Update(C2DUi *pThis);										// �X�V����

		bool bUpdate;															// �X�V�t���O
		UINT nTime;																// ����
		UINT nTiming;															// �_��
	} FLASHING;

	// �t�F�[�h�\��
	typedef struct FADE : public ABILITYBASE
	{
		FADE();																										// �R���X�g���N�^
		FADE(bool bUpdate, bool bLoop, int nTiming, int nAddSign);													// �R���X�g���N�^

		inline void   SetChangeValue(float fChangeValue) { this->fChangeValue = fChangeValue; }						// 1�t���̕ύX�l�̐ݒ�
		inline float& GetChangeValue(void) { return fChangeValue; }													// 1�t���̕ύX�l�̎擾
		inline void   SetAddSign(int nAddSign) { if (nAddSign != 0) { this->nAddSign = (nAddSign > 0) ? 1 : -1; } }	// ���Z���镄���̐ݒ�
		inline int&   GetAddSign(void) { return nAddSign; }															// ���Z���镄���̎擾

		void          Update(C2DUi *pThis);																			// �X�V
		void          UpdateImage(CScene2D *pImage);																// �摜�̍X�V
		void          UpdateImage(CNumericString *m_pNumber);														// �摜�̍X�V

		bool  bUpdate;																								// �X�V�t���O
		bool  bLoop;																								// ���[�v
		float fChangeValue;																							// �A���t�@�l��1�t���̕ύX�l
		int   nTiming;																								// �^�C�~���O
		int   nAddSign;																								// ���Z���镄��
	} FADE;

	// �ݒ�p
	typedef struct SETING
	{
		SETING() {}
		SETING(BITS mask, int  nTextureID, bool bDisp, D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue);

		BITS        mask;			// �}�X�N
		int         nTextureID;		// �e�N�X�`��ID
		bool        bDisp;			// �`��t���O
		D3DXVECTOR3 pos;			// �ʒu
		D3DXCOLOR   col;			// �F
		D3DXVECTOR2 size;			// �T�C�Y
		float       fRotation;		// ��]��
		int         nValue;			// �l
	} SETING;
}

//-------------------------------------------------------------------------------------------------------------
// �G�C���A�X�錾
//-------------------------------------------------------------------------------------------------------------
// �񋓌^
using N2Dui_flag     = N2Duitypes::FLAG;			// �t���O
using N2Dui_mask     = N2Duitypes::MASK;			// �}�X�N
using N2Dui_img      = N2Duitypes::IMGTYPE;			// �摜�^�C�v
using N2Dui_ability  = N2Duitypes::ABILITYTYPE;		// �\�̓^�C�v

// �\����
using N2Dui_abibase  = N2Duitypes::ABILITYBASE;		// �A�r���e�B�x�[�X
using N2Dui_fade     = N2Duitypes::FADE;			// �t�F�[�h�\��
using N2Dui_flashing = N2Duitypes::FLASHING;		// �_�Ŕ\��
using N2Dui_bits     = N2Duitypes::BITS;			// �r�b�g�t���O
using N2Dui_seting   = N2Duitypes::SETING;			// �ݒ�p

#endif