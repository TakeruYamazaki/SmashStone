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

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CPolygonCollider * CPolygonCollider::Create(D3DXVECTOR3 * pVertexsPos)
{
	// 生成
	CPolygonCollider *pCPolyColli = new CPolygonCollider;
	// 頂点位置の設定
	pCPolyColli->SetVtxPos(pVertexsPos);
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
}

//-------------------------------------------------------------------------------------------------------------
// 衝突判定
//-------------------------------------------------------------------------------------------------------------
bool CPolygonCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection)
{
	if (Test3DInsidePolygon(pPos))
	{
		CDebugProc::Print("中にいる\n");
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
