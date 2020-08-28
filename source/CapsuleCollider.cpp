//*************************************************************************************************************
//
// ポリゴンコライダー処理[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "CapsuleCollider.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "game.h"
#include "debugProc.h"
#include "CharEffectOffset.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define CAPCOLLI_SLICES		5
#define CAPCOLLI_STACKS		5

#define CAPCOLLI_BOTTOMUNITVEC_NUMVTX	(3)		// 底面の単位ベクトル計算に必要な頂点数

#define CAPCOLLI_FILENAME						"data/TEXT/ColliInfo/Cylinder.csv"	// ファイル名
#define CAPCOLLI_OPENMODE						"r"		// ファイルの開くモード

#define CAPCOLLI_WORDSIZE						16		// ワードサイズ

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数
//-------------------------------------------------------------------------------------------------------------
CCapsuleCollider::READINFOFILEBUFFER	CCapsuleCollider::m_ReadInfoFileBuff;		// 読みこんだファイル情報

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CCapsuleCollider::CCapsuleCollider() : CScene(PRIORITY_COLLISION)
{
	m_ColliderInfo.TopTransVec = MYLIB_VEC3_UNSET;		// 上面までの長さと向き
	m_ColliderInfo.BottomTransVec = MYLIB_VEC3_UNSET;	// 底面までの長さと向き
	m_ColliderInfo.enmTtpeID = TYPEID_NOEN;				// タイプID
	m_ColliderInfo.trans.pos = MYLIB_VEC3_UNSET;
	m_ColliderInfo.trans.rot = MYLIB_VEC3_UNSET;
	m_ColliderInfo.trans.scal = MYLIB_SCALE_UNSET;
	m_ColliderInfo.pMtxParent = NULL;		// 親のマトリックスポインタ
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CCapsuleCollider::Load(void)
{
	// 読みこんだファイル情報の初期化
	m_ReadInfoFileBuff.nNumReadInfo = MYLIB_INT_UNSET;
	m_ReadInfoFileBuff.pCell = NULL;
	m_ReadInfoFileBuff.pSetThisID = NULL;
#ifdef _DEBUG
	cout << "---------------------------------------------------------------------\n";
	cout << "CCapsuleCollider::Load カプセルコライダーの読み込み開始\n";
	DWORD start = timeGetTime();			// 計測スタート時間
#endif
											// 変数宣言
	FILE *pFile = fopen(CAPCOLLI_FILENAME, CAPCOLLI_OPENMODE);
	// 開けなかった
	if (pFile == NULL)
	{
#ifdef _DEBUG
		cout << "CCapsuleCollider::Load カプセルコライダーの読み込みのファイルが開けませんでした\n";
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
				char aSizeWord[CAPCOLLI_WORDSIZE];		// サイズのフラグ文字
				char aDiffWord[CAPCOLLI_WORDSIZE];		// 差分のフラグ文字
				READINFOFILE_CELL *pCell;					// セルポインタ

															// 初期化
				aSizeWord[0] = MYLIB_CHAR_UNSET;
				aDiffWord[0] = MYLIB_CHAR_UNSET;
				pCell = &m_ReadInfoFileBuff.pCell[nCntInfo];

				/* 一行分の情報を解析する */
				//			   SET     メモ   ID  上大きさ               差分                  タイプ
				sscanf(aRead, "%[^, ],%[^, ], %d, %[^, ], %[^, ], %[^, ],%[^, ], %[^, ], %[^, ], %d",
					&aEmpty, &aEmpty,
					&m_ReadInfoFileBuff.pSetThisID[nCntInfo],
					&aSizeWord,
					&aEmpty, &aEmpty,
					&aDiffWord,
					&aEmpty, &aEmpty,
					&pCell->nColliderType);
				// サイズを設定しない時
				if (strcmp(aSizeWord, "UNSET") != 0)
				{
					// サイズ情報の生成
					pCell->pSizeInfo = new SIZE_INFO;
					//             SET     メモ    ID      
					sscanf(aRead, "%[^, ], %[^, ], %[^, ], %f, %f, %f, ",
						&aEmpty, &aEmpty, &aEmpty,
						&pCell->pSizeInfo->fTop,
						&pCell->pSizeInfo->fBottom,
						&pCell->pSizeInfo->fRadius);
				}
				else
				{// それ以外の時
					pCell->pSizeInfo = NULL;
				}
				// 差分を設定しない時
				if (strcmp(aDiffWord, "UNSET") != 0)
				{
					// 差分の生成
					pCell->pDifference = new D3DXVECTOR3;
					sscanf(aRead, "%[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %f, %f, %f, ",
						&aEmpty, &aEmpty, &aEmpty, &aEmpty, &aEmpty, &aEmpty,
						&pCell->pDifference->x,
						&pCell->pDifference->y,
						&pCell->pDifference->z);
				}
				else
				{// それ以外の時
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
	cout << "CCapsuleCollider::Load カプセルコライダーの読み込み終了\n";
	cout << " CCapsuleCollider::Load カプセルコライダーの読み込み 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]\n";
#endif
	cout << "---------------------------------------------------------------------\n";
	return S_OK;

}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Unload(void)
{
	// 単体情報の個数分ループ
	for (int nCntCell = 0; nCntCell < m_ReadInfoFileBuff.nNumReadInfo; nCntCell++)
	{
		// 大きさの破棄
		if (m_ReadInfoFileBuff.pCell[nCntCell].pSizeInfo != NULL)
		{
			delete m_ReadInfoFileBuff.pCell[nCntCell].pSizeInfo;
			m_ReadInfoFileBuff.pCell[nCntCell].pSizeInfo = NULL;
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
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Init(void)
{
	m_ColliderInfo.nSlices = CAPCOLLI_SLICES * 2;
	m_ColliderInfo.nStacks_1_2 = (CAPCOLLI_STACKS % 2 == 0) ? CAPCOLLI_STACKS: CAPCOLLI_STACKS + 1;
	// 頂点数
	m_ColliderInfo.nNumVertex = (m_ColliderInfo.nStacks_1_2 + 1) * (m_ColliderInfo.nSlices + 1);
	// 見た目の頂点数
	m_ColliderInfo.nNumindex = 2 * m_ColliderInfo.nSlices * (m_ColliderInfo.nStacks_1_2 + 1);
	// 三角形の数
	m_ColliderInfo.nNumPolygon = m_ColliderInfo.nNumVertex - 2;


	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点の作成
	MakeVertex(pDevice);
	// インデックスの作成
	MakeIndex(pDevice);

	// カプセル情報の初期化
	InitCapsInfo();
}
//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Uninit(void)
{
	// 頂点バッファがNULLじゃない時
	if (m_ColliderInfo.pVtexBuff != NULL)
	{
		// 頂点バッファの開放
		m_ColliderInfo.pVtexBuff->Release();
		m_ColliderInfo.pVtexBuff = NULL;
	}

	// インデックスバッファがNULLじゃない時
	if (m_ColliderInfo.pIdxBuff != NULL)
	{
		// インデックスバッファの開放
		m_ColliderInfo.pIdxBuff->Release();
		m_ColliderInfo.pIdxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Update(void)
{
	// シーンがNULLじゃない時
	if (m_ColliderInfo.pScene != NULL)
	{// 衝突判定
		this->Collision();
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::Draw(void)
{
	// レンダラー情報取得
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラーの情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスの取得
	D3DXMATRIX mtxTrans, mtxRot;							// 計算用マトリックス

	// ライティングモード無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Fill Mode の設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	// カリングしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_ColliderInfo.trans.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_ColliderInfo.trans.rot.y,
		m_ColliderInfo.trans.rot.x,
		m_ColliderInfo.trans.rot.z);
	D3DXMatrixMultiply(&m_ColliderInfo.trans.mtxWorld,
		&m_ColliderInfo.trans.mtxWorld,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_ColliderInfo.trans.pos.x,
		m_ColliderInfo.trans.pos.y,
		m_ColliderInfo.trans.pos.z);

	D3DXMatrixMultiply(&m_ColliderInfo.trans.mtxWorld,
		&m_ColliderInfo.trans.mtxWorld,
		&mtxTrans);

	D3DXMatrixMultiply(&m_ColliderInfo.trans.mtxWorld,
		&m_ColliderInfo.trans.mtxWorld,
		&mtxTrans);


	// 親モデルの情報があるとき
	if (m_ColliderInfo.pMtxParent != NULL)
	{
		D3DXMatrixMultiply(&m_ColliderInfo.trans.mtxWorld
			, &m_ColliderInfo.trans.mtxWorld
			, m_ColliderInfo.pMtxParent);
	}

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_ColliderInfo.trans.mtxWorld);

	// カプセル位置の計算
	CalCapPosition();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_ColliderInfo.pVtexBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_ColliderInfo.pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_ColliderInfo.nNumindex, 0, m_ColliderInfo.nNumPolygon);

	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// Fill Mode の設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	// レンダラーの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする

}

//-------------------------------------------------------------------------------------------------------------
// 親のマトリックスポインタの設定
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::SetMtxParent(D3DXMATRIX * pMtxParent)
{
	m_ColliderInfo.pMtxParent = pMtxParent;
}

//-------------------------------------------------------------------------------------------------------------
// シーンのポインタ
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::SetScene(CScene * pScene)
{
	m_ColliderInfo.pScene = pScene;
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CCapsuleCollider * CCapsuleCollider::Create(CScene *pScene, D3DXMATRIX *pMtxParent, int nTypeID)
{
	// 変数宣言
	CCapsuleCollider *pCollider = new CCapsuleCollider;

	int nID = CCapsuleCollider::m_ReadInfoFileBuff.pSetThisID[nTypeID];	// IDの取得
	READINFOFILE_CELL *pCell = &CCapsuleCollider::m_ReadInfoFileBuff.pCell[nID];			// セルのポインタ

	pCollider->SeteumTypeID(nID);
	// 情報の設定と同期
	pCollider->InfoSetSync(
		pCell->pSizeInfo->fRadius,		// 半径
		pCell->pSizeInfo->fTop,			// 上面までのサイズ
		pCell->pSizeInfo->fBottom,		// 底面までのサイズ
		*pCell->pDifference				// 差分
	);

	// 親のマトリックスポインタの設定
	pCollider->SetMtxParent(pMtxParent);
	// シーンの設定
	pCollider->SetScene(pScene);
	// 設定
	pCollider->Init();
	return pCollider;
}

//-------------------------------------------------------------------------------------------------------------
// 情報の設定と同期
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::InfoSetSync(float fRadius, float fLengthMax, float fLengthMin, D3DXVECTOR3 & diff)
{
	m_ColliderInfo.TopTransVec.y = fLengthMax;		// 長さ最大
	m_ColliderInfo.BottomTransVec.y = fLengthMin;	// 長さ最小
	m_ColliderInfo.trans.pos = diff;				// 差分

	m_ColliderInfo.Capsule.fRadius = fRadius;
	m_ColliderInfo.Capsule.Segment.Point.y = fLengthMax;
	m_ColliderInfo.Capsule.Segment.Vec.y = fLengthMin - fLengthMax;
}

//-------------------------------------------------------------------------------------------------------------
// カプセル情報の初期化
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::InitCapsInfo(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 衝突判定
//-------------------------------------------------------------------------------------------------------------
bool CCapsuleCollider::Collision(void)
{
	// 体の時
	if (m_ColliderInfo.enmTtpeID == COLLIPARTS_BODY)
	{// 処理しない
		return false;
	}
	// 変数宣言
	CPlayer          *pOwn            = (CPlayer *)m_ColliderInfo.pScene;						// このコライダーを持っているプレイヤー
	CPlayer          *pOthers         = pOwn->GetAnotherPlayer();								// その他のプレイヤー
	CCapsuleCollider *pOthersCapColli = pOthers->GetCapCollider(CCharacter::COLLIPARTS_BODY);	// その他のプレイヤーのコライダー情報

	if (pOwn->ReadyToHit(m_ColliderInfo.enmTtpeID) == false)
	{
		return false;
	}

	CDebugProc::Print("COLLIPARTS [%d]", m_ColliderInfo.enmTtpeID);


#ifdef _DEBUG
	// 2線分の最短距を求める
	CMylibrary::colCapsuleCapsule(m_ColliderInfo.Capsule, pOthersCapColli->m_ColliderInfo.Capsule) ?
		CDebugProc::Print("当たってる\n"),
		CCharEffectOffset::Set(&pOthers->GetPos(), CCharEffectOffset::STR_ドンッ),
		pOwn->SetHitSound(),
		pOwn->SetAttakHit(true) :
		CDebugProc::Print("当たってない\n");
#else
	// 2線分の最短距を求める
	CMylibrary::colCapsuleCapsule(m_ColliderInfo.Capsule, pOthersCapColli->m_ColliderInfo.Capsule);
#endif // _DEBUG



	return false;
}

//-------------------------------------------------------------------------------------------------------------
// カプセル位置の計算
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::CalCapPosition(void)
{
	// 変数宣言
	D3DXVECTOR3 BottomPoint;	// 底面の位置

	// 上面の位置
	D3DXVec3TransformCoord(&m_ColliderInfo.Capsule.Segment.Point, &m_ColliderInfo.TopTransVec, &m_ColliderInfo.trans.mtxWorld);
	// 底面の位置
	D3DXVec3TransformCoord(&BottomPoint, &m_ColliderInfo.BottomTransVec, &m_ColliderInfo.trans.mtxWorld);

	// ベクトルに算出
	m_ColliderInfo.Capsule.Segment.Vec = BottomPoint - m_ColliderInfo.Capsule.Segment.Point;
}


//-------------------------------------------------------------------------------------------------------------
// 頂点の作成
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_ColliderInfo.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_ColliderInfo.pVtexBuff,
		NULL);

	// 変数宣言
	D3DXVECTOR3 TopPoint = m_ColliderInfo.Capsule.Segment.Point;			// 上面の位置
	D3DXVECTOR3 BottomPoint = m_ColliderInfo.Capsule.Segment.GetEndPoint();	// 底面の位置
	float fSlicesPI = (D3DX_PI * 2.0f) / m_ColliderInfo.nSlices;			// 分割数に対応した円周率
	float fStacksPI = (D3DX_PI * 2.0f) / m_ColliderInfo.nStacks_1_2;		// 分割数に対応した円周率
	// 縦をカウント
	int nCompSlices = (CAPCOLLI_SLICES / 2);
	nCompSlices += (CAPCOLLI_SLICES % 2 == 0) ? 0 : 1;

	// 頂点情報の設定
	VERTEX_3D *pVtx;
	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_ColliderInfo.pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSlices = 0; nCntSlices < m_ColliderInfo.nSlices + 1; nCntSlices++)
	{
		float fSlicesSinValue = sinf((fSlicesPI * nCntSlices));							// サインの値
		float fPos_y = cosf(fSlicesPI * nCntSlices) *  m_ColliderInfo.Capsule.fRadius;	// 半径のY座標の位置
		D3DXVECTOR3 pos = (nCntSlices <  nCompSlices) ? TopPoint : BottomPoint;			// 半径の中心位置

		// 横をカウント
		for (int nCntStacks = 0; nCntStacks < m_ColliderInfo.nStacks_1_2 + 1; nCntStacks++)
		{
			float fStacksRadian = fStacksPI * nCntStacks;								// ラジアン値

			// 頂点座標の設定
			pVtx[0].pos.x = pos.x+ fSlicesSinValue * cosf(fStacksRadian) * m_ColliderInfo.Capsule.fRadius;
			pVtx[0].pos.y = pos.y+ fPos_y;
			pVtx[0].pos.z = pos.z+ fSlicesSinValue * sinf(fStacksRadian) * m_ColliderInfo.Capsule.fRadius;
			// 頂点情報に法線の計算結果代入
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// テクスチャ描写の位置
			pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

			// 頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// 頂点番号加算
			pVtx++;
		}
	}

	// 頂点データをアンロックする
	m_ColliderInfo.pVtexBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// インデックスの作成
//-------------------------------------------------------------------------------------------------------------
void CCapsuleCollider::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_ColliderInfo.nNumindex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_ColliderInfo.pIdxBuff,
		NULL);

	WORD *pIdx;				// インデックスバッファへのポインタ
	int nNumber = 0;		// インデックス番号

	// インデックスバッファをロックしインデックスデータへのポインタを取得
	m_ColliderInfo.pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 縦をカウント
	for (int nCntSlices = 0; nCntSlices < m_ColliderInfo.nSlices; nCntSlices++)
	{
		// 横をカウント
		for (int nCntStacks = 0; nCntStacks < m_ColliderInfo.nStacks_1_2 + 1; nCntStacks++)
		{
			// 順にインデックス格納
			pIdx[nNumber++] = nCntSlices * (m_ColliderInfo.nStacks_1_2 + 1) + nCntStacks;
			pIdx[nNumber++] = pIdx[nNumber - 1] + m_ColliderInfo.nStacks_1_2 + 1;
		}
	}

	// インデックスバッファをアンロックする
	m_ColliderInfo.pIdxBuff->Unlock();
}
