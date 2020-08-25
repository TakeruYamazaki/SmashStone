//==================================================================================================================
//
// Gameの描画 [game.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================

//==================================================================================================================
//	インクルードファイル
//==================================================================================================================
#include "game.h"
#include "number.h"
#include "fade.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "scene3D.h"
#include "camera.h"
#include "light.h"
#include "time.h"
#include "renderer.h"
#include "motionModel.h"
#include "pause.h"
#include "inputGamepad.h"
#include "meshSphere.h"
#include "player.h"
#include "modelCharacter.h"
#include "stone.h"
#include "3DBoxCollider.h"
#include "hitpoint.h"
#include "bar.h"
#include "wall.h"
#include "debugProc.h"
#include "objManager.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include "UI_KO.h"
#include "UI_gameStart.h"
#include "UI_gameResult.h"
#include "UI.h"

//==================================================================================================================
//	マクロ定義
//==================================================================================================================
#define MAX_ROUND			(3)										// 最大ラウンド数
#define PLAYER_START_POS_X 390.0f									// プレイヤーの初期位置X
#define PLAYER_START_POS_Z -585.0f									// プレイヤーの初期位置Z
#define RESPAWN_SIZE 0.000001f										// リスポーンモデルの大きさ

#define TIME_CREATE_STONE	(5 * ONE_SECOND_FPS)					// ストーンを生成する時間
#define TIME_KO_AFTER		(60)									// KOの後の時間

#define TIME_FADE_NEXTROUND	(10)									// ラウンド切り替えのフェード時間

#define DEFAULTPOS_1P		(D3DXVECTOR3(-160.0f, 0.0f, 100.0f))	// 1Pプレイヤーの初期座標
#define DEFAULTPOS_2P		(D3DXVECTOR3(20.0f, 0.0f, -130.0f))		// 2Pプレイヤーの初期座標

//==================================================================================================================
//	静的メンバ変数宣言
//==================================================================================================================
CPlayer				*CGame::m_pPlayer[MAX_PLAYER]	= {};							// キャラクター情報
CMeshField			*CGame::m_pMeshField			= NULL;							// メッシュフィールド情報
CCamera				*CGame::m_pCamera				= NULL;							// カメラ情報
CLight				*CGame::m_pLight				= NULL;							// ライト情報
CLogo				*CGame::m_pLogo					= NULL;							// ロゴ情報
CPause				*CGame::m_pPause				= NULL;							// ポーズ情報
CMeshSphere			*CGame::m_pMeshSphere			= NULL;							// メッシュ球の情報
CTime				*CGame::m_pTime					= NULL;							// タイム情報
CWall				*CGame::m_pWall					= NULL;							// 壁のポインタ
CUI					*CGame::m_pUI					= NULL;							// UIポインタ
CUIKO				*CGame::m_pUIKO					= nullptr;						// KOのポインタ
CUI_GameStart		*CGame::m_pUIGameStart			= nullptr;						// ゲーム開始時のUIのポインタ
CUI_GameResult		*CGame::m_pUIGameResult			= nullptr;						// ゲームリザルトのUIのポインタ
CGame::GAMESTATE	CGame::m_gameState				= CGame::GAMESTATE_NONE;		// ゲーム状態
int					CGame::m_nCounterGameState		= NULL;							// ゲームの状態管理カウンター
int					CGame::m_nNumStone				= 0;							// 生成したストーンの数
int					CGame::m_nCntDecide				= 0;							// ストーン生成のタイミングを決めるカウンタ
int					CGame::m_nRound					= 0;							// ラウンド数
int					CGame::m_nRoundAll				= 0;							// 全ラウンド数
NUM_PLAYER			CGame::m_winPlayer				= NUM_PLAYER::PLAYER_NONE;		// 勝利したプレイヤー
NUM_PLAYER			CGame::m_losePlayer				= NUM_PLAYER::PLAYER_NONE;		// 負けたプレイヤー
CObjectManager		*CGame::m_pObjMana				= nullptr;						// オブジェクトマネージャーのポインタ
bool				CGame::m_bSetPos[STONE_POS]		= {};							// ストーンの生成場所に生成されているか
D3DXVECTOR3			CGame::m_stonePos[STONE_POS] = 									// ストーンの生成場所
{
	D3DXVECTOR3(0.0f, 20.0f, 0.0f),
	D3DXVECTOR3(100.0f, 20.0f, 100.0f),
	D3DXVECTOR3(100.0f, 20.0f, -100.0f),
	D3DXVECTOR3(200.0f, 90.0f, 155.0f),
	D3DXVECTOR3(-100.0f, 20.0f, -100.0f)
};

