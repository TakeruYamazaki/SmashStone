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
#include "kananlibrary.h"
#include "inputGamepad.h"

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
#define CLOCKHANDSUI_WAITTIME 30					// 時計の針UI待機時間
#define CHARATEX_SISE_X 350							// キャラテクスチャ大きさX
#define CHARATEX_SISE_Y 450							// キャラテクスチャ大きさY
#define CHARAUI_POS_Y 600							// 全員キャラUI位置Y
#define CHARAFULLUI_SIZE_X 720						// 全員キャラUI大きさX
#define CHARAFULLUI_SIZE_Y 200						// 全員キャラUI大きさY
#define CHARAFREAMUI_SIZE_X 180						// キャラUI大きさX
#define CHARA1PUI_POS D3DXVECTOR3(200, 230, 0)		// 1PキャラUIの位置
#define CHARA2PUI_POS D3DXVECTOR3(1080, 230, 0)		// 2PキャラUIの位置
#define GEAR_POS_Y 530								// 歯車の位置Y
#define CLOCK_HANDS_DIFF 0.1f						// 回転の初期値
#define READY1PUI_POS D3DXVECTOR3(200, 400, 0)		// 1P準備完了位置
#define READY2PUI_POS D3DXVECTOR3(1080, 400, 0)		// 2P準備完了位置
#define READYUI_SIZE_X 200							// 準備完了大きさX
#define READYUI_SIZE_Y 160							// 準備完了大きさX
#define CHARA_SELECTUI_POS D3DXVECTOR3(640, 60, 0)	// キャラクター選択UI位置
#define CHARA_SELECTUI_SIZE_X 400					// キャラクター選択UI大きさX
#define CHARA_SELECTUI_SIZE_Y 120					// キャラクター選択UI大きさY
#define MAPUI_SIZE_X 300							// マップUI大きさX
#define MAPUI_SIZE_Y 250							// マップUI大きさY
#define MAPUI_POS_X0 650							// マップUI縦の位置0列目
#define MAPUI_POS_X1 1050							// マップUI縦の位置1列目
#define MAPUI_POS_Y0 200							// マップUI横の位置0列目
#define MAPUI_POS_Y1 500							// マップUI横の位置1列目
#define MAPFRAMEUI_SIZE_X 360						// マップ枠線大きさX
#define MAPFRAMEUI_SIZE_Y 290						// マップ枠線大きさY
#define MAPEXPLANATION_POS D3DXVECTOR3(250, SCREEN_HEIGHT / 2, 0)// マップ説明UI位置
#define MAPEXPLANATION_SIZE_X 400					// マップ説明UI大きさX
#define MAPEXPLANATION_SIZE_Y 600					// マップ説明UI大きさY

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
	{ "data/TEXTURE/FULLchara.png" },	// キャラクター全員
	{ "data/TEXTURE/FULLchara.png" },	// 1Pキャラクター
	{ "data/TEXTURE/FULLchara.png" },	// 2Pキャラクター
	{ "data/TEXTURE/1Pchara.png" },		// 1Pキャラクター枠
	{ "data/TEXTURE/2Pchara.png" },		// 2Pキャラクター枠
	{ "data/TEXTURE/Ready.png" },		// 1Pキャラクター準備完了
	{ "data/TEXTURE/Ready.png" },		// 2Pキャラクター準備完了
	{ "data/TEXTURE/PlayerSelect.png" },// プレイヤーセレクトアイコン
	{ "data/TEXTURE/worldMap.jpg" },	// 世界地図
	{ "data/TEXTURE/MapFrame.jpg" },	// マップ選択枠
	{ "data/TEXTURE/map1.jpg" },		// マップ1
	{ "data/TEXTURE/map2.jpg" },		// マップ2
	{ "data/TEXTURE/coming soon.jpg" },	// マップ3(coming soon)
	{ "data/TEXTURE/coming soon.jpg" },	// マップ4(coming soon)
	{ "data/TEXTURE/MapExplanation.jpg" },// マップ説明
	{ "data/TEXTURE/MapExplanation.jpg" },// マップ説明
	{ "data/TEXTURE/MapExplanation.jpg" },// マップ説明
	{ "data/TEXTURE/MapExplanation.jpg" },// マップ説明
	{ "data/TEXTURE/gameBG.png" },		// ゲーム背景
	{ "data/TEXTURE/jewelryBule.png" },	// 宝石青
	{ "data/TEXTURE/jewelryRed.png" },	// 宝石赤
	{ "data/TEXTURE/jewelryYellow.png" },// 宝石黄
	{ "data/TEXTURE/FULLchara.png" },	// 宝石黄
	{ "data/TEXTURE/FULLchara.png" },	// 宝石黄
	{ "data/TEXTURE/charaName0.png" },	// 1Pのキャラクターネーム
	{ "data/TEXTURE/charaName1.png" },	// 2Pのキャラクターネーム
};

