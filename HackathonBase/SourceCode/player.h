//*************************************************************************************************************
//
// �v���C���[����[player.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CScene2D;
class CPlayer : public CScene
{
public:

	typedef enum
	{
		STATE_NONE = -1,
		STATE_NORMAL,
		STATE_CHARGE,
		STATE_SMASH,
		STATE_MAX
	}STATE; 

	// �摜
	enum
	{
		IMG_NONE = -1,		// ����
		IMG_BODY,			// ��
		IMG_GAGE,			// �Q�[�W
		IMG_FRAME,			// �g
		IMG_MAX				// �ő�
	};

	// UI
	enum
	{
		UI_NONE = -1,		// ����
		UI_GAGE,			// �Q�[�W
		UI_FRAME,			// �g
		UI_MAX				// �ő�
	};

	CPlayer() : CScene(PRIORITY_CHARACTER) {}									// �R���X�g���N�^
	~CPlayer() {}																// �f�X�g���N�^

	static CPlayer *     Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);				// ����
	void                 Init(void);											// ������
	void                 Uninit(void);											// �I��
	void                 Update(void);											// �X�V
	void                 Draw(void);											// �`��
	void                 InitImage(D3DXVECTOR3 &pos, D3DXVECTOR2 &size);		// �摜�̏�����

	inline CScene2D*     GetImage(UINT nIndex) { return m_pImage[nIndex]; }		// �摜�̎擾

protected:
	void NormalProc(void);
	void SmashProc(void);
	bool GamepadOperationProc(void);											// �Q�[���p�b�h���쏈��
	bool KeyboardOperationProc(void);											// �L�[�{�[�h���쏈��
	void BodyAction(void);														// �̂̍s������
	static CONST D3DXVECTOR3 m_aDiffpos[UI_MAX];								// �ʒu�̍�
	static CONST D3DXCOLOR   m_aInitUiCol[UI_MAX];								// UI�̏������J���[
	static CONST D3DXVECTOR2 m_aInitUiSize[UI_MAX];								// UI�̏������傫��

	MyVector<CScene2D*>  m_pImage;												// �摜�|�C���^
	VEC3                 m_move;												// �ړ���
	float                m_fRotDest;											// ��]�̓��B�l
	float                m_fSpeed;												// ���x
	STATE                m_state;												// ���
};


#endif