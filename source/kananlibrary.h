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
#include <string>	// string型などの文字列の操作に必要

// ===================================================================
// マクロ定義
// ===================================================================
// ベクトル
#define ZeroVector2		(D3DXVECTOR2(0.0f, 0.0f))
#define OneVector2		(D3DXVECTOR2(1.0f, 1.0f))
#define ZeroVector3		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
#define OneVector3		(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
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
#define MAX_TEXT			(128)									// テキストの最大数
#define COMMENT_BLOCK		("#============================================================\n")	// ブロックコメントの枠
#define COMMENT_NEW_LINE	("\n")									// 改行
#define COMMENT_BLOCK_LINE	("#\n")									// コメント改行
#define COMMENT_AUTHOR		("# Author : KANAN NAGANAWA\n")			// 著者名

// 物理
#define INERTIA				(0.2f)								// 慣性
#define GRAVITY				(0.2f)								// 重力

// 時間
#define ONE_SECOND_FPS		(60)								// 一秒毎のフレーム数

// ===================================================================
// 構造体定義
// ===================================================================
typedef struct INTEGER2
{	// int型の構造体
	int x;
	int y;

	// コンストラクタ
	INTEGER2() {};
	INTEGER2(int nX, int nY)
	{
		x = nX;
		y = nY;
	}
} INTEGER2;

typedef struct MODEL_VTX
{
	D3DXVECTOR3 VtxMax;	// 頂点の最大値
	D3DXVECTOR3 VtxMin;	// 頂点の最小値

						// コンストラクタ
	MODEL_VTX() {};
	MODEL_VTX(D3DXVECTOR3 Max, D3DXVECTOR3 Min)
	{
		VtxMax = Max;
		VtxMin = Min;
	}
} MODEL_VTX;

typedef struct
{	// モデル一つ一つの情報
	char			cModelName[MAX_TEXT];	// モデルのファイル名
	char			cTextureName[MAX_TEXT];	// テクスチャのファイル名
	LPD3DXMESH		mesh;				// メッシュ
	LPD3DXBUFFER	matBuff;			// 頂点情報
	DWORD			matNum;				// マテリアル数
	LPDIRECT3DTEXTURE9 pTexture;		// テクスチャ
	bool			bTex;				// テクスチャがあるか
} MODELINFO;

typedef struct
{	// モデルの詳細を格納する構造体
	MODELINFO modelInfo;	// モデル情報
	MODEL_VTX modelVtx;		// モデル頂点情報
} OBJINFO;

typedef struct
{	// キャラクター一つ一つが持つモデルの情報
	MODELINFO	*pModelInfo;	// モデル名やマテリアルなどのモデル情報
	int			nNumParts;		// モデルのパーツ数
	int			nNumTexture;	// テクスチャ数
} MODELCHARACTER;

typedef enum
{	// 移動用のキー
	ONE_UP = DIK_W,			// 上入力
	ONE_DOWN = DIK_S,		// 下入力
	ONE_LEFT = DIK_A,		// 左入力
	ONE_RIGHT = DIK_D,		// 右入力
	ONE_JUMP = DIK_SPACE,	// ジャンプ入力
	ONE_ATTACK = DIK_N		// 攻撃入力
} PLAYER_MOVE_KEY_ONE;

typedef enum
{	// 移動用のキー
	TWO_UP	= DIK_UPARROW,			// 上入力
	TWO_DOWN = DIK_DOWNARROW,		// 下入力
	TWO_LEFT = DIK_LEFTARROW,		// 左入力
	TWO_RIGHT = DIK_RIGHTARROW,		// 右入力
	TWO_JUMP = DIK_NUMPAD0,			// ジャンプ入力
	TWO_ATTACK = DIK_NUMPAD2		// 攻撃入力
} PLAYER_MOVE_KEY_TWO;

typedef enum
{	// キャラクターの種類
	CHARACTER_NONE,			// 何もない
	CHARACTER_1YASU = 0,	// 1ヤス
	CHARACTER_1YASU_TRANS,	// 1ヤス変身
	CHARACTER_2YASU,		// 2ヤス
	CHARACTER_2YASU_TRANS,	// 2ヤス変身
	CHARACTER_MAX			// キャラクターの最大数
} CHARACTER_TYPE;