int CUI::m_nMapID = 0;	// マップID

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
	TitlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// タイトルの位置
	m_nCntBound = 0;			// タイトルUIバウンドカウンタ
	m_nMode = 0;				// モード番号
	m_nCharaNum[0] = 0;			// キャラ番号
	m_nCharaNum[1] = 3;			// キャラ番号
	m_nCntRot[MAX_PLAYER] = 0;	// 時計の針の回転用カウンタ
	m_nCntWait[MAX_PLAYER] = 0;	// 待機時間用カウンタ
	m_nCntMove[MAX_PLAYER] = 0;	// 移動用カウンタ
	m_nMapID = 0;				// マップ番号
	m_fCntUITitle0 = 0;			// タイトルUI用カウンタ0
	m_fCntUITitle1 = 0;			// タイトルUI用カウンタ1
	m_fCntEnter = 0;			// エンター用カウンタ
	m_fCntUISign = 0;			// 看板用カウンタ
	m_nPlayer = 0;				// プレイヤー番号
	m_fPosMove[MAX_PLAYER] = 0;	// 位置移動変数
	m_fPos[0] = 370;			// 現在の枠線テクスチャの位置X
	m_fPos[1] = 910;			// 現在の枠線テクスチャの位置X
	m_fPosDiff[MAX_PLAYER] = 0;	// 目標の枠線テクスチャの位置X
	m_fPosOld[MAX_PLAYER] = 0;	// 前回の枠線テクスチャの位置X
	m_fRotGear[MAX_PLAYER] = 0;	// 歯車の回転格納変数
	m_fPosCul[MAX_PLAYER] = 0;	// 位置計算用変数
	m_fDiff[MAX_PLAYER] = 0;	// 1フレーム前との距離
	m_fAngle[MAX_PLAYER] = 0;	// 歯車の回転角度
	m_fRad[MAX_PLAYER] = 0;		// ラジアン値
	m_bUITitle0 = false;		// タイトルを動かすかどうか
	m_bUITitle1 = false;		// タイトルを動かすかどうか
	m_bUIEnter = false;			// エンターのα値用変数
	m_bUIClockHands[MAX_PLAYER] = false;// 時計の針が動いたかどうか0
	m_bCharaDecide[MAX_PLAYER] = false;// 自分のキャラクターを選択したかどうか
	m_bStickReturn[MAX_PLAYER] = false;// パッドスティックを戻したかどうか
	m_bMapSelect = false;		// マップを選択したかどうか

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
			if (nCnt > LOGOTYPE_MODEFREAM && nCnt <= LOGOTYPE_SELECTICON)
			{
				// 生成処理
				m_pScene2D[nCnt] = CScene2D::Create();

				// テクスチャを貼る
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
		{// マップ選択画面のとき
			// マップ選択画面で使うUIのとき
			if (nCnt > LOGOTYPE_SELECTICON && nCnt <= LOGOTYPE_MAPEXPLANATION4)
			{
				// 生成処理
				m_pScene2D[nCnt] = CScene2D::Create();

				// テクスチャを貼る
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_GAME)
		{// ゲームのとき
			// ゲームで使うUIのとき
			if (nCnt > LOGOTYPE_MAPEXPLANATION4 && nCnt <= LOGOTYPE_CHARANAME1)
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

	// ゲームパッド変数
	CInputGamepad *pGamepad[MAX_PLAYER];

	// 最大人数までカウント
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// ゲームパッド取得
		pGamepad[nCnt] = CManager::GetInputGamepad(nCnt);
	}

	// UIタイトルの更新処理
	TitleUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);

	// UIチュートリアル更新処理
	TutorialUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);

	// UIマップ選択画面更新処理
	MapSelectUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);

	// UIゲーム更新処理
	GameUpdate();
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
			if (nCnt > LOGOTYPE_MODEFREAM && nCnt <= LOGOTYPE_SELECTICON)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
		{// モードがマップ選択画面のとき
			// マップ選択画面で使うUIのとき
			if (nCnt > LOGOTYPE_SELECTICON && nCnt <= LOGOTYPE_MAPEXPLANATION4)
			{
				// テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_GAME)
		{// モードがゲームのとき
			// ゲームで使うUIのとき
			if (nCnt > LOGOTYPE_MAPEXPLANATION4 && nCnt <= LOGOTYPE_CHARANAME1)
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
			if (nCnt > LOGOTYPE_MODEFREAM && nCnt <= LOGOTYPE_SELECTICON)
			{
				m_pTexture[nCnt]->Release();		// 開放
				m_pTexture[nCnt] = NULL;			// NULLにする
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
		{// モードがマップ選択画面のとき
			// マップ選択画面で使うUIのとき
			if (nCnt > LOGOTYPE_SELECTICON && nCnt <= LOGOTYPE_MAPEXPLANATION4)
			{
				m_pTexture[nCnt]->Release();		// 開放
				m_pTexture[nCnt] = NULL;			// NULLにする
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_GAME)
		{// モードがゲームのとき
			// ゲームで使うUIのとき
			if (nCnt > LOGOTYPE_MAPEXPLANATION4 && nCnt <= LOGOTYPE_CHARANAME1)
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
void CUI::TitleUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1)
{
	// タイトルのとき
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
	{
		// 次のモードにいくかどうか判定取得
		bool bModeNext = CTitle::GetbModeNext();

		if (!bModeNext)
		{
			// タイトルUI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + TitlePos.y, 0.0f), TITLEUI_BEGIN_X + m_fCntUITitle0 - m_fCntUITitle1,
				TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1, LOGOTYPE_TITLE, NORMAL_COLOR);

			// バウンドカウントが規定値以下のとき
			if (m_nCntBound < TITLEUI_BOUND_COUNT)
			{
				// タイトルを動かしていなとき
				if (!m_bUITitle0)
				{
					// タイトルUIの縦の長さが規定値以下のとき
					if (TITLEUI_BEGIN_Y + m_fCntUITitle0 <= TITLEUI_VALUE_Y)
					{
						// タイトルを動かす状態にする
						m_bUITitle0 = true;
					}
					else
					{
						// タイトルカウンタ減算
						m_fCntUITitle0 -= TITLEUI_SMALL_SPEED;
					}
				}
				else
				{// タイトルを動かしていいとき
				 // タイトルUIの縦の長さが[360]以上のとき
					if (TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1 >= TITLEUI_MAXSIZE_VALUE_Y)
					{
						// タイトルを最大まで拡大させた
						m_bUITitle1 = true;

					}
					else if (TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1 <= TITLEUI_MINSIZE_VALUE_Y)
					{// タイトルUIの縦の長さが[330]以下のとき
					 // タイトルを最小まで拡小させた
						m_bUITitle1 = false;
					}

					// タイトルを最大まで拡大させたとき
					if (m_bUITitle1)
					{
						// タイトルカウンタ加算
						m_fCntUITitle1 -= TITLEUI_BOUND_SPEED;
					}
					else
					{// タイトルを最小まで拡小させたとき
					 // タイトルカウンタ減算
						m_fCntUITitle1 += TITLEUI_BOUND_SPEED;
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
						LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f + m_fCntEnter));

					// エンターUIのα値が1.0以上のとき
					if (1.0f + m_fCntEnter >= 1.0f)
					{
						// エンター用α値を減らす状態にする
						m_bUIEnter = true;
					}
					else if (1.0f + m_fCntEnter <= 0.0f)
					{
						// エンター用α値を増やす状態にする
						m_bUIEnter = false;
					}

					// エンターUIを減らす状態のとき
					if (m_bUIEnter)
					{
						// カウンタ減算
						m_fCntEnter -= ENTERUI_ALPHA;
					}
					else
					{// エンターUIを増やす状態のとき
					 // カウンタ加算
						m_fCntEnter += ENTERUI_ALPHA;
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
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, TITLEUI_FINISH_Y + m_fCntUISign / 5, 0.0f), TITLEUI_LAST_X,
				TITLEUI_VALUE_Y, LOGOTYPE_TITLE, NORMAL_COLOR);

			// エンターUI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, ENTERUI_POS_Y, 0.0f), ENTERUI_SIZE_X, ENTERUI_SIZE_Y,
				LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			// 看板UI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SIGNUI_BEGINPOS_Y + m_fCntUISign, 0.0f), SIGNUI_SIZE_X, SIGNUI_SIZE_Y, LOGOTYPE_SIGN, NORMAL_COLOR);

			// モード00UI
			SetUI(D3DXVECTOR3(MODE00UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_fCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODE00, NORMAL_COLOR);
			// モード01UI
			SetUI(D3DXVECTOR3(MODE01UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_fCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODE01, NORMAL_COLOR);

			// 看板UIが規定値より大きい時
			if (SIGNUI_BEGINPOS_Y + m_fCntUISign > SIGNUI_LASTPOS_Y)
			{
				// 看板用カウンタ減算
				m_fCntUISign -= SIGNUI_MOVE_SPEED;
			}

			// モード番号が[0]のとき
			if (m_nMode == 0)
			{
				// モード枠線UI
				SetUI(D3DXVECTOR3(MODE00UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_fCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODEFREAM, NORMAL_COLOR);
			}
			else if (m_nMode == 1)
			{
				// モード枠線UI
				SetUI(D3DXVECTOR3(MODE01UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_fCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODEFREAM, NORMAL_COLOR);
			}

			// ゲームパッド有効時
			if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
				// ゲームパッド操作
				ControlGamepad(pGamepad0, pGamepad1);
			// ゲームパッド無効時
			else
				// キーボード操作
				ControlKeyboard(pKeyboard);
		}
	}
}

//==================================================================================================================
// チュートリアル更新処理
//==================================================================================================================
void CUI::TutorialUpdate(CInputKeyboard * pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1)
{
	// チュートリアルのとき
	if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{
		// キャラクター全員UI
		SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, CHARAUI_POS_Y, 0.0f), CHARAFULLUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_CHARAFULL, NORMAL_COLOR);

		// ゲームパッド有効時
		if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
			// ゲームパッド操作
			ControlGamepad(pGamepad0, pGamepad1);
		// ゲームパッド無効時
		else
			// キーボード操作
			ControlKeyboard(pKeyboard);

		// 時計土台0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCK0, NORMAL_COLOR);
		// 時計の針0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKHANDS0, NORMAL_COLOR);
		// 時計の針回転設定
		m_pScene2D[LOGOTYPE_CLOCKHANDS0]->SetRot(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0), -D3DX_PI / 6 - m_nCntRot[0] * CLOCK_HANDS_DIFF, D3DXVECTOR3(0, 0, 0));
		// 時計の針0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKGEAR0, NORMAL_COLOR);

		// 時計の針が動いていい状態のとき
		if (m_bUIClockHands[0])
		{
			// 時計の針が最大回転値より大きいとき
			if ((-D3DX_PI / 6) * 5 < (-D3DX_PI / 6) - m_nCntRot[0] * CLOCK_HANDS_DIFF)
			{
				// 回転させていく
				m_nCntRot[0]++;
			}
			else
			{
				// 待機時間加算
				m_nCntWait[0]++;

				// 待機時間が0.5秒経ったとき
				if (m_nCntWait[0] >= CLOCKHANDSUI_WAITTIME)
				{
					m_bUIClockHands[0] = false;
				}
			}
		}
		else
		{
			// 待機時間用カウンタ初期化
			m_nCntWait[0] = 0;

			// 回転させていく
			m_nCntRot[0]--;

			// 回転用カウンタが[0]以下になったとき
			if (m_nCntRot[0] <= 0)
			{
				// 回転用カウンタ初期化
				m_nCntRot[0] = 0;
			}
		}

		// 時計土台1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCK1, NORMAL_COLOR);
		// 時計の針1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKHANDS1, NORMAL_COLOR);
		m_pScene2D[LOGOTYPE_CLOCKHANDS1]->SetRot(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0), D3DX_PI / 6 + m_nCntRot[1] * CLOCK_HANDS_DIFF, D3DXVECTOR3(0, 0, 0));
		// 時計の針1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKGEAR1, NORMAL_COLOR);

		// 時計の針が動いていい状態のとき
		if (m_bUIClockHands[1])
		{
			// 時計の針が最大回転値より大きいとき
			if ((D3DX_PI / 6) * 5 > (D3DX_PI / 6) + m_nCntRot[1] * CLOCK_HANDS_DIFF)
			{
				// 回転させていく
				m_nCntRot[1]++;
			}
			else
			{
				// 待機時間加算
				m_nCntWait[1]++;

				// 待機時間が0.5秒経ったとき
				if (m_nCntWait[1] >= CLOCKHANDSUI_WAITTIME)
				{
					m_bUIClockHands[1] = false;
				}
			}
		}
		else
		{
			// 待機時間用カウンタ初期化
			m_nCntWait[1] = 0;

			// 回転させていく
			m_nCntRot[1]--;

			// 回転用カウンタが[0]以下になったとき
			if (m_nCntRot[1] <= 0)
			{
				// 回転用カウンタ初期化
				m_nCntRot[1] = 0;
			}
		}

		// 最大人数までカウント
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// 現在の位置を前回の位置に代入
			m_fPosOld[nCnt] = m_fPos[nCnt];

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
			// 目標位置格納変数
			m_fPosDiff[nCnt] = 370 + (float)m_nCharaNum[nCnt] * CHARAFREAMUI_SIZE_X;
		}

		// 最大人数までカウント
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			float fRadius = CHARAFULLUI_SIZE_Y / 2;											// 歯車テクスチャの半径
			m_fPosCul[nCnt] = ((m_fPosDiff[nCnt] - m_fPos[nCnt]) / 60) * m_fPosMove[nCnt];	// 計算用変数
			m_fDiff[nCnt] = fabsf(m_fPos[nCnt] + m_fPosCul[nCnt] - m_fPosOld[nCnt]);		// 1フレーム後の距離
			m_fAngle[nCnt] = 180 * m_fDiff[nCnt] / (D3DX_PI * fRadius);						// 角度算出
			m_fRad[nCnt] = D3DX_PI * m_fAngle[nCnt] / 360;					// ラジアン値

			// 位置の差が正のとき
			if (m_fPosCul[nCnt] > 0)
			{
				m_fRotGear[nCnt] = m_fRotGear[nCnt] + m_fRad[nCnt];					// 角度格納(加算)
			}
			else if (m_fPosCul[nCnt] < 0)
			{// 位置の差が負のとき
				m_fRotGear[nCnt] = m_fRotGear[nCnt] - m_fRad[nCnt];					// 角度格納(減算)
			}
		}

		// 1Pキャラクター枠線UI
		SetUI(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_1PCHARA_FREAM, NORMAL_COLOR);
		// 歯車0UI
		SetUI(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], GEAR_POS_Y, 0.0f), CHARAFULLUI_SIZE_Y, CHARAFULLUI_SIZE_Y, LOGOTYPE_GEAR0, NORMAL_COLOR);
		// 回転設定
		m_pScene2D[LOGOTYPE_GEAR0]->SetRot(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], GEAR_POS_Y, 0), m_fRotGear[0], D3DXVECTOR3(0, 0, 0));

		// 2Pキャラクター枠線UI
		SetUI(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_2PCHARA_FREAM, NORMAL_COLOR);
		// 歯車1UI
		SetUI(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], GEAR_POS_Y, 0.0f), CHARAFULLUI_SIZE_Y, CHARAFULLUI_SIZE_Y, LOGOTYPE_GEAR1, NORMAL_COLOR);
		// 回転設定
		m_pScene2D[LOGOTYPE_GEAR1]->SetRot(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], GEAR_POS_Y, 0), m_fRotGear[1], D3DXVECTOR3(0, 0, 0));

		// 最大人数までカウント
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// 位置が同じじゃないとき
			if (m_fPosDiff[nCnt] != m_fPos[nCnt])
			{
				// 位置移動用カウンタ加算
				m_fPosMove[nCnt]++;
			}

			// 現在の位置設定
			m_fPos[nCnt] = m_fPos[nCnt] + m_fPosCul[nCnt];

			// 現在の位置を前回の位置に代入
			m_fPosOld[nCnt] = m_fPos[nCnt] + m_fPosCul[nCnt];

		}
		// 1PキャラクターUI
		SetUI(CHARA1PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, LOGOTYPE_1PCHARA, NORMAL_COLOR);
		// テクスチャ設定
		m_pScene2D[LOGOTYPE_1PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0]);
		// 2PキャラクターUI
		SetUI(CHARA2PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, LOGOTYPE_2PCHARA, NORMAL_COLOR);
		// テクスチャ設定
		m_pScene2D[LOGOTYPE_2PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1]);

		// 1Pキャラクター選択されているとき
		if (m_bCharaDecide[0])
		{
			// 1Pキャラクター準備完了
			SetUI(READY1PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, LOGOTYPE_1PREADY, NORMAL_COLOR);
		}
		else
		{// 1Pキャラクター選択されていないとき
			// 1Pキャラクター準備完了
			SetUI(READY1PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, LOGOTYPE_1PREADY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// 2Pキャラクター選択されているとき
		if (m_bCharaDecide[1])
		{
			// 2PキャラクターUI
			SetUI(READY2PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, LOGOTYPE_2PREADY, NORMAL_COLOR);
		}
		else
		{// 2Pキャラクター選択されていないとき
			// 2Pキャラクター準備完了
			SetUI(READY2PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, LOGOTYPE_2PREADY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// キャラクター選択アイコン
		SetUI(CHARA_SELECTUI_POS, CHARA_SELECTUI_SIZE_X, CHARA_SELECTUI_SIZE_Y, LOGOTYPE_SELECTICON, NORMAL_COLOR);
	}
}

//==================================================================================================================
// マップ選択画面の更新処理
//==================================================================================================================
void CUI::MapSelectUpdate(CInputKeyboard * pKeyboard, CInputGamepad * pGamepad0, CInputGamepad * pGamepad1)
{
	// マップ選択画面のとき
	if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
	{
		// 世界地図
		SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, LOGOTYPE_WORLDMAP, NORMAL_COLOR);

		// マップ1
		SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP1, NORMAL_COLOR);

		// マップ2
		SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP2, NORMAL_COLOR);

		// マップ3
		SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y1, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP3, NORMAL_COLOR);

		// マップ4
		SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y1, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP4, NORMAL_COLOR);

		// マップIDが0より小さい時
		if (m_nMapID < 0)
		{
			// 0にする
			m_nMapID = 0;
		}

		// まっぷIDが3より大きい時
		if (m_nMapID > 3)
		{
			// 3にする
			m_nMapID = 3;
		}

		switch (m_nMapID)
		{
		case 0:
			// マップ枠線
			SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, NORMAL_COLOR);
			// マップ説明
			SetUI(MAPEXPLANATION_POS, MAPEXPLANATION_SIZE_X, MAPEXPLANATION_SIZE_Y, LOGOTYPE_MAPEXPLANATION1, NORMAL_COLOR);
			break;
		case 1:
			// マップ枠線
			SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, NORMAL_COLOR);
			// マップ説明
			SetUI(MAPEXPLANATION_POS, MAPEXPLANATION_SIZE_X, MAPEXPLANATION_SIZE_Y, LOGOTYPE_MAPEXPLANATION2, NORMAL_COLOR);
			break;
		case 2:
			// マップ枠線
			SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y1, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, NORMAL_COLOR);
			// マップ説明
			SetUI(MAPEXPLANATION_POS, MAPEXPLANATION_SIZE_X, MAPEXPLANATION_SIZE_Y, LOGOTYPE_MAPEXPLANATION3, NORMAL_COLOR);
			break;
		case 3:
			// マップ枠線
			SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y1, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, NORMAL_COLOR);
			// マップ説明
			SetUI(MAPEXPLANATION_POS, MAPEXPLANATION_SIZE_X, MAPEXPLANATION_SIZE_Y, LOGOTYPE_MAPEXPLANATION4, NORMAL_COLOR);
			break;
		}

		// ゲームパッド有効時
		if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
			// ゲームパッド操作
			ControlGamepad(pGamepad0, pGamepad1);
		// ゲームパッド無効時
		else
			// キーボード操作
			ControlKeyboard(pKeyboard);
	}
}

