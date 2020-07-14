//==================================================================================================================
//
// barクラス[bar.h]
// Author:Seiya Takahashi
//
//==================================================================================================================
#ifndef _BAR_H
#define _BAR_H

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define TEXTURE_BAR 10												// テクスチャの最大数
#define MAX_WIDTH 20.0f												// 横の最大サイズ
#define MAX_WIDTH_FREAM 35.0f										// フレームの横サイズ
#define MAX_HEIGHT 80.0f											// 縦の最大サイズ
#define MAX_HEIGHT_FREAME 160.0f									// フレームの縦サイズ

//==================================================================================================================
// クラスの定義
//==================================================================================================================
class CBar
{
public:

	//=============================================================================
	// 列挙型定義
	//=============================================================================
	typedef enum
	{
		BARTYPE_FREAME0 = 0,	// 0番目のフレーム
		BARTYPE_HP0,			// 0番目の体力バー
		BARTYPE_FREAME1,		// 1番目のフレーム
		BARTYPE_HP1,			// 1番目の体力バー
		BARTYPE_FREMAE2,		// 2番目のフレーム
		BARTYPE_HP2,			// 2番目の体力バー
		BARTYPE_FREAME3,		// 3番目のフレーム
		BARTYPE_HP3,			// 3番目の体力バー
		BARTYPE_FREAME4,		// 4番目のフレーム
		BARTYPE_HP4,			// 4番目の体力バー
		BARTYPE_MAX				// 最大数
	} BARTYPE;					// Barタイプ

	CBar();								// コンストラクタ
	~CBar();							// デストラクタ
	void Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static CBar *Create(void);			// 生成処理
	static HRESULT Load(void);			// テクスチャ読込処理
	static void Unload(void);			// テクスチャ破棄処理

	void SetVertexBar(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);	// 頂点設定処理
	void RotBar(int index, D3DXVECTOR3 pos, float fAngle, D3DXVECTOR3 diff);					// バー回転処理
	void SetColBar(int index, D3DXCOLOR col);													// バー色設定処理

protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTexture[BARTYPE_MAX];	// テクスチャ情報
	static char *m_apFileName[BARTYPE_MAX];				// Barのファイル名

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;					// 頂点バッファ
	VERTEX_2D *m_pVtx;									// 頂点格納
};
#endif
