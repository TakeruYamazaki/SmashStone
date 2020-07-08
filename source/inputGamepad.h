// ===================================================================
//
// XInputゲームパッド処理 [XGamepad.h]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _XGAMEPAD_H_
#define _XGAMEPAD_H_

#define _CRT_SECURE_NO_WARNINGS

// ===================================================================
//インクルード
// ===================================================================
#include "main.h"
#include "input.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define JOY_MAX_RANGE (32768.0f)		// スティック有効範囲

// ===================================================================
// クラス定義
// ===================================================================
class CInputGamepad : public CInput
{
public:
	typedef enum
	{
		JOYPADKEY_X		= XINPUT_GAMEPAD_X,					// X
		JOYPADKEY_Y		= XINPUT_GAMEPAD_Y,					// Y
		JOYPADKEY_A		= XINPUT_GAMEPAD_A,					// A
		JOYPADKEY_B		= XINPUT_GAMEPAD_B,					// B
		JOYPADKEY_L1	= XINPUT_GAMEPAD_LEFT_THUMB,		// L1
		JOYPADKEY_R1	= XINPUT_GAMEPAD_RIGHT_THUMB,		// R1
		JOYPADKEY_L2	= XINPUT_GAMEPAD_LEFT_SHOULDER,		// L2
		JOYPADKEY_R2	= XINPUT_GAMEPAD_RIGHT_SHOULDER,	// R2
		JOYPADKEY_BACK	= XINPUT_GAMEPAD_BACK,				// BACK
		JOYPADKEY_START = XINPUT_GAMEPAD_START,				// START
		JOYPADKEY_LEFT	= XINPUT_GAMEPAD_DPAD_LEFT,			// 方向キー[左]
		JOYPADKEY_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,		// 方向キー[右]
		JOYPADKEY_UP	= XINPUT_GAMEPAD_DPAD_UP,			// 方向キー[上]
		JOYPADKEY_DOWN	= XINPUT_GAMEPAD_DPAD_DOWN,			// 方向キー[下]
		JOYPADKEY_MAX
	} JOYPADKEY;

	CInputGamepad();										// コンストラクタ
	~CInputGamepad();										// デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, int nIndex); // 初期化
	void Uninit(void);										// 終了
	void Update(void);										// 更新

	bool GetbConnect(void) { return m_bConnect; }			// 接続の取得
	bool GetTrigger(JOYPADKEY button);
	bool GetPress(JOYPADKEY button);
	bool GetVibration(void) { return m_bVibration; }		// バイブレーションの取得
	bool GetAllVib(void) { return m_bAllVib; }				// ゲーム内の振動するかの取得
	void GetStickLeft(float *pValueH, float *pValueV);		// 左スティックの取得
	void GetStickRight(float *pValueH, float *pValueV);		// 右スティックの取得

	void SetVibration(bool bVib) { m_bVibration = bVib; }	// バイブレーションの設定
	void SetAllVib(bool bVib) { m_bAllVib = bVib; }			// ゲーム内の振動するかの設定

private:
	HRESULT UpdateControlState(void);						// 接続されているか確認
	void	UpdateVibration(void);							// バイブレーションの更新

	XINPUT_STATE		m_state;							// コントローラーの状態
	XINPUT_STATE		m_stateOld;							// コントローラーの前回の状態
	int					m_nIndexPlayer;						// プレイヤーの番号
	bool				m_bConnect;							// 接続されているか
	bool				m_bVibration;						// 振動するか
	static bool			m_bAllVib;							// ゲーム内の振動するかの設定
};
#endif 