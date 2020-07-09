//*************************************************************************************************************
//
// 自分のライブラリの処理[Mylibrary.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _MYLIBRARY_H_
#define _MYLIBRARY_H_

#define _CRT_SECURE_NO_WARNINGS

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"
#include "XAudio2.h"
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include <vector>
#include <list>
#include <iostream>
using namespace std;

//-------------------------------------------------------------------------------------------------------------
// ライブラリのリンク
//-------------------------------------------------------------------------------------------------------------
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")

//-------------------------------------------------------------------------------------------------------------
// 全体メモ
//-------------------------------------------------------------------------------------------------------------
/*
（operator）オーバーロードできる演算子
	+		-		*		/		%		^
	!		=		<		>		+=		-=
	^=		&=		|=		<<		>>		<<=
	<=		>=		&&		||		++		--
	( )		[ ]		new		delete	&		|
	~		*=		/=		%=		>>=		==
	!=		,		->		->*
*/
/*
	* [pIt] iteratorの略(ポインタ)
	* [src] sourceの略
	* [rhs] right-hand sideの略
	* [lhs] left-hand sideの略
	* [str] stringの略
	* [tmp] temporaryの略
	* [b]   bool型の略
	* [t]   true(真)の略
	* [f]   false(偽)の略
	* [o]   outputの略
	* [r]   resourceの略
*/

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define MYLIB_NORMAL			 0					// 正常に動いたとき
#define MYLIB_ERROR				-1					// 失敗した時
#define MYLIB_STRINGSIZE		128					// 文字列の基本の長さ

#define MYLIB_PNG				".png" ,".PNG"
#define MYLIB_JPG				".jpg" ,".JPG"
#define MYLIB_JPEG				".jpeg" ,".JPEG"
#define MYLIB_IMAGE_EXTENSION	MYLIB_PNG ,MYLIB_JPG ,MYLIB_JPEG

#define MYLIB_TXT				".txt" ,".TXT"
#define MYLIB_INI				".ini" ,".INI"
#define MYLIB_TXT_EXTENSION		MYLIB_TXT ,MYLIB_INI

#define MYLIB_X					".x" ,".X"

#define MYLIB_CLEARARRAYONDECLA	{ NULL }						// 宣言時の配列をクリア

#define MYLIB_INITSTRUCT_WITHCONST	{ }							// コンストラクタのある構造体の初期化

#define MYLIB_D3DXCOR_UNSET			D3DXCOLOR(0.0f,0.0f,0.0f,0.0f)	// 色設定なし
#define MYLIB_D3DXCOR_SET			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)	// 色設定

#define MYLIB_3DVECTOR_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)		// 3Dベクトルをゼロに設定
#define MYLIB_SCALE_UNSET			D3DXVECTOR3(1.0f,1.0f,1.0f)		// スケールの初期化
#define MYLIB_VEC3_XONE				D3DXVECTOR3(1.0f,0.0f,0.0f)		// 3DベクトルXだけ1.0f
#define MYLIB_VEC3_YONE				D3DXVECTOR3(0.0f,1.0f,0.0f)		// 3DベクトルYだけ1.0f
#define MYLIB_VEC3_ZONE				D3DXVECTOR3(0.0f,0.0f,1.0f)		// 3DベクトルZだけ1.0f
#define MYLIB_VEC3_UNSET			D3DXVECTOR3(0.0f,0.0f,0.0f)		// 3Dベクトルの初期化

#define MYLIB_2DVECTOR_ZERO			D3DXVECTOR2(0.0f,0.0f)			// 2Dベクトルをゼロに設定
#define MYLIB_VEC2_XONE				D3DXVECTOR2(1.0f,0.0f)			// 2DベクトルXだけ1.0f
#define MYLIB_VEC2_YONE				D3DXVECTOR2(0.0f,1.0f)			// 2DベクトルYだけ1.0f
#define MYLIB_VEC2_UNSET			D3DXVECTOR2(0.0f,0.0f)			// 2Dベクトルの初期化

#define MYLIB_INT2_UNSET			INTEGER2(0,0)					// INT2設定なし

#define MYLIB_RANGE_UNSET			RANGE(0,0)						// レンジの設定なし

#define MYLIB_FLOAT_UNSET			(0.0f)							// float型の初期化
#define MYLIB_INT_UNSET				(0)								// int型の初期化
#define MYLIB_INT_NOELEM			(-1)							// int型要素なし
#define MYLIB_CHAR_UNSET			'\0'							// char型の初期化(文字列の一番目の初期化)
#define MYLIB_STRING_UNSET			{ 0 }							// 文字列の初期化

#define MYLIB_HALF_SIZE				(0.5f)							// 半分のサイズ
#define MYLIB_ONEQUARTER_SIZE		(0.25f)							// 4分の1のサイズ
#define MYLIB_THREEQUARTERS_SIZE	(0.75f)							// 4分の3のサイズ

#define MYLIB_VSHORT_UNSET			(0x000)							// 1バイト分の変数設定なし

#define MYLIB_ERRORCOUNT_MAX		(1048576)						// エラーカウント

#define MYLIB_VSHORT_UNSET			(0x000)							// 

#define MYLIB_HALFSECONDCOUNT		(30)							// 1フレームカウント毎時のカウント

#define MYLIB_D3DXCORR_BIT			(0)								// 赤
#define MYLIB_D3DXCORG_BIT			(8)								// 緑
#define MYLIB_D3DXCORB_BIT			(16)							// 青
#define MYLIB_D3DXCORA_BIT			(24)							// 透明度

#define MYLIB_S8BYTES				(64)							// 文字列8バイト分
#define MYLIB_S4BYTES				(32)							// 文字列4バイト分

