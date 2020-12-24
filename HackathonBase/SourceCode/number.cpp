//*************************************************************************************************************
//
// �������� [number.cpp]
// Author:IKUTO SEKINE
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "number.h"
#include "manager.h"
#include "Scene2D.h"

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
const float CNumber::m_fHalfPi    = D3DX_PI * ML_HALF_SIZE;		// �~�����̔����̒l
const float CNumber::m_fMinHalfPi = D3DX_PI * -ML_HALF_SIZE;		// �~�����̔����̕��̒l

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CNumber::CNumber(int nNumOrder)
{

}

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CNumber::CNumber()
{

}

//-------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CNumber::~CNumber()
{

}

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
HRESULT CNumber::Load(void)
{
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void CNumber::Unload(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �쐬
//-------------------------------------------------------------------------------------------------------------
CNumber * CNumber::Create(float fSizeX, float fSizeY, D3DXVECTOR3 pos, float fRot, int nNumber, D3DXCOLOR col)
{
	// �i���o�[�̐���
	CNumber *pNumber = new CNumber;
	// ������
	pNumber->Init(fSizeX, fSizeY, nNumber, fRot, pos ,col);
	return pNumber;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
HRESULT CNumber::Init(float fSizeX, float fSizeY, int nNumber, float fRot, D3DXVECTOR3 pos, D3DXCOLOR col)
{
	m_flagbits.mask = E_M_NONE;
	m_nNumber	= nNumber;		// ����
	m_size.x	= fSizeX;		// �傫��X
	m_size.y	= fSizeY;		// �傫��Y
	m_halfSize = m_size * ML_HALF_SIZE;
	m_fRot		= fRot;			// ��]��
	m_pos		= pos;			// �ʒu 
	m_col		= col;			// �F
	// �ӑ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();	// �f�o�C�X�̃|�C���^

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(CRenderer::VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���ւ̃|�C���^
	CRenderer::VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�̐ݒ�
	pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
	pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
	pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
	pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
	pVtx[2].pos.x = m_pos.x + sinf(-m_fRot)*m_halfSize.y + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
	pVtx[2].pos.y = m_pos.y + cosf(-m_fRot)*m_halfSize.y + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
	pVtx[3].pos.x = m_pos.x + sinf(-m_fRot)*m_halfSize.y + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
	pVtx[3].pos.y = m_pos.y + cosf(-m_fRot)*m_halfSize.y + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
	pVtx[0].pos.z = ML_FLOAT_UNSET;
	pVtx[1].pos.z = ML_FLOAT_UNSET;
	pVtx[2].pos.z = ML_FLOAT_UNSET;
	pVtx[3].pos.z = ML_FLOAT_UNSET;
	// �������W�̐ݒ�
	pVtx[0].rhw = 1.0f;		//  1.0f�ŌŒ�
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CNumber::Unint(void)
{
	// ���_�o�b�t�@��NULL����Ȃ���
	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@�̊J��
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CNumber::Update(void)
{
	// �ݒ肳��Ă��鎞
	if (m_flagbits.IsitSet())
	{// ���_���̍X�V
		UpdateVertex();
		m_flagbits.mask = E_M_NONE;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CNumber::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer().GetDevice();

	// ���_�o�b�t�@���X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(CRenderer::VERTEX_2D));

	// ���_�t�H�[�}�b�g�ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �e�N�X�`���̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//-------------------------------------------------------------------------------------------------------------
// ���_�̍X�V
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetVaetex(int * nNumber, D3DXVECTOR3 * pos, D3DXCOLOR * col)
{
	// ���_���ւ̃|�C���^
	CRenderer::VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �����|�C���^��NULL����Ȃ���
	if (nNumber != NULL)
	{// �e�N�X�`���̐ݒ�
		m_nNumber = *nNumber;
		pVtx[0].tex = D3DXVECTOR2((*nNumber*0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (*nNumber*0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((*nNumber*0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (*nNumber*0.1f), 1.0f);
	}
	// �ʒu�|�C���^��NULL����Ȃ���
	if (pos != NULL)
	{// �ʒu�̐ݒ�
		m_pos = *pos;
		pVtx[0].pos.x = pos->x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[0].pos.y = pos->y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[1].pos.x = pos->x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[1].pos.y = pos->y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[2].pos.x = pos->x + sinf(-m_fRot)*m_halfSize.y          + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[2].pos.y = pos->y + cosf(-m_fRot)*m_halfSize.y          + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[3].pos.x = pos->x + sinf(-m_fRot)*m_halfSize.y          + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[3].pos.y = pos->y + cosf(-m_fRot)*m_halfSize.y          + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
	}
	// �J���[�|�C���^��NULL����Ȃ���
	if (col != NULL)
	{// �F�̍X�V
		m_col = *col;
		pVtx[0].col = *col;
		pVtx[1].col = *col;
		pVtx[2].col = *col;
		pVtx[3].col = *col;
	}
	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// ���_�J���[�ݒ�
//-------------------------------------------------------------------------------------------------------------
void CNumber::SetVeatexColor(void)
{
	// ���_���ւ̃|�C���^
	CRenderer::VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// ���_���̍X�V
//-------------------------------------------------------------------------------------------------------------
void CNumber::UpdateVertex(void)
{
	// ���_���ւ̃|�C���^
	CRenderer::VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �ʒu�t���O�������Ă��鎞
	if (Mlf_bit_comp(m_flagbits.mask, E_F_POS))
	{
		pVtx[0].pos.x = m_pos.x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[0].pos.y = m_pos.y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[1].pos.x = m_pos.x + sinf(D3DX_PI - m_fRot)*m_halfSize.y + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[1].pos.y = m_pos.y + cosf(D3DX_PI - m_fRot)*m_halfSize.y + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[2].pos.x = m_pos.x + sinf(-m_fRot)*m_halfSize.y          + sinf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[2].pos.y = m_pos.y + cosf(-m_fRot)*m_halfSize.y          + cosf(m_fMinHalfPi - m_fRot)*m_halfSize.x;
		pVtx[3].pos.x = m_pos.x + sinf(-m_fRot)*m_halfSize.y          + sinf(m_fHalfPi - m_fRot)*m_halfSize.x;
		pVtx[3].pos.y = m_pos.y + cosf(-m_fRot)*m_halfSize.y          + cosf(m_fHalfPi - m_fRot)*m_halfSize.x;
	}
	// �e�N�X�`���t���O�������Ă��鎞
	if (Mlf_bit_comp(m_flagbits.mask, E_F_TEX))
	{
		float fTex = m_nNumber*0.1f;
		pVtx[0].tex.x = fTex;
		pVtx[1].tex.x = 0.1f + fTex;
		pVtx[2].tex.x = pVtx[0].tex.x;
		pVtx[3].tex.x = pVtx[1].tex.x;

	}
	// �J���[�t���O�������Ă��鎞
	if (Mlf_bit_comp(m_flagbits.mask, E_F_COL))
	{
		pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =
			pVtx[3].col = m_col;
	}


	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}