int					CGame::m_nPlayerType[MAX_PLAYER] = {};	// キャラクターセレクト時のタイプを保存

CPolygonCollider* CGame::m_pPolyColli[POLYCOLLI_USE_TYPE] = { NULL };							// ポリゴンコライダーのポインタ

//==================================================================================================================
//	コンストラクタ
//==================================================================================================================
CGame::CGame()
{

}

//==================================================================================================================
//	デストラクタ
//==================================================================================================================
CGame::~CGame()
{

}

//==================================================================================================================
//	初期化処理
//==================================================================================================================
void CGame::Init(void)
{
	/* ロード */
	CNumber::Load();						// 数字テクスチャロード
	CMeshField::Load();						// 床テクスチャロード
	CMotionModel::Load();					// モーション用モデルロード
	CPause::Load();							// ポーズテクスチャロード
	CMeshSphere::Load();					// メッシュ球のテクスチャロード
	CStone::Load();							// ストーンの読み込み
	CBar::Load();							// Barテクスチャロード
	C3DBoxCollider::Load();					// 3Dボックスコライダーの読み込み
	CWall::Load();							// 壁のロード
	CObjectManager::Load();					// オブジェクトマネージャーのロード
	CCapsuleCollider::Load();				// カプセルコライダーのロード
	CUIKO::Load();							// KOのロード
	CUI_GameStart::Load();					// 開始UIのロード
	CUI_GameResult::Load();
	CUI::Load();							// UIロード

	/* 生成 */
	C3DBoxCollider::Create();										// ボックスコライダーの生成
	m_pObjMana    = CObjectManager::Create();						// オブジェクトマネージャーの生成
	m_pWall       = CWall::Create(CWall::WALLTEX_FIELD);			// 壁の生成
	m_pCamera     = CCamera::Create();								// カメラの生成処理
	m_pLight      = CLight::Create();								// ライトの生成処理
	m_pMeshSphere = CMeshSphere::Create();							// メッシュ球の生成処理
	m_pPlayer[PLAYER_ONE] = CPlayer::Create(PLAYER_ONE, (CHARACTER_TYPE)m_nPlayerType[PLAYER_ONE]);	// プレイヤー生成
	m_pPlayer[PLAYER_TWO] = CPlayer::Create(PLAYER_TWO, (CHARACTER_TYPE)m_nPlayerType[PLAYER_TWO]);	// プレイヤー生成
	m_pPlayer[PLAYER_ONE]->SetPos(DEFAULTPOS_1P);
	m_pPlayer[PLAYER_TWO]->SetPos(DEFAULTPOS_2P);

	m_pMeshField  = CMeshField::Create(INTEGER2(4, 4), D3DXVECTOR3(600.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, -40.0f, 50.0f));// メッシュフィールド生成
	m_pUI         = CUI::Create();									// UIの生成処理
	m_pTime       = CTime::Create();								// タイム生成
	m_pPause      = CPause::Create();								// ポーズの生成処理

	// 緩やかな階段
	m_pPolyColli[CPolygonCollider::POLYCOLLI_LONGSTAIRS] = CPolygonCollider::Create(CPolygonCollider::POLYCOLLI_LONGSTAIRS);
	// 階段
	m_pPolyColli[CPolygonCollider::POLYCOLLI_STAIRS] = CPolygonCollider::Create(CPolygonCollider::POLYCOLLI_STAIRS);
	m_pPolyColli[CPolygonCollider::POLYCOLLI_STAIRS_TOP] = CPolygonCollider::Create(CPolygonCollider::POLYCOLLI_STAIRS_TOP);

	/* ゲームの初期化 */
	m_gameState = GAMESTATE_BEFORE;
	m_nCounterGameState = 0;				// ゲームの状態管理カウンターを0にする
	m_nNumStone			= 0;				// 値を初期化
	m_nCntDecide		= 0;				// 値を初期化
	m_nCntAny			= 0;
	m_nRound			= 0;
	m_nRoundAll			= MAX_ROUND;
	m_roundPoint		= INTEGER2(0, 0);

	for (int nCnt = 0; nCnt < STONE_POS; nCnt++)
	{
		m_bSetPos[nCnt] = false;
	}
}