#define MYLIB_BRACKETS_L					"========================================================== \n"		// 区切りの括弧(大)
#define MYLIB_BRACKETS_M					"---------------------------------------------------------- \n"		// 区切りの括弧(中)
#define MYLIB_BRACKETS_S					"#*\n"																// 区切りの括弧(小)

//-------------------------------------------------------------------------------------------------------------
// マクロ関数
//-------------------------------------------------------------------------------------------------------------
# ifdef _DEBUG
#define Mybfunc_Debug_brackets_L		cout << MYLIB_BRACKETS_L			// 区切りの括弧(大)の出力
#define Mybfunc_Debug_brackets_M		cout << MYLIB_BRACKETS_M			// 区切りの括弧(中)の出力
#define Mybfunc_Debug_brackets_S		cout << MYLIB_BRACKETS_S			// 区切りの括弧(小)の出力
#define Mybfunc_Debug_start(str)		cout << str << " == start!!\n"		// 開始(start!!)の出力
#define Mybfunc_Debug_end(str)			cout << str << " == end!!\n"		// 終了(end!!)の出力
#define Mybfunc_Debug_open(str)			cout << str << " == open!!\n"		// 開いた(open!!)の出力
#define Mybfunc_Debug_openfailed(str)	cout << str << " == open failed!!\n"// 開くの失敗(open failed!!)の出力
#define Mybfunc_Debug_close(str)		cout << str << " == close!!\n"		// 閉じる(close!!)の出力
#endif
#define Mybfunc_gate(b)					if(b)								// 扉:真の時 ※使わない
#define Mybfunc_iif(b)					(b == true) ? true: false			// 条件文から真偽を返す
#define Mybfunc_iifEx(b ,t ,f)			(b == true) ? t: f					// 条件文から真偽を返す(真偽引数)
#define Mybfunc_WhenBiggerSet(o ,src)	if(o > src) {o = src;}				// sよりoが大きいとき設定する
#define Mybfunc_WhenSmallerSet(o ,src)	if(o < src) {o = src;}				// sよりoが小さいとき設定する
#define Mybfunc_swap(lhs,rhs,tmp)		do{tmp = rhs; rhs = lhs; lhs = tmp}while(false);	// 交換

