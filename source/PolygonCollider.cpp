//*************************************************************************************************************
//
// ポリゴンコライダー処理[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "PolygonCollider.h"
#include "renderer.h"
#include "debugProc.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define POLYCOLLI_COL	D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)

#define POLYCOLLI_STAIRS_POS		D3DXVECTOR3(145.0f,42.0f,-95.0f)

#define POLYCOLLI_STAIRS_HALFSIZE_X 66.0f
#define POLYCOLLI_STAIRS_HALFSIZE_Y 45.0f
#define POLYCOLLI_STAIRS_HALFSIZE_Z 80.0f

#define POLYCOLLI_STAIRS_TOP_POS	D3DXVECTOR3(115.0f,45.0f,0.0f)+POLYCOLLI_STAIRS_POS

#define POLYCOLLI_STAIRS_TOP_HALFSIZE_X 50.0f
#define POLYCOLLI_STAIRS_TOP_HALFSIZE_Z POLYCOLLI_STAIRS_HALFSIZE_Z

#define POLYCOLLI_LONGSTAIRS_POS	D3DXVECTOR3(0.0f,-12.0f,-244.0f)

#define POLYCOLLI_LONGSTAIRS_HALFSIZE_X 300.0f
#define POLYCOLLI_LONGSTAIRS_HALFSIZE_Y 12.0f
#define POLYCOLLI_LONGSTAIRS_HALFSIZE_Z 27.0f

#define POLYCOLLI_STAIRS_TOP_POS	D3DXVECTOR3(115.0f,45.0f,0.0f)+POLYCOLLI_STAIRS_POS

#define POLYCOLLI_STAIRS_TOP_HALFSIZE_X 50.0f
#define POLYCOLLI_STAIRS_TOP_HALFSIZE_Z POLYCOLLI_STAIRS_HALFSIZE_Z

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 CPolygonCollider::m_conSetingVtxPos[MAP_TYPE_MAX][POLYCOLLI_MAX][POLYCOLLI_USE_VTX] = 	// 設定用頂点位置
{
	{// 広場
		{// 階段
			POLYCOLLI_STAIRS_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_HALFSIZE_X,POLYCOLLI_STAIRS_HALFSIZE_Y,-POLYCOLLI_STAIRS_HALFSIZE_Z),
			POLYCOLLI_STAIRS_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_HALFSIZE_X,-POLYCOLLI_STAIRS_HALFSIZE_Y,-POLYCOLLI_STAIRS_HALFSIZE_Z),
			POLYCOLLI_STAIRS_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_HALFSIZE_X,POLYCOLLI_STAIRS_HALFSIZE_Y,POLYCOLLI_STAIRS_HALFSIZE_Z),
			POLYCOLLI_STAIRS_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_HALFSIZE_X,-POLYCOLLI_STAIRS_HALFSIZE_Y,POLYCOLLI_STAIRS_HALFSIZE_Z),
		},
		{// 階段の上部
			POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_TOP_HALFSIZE_X,0.0f,-POLYCOLLI_STAIRS_TOP_HALFSIZE_Z),
			POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_TOP_HALFSIZE_X,0.0f,-POLYCOLLI_STAIRS_TOP_HALFSIZE_Z),
			POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_TOP_HALFSIZE_X,0.0f,POLYCOLLI_STAIRS_TOP_HALFSIZE_Z),
			POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_TOP_HALFSIZE_X,0.0f,POLYCOLLI_STAIRS_TOP_HALFSIZE_Z),

		},
		{// 緩やかな階段
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X,POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X,POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
		},
		{// 緩やかな階段の上部
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X,POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X,POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
			POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Y,-POLYCOLLI_LONGSTAIRS_HALFSIZE_Z),
		},
	},
};

CPolygonCollider::MAP_POLYCOLII CPolygonCollider::m_PolyPos[MAP_TYPE_MAX] = { NULL };

void CPolygonCollider::CreateDateBase(void)
{
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos = new VTXPOS[POLYCOLLI_MAX];

	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS].vtx_0 = POLYCOLLI_STAIRS_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_HALFSIZE_X, POLYCOLLI_STAIRS_HALFSIZE_Y, -POLYCOLLI_STAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS].vtx_1 = POLYCOLLI_STAIRS_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_HALFSIZE_X, -POLYCOLLI_STAIRS_HALFSIZE_Y, -POLYCOLLI_STAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS].vtx_2 = POLYCOLLI_STAIRS_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_HALFSIZE_X, POLYCOLLI_STAIRS_HALFSIZE_Y, POLYCOLLI_STAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS].vtx_3 = POLYCOLLI_STAIRS_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_HALFSIZE_X, -POLYCOLLI_STAIRS_HALFSIZE_Y, POLYCOLLI_STAIRS_HALFSIZE_Z);

	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS_TOP].vtx_0 = POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_TOP_HALFSIZE_X, 0.0f, -POLYCOLLI_STAIRS_TOP_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS_TOP].vtx_1 = POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_TOP_HALFSIZE_X, 0.0f, -POLYCOLLI_STAIRS_TOP_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS_TOP].vtx_2 = POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(POLYCOLLI_STAIRS_TOP_HALFSIZE_X, 0.0f, POLYCOLLI_STAIRS_TOP_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_STAIRS_TOP].vtx_3 = POLYCOLLI_STAIRS_TOP_POS + D3DXVECTOR3(-POLYCOLLI_STAIRS_TOP_HALFSIZE_X, 0.0f, POLYCOLLI_STAIRS_TOP_HALFSIZE_Z);

	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS].vtx_0 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X, POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS].vtx_1 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X, POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS].vtx_2 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS].vtx_3 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);

	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS_TOP].vtx_0 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X, POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS_TOP].vtx_1 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X, POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS_TOP].vtx_2 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(-POLYCOLLI_LONGSTAIRS_HALFSIZE_X, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);
	m_PolyPos[MAP_TYPE_FIELD].pFieldVtxPos[POLYCOLLI_LONGSTAIRS_TOP].vtx_3 = POLYCOLLI_LONGSTAIRS_POS + D3DXVECTOR3(POLYCOLLI_LONGSTAIRS_HALFSIZE_X, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Y, -POLYCOLLI_LONGSTAIRS_HALFSIZE_Z);


}

