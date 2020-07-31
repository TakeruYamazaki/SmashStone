//==================================================================================================================
//
// KOのUI処理 [ UI_KO.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _UI_KO_H_
#define _UI_KO_H_

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

class CUIKO
{
public:
	CUIKO();	// コンストラクタ
	~CUIKO();				// デストラクタ
	void Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	static CUIKO *Create(void);		// 生成処理
	static HRESULT Load(void);		// ロード
	static void Unload(void);		// アンロード

protected:

private:
	typedef enum
	{
		KOUITEX_K = 0,	// K
		KOUITEX_O,		// O
		KOUITEX_BACK,	// 背景
		KOUI_MAX		// 最大数
	} KO_TEX_TYPE;		// KOテクスチャのタイプ

	void MoveUI(void);				// UIの移動
	void CreateUI(int type);		// UIの生成
	void NextFase(void) { m_nCntFase++; m_nCntAny = 0; }	// 次のフェーズに移行
	static LPDIRECT3DTEXTURE9 m_pTexture[KOUI_MAX];	// テクスチャ情報
	static CPolygon2D *m_pPolygon[KOUI_MAX];			// テクスチャ数分の2DUI
	static char *m_apFileName[KOUI_MAX];			// テクスチャのファイル名
	static D3DXVECTOR3 m_sizeBegin[KOUI_MAX];		// 最初のテクスチャのサイズ
	static D3DXVECTOR3 m_sizeEnd[KOUI_MAX];			// 最後のテクスチャのサイズ
	static D3DXVECTOR3 m_posBegin[KOUI_MAX];		// 最初のテクスチャの位置
	static D3DXVECTOR3 m_posEnd[KOUI_MAX];			// 最後のテクスチャの位置
	int m_nCntFase;									// テクスチャ生成の順序
	int m_nCntAny;									// UIの移動や表示などに使う
};
#endif
