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

//==================================================================================================================
//
// ポーズクラスの定義
//
//==================================================================================================================
// 前方宣言
class CPolygon2D;

class CUI_GameStart
{
public:
	CUI_GameStart();		// コンストラクタ
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
	typedef enum
	{
		STARTUITYPE_ROUND = 0,	// ラウンド
		STARTUITYPE_NUMROUND,	// ラウンド数
		STARTUITYPE_FIGHT,		// ファイト
		STARTUITYPE_MAX			// 最大数
	} UITYPE_GAMESTART;			// UIの種類

	typedef enum
	{
		GAMEUITEX_ROUND = 0,// Round
		GAMEUITEX_1,		// 1
		GAMEUITEX_2,		// 2
		GAMEUITEX_3,		// 3
		GAMEUITEX_FIGHT,	// Fight
		GAMEUITEX_MAX		// 最大数
	} TEX_TYPE_GAMESTART;	// ゲーム開始時のテクスチャのタイプ

	typedef enum
	{
		FASE_NONE = -1,
		FASE_ROUND,		// ラウンド
		FASE_RONNDNUM,	// ラウンド数
		FASE_FIGHT,		// fight
		FASE_MAX		// 最大数
	} GAMESTART_FASE;	// UI表示の順序

	void CreateUI(const int type);							// UIの生成
	void MoveUI(const int type, const int time);			// UIの移動
	void NextFase(void) { m_fase++; m_nCntAny = 0; }		// 次のフェーズに移行
	static LPDIRECT3DTEXTURE9 m_pTexture[GAMEUITEX_MAX];	// テクスチャ情報
	static CPolygon2D		  *m_pPolygon[STARTUITYPE_MAX];	// テクスチャ数分の2DUI
	static char				  *m_apFileName[GAMEUITEX_MAX];	// テクスチャのファイル名
	static D3DXVECTOR3		  m_sizeBegin[STARTUITYPE_MAX];	// 最初のテクスチャのサイズ (ROUND, ROUND数, FIGHTの3つ)
	static D3DXVECTOR3		  m_sizeEnd[STARTUITYPE_MAX];	// 最後のテクスチャのサイズ
	static D3DXVECTOR3		  m_posBegin[STARTUITYPE_MAX];	// 最初のテクスチャの位置
	static D3DXVECTOR3		  m_posEnd[STARTUITYPE_MAX];	// 最後のテクスチャの位置
	int m_fase;												// テクスチャ生成の順序
	int m_nCntAny;											// UIの移動や表示などに使う
};
#endif