void CPolygonCollider::DeleteDateBase(void)
{
	for (int nCntDate = 0; nCntDate < MAP_TYPE_MAX;nCntDate++)
	{
		if (m_PolyPos[nCntDate].pFieldVtxPos == NULL)
		{
			continue;
		}
		delete []m_PolyPos[nCntDate].pFieldVtxPos;
		m_PolyPos[nCntDate].pFieldVtxPos = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CPolygonCollider * CPolygonCollider::Create(int nType)
{
	// 生成
	CPolygonCollider *pCPolyColli = new CPolygonCollider;
	// 頂点位置の設定
	pCPolyColli->SetVtxPos(&m_conSetingVtxPos[0][nType][0]);
	// 初期化
	pCPolyColli->Init();
	return pCPolyColli;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Init(void)
{
	// 変数宣言
	D3DXVECTOR3 vecA = MYLIB_VEC3_UNSET;				// Aベクトル
	D3DXVECTOR3 vecB = MYLIB_VEC3_UNSET;				// Bベクトル

	// 法線ベクトルを求める
	vecA = m_VtxPos[1] - m_VtxPos[2];					// Aベクトル算出
	vecB = m_VtxPos[0] - m_VtxPos[1];					// Bベクトル算出
	D3DXVec3Cross(&m_SurfaceNor, &vecA, &vecB);			// 直交ベクトル算出
	D3DXVec3Normalize(&m_SurfaceNor, &m_SurfaceNor);	// 正規化する

	// 頂点情報の作成
	MakeVertex();
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Uninit(void)
{
	// 頂点バッファがNULLじゃなかった時
	if (m_pVtxBuff != NULL)
	{
		// 頂点バッファの開放
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::Draw(void)
{
	// 変数宣言
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスの取得
	D3DXMATRIX mtxWorld;									// 計算用マトリックス

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);			// ライティングモード無効

	// Fill Mode の設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Fill Mode の設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード無効


}

//-------------------------------------------------------------------------------------------------------------
// 衝突判定
//-------------------------------------------------------------------------------------------------------------
bool CPolygonCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection)
{
	if (Test3DInsidePolygon(pPos))
	{
#ifdef _DEBUG
		CDebugProc::Print("中にいる\n");
#endif
		if (bReflection == true)
		{
			pPos->y = m_VtxPos[2].y +
				(-m_SurfaceNor.x  *	(pPos->x - m_VtxPos[2].x) -
					m_SurfaceNor.z * (pPos->z - m_VtxPos[2].z)) / m_SurfaceNor.y;
			*pOut_Intersect = *pPos;
			*SurfaceNor = m_SurfaceNor;
		}
		else
		{
			pPos->y = m_VtxPos[2].y +
				(-m_SurfaceNor.x  *	(pPos->x - m_VtxPos[2].x) -
				m_SurfaceNor.z * (pPos->z - m_VtxPos[2].z))	/ m_SurfaceNor.y;
		}
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// ポリゴンの中にいるかテスト
//-------------------------------------------------------------------------------------------------------------
bool CPolygonCollider::Test3DInsidePolygon(D3DXVECTOR3 * pPos)
{
	if (CMylibrary::Test3DInsidePolygon(m_VtxPos[0], m_VtxPos[1], m_VtxPos[2], *pPos, -m_SurfaceNor))
	{
		return true;
	}
	else if(CMylibrary::Test3DInsidePolygon(m_VtxPos[1], m_VtxPos[3], m_VtxPos[2], *pPos, -m_SurfaceNor))
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void CPolygonCollider::SetVtxPos(D3DXVECTOR3 * pVertexsPos)
{
	for (int nCntVtx = 0; nCntVtx < POLYCOLLI_USE_VTX; nCntVtx++)
	{
		m_VtxPos[nCntVtx] = pVertexsPos[nCntVtx];
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT CPolygonCollider::MakeVertex(void)
{
	// 変数宣言
	CRenderer *pRenderer = CManager::GetRenderer();					// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();				// デバイスの取得

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * POLYCOLLI_USE_VTX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 座標の設定
	pVtx[0].pos = m_VtxPos[0];
	pVtx[1].pos = m_VtxPos[1];
	pVtx[2].pos = m_VtxPos[2];
	pVtx[3].pos = m_VtxPos[3];

	// 法線ベクトルの設定
	pVtx[0].nor = m_SurfaceNor;
	pVtx[1].nor = m_SurfaceNor;
	pVtx[2].nor = m_SurfaceNor;
	pVtx[3].nor = m_SurfaceNor;

	// 頂点カラー
	pVtx[0].col = POLYCOLLI_COL;
	pVtx[1].col = POLYCOLLI_COL;
	pVtx[2].col = POLYCOLLI_COL;
	pVtx[3].col = POLYCOLLI_COL;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

	return E_NOTIMPL;
}
