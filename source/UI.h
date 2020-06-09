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

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CScene2D;

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
		LOGOTYPE_TITLE = 0,				// タイトル
		LOGOTYPE_ENTER,					// エンターロゴ
		LOGOTYPE_TIMEFRAME,				// タイマーフレーム
		LOGOTYPE_TUTORIAL0,				// チュートリアルロゴ
		LOGOTYPE_TUTORIAL1,				// チュートリアルロゴ
		LOGOTYPE_TUTORIALENTER,			// チュートリアルエンターロゴ
		LOGOTYPE_DE,					// でロゴ
		LOGOTYPE_SKIP,					// スキップロゴ
		LOGOTYPE_PRESSJUMP,				// ジャンプボタンで
		LOGOTYPE_PLAYSWIM,				// 試し泳ぎロゴ
		LOGOTYPE_A,						// キーボードのA
		LOGOTYPE_LEFT,					// 左旋回ロゴ
		LOGOTYPE_D,						// キーボードのD
		LOGOTYPE_RIGHT,					// 右旋回
		LOGOTYPE_S,						// キーボードのS
		LOGOTYPE_REDUCE_SPEED,			// 減速ロゴ
		LOGOTYPE_SPACE,					// キーボードのスペース
		LOGOTYPE_JUMP,					// ジャンプロゴ
		LOGOTYPE_NOW_TIME,				// 現在のタイム
		LOGOTYPE_BEST_TIME,				// 最高のタイム
		LOGOTYPE_DOT0,					// タイムとタイムの間の点0
		LOGOTYPE_DOT1,					// タイムとタイムの間の点1
		LOGOTYPE_DOT2,					// タイムとタイムの間の点2
		LOGOTYPE_DOT3,					// タイムとタイムの間の点3
		LOGOTYPE_PAUSE,					// ポーズロゴ
		LOGOTYPE_GOAL,					// ゴール
		LOGOTYPE_START,					// スタート
		LOGOTYPE_ONE,					// カウント1
		LOGOTYPE_TWO,					// カウント2
		LOGOTYPE_THREE,					// カウント3
		LOGOTYPE_RANK,					// ランク
		LOGOTYPE_RANKING_LOGO,			// ランキングロゴ
		LOGOTYPE_DOT4,					// タイムとタイムの間の点4
		LOGOTYPE_DOT5,					// タイムとタイムの間の点5
		LOGOTYPE_DOT6,					// タイムとタイムの間の点6
		LOGOTYPE_DOT7,					// タイムとタイムの間の点7
		LOGOTYPE_DOT8,					// タイムとタイムの間の点8
		LOGOTYPE_DOT9,					// タイムとタイムの間の点9
		LOGOTYPE_MAX					// 最大
	} UITYPE;

	CUI(PRIORITY type);																	// コンストラクタ
	~CUI();																				// デストラクタ
	void Init(void);																	// 初期化処理
	void Uninit(void);																	// 終了処理
	void Update(void);																	// 更新処理
	void Draw(void);																	// 描画処理

	static CUI *Create(void);															// 生成処理
	static HRESULT Load(void);															// テクスチャ情報ロード処理
	static void Unload(void);															// テクスチャ情報アンロード処理

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ロゴ設定処理

	UITYPE GetType(void);																// ロゴタイプ取得処理

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];		// テクスチャ情報
	static char *m_apFileName[LOGOTYPE_MAX];				// ロゴのファイル名

	UITYPE m_type;											// ロゴタイプ変数
	CScene2D *m_pScene2D[LOGOTYPE_MAX];						// scene2Dの情報

	D3DXVECTOR3 m_pos;										// 位置

	int m_nCounterAnim;										// カウンター
	int m_nPatternAnim;										// パターンNo

	float m_fWidth;											// 縦幅
	float m_fSize;											// 横幅
};
#endif