namespace ikuminLib
{
//-------------------------------------------------------------------------------------------------------------
// 列挙型定義
//-------------------------------------------------------------------------------------------------------------
// 頂点点の原点(対キーパッド)
typedef enum
{
	ORIGINVERTEXTYPE_NONE = 0,			// なし
	ORIGINVERTEXTYPE_LOWERLEFT,			// 左下
	ORIGINVERTEXTYPE_LOWERCENTER,		// 中央下
	ORIGINVERTEXTYPE_LOWERRIGHT,		// 右下
	ORIGINVERTEXTYPE_CENTERLEFT,		// 左中央
	ORIGINVERTEXTYPE_CENTER,			// 中央
	ORIGINVERTEXTYPE_CENTERRIGHT,		// 右中央
	ORIGINVERTEXTYPE_UPPERLEFT,			// 左上
	ORIGINVERTEXTYPE_UPPERCENTER,		// 中央上
	ORIGINVERTEXTYPE_UPPERRIGHT,		// 右上
	ORIGINVERTEXTYPE_ROTATION = 10,		// 回転中央
	ORIGINVERTEXTYPE_ROTLOWERLEFT,		// 回転左下
	ORIGINVERTEXTYPE_ROTLOWERCENTER,	// 回転中央下
	ORIGINVERTEXTYPE_ROTLOWERRIGHT,		// 回転右下
	ORIGINVERTEXTYPE_ROTCENTERLEFT,		// 回転左中央
	ORIGINVERTEXTYPE_ROTCENTER,			// 回転中央
	ORIGINVERTEXTYPE_ROTCENTERRIGHT,	// 回転右中央
	ORIGINVERTEXTYPE_ROTUPPERLEFT,		// 回転左上
	ORIGINVERTEXTYPE_ROTUPPERCENTER,	// 回転中央上
	ORIGINVERTEXTYPE_ROTUPPERRIGHT,		// 回転右上
	ORIGINVERTEXTYPE_MAX				// 最大数
} ORIGINVERTEXTYPE;

// 演算子ID
typedef enum
{
	OPERATOR_ID_NONE = -1,				// 無し
	OPERATOR_ID_ADDITION,				// 加算
	OPERATOR_ID_SUBTRACTION,			// 減算
	OPERATOR_ID_MULTIPLICATION,			// 乗算
	OPERATOR_ID_DIVISION,				// 除算
	OPERATOR_ID_MAX						// 最大数
} OPERATOR_ID;


//-------------------------------------------------------------------------------------------------------------
// 型、構造体定義
//-------------------------------------------------------------------------------------------------------------
typedef const char *    CONST_STRING;	// 変更不可文字列
typedef char *          STRING;			// 文字列

typedef signed char     VSHORT;			// 符号あり1バイト分の変数
typedef unsigned char   UNSIG_VSHORT;	// 符号なし1バイト分の変数

/* * 入力キーのセル */
typedef struct _INPUTKEYCELL
{
	int             nKey;		// キー
	int             nData;		// データ
	_INPUTKEYCELL*  pNext;		// 次のデータポインタ
}INPUTKEYCELL;

/* * 3Dの線分情報 */
typedef struct LINESEGMENT3D
{
public:
	LINESEGMENT3D() {};
	LINESEGMENT3D(D3DXVECTOR3 &start, D3DXVECTOR3 &vector);
	/* * 単項演算子 + */
	LINESEGMENT3D operator + (void) const;
	/* * 単項演算子 - */
	LINESEGMENT3D operator - (void) const;

	/* * 二項演算子 + */
	inline LINESEGMENT3D operator + (CONST LINESEGMENT3D& rhs) const;
	/* * 二項演算子 - */
	inline LINESEGMENT3D operator - (CONST LINESEGMENT3D& rhs) const;
	/* * 二項演算子 * */
	inline LINESEGMENT3D operator * (CONST LINESEGMENT3D& rhs) const;
	/* * 二項演算子 / */
	inline LINESEGMENT3D operator / (CONST LINESEGMENT3D& rhs) const;

	/* * 比較演算子 == */
	inline bool operator == (CONST LINESEGMENT3D& rhs) const;
	/* * 比較演算子 != */
	inline bool operator != (CONST LINESEGMENT3D& rhs) const;

	/* * 代入演算子 += */
	inline LINESEGMENT3D& operator += (CONST LINESEGMENT3D& rhs);
	/* * 代入演算子 -= */
	inline LINESEGMENT3D& operator -= (CONST LINESEGMENT3D& rhs);
	/* * 代入演算子 *= */
	inline LINESEGMENT3D& operator *= (CONST LINESEGMENT3D& rhs);
	/* * 代入演算子 /= */
	inline LINESEGMENT3D& operator /= (CONST LINESEGMENT3D& rhs);

	/* メンバ変数*/
	D3DXVECTOR3 start;			// 始点
	D3DXVECTOR3 vector;			// ベクトル(方向、長さ)正規化ダメ

}LINESEGMENT3D, *PLINESEGMENT3D;

/* * 2Dの線分情報 */
typedef struct LINESEGMENT2D
{
public:
	LINESEGMENT2D() {}
	LINESEGMENT2D(D3DXVECTOR2 start, D3DXVECTOR2 vector)
	{
		this->start = start;
		this->vector = vector;
	}
	D3DXVECTOR2 start;		// 始点
	D3DXVECTOR2 vector;		// ベクトル(方向、長さ)正規化ダメ
}LINESEGMENT2D, *PLINESEGMENT2D;

/* *タイマー情報 */
typedef struct _TIMER_INFO
{
	_TIMER_INFO() {}
	int nStart;		// 開始
	int nEnd;		// 終了
}TIMER_INFO;

/* *
* テクスチャのUV座標の設定用の情報
* Previous version TEXTUREVARIABLES
*/
typedef struct _SETING_UV
{
	_SETING_UV() {}
	_SETING_UV(int nCntU, int nNumU, int nCntV, int nNumV)
	{
		this->nCntU = nCntU;
		this->nNumU = nNumU;
		this->nCntV = nCntV;
		this->nNumV = nNumV;
	}
	int nCntU;	// 横のカウント
	int nNumU;	// 横の個数
	int nCntV;	// 縦のカウント
	int nNumV;	// 縦の個数
}SETING_UV ;

/* *
* テクスチャアニメーション情報
* Previous version ANIMTEXVARIABLES
*/
typedef struct _TEXTUREANIMEINFO
{
	_TEXTUREANIMEINFO() {}
	_TEXTUREANIMEINFO(int nFrame, int nFrameMax, int nIteration)
	{
		this->nFrame = nFrame;
		this->nFrameMax = nFrameMax;
		this->nIteration = nIteration;
	}
	int nFrame;		// フレーム
	int nFrameMax;	// 最大フレーム数
	int nIteration;	// 切り返し(1か-1)
}TEXTUREANIMEINFO;

/* * 範囲 */
typedef struct _RANGE
{
	_RANGE() {}
	_RANGE(int nMax, int nMin)
	{
		this->nMax = nMax;
		this->nMin = nMin;
	}
	//_RANGE(INTEGER2&)
	//RANGE& operator = (INTEGER2&);


	int nMax;	// 最大値
	int nMin;	// 最小値
}RANGE;

/* * int型2つ分 */
typedef struct INTEGER2
{
public:
	INTEGER2() {}
	INTEGER2(int nX, int nY)
	{
		this->nX = nX;
		this->nY = nY;
	}

	int nX;	// 最大値
	int nY;	// 最小値
}INTEGER2, *PINTEGER2;

/* * int型3つ分 */
typedef struct INTEGER3
{
public:
	INTEGER3() {}
	INTEGER3(int X, int Y, int Z)
	{
		nX = X;
		nY = Y;
		nZ = Z;
	}

	int nX,		// X軸成分
		nY,		// Y軸成分
		nZ;		// Z軸成分
} INTEGER3, *PINTEGER3;

/* * int型4つ分 */
typedef struct INTEGER4
{
public:
	INTEGER4() {}
	INTEGER4(int X, int Y, int Z, int W)
	{
		nX = X;
		nY = Y;
		nZ = Z;
		nW = W;
	}
	int nX,		// X軸成分
		nY,		// Y軸成分
		nZ,		// Z軸成分
		nW;		// W軸成分
} INTEGER4, *PINTEGER4;

/*削除予定*/
typedef struct
{
	char string[MYLIB_S8BYTES];
} ITEMTAG, *PITEMTAG;

typedef struct TRANSFORM
{
	TRANSFORM() {}
	TRANSFORM(D3DXVECTOR3 Possition, D3DXVECTOR3 Rotation, D3DXVECTOR3 scale)
	{
		pos = Possition;
		rot = Rotation;
		scal = scale;
	}
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;		// 回転
	D3DXVECTOR3 scal;		// スケール
	D3DXMATRIX mtxWorld;	// マトリックス
} TRANSFORM, *PTRANSFORM;


/*-- モーションキー --*/
typedef struct
{
	int				nFlame;		// フレーム数
	D3DXVECTOR3		*pos;		// 位置
	D3DXVECTOR3		*rot;		// 回転量
} MOTION_KEY;

/*-- モーション --*/
typedef struct
{
	int			nLoop;			// ループするかどうか
	int			nNumKey;		// キー数
	MOTION_KEY	*MotionKey;		// モーションキー
}MOTION_INFO;


// ゲージ情報用の型
typedef struct GAGE_INFO
{
	GAGE_INFO() {}
	GAGE_INFO(RANGE SourceRange, int nSourceValue, float fSourceCoefficient)
	{
		range = SourceRange;
		nValue = nSourceValue;
		fCoefficient = fSourceCoefficient;
	}
	RANGE		range;			// 範囲
	int			nValue;			// いじる値
	float		fCoefficient;	// 係数
} GAGE_INFO;


typedef struct
{
	UNSIG_VSHORT b;	// 青
	UNSIG_VSHORT g;	// 緑
	UNSIG_VSHORT r;	// 赤
} BGR;

typedef struct
{
	UNSIG_VSHORT b;	// 青
	UNSIG_VSHORT g;	// 緑
	UNSIG_VSHORT r;	// 赤
	UNSIG_VSHORT a;	// 透明度
} BGRA;

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
/* 制作中 ベクター型*/
template<class T>
class MyVector
{
public:
	// 
	// 集合体
	typedef T*       pIterator;
	// 集合体（const）
	typedef const T* pConstIterator;

