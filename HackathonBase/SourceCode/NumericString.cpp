//*************************************************************************************************************
//
// ���������񏈗�[NumericString.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "NumericString.h"
#include "number.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CNumericString::CNumericString()
{
}

//-------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CNumericString::~CNumericString()
{
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CNumericString * CNumericString::Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue, int nNumNumber)
{
	CNumericString *pNumericString = new CNumericString;
	pNumericString->Init(pos, col, size, fRotation, nValue, nNumNumber);
	return pNumericString;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Init(D3DXVECTOR3 & pos, D3DXCOLOR & col, D3DXVECTOR2 & size, float & fRotation, int & nValue, int& nNumNumber)
{
	m_col = col;				// �F
	m_pos = pos;				// �ʒu
	m_size = size;				// �傫��
	m_nValue = nValue;			// ���Ԃ̒l
	m_nAddSign = -1;			// ���Z���镄��
	m_fRotation = fRotation;	// ��]��

	// ��������g�������Z�o
	m_nDigits = 0;
	CMylibrary::GetDigits(m_nDigits, m_nValue);
	// �g������-1�̎�
	if (nNumNumber == -1)
	{
		m_nNumNumber = m_nDigits;
	}

	// ���������m��
	m_pNumber.resize(m_nNumNumber);

	// �g�����������[�v
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; nCntNumber++)
	{// ���̐������Z�o
		int nDigits =
			m_nValue %
			(int)powf(10.0f, (float)m_nNumNumber - nCntNumber) /
			(int)powf(10.0f, (float)m_nNumNumber - 1.0f - nCntNumber);
		// �ʒu�̐ݒ�
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.z = 0.0f;
		// ������
		m_pNumber[nCntNumber].Init(m_size.x, m_size.y, nDigits, m_fRotation, pos, m_col);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Uninit(void)
{
	for (auto itr : m_pNumber)
	{
		itr.Unint();
	}
	m_pNumber.clear();
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Update(void)
{
	for (auto itr : m_pNumber)
	{
		itr.Update();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CNumericString::Draw(void)
{
	for (auto itr : m_pNumber)
	{
		itr.Draw();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �l�̉��Z
//-------------------------------------------------------------------------------------------------------------
void CNumericString::AddValue(void)
{
	// �J�n�l�ɕ��������Z
	m_nValue += m_nAddSign;
}

//-------------------------------------------------------------------------------------------------------------
// �����̍X�V
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdateNumber(void)
{
	// �g�����������[�v
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; ++nCntNumber)
	{// ���̐������Z�o
		int nDigits = m_nValue %
			(int)powf(10.0f, (float)m_nNumNumber - nCntNumber) /
			(int)powf(10.0f, (float)m_nNumNumber - 1.0f - nCntNumber);
		// ������
		m_pNumber[nCntNumber].SetNumber(nDigits);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �ʒu�̍X�V
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdatePos(void)
{
	// �g�����������[�v
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; ++nCntNumber)
	{		// �ʒu�̐ݒ�
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.z = 0.0f;
		m_pNumber[nCntNumber].SetPosition(pos);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �傫���̍X�V
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdateSize(void)
{
	// �g�����������[�v
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; ++nCntNumber)
	{		// �ʒu�̐ݒ�
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x * 0.75f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x * 0.75f));
		pos.z = 0.0f;
		m_pNumber[nCntNumber].SetSize(m_size);
		m_pNumber[nCntNumber].SetPosition(pos);
	}
}

//-------------------------------------------------------------------------------------------------------------
// ��]�̍X�V
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdateRotation(void)
{
	// �g�����������[�v
	for (int nCntNumber = 0; nCntNumber < m_nNumNumber; ++nCntNumber)
	{		// �ʒu�̐ݒ�
		D3DXVECTOR3 pos;
		pos.x = m_pos.x + (sinf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.y = m_pos.y + (cosf(m_fRotation + D3DX_PI*0.5f)*nCntNumber*(m_size.x* 0.75f));
		pos.z = 0.0f;
		m_pNumber[nCntNumber].SetRotation(m_fRotation);
		m_pNumber[nCntNumber].SetPosition(pos);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �F�̍X�V
//-------------------------------------------------------------------------------------------------------------
void CNumericString::UpdateCol(void)
{
	for (auto &itr : m_pNumber)
	{
		itr.SetColor(m_col);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �e�N�X�`���̘A��
//-------------------------------------------------------------------------------------------------------------
void CNumericString::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	for (auto &itr : m_pNumber)
	{
		itr.BindTexture(pTexture);
	}
}