//==================================================================================================================
// ゲーム更新処理
//==================================================================================================================
void CUI::GameUpdate(void)
{
	// ゲームのとき
	if (CRenderer::GetMode() == CRenderer::MODE_GAME)
	{
		CPlayer *pPlayer0, *pPlayer1;

		// プレイヤー情報取得
		pPlayer0 = CGame::GetPlayer(0);
		pPlayer1 = CGame::GetPlayer(1);

		// プレイヤーが2体いるとき
		if (pPlayer0 != NULL && pPlayer1 != NULL)
		{
			// プレイヤー番号取得
			m_nCharaNum[0] = pPlayer0->GetCharaType();
			m_nCharaNum[1] = pPlayer1->GetCharaType();
		}

		// ゲーム背景
		SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, LOGOTYPE_GAMEBG, NORMAL_COLOR);

		// 1PキャラクターUI
		SetUI(D3DXVECTOR3(55, 70, 0), 110, 130, LOGOTYPE_PLAYER1, NORMAL_COLOR);
		// テクスチャ設定
		m_pScene2D[LOGOTYPE_PLAYER1]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0] / 2);
		// 2PキャラクターUI
		SetUI(D3DXVECTOR3(1225, 70, 0), 110, 130, LOGOTYPE_PLAYER2, NORMAL_COLOR);
		// テクスチャ設定
		m_pScene2D[LOGOTYPE_PLAYER2]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1] / 2);

		// 1PキャラクターネームUI
		SetUI(D3DXVECTOR3(205, 155, 0), 400, 50, LOGOTYPE_CHARANAME0, NORMAL_COLOR);
		// テクスチャ設定
		m_pScene2D[LOGOTYPE_CHARANAME0]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[0] / 2) * 0.25f, 0.0f + (m_nCharaNum[0] / 2) * 0.25f, 0);
		// 2PキャラクターネームUI
		SetUI(D3DXVECTOR3(1075, 155, 0), 400, 50, LOGOTYPE_CHARANAME1, NORMAL_COLOR);
		// テクスチャ設定
		m_pScene2D[LOGOTYPE_CHARANAME1]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[1] / 2) * 0.25f, 0.0f + (m_nCharaNum[1] / 2) * 0.25f, 0);
	}
}