	// コンストラクタ
	MyVector()
	{
		this->m_nSize = 0;
		this->m_nCapacity = 0;
		this->m_pData = NULL;
	}
	// コンストラクタ
	MyVector(const MyVector<T>& source)
	{
		this->m_nSize = 0;
		this->m_nCapacity = 0;
		this->m_pData = NULL;
		operator=(source);
	}
	// = のオーバーロード
	inline MyVector<T>& operator=(const MyVector<T>& source)
	{
		clear();
		resize(source.this->m_nSize);
		memcpy(this->m_pData, source.m_pData, (size_t)this->m_nSize * sizeof(T));
		return *this;
	}

	// デストラクタ
	inline ~MyVector()
	{
		if (this->m_pData != NULL)
		{
			free(this->m_pData);
			this->m_pData = NULL;
		}
	}

	// コンテナが空かどうかを判定する
	inline bool empty(void) const
	{
		return Mybfunc_iif(this->m_nSize == 0);
	}
	// サイズの取得
	inline int size(void) const
	{
		return this->m_nSize;
	}
	// 内容量の取得
	inline int capacity(void) const
	{
		return this->m_nCapacity;
	}

	// []のオーバーロード
	inline T& operator[](int nArray)
	{
		return this->m_pData[nArray];
	}
	// []のオーバーロード
	inline const T& operator[](int nArray) const
	{
		return this->m_pData[nArray];
	}
	// クリア
	inline void clear(void)
	{
		if (this->m_pData != NULL)
		{
			this->m_nSize = 0;
			this->m_nCapacity = 0;
			free(this->m_pData);
			this->m_pData = NULL;
		}
	}
	// 先頭要素を指すイテレータを取得する。
	inline pIterator begin(void)
	{
		return this->m_pData;
	}
	// 先頭要素を指すイテレータを取得する。
	inline const pIterator begin(void) const
	{
		return this->m_pData;
	}
	// 末尾の次を指すイテレータを取得する
	inline pIterator end(void)
	{
		return this->m_pData + this->m_nSize;
	}
	// 末尾の次を指すイテレータを取得する
	inline pConstIterator end(void) const
	{
		return this->m_pData + this->m_nSize;
	}
	// 先頭要素への参照を取得する。
	inline T& front(void)
	{
		return this->m_pData[0];
	}
	// 先頭要素への参照を取得する。
	inline const T& front(void) const
	{
		return this->m_pData[0];
	}
	// 末尾要素への参照を取得する
	inline T& back(void)
	{
		return this->m_pData[this->m_nSize - 1];
	}
	// 末尾要素への参照を取得する
	inline const T& back(void) const
	{
		return this->m_pData[this->m_nSize - 1];
	}
	// 他のvectorオブジェクトとデータを入れ替える。
	inline void swap(MyVector<T>& rhs)
	{
		// 変数宣言
		int       nRhs_size;		// 右辺のサイズ
		int       nRhs_cap;			// 右辺の内容量
		pIterator pRhs_data;		// 右辺のデータ

									// サイズの交換
		Mybfunc_swap(this->m_nSize, rhs.m_nSize, nRhs_size);
		// 内容量の交換
		Mybfunc_swap(this->m_nCapacity, rhs.m_nCapacity, nRhs_cap);
		// データの交換
		Mybfunc_swap(this->m_pData, rhs.m_pData, pRhs_data);
	}
	// 要素数を変更する
	inline void resize(int nNewSize)
	{
		// 新しいサイズが大きいとき
		if (nNewSize > this->m_nCapacity)
		{
			// 余裕を作る
			reserve(AddCapacity(nNewSize));
			// サイズを設定
			this->m_nSize = nNewSize;
		}
	}
	// 要素数を変更する
	inline void resize(int nNewSize, const T& tValue)
	{
		// 内容量より新しいサイズが大きいとき
		if (nNewSize > this->m_nCapacity)
		{	// 余裕を作る
			reserve(AddCapacity(nNewSize));
		}
		// サイズより新しいサイズが大きいとき
		if (nNewSize > this->m_nSize)
		{
			// 新しい分をコピーする
			for (int nCntData = this->m_nSize; nCntData < nNewSize; nCntData++)
			{
				memcpy(&Data[nCntData], &tValue, sizeof(tValue));
			}
		}
		// サイズを設定
		this->m_nSize = nNewSize;
	}
	// 内容量を変更する
	inline void reserve(int nNewCapacity)
	{
		// 新しい容量が今の容量以下の時処理しない
		if (nNewCapacity <= this->m_nCapacity)
		{
			return;
		}
		// 変数宣言
		pIterator pNewData;	// 新しいデータ
							// 領域の確保
		pNewData = (pIterator)malloc(nNewCapacity * sizeof(T));
		// データがあった時
		if (this->m_pData != NULL)
		{// メモリをコピー
			memcpy(pNewData, this->m_pData, this->m_nSize * sizeof(T));
			// データを開放
			free(this->m_pData);
		}
		// データに新しいデータを代入
		this->m_pData = pNewData;
		// 内容量を設定
		this->m_nCapacity = nNewCapacity;
	}

