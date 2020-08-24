//*************************************************************************************************************
//
// ポリゴンコライダー処理[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _POLYGONCOLLIDER_H_
#define _POLYGONCOLLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#define POLYCOLLI_USE_VTX	4

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CPolygonCollider : public CScene
{
public:
	enum
	{
		MAP_TYPE_NONE = -1,	// 無し
		MAP_TYPE_FIELD,		// 広場
		MAP_TYPE_MAX		// 最大
	};


	enum
	{
		POLYCOLLI_STAIRS = 0,		// 階段
		POLYCOLLI_STAIRS_TOP,		// 階段の上部
		POLYCOLLI_LONGSTAIRS,		// 緩やかな階段
		POLYCOLLI_LONGSTAIRS_TOP,	// 緩やかな階段の上部
		POLYCOLLI_MAX				// 最大
	};

	typedef struct VTXPOS
	{
		D3DXVECTOR3 vtx_0;			// 頂点0
		D3DXVECTOR3 vtx_1;			// 頂点1
		D3DXVECTOR3 vtx_2;			// 頂点2
		D3DXVECTOR3 vtx_3;			// 頂点3
	}VTXPOS;
	
	typedef struct MAP_POLYCOLII
	{
		VTXPOS *pFieldVtxPos;		// フィールドの頂点座標
	} MAP_POLYCOLII;

	

	CPolygonCollider() : CScene(PRIORITY_COLLISION) {}			// コンストラクタ
	~CPolygonCollider() {}										// デストラクタ

	static void CreateDateBase(void);							// データベースの生成
	static void DeleteDateBase(void);							// データベースの破棄
	static CPolygonCollider* Create(int nType);					// 生成

	void Init(void);											// 初期化
	void Uninit(void);											// 終了
	void Update(void) {};										// 更新
	void Draw(void);											// 描画

	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection);		// 衝突判定

	bool Test3DInsidePolygon(D3DXVECTOR3 *pPos);				// ポリゴンの中にいるかテスト

private:
	void SetVtxPos(D3DXVECTOR3 *pVertexsPos);												// 頂点位置の設定
	HRESULT MakeVertex(void);																// 頂点情報の作成
	static D3DXVECTOR3 m_conSetingVtxPos[MAP_TYPE_MAX][POLYCOLLI_MAX][POLYCOLLI_USE_VTX];	// 設定用頂点位置
	static MAP_POLYCOLII m_PolyPos[MAP_TYPE_MAX];											// ポリゴンの座標
	LPDIRECT3DVERTEXBUFFER9  m_pVtxBuff;													// バッファ
	D3DXVECTOR3              m_VtxPos[POLYCOLLI_USE_VTX];									// 頂点位置
	D3DXVECTOR3              m_SurfaceNor;													// 面の法線
};

#endif