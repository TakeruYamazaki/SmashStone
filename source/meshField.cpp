//==================================================================================================================
//
// メッシュフィールド処理[meshField.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "renderer.h"
#include "meshField.h"
#include "inputKeyboard.h"
#include "debugProc.h"
#include "player.h"
#include "tutorial.h"
#include "game.h"
#include "title.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define WhileX			250.0f							// イチマスの長さ横
#define WhileY			250.0f							// イチマスの長さ高さ
#define WhileZ			250.0f							// イチマスの長さ縦
#define MASS_WIDTH		(2)								// 横のマス
#define MASS_DEPTH		(2)								// 縦のマス

#define FIELD_ALPHA		1.0f							// メッシュフィールドアルファ
#define FIELD_TEXTUE	"data/TEXTURE/ground000.jpg"	// 読み込むテクスチャのソース先

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CMeshField::m_pTexture = NULL;			// テクスチャ変数
CMeshField *CMeshField::m_pMeshField = NULL;				// メッシュフィールドの情報
CPlayer *CMeshField::m_pPlayer = NULL;						// プレイヤーの情報

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CMeshField::CMeshField(PRIORITY type = CScene::PRIORITY_FIELD) : CScene(type)
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CMeshField::~CMeshField()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CMeshField::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// デバイスの取得

	// 位置・回転の初期設定
	m_pos		= ZeroVector3;		// 位置
	m_rot		= ZeroVector3;		// 回転
	m_vectorA	= ZeroVector3;		// 一つ目の外積用変数
	m_vectorB	= ZeroVector3;		// 二つ目の外積用変数
	m_vectorC	= ZeroVector3;		// 三つ目の外積用変数
	nNumber		= 0;				// 配列の番号
	StartBox	= m_nWidth + 1;		// 始まる箱
	EndBox		= 0;				// 引かれる箱
	fDivide		= 0;				// sinの中身を割る変数

	m_aVecA = new D3DXVECTOR3[m_nWidth * m_nDepth * 2];		// 法線ベクトルを面の数分一時的に格納
	m_aVecB = new D3DXVECTOR3[m_nWidth * m_nDepth * 2];		// 法線ベクトルを面の数分一時的に格納

	m_nNumVertex = ((m_nWidth + 1) * m_nDepth * 2) + (m_nDepth - 1) * 2;	// 総頂点数
	m_nNumIndex = (m_nDepth + 1) * (m_nWidth + 1);							// 総インデックス数
	m_nNumPolygon = m_nNumVertex - 2;										// 三角形の数

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファを生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// 縦をカウント
	for (int nDepth = 0; nDepth < m_nDepth + 1; nDepth++)
	{
		// 横をカウント
		for (int nWide = 0; nWide < m_nWidth + 1; nWide++)
		{
			///////////////////////////////////////////////ナミナミ～～///////////////////////////////////////////////
			// 頂点座標の設定
			//m_pVtx[0].pos = D3DXVECTOR3((-WhileX * Width) / 2 + WhileX * nWide, cosf(D3DX_PI / 6 * nDepth + fDivide) * WhileY, (WhileZ / 2 * Depth) - WhileZ * nDepth);

			// 頂点座標の設定
			m_pVtx[0].pos.x = (-WhileX * m_nWidth) / 2 + WhileX * nWide;
			m_pVtx[0].pos.y = 0.0f;
			m_pVtx[0].pos.z = (WhileZ / 2 * m_nDepth) - WhileZ * nDepth;

			// 頂点カラー
			m_pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ描写の位置
			m_pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			m_pVtx++;
		}
	}

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

	WORD *pIdx;				// インデックスバッファへのポインタ

	// インデックスバッファをロックしインデックスデータへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 縦をカウント
	for (int nCntY = 0; nCntY < m_nDepth; nCntY++)
	{
		// 一行目じゃないとき
		if (nCntY != 0)
		{
			pIdx[0] = (m_nWidth + 1) + nCntY * (m_nWidth + 1);
			pIdx++;
		}
		// 横をカウント
		for (int nCntX = 0; nCntX < m_nWidth + 1; nCntX++)
		{
			pIdx[0] = (m_nWidth + 1) + nCntX + nCntY * (m_nWidth + 1);
			pIdx[1] = nCntX + nCntY * (m_nWidth + 1);
			pIdx += 2;
		}

		// 縮退ポリゴン
		if (nCntY + 1 < m_nDepth)
		{
			pIdx[0] = m_nWidth + nCntY * (m_nWidth + 1);
			pIdx++;
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	// 法線の設定
	SetNor();
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CMeshField::Uninit(void)
{
	// 頂点バッファがNULLじゃないとき
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();				// 頂点バッファの開放
		m_pVtxBuff = NULL;					// NULLにする
	}

	// インデックスバッファがNULLじゃないとき
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();				// インデックスバッファの開放
		m_pIdxBuff = NULL;					// NULLにする
	}

	// nullcheck
	if (m_aVecA)
	{
		// 破棄
		delete[] m_aVecA;
		m_aVecA = nullptr;
	}

	// nullcheck
	if (m_aVecB)
	{
		// 破棄
		delete[] m_aVecB;
		m_aVecB = nullptr;
	}
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CMeshField::Update(void)
{

}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CMeshField::Draw(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラーの情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;							// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//==================================================================================================================
// 位置設定
//==================================================================================================================
void CMeshField::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==================================================================================================================
// ポリゴン生成
//==================================================================================================================
CMeshField *CMeshField::Create(void)
{
	// メモリを動的に確保
	m_pMeshField = new CMeshField(CScene::PRIORITY_FIELD);

	// メッシュ球があるとき
	if (m_pMeshField != NULL)
	{
		// メッシュのマス数を設定
		m_pMeshField->m_nWidth = MASS_WIDTH;
		m_pMeshField->m_nDepth = MASS_DEPTH;

		// シーン初期化
		m_pMeshField->Init();
	}

	// 値を返す
	return m_pMeshField;
}

//==================================================================================================================
// テクスチャ情報ロード
//==================================================================================================================
HRESULT CMeshField::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// レンダラーの情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// デバイスを取得する

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		FIELD_TEXTUE,							// ファイルの名前
		&m_pTexture);							// 読み込むメモリー

	// 値を返す
	return S_OK;
}

