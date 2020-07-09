//*************************************************************************************************************
//
// ストーン処理[stone.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _STONE_H_
#define _STONE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "sceneX.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#ifndef CSTONE_USE_TEXTURE
//#define CSTONE_USE_TEXTURE	// テクスチャを使う
#endif // !CSTONE_USE_TEXTURE

#ifndef CSTONE_USE_NULLPTR
#define CSTONE_USE_NULLPTR		// nullptrを使う
#endif // CSTONE_USE_NULL

#ifndef CSTONE_UPDATE
#define CSTONE_UPDATE
#endif // !CSTONE_UPDATE

#ifndef CSTONE_DRAW
#define CSTONE_DRAW
#endif // !CSTONE_DRAW


// DEBUG時
#ifdef _DEBUG

#ifndef CSTONE_DEBUG_DRAW
//#define CSTONE_DEBUG_DRAW
#endif // !CSTONE_DEBUG_DRAW

#ifndef CSTONE_LAOD_DEBUG_DRAW
#define CSTONE_LAOD_DEBUG_DRAW
#endif // !CSTONE_LAOD_DEBUG_DRAW

//#undef CSTONE_UPDATE
//#undef CSTONE_DRAW
#endif // _DEBUG


//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CStone : public CSceneX
{
public:
	typedef enum
	{
		STONE_ID_NONE = -1,
		STONE_ID_DEFAULT,
		STONE_ID_MAX
	} STONE_ID;

	// モデル情報
	typedef struct
	{
		LPD3DXMESH			pMesh;			// メッシュ情報のポインタ
		LPD3DXBUFFER		pBuffMat;		// マテリアル情報のポインタ
		DWORD				nNumMat;		// マテリアルの数
		D3DXVECTOR3			vtxMax;			// 最大頂点
		D3DXVECTOR3			vtxMin;			// 最小頂点
		float				*pAlphaValue;	// アルファ値のポインタ
#ifdef CSTONE_USE_TEXTURE
		int					*pTextureID;	// テクスチャID
		LPDIRECT3DTEXTURE9	*ppTexture;		// テクスチャ情報
		int					nNumTexture;	// テクスチャ数
#endif
	} MODEL_INFO;

	// コンストラクタ
	CStone();
	// 引数ありコンストラクタ
	CStone(PRIORITY type);
	// デストラクタ
	~CStone() {};
	// 読み込み(マネージャーの初期化に入れる)
	static HRESULT Load(void);
	// 開放(マネージャーの終了処理に入れる)
	static void Unload(void);
	// 資源の調達
	static CString *GetResource(void);

	// 生成
	static CStone* Create(
		CONST STONE_ID     eumID = STONE_ID_NONE,
		CONST D3DXVECTOR3  &pos = MYLIB_3DVECTOR_ZERO
	);

	// 初期化
	void Init(void);
	// 終了
	void Uninit(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);

	// 入手する
	void Catch(void);

private:
	/* メンバ関数 */
	// 最大最小の頂点位置の取得
	static void GetMaxMinVertices(int nIndex);
#ifdef CSTONE_USE_TEXTURE
	// Xファイルから取得したテクスチャからIDの取得する
	static void CreateTexturefrom_Xfile(const int nIndex, CONST STRING sFileName);
#endif
	/* メンバ変数　*/
	static MODEL_INFO* m_pAllStoneTypeInfo;		// ストーンの全てのモデル情報
	static int         m_nNumTypeAll;			// ストーンの種類数
	static const float m_fDoublePi;				// 円周率の2倍
	STONE_ID           m_enmStoneID;			// ストーンID
	unsigned int       m_fCntShake;				// ゆれるカウント
	MODELINFO          m_pModelInfo;			// モデルを参照するための情報
	int                m_nBoxClliderID;			// ボックスコライダーID
#ifdef CSTONE_DEBUG_DRAW
	static int         m_nNumAll;				// 全ての個数
	int                m_nNumID;				// 個数ID(何個目)
#endif // CSTONE_DEBUG_DRAW
};




#endif