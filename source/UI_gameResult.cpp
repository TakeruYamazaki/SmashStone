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

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define POS_END_VICTORY		(D3DXVECTOR3(SCREEN_WIDTH / 2, 550.0f, 0.0f))	// victoryの移動後の位置
#define SIZE_END_VICTORY	(D3DXVECTOR3(700.0f, 300.0f, 0.0f))				// victoryの拡大後のサイズ
#define TIME_ZOOM_VICTORY	(60)											// victoryの移動・拡大の時間

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
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 200.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 200.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 200.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 200.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 200.0f, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT + 200.0f, 0.0f)
};

D3DXVECTOR3		  CUI_GameResult::m_sizeBegin[RESULTUITYPE_MAX]
{
	D3DXVECTOR3(300.0f, 100.0f, 0.0f),
	D3DXVECTOR3(300.0f, 100.0f, 0.0f),
	D3DXVECTOR3(300.0f, 100.0f, 0.0f),
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
	// カウンタの加算
	m_nCntAny++;

	if (m_fase == FASE_VICTORY && m_nCntAny < TIME_ZOOM_VICTORY)
		MoveUI();
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
// UIの生成
//==================================================================================================================
void CUI_GameResult::CreateUI(const int type)
{
	// 生成
	m_pPolygon[type] = CPolygon2D::Create();
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