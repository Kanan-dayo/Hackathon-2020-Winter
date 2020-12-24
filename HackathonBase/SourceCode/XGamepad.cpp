// ===================================================================
//
// XInput�Q�[���p�b�h���� [ XGamepad.cpp ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#include "XGamepad.h"
#include "manager.h"
#include "debugProc.h"

// ===================================================================
// �}�N����`
// ===================================================================
#define STICK_MAX_RANGE (32768.0f)	// �X�e�B�b�N�̍ő�p�x

// ===================================================================
// �ÓI�����o�ϐ��̏�����
// ===================================================================
bool CXGamepad::m_bAllVib = true;

// ===================================================================
// �R���X�g���N�^
// ===================================================================
CXGamepad::CXGamepad()
{
	
}

// ===================================================================
// �f�X�g���N�^
// ===================================================================
CXGamepad::~CXGamepad()
{

}

// ===================================================================
// ������
// ===================================================================
HRESULT CXGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_bVibration = false;
	return S_OK;
}

// ===================================================================
// �I��
// ===================================================================
void CXGamepad::Uninit(void)
{
	// �g�p���Ȃ�
	// false�ɂ��邱�Ƃ� [�{�^�����������E�X�e�B�b�N�������E�g���K�[0�E�o�C�u���[�V������~] �𑗐M����
	XInputEnable(false);
}

// ===================================================================
// �X�V
// ===================================================================
void CXGamepad::Update(void)
{
	// �O��̏��̕ۑ�
	m_stateOld = m_state;

	// �ڑ��̊m�F
	UpdateControlState();

	// ���X�e�B�b�N�̒l���f�b�h�]�[��(���ʂȒl)��
	if ((m_state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		 m_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		 m_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		// 0�ɐ���
		m_state.Gamepad.sThumbLX = 0;
		m_state.Gamepad.sThumbLY = 0;
	}

	// �E�X�e�B�b�N�̒l���f�b�h�]�[��(���ʂȒl)��
	if ((m_state.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 m_state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(m_state.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 m_state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		// 0�ɐ���
		m_state.Gamepad.sThumbRX = 0;
		m_state.Gamepad.sThumbRY = 0;
	}

	// �o�C�u���[�V�����̍X�V
	UpdateVibration();
}

// ===================================================================
// �ڑ�����Ă��邩�m�F
// ===================================================================
HRESULT CXGamepad::UpdateControlState(void)
{
	// ������
	ZeroMemory(&m_state, sizeof(XINPUT_STATE));

	// �R���g���[���[�X�e�[�g�̎擾
	DWORD dwResult = XInputGetState(0, &m_state);

		// �擾�ł���
		if (dwResult == ERROR_SUCCESS)
			m_bConnect = true;
		// �ł��Ȃ�����
		else
			m_bConnect = false;

	return S_OK;
}

// ===================================================================
// �o�C�u���[�V�����̍X�V
// ===================================================================
void CXGamepad::UpdateVibration(void)
{
	// �U������ݒ�ɂ��Ă���
	if (m_bAllVib)
	{
		XINPUT_VIBRATION vibration; // �o�C�u���[�V�����̍\����

		// ������
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

		// ���ꂼ��̎��g��ݒ�
		if (m_bVibration)
			vibration.wLeftMotorSpeed = 65535;
		else
			vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;

		// �o�C�u���[�V���������s
		XInputSetState(0, &vibration);
	}
}

// ===================================================================
// �g���K�[����
// ===================================================================
bool CXGamepad::GetTrigger(JOYPADKEY button)
{
	// �{�^���̏���
	if (m_state.Gamepad.wButtons & button)
	{
		// ���݂̃L�[�ƑO��̃L�[���Ⴄ
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	// �g���K�[�L�[�̏���
	if (button == JOYPADKEY_L2 &&
		m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	// �g���K�[�L�[�̏���
	if (button == JOYPADKEY_R2 &&
		m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	return false;
}

// ===================================================================
// �v���X����
// ===================================================================
bool CXGamepad::GetPress(JOYPADKEY button)
{
	// �{�^���̏���
	if (m_state.Gamepad.wButtons & button)
		return true;

	// �g���K�[�L�[�̏���
	if (button == JOYPADKEY_L2 &&
		m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		// bLeftTrigger���}�N�����傫����Γ���
		return true;

	// �g���K�[�L�[�̏���
	if (button == JOYPADKEY_R2 &&
		m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		// bRightTrigger���}�N�����傫����Γ���
		return true;

	return false;
}

// ===================================================================
// ���X�e�B�b�N�̎擾
// ===================================================================
void CXGamepad::GetStickLeft(float * pValueH, float * pValueV)
{
	// �ő�p�x�Ŋ���A-1.0f �` 1.0f �ŏo��
	*pValueH = m_state.Gamepad.sThumbLX / STICK_MAX_RANGE;
	*pValueV = m_state.Gamepad.sThumbLY / STICK_MAX_RANGE;

}

// ===================================================================
// �E�X�e�B�b�N�̎擾
// ===================================================================
void CXGamepad::GetStickRight(float * pValueH, float * pValueV)
{
	// �ő�p�x�Ŋ���A-1.0f �` 1.0f �ŏo��
	*pValueH = m_state.Gamepad.sThumbRX / STICK_MAX_RANGE;
	*pValueV = m_state.Gamepad.sThumbRY / STICK_MAX_RANGE;
}
