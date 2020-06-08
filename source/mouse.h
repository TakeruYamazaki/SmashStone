// ===================================================================
//
// マウス処理 [ mouse.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "input.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define NUM_BUTTON_MAX (8)

// ===================================================================
// クラス定義
// ===================================================================
class CMouse : public CInput
{
public:
	CMouse();
	~CMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	bool GetPress(int nKey);						// プレス取得
	bool GetPressTrigger(int nKey);					// トリガー取得
	LONG GetposX(void);								// カーソル座標(X)の取得
	LONG GetposY(void);								// カーソル座標(Y)の取得
	HWND GetWnd(void);								// hWndの取得

private:
	DIMOUSESTATE2 m_mouseState;						// マウスの状態
	float m_posX;									// マウス座標(X)
	float m_posY;									// マウス座標(Y)

	LPDIRECTINPUTDEVICE8	m_pDevice = NULL;		// 入力デバイス(マウス)へのポインタ

	BYTE	m_aButtonState[NUM_BUTTON_MAX];			// マウスの入力情報ワーク
	BYTE	m_aButtonStateTrigger[NUM_BUTTON_MAX];	// マウスのトリガー

	HWND	m_hWnd;									// ウィンドウハンドル
};

#endif