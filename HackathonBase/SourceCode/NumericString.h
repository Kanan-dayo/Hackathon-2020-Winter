//*************************************************************************************************************
//
// ���������񏈗�[NumericString.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _NUMERICSTRING_H_
#define _NUMERICSTRING_H_

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
class CNumber;
class CNumericString
{
public:
	CNumericString();				// �R���X�g���N�^
	~CNumericString();				// �f�X�g���N�^

	static CNumericString *Create(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR2 size, float fRotation, int nValue, int nNumNumber = -1);

	void Init(D3DXVECTOR3 &pos, D3DXCOLOR &col, D3DXVECTOR2 &size, float &fRotation, int &nValue, int &nNumNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddValue(void);

	// �X�V
	void UpdateNumber(void);
	void UpdatePos(void);
	void UpdateSize(void);
	void UpdateRotation(void);
	void UpdateCol(void);

	// �ݒ�
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);									// �e�N�X�`�����֘A�t����
	inline void SetPos(D3DXVECTOR3 &pos) { m_pos = pos; }
	inline void SetCol(D3DXCOLOR &col) { m_col = col; }
	inline void SetSize(D3DXVECTOR2 &size) { m_size = size; }
	inline void SetRotation(float fRotation) { m_fRotation = fRotation; }
	inline void SetValue(int nValue) { m_nValue = nValue; }

	// �擾
	inline int &GetValue(void) { return m_nValue; }				// �l�̎擾
	inline int &GetDigits(void) { return m_nDigits; }			// ����(�`�搔)�̎擾
	inline int &GetNumNumber(void) { return m_nNumNumber; }		// �g�����̎擾
	inline int &GetAddSign(void) { return m_nAddSign; }			// ���Z���镄���̎擾
	inline D3DXCOLOR & GetCol(void) { return m_col; }			// �F�̎擾
	inline D3DXVECTOR3 & GetPos(void) { return m_pos; }			// �ʒu�̎擾
	inline D3DXVECTOR2 GetSize(void) { return m_size; }			// �傫���̎擾
	inline float &GetRotation(void) { return m_fRotation; }		// ��]�ʂ̎擾

	MyVector<CNumber>			m_pNumber;			// �������
private:
	int							m_nValue;			// �l
	int							m_nDigits;			// ����(�`�搔)
	int							m_nNumNumber;		// �g����
	int							m_nAddSign;			// ���Z���镄��
	D3DXCOLOR					m_col;				// �F
	D3DXVECTOR3					m_pos;				// �ʒu
	D3DXVECTOR2					m_size;				// �傫��
	float						m_fRotation;		// ��]��
};

#endif