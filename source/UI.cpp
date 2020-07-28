//==================================================================================================================
//
// UI処理[UI.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "scene2D.h"
#include "debugProc.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define ENTER_SIZEX 300.0f							// エンターロゴX
#define ENTER_SIZEY 150.0f							// エンターロゴY
#define TITLE_ENTER_POSY 600.0f						// タイトルエンターロゴ位置Y
#define NORMAL_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)// 画像のままの色
#define TITLEUI_BEGIN_X 2000						// タイトルUI最初の横の大きさ
#define TITLEUI_BEGIN_Y 1200						// タイトルUI最初の縦の大きさ
#define TITLEUI_SMALL_SPEED 15						// タイトルUIの小さくする速度
#define TITLEUI_VALUE_Y	350							// 減少サイズの値Y
#define TITLEUI_LAST_X 1135							// タイトルUI最後の値X
#define TITLEUI_BOUND_SPEED 5						// タイトルUIのバウンド速度
#define TITLEUI_MAXSIZE_VALUE_Y 370					// タイトルUI最大サイズ縦
#define TITLEUI_MINSIZE_VALUE_Y 330					// タイトルUI最大サイズ縦
#define TITLEUI_BOUND_COUNT 60						// タイトルUIバウンドカウンタ
#define TITLEUI_FINISH_Y 250						// タイトルUIの最後の位置Y
#define TITLEUI_UP_SPEED 2							// タイトルUI上がる速度
#define ENTERUI_SIZE_X 950							// エンターUI大きさ横
#define ENTERUI_SIZE_Y 80							// エンターUI大きさ縦
#define ENTERUI_POS_Y 600							// エンターUI位置Y
#define ENTERUI_ALPHA 0.03f							// エンターUIα値変更値
#define SIGNUI_BEGINPOS_Y 800						// 看板UI最初の位置
#define SIGNUI_LASTPOS_Y 600						// 看板UI最後の位置
#define SIGNUI_LASTPOS_Y 600						// 看板UI最後の位置
#define SIGNUI_MOVE_SPEED 8.0f						// 看板UI動く速度
#define SIGNUI_SIZE_X 700							// 看板UI大きさX
#define SIGNUI_SIZE_Y 500							// 看板UI大きさY
#define MODE00UI_BEGINPOS_X 480						// モード00UI最初の位置X
#define MODE01UI_BEGINPOS_X 810						// モード01UI最初の位置X
#define MODEUI_BEGINPOS_Y 780						// モードUI最初の位置Y
#define MODEUI_SIZE_X 300							// モードUI大きさX
#define MODEUI_SIZE_Y 120							// モードUI大きさY
#define CLOCKUI0_POS_X 290							// 時計UI位置X
#define CLOCKUI1_POS_X 990							// 時計UI位置X
#define CHARATEX_SISE_X 350							// キャラテクスチャ大きさX
#define CHARATEX_SISE_Y 450							// キャラテクスチャ大きさY
#define CHARAUI_POS_Y 600							// 全員キャラUI位置Y
#define CHARAFULLUI_SIZE_X 720						// 全員キャラUI大きさX
#define CHARAFULLUI_SIZE_Y 200						// 全員キャラUI大きさY
#define CHARAFREAMUI_SIZE_X 180						// キャラUI大きさX

//==================================================================================================================
// 静的メンバー変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI::m_pTexture[LOGOTYPE_MAX] = {};		// テクスチャ情報
char *CUI::m_apFileName[LOGOTYPE_MAX] =						// 読み込むモデルのソース先
{
	{ "data/TEXTURE/title.png" },		// タイトル
	{ "data/TEXTURE/PressStart.png" },	// エンター
	{ "data/TEXTURE/arrow.png" },		// コンパス矢印
	{ "data/TEXTURE/sign00.png" },		// 看板
	{ "data/TEXTURE/Mode00.png" },		// モード00
	{ "data/TEXTURE/Mode01.png" },		// モード01
	{ "data/TEXTURE/ModeFream.png" },	// モード枠線
	{ "data/TEXTURE/gear.png" },		// 歯車0
	{ "data/TEXTURE/clock.png" },		// 時計土台0
	{ "data/TEXTURE/Clock hands.png" },	// 時計の針0
	{ "data/TEXTURE/clock gear.png" },	// 時計の歯車0
	{ "data/TEXTURE/gear.png" },		// 歯車1
	{ "data/TEXTURE/clock.png" },		// 時計土台1
	{ "data/TEXTURE/Clock hands.png" },	// 時計の針1
	{ "data/TEXTURE/clock gear.png" },	// 時計の歯車1
	{ "data/TEXTURE/FULLchara.jpg" },	// キャラクター全員
	{ "data/TEXTURE/FULLchara.jpg" },	// キャラクター全員
	{ "data/TEXTURE/FULLchara.jpg" },	// キャラクター全員
	{ "data/TEXTURE/1Pchara.png" },		// 1Pキャラクター
	{ "data/TEXTURE/2Pchara.png" },		// 2Pキャラクター
};

