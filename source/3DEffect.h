//*************************************************************************************************************
//
// 3Dエフェクト処理[3DEffect.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _3DEFFECT_H_
#define _3DEFFECT_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"
#include "scene.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define _3DEFFE_USEQUANTITY 1000		// 使用するエフェクト個数
#define _3DEFFE_USEVERTEX   4			// 使用する頂点数

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C3DEffect :public CScene
{
public:
	// 列挙型定義
	typedef enum TYPE
	{
		TYPE_NONE = -1,		// 無し
		TYPE_NORMAL,		// 通常
		TYPE_CHAR,			// 文字
		TYPE_MAX			// 最大
	}TYPE;

	typedef enum TEXTURETYPE
	{
		TEXTYPE_NONE = -1,		// 無し
		TEXTYPE_SPARKLE1,		// キラキラ1
		TEXTYPE_SPARKLE2,		// キラキラ2
		TEXTYPE_SPARKLE3,		// キラキラ3
		TEXTYPE_char_ー,		// 文字ー
		TEXTYPE_char_キ,		// 文字キ
		TEXTYPE_char_ギ,		// 文字ギ
		TEXTYPE_char_グ,		// 文字グ
		TEXTYPE_char_ゲ,		// 文字ゲ
		TEXTYPE_char_ゴ,		// 文字ゴ
		TEXTYPE_char_ザ,		// 文字ザ
		TEXTYPE_char_ズ,		// 文字ズ
		TEXTYPE_char_タ,		// 文字タ
		TEXTYPE_char_ダ,		// 文字ダ
		TEXTYPE_char_ツ,		// 文字ツ
		TEXTYPE_char_ド,		// 文字ド
		TEXTYPE_char_ヤ,		// 文字ヤ
		TEXTYPE_char_ン,		// 文字ン
		TEXTYPE_MAX				// 最大
	}TEXTURETYPE;

	typedef struct VIBRATION_OPTION
	{
		VIBRATION_OPTION() {}		// コンストラクタ
		~VIBRATION_OPTION() {}		// デストラクタ
		
		int   nMaxDiviDir;			// ランダムの方向最大分割数
		float fScalDirValue;		// 分割数の縮小値
		int   nMaxDist;				// ランダムの最大分割数
		float fScalDistValue;		// 分割数の縮小値

	}VIBRATION_OPTION;

	typedef struct VIBRATION
	{
		VIBRATION() :VecDir(MYLIB_3DVECTOR_ZERO), fDist(MYLIB_FLOAT_UNSET), bRandDist(false),Pos(MYLIB_3DVECTOR_ZERO){}		// コンストラクタ
		~VIBRATION() {}		// デストラクタ

		D3DXVECTOR3 VecDir;				// 方向ベクトル
		float       fDist;				// 距離
		D3DXVECTOR3 Pos;				// 位置
		bool        bRandDist;			// 距離ランダム化フラグ
		void        Update(void);		// 更新処理
		void        GetVecDir(void);	// 方向ベクトルの取得
		void        GetDist(void);		// 距離の取得
		void        CalPos(void);		// 位置の計算
	}VIBRATION;

	typedef struct SETINGPARAM
	{
		SETINGPARAM() :
			bBillBoard(false),
			type(MYLIB_INT_UNSET),
			nTexType(MYLIB_INT_UNSET),
			pos(MYLIB_3DVECTOR_ZERO),
			pParent(nullptr),
			move(MYLIB_3DVECTOR_ZERO),
			col(MYLIB_D3DXCOR_UNSET),
			fRadius(MYLIB_FLOAT_UNSET),
			nLife(MYLIB_INT_UNSET),
			fGravity(MYLIB_FLOAT_UNSET),
			fRadiusValue(MYLIB_FLOAT_UNSET) {}

		bool        bBillBoard;		// ビルボードフラグ
		int         type;			// エフェクトの種類
		int         nTexType;		// テクスチャの種類
		D3DXVECTOR3 pos;			// 位置
		D3DXVECTOR3 *pParent;		// 親の位置
		D3DXVECTOR3 move;			// 移動量
		D3DXCOLOR   col;			// 色
		float       fRadius;		// 半径
		int         nLife;			// ライフ
		float       fGravity;		// 重力
		float       fRadiusValue;	// 半径の変化値
	} SETINGPARAM;

	// 構造体定義
	typedef struct PARAMETER
	{
		bool        bUse;			// 使用フラグ
		bool        bDisp;			// 描画フラグ
		bool        bBillBoard;		// ビルボードフラグ
		TRANSFORM   Trans;			// トランス情報
		D3DXVECTOR3 *pParent;		// 親の位置
		D3DXVECTOR3 move;			// 移動量
		D3DXCOLOR   col;			// 色
		float       fRadius;		// 半径
		float       fAngle;			// 角度
		float       fRadiusValue;	// 半径の変化値
		float       fAlphaValue;	// アルファ値の変化値
		int         nLife;			// 持ち時間
		int         nTexType;		// テクスチャの種類
		float       fGravity;		// 重力処理
		TYPE        type;			// 種類
		VIBRATION   Vibrat;			// 振動
	}PARAMETER;

	C3DEffect() : CScene(PRIORITY_EFFECT) {};	// コンストラクタ
	~C3DEffect() {};							// デストラクタ

	static void Load(void);						// 読み込み
	static void Unload(void);					// 開放
	void Make(void);							// 作成
	void Init(void);							// 初期化
	void Uninit(void);							// 終了
	void Update(void);							// 更新
	void Draw(void);							// 描画

	static void Set(SETINGPARAM &Seting);		// 位置の設定
private:
	/* メンバ関数 */
	static void CreateTexture(CONST_STRING SetingStr,const int &nCntInfo, LPDIRECT3DDEVICE9 pDevice);	// テクスチャの作成
	HRESULT     MakeVertex(LPDIRECT3DDEVICE9 pDevice);													// 頂点情報の作成
	static void SetVartexSize(VERTEX_3D *pVtx, PARAMETER &Effect);										// 頂点サイズの設定
	static void SetVetexColor(VERTEX_3D *pVtx, PARAMETER &Effect);										// 頂点カラーの設定

	/* メンバ変数 */
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// バッファ情報
	static LPDIRECT3DTEXTURE9      m_pTexInfo[TEXTYPE_MAX];				// テクスチャ情報
	static const float             m_cfBaseAngle;						// 基本角度
	static int                     m_nNumTextureMax;					// テクスチャの最大数
	static PARAMETER               m_EffectPram[_3DEFFE_USEQUANTITY];	// エフェクトのパラメータ
	static VIBRATION_OPTION        m_VibrationOption;					// 振動のオプション

};

#endif