typedef enum
{	// プレイヤー数
	PLAYER_ONE = 0,	// 1P
	PLAYER_TWO,		// 2P
	PLAYER_MAX		// 最大数
} NUM_PLAYER;

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

	static void CalcMatrix(D3DXMATRIX *pMtx, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot);				// マトリックス計算
	static void CalcShadowMatrix(D3DXMATRIX & mtxShadow, D3DXVECTOR3 const & pos, D3DXVECTOR3 const & nor);	// 影のマトリックス計算

	static MODEL_VTX OutputModelVtxColl(LPD3DXMESH mesh);												// モデルのメッシュから頂点座標の最大・最小を出力

	static void ReleaseModelInfo(MODELINFO *pModelInfo);												// モデル情報の破棄
	static void Billboard(D3DXMATRIX *mtxWorld, const D3DXMATRIX mtxView);								// ビルボード化処理
	static void InterpolationRot(D3DXVECTOR3 *rot);														// 回転の補間
	static void InterpolationFloat(float & Float);														// float型の補間
	static void InertiaMove(D3DXVECTOR3 *move);															// 慣性の処理
	static void InertiaMoveXZ(D3DXVECTOR3 *move);															// 慣性の処理
	static void Gravity(float & fMoveY);																	// 重力の処理
	static void IntegerUpperLimit(int *nValue, int upperLimit);											// int型の上限
	static void IntegerLowerLimit(int *nValue, int lowerLimit);											// int型の下限
	static bool FloatLowerLimit(float *fValue, const float &lowerLimit);								// float型の下限
	static bool LimitVector3(D3DXVECTOR3 &Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit);		// 上限下限の制限処理
	static bool LimitVector2(D3DXVECTOR2 &Value, const D3DXVECTOR2 lowerLimit, const D3DXVECTOR2 upperLimit);		// 上限下限の制限処理
	static void LimitLoopVector3(D3DXVECTOR3 *Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit);	// 上限下限のループ処理
	static void SetFog(const float fStartPos, const float fEndPos, const D3DXCOLOR col);							// フォグの生成
	static void LimitColor(D3DXCOLOR &col, const D3DXCOLOR lowerLimit, const D3DXCOLOR upperLimit);		// カラーの下限上限

	static void CreateModelInfo(MODELINFO *pModelInfo);													// モデルの生成

	static void PrintBlockCommentFrame(void);															// ブロックコメント枠の表示
	static void StartBlockComment(const std::string &str);												// ブロックコメントの開始
	static void EndBlockComment(const std::string &str);												// ブロックコメントの終了

	static HRESULT Up(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// 上入力
	static HRESULT Down(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// 下入力
	static HRESULT Left(CInputKeyboard	*Key,	CInputGamepad *pGamepad);	// 左入力
	static HRESULT Right(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// 右入力
	static HRESULT Decide(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// 決定入力
	static HRESULT Return(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// 戻り入力
	static HRESULT Pause(CInputKeyboard *Key,	CInputGamepad *pGamepad);	// ポーズ入力
	static HRESULT GetMoveByKeyboard(CInputKeyboard *pKey, int nPlayer);	// キーボードで移動入力しているか取得
	static HRESULT GetMoveByGamepad(CInputGamepad *pGamepad);					// ゲームパッドで相殺しているか取得

	static float OutputSqrt(D3DXVECTOR3 difpos);				// 平方根計算
	static float OutputDistance(D3DXVECTOR3 difpos);			// 距離計算

#ifdef _DEBUG
	static void ShowDebugInfo(void);					// ImGuiの更新
	static void ShowOffsetInfo(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move);					// 座標などの情報
	static bool GetWire(void) { return m_WireFrame; }	// ワイヤーフレーム描画の取得
#endif

private:

#ifdef _DEBUG
	static bool m_WireFrame;			// ワイヤーフレーム描画の切り替え用
	static int m_nCulling;				// カリングの切り替え用
#endif

};
#endif