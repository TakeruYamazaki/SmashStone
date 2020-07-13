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
#include <memory>	// スマートポインタの使用に必要

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define STONE_POS (5)

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
		GAMESTATE_NORMAL,		// 通常状態
		GAMESTATE_PAUSE,		// ポーズ状態
		GAMESTATE_START_OVER,	// 初めからやり直す
		GAMESTATE_BREAK,		// ゲーム中断
		GAMESTATE_END,			// ゲームの終了
		GAMESTATE_MAX			// 最大
	} GAMESTATE;				// ゲームの状態

	enum
	{
		POLYCOLLI_STAIRS = 0,
		POLYCOLLI_LONGSTAIRS,
		POLYCOLLI_MAX
	};

	CGame();									// コンストラクタ
	~CGame();									// デストラクタ
	void Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理
	static CGame *Create(void);					// 生成処理

	static void SetGameState(GAMESTATE state)	{ m_gameState = state; }	// ゲームの状態設定
	static void RemoveNumStone(void)			{ m_nNumStone--; }			// ストーンの数を減算
	static void SetNumStone(const int &nStone)	{ m_nNumStone = nStone; }	// ストーンの数の設定

	static GAMESTATE GetGameState(void)			{ return m_gameState; }			// ゲームの状態取得
	static CPlayer *GetPlayer(int nPlayer)		{ return m_pPlayer[nPlayer]; }	// キャラクターの情報取得処理
	static CCamera *GetCamera(void)				{ return m_pCamera; }			// カメラの取得
	static CLight *GetLight(void)				{ return m_pLight; }			// ライトの取得
	static CMeshField *GetMeshField(void)		{ return m_pMeshField; }		// メッシュフィールドの取得
	static int GetNumStone(void)				{ return m_nNumStone; }			// ストーンの数を取得
	static CWall *GetWall(void)					{ return m_pWall; }				// 壁の取得
	static CObjectManager *GetObjMana(void)		{ return m_pObjMana.get(); }			// オブジェクトマネージャーを取得

	static CPolygonCollider* GetpolyColly(int nIndex) { return m_pPolyColli[nIndex]; }
protected:

private:
	void DecideCreateStone(void);				// ストーンを生成するか決める
	static GAMESTATE m_gameState;				// ゲーム状態
	static CPlayer *m_pPlayer[MAX_PLAYER];		// プレイヤーの配列ポインタ
	static CMeshField *m_pMeshField;			// メッシュフィールドの情報ポインタ
	static CCamera *m_pCamera;					// カメラの情報ポインタ
	static CLight *m_pLight;					// ライトの情報ポインタ
	static CLogo *m_pLogo;						// ロゴの情報ポインタ
	static CPause *m_pPause;					// ポーズの情報ポインタ
	static CMeshSphere *m_pMeshSphere;			// メッシュ球の情報ポインタ
	static CTime *m_pTime;						// タイムの情報ポインタ
	static CWall *m_pWall;						// 壁のポインタ
	static int m_nCounterGameState;				// ゲームの状態管理カウンター
	static int m_nNumStone;						// 生成したストーンの数
	static int m_nCntDecide;					// ストーン生成のタイミングを決めるカウンタ
	static D3DXVECTOR3 m_stonePos[STONE_POS];	// ストーンの生成場所
	static std::unique_ptr<CObjectManager> m_pObjMana;	// オブジェクトマネージャーのポインタ
	static CPolygonCollider* m_pPolyColli[POLYCOLLI_MAX];		// ポリゴンコライダーのポインタ
	D3DXMATRIX  m_mtxWorld;						// マトリックス
};
#endif
