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
	CPolygonCollider() : CScene(PRIORITY_COLLISION) {}			// コンストラクタ
	~CPolygonCollider() {}										// デストラクタ

	static CPolygonCollider* Create(D3DXVECTOR3 *pVertexsPos);	// 生成

	void Init(void);											// 初期化
	void Uninit(void);											// 終了
	void Update(void) {};										// 更新
	void Draw(void);											// 描画

	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection);		// 衝突判定

	bool Test3DInsidePolygon(D3DXVECTOR3 *pPos);				// ポリゴンの中にいるかテスト

private:
	void SetVtxPos(D3DXVECTOR3 *pVertexsPos);					// 頂点位置の設定
	HRESULT MakeVertex(void);									// 頂点情報の作成

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;							// バッファ
	D3DXVECTOR3             m_VtxPos[POLYCOLLI_USE_VTX];		// 頂点位置
	D3DXVECTOR3             m_SurfaceNor;						// 面の法線
};

#endif