//*************************************************************************************************************
//
// �e�N�X�`������[texture.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "manager.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CTexture
{
public:
	enum
	{
		NAME_NONE = -1,					// ����
		NAME_FRAME,
		NAME_NUMBER,
		NAME_PleasePressButton000,
		NAME_MAX						// �ő吔
	};
	enum
	{
		TEXTURE_TOPID_NONE     = -1,						// ����
		TEXTURE_TOPID_MAX      = NAME_MAX,					// �ő吔
	};
	CTexture();																												// �R���X�g���N�^
	~CTexture();																											// �f�X�g���N�^
	static void               Load(void);																					// �ǂݍ���
	static void               Unload(void);																					// �J��
	static void               CreateTexture(void);																			// �e�N�X�`���̐���
	static LPDIRECT3DTEXTURE9 GetTextureInfo(int nIndex);																	// �e�N�X�`���̎擾
	static int                GetNumTextureMax(void);																		// �e�N�X�`�����̍ő�
	static void               GetFileName(const int nIndex, STRING output);													// �t�@�C�����̎擾
	static CONST_STRING       GetFileName(const int nIndex);																// �t�@�C�����̎擾
private:
	/* �����o�֐� */
	static void               GetResource(void);																			// �����̒��B
	static void               ReadFromLine(CONST_STRING cnpLine, CONST_STRING cnpEntryType, CONST_STRING cnpEntryData);		// 1�s�������ǂݎ��
	static void               NumLoad(CONST_STRING cnpLine);																// ���̓ǂݍ���
	static void               FileNameLoad(CONST_STRING cnpLine);															// �t�@�C�����̓ǂݍ���
	/* �����o�ϐ� */
	static MyVector<LPDIRECT3DTEXTURE9> m_pTexture;																			// �e�N�X�`�����̃|�C���^
	static MyVector<CString>            m_pFileName;																		// �t�@�C�����̃|�C���^
	static int                          m_nNumTextureMax;																	// �ő�e�N�X�`����
};
#endif