//==================================================================================================================
//	終了処理
//==================================================================================================================
void CGame::Uninit(void)
{
	// ゲーム状態を通常にする
	m_gameState = GAMESTATE_NORMAL;

	// 全ての開放
	CScene2D::ReleaseAll();				// 2Dのもの全て破棄
	CScene3D::ReleaseAll();				// 3Dのもの全て破棄

	/* アンロード */
	CCapsuleCollider::Unload();
	CWall::Unload();					// 壁の開放
	C3DBoxCollider::Unload();			// 3Dボックスコライダーの開放
	CStone::Unload();					// ストーンの開放
	CNumber::Unload();					// 数字テクスチャアンロード
	CMeshField::Unload();				// 床テクスチャアンロード
	CMotionModel::Unload();				// モーション用モデルアンロード
	CBar::Unload();						// Barテクスチャアンロード
	CObjectManager::Unload();			// オブジェクトマネージャーのアンロード
	CUIKO::Unload();					// KOのアンロード
	CUI_GameStart::Unload();			// 開始時のUIのアンロード
	CUI_GameResult::Unload();
	CUI::Unload();						// UIアンロード

	// 万が一残っていた場合
	if (m_pUIGameStart)
	{
		// 破棄
		m_pUIGameStart->Uninit();
		delete m_pUIGameStart;
		m_pUIGameStart = nullptr;
	}

	// 万が一残っていた場合
	if (m_pUIGameResult)
	{
		// 破棄
		m_pUIGameResult->Uninit();
		delete m_pUIGameResult;
		m_pUIGameResult = nullptr;
	}

	// 万が一残っていた場合
	if (m_pUIKO)
	{
		// 破棄
		m_pUIKO->Uninit();
		delete m_pUIKO;
		m_pUIKO = nullptr;
	}

	// ポーズの終了処理
	m_pPause->Uninit();

	m_pObjMana = nullptr;				// ポインタNULL

	delete m_pPause;					// メモリ削除
	m_pPause = nullptr;					// ポインタNULL

	delete m_pCamera;					// メモリ削除
	m_pCamera = nullptr;				// ポインタNULL

	delete m_pLight;					// メモリ削除
	m_pLight = nullptr;					// ポインタNULL

	m_pTime = nullptr;					// ポインタNULL
}

//==================================================================================================================
//	更新処理
//==================================================================================================================
void CGame::Update(void)
{
	// 開始前
	if (m_gameState == GAMESTATE_BEFORE)
		GameBefore();
	// 通常のとき
	if (m_gameState == GAMESTATE_NORMAL)
		GameNormal();
	// ゲーム状態がポーズのとき
	else if (m_gameState == GAMESTATE_PAUSE)
		GamePause();
	// KOのとき
	else if (m_gameState == GAMESTATE_KO)
		GameKO();
	// KOの後
	else if (m_gameState == GAMESTATE_KO_AFTER)
		GameKOAfter();
	// 次のラウンド
	else if (m_gameState == GAMESTATE_NEXTROUND)
		NextRound();
	// リザルト
	else if (m_gameState == GAMESTATE_RESULT)
		GameResult();

#ifdef _DEBUG
	// キーボードの[0]を押したとき
	if (m_gameState != GAMESTATE_RESULT && CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RETURN))
	{
		// フェード取得
		CFade::FADE fade = CFade::GetFade();

		// フェードが何もない時
		if (fade == CFade::FADE_NONE)
			// フェードを設定する
			CFade::SetFade(CRenderer::MODE_TITLE, DEFAULT_FADE_TIME);
	}
#endif // _DEBUG
}

//==================================================================================================================
//	描画処理
//==================================================================================================================
void CGame::Draw(void)
{
	// カメラの設定
	m_pCamera->SetCamera();

	// カメラの描画処理
	m_pCamera->Draw();

	// ポーズ状態がtrueのとき
	if (m_pPause->GetPause() == true)
		// ポーズの更新処理
		m_pPause->Draw();

	// ゲーム開始時のUIの描画
	if (m_pUIGameStart)
		m_pUIGameStart->Draw();

	// KOの描画
	if (m_pUIKO)
		m_pUIKO->Draw();

	// リザルトのUIの描画
	if (m_pUIGameResult)
		m_pUIGameResult->Draw();
}

//==================================================================================================================
//	生成処理
//==================================================================================================================
CGame * CGame::Create(void)
{
	// メモリ確保
	CGame *pGame = new CGame;
	// 初期化
	pGame->Init();
	// 値を返す
	return pGame;
}

