//*************************************************************************************************************
//
// ���U���g���� [result.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CResult : public CMode
{
public:
	typedef enum
	{
		RESULT_TITLE = 0,	// ���U���g
		RESULT_TEXT,		// �j�󂵂��y�A��
		RESULT_NUM,			// �y�A��
		RESULT_NUMPAIR,		// �Z�y�A
		RESULT_MAX
	} RESULT_UI;

	CResult() {};					// �R���X�g���N�^
	~CResult() {};					// �f�X�g���N�^

	static CResult *Create(void);		// ����
	static HRESULT LoadUIInfo(void);	// UI�̏��̃��[�h

	static void SetNumPair(int nPair) { m_nNumPair = nPair; }	// �y�A���̐ݒ�

	void Init(void);					// ������
	void Uninit(void);					// �I��
	void Update(void);					// �X�V
	void Draw(void);					// �`��

private:
	static D3DXVECTOR3 m_posUI[RESULT_MAX];		// UI�̍��W
	static D3DXVECTOR2 m_sizeUI[RESULT_MAX];	// UI�̃T�C�Y
	static int m_nNumPair;						// �y�A��

	C2DUi *m_p2DUI[RESULT_MAX];					// �QDUI�|�C���^
};

#endif