//==================================================================================================================
//
// 管理[manager.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CRenderer;
class CInputKeyboard;
class CInputGamepad;
class CMouse;

//==================================================================================================================
//
// マネージャクラスの定義
//
//==================================================================================================================
class CManager
{
public:
	CManager();														// コンストラクタ
	~CManager();													// デストラクタ
	HRESULT Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow);		// 初期化処理
	void Uninit(void);												// 終了処理
	void Update(void);												// 更新処理
	void Draw(void);												// 描画処理

	static CRenderer *GetRenderer(void);							// レンダラーの取得
	static CInputKeyboard *GetInputKeyboard(void);					// キーボードの取得
	static CInputGamepad *GetInputGamepad(int nPlayer);				// ゲームパッドの取得
	static CMouse *GetMouse(void) { return m_pMouse; }				// マウスの取得

#ifdef _DEBUG
	static bool GetShowImGui(void) { return m_bShowWindow; }		// 表示の有無の取得
	static void SetShowImGui(bool bShow) { m_bShowWindow = bShow; }	// 表示の有無の設定
#endif

protected:

private:
	static CMouse	*m_pMouse;										// マウスのポインタ
	static CRenderer *m_pRenderer;									// レンダラー情報のポインタ
	static CInputKeyboard *m_pInputKeyboard;						// キーボード情報のポインタ
	static CInputGamepad *m_pInputGamepad[MAX_PLAYER];				// ゲームパッド情報のポインタ

#ifdef _DEBUG
	HRESULT LoadImGuiInfo(void);			// ImGui情報の読み込み
	HRESULT SaveImGuiInfo(void);			// ImGui情報の保存
	static char	m_cFileImGui[64];		// ImGui設定のファイル名
	static bool	m_bShowWindow;			// 表示の有無
#endif
};
#endif