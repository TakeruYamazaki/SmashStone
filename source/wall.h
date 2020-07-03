//*************************************************************************************************************
//
// 壁の処理[wall.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _WALL_H_
#define _WALL_H_

#define _CRT_SECURE_NO_WARNINGS

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define WALL_MAX	(4)		// 壁の最大数

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CWall : public CScene
{
public:

	/* 列挙型定義 */
	// 壁のタイプ
	enum WALLTEX
	{
		WALLTEX_NONE = -1,	// 無し
		WALLTEX_FIELD,		// フィールド
		WALLTEX_MAX			// 最大数
	};

	enum SETINGPOS
	{
		SETINGPOS_NONE =-1,	// 無し
		SETINGPOS_POSIX,	// 正のXの位置
		SETINGPOS_POSIZ,	// 正のZの位置
		SETINGPOS_NEGX,		// 負のXの位置
		SETINGPOS_NEGZ,		// 負のZの位置
		SETINGPOS_MAX		// 最大数
	};
	
	/* 構造体定義 */
	// 壁の単体情報
	typedef struct
	{
		TRANSFORM   trans;	// トランス情報
		D3DXVECTOR3 nor;	// 面の法線情報
	} SINGLEINFO;

	/* メンバ関数 */
	// コンストラクタ
	inline CWall() : CScene() {}
	// デストラクタ
	inline ~CWall() {}

	// 読み込み
	static HRESULT Load(void);
	// 開放
	static void Unload(void);
	// 生成
	static CWall* Create(WALLTEX enmWallTex);

	// 初期化
	void Init(void);
	// 終了
	void Uninit(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// テクスチャタイプの設定
	void BindTexType(WALLTEX enmTexType);
private:
	/* メンバ関数 */
	// 頂点情報の作成
	HRESULT MakeVertex(void);
	/* メンバ変数 */
	static LPDIRECT3DTEXTURE9 m_pAllTexture[WALLTEX_MAX];	// 全てのテクスチャ情報
	static D3DXVECTOR2        m_Length;						// 中心からの距離
	static D3DXVECTOR3        m_CenterPos;					// 中心位置
	static float              m_fHeight;					// 高さ
	LPDIRECT3DTEXTURE9        m_pTexture;					// テクスチャ情報のポインタ
	LPDIRECT3DVERTEXBUFFER9   m_pVtexBuff;					// 頂点バッファのポインタ
	SINGLEINFO                m_SingleInfo[WALL_MAX];		// 壁単体の情報

};



#endif