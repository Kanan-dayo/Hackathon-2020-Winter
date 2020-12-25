//*************************************************************************************************************
//
// �V�[��2D����[scene2D.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CScene2D :public CScene
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
		E_M_POS = 0b00000001,
		E_M_TEX = 0b00000010,
		E_M_COL = 0b00000100,
		E_M_MAX = E_M_POS | E_M_TEX | E_M_COL,
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


	CScene2D(CScene::PRIORITY priority);											// �R���X�g���N�^
	~CScene2D();																	// �f�X�g���N�^

	static CScene2D *Create(														// ����
		CScene::PRIORITY priority,													// �`��D�揇��
		D3DXVECTOR3      pos,														// �ʒu
		D3DXCOLOR        col,														// �F
		ORIGINVERTEXTYPE OriginType,												// ���_�^�C�v
		D3DXVECTOR2      size,														// �傫��
		float            fRotation = 0.0f,											// ��]��
		TEXTUREUV        TexUVInfo = TEXTUREUV(0, 1, 0, 1));						// �e�N�X�`��UV�ݒ�

	void Init(void);																// ������
	void Uninit(void);																// �I��
	void Update(void);																// �X�V
	void Draw(void);																// �`��

	void UpdateVertex(void);														// ���_�̍X�V
	void UpdateVertex(																// ���_�̍X�V
		bool bUpdatePos,															// �ʒu�X�V�t���O
		bool bUpdateCol = false,													// �F�X�V�t���O
		bool bUpdateUVTex = false);													// UV���W�X�V�t���O

	void                    BindTexture(LPDIRECT3DTEXTURE9 pTexture);				// �e�N�X�`�����֘A�t����

	LPDIRECT3DVERTEXBUFFER9 GetpVtxBuff(void);										// ���_�o�b�t�@�̎擾

	void                    SetOriginType(ORIGINVERTEXTYPE OriginType);				// ���_�^�C�v�̐ݒ�
	ORIGINVERTEXTYPE*       GetOriginType(void);									// ���_�^�C�v�̎擾

	void                    SetPosition(D3DXVECTOR3& pos);							// �ʒu�̐ݒ�
	D3DXVECTOR3*            GetPosition(void);										// �ʒu�̎擾

	void                    SetColor(D3DXCOLOR& col);								// �F�̐ݒ�
	void                    SetColorR(const float fR);									// �ԐF�̐ݒ�
	void                    SetColorG(const float fG);									// �ΐF�̐ݒ�
	void                    SetColorB(const float fB);									// �F�̐ݒ�
	void                    SetColorA(const float fA);									// �A���t�@�l�̐ݒ�

	D3DXCOLOR*              GetColor(void);											// �F�̎擾

	void                    SetSize(D3DXVECTOR2& size);								// �傫���̐ݒ�
	void                    SetSizeX(const float fSize);							// �傫��X�̐ݒ�
	void                    SetSizeY(const float fSize);							// �傫��Y�̐ݒ�
	D3DXVECTOR2*            GetSize(void);											// �傫���̎擾

	void                    SetSetingUV(TEXTUREUV& SetingUV);						// �e�N�X�`����UV���W�̐ݒ�p�̏��̐ݒ�
	void                    SetSetingUVCountU(const int nCntU);						// �e�N�X�`����UV���W�̐ݒ�p�̏�񉡂̃J�E���g�̐ݒ�
	void                    SetSetingUVCountV(const int nCntV);						// �e�N�X�`����UV���W�̐ݒ�p�̏��c�̃J�E���g�̐ݒ�
	void                    SetSetingUVNumU(const int nNumU);						// �e�N�X�`����UV���W�̐ݒ�p�̏�񉡂̌��̐ݒ�
	void                    SetSetingUVNumV(const int nNumV);						// �e�N�X�`����UV���W�̐ݒ�p�̏��c�̌��̐ݒ�
	TEXTUREUV*              GetSetingUV(void);										// �e�N�X�`����UV���W�̐ݒ�p�̏��̎擾

	void                    SetAnimTexVariables(TEXTUREANIMEINFO& TextureAnimeInfo);// �e�N�X�`���A�j���[�V�������̐ݒ�
	void                    SetAnimTexVariFrame(const int nFrame);					// �e�N�X�`���A�j���[�V�����t���[�����̐ݒ�
	void                    SetAnimTexVariFrameMax(const int nFrameMax);			// �e�N�X�`���A�j���[�V�����̍ő�t���[�����̐ݒ�
	TEXTUREANIMEINFO*       GetAnimTexVariables(void);								// �e�N�X�`���A�j���[�V�������̎擾

	void                    SetRotation(const float fRotation);						// ��]�ʂ̐ݒ�
	float                   GetRotation(void);										// ��]�ʂ̎擾
	int                     CalculatVatexUVMap(bool bLoop, int nFrameMax, int &nFrame, int nNumWidht, int nNumHight, int &nCountWidht, int &nCountHight);
	void                    CalSelectionUVMap(bool bIteration, int nFrameMax, int &nFrame, int nNumWidht, int &nCountWidht, int nCountHight, int &m_nIteration);
	void                    CalTurnBackUVMap(int nFrameMax, int &nFrame, int nNumWidht, int &nCountWidht, int nCountHight, int &m_nIteration);
	inline void             SetDisp(const bool bDisp) { m_bDisp = bDisp; }
	inline bool &           GetDisp(void) { return m_bDisp; }

	inline void             Setflagbits(UINT mask) { m_flagbits.mask = mask; }												// �t���O�r�b�g�̐ݒ�
	inline void             SetPosflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_POS); }										// �ʒu�t���O��ݒ�
	inline void             SetTexflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_TEX); }										// �e�N�X�`���t���O��ݒ�
	inline void             SetColflag(void) { Mlf_bit_set(m_flagbits.mask, E_F_COL); }										// �J���[�t���O��ݒ�
	inline void             UnsetPosflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_POS); }									// �ʒu�t���O���O��
	inline void             UnsetTexflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_TEX); }									// �e�N�X�`���t���O���O��
	inline void             UnsetColflag(void) { Mlf_bit_clear(m_flagbits.mask, E_F_COL); }									// �J���[�t���O���O��

	// �X�N���[������
	void ScrollTex(float const & fTexY);
