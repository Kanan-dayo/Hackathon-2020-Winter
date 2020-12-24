//*************************************************************************************************************
//
// 2DUI����[2DUI.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _2DUI_H_
#define _2DUI_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "2DUITypes.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CNumericString;
class CScene2D;
class C2DUi : public CScene
{
public:


	typedef union
	{
		void*           pVoidptr;		// nullcheck
		N2Dui_abibase*  pBase;			// �x�[�X
		N2Dui_flashing* pFlashing;		// �_�ł̃|�C���^
		N2Dui_fade*     pFade;			// �t�F�[�h�̃|�C���^
	} ABILITY;	

	typedef union IMAGE
	{
		IMAGE() { pVoidptr = nullptr; }
		void*           pVoidptr;		// nullcheck
		CNumericString* pNumber;		// �����̃|�C���^
		CScene2D*       pImage;			// �摜�̃|�C���^
	}IMAGE;

	C2DUi() : CScene(PRIORITY_BUI) {}																							// �R���X�g���N�^
	~C2DUi() {}																													// �f�X�g���N�^

	static C2DUi * Create(N2Dui_seting &seting, PRIORITY pri);																					// ����
	void Init(N2Dui_seting &seting, PRIORITY& pri);																								// ������
	void Init(void);																											// ������
	void Uninit(void);																											// �I��
	void Update(void);																											// �X�V
	void Draw(void);																											// �`��

	void          ChangeDispFlag(void);																							// �`��t���O�̐؂�ւ�

	inline void   SetTextureID(int nTextureID) { m_nTextureID = nTextureID; }													// �e�N�X�`��ID�̐ݒ�
	inline int&   GetTextureID(void) { return m_nTextureID; }																	// �e�N�X�`��ID�̎擾
	inline void   SetDisp(bool bDisp) { m_bDisp = bDisp; ReflectDispFlagInImage(); }											// �`��t���O�̐ݒ�
	inline bool&  GetDisp(void) { return m_bDisp; }																				// �`��t���O�̎擾

	void SetFadeAbility(N2Dui_fade &fade);
	void SetFlashingAbility(N2Dui_flashing &flashing);

	ABILITY *GetAbilityDataTopPtr(void);																						// �擪�̃A�r���e�B�f�[�^�̃|�C���^�̎擾
	ABILITY &GetAbilityData(N2Dui_ability type);																				// �A�r���e�B�f�[�^�̎擾

	UINT &GetMask(void) { return m_mask.unMask; }																				// �}�X�N�̎擾
	IMAGE &GetImage(void) { return m_Image[m_ImageType]; }																		// �摜�̎擾(������)
	IMAGE *GetImagePtr(void) { return &m_Image[m_ImageType]; }																	// �摜�̃|�C���^�̎擾(������)
protected:
	void ReflectDispFlagInImage(void);																							// �`��t���O���摜�ɔ��f
	int                    m_nTextureID;														// �e�N�X�`��ID
	bool                   m_bDisp;																// �`��t���O
	MyVector<ABILITY>      m_AbilityData;														// �A�r���e�B�f�[�^
	MyVector<IMAGE>        m_Image;																// �摜�f�[�^
	N2Dui_bits             m_mask;																// �r�b�g�t���O
	N2Dui_img              m_ImageType;

};


#endif