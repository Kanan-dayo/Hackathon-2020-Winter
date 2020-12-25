//*************************************************************************************************************
//
// �X�g�[���[���� [story.h]
// Author:NISHIYAMA KOKI
//
//*************************************************************************************************************
#ifndef _STORY_H_
#define _STORY_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "mode.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class C2DUi;
class CStory : public CMode
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_STORYBG = 0,
		TYPE_STORY_1,
		TYPE_STORY_2,
		TYPE_STORY_SANTA,
		TYPE_MAX
	} TYPE;



	/* �X�g�[���[�ƃ`���[�g���A���ǂ����ł��g���邩�� */
	// �g�嗦���
	typedef struct SCALING
	{
		SCALING()
		{
			OriginSize = D3DXVECTOR2(0.0f, 0.0f);
			fScal = 1.0f;
			fScalChange = 0.0f;
			fScalDiff = 0.0f;
			nCntTimeChange = 0;
		}
		// �ݒ�
		void Set(
			D3DXVECTOR2 const & SouceSize,
			float const & fSouceScalChange
		);
		D3DXVECTOR2 OriginSize;			// ���̃T�C�Y
		int nCntTimeChange;				// �؂�ւ�鎞�ԃJ�E���g
		float fScal;					// ���݂̊g�嗦
		float fScalChange;				// �ڕW�g�嗦
		float fScalDiff;				// �g�嗦�㏸��
	} SCALING, *P_SCALING;

	typedef struct PERFORM2DUI
	{
		SCALING * pScal;
		C2DUi * pC2dui;
		bool	bMove;
	} PERFORM2DUI;
	// �T�C�Y�ύX�̍X�V����
	static void Update_SizeChange(PERFORM2DUI * pPerfomUi);
	// ���oUI�̏���������
	static void Init_PerfomUi(int const & nMaxUi, PERFORM2DUI * pPerfomUi);






	/* �֐� */
	CStory() {};						// �R���X�g���N�^
	~CStory() {};						// �f�X�g���N�^

	static CStory *Create(void);		// ����
	void Init(void);					// ������
	void Uninit(void);					// �I��
	void Update(void);					// �X�V
	void Draw(void);					// �`��

private:
	// ���ŋ��ړ�����
	void Update_UiMove(int const & nUi);

	/* �ϐ� */
	PERFORM2DUI m_apPerfomUi[TYPE::TYPE_MAX];	// �QDUI�|�C���^
	int			m_nPushButton;					// �������{�^����
	int			m_nCntFram;						// �t���[���J�E���g
};

#endif