//==================================================================================================================
// テクスチャ情報破棄
//==================================================================================================================
void CMeshField::Unload(void)
{
	// テクスチャ情報があるとき
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();			// テクスチャ情報の開放
		m_pTexture = NULL;				// NULLにする
	}
}

//==================================================================================================================
// メッシュフィールド情報取得
//==================================================================================================================
CMeshField * CMeshField::GetMeshField(void)
{
	return m_pMeshField;
}

//==================================================================================================================
// 位置取得
//==================================================================================================================
D3DXVECTOR3 CMeshField::GetPos(void)
{
	return m_pos;
}

//==================================================================================================================
// 移動量取得
//==================================================================================================================
D3DXVECTOR3 CMeshField::GetMove(void)
{
	return m_move;
}

//==================================================================================================================
// 高さの取得
//==================================================================================================================
float CMeshField::GetHeight(D3DXVECTOR3 &pos)
{
	// 再初期化
	nNumber = 0;				// 配列の番号
	StartBox = m_nWidth + 1;	// 始まる箱
	EndBox = 0;					// 引かれる箱
	float VecY;					// 高さ調整用
	int nCntVec = 0;			// 法線のカウンタ

	// 頂点データの範囲をロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	//縦をカウント
	for (int nDepth = 0; nDepth < m_nDepth + 1; nDepth++)
	{
		// 横をカウント
		for (int nWide = 0; nWide < m_nWidth + 1; nWide++, nCntVec++)
		{
				// 端は計算させない
				if (nWide != m_nWidth && nDepth != m_nDepth)
				{
					// 計算用の座標
					D3DXVECTOR3 VtxPos0 = m_pVtx[0].pos;
					D3DXVECTOR3 VtxPos1 = m_pVtx[1].pos;
					D3DXVECTOR3 VtxPos3 = m_pVtx[1 + m_nWidth].pos;
					D3DXVECTOR3 VtxPos4 = m_pVtx[2 + m_nWidth].pos;
					
					// 全ての外積が正だと、そのポリゴンの上にいる
					if (GetNor(pos, VtxPos4, VtxPos3, VtxPos0) && GetPos().y >= pos.y)
					{
						VecY =	VtxPos3.y +
								(m_aVecB[nCntVec].x  *	(pos.x - VtxPos3.x)
								- m_aVecB[nCntVec].z *	(pos.z - VtxPos3.z))
								/ m_aVecB[nCntVec].y;

						// 頂点データをアンロック
						m_pVtxBuff->Unlock();

						// 高さを修正
						return GetPos().y + VecY;
					}

					// 2枚目のポリゴン
					if (GetNor(pos, VtxPos0, VtxPos1, VtxPos4) && GetPos().y >= pos.y)
					{
						VecY =	VtxPos1.y +
								(m_aVecA[nCntVec].x	 *	(pos.x - VtxPos1.x)
								- m_aVecA[nCntVec].z *	(pos.z - VtxPos1.z))
								/ m_aVecA[nCntVec].y;

						// 頂点データをアンロック
						m_pVtxBuff->Unlock();

						// 高さを修正
						return GetPos().y + VecY;
					}

					// 配列の番号1プラスする
					StartBox++;
					EndBox++;
					nNumber++;
			}
		}
		// 横終わったら配列の番号を1プラスする
		StartBox++;
		EndBox++;
	}

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

	return m_pos.y;
}