	// 新たな要素を末尾に追加する
	inline void push_back(const T& tValue)
	{
		// サイズと内容量が同じとき
		if (this->m_nSize == this->m_nCapacity)
		{
			// 余裕を作る
			reserve(AddCapacity(this->m_nSize + 1));
		}
		// 値をコピーする
		memcpy(&this->m_pData[this->m_nSize], &tValue, sizeof(tValue));
		// サイズをインクリメント
		this->m_nSize++;
	}
	// 末尾要素を削除する
	inline void pop_back()
	{
		// 大きさをデクリメント
		this->m_nSize--;
	}

	// 指定した要素を削除する。
	inline pIterator erase(const pIterator pIt)
	{
		// 変数宣言
		const int nOff; //  削除する先頭
		nOff = pIt - this->m_pData;
		// メモリを移動する
		memmove(
			this->m_pData + nOff,
			this->m_pData + nOff + 1,
			((size_t)this->m_nSize - (size_t)nOff - 1) * sizeof(T)
		);
		// サイズをデクリメント
		this->m_nSize--;
		// 削除した次のイテレーターを返す
		return this->m_pData + nOff;
	}
	// 指定した範囲の要素を削除
	inline pIterator erase(pConstIterator pIt, pConstIterator pIt_last)
	{
		// 変数宣言
		const int nCount = pIt_last - pIt;	// 削除するカウント
		const int nOff = pIt - Data;			// 削除する先頭
												// メモリを移動する
		memmove(
			this->m_pData + nOff,
			this->m_pData + nOff + nCount,
			((size_t)this->m_nSize - (size_t)nOff - nCount) * sizeof(T));
		// サイズを減らす
		this->m_nSize -= (int)nCount;
		// 削除した次のポインタを渡す
		return this->m_pData + nOff;
	}

	// 任意の位置に新たな要素を挿入する
	inline pIterator insert(pConstIterator pIt, const T& tValue)
	{
		// 変数宣言
		const int nOff = pIt - this->m_pData;	// 変更する先頭
												// サイズと内容量が同じとき
		if (this->m_nSize == this->m_nCapacity)
		{// 内容量を変更する
			reserve(AddCapacity(this->m_nSize + 1));
		}
		// 削除する先頭よりサイズの方が大きいとき
		if (nOff < (int)this->m_nSize)
		{// メモリを移動する
			memmove(
				this->m_pData + nOff + 1,
				this->m_pData + nOff,
				((size_t)this->m_nSize - (size_t)nOff) * sizeof(T)
			);
		}
		// 変更するメモリにコピー
		memcpy(&this->m_pData[nOff], &tValue, sizeof(tValue));
		// サイズをインクリメント
		this->m_nSize++;
		// 挿入したポインタを返す
		return this->m_pData + nOff;
	}

	// 任意の値が要素内に含まれているか
	inline bool contains(const T& tValue) const
	{
		// 変数宣言
		pConstIterator pData = this->m_pData;						// データポインタ
		pConstIterator pData_end = this->m_pData + this->m_nSize;	// データの終わり
		while (pData < pData_end)
		{
			if (*pData++ == tValue)
			{
				return true;
			}
		}
		return false;
	}
	// 指定された値を検索する
	inline pIterator find(const T& tValue)
	{
		// 変数宣言
		pIterator pData = this->m_pData;							// データポインタ
		pConstIterator pData_end = this->m_pData + this->m_nSize;	// データの終わり
		while (pData < pData_end)
		{
			if (*pData == tValue)
			{
				break;
			}
			else
			{
				++pData;
			}
		}
		return pData;
	}
	// 指定された値を検索する
	inline const pIterator find(const T& tValue) const
	{
		// 変数宣言
		pConstIterator pData = this->m_pData;						// データポインタ
		pConstIterator pData_end = this->m_pData + this->m_nSize;	// データの終わり
		while (pData < pData_end)
		{
			if (*data == tValue)
			{
				break;
			}
			else
			{
				++pData;
			}
		}
		return pData;
	}
	// 指定された値を検索し削除する
	inline bool find_erase(const T& tValue)
	{
		// 変数宣言
		pConstIterator pIt = find(tValue);
		if (pIt < this->m_pData + this->m_nSize)
		{
			erase(pIt);
			return true;
		}
		return false;
	}
	// イテレーターからインデックスにする
	inline int index_from_ptr(pConstIterator pIt) const
	{
		// 変数宣言
		const int nOff = pIt - this->m_pData;
		return nOff;
	}
private:
	// 内容量を増やす_grow_capacity
	inline int AddCapacity(int nSize) const
	{
		// 変数宣言
		int nNewCapacity;	// 新しい容量
							// 新しい容量を設定
		nNewCapacity = Mybfunc_iifEx(
			(this->m_nCapacity != 0),
			(this->m_nCapacity + this->m_nCapacity / 2),
			8);
		// 大きいほうを返す
		return Mybfunc_iifEx(
			(nNewCapacity > this->m_nSize),
			nNewCapacity,
			this->m_nSize);
	}

