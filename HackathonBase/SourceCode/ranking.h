//*************************************************************************************************************
//
// �����L���O���� [ranking.h]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CRanking : public CMode
{
public:
	typedef enum
	{	// �����L���O��UI
		RANKING_RANKED,		// ����
		RANKING_NUM,		// �y�A��
		RANKING_NUMPAIR,	// �Z�y�A
		RANKING_MAX
	} RANKING_UI;

	typedef enum
	{	// �����L���O�̏���
		RANK_1ST,			// 1
		RANK_2ND,			// 2
		RANK_3RD,			// 3
		RANK_4TH,			// 4
		RANK_5TH,			// 5
		RANK_MAX
	} RANKING_RANK;

	CRanking() {};					// �R���X�g���N�^
	~CRanking() {};					// �f�X�g���N�^

	static CRanking *Create(void);		// ����
	static HRESULT LoadUIInfo(void);	// UI�̏��̃��[�h
	static HRESULT LoadRank(void);		// �����L���O�̃��[�h
	static HRESULT SaveRanking(void);		// �����L���O�̃Z�[�u

	static void SetPlayerScore(int nScore) { m_nPlayerScore = nScore; m_bSort = true; }	// �v���C���[�X�R�A�̐ݒ�

	void Init(void);				// ������
	void Uninit(void);				// �I��
	void Update(void);				// �X�V
	void Draw(void);				// �`��

private:
	void SortScore(void);
	static D3DXVECTOR3 m_posUI[RANK_MAX][RANKING_MAX];		// UI�̍��W
	static D3DXVECTOR2 m_sizeUI[RANK_MAX][RANKING_MAX];		// UI�̃T�C�Y
	static int		   m_nNumPair[RANK_MAX];				// �y�A��

	static int m_nPlayerScore;								// �v���C���[�̃X�R�A
	static int m_nPlayerRank;								// �v���C���[�̏���
	static bool m_bSort;									// �\�[�g���邩

	C2DUi *m_p2DUI[RANK_MAX][RANKING_MAX];					// �QDUI�|�C���^
	C2DUi *m_pUIRaking;										// �����L���OUI
	C2DUi *m_pUIBG;											// �w�iUI
};

#endif