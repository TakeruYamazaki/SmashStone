//==================================================================================================================
//
// 管理[manager.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "manager.h"
#include "scene2D.h"
#include "scene3D.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "inputGamepad.h"
#include "mouse.h"

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================
CRenderer *CManager::m_pRenderer = NULL;				// レンダラー情報
CInputKeyboard *CManager::m_pInputKeyboard = NULL;		// キーボード情報
CInputGamepad *CManager::m_pInputGamepad[MAX_PLAYER] = {};		// ゲームパッド情報
CMouse *CManager::m_pMouse = nullptr;

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CManager::CManager()
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CManager::~CManager()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
HRESULT CManager::Init(HINSTANCE hInstance,HWND hWnd, BOOL bWindow)
{
	// レンダラー動的に確保
	m_pRenderer = new CRenderer;

	// キーボード動的に確保
	m_pInputKeyboard = new CInputKeyboard;

	// コントローラー動的に確保
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pInputGamepad[nCnt] = new CInputGamepad;
	}

	m_pMouse = new CMouse;

	// 初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return-1;
	}

	// キーボード初期化
	m_pInputKeyboard->Init(hInstance, hWnd);

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// コントローラー初期化
		m_pInputGamepad[nCnt]->Init(hInstance, hWnd, nCnt);
		if (!m_pInputGamepad[nCnt]->GetbConnect())
		{
			printf("ゲームパッド[%d]が読み込まれていません\n", nCnt);
		}
	}

	m_pMouse->Init(hInstance, hWnd);

	// 値を返す
	return S_OK;
}

//==================================================================================================================
// 終了
//==================================================================================================================
void CManager::Uninit(void)
{
	// 全てを破棄
	CScene::ReleaseAll();

	// レンダラーがあるとき
	if (m_pRenderer != NULL)
	{
		// レンダラー終了処理
		m_pRenderer->Uninit();

		// レンダラー破棄
		delete m_pRenderer;

		// NULLにする
		m_pRenderer = NULL;
	}

	// キーボードがあるとき
	if (m_pInputKeyboard != NULL)
	{
		// キーボードの終了処理
		m_pInputKeyboard->Uninit();

		// 破棄
		delete m_pInputKeyboard;

		// NULLにする
		m_pInputKeyboard = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// nullcheck
		if (m_pInputGamepad)
		{
			// 破棄
			m_pInputGamepad[nCnt]->Uninit();
			delete m_pInputGamepad[nCnt];
			m_pInputGamepad[nCnt] = nullptr;
		}
	}

	if (m_pMouse)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = nullptr;
	}
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CManager::Update(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();					// レンダラーの情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();				// デバイスを取得する

	//キーボード取得
	CInputKeyboard *pInputKeyboard = GetInputKeyboard();

	// レンダラーがあるとき
	if (m_pRenderer != NULL)
	{
		// レンダラー更新処理
		m_pRenderer->Update();
	}

	// キーボードがあるとき
	if (m_pInputKeyboard != NULL)
	{
		// キーボード更新処理
		m_pInputKeyboard->Update();
	}

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pInputGamepad[nCnt]->Update();
	}

	if (m_pMouse)
		m_pMouse->Update();
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CManager::Draw(void)
{
	// レンダラーがあるとき
	if (m_pRenderer != NULL)
	{
		// レンダラーの描画処理
		m_pRenderer->Draw();
	}
}

//==================================================================================================================
// レンダラー情報取得
//==================================================================================================================
CRenderer *CManager:: GetRenderer(void)
{
	return m_pRenderer;
}

//==================================================================================================================
// キーボード情報取得
//==================================================================================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//==================================================================================================================
// コントローラー情報取得
//==================================================================================================================
CInputGamepad * CManager::GetInputGamepad(int nPlayer)
{
	return m_pInputGamepad[nPlayer];
}
