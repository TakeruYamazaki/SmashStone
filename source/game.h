//==================================================================================================================
//
// ゲーム処理[game.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _GAME_H
#define _GAME_H

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"
#include "Mylibrary.h"
#include "kananlibrary.h"
#include "stone.h"
#include <memory>	// スマートポインタの使用に必要

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define STONE_POS (5)
#define POLYCOLLI_USE_TYPE	4

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CMeshField;
class CCamera;
class CLight;
class CLogo;
class CModel;
class CPause;
class CMeshSphere;
class CPlayer;
class CTime;
class CWall;
class CObjectManager;
class CPolygonCollider;
class CUIKO;
class CUI_GameStart;
class CUI_GameResult;
class CRenderer;
class CUI;

//==================================================================================================================
//
// ゲームクラスの定義
//
//==================================================================================================================
class CGame
{
public:
	// ==========================================================
	// ゲームの状態
	// ==========================================================
	typedef enum
	{
		GAMESTATE_NONE = 0,		// 何もしていない状態
		GAMESTATE_BEFORE,		// ゲームの前
		GAMESTATE_NORMAL,		// 通常状態
		GAMESTATE_PAUSE,		// ポーズ状態
		GAMESTATE_START_OVER,	// 初めからやり直す
		GAMESTATE_BREAK,		// ゲーム中断
		GAMESTATE_KO,			// KO演出
		GAMESTATE_KO_AFTER,		// KO後
		GAMESTATE_NEXTROUND,	// 次のラウンドへ
		GAMESTATE_RESULT,		// リザルト
		GAMESTATE_END,			// 終了
		GAMESTATE_MAX			// 最大
	} GAMESTATE;				// ゲームの状態

	CGame();									// コンストラクタ
	~CGame();									// デストラクタ
	void Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理
	static CGame *Create(void);					// 生成処理

	static void SetPlayerType(const int nPlayer, const int type)
		{ m_nPlayerType[nPlayer] = type * 2; }									// プレイヤータイプを保存
	static void SetStageType(const int type)	{ m_nStageType = type; }		// ステージタイプの設定
	static void SetGameState(GAMESTATE state)	{ m_gameState = state; }		// ゲームの状態設定
	static void RemoveNumStone(int nIndexPos)	{ m_nNumStone--; m_bSetPos[nIndexPos] = false; }			// ストーンの数を減算
	static void RemoveTypeStone(int nType)		{ m_bGetType[nType] = false; }			// ストーンの取得を無効
	static void SetNumStone(const int &nStone)	{ m_nNumStone = nStone; }		// ストーンの数の設定
	static void SetNextMode(const int nextMode);								// 次のモードへ移行

	static GAMESTATE GetGameState(void)			{ return m_gameState; }			// ゲームの状態取得
	static CPlayer *GetPlayer(int nPlayer)		{ return m_pPlayer[nPlayer]; }	// キャラクターの情報取得処理
	static CCamera *GetCamera(void)				{ return m_pCamera; }			// カメラの取得
	static CLight *GetLight(void)				{ return m_pLight; }			// ライトの取得
	static CMeshField *GetMeshField(void)		{ return m_pMeshField; }		// メッシュフィールドの取得
	static int GetNumStone(void)				{ return m_nNumStone; }			// ストーンの数を取得
	static CWall *GetWall(void)					{ return m_pWall; }				// 壁の取得
	static CObjectManager *GetObjMana(void)		{ return m_pObjMana; }			// オブジェクトマネージャーを取得
	static CPolygonCollider* GetpolyColly(int nIndex) { return m_pPolyColli[nIndex]; }
	static int GetRound(void)					{ return m_nRound; }			// 現在のラウンド数取得
	static int GetAllRound(void)				{ return m_nRoundAll; }			// 全ラウンド数の取得
	static NUM_PLAYER GetLosePlayer(void)		{ return m_losePlayer; }		// 負けたプレイヤーの取得
	static void AppearStone(void);				// どこからでも呼び出せるストーン出現
	static bool GetbGetType(int nType)			{ return m_bGetType[nType]; }	// 取得されたタイプの取得

protected:

private:
	void GameBefore(void);							// ゲームの前の更新
	void GameNormal(void);							// 通常の更新
	void GamePause(void);							// ポーズの更新
	void GameKO(void);								// KOの更新
	void GameKOAfter(void);							// KOの後の更新
	void SwitchPause(void);							// ポーズの切り替え
	void NextRound(void);							// 次のラウンドへ
	void GameResult(void);							// ゲームのリザルト
	void DecideCreateStone(void);					// ストーンを生成するか決める
	static GAMESTATE m_gameState;					// ゲーム状態
	static int m_nPlayerType[MAX_PLAYER];			// キャラクターセレクト時のタイプを保存
	static CPlayer *m_pPlayer[MAX_PLAYER];			// プレイヤーの配列ポインタ
	static CMeshField *m_pMeshField;				// メッシュフィールドの情報ポインタ
	static CCamera *m_pCamera;						// カメラの情報ポインタ
	static CLight *m_pLight;						// ライトの情報ポインタ
	static CLogo *m_pLogo;							// ロゴの情報ポインタ
	static CPause *m_pPause;						// ポーズの情報ポインタ
	static CMeshSphere *m_pMeshSphere;				// メッシュ球の情報ポインタ
	static CTime *m_pTime;							// タイムの情報ポインタ
	static CWall *m_pWall;							// 壁のポインタ
	static CUI *m_pUI;								// UIポインタ
	static int m_nCounterGameState;					// ゲームの状態管理カウンター
	static int m_nNumStone;							// 生成したストーンの数
	static int m_nCntDecide;						// ストーン生成のタイミングを決めるカウンタ
	static bool m_bSetPos[STONE_POS];				// ストーンが生成されているか
	static bool m_bGetType[CStone::STONE_ID_MAX];		// ストーンが取得されているか
	static CObjectManager *m_pObjMana;				// オブジェクトマネージャーのポインタ
	static CPolygonCollider* m_pPolyColli[POLYCOLLI_USE_TYPE];		// ポリゴンコライダーのポインタ
	static CUIKO *m_pUIKO;							// KOのポインタ
	static CUI_GameStart *m_pUIGameStart;			// ゲーム開始時のUIのポインタ
	static CUI_GameResult *m_pUIGameResult;			// ゲームリザルトのUIのポインタ
	static NUM_PLAYER m_winPlayer;					// 勝利したプレイヤー
	static NUM_PLAYER m_losePlayer;					// 負けたプレイヤー
	INTEGER2 m_roundPoint;							// ラウンドのポイント数
	static int m_nRound;							// 現在のラウンド
	static int m_nRoundAll;							// 全ラウンド数
	static int m_nStageType;						// ステージのタイプ
	int m_nCntAny;									// モード毎の様々な状況で使う(主に時間管理)
};
#endif
