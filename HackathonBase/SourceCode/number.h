//*************************************************************************************************************
//
// �����̏���[number.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CNumber
{
public:
	/* �񋓌^��` */
	enum
	{
		E_F_NONE = -1,
		E_F_POS,
		E_F_TEX,
		E_F_COL,
		E_F_MAX
	};

	enum
	{
		E_M_NONE = 0b00000000,
		E_M_POS  = 0b00000001,
		E_M_TEX  = 0b00000010,
		E_M_COL  = 0b00000100,
		E_M_MAX  = E_M_POS | E_M_TEX | E_M_COL,
	};

	/* �\���̒�` */
	typedef struct BITS
	{
		union {
			struct {
				UINT pos : 1;
				UINT tex : 1;
				UINT col : 1;
			};
			UINT mask;
		};

		inline bool IsitSet(void) { return mask != ML_INT_UNSET; }														// �ݒ肳��Ă��邩?
	}BITS;


	CNumber(int nNumOrder);																								// �R���X�g���N�^
	CNumber();																											// �R���X�g���N�^
	~CNumber();																											// �f�X�g���N�^
	static HRESULT      Load(void);																						// �ǂݍ���
	static void         Unload(void);																					// �J��
	static CNumber*     Create(float fSizeX, float fSizeY, D3DXVECTOR3 pos, float fRot, int nNumber, D3DXCOLOR col);	// ����
	HRESULT             Init(float fSizeX, float fSizeY, int nNumber, float fRot, D3DXVECTOR3 pos, D3DXCOLOR col);		// ������
	void                Unint(void);																					// �I��
	void                Update(void);																					// �X�V
	void                Draw(void);																						// �`��
	void                SetVaetex(int *nNumber = NULL, D3DXVECTOR3 *pos = NULL, D3DXCOLOR *col = NULL);					// ���_�̍X�V
	void                SetVeatexColor(void);																			// ���_�J���[�ݒ�

	inline void         SetPosition(D3DXVECTOR3 &pos) { if (m_pos != pos) { m_pos = pos; SetPosflag(); } }				// �ʒu�̐ݒ�
	inline void         SetNumber(int nNumber)     { if (m_nNumber != nNumber) { m_nNumber = nNumber; SetTexflag(); } }	// �����̐ݒ�
	inline void         SetSize(D3DXVECTOR2 &size) { if (m_size != size) { m_size = size; m_halfSize = size*ML_HALF_SIZE; SetPosflag(); } }				// �傫���̐ݒ�
	inline void         SetRotation(float fRot)    { if (m_fRot != fRot) { m_fRot = fRot; SetTexflag(); } }				// ��]�ʂ̐ݒ�
	inline void         SetColor(D3DXCOLOR &col)   { if (m_col != col) { m_col = col; SetColflag(); } }					// ��]�ʂ̐ݒ�
	inline void         BindTexture(LPDIRECT3DTEXTURE9 pTexture) { m_pTexture = pTexture; }								// �e�N�X�`�����̐ݒ�

	void                Setflagbits(UINT mask) { m_flagbits.mask = mask; }												// �t���O�r�b�g�̐ݒ�
	void                SetPosflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_POS); }										// �ʒu�t���O��ݒ�
	void                SetTexflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_TEX); }										// �e�N�X�`���t���O��ݒ�
	void                SetColflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_COL); }										// �J���[�t���O��ݒ�
	void                UnsetPosflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_POS); }									// �ʒu�t���O���O��
	void                UnsetTexflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_TEX); }									// �e�N�X�`���t���O���O��
	void                UnsetColflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_COL); }									// �J���[�t���O���O��

	inline D3DXCOLOR*   GetColorPtr(void) { return &m_col; }															// �J���[�|�C���^�̎擾
	inline D3DXVECTOR2* GetSizePtr(void) { return &m_size; }															// �傫���|�C���^�̎擾
	inline D3DXVECTOR3* GetPosPtr(void) { return &m_pos; }																// �ʒu�|�C���^�̎擾
	inline float*       GetRotPtr(void) { return &m_fRot; }																// �����|�C���^�̎擾
	inline int*         GetNumberPtr(void) { return &m_nNumber; }														// �����|�C���^�̎擾
	inline D3DXCOLOR&   GetColor(void) { return m_col; }																// �J���[�|�C���^�̎擾
	inline D3DXVECTOR2& GetSize(void) { return m_size; }																// �傫���|�C���^�̎擾
	inline D3DXVECTOR3& GetPos(void) { return m_pos; }																	// �ʒu�|�C���^�̎擾
	inline float&       GetRot(void) { return m_fRot; }																	// �����|�C���^�̎擾
	inline int&         GetNumber(void) { return m_nNumber; }															// �����|�C���^�̎擾

private:
	void                UpdateVertex(void);																				// ���_���̍X�V

	static const float		m_fHalfPi;												// �~�����̔����̒l
	static const float		m_fMinHalfPi;											// �~�����̔����̕��̒l
	LPDIRECT3DTEXTURE9      m_pTexture;		// �e�N�X�`�����̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		// �o�b�t�@���̃|�C���^
	int                     m_nNumber;		// ����
	float                   m_fRot;			// ��]��
	D3DXVECTOR2             m_size;			// �傫��
	D3DXVECTOR2             m_halfSize;		// �����̑傫��
	D3DXVECTOR3             m_pos;			// �ʒu
	D3DXCOLOR               m_col;			// �F
	BITS                    m_flagbits;		// �t���O�r�b�g
};

#endif