protected:
	/* -- �����o�֐� -- */
	HRESULT                 MakeVatex(LPDIRECT3DDEVICE9 pDevice);					// ���_�̍쐬
	void                    SetVatexPosition(CRenderer::VERTEX_2D *pVtx, ORIGINVERTEXTYPE *pType, D3DXVECTOR3 * pPos, D3DXVECTOR2 * pSize, float *pRotation);			// ���_�ʒu�̐ݒ�
	void                    SetVatexPosition(CRenderer::VERTEX_2D *pVtx);			// ���_�ʒu�̐ݒ�
	void                    SetVatexColor(CRenderer::VERTEX_2D *pVtx);				// ���_�J���[�̐ݒ�
	void                    SetVatexTexture(CRenderer::VERTEX_2D *pVtx);			// ���_�e�N�X�`���̐ݒ�
	/* -- �����o�ϐ� -- */
	static const float		m_fHalfPi;												// �~�����̔����̒l
	static const float		m_fMinHalfPi;											// �~�����̔����̕��̒l
	LPDIRECT3DTEXTURE9      m_pTexture;												// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;												// ���_�o�b�t�@�ւ̃|�C���^
	ORIGINVERTEXTYPE        m_OriginType;											// ���_�̎��
	D3DXVECTOR3             m_pos;													// �ʒu
	D3DXCOLOR               m_col;													// �F
	D3DXVECTOR2             m_size;													// �傫��
	TEXTUREUV               m_TexUVInfo;											// �e�N�X�`��UV���W�̏��
	float                   m_fRotation;											// ��]��
	bool                    m_bDisp;												// �`��t���O
	BITS                    m_flagbits;												// �t���O�r�b�g
	TEXTUREANIMEINFO        m_TexAnimInfo;											// �e�N�X�`���A�j���[�V�������
};

#endif