//==================================================================================================================
// 法線の取得
//==================================================================================================================
bool CMeshField::GetNor(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & VtxPos0, const D3DXVECTOR3 & VtxPos1, const D3DXVECTOR3 & VtxPos2)
{
	D3DXVECTOR3 nor0 = ZeroVector3;
	D3DXVECTOR3 nor1 = ZeroVector3;
	D3DXVECTOR3 nor2 = ZeroVector3;

	// 1頂点目
	D3DXVECTOR3 Vec0_P;
	D3DXVECTOR3 Vec0_1;
	// 2
	D3DXVECTOR3 Vec1_P;
	D3DXVECTOR3 Vec1_2;
	// 3
	D3DXVECTOR3 Vec2_P;
	D3DXVECTOR3 Vec2_0;

	// 1頂点目
	Vec0_P = pos - VtxPos0;
	Vec0_1 = VtxPos1 - VtxPos0;

	// 2
	Vec1_P = pos - VtxPos1;
	Vec1_2 = VtxPos2 - VtxPos1;

	// 3
	Vec2_P = pos - VtxPos2;
	Vec2_0 = VtxPos0 - VtxPos2;

	// 1つ目の法線
	D3DXVec3Cross(&nor0, &Vec0_1, &Vec0_P);

	// 2
	D3DXVec3Cross(&nor1, &Vec1_2, &Vec1_P);

	// 3
	D3DXVec3Cross(&nor2, &Vec2_0, &Vec2_P);

	if (nor0.y >= 0 && nor1.y >= 0 && nor2.y >= 0)
		return true;
	else
		return false;
}

