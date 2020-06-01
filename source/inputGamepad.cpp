// ===================================================================
//
// XInputゲームパッド処理 [ inputGamepad.cpp ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#include "inputGamepad.h"
#include "manager.h"
#include "debugProc.h"

// ===================================================================
// 静的メンバ変数の初期化
// ===================================================================
bool CInputGamepad::m_bAllVib = false;	// 振動設定

// ===================================================================
// コンストラクタ
// ===================================================================
CInputGamepad::CInputGamepad()
{

}

// ===================================================================
// デストラクタ
// ===================================================================
CInputGamepad::~CInputGamepad()
{

}

// ===================================================================
// 初期化
// ===================================================================
HRESULT CInputGamepad::Init(HINSTANCE hInstance, HWND hWnd)
{
	m_bVibration = false;
	return S_OK;
}

// ===================================================================
// 終了
// ===================================================================
void CInputGamepad::Uninit(void)
{
	// 使用しない
	// falseにすることで [ボタン押下無し・スティック軸中央・トリガー0・バイブレーション停止] を送信する
	XInputEnable(false);
}

// ===================================================================
// 更新
// ===================================================================
void CInputGamepad::Update(void)
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
HRESULT CInputGamepad::UpdateControlState(void)
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
void CInputGamepad::UpdateVibration(void)
{
	// 振動する設定が切れている
	if (!m_bAllVib)
	{
		// 処理を終える
		return;
	}

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

// ===================================================================
// トリガー入力
// ===================================================================
bool CInputGamepad::GetTrigger(JOYPADKEY button)
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
bool CInputGamepad::GetPress(JOYPADKEY button)
{
	// ボタンの処理
	if (m_state.Gamepad.wButtons & button)
		return true;

	// トリガーキーの処理
	if (button == JOYPADKEY_L2 &&
		m_state.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;		// bLeftTriggerがマクロより大きければ入力

	// トリガーキーの処理
	if (button == JOYPADKEY_R2 &&
		m_state.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;		// bRightTriggerがマクロより大きければ入力

	return false;
}

// ===================================================================
// 左スティックの取得
// ===================================================================
void CInputGamepad::GetStickLeft(float * pValueH, float * pValueV)
{
	// スティックの有効範囲で割り、-1.0f ～ 1.0f で出力する
	*pValueH = m_state.Gamepad.sThumbLX / JOY_MAX_RANGE;
	*pValueV = m_state.Gamepad.sThumbLY / JOY_MAX_RANGE;

}

// ===================================================================
// 右スティックの取得
// ===================================================================
void CInputGamepad::GetStickRight(float * pValueH, float * pValueV)
{
	// スティックの有効範囲で割り、-1.0f ～ 1.0f で出力する
	*pValueH = m_state.Gamepad.sThumbRX / JOY_MAX_RANGE;
	*pValueV = m_state.Gamepad.sThumbRY / JOY_MAX_RANGE;
}