	int m_nSize;		// サイズ
	int m_nCapacity;	// 内容量
	T*  m_pData;		// データポインタ
};

class CInputkey
{
	/* * コンストラクタ */
	CInputkey() {}
	/* * デストラクタ */
	~CInputkey() {}
	/* * 初期化 */
	void Init(void);
	/* * キーデータの初期化 */
	void KeyInit(void);
	/* * キーデータの生成 */
	void KeyCreate(void);
	/* * キーデータリストの開放 */
	void KeyListRelease(void);
	/* * キーデータの挿入 */
	void KeyInsert(int nData, int nKey);
	/* * キーデータを抜き取る　*/
	void KeyPullout(int nKey);
	/* * キーデータの取得 */
	int GetKeyData(int nKey);
	/* * サイズの取得 */
	int GetSize(void);
private:
	int           m_nSize;		// サイズ（データ数)
	INPUTKEYCELL* m_pKeyTop;	// キーの先頭ポインタ
};

class CString
{
public:
	/* * コンストラクタ */
	CString();
	/* * デストラクタ */
	~CString() {}
	/* * 初期化 */
	inline void Init(void);
	/* * 開放処理 */
	inline void release(void);
	/* * 文字列の設定(設定していた場合は再設定する) */
	inline void Set(STRING string);
	/* * 文字列の取得 */
	inline STRING Get(void);
private:
	STRING m_string;		// 文字列ポインタ
	int m_nStringLength;	// 文字列の長さ
};


class CMylibrary
{
public:
	//----------------------------------------------------------------------------------------------------
	// 2D
	//----------------------------------------------------------------------------------------------------

	//* [contents] 2Dベクトルを生成する{ B - A }
	//* [in] 3DposA、3DposB
	//* [out] 生成した2Dベクトル
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 2Dベクトルを生成する{ B - A }
	//* [in] 2DposA、2DposB
	//* [out] 生成した2Dベクトル
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 2Dベクトルを生成する{ B - A }
	//* [in] 3DposA、2DposB
	//* [out] 生成した2Dベクトル
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 2Dベクトルを生成する{ B - A }
	//* [in] 2DposA、3DposB
	//* [out] 生成した2Dベクトル
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 2Dの外積を生成する
	//* [in] 3DvecA、3DvecB
	//* [return] 生成した2D外積
	static float Create2DCross(CONST D3DXVECTOR3 *vecA, CONST D3DXVECTOR3 *vecB);

	//* [contents] 2Dの外積を生成する
	//* [in] 2DvecA、2DvecB
	//* [return] 生成した2D外積
	static float Create2DCross(CONST D3DXVECTOR2 *vecA, CONST D3DXVECTOR2 *vecB);

	//* [contents] 2Dの内積を生成する
	//* [in] 3DvecA、3DvecB
	//* [return] 生成した2D外積
	static float Create2DDot(CONST D3DXVECTOR3* vecA, CONST D3DXVECTOR3* vecB);

	//* [contents] 2Dの内積を生成する
	//* [in] 2DvecA、2DvecB
	//* [return] 生成した2D外積
	static float Create2DDot(CONST D3DXVECTOR2* vecA, CONST D3DXVECTOR2* vecB);

	//* [contents] 2Dのベクトルの長さを求める
	//* [in] 2Dvec
	//* [return] ベクトルの長さ
	static float Get2DVecLength(D3DXVECTOR2 *vec);

	//* [contents] 2Dの差分の距離
	//* [in] 3D差分
	//* [return] 計算した距離
	static float Get2DToPointDistance(D3DXVECTOR2 *diffPos);

	//* [contents] 2Dの単位ベクトルを生成
	//* [in] 2Dvec
	//* [out] 生成した単位ベクトル
	static void CreateUnitVec2D(D3DXVECTOR2 *Output, D3DXVECTOR2 *Vec);

	//* [contents] 2Dの差分の角度
	//* [in] 3DposA, 3DposB
	//* [out] 計算した角度
	static void Get2DToPointAngle(float *pOutput, D3DXVECTOR3 *posA, D3DXVECTOR3 *posB);

	//* [contents] 2Dの差分の角度
	//* [in] 3D差分
	//* [out] 計算した角度
	static void Get2DToPointAngle(float *pOutput, D3DXVECTOR3 *diffPos);

	//* [contents] 2D内積を用いて直線同士の交点を求める
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D
	//* [out] 交点
	//* [return] bool （true 交わっている、false 交わっていない）
	static bool Get2DIntersectionOfStraightLines(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB, CONST D3DXVECTOR3 *posC, CONST D3DXVECTOR3 *posD);

	//* [contents] 2D外積を用いて線分同士の交点を求める
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D, 法線
	//* [out] 交点
	//* [return] bool （true 交わっている、false 交わっていない）
	static bool Get2DIntersectionOfLineSegments(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB, CONST D3DXVECTOR3 *posC, CONST D3DXVECTOR3 *posD);

	//* [contents] 2D直線同士が交差しているか求める
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D
	//* [out] 交点
	//* [return] bool （true 交わっている、false 交わっていない）
	static bool Get2DIntersectionOfStraightLineSegments(D3DXVECTOR3 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB, CONST D3DXVECTOR3 * posC, CONST D3DXVECTOR3 * posD);

	//* [contents] 2D四角形の当たり判定
	//* [in] 位置A,位置B, 大きさA, 大きさB
	//* [return] bool （true 接触している、false 接触していない）
	static bool Test2DCollisionRect(D3DXVECTOR3 *posA, D3DXVECTOR3 *posB, D3DXVECTOR2 *sizeA = NULL, D3DXVECTOR2 *sizeB = NULL);

