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
//==================================================================================================================
//	マクロ定義
//==================================================================================================================
#define PLAYER_START_POS_X 390.0f									// プレイヤーの初期位置X
#define PLAYER_START_POS_Z -585.0f									// プレイヤーの初期位置Z
#define RESPAWN_SIZE 0.000001f										// リスポーンモデルの大きさ

#define TIME_CREATE_STONE (5 * ONE_SECOND_FPS)						// ストーンを生成する時間

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
CHitPoint			*CGame::m_pHitPoint				= NULL;							// HPの情報
CTime				*CGame::m_pTime					= NULL;							// タイム情報
CWall				*CGame::m_pWall					= NULL;							// 壁のポインタ
CGame::GAMESTATE	CGame::m_gameState				= CGame::GAMESTATE_NONE;		// ゲーム状態
int					CGame::m_nCounterGameState		= NULL;							// ゲームの状態管理カウンター
int					CGame::m_nNumStone				= 0;							// 生成したストーンの数
int					CGame::m_nCntDecide				= 0;							// ストーン生成のタイミングを決めるカウンタ
std::unique_ptr<CObjectManager>	CGame::m_pObjMana	= nullptr;						// オブジェクトマネージャーのポインタ

D3DXVECTOR3			CGame::m_stonePos[STONE_POS] = 									// ストーンの生成場所
{
	D3DXVECTOR3(0.0f, 20.0f, 0.0f),
	D3DXVECTOR3(100.0f, 20.0f, 100.0f),
	D3DXVECTOR3(100.0f, 20.0f, -100.0f),
	D3DXVECTOR3(-100.0f, 20.0f, 100.0f),
	D3DXVECTOR3(-100.0f, 20.0f, -100.0f)
};

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
	CCylinderCoillider::Load();				// シリンダーコライダーのロード

	/* 生成 */
	C3DBoxCollider::Create();										// ボックスコライダーの生成
	m_pObjMana    = CObjectManager::Create();						// オブジェクトマネージャーの生成
	m_pWall       = CWall::Create(CWall::WALLTEX_FIELD);			// 壁の生成
	m_pCamera     = CCamera::Create();								// カメラの生成処理
	m_pLight      = CLight::Create();								// ライトの生成処理
	m_pMeshSphere = CMeshSphere::Create();							// メッシュ球の生成処理
	// プレイヤーの最大数までカウント
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		m_pPlayer[nCnt] = CPlayer::Create(nCnt, CHARACTER_1YASU);	// プレイヤー生成
	}
	m_pMeshField = CMeshField::Create();							// メッシュフィールド生成
	m_pHitPoint  = CHitPoint::Create();								// HP生
	m_pTime      = CTime::Create();									// タイム生成
	m_pPause     = CPause::Create();								// ポーズの生成処理

	/* ゲームの初期化 */
	SetGameState(GAMESTATE_NORMAL);			// 通常状態に設定
	m_nCounterGameState = 0;				// ゲームの状態管理カウンターを0にする
	m_nNumStone			= 0;				// 値を初期化
	m_nCntDecide		= 0;				// 値を初期化
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
	CCylinderCoillider::Unload();
	CWall::Unload();					// 壁の開放
	C3DBoxCollider::Unload();			// 3Dボックスコライダーの開放
	CStone::Unload();					// ストーンの開放
	CNumber::Unload();					// 数字テクスチャアンロード
	CMeshField::Unload();				// 床テクスチャアンロード
	CMotionModel::Unload();				// モーション用モデルアンロード
	CBar::Unload();						// Barテクスチャアンロード
	CObjectManager::Unload();			// オブジェクトマネージャーのアンロード

	// ポーズの終了処理
	m_pPause->Uninit();

	m_pObjMana->Uninit();				// 終了処理
	m_pObjMana.reset();					// メモリ削除
	m_pObjMana = nullptr;				// ポインタNULL

	delete m_pPause;					// メモリ削除
	m_pPause = nullptr;					// ポインタNULL

	delete m_pCamera;					// メモリ削除
	m_pCamera = nullptr;				// ポインタNULL

	delete m_pLight;					// メモリ削除
	m_pLight = nullptr;					// ポインタNULL

	m_pHitPoint = nullptr;				// ポインタNULL

	m_pTime = nullptr;					// ポインタNULL
}

//==================================================================================================================
//	更新処理
//==================================================================================================================
void CGame::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// フェード取得
	CFade::FADE fade = CFade::GetFade();

	// ゲームの状態取得
	m_gameState = GetGameState();

	// ゲーム状態がポーズのとき
	if (m_gameState == GAMESTATE_PAUSE)
	{
		// ポーズの更新処理
		m_pPause->Update();

		// ゲーム状態が初めからやり直すとき
		if (m_gameState == GAMESTATE_START_OVER)
		{
			// フェードを設定する
			CFade::SetFade(CRenderer::MODE_GAME);
		}
		else if (m_gameState == GAMESTATE_BREAK)
		{// ゲーム状態が中断のとき
			// フェードを設定する
			CFade::SetFade(CRenderer::MODE_TITLE);
		}
	}
	else
	{
		// カメラの更新処理
		m_pCamera->Update();

		// ライトの更新処理
		m_pLight->Update();

		// ストーンを生成するか決める
		DecideCreateStone();

		// オブジェクトマネージャーの更新
		m_pObjMana->Update();
	}

	// キーボードの[P] 又は コントローラーの[START]ボタンが押されたとき
	if (pInputKeyboard->GetKeyboardTrigger(DIK_P))
	{// ポーズ切り替え
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
#ifdef _DEBUG
	// キーボードの[0]を押したとき
	if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN))
	{
		// フェードが何もない時
		if (fade == CFade::FADE_NONE)
		{
			// フェードを設定する
			CFade::SetFade(CRenderer::MODE_RANKING);
		}
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

	// オブジェクトマネージャーの描画処理
	m_pObjMana->Draw();

	// ポーズ状態がtrueのとき
	if (m_pPause->GetPause() == true)
	{
		// ポーズの更新処理
		m_pPause->Draw();
	}
}

//==================================================================================================================
//	生成処理
//==================================================================================================================
CGame * CGame::Create(void)
{
	// 変数宣言
	CGame *pGame = NULL;		// ゲーム変数NULL
	pGame = new CGame;			// 動的に確保
	pGame->Init();				// 初期化処理
	return pGame;				// 値を返す
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
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_1) && 
		m_nNumStone + GetPlayer(0)->GetNumStone() + GetPlayer(1)->GetNumStone() < 3)
	{
		// 決められた位置からランダムで生成
		CStone::Create(CStone::STONE_ID_DEFAULT, m_stonePos[rand() % STONE_POS + 1]);
		m_nNumStone++;
	}
#endif

	// 時間以内
	if (m_nCntDecide <= TIME_CREATE_STONE)
	{
		// 処理を終える
		return;
	}

	if (m_nNumStone + GetPlayer(0)->GetNumStone() + GetPlayer(1)->GetNumStone() < 3)
	{
		// 決められた位置からランダムで生成
		CStone::Create(CStone::STONE_ID_DEFAULT, m_stonePos[rand() % STONE_POS + 1]);
		// 出現数を加算
		m_nNumStone++;
	}

	// カウンタを初期化
	m_nCntDecide = 0;
}
