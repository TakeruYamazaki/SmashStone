//==================================================================================================================
//
// ゲーム開始時のUI処理 [ UI_gameStart.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _UI_START_H_
#define _UI_START_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
//
// ポーズクラスの定義
//
//==================================================================================================================
// 前方宣言
class CScene2D;

class CUI_GameStart
{
public:
	typedef enum
	{
		GAMEUITEX_ROUND = 0,// Round
		GAMEUITEX_1,		// 1
		GAMEUITEX_2,		// 2
		GAMEUITEX_3,		// 3
		GAMEUITEX_FIGHT,	// Fight
		GAMEUITEX_MAX		// 最大数
	} GAMEUITEX_TYPE;		// ゲーム開始時のテクスチャのタイプ

	CUI_GameStart();			// コンストラクタ
	~CUI_GameStart();		// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	static CUI_GameStart *Create(void);		// 生成処理
	static HRESULT Load(void);				// ロード
	static void Unload(void);				// アンロード

protected:

private:
	void CreateUI(int type);		// UIの生成
	static LPDIRECT3DTEXTURE9 m_pTexture[GAMEUITEX_MAX];	// テクスチャ情報
	static CScene2D *m_pScene2D[GAMEUITEX_MAX];				// テクスチャ数分の2DUI
	static char *m_apFileName[GAMEUITEX_MAX];				// テクスチャのファイル名
	int m_nCntFase;											// テクスチャ生成の順序
	int m_nCntAny;											// UIの移動や表示などに使う
};
#endif
