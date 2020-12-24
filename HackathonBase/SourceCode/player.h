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
	static CONST D3DXVECTOR2 m_aDiffpos[UI_MAX];
	static CONST D3DXCOLOR   m_aInitUiCol[UI_MAX];
	static CONST D3DXVECTOR2 m_aInitUiSize[UI_MAX];

	MyVector<CScene2D*>  m_pImage;
	D3DXVECTOR3          m_move;
};


#endif