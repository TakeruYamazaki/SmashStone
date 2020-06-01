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
#define ZeroVector2		(D3DXVECTOR2(0.0f, 0.0f))
#define OneVector2		(D3DXVECTOR2(1.0f, 1.0f))
#define ZeroVector3		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define OneVec2Zero		(D3DXVECTOR3(1.0f, 1.0f, 0.0f))
#define ZeroVector4		(D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f))
#define ZeroColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f))
#define AlphaColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))
#define WhiteColor		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
#define BlackColor		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))
#define AbobeNormal		(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
#define MAX_TEXT		(128)

// ===================================================================
// 構造体定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CKeyboard;
class CXGamepad;

class CKananLibrary
{
public:
	CKananLibrary() {};
	~CKananLibrary() {};

	static void CalcMatrix(D3DXMATRIX *pMtx, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot);			// マトリックス計算
	static void ReleaseModelInfo(MODELINFO *pModelInfo);												// モデル情報の破棄
	static void Billboard(D3DXMATRIX *mtxWorld, const D3DXMATRIX mtxView);								// ビルボード化処理
	static void InterpolationRot(D3DXVECTOR3 *rot);														// 回転の補間
	static void InertiaMove(D3DXVECTOR3 *move);															// 慣性の処理
	static void IntegerUpperLimit(int *nValue, int upperLimit);											// int型の上限
	static void IntegerLowerLimit(int *nValue, int lowerLimit);											// int型の下限
	static bool LimitVector3(D3DXVECTOR3 *Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit);		// 上限下限の制限処理
	static void LimitLoopVector3(D3DXVECTOR3 *Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit);	// 上限下限のループ処理
	static void SetFog(const float fStartPos, const float fEndPos, const D3DXCOLOR col);							// フォグの生成
	static void PrintBlockCommentFrame(void)