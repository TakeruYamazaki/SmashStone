//==================================================================================================================
//
// KOのUI演出 [ UI_KO.cpp ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS							// 警告回避文

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "UI_KO.h"
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
#define TIME_ZOOM_KO		(30)								// KOの拡大の時間
#define TIME_SLIDE_SHADOW	(10)								// 影がスライドする時間
#define TIME_AFTER_SLIDE	(60)								// スライド後の余韻
#define POS_SLIDE_SHADOW	(D3DXVECTOR3(10.0f, 10.0f, 0.0f))	// 影との座標差

//==================================================================================================================
// 静的メンバ変数宣言
//==================================================================================================================
LPDIRECT3DTEXTURE9	CUIKO::m_pTexture[KOUI_MAX]		= {};
CPolygon2D			*CUIKO::m_pPolygon[KOUI_MAX]	= {};
char				*CUIKO::m_apFileName[KOUI_MAX]	=
{
	{ "data/TEXTURE/K.png" },
	{ "data/TEXTURE/O.png" },
	{ "data/TEXTURE/KO_back.png" }
};

D3DXVECTOR3			CUIKO::m_sizeBegin[KOUI_MAX] =
{
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f),
	D3DXVECTOR3(2400.0f, 2400.0f, 0.0f),
	D3DXVECTOR3(2000.0f, 1200.0f, 0.0f)
};

D3DXVECTOR3			CUIKO::m_sizeEnd[KOUI_MAX] =
{
	D3DXVECTOR3(755.0f, 640.0f, 0.0f),
	D3DXVECTOR3(755.0f, 640.0f, 0.0f),
	D3DXVECTOR3(2000.0f, 1200.0f, 0.0f)
};

D3DXVECTOR3			CUIKO::m_posBegin[KOUI_MAX] =
{
	D3DXVECTOR3(120.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(1110.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, 0.0f)
};

D3DXVECTOR3			CUIKO::m_posEnd[KOUI_MAX] =
{
	D3DXVECTOR3(325.0f, SCREEN_HEIGHT / 2 + 15, 0.0f),
	D3DXVECTOR3(945.0f, SCREEN_HEIGHT / 2 + 15, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, 0.0f)
};

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CUIKO::CUIKO()
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CUIKO::~CUIKO()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUIKO::Init(void)
{
	// 生成してテクスチャバインド
	CreateUI(KOUITEX_BACK);
	m_pPolygon[KOUITEX_BACK]->SetbShow(false);

	// 生成してテクスチャバインド
	CreateUI(KOUITEX_K);

	// 要素の初期化
	m_nCntFase = 0;
	m_nCntAny  = 0;
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CUIKO::Uninit(void)
{
	for (int nCnt = 0; nCnt < KOUI_MAX; nCnt++)
	{
		if (!m_pPolygon[nCnt])
			continue;
		m_pPolygon[nCnt]->Uninit();
		delete m_pPolygon[nCnt];
		m_pPolygon[nCnt] = nullptr;
	}
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUIKO::Update(void)
{
	// 移動時間を加算
	m_nCntAny++;

	// UIの拡縮
	if (m_nCntAny < TIME_ZOOM_KO && m_nCntFase < KOUITEX_BACK)
		// UIの移動
		MoveUI();

	// Oの生成
	if (m_nCntAny == TIME_ZOOM_KO && m_nCntFase < KOUITEX_O)
	{
		// 次のフェーズ
		NextFase();
		// 生成してテクスチャバインド
		CreateUI(m_nCntFase);
	}

	// Oの拡縮が終了した
	if (m_nCntFase == KOUITEX_O && m_nCntAny == TIME_ZOOM_KO)
	{
		// 次のフェーズ
		NextFase();
		// 影を表示
		m_pPolygon[KOUITEX_BACK]->SetbShow(true);
	}

	// UIのスライド
	if (m_nCntFase == KOUITEX_BACK)
	{
		if (m_nCntAny < TIME_SLIDE_SHADOW)
		{
			// KOを左上にずらす
			for (int nCnt = 0; nCnt < KOUITEX_BACK; nCnt++)
			{
				m_pPolygon[nCnt]->SetPos(*m_pPolygon[nCnt]->GetPos() - POS_SLIDE_SHADOW / TIME_SLIDE_SHADOW);
			}
			// 影を反対にずらす
			m_pPolygon[KOUITEX_BACK]->SetPos(*m_pPolygon[KOUITEX_BACK]->GetPos() + POS_SLIDE_SHADOW / TIME_SLIDE_SHADOW);
		}
		else
			// 次のフェーズ
			NextFase();
	}

	if (m_nCntFase == KOUI_MAX && m_nCntAny == TIME_AFTER_SLIDE)
		CGame::SetGameState(CGame::GAMESTATE_NEXTROUND);

	for (int nCnt = 0; nCnt < KOUI_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
			m_pPolygon[nCnt]->Update();
	}
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUIKO::Draw(void)
{
	// 背景を先に描画
	if (m_pPolygon[KOUITEX_BACK]->GetbShow())
		m_pPolygon[KOUITEX_BACK]->Draw();

	for (int nCnt = 0; nCnt < KOUITEX_BACK; nCnt++)
	{
		// 無ければ処理しない
		if (!m_pPolygon[nCnt])
			continue;
		// 描画
		m_pPolygon[nCnt]->Draw();
	}
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CUIKO * CUIKO::Create(void)
{
	// メモリ確保
	CUIKO *pUI = new CUIKO;
	// 初期化
	pUI->Init();
	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャ情報ロード
//==================================================================================================================
HRESULT CUIKO::Load(void)
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ブロックコメント
	CKananLibrary::StartBlockComment("KO演出のテクスチャの読み込み開始");

	// テクスチャの読み込み
	for (int nCnt = 0; nCnt < KOUI_MAX; nCnt++)
	{
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt])))
			std::cout << m_apFileName[nCnt] << " の読み込み成功" << std::endl;	// 成功
		else
			std::cout << m_apFileName[nCnt] << " の読み込み失敗" << std::endl;	// 失敗
	}

	// 成功
	CKananLibrary::EndBlockComment("KO演出のテクスチャの読み込み終了");
	return S_OK;
}

//==================================================================================================================
// テクスチャ情報破棄
//==================================================================================================================
void CUIKO::Unload(void)
{
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < KOUI_MAX; nCnt++)
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
// UIの移動
//==================================================================================================================
void CUIKO::MoveUI(void)
{
	// 位置とサイズの差分を求める
	D3DXVECTOR3 difPos = m_posEnd[m_nCntFase] - m_posBegin[m_nCntFase];
	D3DXVECTOR3 difSize = m_sizeEnd[m_nCntFase] - m_sizeBegin[m_nCntFase];
	// 段々移動
	m_pPolygon[m_nCntFase]->SetPos(*m_pPolygon[m_nCntFase]->GetPos() + difPos / TIME_ZOOM_KO);
	m_pPolygon[m_nCntFase]->SetSize(*m_pPolygon[m_nCntFase]->GetSize() + difSize / TIME_ZOOM_KO);
}

//==================================================================================================================
// UIの生成
//==================================================================================================================
void CUIKO::CreateUI(int type)
{
	// 生成
	m_pPolygon[type] = CPolygon2D::Create();
	// テクスチャバインド
	m_pPolygon[type]->BindTexture(m_pTexture[type]);
	// 位置とサイズを設定
	m_pPolygon[type]->SetSize(m_sizeBegin[type]);
	m_pPolygon[type]->SetPos(m_posBegin[type]);
	m_pPolygon[type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}
