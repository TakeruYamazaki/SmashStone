//==================================================================================================================
//
// シーンを使わない2Dポリゴン処理 [ polygon2D.h ]
// Author : KANAN NAGANAWA
//
//==================================================================================================================
#ifndef _2DPOLYGON_H_
#define _2DPOLYGON_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================

//==================================================================================================================
//
// 描画2Dクラスの定義
//
//==================================================================================================================
class CPolygon2D
{
public:
	typedef enum
	{
		POSSTART_TOP_LEFT,			// 上左
		POSSTART_TOP_CENTRAL,		// 上中央
		POSSTART_TOP_RIGHT,			// 上右
		POSSTART_CENTRAL_LEFT,		// 中央左
		POSSTART_CENTRAL_CENTRAL,	// 中央中央
		POSSTART_CENTRAL_RIGHT,		// 中央右
		POSSTART_BOTTOM_LEFT,		// 下左
		POSSTART_BOTTOM_CENTRAL,	// 下中央
		POSSTART_BOTTOM_RIGHT,		// 下右
		POSSTART_MAX				// 最大数
	} POSSTART_TYPE;				// 位置の始点の種類

	CPolygon2D();		// コンストラクタ
	~CPolygon2D();		// デストラクタ
	virtual HRESULT Init(void);	// 初期化
	virtual void Uninit(void);	// 終了
	virtual void Update(void);	// 更新
	virtual void Draw(void);	// 描画

	static CPolygon2D *Create(void);										// 生成
	void BindTexture(const LPDIRECT3DTEXTURE9 tex)	{ m_pTexture = tex; }	// テクスチャのバインド

	void SetPosStart(const POSSTART_TYPE posStart)	{ m_posStart = posStart; }	// 位置の始点
	void SetPos(const D3DXVECTOR3 & pos)			{ m_pos = pos; }			// 位置設定処理
	void SetbShow(bool bShow)						{ m_bShow = bShow; }		// 描画するか設定
	void SetCol(const D3DXCOLOR & col);											// 色設定処理
	void SetSize(const D3DXVECTOR3 & size);										// 大きさ設定処理
	void SetAnim(const D3DXVECTOR2 & UV, const D3DXVECTOR2 & size);				// テクスチャ座標設定

	D3DXVECTOR3 *GetPos(void)	{ return &m_pos; }						// 位置取得処理
	D3DXVECTOR3 *GetSize(void)	{ return &m_size; }						// 大きさ取得処理
	bool GetbShow(void)			{ return m_bShow; }						// 描画するか取得

protected:
	POSSTART_TYPE m_posStart;											// 位置の視点
	D3DXVECTOR3 m_pos;													// 位置
	D3DXVECTOR3 m_size;													// 大きさ
	D3DXVECTOR3 m_rot;													// 回転
	D3DXCOLOR m_col;													// カラー
	bool  m_bShow;														// 描画するか
	float m_fAngle;														// 角度
	float m_fLength;													// 長さ

private:
	void MakeVertexPos(void);											// 頂点座標の設定
	LPDIRECT3DTEXTURE9 m_pTexture;										// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;									// 頂点バッファ

};
#endif