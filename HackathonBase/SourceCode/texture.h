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
		NAME_FRAME,						// �g
		NAME_GAGE,						// �Q�[�W
		NAME_NUMBER,					// ����
		NAME_PLAYER,					// �v���C���[
		NAME_PleasePressButton000,		// �{�^���������Ă�������
		NAME_PAIR,						// ���j�y�A��
		NAME_READY,						// ���f�B
		NAME_GO,						// �S�[
		NAME_FINISH,					// �t�B�j�b�V��

		NAME_RESULT,					// ���U���g
		NAME_RESULT_TEXT,				// ���U���g�̃e�L�X�g
		NAME_RESULT_NUMPAIR,			// ���U���g�̑g��

		NAME_RANKING,					// �����L���O
		NAME_RANKING_1st,				// 1��
		NAME_RANKING_2nd,				// 2��
		NAME_RANKING_3rd,				// 3��
		NAME_RANKING_4th,				// 4��
		NAME_RANKING_5th,				// 5��

		NAME_HEART,						// �n�[�g
		NAME_STUDENT_0,					// �w�� �j �E
		NAME_STUDENT_1,					// �w�� �� ��
		NAME_COUPLE_0,					// �v�w �j �E
		NAME_COUPLE_1,					// �v�w �� ��
		NAME_NEWLYWED_0,				// �V�� �� �E
		NAME_NEWLYWED_1,				// �V�� �j ��

		NAME_TITLE_BG,					// �^�C�g���w�i
		NAME_TITLE_NAME,				// �^�C�g����
		NAME_TITLE_PRESSSTART,			// �^�C�g���v���X�X�^�[�g

		NAME_STORY_SANTA,				// �X�g�[���[�T���^
		NAME_STORY_SANTAIKARI,			// �X�g�[���[�T���^�{��
		NAME_STORY_SLID1,				// �X�g�[���[�X���C�h�P
		NAME_STORY_SLID2,				// �X�g�[���[�X���C�h�Q
		NAME_STORY_TUTORIALSKIP,		// �X�g�[���[�`���[�g���A���X�L�b�v

		NAME_TUTORIAL_BG,				// �`���[�g���A���w�i
		NAME_TUTORIAL_TUTORIALSKIP,		// �`���[�g���A���X�L�b�v

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
