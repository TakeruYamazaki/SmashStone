// ===================================================================
//
// ライブラリ処理 [ kananlibrary.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _KANANLIBRARY_H_
#define _KANANLIBRARY_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"

// ===================================================================
// マクロ定義
// ===================================================================
// ベクトル
#define ZeroVector2		(D3DXVECTOR2(0.0f, 0.0f))
#define OneVector2		(D3DXVECTOR2(1.0f, 1.0f))
#define ZeroVector3		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define OneVec2Zero		(D3DXVECTOR3(1.0f, 1.0f, 0.0f))
#define ZeroVector4		(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))

// カラー
#define ZeroColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))		// 無色
#define AlphaColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))		// 透明
#define WhiteColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))		// 白
#define BlackColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))		// 黒

// 法線
#define AboveNormal		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))			// 真上の法線ベクトル

// テキスト
#define MAX_TEXT		(128)									// テキストの最大数

// 物理
#define INERTIA			(0.3f)									// 慣性

// ===================================================================
// クラス定義
// ===================================================================
class CInputKeyboard;
class CInputGamepad;

class CKananLibrary
{
public:
	CKananLibrary() {};
	~CKananLibrary() {};

	static void CalcMatrix(D3DXMATRIX *pMtx, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot);			// マトリックス計算

	static void InterpolationRot(D3DXVECTOR3 *rot);		// 回転の補間
	static void InterpolationFloat(float & Float);		// float型の補間
	static void InertiaMove(D3DXVECTOR3 *move);			// 慣性の処理
	static void PrintBlockCommentFrame(void);			// ブロックコメント枠の表示

	static HRESULT Up(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// 上入力
	static HRESULT Down(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// 下入力
	static HRESULT Left(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// 左入力
	static HRESULT Right(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// 右入力
	static HRESULT Decide(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// 決定入力
	static HRESULT Return(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// 戻り入力
	static HRESULT Pause(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// ポーズ入力

	static float OutputSqrt(D3DXVECTOR3 difpos);				// 平方根計算
	static float OutputDistance(D3DXVECTOR3 difpos);			// 距離計算

#ifdef _DEBUG
	static void ShowDebugInfo(void);					// ImGuiの更新
	static bool GetWire(void) { return m_WireFrame; }	// ワイヤーフレーム描画の取得
#endif

private:

#ifdef _DEBUG
	static bool m_WireFrame;			// ワイヤーフレーム描画の切り替え用
	static int m_nCulling;				// カリングの切り替え用
#endif

};
#endif