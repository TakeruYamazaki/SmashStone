//==================================================================================================================
//
// ゲームリザルトのUI演出 [ UI_gameResult.cpp ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS							// 警告回避文

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "UI_gameResult.h"
#include "renderer.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "inputGamepad.h"
#include "game.h"
#include "fade.h"
#include "kananlibrary.h"
#include "polygon2D.h"
#include "inputGamepad.h"
#include "inputKeyboard.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define POS_END_VICTORY		(D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f))	// victoryの移動後の位置
#define SIZE_END_VICTORY	(D3DXVECTOR3(800.0f, 260.0f, 0.0f))				// victoryの拡大後のサイズ
#define TIME_ZOOM_VICTORY	(30)											// victoryの移動・拡大の時間
#define TIME_SHOW_VICTORY	(90)											// victoryを見せる時間

#define TIME_INPUT_MAX		(10)											// 連続入力防止用

//==================================================================================================================
// 静的メンバ変数宣言
//==================================================================================================================
LPDIRECT3DTEXTURE9	CUI_GameResult::m_pTexture[RESULTUITYPE_MAX] = {};
CPolygon2D			*CUI_GameResult::m_pPolygon[RESULTUITYPE_MAX] = {};
char				*CUI_GameResult::m_apFileName[RESULTUITYPE_MAX] =
{
	{ "data/TEXTURE/victory.png" },
	{ "data/TEXTURE/frame.png" },
	{ "data/TEXTURE/select.png" },
	{ "data/TEXTURE/rematch.png" },
	{ "data/TEXTURE/return_select.png" },
	{ "data/TEXTURE/return_title.png" }
};

D3DXVECTOR3		  CUI_GameResult::m_posBegin[RESULTUITYPE_MAX]
{
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 150.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 170.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 170.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 170.0f, 0.0f)
};

D3DXVECTOR3		  CUI_GameResult::m_sizeBegin[RESULTUITYPE_MAX]
{
	D3DXVECTOR3(2000.0f, 1000.0f, 0.0f),
	D3DXVECTOR3(400.0f, 500.0f, 0.0f),
	D3DXVECTOR3(350.0f, 100.0f, 0.0f),
	D3DXVECTOR3(300.0f, 100.0f, 0.0f),
	D3DXVECTOR3(300.0f, 100.0f, 0.0f),
	D3DXVECTOR3(300.0f, 100.0f, 0.0f)
};

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CUI_GameResult::CUI_GameResult()
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CUI_GameResult::~CUI_GameResult()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUI_GameResult::Init(void)
{
	// victory生成
	CreateUI(RESULTUITYPE_VICTORY);

	// 要素の初期化
	m_fase = FASE_VICTORY;
	m_nCntAny = 0;
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CUI_GameResult::Uninit(void)
{
	// UIの数だけ繰り返す
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
	{
		// nullcheck
		if (!m_pPolygon[nCnt])
			// 処理しない
			continue;
		// 破棄
		m_pPolygon[nCnt]->Uninit();
		delete m_pPolygon[nCnt];
		m_pPolygon[nCnt] = nullptr;
	}
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUI_GameResult::Update(void)
{
	// 勝利
	if (m_fase == FASE_VICTORY)
		Victory();
	// リザルト
	else if (m_fase == FASE_RESULT)
		Result();

	// UIの更新
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
			m_pPolygon[nCnt]->Update();
	}
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI_GameResult::Draw(void)
{
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
	{
		// nullcheck
		if (!m_pPolygon[nCnt])
			// 処理しない
			continue;
		// 描画
		m_pPolygon[nCnt]->Draw();
	}
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CUI_GameResult * CUI_GameResult::Create(void)
{
	// メモリ確保
	CUI_GameResult *pUI = new CUI_GameResult;
	// 初期化
	pUI->Init();
	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャ情報ロード
//==================================================================================================================
HRESULT CUI_GameResult::Load(void)
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ブロックコメント
	CKananLibrary::StartBlockComment("ゲーム開始時のテクスチャの読み込み開始");

	// テクスチャの読み込み
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
	{
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt])))
			std::cout << m_apFileName[nCnt] << " の読み込み成功" << std::endl;	// 成功
		else
			std::cout << m_apFileName[nCnt] << " の読み込み失敗" << std::endl;	// 失敗
	}

	// 成功
	CKananLibrary::EndBlockComment("ゲーム開始時のテクスチャの読み込み終了");
	return S_OK;
}

//==================================================================================================================
// テクスチャ情報破棄
//==================================================================================================================
void CUI_GameResult::Unload(void)
{
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < RESULTUITYPE_MAX; nCnt++)
	{
		// テクスチャがあるとき
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();	// テクスチャの開放
			m_pTexture[nCnt] = NULL;		// NULLにする
		}
	}
}

//==================================================================================================================
// 勝利
//==================================================================================================================
void CUI_GameResult::Victory(void)
{
	// カウンタの加算
	m_nCntAny++;

	// UI移動
	if (m_nCntAny < TIME_ZOOM_VICTORY)
		MoveUI();
	// 少し経ってから次へ
	else if (m_nCntAny >= TIME_SHOW_VICTORY)
		NextFase();
}

