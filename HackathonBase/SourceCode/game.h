//*************************************************************************************************************
//
// �Q�[������ [game.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _GAME_H_
#define _GAME_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CPlayer;
class CGame : public CMode
{
public:

	typedef enum
	{
		MODE_NONE = -1,
		MODE_NORMAL,
		MODE_START,
		MODE_FINISH,
	} MODE;

	CGame() {};					// �R���X�g���N�^
	~CGame() {};					// �f�X�g���N�^

	static CGame *Create(void);	// ����
	void Init(void);				// ������
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��

	C2DUi *Get2dui(void) { return m_pC2dui; }
	CPlayer *GetPlayer(void) { return m_pPlayer; }

private:
	void ModeTrans(void);			// ���[�h�J��
	void SetMode(MODE mode);		// ���[�h�̐ݒ�

	static CONST D3DXVECTOR3 m_InitPosPlayer;
	static CONST D3DXVECTOR2 m_InitSizePlayer;
	C2DUi*   m_pC2dui;			// �QDUI�|�C���^
	CPlayer* m_pPlayer;			// �v���C���[�̃|�C���^[
	MODE     m_mode;			// ���[�h
	UINT     m_nCntMode;		// ���[�h�J�E���g
};

#endif