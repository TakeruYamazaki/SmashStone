//==================================================================================================================
//
// UI[UI.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_H_
#define _UI_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"
#include "scene.h"
#include "stone.h"

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CScene2D;
class CInputKeyboard;
class CInputGamepad;

//==================================================================================================================
//
// ロゴクラスの定義
//
//==================================================================================================================
class CUI : public CScene
{
public:
	//=============================================================================
	// 列挙型定義
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_TITLE = 0,			// タイトル
		LOGOTYPE_ENTER,				// エンターロゴ
		LOGOTYPE_ARROW,				// コンパス矢印
		LOGOTYPE_SIGN,				// 看板
		LOGOTYPE_MODE00,			// モード00
		LOGOTYPE_MODE01,			// モード01
		LOGOTYPE_MODEFREAM,			// モード枠線
		LOGOTYPE_GEAR0,				// 歯車0
		LOGOTYPE_CLOCK0,			// 時計土台0
		LOGOTYPE_CLOCKHANDS0,		// 時計の針0
		LOGOTYPE_CLOCKGEAR0,		// 時計の歯車0
		LOGOTYPE_GEAR1,				// 歯車0
		LOGOTYPE_CLOCK1,			// 時計土台0
		LOGOTYPE_CLOCKHANDS1,		// 時計の針0
		LOGOTYPE_CLOCKGEAR1,		// 時計の歯車0
		LOGOTYPE_CHARAFULL,			// キャラクター全員
		LOGOTYPE_1PCHARA,			// 1Pキャラクター
		LOGOTYPE_2PCHARA,			// 2Pキャラクター
		LOGOTYPE_1PCHARA_FREAM,		// 1Pキャラクター枠線
		LOGOTYPE_2PCHARA_FREAM,		// 2Pキャラクター枠線
		LOGOTYPE_1PCHARA_NAME,		// 1Pキャラクターネーム
		LOGOTYPE_2PCHARA_NAME,		// 2Pキャラクターネーム
		LOGOTYPE_1PREADY,			// 1Pキャラクター準備完了
		LOGOTYPE_2PREADY,			// 2Pキャラクター準備完了
		LOGOTYPE_SELECTICON,		// プレイヤーセレクトアイコン
		LOGOTYPE_WORLDMAP,			// 世界地図
		LOGOTYPE_MAPFRAME,			// マップ選択枠
		LOGOTYPE_MAP1,				// マップ1
		LOGOTYPE_MAP2,				// マップ2
		LOGOTYPE_MAP3,				// マップ3
		LOGOTYPE_MAP4,				// マップ4
		LOGOTYPE_MAPEXPLANATION1,	// マップ説明
		LOGOTYPE_MAPEXPLANATION2,	// マップ説明
		LOGOTYPE_MAPEXPLANATION3,	// マップ説明
		LOGOTYPE_MAPEXPLANATION4,	// マップ説明
		LOGOTYPE_GAMEBG,			// ゲーム背景
		LOGOTYPE_JEWELRYBG1P,		// 宝石背景1P
		LOGOTYPE_JEWELRYBG2P,		// 宝石背景2P
		LOGOTYPE_JEWELRYRED,		// 宝石赤
		LOGOTYPE_JEWELRYBULE,		// 宝石青
		LOGOTYPE_JEWELRYGREEN,		// 宝石緑
		LOGOTYPE_PLAYER1,			// プレイヤーアイコン1P
		LOGOTYPE_PLAYER2,			// プレイヤーアイコン2P
		LOGOTYPE_CHARANAME0,		// 1Pのキャラクターネーム
		LOGOTYPE_CHARANAME1,		// 2Pのキャラクターネーム
		LOGOTYPE_MAX				// 最大
	} UITYPE;

	CUI(PRIORITY type);				// コンストラクタ
	~CUI();							// デストラクタ
	void Init(void);				// 初期化処理
	void Uninit(void);				// 終了処理
	void Update(void);				// 更新処理
	void Draw(void);				// 描画処理

	static CUI *Create(void);		// 生成処理
	static HRESULT Load(void);		// テクスチャ情報ロード処理
	static void Unload(void);		// テクスチャ情報アンロード処理

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ロゴ設定処理

	static int GetMapID(void) { return m_nMapID; }	// マップIDの取得
	UITYPE GetType(void);			// ロゴタイプ取得処理
	int GetCharaNum(int nPlayer) { return m_nCharaNum[nPlayer]; }	// プレイヤーのタイプを取得
	bool GetCharaDecide(int nPlayer) { return m_bCharaDecide[nPlayer]; }
	bool GetSelectMap(void) { return m_bMapSelect; }	// マップの選択状況を取得