	//* [contents] 頂点位置の設定
	//* [in] 原点タイプ, 位置, 大きさ, 半径, 回転量
	//* [out] 頂点位置
	static void SetVatexPostion(D3DXVECTOR3 **ppOut,ORIGINVERTEXTYPE type, D3DXVECTOR3 *pPos, D3DXVECTOR2 *pSize, float fRadius = 0.0f, float fRotation = 0.0f);

	//-------------------------------------------------------------------------------------------------------------
	// 3D
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] 3Dベクトルを生成する
	//* [in] 3DposA、3DposB
	//* [out] 生成した3Dベクトル
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3Dベクトルを生成する
	//* [in] 2DposA、2DposB
	//* [out] 生成した3Dベクトル
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 3Dベクトルを生成する
	//* [in] 2DposA、3DposB
	//* [out] 生成した3Dベクトル
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3Dベクトルを生成する
	//* [in] 3DposA、2DposB
	//* [out] 生成した3Dベクトル
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 3Dの外積を生成する
	//* [in] 3DvecA、3DvecB
	//* [out] 生成した3D外積
	static void Create3DCross(D3DXVECTOR3 *resulut, D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3Dの平面外積を生成する
	//* [in] 3DvecA、3DvecB
	//* [return] 生成した平面3D外積
	static float CreateflatCross(CONST D3DXVECTOR3 *vecA, CONST D3DXVECTOR3 *vecB);

	//* [contents] 3Dの内積を生成する
	//* [in] 3DvecA、3DvecB
	//* [return] 生成した3D内積
	static float Create3DDot(D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3Dの内積を生成する
	//* [in] 3DvecA、3DvecB
	//* [out] 生成した3D内積
	static void Create3DDot(float *out, D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3Dの2D上のベクトルの長さを求める
	//* [in] 3DvecA、3DvecB
	//* [out] 生成した3D内積
	static float GetFlatVecLength(D3DXVECTOR3 *vec);

	//* [contents] 3Dの2D上のベクトルの長さを求める
	//* [in] 3Dvec
	//* [out] 生成したベクトルの長さ
	static float Get3DVecLength(D3DXVECTOR3 *vec);

	//* [contents] 3Dの2点のベクトルの長さを求める
	//* [in] 3DvecA、3DvecB
	//* [return] 生成したベクトルの長さ
	static float Get3DClossLength(D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3Dの2点のベクトルの長さを求める
	//* [in] 3Dvec
	//* [out] 生成したベクトルの長さ
	static void Get3DVecLength(float *out, D3DXVECTOR3 * vec);

	//* [contents] 3Dの2点の外積の長さを求める
	//* [in] 3DposA、3DposB
	//* [out] 生成したベクトルの長さ
	static void Get3DClossLength(float *out, D3DXVECTOR3 *posA, D3DXVECTOR3 *posB);

	//* [contents] 3Dの単位ベクトルを生成
	//* [in] 3Dvec
	//* [out] 生成した単位ベクトル
	static void CreateUnitVector(D3DXVECTOR3 *out, D3DXVECTOR3 *Vec);

	//* [contents] 3Dの頂点間の距離の取得
	//* [in] 3DposA、3DposB
	//* [return] 計算した距離
	static float Get3DToPointDistance(CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3Dの差分の距離
	//* [in] 3D差分
	//* [return] 計算した距離
	static float Get3DToDiffDistance(D3DXVECTOR3 *diffPos);

	//* [contents] 3Dの差分の距離
	//* [in] 3D差分
	//* [out] 計算した距離
	static void Get3DToPointDistance(float *Length, D3DXVECTOR3 *diffPos);

	//* [contents] ポリゴンの裏にいるかテスト
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P)
	//* [return] bool （範囲内 == true 範囲外 == false）
	static bool TestIfPolygonBack(CONST D3DXVECTOR3 & posA, CONST D3DXVECTOR3 & posP, CONST D3DXVECTOR3 & nom);

	//* [contents] ポリゴンの範囲にいるかテスト
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P)
	//* [return] bool （範囲内 == true 範囲外 == false）
	static bool Test3DInsidePolygon(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP);

	//* [contents] ポリゴンの範囲にいるかテスト
	//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P), ポリゴンの法線
	//* [return] bool （範囲内 == true 範囲外 == false）
	static bool Test3DInsidePolygon(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP, CONST D3DXVECTOR3 & nom);

	//* [contents] 直線同士の距離と最短点座標の取得
	//* [in] 位置A , ベクトルA, 位置B, ベクトルB
	//* [out] 距離, 最短点座標A, 最短点座標B
	static bool Calc2LineNearestDistAndPos(D3DXVECTOR3* posA, D3DXVECTOR3* vecA, D3DXVECTOR3* posB, D3DXVECTOR3* vecB, float* fOutDist, D3DXVECTOR3* pOutPosA, D3DXVECTOR3* pOutPosB);

	//* [contents] 外積で内外位置関係を調べる
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] bool （内 == true 外 == false）
	static bool CrossRelationship(D3DXVECTOR3 &posP, D3DXVECTOR3 &posA, D3DXVECTOR3 &posB);

	//* [contents] 内積で表裏位置関係を調べる
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] bool （表 == true 裏 == false）
	static bool DotRelationship(CONST D3DXVECTOR3 &Dot, CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, float fTolerance = 0.0f);

	//* [contents] 2D一辺を内積で位置関係を調べる
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] bool （表 == true 裏 == false）
	static bool OneSideDotRelationship(CONST D3DXVECTOR3 &Dot, CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, float fTolerance = 0.0f);

