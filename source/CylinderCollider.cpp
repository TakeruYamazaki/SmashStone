//*************************************************************************************************************
//
// シリンダーコライダー処理[CylinderCoillider.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "CylinderCollider.h"
#include "DebugProc.h"
#include "renderer.h"
#include "3DBoxCollider.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define CYLICOILLI_NUMSURFACEH			(6)		// 横の個数
#define CYLICOILLI_NUMSURFACEV			(1)		// 縦の個数
#define CYLICOILLI_BOTTOMUNITVEC_NUMVTX	(3)		// 底面の単位ベクトル計算に必要な頂点数

#define CYLICOILLI_FILENAME						"data/TEXT/ColliInfo/Cylinder.csv"	// ファイル名
#define CYLICOILLI_OPENMODE						"r"		// ファイルの開くモード

#define CYLICOILLI_WORDSIZE						16		// ワードサイズ

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CCylinderCoillider::READINFOFILEBUFFER	CCylinderCoillider::m_ReadInfoFileBuff;		// 読みこんだファイル情報

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CCylinderCoillider::CCylinderCoillider() :CScene(CScene::PRIORITY_COLLISION)
{
	// それぞれの総数初期化
	m_nNumVertex     = (CYLICOILLI_NUMSURFACEH + 1)*(CYLICOILLI_NUMSURFACEV + 1);
	m_nNumIndex      = ((CYLICOILLI_NUMSURFACEH * 2) + 2) * CYLICOILLI_NUMSURFACEV + 2 * ((CYLICOILLI_NUMSURFACEV - 1));
	m_nNumPolygon    = (CYLICOILLI_NUMSURFACEV * CYLICOILLI_NUMSURFACEH * 2 + 4 * (CYLICOILLI_NUMSURFACEV - 1));

	m_nNumVertex = (CYLICOILLI_NUMSURFACEH + 1)*(CYLICOILLI_NUMSURFACEV + 1);
	m_nNumIndex = ((CYLICOILLI_NUMSURFACEH * 2) + 2) * CYLICOILLI_NUMSURFACEV + 2 * ((CYLICOILLI_NUMSURFACEV - 1));
	m_nNumPolygon = (CYLICOILLI_NUMSURFACEV * CYLICOILLI_NUMSURFACEH * 2 + 4 * (CYLICOILLI_NUMSURFACEV - 1));

	m_TopPoint       = MYLIB_3DVECTOR_ZERO;
	m_BottomPoint    = MYLIB_3DVECTOR_ZERO;
	m_BottomTransVec = MYLIB_3DVECTOR_ZERO;
	m_TopTransVec    = MYLIB_3DVECTOR_ZERO;
	m_Difference     = MYLIB_3DVECTOR_ZERO;
	m_fRadius        = MYLIB_FLOAT_UNSET;
	m_BottomNormal   = MYLIB_3DVECTOR_ZERO;
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
CCylinderCoillider::~CCylinderCoillider()
{
	m_pPareMtxWorld = NULL;
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CCylinderCoillider::Load(void)
{
	// 読みこんだファイル情報の初期化
	m_ReadInfoFileBuff.nNumReadInfo = MYLIB_INT_UNSET;
	m_ReadInfoFileBuff.pCell = NULL;
	m_ReadInfoFileBuff.pSetThisID = NULL;
#ifdef _DEBUG
	cout << "---------------------------------------------------------------------\n";
	cout << "CCylinderCoillider::Load シリンダーコライダーの読み込み開始\n";
	DWORD start = timeGetTime();			// 計測スタート時間
#endif
	// 変数宣言
	FILE *pFile = fopen(CYLICOILLI_FILENAME, CYLICOILLI_OPENMODE);
	// 開けなかった
	if (pFile == NULL)
	{
#ifdef _DEBUG
		cout << "CCylinderCoillider::Load シリンダーコライダーの読み込みのファイルが開けませんでした\n";
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
				char aSizeWord[CYLICOILLI_WORDSIZE]	;		// サイズのフラグ文字
				char aDiffWord[CYLICOILLI_WORDSIZE];		// 差分のフラグ文字
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
	cout << "CCylinderCoillider::Load シリンダーコライダーの読み込み終了\n";
	cout << " CCylinderCoillider::Load シリンダーコライダーの読み込み 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]\n";
#endif
	cout << "---------------------------------------------------------------------\n";
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Unload(void)
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
// 生成
//-------------------------------------------------------------------------------------------------------------
CCylinderCoillider * CCylinderCoillider::Create(TYPEID enmTypeId, D3DXMATRIX *pMtxWorld)
{
	// 変数宣言
	CCylinderCoillider *pCylinderCoillider	= new CCylinderCoillider;										// シリンダーコライダーのポインタ
	int nID									= CCylinderCoillider::m_ReadInfoFileBuff.pSetThisID[enmTypeId];	// IDの取得
	READINFOFILE_CELL *pCell				= &CCylinderCoillider::m_ReadInfoFileBuff.pCell[nID];			// セルのポインタ

	// 情報の設定と同期
	pCylinderCoillider->InfoSetSync(
		pCell->pSizeInfo->fRadius,		// 半径
		pCell->pSizeInfo->fTop,			// 上面までのサイズ
		pCell->pSizeInfo->fBottom,		// 底面までのサイズ
		*pCell->pDifference				// 差分
	);

	// 親モデルの設定
	pCylinderCoillider->SetPareMtxWorld(pMtxWorld);

	// タイプIDの設定
	pCylinderCoillider->SetTypeID(enmTypeId);



	return pCylinderCoillider;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスのポインタ

	// 頂点情報の作成
	MakeVertex(pDevice);

	// インデクス情報の作成
	MakeIndex(pDevice);
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Uninit(void)
{
	// 頂点バッファNULLじゃない時
	if (m_pVtexBuff != NULL)
	{
		// 頂点バッファの開放
		m_pVtexBuff->Release();
		m_pVtexBuff = NULL;
	}
	// インデックスバッファNULLじゃない時
	if (m_pIdxBuff != NULL)
	{
		// インデックスバッファの開放
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Update(void)
{
	//CDebugProc::Print("親情報のマトリックス[%.3f][%.3f][%.3f]\n", m_pPareMtxWorld->_41, m_pPareMtxWorld->_42, m_pPareMtxWorld->_43);
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// デバイスのポインタ
	D3DXMATRIX			mtxRot, mtxTrans;									// 計算用マトリックス

	// ライティングモード無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 親モデルの情報があるとき
	if (m_pPareMtxWorld != NULL)
	{
		D3DXMatrixMultiply(&m_mtxWorld
			, &m_mtxWorld
			, m_pPareMtxWorld);
	}


	/* ワールド座標から座標を算出 */
	// 上面の位置
	D3DXVec3TransformCoord(&m_TopPoint, &m_TopTransVec, &m_mtxWorld);
	// 底面の位置
	D3DXVec3TransformCoord(&m_BottomPoint, &m_BottomTransVec, &m_mtxWorld);


	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtexBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);
#ifdef _DEBUG
	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
#endif
	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// レンダラーの設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面(左回り)をカリングする

	// 底面の法線と1辺のベクトルの算出
	CalculateBottomNormal();
}

//-------------------------------------------------------------------------------------------------------------
// 情報の設定と同期
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::InfoSetSync(float fRadius, float fLengthMax, float fLengthMin, D3DXVECTOR3 &diff)
{
	// 設定1
	m_fRadius			= fRadius;		// 半径
	m_TopTransVec.y		= fLengthMax;	// 長さ最大
	m_BottomTransVec.y	= fLengthMin;	// 長さ最小
	m_TopPoint.y		= fLengthMax;	// 上面の位置
	m_BottomPoint.y		= fLengthMin;	// 底面の位置
	m_Difference		= diff;			// 差分

	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	MakeVertex(pDevice);

	// インデクス情報の作成
	MakeIndex(pDevice);
}

//-------------------------------------------------------------------------------------------------------------
// 親モデルの設定
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::SetPareMtxWorld(D3DXMATRIX* pPareMtxWorld)
{
	m_pPareMtxWorld = pPareMtxWorld;
}

//-------------------------------------------------------------------------------------------------------------
// ワールドマトリックスの取得
//-------------------------------------------------------------------------------------------------------------
D3DXMATRIX *CCylinderCoillider::GetMtxWorld(void)
{
	return&m_mtxWorld;
}

//-------------------------------------------------------------------------------------------------------------
// 半径の取得
//-------------------------------------------------------------------------------------------------------------
float CCylinderCoillider::GetRadius(void)
{
	return m_fRadius;
}

//-------------------------------------------------------------------------------------------------------------
// 最大点の取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * CCylinderCoillider::GetTopPoint(void)
{
	return &m_TopPoint;
}

//-------------------------------------------------------------------------------------------------------------
// 最小点の取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * CCylinderCoillider::GetBottomPoint(void)
{
	return  &m_BottomPoint;
}

//-------------------------------------------------------------------------------------------------------------
// 底面の法線を算出
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::CalculateBottomNormal(void)
{
	// 変数宣言
	D3DXVECTOR3 aWorkPos[CYLICOILLI_BOTTOMUNITVEC_NUMVTX];	// 計算用位置情報
	VERTEX_3D   *pVtx;												// 頂点情報の設定

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	// 必要な頂点数ループ	/* 上面か底面の頂点 */
	for (int nCntVtx = 0; nCntVtx < CYLICOILLI_BOTTOMUNITVEC_NUMVTX; nCntVtx++)
	{
		// 頂点座標を算出
		D3DXVec3TransformCoord(&aWorkPos[nCntVtx], &pVtx[0].pos, &m_mtxWorld);
		// 頂点ポインタを進める
		pVtx++;
	}

	// 頂点データをアンロック
	m_pVtexBuff->Unlock();

	// 外積計算
	D3DXVec3Cross(&m_BottomNormal,
		&(aWorkPos[1] - aWorkPos[0]),
		&(aWorkPos[2] - aWorkPos[0]));
	// 単位ベクトルの生成
	D3DXVec3Normalize(&m_BottomNormal, &m_BottomNormal);
}

//-------------------------------------------------------------------------------------------------------------
// 底面の法線ベクトルの取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * CCylinderCoillider::GetBottomNormal(void)
{
	return &m_BottomNormal;
}

//-------------------------------------------------------------------------------------------------------------
// タイプID
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::SetTypeID(TYPEID enmTypeID)
{
	m_enmTtpeID = enmTypeID;
}

//-------------------------------------------------------------------------------------------------------------
// 衝突
//-------------------------------------------------------------------------------------------------------------
bool CCylinderCoillider::Collision(int n3DBoxColliderID)
{
	if (InsideBoxTest(n3DBoxColliderID, &m_TopPoint) == true)
	{
		return true;
	}
	if (InsideBoxTest(n3DBoxColliderID, &m_BottomPoint) == true)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// ボックス内にいるかテスト
//-------------------------------------------------------------------------------------------------------------
bool CCylinderCoillider::InsideBoxTest(int n3DBoxColliderID, D3DXVECTOR3 *pPos)
{
	// 変数宣言
	C3DBoxCollider::_3DBOXCOLLIDER *p3DBoxInfo = C3DBoxCollider::GetInfo(n3DBoxColliderID);
	// 使用されているとき
	if (p3DBoxInfo->bUse == false)
	{
		return false;
	}
	// 上側の裏にいるとき
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[0], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_TOP]) == false)
	{
		CDebugProc::Print("上\n");
		return false;

	}
	// 右側の裏にいるとき
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[1], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_RIGHT]) == false)
	{
		CDebugProc::Print("右\n");
		return false;

	}
	// 左側の裏にいるとき
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[0], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_LEFT]) == false)
	{
		CDebugProc::Print("左\n");
		return false;

	}
	// 前側の裏にいるとき
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[0], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_FRONT]) == false)
	{
		CDebugProc::Print("前\n");
		return false;

	}
	// 後ろ側の裏にいるとき
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[3], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_REAR]) == false)
	{
		CDebugProc::Print("後ろ\n");
		return false;

	}
	// 下側の裏にいるとき
	if (CMylibrary::TestIfPolygonBack(p3DBoxInfo->Vertex[4], *pPos, p3DBoxInfo->surfaceNom[C3DBoxCollider::SURFACE_TYPE_BOTTOM]) == false)
	{
		CDebugProc::Print("下\n");
		return false;

	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の作成
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_3D *pVtx;	// 頂点情報の設定

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// 頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtexBuff,
		NULL);

	// 頂点データの範囲ロックし、頂点バッファへのポインタ取得
	m_pVtexBuff->Lock(0, 0, (void**)&pVtx, 0); 

	// 角度の計算
	float fAngle = ((D3DX_PI * 2) / CYLICOILLI_NUMSURFACEH);

	// 長さ
	float fLength[CYLICOILLI_NUMSURFACEV + 1] = {
		m_BottomTransVec.y,		// 最小
		m_TopTransVec.y			// 最大
	};
	m_TopPoint.y    = m_TopTransVec.y;
	m_BottomPoint.y = m_BottomTransVec.y;
	// 縦の四角形の個数分
	for (int nCntVertical = 0; nCntVertical < CYLICOILLI_NUMSURFACEV + 1; nCntVertical++)
	{
		// 横の四角形の個数分
		for (int nCntHeng = 0; nCntHeng < CYLICOILLI_NUMSURFACEH + 1; nCntHeng++)
		{
			pVtx[0].pos.x = sinf(fAngle*nCntHeng)*m_fRadius;
			pVtx[0].pos.y = fLength[nCntVertical];
			pVtx[0].pos.z = cosf(fAngle*nCntHeng)*m_fRadius;
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx++;
		}
	}

	// 頂点データをアンロック
	m_pVtexBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// インデックスの作成
