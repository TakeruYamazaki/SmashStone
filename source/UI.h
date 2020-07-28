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
class CInputKeyboard;

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

	UITYPE GetType(void);			// ロゴタイプ取得処理

protected:

private:
	void TitleUpdate(CInputKeyboard *pInputKeyboard);	// タイトルの更新処理
	void TutorialUpdate(CInputKeyboard *pInputKeyboard);// チュートリアルの更新処理

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// テクスチャ情報
	static char *m_apFileName[LOGOTYPE_MAX];			// ロゴのファイル名

	UITYPE m_type;										// ロゴタイプ変数
	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2Dの情報

	D3DXVECTOR3 TitlePos;								// タイトルの位置

	int m_nCntBound;									// タイトルUIバウンドカウンタ
	int m_nMode;										// モード番号
	int m_nCharaNum[MAX_PLAYER];						// キャラ番号

	float m_nCntUITitle0;								// タイトルUI用カウンタ0
	float m_nCntUITitle1;								// タイトルUI用カウンタ1
	float m_nCntEnter;									// エンター用カウンタ
	float m_nCntUISign;									// 看板用カウンタ

	bool m_bUITitle0;									// タイトルを動かすかどうか0
	bool m_bUITitle1;									// タイトルを動かすかどうか1
	bool m_bUIEnter;									// エンターのα値用変数
};
#endif