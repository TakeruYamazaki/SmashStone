//=============================================================================
//
//	マウス処理 [ mouse.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "mouse.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 構造体定義
//=============================================================================

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// 静的メンバ変数
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CMouse::CMouse()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CMouse::~CMouse()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	m_hWnd = hWnd;

	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		std::cout << "マウス生成失敗" << std::endl;
		return E_FAIL;
	}

	m_pDevice->SetDataFormat(&c_dfDIMouse2); // マウス用のデータ・フォーマットを設定

	m_pDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; // 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）    
	m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);

	// 入力制御開始
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CMouse::Uninit(void)
{
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CMouse::Update(void)
{
	POINT point;

	// 値の初期化    
	m_mouseState.rgbButtons[0] = 0;

	GetCursorPos(&point);
	ScreenToClient(m_hWnd, &point);
	m_posX = (float)point.x;
	m_posY = (float)point.y;

	// 値の更新 
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &m_mouseState)))
	{
		for (int nCntButton = 0; nCntButton < NUM_BUTTON_MAX; nCntButton++)
		{
			// 排他的論理和かどうか判断
			m_aButtonStateTrigger[nCntButton] = (m_aButtonState[nCntButton] ^ m_mouseState.rgbButtons[nCntButton]) & m_mouseState.rgbButtons[nCntButton];
			m_aButtonState[nCntButton] = m_mouseState.rgbButtons[nCntButton];	// キープレス情報保存
		}
	}
	else
	{
		m_pDevice->Acquire(); // １発目や２発目にエラーが出るが無視してよい。 
	}
}

//=============================================================================
// プレス処理
//=============================================================================
bool CMouse::GetPress(int nButton)
{
	return(m_mouseState.rgbButtons[nButton] & 0x80) ? true : false;
}

//=============================================================================
// トリガー処理
//=============================================================================
bool CMouse::GetPressTrigger(int nButton)
{
	return(m_aButtonStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
// マウスのX座標の取得
//=============================================================================
LONG CMouse::GetposX(void)
{
	return (LONG)m_posX;
}

//=============================================================================
// マウスのY座標の取得
//=============================================================================
LONG CMouse::GetposY(void)
{
	return (LONG)m_posY;
}

//=============================================================================
// ウィンドウハンドルの取得
//=============================================================================
HWND CMouse::GetWnd(void)
{
	return m_hWnd;
}
