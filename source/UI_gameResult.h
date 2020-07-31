//==================================================================================================================
//
// ゲームリザルトのUI処理 [ UI_gameResult.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _UI_RESULT_H_
#define _UI_RESULT_H_

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

class CUI_GameResult
{
public:
	CUI_GameResult();		// コンストラクタ
	~CUI_GameResult();		// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	static CUI_GameResult *Create(void);	// 生成処理
	static HRESULT Load(void);				// ロード
	static void Unload(void);				// アンロード

protected:

private:
	typedef enum
	{
		RESULTUITYPE_VICTORY = 0,	// victory
		RESULTUITYPE_FRAME,			// 枠
		RESULTUITYPE_CURSOR,		// カーソル
		RESULTUITYPE_REMATCH,		// 再戦
		RESULTUITYPE_RETURNSELECT,	// キャラセレクト
		RESULTUITYPE_RETURNTITLE,		// タイトルへ
		RESULTUITYPE_MAX			// 最大数
	} TEX_TYPE_GAMERESULT;			// ゲームリザルトのテクスチャのタイプ

	typedef enum
	{
		FASE_VICTORY,	// victory表示
		FASE_RESULT,	// リザルト表示
		FASE_MAX		// 最大数
	} GAMESTART_FASE;	// UI表示の順序

	void Victory(void);
	void Result(void);
	void CursorUp(void);
	void CursorDown(void);
	void CursorDecide(void);
	void CreateUI(const int type);								// UIの生成
	void MoveUI(void);											// UIの移動
	void NextFase(void) { m_fase++; m_nCntAny = 0; }			// 次のフェーズに移行
	static LPDIRECT3DTEXTURE9 m_pTexture[RESULTUITYPE_MAX];		// テクスチャ情報
	static CPolygon2D		  *m_pPolygon[RESULTUITYPE_MAX];	// テクスチャ数分の2DUI
	static char				  *m_apFileName[RESULTUITYPE_MAX];	// テクスチャのファイル名
	static D3DXVECTOR3		  m_posBegin[RESULTUITYPE_MAX];		// UIの初期位置
	static D3DXVECTOR3		  m_sizeBegin[RESULTUITYPE_MAX];	// UIの初期サイズ
	int m_fase;													// テクスチャ生成の順序
	int m_nCntAny;												// UIの移動や表示などに使う
};
#endif
#pragma once
