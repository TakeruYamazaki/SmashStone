//*************************************************************************************************************
//
//ボックスコライダー処理[3DBoxCollider.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#include "3DBoxCollider.h"
#include "DebugProc.h"
#include "mouse.h"
#include "game.h"
#include "MeshField.h"
#include "manager.h"
#include "renderer.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define _3DBOXCOLLIDER_HALF_SIZE	0.5f								// サイズの半分
#define _3DBOXCOLLIDER_FILENAME		"data/TEXT/ColliInfo/3DBox.csv"		// ファイル名
#define _3DBOXCOLLIDER_OPENMODE		"r"									// ファイルの開くモード
// ファイル読み込を実行する
#define _3DBOXCOLLIDER_LOADFLAG_
#undef _3DBOXCOLLIDER_LOADFLAG_

#define _3DBOXCOLLI_WORDSIZE					16		// ワードサイズ

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9			   C3DBoxCollider::m_pVtxBuff = NULL;					// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9				   C3DBoxCollider::m_pTexture = NULL;					// テクスチャの情報のポインタ
LPDIRECT3DINDEXBUFFER9			   C3DBoxCollider::m_pIdxBuff = NULL;					// インデックスのバッファのポインタ
C3DBoxCollider::READINFOFILEBUFFER C3DBoxCollider::m_ReadInfoFileBuff;					// 読みこんだファイル情報
C3DBoxCollider::_3DBOXCOLLIDER	   C3DBoxCollider::m_ColliderInfo[_3DBOXCOLLIDER_MAX];	// コライダー情報

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
C3DBoxCollider::C3DBoxCollider(CScene::PRIORITY priority) : CScene(priority)
{
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DBoxCollider::Load(void)
{
	// 読みこんだファイル情報の初期化
	m_ReadInfoFileBuff.nNumReadInfo	= MYLIB_INT_UNSET;
	m_ReadInfoFileBuff.pCell		= NULL;
	m_ReadInfoFileBuff.pSetThisID	= NULL;
#ifdef _DEBUG
	cout << "---------------------------------------------------------------------\n";
	cout << "C3DBoxCollider::Load ボックスコライダーの読み込み開始\n";
	DWORD start = timeGetTime();			// 計測スタート時間
#endif
	// 変数宣言
	FILE *pFile = fopen(_3DBOXCOLLIDER_FILENAME, _3DBOXCOLLIDER_OPENMODE);
	// 開けなかった
	if (pFile == NULL)
	{
#ifdef _DEBUG
		cout << "C3DBoxCollider::Load ボックスコライダーの読み込みのファイルが開けませんでした\n";
		cout << "---------------------------------------------------------------------\n";
#endif
		return E_FAIL;
	}

	// 変数宣言
	char aRead[MYLIB_STRINGSIZE];		// 読み込み用
	char aComp[MYLIB_STRINGSIZE];		// 比較用
	char aEmpty[MYLIB_STRINGSIZE];		// 要らないもの用
	int  nCntInfo;						// 情報カウント

	aRead[0] = MYLIB_CHAR_UNSET;		// 読み込み用
	aComp[0] = MYLIB_CHAR_UNSET;		// 比較用
	aEmpty[0] = MYLIB_CHAR_UNSET;		// 要らないもの用
	nCntInfo = MYLIB_INT_UNSET;			// 情報カウント

#ifdef _DEBUG
	int  nCntError;						// エラーカウント
	nCntError = MYLIB_INT_UNSET;		// エラーカウント
#endif
	// スクリプトが来るまでとばす
	while (strcmp(aComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(aRead, sizeof(aRead), pFile);
		// 読み込んど文字列代入
		sscanf(aRead, "%[^,]s", &aComp);
#ifdef _DEBUG
		// エラーカウントをインクリメント
		if (++nCntError > 1048576)
		{// エラー
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
#endif
	}
	// END_SCRIPTまでループ
	while (strcmp(aComp, "END_SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(aRead, sizeof(aRead), pFile);
		// 読み込んど文字列代入
		sscanf(aRead, "%[^,]s", &aComp);
#ifdef _DEBUG
		// エラーカウントをインクリメント
		if (++nCntError > 1048576)
		{// エラー
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
#endif
		if (m_ReadInfoFileBuff.nNumReadInfo <= 0)
		{
			// 設定する個数
			if (strcmp(aComp, "SET_NUM") == 0)
			{
				sscanf(aRead, "%[^,],%d", &aEmpty, &m_ReadInfoFileBuff.nNumReadInfo);
				// 単体情報を個数分生成
				m_ReadInfoFileBuff.pCell = new READINFOFILE_CELL[m_ReadInfoFileBuff.nNumReadInfo];
				// 初期化するIDを個数分生成
				m_ReadInfoFileBuff.pSetThisID = new int[m_ReadInfoFileBuff.nNumReadInfo];
				// 文字列の初期化
				aComp[0] = '\0';
				cout << "読みこみ個数を取得しました\n";
			}
		}
		else
		{
			// 設定するもの
			if (strcmp(aComp, "SET") == 0)
			{
				// 変数宣言
				char aSizeWord[_3DBOXCOLLI_WORDSIZE];	// 大きさのフラグ文字
				char aDiffWord[_3DBOXCOLLI_WORDSIZE];	// 差分のフラグ文字
				READINFOFILE_CELL *pCell;					// セルポインタ

				// 初期化
				aSizeWord[0] = MYLIB_CHAR_UNSET;
				aDiffWord[0] = MYLIB_CHAR_UNSET;
				pCell = &m_ReadInfoFileBuff.pCell[nCntInfo];
				/* 一行分の情報を解析する */
				//			   SET     メモ   ID   大きさX 大きさY 大きさX 差分X   差分Y   差分Z   タイプ
				sscanf(aRead, "%[^, ], %[^, ], %d, %[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %d",
					&aEmpty, &aEmpty,
					&m_ReadInfoFileBuff.pSetThisID[nCntInfo],
					&aSizeWord,
					&aEmpty, &aEmpty,
					&aDiffWord,
					&aEmpty, &aEmpty, 
					&pCell->nColliderType);

				// サイズを設定する時
				if (strcmp(aSizeWord, "UNSET") != 0)
				{
					// サイズの生成
					pCell->pSize = new D3DXVECTOR3;
					// 大きさを読み込む
					sscanf(aRead, "%[^, ], %[^, ], %[^, ], %f, %f, %f, ",
						&aEmpty, &aEmpty, &aEmpty,
						&pCell->pSize->x,
						&pCell->pSize->y,
						&pCell->pSize->z);
				}
				else
				{// それ以外の時
					pCell->pSize = NULL;
				}
				// 差分を設定する時
				if (strcmp(aDiffWord, "UNSET") != 0)
				{
					// 差分の生成
					pCell->pDifference = new D3DXVECTOR3;
					// 差分を読み込む
					sscanf(aRead, "%[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %f, %f, %f, ",
						&aEmpty, &aEmpty, &aEmpty, &aEmpty, &aEmpty, &aEmpty,
						&pCell->pDifference->x,
						&pCell->pDifference->y,
						&pCell->pDifference->z);
				}
				else
				{// それ以外の時1
					pCell->pDifference = NULL;
				}
				// 情報カウントインクリメント
				nCntInfo++;
				// 文字列の初期化
				aComp[0] = '\0';
#ifdef _DEBUG
				cout << "[" << nCntInfo << "]個目の情報を取得しました\n";
#endif
			}
		}
	}
	fclose(pFile);
#ifdef _DEBUG
	DWORD end = timeGetTime();			// 計測スタート時間
	cout << "C3DBoxCollider::Load ボックスコライダーの読み込み終了\n";
	cout << "C3DBoxCollider::Load ボックスコライダーの読み込み 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]\n";
#endif
	cout << "---------------------------------------------------------------------\n";
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Unload(void)
{
	// 単体情報の個数分ループ
	for (int nCntCell = 0; nCntCell < m_ReadInfoFileBuff.nNumReadInfo; nCntCell++)
	{
		// 大きさの破棄
		if (m_ReadInfoFileBuff.pCell[nCntCell].pSize != NULL)
		{
			delete m_ReadInfoFileBuff.pCell[nCntCell].pSize;
			m_ReadInfoFileBuff.pCell[nCntCell].pSize = NULL;
		}
		// 差分の破棄
		if (m_ReadInfoFileBuff.pCell[nCntCell].pDifference != NULL)
		{
			delete m_ReadInfoFileBuff.pCell[nCntCell].pDifference;
			m_ReadInfoFileBuff.pCell[nCntCell].pDifference = NULL;
		}
	}
	// 単体情報を破棄
	if (m_ReadInfoFileBuff.pCell != NULL)
	{
		delete[]m_ReadInfoFileBuff.pCell;
		m_ReadInfoFileBuff.pCell = NULL;
	}
	// 初期化するIDの破棄
	if (m_ReadInfoFileBuff.pSetThisID != NULL)
	{
		delete[]m_ReadInfoFileBuff.pSetThisID;
		m_ReadInfoFileBuff.pSetThisID = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
C3DBoxCollider * C3DBoxCollider::Create(void)
{
	// 変数宣言
	C3DBoxCollider *p3DCollider = new C3DBoxCollider(CScene::PRIORITY_COLLISION);	// 3Dボックスコライダーポインタ

	// 初期化
	p3DCollider->Init();

	return p3DCollider;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Init(void)
{
	// 値情報の初期化
	InitColliderInfo();

	// 頂点の作成
	MakeVertex();

	// インデックスの作成
	MakeIndex();
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Update(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// 頂点バッファを生成
	D3DXMATRIX mtxTrans, mtwWorld;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ライティングモード無効

	int nCntVertexIndex = 0;
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++)
	{
		if (m_ColliderInfo[nCntCollider].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&m_ColliderInfo[nCntCollider].mtxWorld);

			// 位置を反映
			D3DXMatrixTranslation(
				&mtxTrans,
				m_ColliderInfo[nCntCollider].pos.x,
				m_ColliderInfo[nCntCollider].pos.y,
				m_ColliderInfo[nCntCollider].pos.z);

			D3DXMatrixMultiply(
				&m_ColliderInfo[nCntCollider].mtxWorld,
				&m_ColliderInfo[nCntCollider].mtxWorld,
				&mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &m_ColliderInfo[nCntCollider].mtxWorld);

			// 頂点バッファをデータストリームにバインド
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをストリームに設定
			pDevice->SetIndices(m_pIdxBuff);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);
#ifdef _DEBUG
			// テクスチャの設定
			pDevice->SetTexture(0, m_pTexture);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_LINELIST,
				nCntVertexIndex,
				0,
				_3DBOXCOLLIDER_NUM_INDEX,
				0,
				_3DBOXCOLLIDER_NUM_PRIMITIVE);
#endif
		}
		nCntVertexIndex += _3DBOXCOLLIDER_USE_VERTEX;
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード無効

}

//-------------------------------------------------------------------------------------------------------------
// 衝突
//-------------------------------------------------------------------------------------------------------------
bool C3DBoxCollider::Collision(D3DXVECTOR3 & pos)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;
	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX)
	{
		if (m_ColliderInfo[nCntCollider].bUse)
		{
			//if (CManager::GetMouse()->GetTrigger(0))
			//{
			//	bool bOk = false;
			//	if (CMylibrary::Test3DInsidePolygon(m_ColliderInfo[nCntCollider].Vertex[0], m_ColliderInfo[nCntCollider].Vertex[1], m_ColliderInfo[nCntCollider].Vertex[2], pos, m_ColliderInfo[nCntCollider].surfaceNom[C3DBoxCollider::SURFACE_TYPE_TOP]))
			//	{
			//		bOk = true;
			//	}
			//	if (CMylibrary::Test3DInsidePolygon(m_ColliderInfo[nCntCollider].Vertex[2], m_ColliderInfo[nCntCollider].Vertex[3], m_ColliderInfo[nCntCollider].Vertex[0], pos, m_ColliderInfo[nCntCollider].surfaceNom[C3DBoxCollider::SURFACE_TYPE_TOP]))
			//	{
			//		bOk = true;
			//	}
			//	if (bOk == false)
			//	{
			//		continue;
			//	}
			//	bOk = false;
			//	if (CMylibrary::Test3DInsidePolygon(m_ColliderInfo[nCntCollider].Vertex[4], m_ColliderInfo[nCntCollider].Vertex[5], m_ColliderInfo[nCntCollider].Vertex[6], pos, m_ColliderInfo[nCntCollider].surfaceNom[C3DBoxCollider::SURFACE_TYPE_BOTTOM]))
			//	{
			//		bOk = true;
			//	}
			//	else if (CMylibrary::Test3DInsidePolygon(m_ColliderInfo[nCntCollider].Vertex[6], m_ColliderInfo[nCntCollider].Vertex[7], m_ColliderInfo[nCntCollider].Vertex[4], pos, m_ColliderInfo[nCntCollider].surfaceNom[C3DBoxCollider::SURFACE_TYPE_BOTTOM]))
			//	{
			//		bOk = true;
			//	}
			//	if (bOk == false)
			//	{
			//		continue;
			//	}
			//}
			return true;
		}
	}
	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// ダメージ判定
//-------------------------------------------------------------------------------------------------------------
bool C3DBoxCollider::CollisionDamage(int n3DBoxColliderID, int nNoneColisitionID)
{
	// 3Dボックスコライダーのポインタ
	_3DBOXCOLLIDER *pOwnerCollider = &m_ColliderInfo[n3DBoxColliderID];	// 持ち主のポインタ
	_3DBOXCOLLIDER *pOtherCollider = &m_ColliderInfo[0];				// その他のポインタ
	bool		   bCollision	   = false;								// 衝突フラグ
	// 頂点情報の設定
	VERTEX_3D *pVtx;
	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// コライダー数分ループ
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX, pOtherCollider++)
	{
		if (nCntCollider == n3DBoxColliderID ||
			nCntCollider == nNoneColisitionID)
		{// 同じ時は除外
			continue;
		}
		if (pOtherCollider->bUse == false)
		{// 使用していないものは除外
			continue;
		}
		if (pOtherCollider->pScene == NULL)
		{// Sceneが設定されていないものは除外
			continue;
		}
		//if (pOtherCollider->pScene->GetObjectTyoe() != CScene::TYPE_ENEMY &&
		//	pOtherCollider->pScene->GetObjectTyoe() != CScene::TYPE_PLAYER)
		//{// 敵とプレイヤー以外の時除外
		//	continue;
		//}
		if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_NORMAL)
		{
			// Y軸が重なっているとき
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				// Z軸の範囲
				if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// 左の当たり判定
					if (pOwnerCollider->pos.x + pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						return true;
					}
					// 右の当たり判定
					else if (pOwnerCollider->pos.x - pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						return true;
					}
				}
				// X軸の範囲
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// 左の当たり判定
					if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						return true;
					}
					// 右の当たり判定
					else if (pOwnerCollider->pos.z - pOwnerCollider->size.z* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						return true;
					}
				}
			}
			if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
				pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
			{
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					if (pOwnerCollider->pos.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						return true;
					}
					else if (pOwnerCollider->pos.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						return true;
					}
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_CYLINDER)
		{
			// Y軸が重なっているとき
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
				float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + pOtherCollider->size.x;
				if (diffPos.x*diffPos.x +
					diffPos.y*diffPos.y <=
					(fRadius*fRadius))
				{

					// 頂点データをアンロック
					m_pVtxBuff->Unlock();
					return true;
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_SPHERE)
		{
			// 敵とプレイヤーの差分を求める
			D3DXVECTOR3 diffPos = pOwnerCollider->pos - pOtherCollider->pos;
			float fRadius = pOwnerCollider->size.x + pOtherCollider->size.x;

			// 三平方の定理から敵とプレイヤーの距離を比較
			if (diffPos.x*diffPos.x +
				diffPos.y*diffPos.y +
				diffPos.z*diffPos.z <=
				(fRadius* fRadius))
			{
				// 頂点データをアンロック
				m_pVtxBuff->Unlock();
				return true;
			}
		}
	}
	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
	return bCollision;
}

//-------------------------------------------------------------------------------------------------------------
// 衝突したかだけ判定
//-------------------------------------------------------------------------------------------------------------
bool C3DBoxCollider::Collisionoverlap(int n3DBoxColliderID,int* pHitID, int nNoneColisitionID)
{
	// 変数宣言
	_3DBOXCOLLIDER *pOwnerCollider = &m_ColliderInfo[n3DBoxColliderID];	// 持ち主のポインタ
	_3DBOXCOLLIDER *pOtherCollider = &m_ColliderInfo[0];				// その他のポインタ
	bool		   bCollision	   = false;								// 衝突フラグ
	// 頂点情報の設定
	VERTEX_3D *pVtx;
	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// コライダー数分ループ
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX, pOtherCollider++)
	{
		if (nCntCollider == n3DBoxColliderID ||
			nCntCollider == nNoneColisitionID)
		{// 同じ時は除外
			continue;
		}
		if (pOtherCollider->bUse == false)
		{// 使用していないものは除外
			continue;
		}
		if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_NORMAL)
		{
			// Y軸が重なっているとき
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				// Z軸の範囲
				if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// 左の当たり判定
					if (pOwnerCollider->pos.x + pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
					// 右の当たり判定
					else if (pOwnerCollider->pos.x - pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
				}
				// X軸の範囲
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// 左の当たり判定
					if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
					// 右の当たり判定
					else if (pOwnerCollider->pos.z - pOwnerCollider->size.z* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
				}
			}
			if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
				pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
			{
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					if (pOwnerCollider->pos.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
					else if (pOwnerCollider->pos.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// 頂点データをアンロック
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_CYLINDER)
		{
			// Y軸が重なっているとき
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
				float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + pOtherCollider->size.x;
				if (diffPos.x*diffPos.x +
					diffPos.y*diffPos.y <=
					(fRadius*fRadius))
				{

					// 頂点データをアンロック
					m_pVtxBuff->Unlock();
					*pHitID = nCntCollider;
					return true;
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_SPHERE)
		{
			// 敵とプレイヤーの差分を求める
			D3DXVECTOR3 diffPos = pOwnerCollider->pos - pOtherCollider->pos;
			float fRadius = pOwnerCollider->size.x + pOtherCollider->size.x;

			// 三平方の定理から敵とプレイヤーの距離を比較
			if (diffPos.x*diffPos.x +
				diffPos.y*diffPos.y +
				diffPos.z*diffPos.z <=
				(fRadius* fRadius))
			{
				// 頂点データをアンロック
				m_pVtxBuff->Unlock();
				*pHitID = nCntCollider;
				return true;
			}
		}
	}
	//CDebugProc::print("PlayerColli = [%.4f] [%.4f] [%.4f]\n", pOwnerCollider->pos.x, pOwnerCollider->pos.y, pOwnerCollider->pos.z);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
	*pHitID = -1;
	return bCollision;

}

//-------------------------------------------------------------------------------------------------------------
// ブロック状の衝突判定
//-------------------------------------------------------------------------------------------------------------
bool C3DBoxCollider::CollisionBox(int n3DBoxColliderID, D3DXVECTOR3 &pos, D3DXVECTOR3 &move)
{
	// 変数宣言
	_3DBOXCOLLIDER		 *pOwnerCollider = &m_ColliderInfo[n3DBoxColliderID];	// 持ち主のポインタ
	_3DBOXCOLLIDER		 *pOtherCollider = &m_ColliderInfo[0];					// その他のポインタ
	bool				 bCollision	   = false;									// 衝突フラグ
	VERTEX_3D *pVtx;													// 頂点情報ポインタ

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// コライダー数分ループ
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX, pOtherCollider++)
	{
		if (nCntCollider == n3DBoxColliderID)
		{// 同じ時は除外
			continue;
		}
		if (pOtherCollider->bUse == false)
		{// 使用していないものは除外
			continue;
		}
		if (pOtherCollider->ColliderSubType == C3DBoxCollider::COLLIDER_SUB_OVERRAP)
		{// 衝突しているか判別する種類は除く
			continue;
		}
		if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_NORMAL)
		{
			// Y軸が重なっているとき
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				// Z軸の範囲
				if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// 左の当たり判定
					if (pOwnerCollider->pos.x + pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.x = pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE - 0.01f;
						pos.x = pOwnerCollider->pos.x;
						move.x = 0.0f;
					}
					// 右の当たり判定
					else if (pOwnerCollider->pos.x - pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.x = pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE + 0.01f;
						pos.x = pOwnerCollider->pos.x;
						move.x = 0.0f;
					}
				}
				// X軸の範囲
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// 左の当たり判定
					if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.z = pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE - 0.01f;
						pos.z = pOwnerCollider->pos.z;
						move.z = 0.0f;
					}
					// 右の当たり判定
					else if (pOwnerCollider->pos.z - pOwnerCollider->size.z* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.z = pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE + 0.01f;
						pos.z = pOwnerCollider->pos.z;
						move.z = 0.0f;
					}
				}
			}
			if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
				pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
			{
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					if (pOwnerCollider->pos.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.y = pOwnerCollider->posOld.y;
						pos.y = pOwnerCollider->pos.y;
						move.y = 0.0f;
					}
					else if (pOwnerCollider->pos.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.y = pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + 0.0001f;
						pos.y = pOwnerCollider->pos.y - pOwnerCollider->difference.y;
						// 移動量の初期化
						move.y = 0.0f;
						bCollision = true;
					}
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_CYLINDER)
		{
			// Y軸が重なっているとき
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
				float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + (sqrtf(pOtherCollider->size.x*pOtherCollider->size.x + pOtherCollider->size.z*pOtherCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE);
				if (diffPos.x*diffPos.x +
					diffPos.y*diffPos.y <=
					(fRadius*fRadius))
				{
					// 角度を設定
					float radian = atan2f(-diffPos.x, -diffPos.y);
					pOwnerCollider->pos.x = pOtherCollider->pos.x + sinf(radian)*(fRadius);
					pOwnerCollider->pos.z = pOtherCollider->pos.z + cosf(radian)*(fRadius);
					pos.x = pOwnerCollider->pos.x;
					pos.z = pOwnerCollider->pos.z;
					
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_SPHERE)
		{
			// 敵とプレイヤーの差分を求める
			D3DXVECTOR3 diffPos = pOwnerCollider->pos - pOtherCollider->pos;
			float fRadius = pOwnerCollider->size.x + pOtherCollider->size.x;

			// 三平方の定理から敵とプレイヤーの距離を比較
			if (diffPos.x*diffPos.x +
				diffPos.y*diffPos.y +
				diffPos.z*diffPos.z <=
				(fRadius* fRadius))
			{
				// 角度を設定
				float fRadian = atan2f(diffPos.x, diffPos.z);
				pOwnerCollider->pos.x = pOtherCollider->pos.x + sinf(fRadian)*(fRadius);
				pOwnerCollider->pos.z = pOtherCollider->pos.z + cosf(fRadian)*(fRadius);
				pos.x = pOwnerCollider->pos.x;
				pos.z = pOwnerCollider->pos.z;
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_CYLINDER_CANRIDE)
		{
			// Y軸を考慮しないで入っていた場合
			D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
			float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + (sqrtf(pOtherCollider->size.x*pOtherCollider->size.x + pOtherCollider->size.z*pOtherCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE);
			if (diffPos.x*diffPos.x +
				diffPos.y*diffPos.y <=
				(fRadius*fRadius))
			{
				if (pOwnerCollider->pos.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->posOld.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
				{
					pOwnerCollider->pos.y = pOwnerCollider->posOld.y;
					pos.y = pOwnerCollider->pos.y;
					move.y = 0.0f;
				}
				else if (pOwnerCollider->pos.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->posOld.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
				{
					pOwnerCollider->pos.y = pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + 0.0001f;
					pos.y = pOwnerCollider->pos.y - pOwnerCollider->difference.y;
					// 移動量の初期化
					move.y = 0.0f;
					bCollision = true;
				}
			}
			// Y軸が重なっているとき
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
				float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + (sqrtf(pOtherCollider->size.x*pOtherCollider->size.x + pOtherCollider->size.z*pOtherCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE);
				if (diffPos.x*diffPos.x +
					diffPos.y*diffPos.y <=
					(fRadius*fRadius))
				{
					// 角度を設定
					float radian = atan2f(-diffPos.x, -diffPos.y);
					pOwnerCollider->pos.x = pOtherCollider->pos.x + sinf(radian)*(fRadius);
					pOwnerCollider->pos.z = pOtherCollider->pos.z + cosf(radian)*(fRadius);
					pos.x = pOwnerCollider->pos.x;
					pos.z = pOwnerCollider->pos.z;

				}
			}
		}
	}
	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
	return bCollision;
}

//-------------------------------------------------------------------------------------------------------------
// 設定
//-------------------------------------------------------------------------------------------------------------
int C3DBoxCollider::Set(D3DXVECTOR3 &size, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &difference, COLLIDER_TYPE colliType, COLLIDER_SUBTYPE ColliderSubType,CScene * pScene)
{
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++)
	{
		if (m_ColliderInfo[nCntCollider].bUse == false)
		{
			// 頂点情報の設定
			VERTEX_3D *pVtx;
			// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// 頂点情報の更新
			pVtx += _3DBOXCOLLIDER_USE_VERTEX * nCntCollider;
			m_ColliderInfo[nCntCollider].difference		= difference;		// 差分
			m_ColliderInfo[nCntCollider].size			= size;				// 大きさ
			m_ColliderInfo[nCntCollider].pos			= pos + difference;	// 位置
			m_ColliderInfo[nCntCollider].rot			= rot;				// 回転
			m_ColliderInfo[nCntCollider].bUse			= true;				// 使用フラグ
			m_ColliderInfo[nCntCollider].ColliderType	= colliType;		// 衝突種類
			m_ColliderInfo[nCntCollider].pScene			= pScene;			// sceneの設定
			m_ColliderInfo[nCntCollider].ColliderSubType = ColliderSubType;	// サブタイプの設定
			// 頂点位置の更新
			SetVertexPosition(pVtx, nCntCollider);
			SetVertexPosResult(pVtx, nCntCollider);
			SetSurfaceNom(pVtx, nCntCollider);
			// 頂点データをアンロック
			m_pVtxBuff->Unlock();

			return nCntCollider;
		}
	}
	return -1;
}

//-------------------------------------------------------------------------------------------------------------
// シーンポインタの設定
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetScene(int n3DBoxColliderID, CScene * pScene)
{
	m_ColliderInfo[n3DBoxColliderID].pScene = pScene;
}

//-------------------------------------------------------------------------------------------------------------
// シーンポインタの取得
//-------------------------------------------------------------------------------------------------------------
CScene * C3DBoxCollider::GetScene(int n3DBoxColliderID)
{
	return m_ColliderInfo[n3DBoxColliderID].pScene;
}

//-------------------------------------------------------------------------------------------------------------
// 情報を変更する
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::ChangeInfo(int nIndex, D3DXVECTOR3 & size, D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	if (m_ColliderInfo[nIndex].bUse == true)
	{
		// 頂点情報の設定
		VERTEX_3D *pVtx;
		// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報の更新
		pVtx += _3DBOXCOLLIDER_USE_VERTEX * nIndex;

		m_ColliderInfo[nIndex].size = size;		// 大きさ
		m_ColliderInfo[nIndex].pos = pos;		// 位置
		m_ColliderInfo[nIndex].rot = rot;		// 回転

		// 頂点位置の更新
		SetVertexPosition(pVtx, nIndex);
		SetVertexPosResult(pVtx, nIndex);
		SetSurfaceNom(pVtx, nIndex);

		// 頂点データをアンロック
		m_pVtxBuff->Unlock();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 情報を変更する
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::ChangeInfo(int nIndex, D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	if (m_ColliderInfo[nIndex].bUse == true)
	{
		// 頂点情報の設定
		VERTEX_3D *pVtx;
		// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報の更新
		pVtx += _3DBOXCOLLIDER_USE_VERTEX * nIndex;

		m_ColliderInfo[nIndex].pos = pos;		// 位置
		m_ColliderInfo[nIndex].rot = rot;		// 回転

		// 頂点位置の更新
		SetVertexPosition(pVtx, nIndex);
		SetVertexPosResult(pVtx, nIndex);
		SetSurfaceNom(pVtx, nIndex);

		// 頂点データをアンロック
		m_pVtxBuff->Unlock();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 位置の変更
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::ChangePosition(int nID,D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	if (m_ColliderInfo[nID].bUse == true)
	{
		// 頂点情報の設定
		VERTEX_3D *pVtx;
		// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報の更新
		pVtx += _3DBOXCOLLIDER_USE_VERTEX * nID;
		m_ColliderInfo[nID].posOld = m_ColliderInfo[nID].pos;
		m_ColliderInfo[nID].pos = pos + m_ColliderInfo[nID].difference;		// 位置
		m_ColliderInfo[nID].rot = rot;		// 回転

		// 頂点位置の更新
		SetVertexPosition(pVtx, nID);
		SetVertexPosResult(pVtx, nID);
		SetSurfaceNom(pVtx, nID);
		// 頂点データをアンロック
		m_pVtxBuff->Unlock();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 面の法線を設定する
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetSurfaceNom(VERTEX_3D *pVtx, int nIndex)
{
	{// 上の面の法線
		// それぞれのベクトルを作成
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[1] - m_ColliderInfo[nIndex].Vertex[0];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[3] - m_ColliderInfo[nIndex].Vertex[0];
		// 変数宣言
		D3DXVECTOR3 nomA_B;						// ポリゴン0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_TOP] = nomA_B;
	}

	{// 下の面の法線
		// それぞれのベクトルを作成
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[7] - m_ColliderInfo[nIndex].Vertex[4];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[5] - m_ColliderInfo[nIndex].Vertex[4];
		// 変数宣言
		D3DXVECTOR3 nomA_B;						// ポリゴン0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_BOTTOM] = nomA_B;
	}

	{// 右の面の法線
	 // それぞれのベクトルを作成
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[5] - m_ColliderInfo[nIndex].Vertex[1];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[2] - m_ColliderInfo[nIndex].Vertex[1];
		// 変数宣言
		D3DXVECTOR3 nomA_B;						// ポリゴン0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_RIGHT] = nomA_B;
	}

	{// 左の面の法線
	 // それぞれのベクトルを作成
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[3] - m_ColliderInfo[nIndex].Vertex[0];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[4] - m_ColliderInfo[nIndex].Vertex[0];
		// 変数宣言
		D3DXVECTOR3 nomA_B;						// ポリゴン0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_LEFT] = nomA_B;
	}
	{//  前の面の法線
	 // それぞれのベクトルを作成
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[4] - m_ColliderInfo[nIndex].Vertex[0];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[1] - m_ColliderInfo[nIndex].Vertex[0];
		// 変数宣言
		D3DXVECTOR3 nomA_B;						// ポリゴン0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_FRONT] = nomA_B;
	}
	{//  後ろの面の法線
	 // それぞれのベクトルを作成
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[2] -m_ColliderInfo[nIndex].Vertex[3];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[7] -m_ColliderInfo[nIndex].Vertex[3];
		// 変数宣言
		D3DXVECTOR3 nomA_B;						// ポリゴン0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_REAR] = nomA_B;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 計算後の頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetVertexPosResult(int nIndex)
{
	D3DXMATRIX mtxRot, mtxTrans, mtwWorld;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxTrans);
	D3DXMatrixIdentity(&mtwWorld);

	D3DXMatrixIdentity(&m_ColliderInfo[nIndex].mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		m_ColliderInfo[nIndex].rot.y,
		m_ColliderInfo[nIndex].rot.x,
		m_ColliderInfo[nIndex].rot.z);

	D3DXMatrixMultiply(
		&m_ColliderInfo[nIndex].mtxWorld,
		&m_ColliderInfo[nIndex].mtxWorld,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		m_ColliderInfo[nIndex].pos.x,
		m_ColliderInfo[nIndex].pos.y,
		m_ColliderInfo[nIndex].pos.z);

	D3DXMatrixMultiply(
		&m_ColliderInfo[nIndex].mtxWorld,
		&m_ColliderInfo[nIndex].mtxWorld,
		&mtxTrans);

	// 頂点情報の設定
	VERTEX_3D *pVtx;
	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += _3DBOXCOLLIDER_USE_VERTEX*nIndex;

	for (int nCntVert = 0; nCntVert < _3DBOXCOLLIDER_USE_VERTEX; nCntVert++)
	{
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			0.0f,
			0.0f,
			0.0f);
		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pVtx[nCntVert].pos.x,
			pVtx[nCntVert].pos.y,
			pVtx[nCntVert].pos.z);

		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&mtxTrans);

		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&m_ColliderInfo[nIndex].mtxWorld);

		m_ColliderInfo[nIndex].Vertex[nCntVert].x = mtwWorld._41;
		m_ColliderInfo[nIndex].Vertex[nCntVert].y = mtwWorld._42;
		m_ColliderInfo[nIndex].Vertex[nCntVert].z = mtwWorld._43;
	}
	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 計算後の頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetVertexPosResult(VERTEX_3D * pVtx, int nIndex)
{
	D3DXMATRIX mtxRot, mtxTrans, mtwWorld;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtwWorld);

	D3DXMatrixIdentity(&m_ColliderInfo[nIndex].mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		m_ColliderInfo[nIndex].rot.y,
		m_ColliderInfo[nIndex].rot.x,
		m_ColliderInfo[nIndex].rot.z);

	D3DXMatrixMultiply(
		&m_ColliderInfo[nIndex].mtxWorld,
		&m_ColliderInfo[nIndex].mtxWorld,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		m_ColliderInfo[nIndex].pos.x,
		m_ColliderInfo[nIndex].pos.y,
		m_ColliderInfo[nIndex].pos.z);

	D3DXMatrixMultiply(
		&m_ColliderInfo[nIndex].mtxWorld,
		&m_ColliderInfo[nIndex].mtxWorld,
		&mtxTrans);

	for (int nCntVert = 0; nCntVert < _3DBOXCOLLIDER_USE_VERTEX; nCntVert++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&mtwWorld);

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pVtx[nCntVert].pos.x,
			pVtx[nCntVert].pos.y,
			pVtx[nCntVert].pos.z);

		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&mtxTrans);

		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&m_ColliderInfo[nIndex].mtxWorld);

		m_ColliderInfo[nIndex].Vertex[nCntVert].x = mtwWorld._41;
		m_ColliderInfo[nIndex].Vertex[nCntVert].y = mtwWorld._42;
		m_ColliderInfo[nIndex].Vertex[nCntVert].z = mtwWorld._43;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 情報の取得
//-------------------------------------------------------------------------------------------------------------
C3DBoxCollider::_3DBOXCOLLIDER *C3DBoxCollider::GetInfo(int nIndex)
{
	return &m_ColliderInfo[nIndex];
}

//-------------------------------------------------------------------------------------------------------------
// 解除
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::unset(int nID)
{
	CMylibrary::Setdifferentvalue(m_ColliderInfo[nID].bUse, false);
	m_ColliderInfo[nID].bUse = false;
}

//-------------------------------------------------------------------------------------------------------------
// 情報の初期化
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::InitColliderInfo(void)
{
	// コライダーの最大個数分ループ
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++)
	{
		// 使用フラグの初期化
		m_ColliderInfo[nCntCollider].bUse = false;
		// 位置の初期化
		m_ColliderInfo[nCntCollider].pos = MYLIB_3DVECTOR_ZERO;
		// 前回の位置の初期化
		m_ColliderInfo[nCntCollider].posOld = MYLIB_3DVECTOR_ZERO;
		// 回転量の初期化
		m_ColliderInfo[nCntCollider].rot = MYLIB_3DVECTOR_ZERO;
		// 大きさの初期化
		m_ColliderInfo[nCntCollider].size = MYLIB_3DVECTOR_ZERO;
		// 面の法線の初期化
		for (int nCntVertex = 0; nCntVertex < _3DBOXCOLLIDER_NUM_SURFACE; nCntVertex++)
		{
			m_ColliderInfo[nCntCollider].surfaceNom[nCntVertex] = MYLIB_3DVECTOR_ZERO;
		}
		// 頂点位置の初期化
		for (int nCntVertex = 0; nCntVertex < _3DBOXCOLLIDER_USE_VERTEX; nCntVertex++)
		{
			m_ColliderInfo[nCntCollider].Vertex[0] = MYLIB_3DVECTOR_ZERO;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// コライダー情報の設定
//-------------------------------------------------------------------------------------------------------------
int C3DBoxCollider::SetColliderInfo(D3DXVECTOR3 *pPos, CScene *pScene, COLLIDER_SUBTYPE ColliderSubType, int nID)
{
	// コライダーIDに変換する
	ConvertColliderID(&nID);
	// 読みこむ個数分ループ
	for (int nCntRead = 0; nCntRead < m_ReadInfoFileBuff.nNumReadInfo; nCntRead++)
	{
		// 設定するIDと任意のIDが同じとき設定する
		if (m_ReadInfoFileBuff.pSetThisID[nCntRead] == nID)
		{
			return C3DBoxCollider::Set(
				Mybfunc_iifEx(m_ReadInfoFileBuff.pCell[nCntRead].pSize != NULL, *m_ReadInfoFileBuff.pCell[nCntRead].pSize, MYLIB_3DVECTOR_ZERO),
				*pPos,
				MYLIB_3DVECTOR_ZERO,
				Mybfunc_iifEx(m_ReadInfoFileBuff.pCell[nCntRead].pDifference != NULL, *m_ReadInfoFileBuff.pCell[nCntRead].pDifference, MYLIB_3DVECTOR_ZERO),
				(COLLIDER_TYPE)m_ReadInfoFileBuff.pCell[nCntRead].nColliderType, ColliderSubType,
				pScene);
		}
	}
	return -1;
}


//-------------------------------------------------------------------------------------------------------------
// 任意のIDをコライダーIDに変換
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::ConvertColliderID(int * pID)
{
	switch (*pID)
	{
		/* IDと同じなら何もしない */
	case ID_CHARACTER: return; break;
	case ID_STONE:   return; break;
		/* IDと違いうときスルー */
	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DBoxCollider::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// 頂点バッファを生成

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * _3DBOXCOLLIDER_MAX * _3DBOXCOLLIDER_USE_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX)
	{
		for (int nCntVert = 0; nCntVert < _3DBOXCOLLIDER_USE_VERTEX; nCntVert++)
		{
			// 頂点の設定
			pVtx[nCntVert].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// 法線ベクトル
			pVtx[nCntVert].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			// 頂点カラー
			pVtx[nCntVert].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// texture座標の設定
			pVtx[nCntVert].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();


	return E_NOTIMPL;
}
//-------------------------------------------------------------------------------------------------------------
// インデックス情報の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DBoxCollider::MakeIndex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// 頂点バッファを生成

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) *_3DBOXCOLLIDER_NUM_INDEX*_3DBOXCOLLIDER_MAX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックスデータへのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、インデックスバッファのポインタ取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntSurface = 0; nCntSurface < 2; nCntSurface++)
	{
		for (int nCntTopBottom = 0; nCntTopBottom < 4; nCntTopBottom++)
		{
			pIdx[0] = nCntTopBottom + (nCntSurface * 4);
			if (nCntTopBottom != 3)
			{
				pIdx[1] = 1 + nCntTopBottom + (nCntSurface * 4);
			}
			else
			{
				pIdx[1] = 0 + (nCntSurface * 4);
			}
			pIdx += 2;
		}
	}
	for (int nCntMidle = 0; nCntMidle < 4; nCntMidle++)
	{
		pIdx[0] = 0 + nCntMidle;
		pIdx[1] = 4 + nCntMidle;
		pIdx += 2;
	}
	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
	return E_NOTIMPL;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点位置の設定
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetVertexPosition(VERTEX_3D *pVtx, int nIndex)
{
	pVtx[0].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[1].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[2].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*-_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[3].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*-_3DBOXCOLLIDER_HALF_SIZE);

	pVtx[4].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[5].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[6].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*-_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[7].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*-_3DBOXCOLLIDER_HALF_SIZE);
}