//==================================================================================================================
// リザルト
//==================================================================================================================
void CUI_GameResult::Result(void)
{
	// victoryが残る最初だけ通る
	if (m_pPolygon[RESULTUITYPE_VICTORY])
	{
		// victory破棄
		m_pPolygon[RESULTUITYPE_VICTORY]->Uninit();
		delete m_pPolygon[RESULTUITYPE_VICTORY];
		m_pPolygon[RESULTUITYPE_VICTORY] = nullptr;

		// 他のUIを生成
		for (int nCnt = 1; nCnt < RESULTUITYPE_MAX; nCnt++)
		{
			CreateUI(nCnt);
		}
	}

	// 入力の取得 (ゲームパッドは負けたプレイヤー)
	CInputKeyboard *pKey = CManager::GetInputKeyboard();
	CInputGamepad *pGamepad = CManager::GetInputGamepad((int)(CGame::GetLosePlayer()));

	// 決定
	if (CKananLibrary::Decide(pKey, pGamepad))
		CursorDecide();

	static int nCntInput = 0;
	if (nCntInput < TIME_INPUT_MAX)
	{
		nCntInput++;
		return;
	}
	
	// 上昇
	if (CKananLibrary::Up(pKey, pGamepad))
	{
		CursorUp();
		nCntInput = 0;
	}
	// 下降
	else if (CKananLibrary::Down(pKey, pGamepad))
	{
		CursorDown();
		nCntInput = 0;
	}
}

//==================================================================================================================
// カーソルの上昇
//==================================================================================================================
void CUI_GameResult::CursorUp(void)
{
	// カーソル位置取得
	D3DXVECTOR3 posCursor = *m_pPolygon[RESULTUITYPE_CURSOR]->GetPos();
	
	// タイトル位置へ
	if (posCursor == m_posBegin[RESULTUITYPE_REMATCH])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_RETURNTITLE]);
	// 再戦位置へ
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNSELECT])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_REMATCH]);
	// キャラセレクト位置へ
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNTITLE])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_RETURNSELECT]);

}

//==================================================================================================================
// カーソルの下降
//==================================================================================================================
void CUI_GameResult::CursorDown(void)
{
	// カーソル位置取得
	D3DXVECTOR3 posCursor = *m_pPolygon[RESULTUITYPE_CURSOR]->GetPos();

	// キャラセレクト位置へ
	if (posCursor == m_posBegin[RESULTUITYPE_REMATCH])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_RETURNSELECT]);
	// タイトル位置へ
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNSELECT])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_RETURNTITLE]);
	// 再戦位置へ
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNTITLE])
		m_pPolygon[RESULTUITYPE_CURSOR]->SetPos(m_posBegin[RESULTUITYPE_REMATCH]);
}

//==================================================================================================================
// 選択したものの決定
//==================================================================================================================
void CUI_GameResult::CursorDecide(void)
{
	// カーソル位置取得
	D3DXVECTOR3 posCursor = *m_pPolygon[RESULTUITYPE_CURSOR]->GetPos();

	// もう一度ゲームへ
	if (posCursor == m_posBegin[RESULTUITYPE_REMATCH])
		CGame::SetNextMode(CRenderer::MODE_GAME);
	// タイトル位置へ
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNSELECT])
		CGame::SetNextMode(CRenderer::MODE_TUTORIAL);
	// 再戦位置へ
	else if (posCursor == m_posBegin[RESULTUITYPE_RETURNTITLE])
		CGame::SetNextMode(CRenderer::MODE_TITLE);
}

//==================================================================================================================
// UIの生成
//==================================================================================================================
void CUI_GameResult::CreateUI(const int type)
{
	// 生成
	m_pPolygon[type] = CPolygon2D::Create();
	m_pPolygon[type]->BindTexture(m_pTexture[type]);
	// 位置とサイズを設定
	m_pPolygon[type]->SetSize(m_sizeBegin[type]);
	m_pPolygon[type]->SetPos(m_posBegin[type]);
	m_pPolygon[type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}

//==================================================================================================================
// UIの移動
//==================================================================================================================
void CUI_GameResult::MoveUI(void)
{
	// 位置とサイズの差分を求める
	D3DXVECTOR3 difPos = POS_END_VICTORY - m_posBegin[RESULTUITYPE_VICTORY];
	D3DXVECTOR3 difSize = SIZE_END_VICTORY - m_sizeBegin[RESULTUITYPE_VICTORY];
	// 段々移動
	m_pPolygon[RESULTUITYPE_VICTORY]->SetPos(*m_pPolygon[RESULTUITYPE_VICTORY]->GetPos() + difPos / (float)TIME_ZOOM_VICTORY);
	m_pPolygon[RESULTUITYPE_VICTORY]->SetSize(*m_pPolygon[RESULTUITYPE_VICTORY]->GetSize() + difSize / (float)TIME_ZOOM_VICTORY);
}