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
#include "kananlibrary.h"
#include "objManager.h"
#include "charaParam.h"

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================
CRenderer *CManager::m_pRenderer = NULL;				// レンダラー情報
CInputKeyboard *CManager::m_pInputKeyboard = NULL;		// キーボード情報
CInputGamepad *CManager::m_pInputGamepad[MAX_PLAYER] = {};		// ゲームパッド情報
CMouse *CManager::m_pMouse = nullptr;

#ifdef _DEBUG
char CManager::m_cFileImGui[64]	= "data/TEXT/ImGuiInfo.txt";
bool CManager::m_bShowWindow	= false;
#endif

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

	// ImGui情報のロード
	LoadImGuiInfo();
	
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

	// ImGui情報のアンロード
	SaveImGuiInfo();
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

#ifdef _DEBUG
//==================================================================================================================
// ImGui情報の読み込み
//==================================================================================================================
HRESULT CManager::LoadImGuiInfo(void)
{
	// 変数宣言
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";

	CKananLibrary::StartBlockComment("ImGuiの設定の読み込み開始");

	// ファイルを開く
	pFile = fopen(m_cFileImGui, "r");

	// nullcheck
	if (!pFile)
	{
		// 失敗
		printf("ファイルを開けませんでした\n");
		return E_FAIL;
	}

	// スクリプトがくるまで繰り返す
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		// 一行ずつ読み込み
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);
	}

	// エンドスクリプトが来るまで繰り返す
	while (strcmp(cHeadText, "END_SCRIPT") != 0)
	{
		// 一行ずつ読み込み
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);

		// 改行
		if (strcmp(cHeadText, "\n") == 0)
			continue;
		// 表示の有無
		else if (strcmp(cHeadText, "SHOW_DEBUG_WINDOW") == 0)
		{
			// 表示の読み込み
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// 表示
			if (bShow == 1)
				m_bShowWindow = true;
		}
		// 表示の有無
		else if (strcmp(cHeadText, "SHOW_OBJECT_WINDOW") == 0)
		{
			// 表示の読み込み
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// 表示
			if (bShow == 1)
				CObjectManager::SetShowObjWindow(true);
		}
		// 表示の有無
		else if (strcmp(cHeadText, "SHOW_CHARAPARAM_WINDOW") == 0)
		{
			// 表示の読み込み
			int bShow = 0;
			sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &bShow);
			// 表示
			if (bShow == 1)
				CCharaParam::SetShowWindow(true);
		}
	}
	// ファイルを閉じる
	fclose(pFile);

	// 成功
	CKananLibrary::EndBlockComment("パラメーターファイルの読み込み終了");
	return S_OK;
}

//==================================================================================================================
// ImGui情報の保存
//==================================================================================================================
HRESULT CManager::SaveImGuiInfo(void)
{
	// 変数宣言
	FILE *pFile;

	// テキストファイルを見やすくするコメント
	char cEqual[2] = "=";
	// 変数名用と書き込み用の変数
	char cHeadText[MAX_TEXT];
	char cWriteText[MAX_TEXT];

	// コンソールに表示
	CKananLibrary::StartBlockComment("ImGuiの設定のセーブ開始");

	// ファイルを開く
	pFile = fopen(m_cFileImGui, "w");

	// nullcheck
	if (!pFile)
	{
		// コンソールに表示
		CKananLibrary::EndBlockComment("ファイルを開けませんでした");
		// 失敗
		return E_FAIL;
	}

	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs(COMMENT_BLOCK_LINE, pFile);												// #
	fputs("# ImGui設定\n", pFile);													// # ImGui設定
	fputs(COMMENT_AUTHOR, pFile);													// # Author : KANAN NAGANAWA
	fputs(COMMENT_BLOCK_LINE, pFile);												// #
	fputs(COMMENT_BLOCK, pFile);													// #=====================================================
	fputs("SCRIPT	# 消さないで\n", pFile);										// SCRIPT
	fputs(COMMENT_NEW_LINE, pFile);													// \n

	strcpy(cHeadText, "SHOW_DEBUG_WINDOW");
	sprintf(cWriteText, "%s %s %d # ImGuiウィンドウ表示の有無\n",
		&cHeadText,
		&cEqual,
		m_bShowWindow);
	fputs(cWriteText, pFile);														// SHOW_DEBUG_WINDOW = bShow

	strcpy(cHeadText, "SHOW_OBJECT_WINDOW");
	sprintf(cWriteText, "%s %s %d # オブジェクト情報表示の有無\n",
		&cHeadText,
		&cEqual,
		CObjectManager::GetShowObjWindow());
	fputs(cWriteText, pFile);														// SHOW_OBJECT_WINDOW = bShow

	strcpy(cHeadText, "SHOW_CHARAPARAM_WINDOW");
	sprintf(cWriteText, "%s %s %d # キャラパラメーター表示の有無\n",
		&cHeadText,
		&cEqual,
		CCharaParam::GetShowWindow());
	fputs(cWriteText, pFile);														// SHOW_CHARAPARAM_WINDOW = bShow

	fputs(COMMENT_NEW_LINE, pFile);													// \n

	fputs("END_SCRIPT	# 消さないで \n", pFile);									// END_SCRIPT

	// ファイルを閉じる
	fclose(pFile);

	// 成功
	CKananLibrary::EndBlockComment("ImGuiの設定のセーブ終了");
	return S_OK;
}

#endif