//==================================================================================================================
//
// コンストラクタ
//
//==================================================================================================================
CUI::CUI(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// デストラクタ
//
//==================================================================================================================
CUI::~CUI()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUI::Init(void)
{
	TitlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// タイトルの位置
	m_nCntBound = 0;			// タイトルUIバウンドカウンタ
	m_nMode = 0;				// モード番号
	m_nCharaNum[0] = 0;			// キャラ番号
	m_nCharaNum[1] = 3;			// キャラ番号
	m_nCntUITitle0 = 0;			// タイトルUI用カウンタ0
	m_nCntUITitle1 = 0;			// タイトルUI用カウンタ1
	m_nCntEnter = 0;			// エンター用カウンタ
	m_nCntUISign = 0;			// 看板用カウンタ
	m_bUITitle0 = false;		// タイトルを動かすかどうか
	m_bUITitle1 = false;		// タイトルを動かすかどうか
	m_bUIEnter = false;			// エンターのα値用変数

	// ロゴの最大枚数カウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// タイトルのとき
		if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
		{
			// タイトルで使うUIのとき
			if (nCnt <= LOGOTYPE_MODEFREAM)
			{
				// 生成処理
				m_pScene2D[nCnt] = CScene2D::Create();

				// テクスチャを貼る
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
		{// チュートリアルのとき
			// チュートリアルで使うUIのとき
			if (nCnt <= LOGOTYPE_2PCHARA_FREAM)
			{
				// 生成処理
				m_pScene2D[nCnt] = CScene2D::Create();

				// テクスチャを貼る
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}
		}
	}
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CUI::Uninit(void)
{

}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUI::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// UIタイトルの更新処理
	TitleUpdate(pInputKeyboard);

	// UIチュートリアル更新処理
	TutorialUpdate(pInputKeyboard);

}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI::Draw(void)
{

}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CUI *CUI::Create(void)
{
	// シーン動的に確保
	CUI *pUI = new CUI(CScene::PRIORITY_UI);

	// シーン初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャロード
//==================================================================================================================
HRESULT CUI::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスの取得

	//==============================テクスチャの読み込み==============================//
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// モードがタイトルのとき
		if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
		{
			// タイトルで使うUIのとき
			if (nCnt <= LOGOTYPE_MODEFREAM)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
		{// モードがチュートリアルのとき
			// チュートリアルで使うUIのとき
			if (nCnt <= LOGOTYPE_2PCHARA_FREAM)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
			}
		}
	}

	// 値を返す
	return S_OK;
}

//==================================================================================================================
// 破棄
//==================================================================================================================
void CUI::Unload(void)
{
	// ロゴの最大種類までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// モードがタイトルのとき
		if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
		{
			// タイトルで使うロゴのとき
			if (nCnt <= LOGOTYPE_MODEFREAM)
			{
				m_pTexture[nCnt]->Release();		// 開放
				m_pTexture[nCnt] = NULL;			// NULLにする
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
		{// モードがチュートリアルのとき
			// チュートリアルで使うUIのとき
			if (nCnt <= LOGOTYPE_2PCHARA_FREAM)
			{
				m_pTexture[nCnt]->Release();		// 開放
				m_pTexture[nCnt] = NULL;			// NULLにする
			}
		}
	}
}

//==================================================================================================================
// 種類取得
//==================================================================================================================
CUI::UITYPE CUI::GetType(void)
{
	return m_type;
}

//==================================================================================================================
// タイトル更新処理
//==================================================================================================================
void CUI::TitleUpdate(CInputKeyboard *pInputKeyboard)
{
	// タイトルのとき
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
	{
		// 次のモードにいくかどうか判定取得
		bool bModeNext = CTitle::GetbModeNext();

		if (!bModeNext)
		{
			// タイトルUI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + TitlePos.y, 0.0f), TITLEUI_BEGIN_X + m_nCntUITitle0 - m_nCntUITitle1,
				TITLEUI_BEGIN_Y + m_nCntUITitle0 + m_nCntUITitle1, LOGOTYPE_TITLE, NORMAL_COLOR);

			// バウンドカウントが規定値以下のとき
			if (m_nCntBound < TITLEUI_BOUND_COUNT)
			{
				// タイトルを動かしていなとき
				if (!m_bUITitle0)
				{
					// タイトルUIの縦の長さが規定値以下のとき
					if (TITLEUI_BEGIN_Y + m_nCntUITitle0 <= TITLEUI_VALUE_Y)
					{
						// タイトルを動かす状態にする
						m_bUITitle0 = true;
					}
					else
					{
						// タイトルカウンタ減算
						m_nCntUITitle0 -= TITLEUI_SMALL_SPEED;
					}
				}
				else
				{// タイトルを動かしていいとき
				 // タイトルUIの縦の長さが[360]以上のとき
					if (TITLEUI_BEGIN_Y + m_nCntUITitle0 + m_nCntUITitle1 >= TITLEUI_MAXSIZE_VALUE_Y)
					{
						// タイトルを最大まで拡大させた
						m_bUITitle1 = true;

					}
					else if (TITLEUI_BEGIN_Y + m_nCntUITitle0 + m_nCntUITitle1 <= TITLEUI_MINSIZE_VALUE_Y)
					{// タイトルUIの縦の長さが[330]以下のとき
					 // タイトルを最小まで拡小させた
						m_bUITitle1 = false;
					}

					// タイトルを最大まで拡大させたとき
					if (m_bUITitle1)
					{
						// タイトルカウンタ加算
						m_nCntUITitle1 -= TITLEUI_BOUND_SPEED;
					}
					else
					{// タイトルを最小まで拡小させたとき
					 // タイトルカウンタ減算
						m_nCntUITitle1 += TITLEUI_BOUND_SPEED;
					}

					// タイトルUIバウンドカウンタ加算
					m_nCntBound++;
				}
			}
			else
			{// バウンドカウントが規定値を超えたとき
			 // タイトルUIの位置Yが規定値以下のとき
				if (SCREEN_HEIGHT / 2 + TitlePos.y < TITLEUI_FINISH_Y)
				{
					// エンターUI
					SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, ENTERUI_POS_Y, 0.0f), ENTERUI_SIZE_X, ENTERUI_SIZE_Y,
						LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f + m_nCntEnter));

					// エンターUIのα値が1.0以上のとき
					if (1.0f + m_nCntEnter >= 1.0f)
					{
						// エンター用α値を減らす状態にする
						m_bUIEnter = true;
					}
					else if (1.0f + m_nCntEnter <= 0.0f)
					{
						// エンター用α値を増やす状態にする
						m_bUIEnter = false;
					}

					// エンターUIを減らす状態のとき
					if (m_bUIEnter)
					{
						// カウンタ減算
						m_nCntEnter -= ENTERUI_ALPHA;
					}
					else
					{// エンターUIを増やす状態のとき
					 // カウンタ加算
						m_nCntEnter += ENTERUI_ALPHA;
					}
				}
				else
				{
					// タイトル位置加算
					TitlePos.y -= TITLEUI_UP_SPEED;
				}
			}
		}
		else
		{
			// タイトルUI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, TITLEUI_FINISH_Y + m_nCntUISign / 5, 0.0f), TITLEUI_LAST_X,
				TITLEUI_VALUE_Y, LOGOTYPE_TITLE, NORMAL_COLOR);

			// エンターUI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, ENTERUI_POS_Y, 0.0f), ENTERUI_SIZE_X, ENTERUI_SIZE_Y,
				LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			// 看板UI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SIGNUI_BEGINPOS_Y + m_nCntUISign, 0.0f), SIGNUI_SIZE_X, SIGNUI_SIZE_Y, LOGOTYPE_SIGN, NORMAL_COLOR);

			// モード00UI
			SetUI(D3DXVECTOR3(MODE00UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_nCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODE00, NORMAL_COLOR);
			// モード01UI
			SetUI(D3DXVECTOR3(MODE01UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_nCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODE01, NORMAL_COLOR);

			// 看板UIが規定値より大きい時
			if (SIGNUI_BEGINPOS_Y + m_nCntUISign > SIGNUI_LASTPOS_Y)
			{
				// 看板用カウンタ減算
				m_nCntUISign -= SIGNUI_MOVE_SPEED;
			}

			// モード番号が[0]のとき
			if (m_nMode == 0)
			{
				// モード枠線UI
				SetUI(D3DXVECTOR3(MODE00UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_nCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODEFREAM, NORMAL_COLOR);
			}
			else if (m_nMode == 1)
			{
				// モード枠線UI
				SetUI(D3DXVECTOR3(MODE01UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_nCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODEFREAM, NORMAL_COLOR);
			}

			// キーボードの[→]を押したとき
			if (pInputKeyboard->GetKeyboardTrigger(DIK_RIGHTARROW))
			{
				m_nMode = 1;
			}
			else if (pInputKeyboard->GetKeyboardTrigger(DIK_LEFTARROW))
			{// キーボードの[←]を押したとき
				m_nMode = 0;
			}
		}
	}
}

//==================================================================================================================
// チュートリアル更新処理
//==================================================================================================================
void CUI::TutorialUpdate(CInputKeyboard * pInputKeyboard)
{
	// チュートリアルのとき
	if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{
		// キャラクター全員UI
		SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, CHARAUI_POS_Y, 0.0f), CHARAFULLUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_CHARAFULL, NORMAL_COLOR);

		// キーボードの[A]が押されたとき
		if (pInputKeyboard->GetKeyboardTrigger(DIK_A))
		{
			// キャラ番号減算
			m_nCharaNum[0] -= 1;
		}
		else if (pInputKeyboard->GetKeyboardTrigger(DIK_D))
		{// キーボードの[D]が押されたとき
			// キャラ番号加算
			m_nCharaNum[0] += 1;
		}

		// キーボードの[←]が押されたとき
		if (pInputKeyboard->GetKeyboardTrigger(DIK_LEFTARROW))
		{
			// キャラ番号減算
			m_nCharaNum[1] -= 1;
		}
		else if (pInputKeyboard->GetKeyboardTrigger(DIK_RIGHTARROW))
		{// キーボードの[→]が押されたとき
			// キャラ番号加算
			m_nCharaNum[1] += 1;
		}

		// 時計土台0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCK0, NORMAL_COLOR);
		// 時計の針0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKHANDS0, NORMAL_COLOR);
		// 時計の針0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKGEAR0, NORMAL_COLOR);

		// 時計土台1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCK1, NORMAL_COLOR);
		// 時計の針1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKHANDS1, NORMAL_COLOR);
		// 時計の針1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKGEAR1, NORMAL_COLOR);

		// 最大人数までカウント
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// キャラ番号が0より小さくなったとき
			if (m_nCharaNum[nCnt] < 0)
			{
				// 0に戻す
				m_nCharaNum[nCnt] = 0;
			}

			// キャラ番号が3より大きくなったとき
			if (m_nCharaNum[nCnt] > 3)
			{
				// 3に戻す
				m_nCharaNum[nCnt] = 3;
			}
		}

		// 1Pキャラクター枠線UI
		SetUI(D3DXVECTOR3(370 + (float)m_nCharaNum[0] * CHARAFREAMUI_SIZE_X, CHARAUI_POS_Y, 0.0f), 
			CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_1PCHARA_FREAM, NORMAL_COLOR);

		// 歯車0UI
		SetUI(D3DXVECTOR3(370 + (float)m_nCharaNum[0] * CHARAFREAMUI_SIZE_X, CHARAUI_POS_Y, 0.0f),
			CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_GEAR0, NORMAL_COLOR);

		// 2Pキャラクター枠線UI
		SetUI(D3DXVECTOR3(370 + (float)m_nCharaNum[1] * CHARAFREAMUI_SIZE_X, CHARAUI_POS_Y, 0.0f), 
			CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_2PCHARA_FREAM, NORMAL_COLOR);

		// 1PキャラクターUI
		SetUI(D3DXVECTOR3(200, 230, 0.0f), CHARATEX_SISE_X, CHARATEX_SISE_Y, LOGOTYPE_1PCHARA, NORMAL_COLOR);
		// テクスチャ設定
		m_pScene2D[LOGOTYPE_1PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0]);
		// 2PキャラクターUI
		SetUI(D3DXVECTOR3(1080, 230, 0.0f), CHARATEX_SISE_X, CHARATEX_SISE_Y, LOGOTYPE_2PCHARA, NORMAL_COLOR);
		// テクスチャ設定
		m_pScene2D[LOGOTYPE_2PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1]);
	}
}

//==================================================================================================================
// ロゴ作成
//==================================================================================================================
void CUI::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);									// 位置設定
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// 大きさ設定
	m_pScene2D[nCnt]->SetCol(col);									// 色設定
}