//==================================================================================================================
// 移動量設定
//==================================================================================================================
void CMeshField::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//==================================================================================================================
// 法線の設定
//==================================================================================================================
void CMeshField::SetNor(void)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	//////////////////////////////法線ベクトル//////////////////////////////
	// 縦をカウント
	for (int nDepth = 0; nDepth < m_nDepth + 1; nDepth++)
	{
		// 横をカウント
		for (int nWide = 0; nWide < m_nWidth + 1; nWide++)
		{
			// インデックス番号
			int nIdx = nDepth * (m_nWidth + 1) + nWide;

			// 端は計算させない
			if (nWide != m_nWidth && nDepth != m_nDepth)
			{
				m_vectorA = m_pVtx[StartBox].pos - m_pVtx[EndBox].pos;										// 一つ目のベクトル
				m_vectorB = m_pVtx[StartBox + 1].pos - m_pVtx[EndBox].pos;									// 二つ目のベクトル
				m_vectorC = m_pVtx[EndBox + 1].pos - m_pVtx[EndBox].pos;									// 三つ目のベクトル
				D3DXVec3Cross(&m_aVecA[nNumber], &m_vectorA, &m_vectorB);									// 二つのベクトルの直交ベクトル
				D3DXVec3Normalize(&m_aVecA[nNumber], &m_aVecA[nNumber]);									// 正規化する
				D3DXVec3Cross(&m_aVecB[nNumber], &m_vectorB, &m_vectorC);									// 二つのベクトルの直交ベクトル
				D3DXVec3Normalize(&m_aVecB[nNumber], &m_aVecB[nNumber]);									// 正規化する

																											// 配列の番号1プラスする
				StartBox++;
				EndBox++;
				nNumber++;
			}

			if (nIdx == m_nWidth)											// 右上だったとき
			{
				// 法線ベクトル
				m_pVtx[nIdx].nor = m_aVecB[m_nWidth - 1];
			}
			else if (nIdx == m_nDepth * (m_nWidth + 1))						// 左下だったとき
			{
				// 法線ベクトル
				m_pVtx[nIdx].nor = m_aVecA[(m_nDepth - 1) * m_nWidth];
			}
			else if (nIdx == 0)											// 左上だったとき
			{
				// 法線ベクトル
				m_pVtx[nIdx].nor = (m_aVecA[nIdx] + m_aVecB[nIdx]) / 2;
			}
			else if (nIdx == (m_nDepth + 1) * (m_nWidth + 1) - 1)				// 右下だったとき
			{
				// 法線ベクトル
				m_pVtx[nIdx].nor = (m_aVecA[m_nDepth * m_nWidth - 1] + m_aVecB[m_nDepth * m_nWidth - 1]) / 2;
			}
			else if (nWide != 0 && nWide != m_nWidth)						// 横の列が端じゃないとき
			{
				if (nDepth == 0)										// 縦が一列目だったとき
				{
					// 法線ベクトル
					m_pVtx[nIdx].nor = (m_aVecB[nWide - 1] + m_aVecA[nWide] + m_aVecB[nWide]) / 3;
				}
				else if (nDepth == m_nDepth)								// 縦が最後尾だったとき
				{
					// 法線ベクトル
					m_pVtx[nIdx].nor = (m_aVecB[(m_nDepth - 1) * m_nWidth - 1 + nWide] +
										m_aVecA[(m_nDepth - 1) * m_nWidth + nWide] +
										m_aVecB[(m_nDepth - 1) * m_nWidth + nWide]) / 3;
				}
				else													// それ以外
				{
					// 法線ベクトル
					m_pVtx[nIdx].nor = (m_aVecA[(nWide - 1) + (nDepth - 1) * m_nWidth] +
										m_aVecB[(nWide - 1) + (nDepth - 1) * m_nWidth] +
										m_aVecA[(nWide - 1) + (nDepth - 1) * m_nWidth + 1] +
										m_aVecB[(nWide - 1) + (nDepth - 1) * m_nWidth + m_nWidth] +
										m_aVecA[(nWide - 1) + (nDepth - 1) * m_nWidth + m_nWidth + 1] +
										m_aVecB[(nWide - 1) + (nDepth - 1) * m_nWidth + m_nWidth + 1]) / 6;
				}
			}
			else if (nDepth != 0 && nDepth != m_nDepth)					// 縦の列が端じゃないとき
			{
				if (nWide == 0)											// 先頭のとき
				{
					// 法線ベクトル
					m_pVtx[nIdx].nor = (m_aVecA[(nDepth - 1) * m_nWidth] +
						m_aVecB[m_nWidth + (nDepth - 1) * m_nWidth] +
						m_aVecA[m_nWidth + (nDepth - 1) * m_nWidth]) / 3;
				}
				else if (nWide == m_nWidth)								// 最後尾のとき
				{
					// 法線ベクトル
					m_pVtx[nIdx].nor = (m_aVecB[m_nWidth - 1 + m_nWidth * (nDepth - 1)] +
						m_aVecA[m_nWidth - 1 + m_nWidth * (nDepth - 1)] +
						m_aVecB[m_nWidth - 1 + m_nWidth * nDepth]) / 3;
				}
			}
		}
		// 配列の番号1プラスする
		StartBox++;
		EndBox++;
	}

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}
