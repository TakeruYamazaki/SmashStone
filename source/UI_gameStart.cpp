//==================================================================================================================
//
// ゲーム開始時のUI演出 [ UI_gameStart.cpp ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS							// 警告回避文

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "UI_gameStart.h"
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
#define TIME_CREATE_ROUND		   (60)								// フェードイン後のRoundの生成時間
#define TIME_ZOOM_GAMESTART_ROUND  (20)								// UI拡大の時間
#define TIME_DELAY_GAMESTART_ROUND (TIME_ZOOM_GAMESTART_ROUND + 30) // fight前のディレイ
#define TIME_ZOOM_GAMESTART_FIGHT  (20)								// UI拡大の時間
#define TIME_DELAY_GAMESTART_FIGHT (50)								// 画面外に移動する前のディレイ
#define TIME_LEAVE_GAMESTART_FIGHT (20)								// UIが消える時間
#define TIME_END_GAMESTART		   (70)								// この処理を終える時間

#define POS_SLIDE_FIGHT			   (D3DXVECTOR3(SCREEN_WIDTH + 450.0f, 0.0f, 0.0f))	// fightのずれる値

//==================================================================================================================
// 静的メンバ変数宣言
//==================================================================================================================
LPDIRECT3DTEXTURE9	CUI_GameStart::m_pTexture[GAMEUITEX_MAX] = {};
CPolygon2D			*CUI_GameStart::m_pPolygon[STARTUITYPE_MAX] = {};
char				*CUI_GameStart::m_apFileName[GAMEUITEX_MAX] =
{
	{ "data/TEXTURE/round.png" },
	{ "data/TEXTURE/round_1.png" },
	{ "data/TEXTURE/round_2.png" },
	{ "data/TEXTURE/round_3.png" },
	{ "data/TEXTURE/fight.png" }
};

D3DXVECTOR3			CUI_GameStart::m_sizeBegin[STARTUITYPE_MAX]
{
	D3DXVECTOR3(100.0f, 50.0f, 0.0f),
	D3DXVECTOR3(900.0f, 900.0f, 0.0f),
	D3DXVECTOR3(750.0f, 250.0f, 0.0f)
};

D3DXVECTOR3			CUI_GameStart::m_sizeEnd[STARTUITYPE_MAX]
{
	D3DXVECTOR3(700.0f, 250.0f, 0.0f),
	D3DXVECTOR3(200.0f, 250.0f, 0.0f),
	D3DXVECTOR3(750.0f, 250.0f, 0.0f)
};

