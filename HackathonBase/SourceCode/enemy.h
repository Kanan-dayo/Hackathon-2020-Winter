//*************************************************************************************************************
//
// �G�l�~�[����[enemy.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
class CEnemy : public CScene
{
public:

	typedef enum
	{
		STATE_NONE = -1,		// ����
		STATE_STANDBY,			// �X�^���o�C
		STATE_NORMAL,			// �ʏ�
		STATE_DIE,				// ��
		STATE_MAX				// �ő吔
	}STATE;

	typedef enum
	{
		TYPE_NONE = -1,		// ����
		TYPE_COUPLE,		// �v�w
		TYPE_NEWLYWED,		// �V��
		TYPE_STUDENT,		// �w��
		TYPE_MAX			// �ő吔
	}TYPE;

	// �摜
	enum
	{
		IMG_NONE = -1,		// ����
		IMG_HEART,			// �n�[�g
		IMG_CHAR1,			// �L����1
		IMG_CHAR2,			// �L����2
		IMG_MAX				// �ő�
	};

	// UI
	enum
	{
		UI_NONE = -1,		// ����
		UI_CHAR1,			// �L����1
		UI_CHAR2,			// �L����2
		UI_MAX				// �ő�
	};

	CEnemy() : CScene(PRIORITY_CHARACTER) {}									// �R���X�g���N�^
	~CEnemy() {}																// �f�X�g���N�^

	static CEnemy *      Create(D3DXVECTOR3 pos, TYPE type);					// ����
	void                 Init(void);											// ������
	void                 Uninit(void);											// �I��
	void                 Update(void);											// �X�V
	void                 Draw(void);											// �`��
	void                 InitImage(D3DXVECTOR3 &pos, TYPE& type);				// �摜�̏�����

	void                 SetState(CONST STATE state);							// ��Ԃ̐ݒ�

	inline CScene2D*     GetImage(UINT nIndex) { return m_pImage[nIndex]; }		// �摜�̎擾

protected:
	void NormalProc(void);														// �ʏ폈��
	void DieProc(void);															// ���ʏ���

	static CONST D3DXVECTOR3 m_aDiffpos[UI_MAX];								// �ʒu�̍�
	static CONST D3DXVECTOR2 m_aInitUiSize[UI_MAX];								// UI�̏������傫��
	TYPE                 m_type;												// ���
	MyVector<CScene2D*>  m_pImage;												// �摜�|�C���^
	VEC3                 m_move;												// �ړ���
	float                m_fSpeed;												// ���x
	STATE                m_state;												// ���
	UINT                 m_nCntState;											// ��ԃJ�E���g
};


#endif