protected:

private:
	void TitleUpdate(CInputKeyboard *pKeyboard, CInputGamepad *Gamepad0, CInputGamepad *pGamepad1);	// タイトルの更新処理
	void TutorialUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// チュートリアルの更新処理
	void MapSelectUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// マップ選択画面の更新処理
	void GameUpdate(void);								// ゲームの更新処理
	void ControlGamepad(CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// ゲームパッド操作
	void ControlKeyboard(CInputKeyboard *pKeyboard);	// キーボード操作

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// テクスチャ情報
	static char *m_apFileName[LOGOTYPE_MAX];			// ロゴのファイル名

	UITYPE m_type;										// ロゴタイプ変数
	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2Dの情報

	D3DXVECTOR3 TitlePos;								// タイトルの位置

	int m_nCntBound;									// タイトルUIバウンドカウンタ
	int m_nMode;										// モード番号
	int m_nCharaNum[MAX_PLAYER];						// キャラ番号
	int m_nCntRot[MAX_PLAYER];							// 時計の針回転用カウンタ
	int m_nCntWait[MAX_PLAYER];							// 待機時間用カウンタ
	int m_nCntMove[MAX_PLAYER];							// 枠線移動用カウンタ
	int m_nPlayer;										// プレイヤー番号
	static int m_nMapID;								// マップ番号

	float m_fCntUITitle0;								// タイトルUI用カウンタ0
	float m_fCntUITitle1;								// タイトルUI用カウンタ1
	float m_fCntEnter;									// エンター用カウンタ
	float m_fCntUISign;									// 看板用カウンタ
	float m_fPosMove[MAX_PLAYER];						// 位置移動用カウンタ
	float m_fPos[MAX_PLAYER];							// 現在の枠線テクスチャ位置X
	float m_fPosDiff[MAX_PLAYER];						// 目標の枠線テクスチャ位置X
	float m_fPosOld[MAX_PLAYER];						// 前回の枠線テクスチャ位置X
	float m_fRotGear[MAX_PLAYER];						// 歯車の回転格納変数
	float m_fPosCul[MAX_PLAYER];						// 位置計算用変数
	float m_fDiff[MAX_PLAYER];							// 1フレーム前との距離
	float m_fAngle[MAX_PLAYER];							// 歯車の回転角度
	float m_fRad[MAX_PLAYER];							// ラジアン値

	bool m_bUITitle0;									// タイトルを動かすかどうか0
	bool m_bUITitle1;									// タイトルを動かすかどうか1
	bool m_bUIEnter;									// エンターのα値用変数
	bool m_bUIClockHands[MAX_PLAYER];					// 時計の針が動いているかどうか
	bool m_bCharaDecide[MAX_PLAYER];					// 自分のキャラクターを選択したかどうか
	bool m_bStickReturn[MAX_PLAYER];					// パッドのスティックを戻したかどうか
	bool m_bMapSelect;									// マップを選択したかどうか
	bool m_bTransform[MAX_PLAYER];						// 変身したかどうか
	bool m_bStoneID[MAX_PLAYER][CStone::STONE_ID_MAX];	// ストーンID

};
#endif