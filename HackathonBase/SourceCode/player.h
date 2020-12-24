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
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CScene2D;
class CPlayer : public CScene
{
public:

	typedef enum
	{
		IMG_NONE = -1,
		IMG_BODY,
		IMG_GAGE,
		IMG_FRAME,
		IMG_MAX
	}IMG;

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
	MyVector<CScene2D*>  m_pImage;
	D3DXVECTOR3          m_move;
};


#endif