//==================================================================================================================
//	次のモードへ移行
//==================================================================================================================
void CGame::SetNextMode(const int nextMode)
{
	// nullcheck
	if (m_pUIGameResult)
	{
		// 破棄
		m_pUIGameResult->Uninit();
		delete m_pUIGameResult;
		m_pUIGameResult = nullptr;
	}

	m_gameState = GAMESTATE_END;

	// フェード取得
	CFade::FADE fade = CFade::GetFade();

	// フェードが何もない時
	if (fade == CFade::FADE_NONE)
		// フェードを設定する
		CFade::SetFade((CRenderer::MODE)nextMode, DEFAULT_FADE_TIME);
}

//==================================================================================================================
//	どこからでも呼び出せるストーン生成処理
//==================================================================================================================
void CGame::AppearStone(void)
{
	// ランダムでポイントを決める
	int RandValue = DecideRandomPos();
	// 決められた位置からランダムで生成
	CStone::Create(RandValue, CStone::STONE_ID_DEFAULT, m_stonePos[RandValue]);
	// 生成された
	m_bSetPos[RandValue] = true;
	// 出現数を加算
	m_nNumStone++;
}

//==================================================================================================================
//	ゲームの前の更新
//==================================================================================================================
void CGame::GameBefore(void)
{
	if (!m_pUIGameStart)
		m_pUIGameStart = CUI_GameStart::Create();

	// カメラの更新処理
	m_pCamera->Update();

	// ライトの更新処理
	m_pLight->Update();

	if (m_pUIGameStart)
		m_pUIGameStart->Update();
}

//==================================================================================================================
//	通常の更新
//==================================================================================================================
void CGame::GameNormal(void)
{
	// nullcheck
	if (m_pUIGameStart)
	{
		// 破棄
		m_pUIGameStart->Uninit();
		delete m_pUIGameStart;
		m_pUIGameStart = nullptr;
	}

	// カメラの更新処理
	m_pCamera->Update();

	// ライトの更新処理
	m_pLight->Update();

	// ストーンを生成するか決める
	DecideCreateStone();

	// どちらかのプレイヤーのライフが0
	if (GetPlayer(PLAYER_ONE)->GetLife() <= 0 ||
		GetPlayer(PLAYER_TWO)->GetLife() <= 0)
		// KO
		m_gameState = GAMESTATE_KO;

	// ポーズの切り替え
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_P))
		SwitchPause();
}

//==================================================================================================================
//	ポーズの更新
//==================================================================================================================
void CGame::GamePause(void)
{
	// ポーズの更新処理
	m_pPause->Update();

	// リトライ
	if (m_gameState == GAMESTATE_START_OVER)
		CFade::SetFade(CRenderer::MODE_GAME, DEFAULT_FADE_TIME);
	// タイトルに戻る
	else if (m_gameState == GAMESTATE_BREAK)
		CFade::SetFade(CRenderer::MODE_TITLE, DEFAULT_FADE_TIME);
}

//==================================================================================================================
//	KOの更新
//==================================================================================================================
void CGame::GameKO(void)
{
	// KOのUIを生成
	if (!m_pUIKO)
		m_pUIKO = CUIKO::Create();

	// KOの更新
	if (m_pUIKO)
		m_pUIKO->Update();
}

//==================================================================================================================
//	KOの後の更新
//==================================================================================================================
void CGame::GameKOAfter(void)
{
	// カウンタの加算
	m_nCntAny++;
	// 一定時間で次へ
	if (m_nCntAny >= TIME_KO_AFTER &&
		!m_pPlayer[PLAYER_ONE]->GetbJump() && !m_pPlayer[PLAYER_TWO]->GetbJump())
	{
		m_nCntAny = 0;
		m_gameState = GAMESTATE_NEXTROUND;
		return;
	}

	// カメラの更新処理
	m_pCamera->Update();
	// ライトの更新処理
	m_pLight->Update();
}

//==================================================================================================================
//	ポーズの切り替え
//==================================================================================================================
void CGame::SwitchPause(void)
{
	// ゲーム状態がポーズのとき
	if (m_gameState == GAMESTATE_PAUSE)
	{
		// ゲーム状態をNORMALにする
		m_gameState = GAMESTATE_NORMAL;
		// ポーズ状態の設定
		m_pPause->SetPause(false);
	}
	else
	{// ゲーム状態がポーズじゃないとき
	 // ゲーム状態をポーズにする
		m_gameState = GAMESTATE_PAUSE;
		// ポーズ状態の設定
		m_pPause->SetPause(true);
	}
}

