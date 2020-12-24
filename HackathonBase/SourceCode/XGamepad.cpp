// ===================================================================
//
// XInputゲームパッド処理 [ XGamepad.cpp ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#include "XGamepad.h"
#include "manager.h"
#include "debugProc.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define STICK_MAX_RANGE (32768.0f)	// スティックの最大角度

// ===================================================================
// 静的メンバ変数の初期化
// ===================================================================
bool CXGamepad::m_bAllVib = true;

// ===================================================================
// コンストラクタ
// ===================================================================
CXGamepad::CXGamepad()
{
	
}

// ===================================================================
// デストラクタ
// ===================================================================
CXGamepad::~CXGamepad()
{

}

// ===================================================================
// 初期化
// ===================================================================
HRESULT CXGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_bVibration = false;
	return S_OK;
}

// ===================================================================
// 終了
// ===================================================================
void CXGamepad::Uninit(void)
{
	// 使用しない
	// falseにすることで [ボタン押下無し・スティック軸中央・トリガー0・バイブレーション停止] を送信する
	XInputEnable(false);
}

// ===================================================================
// 更新
// ===================================================================
void CXGamepad::Update(void)
{
	// 前回の情報の保存
	m_stateOld = m_state;

	// 接続の確認
	UpdateControlState();

	// 左スティックの値がデッドゾーン(微量な値)内
	if ((m_state.Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		 m_state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(m_state.Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		 m_state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		// 0に正す
		m_state.Gamepad.sThumbLX = 0;
		m_state.Gamepad.sThumbLY = 0;
	}

	// 右スティックの値がデッドゾーン(微量な値)内
	if ((m_state.Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 m_state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(m_state.Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		 m_state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		// 0に正す
		m_state.Gamepad.sThumbRX = 0;
		m_state.Gamepad.sThumbRY = 0;
	}

	// バイブレーションの更新
	UpdateVibration();
}

// ===================================================================
// 接続されているか確認
// ===================================================================
HRESULT CXGamepad::UpdateControlState(void)
{
	// 初期化
	ZeroMemory(&m_state, sizeof(XINPUT_STATE));

	// コントローラーステートの取得
	DWORD dwResult = XInputGetState(0, &m_state);

		// 取得できた
		if (dwResult == ERROR_SUCCESS)
			m_bConnect = true;
		// できなかった
		else
			m_bConnect = false;

	return S_OK;
}

// ===================================================================
// バイブレーションの更新
// ===================================================================
void CXGamepad::UpdateVibration(void)
{
	// 振動する設定にしている
	if (m_bAllVib)
	{
		XINPUT_VIBRATION vibration; // バイブレーションの構造体

		// 初期化
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

		// それぞれの周波を設定
		if (m_bVibration)
			vibration.wLeftMotorSpeed = 65535;
		else
			vibration.wLeftMotorSpeed = 0;
		vibration.wRightMotorSpeed = 0;

		// バイブレーションを実行
		XInputSetState(0, &vibration);
	}
}

// ===================================================================
// トリガー入力
// ===================================================================
bool CXGamepad::GetTrigger(JOYPADKEY button)
{
	// ボタンの処理
	if (m_state.Gamepad.wButtons & button)
	{
		// 現在のキーと前回のキーが違う
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	// トリガーキーの処理
	if (button == JOYPADKEY_L2 &&
		m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	// トリガーキーの処理
	if (button == JOYPADKEY_R2 &&
		m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		if (m_state.Gamepad.wButtons & button && !(m_stateOld.Gamepad.wButtons & button))
			return true;
	}

	return false;
}

// ===================================================================
// プレス入力
// ===================================================================
bool CXGamepad::GetPress(JOYPADKEY button)
{
	// ボタンの処理
	if (m_state.Gamepad.wButtons & button)
		return true;

	// トリガーキーの処理
	if (button == JOYPADKEY_L2 &&
		m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		// bLeftTriggerがマクロより大きければ入力
		return true;

	// トリガーキーの処理
	if (button == JOYPADKEY_R2 &&
		m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		// bRightTriggerがマクロより大きければ入力
		return true;

	return false;
}

// ===================================================================
// 左スティックの取得
// ===================================================================
void CXGamepad::GetStickLeft(float * pValueH, float * pValueV)
{
	// 最大角度で割り、-1.0f ～ 1.0f で出す
	*pValueH = m_state.Gamepad.sThumbLX / STICK_MAX_RANGE;
	*pValueV = m_state.Gamepad.sThumbLY / STICK_MAX_RANGE;

}

// ===================================================================
// 右スティックの取得
// ===================================================================
void CXGamepad::GetStickRight(float * pValueH, float * pValueV)
{
	// 最大角度で割り、-1.0f ～ 1.0f で出す
	*pValueH = m_state.Gamepad.sThumbRX / STICK_MAX_RANGE;
	*pValueV = m_state.Gamepad.sThumbRY / STICK_MAX_RANGE;
}