//-------------------------------------------------------------------------------------------------------------
void CCylinderCoillider::MakeIndex(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// インデックスデータへのポインタ
	WORD *pIdx;

	// インデックスバッファをロックし、インデックスバッファのポインタ取得
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	// 奥行の個数
	for (int nCntVertical = 0; nCntVertical < CYLICOILLI_NUMSURFACEV; nCntVertical++)
	{
		// 最初だけはいらないようにするため
		if (nCntVertical > 0)
		{
			// 縮退ポリゴン分
			pIdx[0] = (nCntVertical*(CYLICOILLI_NUMSURFACEH + 1) + CYLICOILLI_NUMSURFACEH + 1);
			pIdx++;
		}
		// 横の個数
		for (int nCntInd = 0; nCntInd < CYLICOILLI_NUMSURFACEH + 1; nCntInd++)
		{
			// 縮退ポリゴンを除いた分
			pIdx[0] = nCntVertical*(CYLICOILLI_NUMSURFACEH + 1) + nCntInd + CYLICOILLI_NUMSURFACEH + 1;
			pIdx[1] = nCntVertical*(CYLICOILLI_NUMSURFACEH + 1) + nCntInd;
			pIdx += 2;
		}
		// 最後だけ入らないようにする
		if (nCntVertical < CYLICOILLI_NUMSURFACEV - 1)
		{
			// 縮退ポリゴン分
			pIdx[0] = (nCntVertical*(CYLICOILLI_NUMSURFACEH + 1) + CYLICOILLI_NUMSURFACEH);
			pIdx++;
		}
	}
	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
}
