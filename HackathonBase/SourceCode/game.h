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
class CEnemyMana;
class CGame : public CMode
{
public:

	typedef enum
	{
		MODE_NONE = -1,	// �����Ȃ�
		MODE_BEGIN,		// �J�n��
		MODE_READY,		// ���f�B
		MODE_GO,		// �S�[
		MODE_NORMAL,	// �ʏ�
		MODE_START,		//
		MODE_FINISH,	// �t�B�j�b�V��
		MODE_END,		// �Q�[���I��
	} MODE;

	typedef enum
	{	// �Q�[��UI�̎��
		GAMEUI_TIMER,	// �^�C�}�[UI
		GAMEUI_SCORE,	// �X�R�AUI
		GAMEUI_PAIR,	// �Z�y�AUI
		GAMEUI_READY,	// ���f�B
		GAMEUI_GO,		// �S�[
		GAMEUI_FINISH,	// �t�B�j�b�V��
		GAMEUI_MAX
	} GAMEUI;

	CGame() {};					// �R���X�g���N�^
	~CGame() {};					// �f�X�g���N�^

	static CGame *Create(void);	// ����
	static HRESULT LoadUIInfo(void);	// UI���̃��[�h
	static void AddNumKill(int nValue);	// ���j�������Z
	static void AddTime(int nValue);	// �^�C�������Z
	void Init(void);				// ������
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��
	MODE GetGameMode(void) { return m_mode; }
	C2DUi *GetGameUI(UINT nIndex) { return m_pGameUI[nIndex]; }
	CPlayer *GetPlayer(void) { return m_pPlayer; }
	CEnemyMana * GetEnemyMana(void) { return m_pEnemyMana; }
private:
	void CreateGameUI(void);		// �Q�[��UI�̐���
	void ModeTrans(void);			// ���[�h�J��
	void SetMode(MODE mode);		// ���[�h�̐ݒ�
	void UpdateTimer(void);			// �^�C�}�[�̍X�V
	void ModeBegin(void);			// �J�n��
	void ModeReady(void);			// ���f�B��
	void ModeGo(void);				// �S�[��
	void ModeNormal(void);			// �ʏ펞
	void ModeFinish(void);			// �t�B�j�b�V����

	static CONST D3DXVECTOR3 m_InitPosPlayer;
	static CONST D3DXVECTOR2 m_InitSizePlayer;
	static D3DXVECTOR3 m_posUI[GAMEUI_MAX];		// UI���W
	static D3DXVECTOR2 m_sizeUI[GAMEUI_MAX];	// UI�T�C�Y
	static int m_nNumKill;						// �|�����G��
	static int m_nCntTime;						// �c��^�C��
	static C2DUi* m_pGameUI[GAMEUI_MAX];		// �Q�[��UI�|�C���^
	CEnemyMana * m_pEnemyMana;					// �G�}�l�[�W���[�̃|�C���^
	CPlayer* m_pPlayer;							// �v���C���[�̃|�C���^[
	MODE     m_mode;							// ���[�h
	UINT     m_nCntMode;						// ���[�h�J�E���g
	int		 m_nCntFrame;						// �t���[���J�E���g
};

#endif