D3DXVECTOR3			CUI_GameStart::m_posBegin[STARTUITYPE_MAX]
{
	D3DXVECTOR3(500.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(700.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(-450.0f, SCREEN_HEIGHT / 2, 0.0f)
};

D3DXVECTOR3			CUI_GameStart::m_posEnd[STARTUITYPE_MAX]
{
	D3DXVECTOR3(500.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(1000.0f, SCREEN_HEIGHT / 2, 0.0f),
	D3DXVECTOR3(SCREEN_WIDTH / 2 + 50.0f, SCREEN_HEIGHT / 2, 0.0f)
};

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CUI_GameStart::CUI_GameStart()
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CUI_GameStart::~CUI_GameStart()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUI_GameStart::Init(void)
{
	// 要素の初期化
	m_fase = FASE_NONE;
	m_nCntAny = 0;
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CUI_GameStart::Uninit(void)
{
	for (int nCnt = 0; nCnt < STARTUITYPE_MAX; nCnt++)
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
void CUI_GameStart::Update(void)
{
	// カウンタの加算
	m_nCntAny++;

	if (m_fase == FASE_NONE)
	{
		if (m_nCntAny == TIME_CREATE_ROUND)
		{
			// 次のフェーズに移行
			NextFase();
			// 現在のラウンドの番号生成
			CreateUI(m_fase);
		}
	}
	else if (m_fase == FASE_ROUND)
	{
		// UIの移動・拡縮
		if (m_nCntAny < TIME_ZOOM_GAMESTART_ROUND)
			MoveUI(m_fase, TIME_ZOOM_GAMESTART_ROUND);
		else
		{
			// 次のフェーズに移行
			NextFase();
			// 現在のラウンドの番号生成
			CreateUI(m_fase);
		}
	}
	else if (m_fase == FASE_RONNDNUM)
	{
		// UIの移動・拡縮
		if (m_nCntAny < TIME_ZOOM_GAMESTART_ROUND)
			MoveUI(m_fase, TIME_ZOOM_GAMESTART_ROUND);
		else if (m_nCntAny == TIME_DELAY_GAMESTART_ROUND)
		{
			// 今まで出していたものを非表示にする
			m_pPolygon[STARTUITYPE_NUMROUND]->SetbShow(false);
			m_pPolygon[STARTUITYPE_ROUND]->SetbShow(false);
			// 次のフェーズに移行
			NextFase();
			// 現在のラウンドの番号生成
			CreateUI(m_fase);
		}
	}
	else if (m_fase == FASE_FIGHT)
	{
		// UIの移動・拡縮
		if (m_nCntAny < TIME_ZOOM_GAMESTART_FIGHT)
			MoveUI(m_fase, TIME_ZOOM_GAMESTART_FIGHT);
		// UIの画面外移動
		else if (m_nCntAny < TIME_LEAVE_GAMESTART_FIGHT + TIME_DELAY_GAMESTART_FIGHT && m_nCntAny > TIME_DELAY_GAMESTART_FIGHT)
			m_pPolygon[m_fase]->SetPos(*m_pPolygon[m_fase]->GetPos() + POS_SLIDE_FIGHT / TIME_LEAVE_GAMESTART_FIGHT);
		// モードを切り替えて処理を終了
		else if (m_nCntAny > TIME_END_GAMESTART)
			CGame::SetGameState(CGame::GAMESTATE_NORMAL);
	}

	for (int nCnt = 0; nCnt < STARTUITYPE_MAX; nCnt++)
	{
		if (m_pPolygon[nCnt])
			m_pPolygon[nCnt]->Update();
	}
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI_GameStart::Draw(void)
{	
	for (int nCnt = 0; nCnt < STARTUITYPE_MAX; nCnt++)
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
CUI_GameStart * CUI_GameStart::Create(void)
{
	// メモリ確保
	CUI_GameStart *pUI = new CUI_GameStart;
	// 初期化
	pUI->Init();
	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャ情報ロード
//==================================================================================================================
HRESULT CUI_GameStart::Load(void)
{
	// デバイスを取得する
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ブロックコメント
	CKananLibrary::StartBlockComment("ゲーム開始時のテクスチャの読み込み開始");

	// テクスチャの読み込み
	for (int nCnt = 0; nCnt < GAMEUITEX_MAX; nCnt++)
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
void CUI_GameStart::Unload(void)
{
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < GAMEUITEX_MAX; nCnt++)
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
void CUI_GameStart::CreateUI(const int type)
{
	// 生成
	m_pPolygon[type] = CPolygon2D::Create();
	// テクスチャバインド
	if (type == FASE_RONNDNUM)
		m_pPolygon[type]->BindTexture(m_pTexture[CGame::GetRound() + 1]);
	else if (type == FASE_FIGHT)
		m_pPolygon[type]->BindTexture(m_pTexture[GAMEUITEX_FIGHT]);
	else
		m_pPolygon[type]->BindTexture(m_pTexture[type]);

	// 位置とサイズを設定
	m_pPolygon[type]->SetSize(m_sizeBegin[type]);
	m_pPolygon[type]->SetPos(m_posBegin[type]);
	m_pPolygon[type]->SetPosStart(CPolygon2D::POSSTART_CENTRAL_CENTRAL);
}

//==================================================================================================================
// UIの移動
//==================================================================================================================
void CUI_GameStart::MoveUI(const int type, const int time)
{
	// 位置とサイズの差分を求める
	D3DXVECTOR3 difPos = m_posEnd[type] - m_posBegin[type];
	D3DXVECTOR3 difSize = m_sizeEnd[type] - m_sizeBegin[type];
	// 段々移動
	m_pPolygon[type]->SetPos(*m_pPolygon[type]->GetPos() + difPos / (float)time);
	m_pPolygon[type]->SetSize(*m_pPolygon[type]->GetSize() + difSize / (float)time);
}