//==================================================================================================================
//	次のラウンドへ
//==================================================================================================================
void CGame::NextRound(void)
{
	// nullcheck
	if (m_pUIKO)
	{
		// 破棄
		m_pUIKO->Uninit();
		delete m_pUIKO;
		m_pUIKO = nullptr;
	}

	// ラウンド数を加算
	m_nRound++;
	// プレイヤーのラウンドポイントを加算
	if (GetPlayer(PLAYER_ONE)->GetLife() <= 0 &&
		GetPlayer(PLAYER_TWO)->GetLife() <= 0)
		m_nRound--;
	else if (GetPlayer(PLAYER_ONE)->GetLife() > 0)
		m_roundPoint.nX++;
	else if (GetPlayer(PLAYER_TWO)->GetLife() > 0)
		m_roundPoint.nY++;

	// どちらかが最大まで得点したら終了
	if (m_roundPoint.nX == (int)((float)m_nRoundAll / 2.0f + 0.5f) ||
		m_roundPoint.nY == (int)((float)m_nRoundAll / 2.0f + 0.5f))
	{
		// 勝者を決める
		if (m_roundPoint.nX > m_roundPoint.nY)
		{
			m_winPlayer = PLAYER_ONE;
			m_losePlayer = PLAYER_TWO;
		}
		else
		{
			m_winPlayer = PLAYER_TWO;
			m_losePlayer = PLAYER_ONE;
		}		// リザルトへ
		m_gameState = GAMESTATE_RESULT;
	}
	// それ以外は再バトル
	else
	{
		// フェード取得
		CFade *pFade = CManager::GetRenderer()->GetFade();
		// フェード時間を設定し、空のフェード実行
		pFade->SetFade(CRenderer::MODE_NONE, TIME_FADE_NEXTROUND);
		// バトル前へ
		m_gameState = GAMESTATE_BEFORE;
		m_pPlayer[PLAYER_ONE]->SetPos(DEFAULTPOS_1P);
		m_pPlayer[PLAYER_TWO]->SetPos(DEFAULTPOS_1P);
		m_pPlayer[PLAYER_ONE]->SetLife(m_pPlayer[PLAYER_ONE]->GetMaxLife());
		m_pPlayer[PLAYER_TWO]->SetLife(m_pPlayer[PLAYER_TWO]->GetMaxLife());
	}
}

//==================================================================================================================
//	ゲームのリザルト
//==================================================================================================================
void CGame::GameResult(void)
{
	// 生成
	if (!m_pUIGameResult)
		m_pUIGameResult = CUI_GameResult::Create();

	// 勝利時の行動
	m_pPlayer[m_winPlayer]->VictoryAction();

	// 更新
	if (m_pUIGameResult)
		m_pUIGameResult->Update();
}

//==================================================================================================================
//	ストーンを生成するか決める
//==================================================================================================================
void CGame::DecideCreateStone(void)
{
	// 乱数化
	srand((unsigned int)time(NULL));

	// カウンタを加算
	m_nCntDecide++;

#ifdef _DEBUG
	// 所持数・出現数の合計が3未満
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_1) && CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LSHIFT) && 
		m_nNumStone + GetPlayer(PLAYER_ONE)->GetNumStone() + GetPlayer(PLAYER_TWO)->GetNumStone() < 3)
		// ストーン生成
		AppearStone();
#endif

	// 時間以内
	if (m_nCntDecide <= TIME_CREATE_STONE)
		// 処理を終える
		return;

	// 所持数・出現数の合計が3未満
	if (m_nNumStone + GetPlayer(PLAYER_ONE)->GetNumStone() + GetPlayer(PLAYER_TWO)->GetNumStone() < 3)
		// ストーン生成
		AppearStone();

	// カウンタを初期化
	m_nCntDecide = 0;
}

//==================================================================================================================
//	生成位置をランダムで決める
//==================================================================================================================
int CGame::DecideRandomPos(void)
{
	// ランダムの範囲
	int RandRange = STONE_POS;
	
	// 生成された数分範囲を減らす
	for (int nCnt = 0; nCnt < STONE_POS; nCnt++)
	{
		if (m_bSetPos[nCnt])
			RandRange--;
	}

	// 範囲の分だけメモリ確保
	int *RandPos = new int[RandRange];
	// 番号カウンタ
	int nCntRand = 0;

	// 生成されていない番号を配列に保存
	for (int nCnt = 0; nCnt < STONE_POS; nCnt++)
	{
		if (!m_bSetPos[nCnt])
		{
			RandPos[nCntRand] = nCnt;
			nCntRand++;
		}
	}

	// 出力用
	int outValue = RandPos[rand() % RandRange];
	// ポインタの破棄
	delete[] RandPos;

	// 値を返す
	return outValue;
}