	//* [contents] 3D点と直線の距離
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] 計算した距離
	static float Get3DLengthDotAndLine(CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] 3D線上の最近点を算出
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [out] 最近点
	static void Get3DNearestPointOnline(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] 3D平面上の点と直線のの距離
	//* [in] 位置P(点P), 頂点位置A, 頂点位置B
	//* [return] 計算した距離
	static float Get3DFlatLengthDotAndLine(CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] ビルボードの設定
	//* [in] デバイスポインタ
	//* [out] マトリックス
	static void SetBillboard(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX *mtxOutput);

	//* [contents] ビルボードロー回転の打ち消しの設定
	//* [in] デバイスポインタ
	//* [out] マトリックス
	static void SetBillboardNotRoll(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX *mtxOutput);

	//-------------------------------------------------------------------------------------------------------------
	// 2,3D
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] 減速運動処理
	//* [in] (減速)係数
	//* [out] 3DVec移動量
	static void SlowingMove(D3DXVECTOR3 *pMove, const float fCoefficient);

	//* [contents] 減速運動処理
	//* [in] (減速)係数
	//* [out] 移動量
	static void SlowingMove(float *pMove, const float fCoefficient);

	//* [contents] 追従移動処理
	//* [in] 追う位置, 追われる位置, (追従)係数
	//* [out] 3DVec移動量
	static void FollowingMove(D3DXVECTOR3 *move, const D3DXVECTOR3 *ChasPos, const D3DXVECTOR3 *ChasedPos, const float *SpeedFactor);

	//* [contents] 点と点の距離を比較する
	//* [in] 距離, ソース（差分の場合追加ソースなし）, 追加ソース
	//* [return] bool （true 設定した距離より近い、false 設定した距離より遠い）
	static bool TestToPointDistance(float fDistance, CONST D3DXVECTOR3 *Source, CONST D3DXVECTOR3 *AddSource = NULL);

	//* [contents] 回転量を直す
	//* [out] 向きor回転量
	static void SetFixTheRotation(float *Rotation);

	//* [contents] 回転量を直す
	//* [out] Vec3向きor回転量
	static void SetVec3FixTheRotation(D3DXVECTOR3 *Rotation);

	//-------------------------------------------------------------------------------------------------------------
	// COLLAR
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] COLORREFからD3DXCOLORに変換する(アルファ値なし)
	//* [in] システムカラー
	//* [out] DirectX3Dカラー
	static void SetD3DXCOLORfromREF_WithoutAlpha(COLORREF *SystemColor, D3DXCOLOR *col);

	//* [contents]  COLORREFからD3DXCOLORに変換する
	//* [in] システムカラー
	//* [out] DirectX3Dカラー
	static void SetD3DXCOLORfromREF(COLORREF *SystemColor, D3DXCOLOR *col);

	//-------------------------------------------------------------------------------------------------------------
	// Misc
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] 桁数を取得する
	//* [in] ソース（数字）
	//* [out] 桁数
	static void GetDigits(int &out, int nSource);

	//* [contents] 桁数を取得する
	//* [in] ソース（数字）
	//* [return] 桁数
	static int GetDigits(int nSource);

	//* [contents] 桁数を取得する(引数あり)
	//* [in] ソース, 数字, 基数（進数）
	//* [out] 桁数
	static void GetDigitsEx(int &out, int nSource, int nRadix);

	//* [contents] 桁数を取得する(引数あり)
	//* [in] ソース, 数字, 基数（進数）
	//* [return] 桁数
	static int GetDigitsEx(int nSource, int nRadix);

	//* [contents] キーIDの参照したキー名を取得
	//* [in] キーID
	//* [out] キー名
	static void GetKeyInputInfo(int nKeyID, STRING KeyName = NULL);

	//* [contents] 繰り上げた数を取得
	//* [in] ソース, 基数
	//* [return] 繰り上げた数
	static int GetRoundedUp(int nSource, int nBase);

	//-------------------------------------------------------------------------------------------------------------
	// FILE
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] テキストファイルから個数を取得する
	//* [in] ファイル名, 項目名
	//* [return] 個数
	static int GetNumberFromText(CONST_STRING TextFile, CONST_STRING SetItem);

	//* [contents] テキストファイルにスクリプトが存在するかチェック
	//* [in] ファイルポインタ
	//* [return] bool (true 存在した、false 存在しない)
	static bool CheckforScript(FILE *pFile);

	//-------------------------------------------------------------------------------------------------------------
	// STRING
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] 文字列の確保、設定
	//* [in] ソース(文字列)
	//* [return] 文字列ポインタ
	static STRING SetStringAlloc(CONST_STRING source);

	//* [contents] 文字列からint型に変換する(文字列から数字を抜き取る)
	//* [in] 文字列
	//* [return] 変換した数字
	static int GetConvertStoI(STRING str);

	//* [contents] int型から文字列に変換する
	//* [in] 変換する値、基数（進数）
	//* [out] 変換した文字列
	void ConvertItoS(STRING pOut, int nVal, int nRadix);

	//* [contents] int型から文字列を生成し、変換する
	//* [in] 基数（進数）
	//* [return] 変換した文字列
	STRING ConvertItoSAlloc(int nVal, int nRadix);

	//* [contents] int型から文字列を生成し、変換する
	//* [in] 変換する値、基数（進数）
	//* [out] 変換した文字列
	void ConvertItoSAlloc(char **ppOut, int nVal, int nRadix);

	//-------------------------------------------------------------------------------------------------------------
	// TEMPLATE
	//-------------------------------------------------------------------------------------------------------------
	/* MEMO
	* エラーを回避するためにinline関数にする
	*/

	// 違う値の時に設定
	// input : (変更するデータ、)
	template <class T>
	inline static bool Setdifferentvalue(T &tData, CONST T tSource)
	{
		if (tData != tSource)
		{
			tData = tSource;
			return true;
		}
		return false;
	}
};

}// namespace ikuminLib

using namespace ikuminLib;

// インライン関数用ファイルをインクルード
#include "Mylibrary.inl"

#endif