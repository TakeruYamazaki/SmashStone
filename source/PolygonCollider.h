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
		POLYCOLLI_STAIRS = 0,		// 階段
		POLYCOLLI_STAIRS_TOP,		// 階段の上部
		POLYCOLLI_LONGSTAIRS,		// 緩やかな階段
		POLYCOLLI_LONGSTAIRS_TOP,	// 緩やかな階段の上部
		POLYCOLLI_MAX				// 最大
	};



	CPolygonCollider() : CScene(PRIORITY_COLLISION) {}			// コンストラクタ
	~CPolygonCollider() {}										// デストラクタ

	static CPolygonCollider* Create(int nType);					// 生成

	void Init(void);											// 初期化
	void Uninit(void);											// 終了
	void Update(void) {};										// 更新
	void Draw(void);											// 描画

	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection);		// 衝突判定

	bool Test3DInsidePolygon(D3DXVECTOR3 *pPos);				// ポリゴンの中にいるかテスト

private:
	void SetVtxPos(D3DXVECTOR3 *pVertexsPos);					// 頂点位置の設定
	HRESULT MakeVertex(void);									// 頂点情報の作成
	
	static D3DXVECTOR3 m_conSetingVtxPos[POLYCOLLI_MAX][POLYCOLLI_USE_VTX];	// 設定用頂点位置
	LPDIRECT3DVERTEXBUFFER9  m_pVtxBuff;											// バッファ
	D3DXVECTOR3              m_VtxPos[POLYCOLLI_USE_VTX];							// 頂点位置
	D3DXVECTOR3              m_SurfaceNor;											// 面の法線
};

#endif