//==================================================================================================================
// ゲームパッド操作
//==================================================================================================================
void CUI::ControlGamepad(CInputGamepad * pGamepad0, CInputGamepad *pGamepad1)
{
	float fValueX0, fValueY0 = 0;	// ゲームパッドのスティック情報の取得用
	float fValueX1, fValueY1 = 0;	// ゲームパッドのスティック情報の取得用

	// 左スティック取得
	pGamepad0->GetStickLeft(&fValueX0, &fValueY0);
	pGamepad1->GetStickLeft(&fValueX1, &fValueY1);

	//// 何も入力されていなければ、処理しない
	//if (FAILED(CKananLibrary::GetMoveByGamepad(pGamepad0)) &&
	//	fValueX0 == 0 && fValueY0 == 0 &&
	//	FAILED(CKananLibrary::GetMoveByGamepad(pGamepad1)) &&
	//	fValueX1 == 0 && fValueY1 == 0)
	//{
	//	return;
	//}

	// タイトルのとき
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
	{
		// 左に傾けたとき
		if (fValueX0 < 0 || fValueX1 < 0)
		{
			// モード0
			m_nMode = 0;
		}
		else if (fValueX0 > 0 || fValueX1 > 0)
		{// 右に傾けたとき
			// モード1
			m_nMode = 1;
		}
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{// チュートリアルのとき
		// 自分のキャラクターを選択してないとき
		if (!m_bCharaDecide[0])
		{
			// スティックを戻しているとき
			if (m_bStickReturn[0])
			{
				// 左に傾けたとき
				if (fValueX0 < 0)
				{
					// キャラ番号減算
					m_nCharaNum[0] -= 1;

					// 時計の針が動いていい状態にする
					m_bUIClockHands[0] = true;

					// 位置移動用カウンタ初期化
					m_fPosMove[0] = 0;

					// スティックを戻したかどうか
					m_bStickReturn[0] = false;
				}
				else if (fValueX0 > 0)
				{// 右に傾けたとき
					// キャラ番号加算
					m_nCharaNum[0] += 1;

					// 時計の針が動いていい状態にする
					m_bUIClockHands[0] = true;

					// 位置移動用カウンタ初期化
					m_fPosMove[0] = 0;

					// スティックを戻したかどうか
					m_bStickReturn[0] = false;
				}

				// 決定ボタンを押したとき
				if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_B))
				{
					// キャラクターを選択した状態にする
					m_bCharaDecide[0] = true;
				}
			}

			// スティック角度0のとき
			if (fValueX0 == 0)
			{
				// スティックを戻している状態にする
				m_bStickReturn[0] = true;
			}
		}
		else
		{// 1Pが自分のキャラクターが選択されているとき
			// 戻るボタンを押したとき
			if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_A))
			{
				// キャラクターを選択していない状態にする
				m_bCharaDecide[0] = false;
			}
		}


		// 2Pのキャラクターが選ばれていないとき
		if (!m_bCharaDecide[1])
		{
			// スティックを戻しているとき
			if (m_bStickReturn[1])
			{
				// 左に傾けたとき
				if (fValueX1 < 0)
				{
					// キャラ番号減算
					m_nCharaNum[1] -= 1;

					// 時計の針が動いていい状態にする
					m_bUIClockHands[1] = true;

					// 位置移動用カウンタ初期化
					m_fPosMove[1] = 0;

					// スティックが戻っていない状態にする
					m_bStickReturn[1] = false;
				}
				else if (fValueX1 > 0)
				{
					// キャラ番号加算
					m_nCharaNum[1] += 1;

					// 時計の針が動いていい状態にする
					m_bUIClockHands[1] = true;

					// 位置移動用カウンタ初期化
					m_fPosMove[1] = 0;

					// スティックが戻っていない状態にする
					m_bStickReturn[1] = false;
				}

				// 決定ボタンを押したとき
				if (pGamepad1->GetTrigger(CInputGamepad::JOYPADKEY_B))
				{
					// キャラクターを選択した状態にする
					m_bCharaDecide[1] = true;
				}
			}

			// スティックが傾いていないとき
			if (fValueX1 == 0)
			{
				// スティックが戻っている状態にする
				m_bStickReturn[1] = true;
			}
		}
		else
		{// 2Pが自分のキャラクターを選択しているとき
			// 戻るボタンを押したとき
			if (pGamepad1->GetTrigger(CInputGamepad::JOYPADKEY_A))
			{
				// キャラクターを選択していない状態にする
				m_bCharaDecide[1] = false;
			}
		}
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
	{// マップ選択画面のとき
		// マップが選択されていないとき
		if (!m_bMapSelect)
		{
			// 左に傾けたとき
			if (fValueX0 < 0)
			{
				// マップ番号1減算
				m_nMapID -= 1;
			}
			else if (fValueX0 > 0)
			{// 右に傾けたとき
				// マップ番号1加算
				m_nMode += 1;
			}

			// 上に傾けたとき
			if (fValueY0 < 0)
			{
				// マップ番号-2
				m_nMapID -= 2;
			}
			else if (fValueY0 > 0)
			{// 下に傾けたとき
				// マップ番号+2
				m_nMapID += 2;
			}

			// 決定ボタンを押したとき
			if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_B))
			{
				// マップを選択した状態にする
				m_bMapSelect = true;
			}
		}
	}
}
//==================================================================================================================
// キーボード処理
//==================================================================================================================
void CUI::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// タイトルのとき
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
	{
		// キーボードの[→]を押したとき
		if (pKeyboard->GetKeyboardTrigger(DIK_RIGHTARROW))
		{
			m_nMode = 1;
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_LEFTARROW))
		{// キーボードの[←]を押したとき
			m_nMode = 0;
		}

		// キーボードの[D]を押したとき
		if (pKeyboard->GetKeyboardTrigger(DIK_D))
		{
			m_nMode = 1;
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_A))
		{// キーボードの[A]を押したとき
			m_nMode = 0;
		}
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{// チュートリアルのとき

		// 自分のキャラクターを選択してないとき
		if (!m_bCharaDecide[0])
		{
			// キーボードの[A]が押されたとき
			if (pKeyboard->GetKeyboardTrigger(ONE_LEFT))
			{
				// キャラ番号減算
				m_nCharaNum[0] -= 1;

				// 時計の針が動いていい状態にする
				m_bUIClockHands[0] = true;

				// 位置移動用カウンタ初期化
				m_fPosMove[0] = 0;
			}
			else if (pKeyboard->GetKeyboardTrigger(ONE_RIGHT))
			{// キーボードの[D]が押されたとき
			 // キャラ番号加算
				m_nCharaNum[0] += 1;

				// 時計の針が動いていい状態にする
				m_bUIClockHands[0] = true;

				// 位置移動用カウンタ初期化
				m_fPosMove[0] = 0;
			}

			// 1Pが決定ボタンをおしたとき
			if (pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
			{
				// キャラクターを選択した状態にする
				m_bCharaDecide[0] = true;
			}
		}
		else
		{// 1Pが自分のキャラクターを選択しているとき
			// 1Pが決定ボタンをおしたとき
			if (pKeyboard->GetKeyboardTrigger(ONE_JUMP))
			{
				// キャラクターを選択していない状態にする
				m_bCharaDecide[0] = false;
			}
		}

		// 2Pのキャラクターが選ばれていないとき
		if (!m_bCharaDecide[1])
		{
			// キーボードの[←]が押されたとき
			if (pKeyboard->GetKeyboardTrigger(TWO_LEFT))
			{
				// キャラ番号減算
				m_nCharaNum[1] -= 1;

				// 時計の針が動いていい状態にする
				m_bUIClockHands[1] = true;

				// 位置移動用カウンタ初期化
				m_fPosMove[1] = 0;
			}
			else if (pKeyboard->GetKeyboardTrigger(TWO_RIGHT))
			{// キーボードの[→]が押されたとき
			 // キャラ番号加算
				m_nCharaNum[1] += 1;

				// 時計の針が動いていい状態にする
				m_bUIClockHands[1] = true;

				// 位置移動用カウンタ初期化
				m_fPosMove[1] = 0;
			}

			// 2Pが決定ボタンをおしたとき
			if (pKeyboard->GetKeyboardTrigger(TWO_ATTACK))
			{
				// キャラクターを選んだ状態にする
				m_bCharaDecide[1] = true;
			}
		}
		else
		{// 2Pが自分のキャラクターを選択しているとき
			// 2Pが決定ボタンをおしたとき
			if (pKeyboard->GetKeyboardTrigger(TWO_JUMP))
			{
				// キャラクターを選択していない状態にする
				m_bCharaDecide[1] = false;
			}
		}
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
	{// マップ選択画面のとき

		// マップが選択されていないとき
		if (!m_bMapSelect)
		{
			// 1Pが左入力をしたとき
			if (pKeyboard->GetKeyboardTrigger(ONE_LEFT))
			{
				// マップID小さくなる
				m_nMapID -= 1;
			}
			else if (pKeyboard->GetKeyboardTrigger(ONE_RIGHT))
			{// 1Pが右入力をしたとき
				// マップID大きくなる
				m_nMapID += 1;
			}
			else if (pKeyboard->GetKeyboardTrigger(ONE_UP))
			{// 1Pが上入力をしたとき
				// マップID-2する
				m_nMapID -= 2;
			}
			else if (pKeyboard->GetKeyboardTrigger(ONE_DOWN))
			{// 1Pがした入力をしたとき
				// まっぷID+2する
				m_nMapID += 2;
			}

			// 1Pが決定ボタンを押したとき
			if (pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
			{
				// マップを選んだ状態にする
				m_bMapSelect = true;
			}
		}
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
