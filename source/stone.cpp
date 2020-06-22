//*************************************************************************************************************
//
// ストーン処理[stone.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "stone.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "3DBoxCollider.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define CSTONE_INITVTXVALUE_MAX		1000000.0f
#define CSTONE_INITVTXVALUE_MIN		-1000000.0f

#define CSTONE_SHAKE_SIZE			0.1f			// ゆれの大きさ
#define CSTONE_SHAKECOEFF			0.02f			// ゆれる係数
#define CSTONE_ROTSPEED				0.02f			// 回転速度

#define CSTONE_SOPENFILENAME		"data/TEXT/StoneInfo/StoneTypeFileName.txt"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CStone::MODEL_INFO* CStone::m_pAllStoneTypeInfo = NULL;				// ストーンの全てのモデル情報
int                 CStone::m_nNumTypeAll       = MYLIB_INT_UNSET;	// ストーンの種類数

#ifdef CSTONE_DEBUG_DRAW
int                 CStone::m_nNumAll           = MYLIB_INT_UNSET;	// 全ての個数
#endif
//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
CStone::CStone()
{
}

//-------------------------------------------------------------------------------------------------------------
// 引数ありコンストラクタ
//-------------------------------------------------------------------------------------------------------------
CStone::CStone(PRIORITY type) : CSceneX(type)
{
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CStone::Load(void)
{
#ifdef CSTONE_LAOD_DEBUG_DRAW
	cout << "---------------------------------------------------------------------\n";
	cout << "CStone::Load ストーンのモデル読み込み開始\n";
	DWORD start = timeGetTime();			// 計測スタート時間
#endif
	// 変数宣言
	CString* pFileName;			// ファイル名
	pFileName = CStone::GetResource();
	// 取得に失敗した時
	if (pFileName == NULL)
	{
		return E_FAIL;
	}
	// ストーンの全てのモデル情報の生成
	if (m_pAllStoneTypeInfo == NULL)
	{
		m_pAllStoneTypeInfo = new MODEL_INFO[m_nNumTypeAll];
	}

	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		// デバイスの取得
	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// ストーンの種類数ループ
	for (int nCntModelType = 0; nCntModelType < m_nNumTypeAll; nCntModelType++)
	{
#ifdef CSTONE_USE_TEXTURE
		// テクスチャ情報の初期化
		m_pAllStoneTypeInfo[nCntModelType].pTextureID = NULL;
		m_pAllStoneTypeInfo[nCntModelType].ppTexture = NULL;
		m_pAllStoneTypeInfo[nCntModelType].nNumTexture = MYLIB_INT_UNSET;
#endif
		// Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX(pFileName[nCntModelType].Get(),
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_pAllStoneTypeInfo[nCntModelType].pBuffMat,
			NULL,
			&m_pAllStoneTypeInfo[nCntModelType].nNumMat,
			&m_pAllStoneTypeInfo[nCntModelType].pMesh)))
		{
			return E_FAIL;
		}
		else
		{
#ifdef CSTONE_LAOD_DEBUG_DRAW
			cout << "[" << nCntModelType << "]個目読みこみました\n";
#endif

		}
		// 最大最小の頂点位置の取得
		GetMaxMinVertices(nCntModelType);
#ifdef CSTONE_USE_TEXTURE
		// Xファイルから取得したテクスチャを生成
		CreateTexturefrom_Xfile(nCntModelType, pFileName[nCntModelType].Get());
#endif
	}

	// 文字列の開放
	for (int nCntFileName = 0; nCntFileName < m_nNumTypeAll; nCntFileName++)
	{
		pFileName[nCntFileName].release();
	}
	// ファイル名の開放
	delete[] pFileName;
	pFileName = NULL;

#ifdef CSTONE_LAOD_DEBUG_DRAW
	DWORD end = timeGetTime();			// 計測スタート時間
	cout << "CStone::Load ストーンのモデル読み込み終了\n";
	cout << " CStone::Load ストーンのモデル読み込み 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]\n";
	cout << "---------------------------------------------------------------------\n";
#endif
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CStone::Unload(void)
{
	if (m_pAllStoneTypeInfo != NULL)
	{
		// ストーンの種類数ループ
		for (int nCntModelType = 0; nCntModelType < m_nNumTypeAll; nCntModelType++)
		{
#ifdef CSTONE_USE_TEXTURE
			// テクスチャIDが使われているとき
			if (m_pAllStoneTypeInfo[nCntModelType].pTextureID != NULL)
			{// テクスチャIDの破棄
				delete[] m_pAllStoneTypeInfo[nCntModelType].pTextureID;
				m_pAllStoneTypeInfo[nCntModelType].pTextureID = NULL;
			}
			// テクスチャが使われていた時
			if (m_pAllStoneTypeInfo[nCntModelType].ppTexture != NULL)
			{
				// テクスチャの開放
				for (int nCntTex = 0; nCntTex < m_pAllStoneTypeInfo[nCntModelType].nNumTexture; nCntTex++)
				{
					m_pAllStoneTypeInfo[nCntModelType].ppTexture[nCntTex]->Release();
					m_pAllStoneTypeInfo[nCntModelType].ppTexture[nCntTex] = NULL;
				}
			}
#endif
			// マテリアルが使われているとき
			if (m_pAllStoneTypeInfo[nCntModelType].pBuffMat != NULL)
			{// マテリアルの破棄
				m_pAllStoneTypeInfo[nCntModelType].pBuffMat->Release();
				m_pAllStoneTypeInfo[nCntModelType].pBuffMat = NULL;
			}
			// メッシュが使われているとき
			if (m_pAllStoneTypeInfo[nCntModelType].pMesh != NULL)
			{// メッシュの破棄
				m_pAllStoneTypeInfo[nCntModelType].pMesh->Release();
				m_pAllStoneTypeInfo[nCntModelType].pMesh = NULL;
			}

		}
		// 全種のモデル情報の破棄
		delete[m_nNumTypeAll] m_pAllStoneTypeInfo;
		m_pAllStoneTypeInfo = NULL;
	}
#ifdef CSTONE_DEBUG_DRAW
	// 全ての個数の初期化
	CStone::m_nNumAll = MYLIB_INT_UNSET;
#endif
}

//-------------------------------------------------------------------------------------------------------------
// 資源の調達
//-------------------------------------------------------------------------------------------------------------
CString * CStone::GetResource(void)
{
#ifdef CSTONE_LAOD_DEBUG_DRAW
	DWORD start = timeGetTime();			// 計測スタート時間
#endif

	// 変数宣言
	CONST_STRING sOpenFileName = CSTONE_SOPENFILENAME;
	// ファイルを開く
	FILE *pFile;
	// 失敗した時
	if ((pFile = fopen(sOpenFileName, "r")) == NULL)
	{
		return NULL;
	}
	// 変数宣言
	CString * pFileName;			// ファイル名
	char cRead[MYLIB_STRINGSIZE];	// 読み込み用
	char cComp[MYLIB_STRINGSIZE];	// 比較用
	char cEmpty[MYLIB_STRINGSIZE];	// 要らないもの用
	int nCntFileName;				// ファイル名カウント

	// 変数の初期化
	pFileName = NULL;
	cRead[0] = MYLIB_CHAR_UNSET;
	cComp[0] = MYLIB_CHAR_UNSET;
	cEmpty[0] = MYLIB_CHAR_UNSET;
	nCntFileName = MYLIB_INT_UNSET;

	// SCRIPTまで飛ばす
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);
	}

	// END_SCRIPTまでループ
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		if (strcmp(cComp, "NUMBER") == 0)
		{
			if (pFileName == NULL)
			{
				// 文字列から情報を抜き取る
				sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &m_nNumTypeAll);
				// ファイル名の生成
				pFileName = new CString[m_nNumTypeAll];
			}
			// 比較用の初期化
			cComp[0] = MYLIB_CHAR_UNSET;
		}
		else if (strcmp(cComp, "FILENAME") == 0)
		{
			if (pFileName != NULL)
			{
				// 変数宣言
				char aSeting[MYLIB_STRINGSIZE];
				aSeting[0] = MYLIB_CHAR_UNSET;
				// 文字列から情報を抜き取る
				sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &aSeting);
				// 文字列の設定
				pFileName[nCntFileName].Set(aSeting);
				// ファイルカウントをインクリメント
				nCntFileName++;
			}
			// 比較用の初期化
			cComp[0] = MYLIB_CHAR_UNSET;
		}
	}
	// ファイルを閉じる
	fclose(pFile);
#ifdef CSTONE_LAOD_DEBUG_DRAW
	DWORD end = timeGetTime();			// 計測スタート時間
	cout << "CStone::GetResource 資源調達 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]\n";
#endif

	// ファイル名ポインタを返す
	return pFileName;
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CStone * CStone::Create(CONST STONE_ID eumID, CONST D3DXVECTOR3 & pos)
{
	// IDが設定されてい時
	if (eumID == STONE_ID_NONE)
	{
#ifdef CSTONE_USE_NULLPTR
		return nullptr;
#else
		return NULL;
#endif
	}
	// 変数宣言
	CStone* pStone;		// ストーンポインタ
	// 生成
	pStone = new CStone(CScene::PRIORITY_STONE);
	// 生成失敗した時
	if (pStone == NULL)
	{
#ifdef CSTONE_USE_NULLPTR
		return nullptr;
#else
		return NULL;
#endif
	}

	/* --- 初期化処理 --- */
	// CSceneXの初期化
	pStone->CSceneX::Init();
	// 揺れるカウントの初期化
	pStone->m_fCntShake = MYLIB_INT_UNSET;
	// ストーンのIDの設定
	pStone->m_enmStoneID = eumID;
	// 位置の設定
	pStone->m_pos = pos;
	// ボックスコライダーの設定
	pStone->m_nBoxClliderID = C3DBoxCollider::SetColliderInfo(&pStone->m_pos, pStone, C3DBoxCollider::ID_STONE);

#ifdef CSTONE_DEBUG_DRAW
	// 全ての個数を増やす
	CStone::m_nNumAll++;
	// IDを設定
	pStone->m_nNumID = CStone::m_nNumAll;
#endif

	// モデル情報の設定
	pStone->m_pModelInfo.matBuff = m_pAllStoneTypeInfo[eumID].pBuffMat;
	pStone->m_pModelInfo.matNum = m_pAllStoneTypeInfo[eumID].nNumMat;
	pStone->m_pModelInfo.mesh = m_pAllStoneTypeInfo[eumID].pMesh;
	pStone->m_pModelInfo.pTexture = NULL;
	// モデル情報の結合
	pStone->BindModel(pStone->m_pModelInfo);
	return pStone;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CStone::Init(void)
{
	this->CSceneX::Init();
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CStone::Uninit(void)
{
	this->CSceneX::Uninit();
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CStone::Update(void)
{
#ifdef CSTONE_UPDATE

	// 回転処理
	CMylibrary::SetFixTheRotation(&(this->m_rot.y += CSTONE_ROTSPEED));
	// ゆらゆら
	this->m_pos.y += sinf((m_fCntShake++) * CSTONE_SHAKECOEFF) * CSTONE_SHAKE_SIZE;

	C3DBoxCollider::ChangePosition(this->m_nBoxClliderID, this->m_pos, MYLIB_3DVECTOR_ZERO);
	//if (C3DBoxCollider::Collisionoverlap(this->m_nBoxClliderID))
	//{
	//	CDebugProc::Print("[%d]個目のStoneに当たった\n", this->m_nNumID);
	//	C3DBoxCollider::unset(this->m_nBoxClliderID);
	//	CScene::Release();
	//}
#ifdef CSTONE_DEBUG_DRAW
	CDebugProc::Print("[%d]個目Stoneの位置Y = [%.4f]\n", this->m_nNumID,this->m_pos.y);
	CDebugProc::Print("[%d]個目Stoneの回転量 = [%.4f]\n", this->m_nNumID,this->m_rot.y);
#endif // CSTONE_DEBUG_DRAW
#endif // CSTONE_UPDATE
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CStone::Draw(void)
{
#ifdef CSTONE_DRAW
	this->CSceneX::Draw();
#endif // CSTONE_DRAW
}

//-------------------------------------------------------------------------------------------------------------
// 入手する
//-------------------------------------------------------------------------------------------------------------
void CStone::Catch(void)
{
	C3DBoxCollider::unset(this->m_nBoxClliderID);
	CScene::Release();
}

//-------------------------------------------------------------------------------------------------------------
// 最大最小の頂点位置の取得
//-------------------------------------------------------------------------------------------------------------
void CStone::GetMaxMinVertices(int nIndex)
{
	// 変数宣言
	int	  nNumVertices;			// 頂点数
	DWORD sizeFVF;				// 頂点フォーマットサイズ
	BYTE  *pVertexBuffer;		// 頂点バッファのポインタ

	m_pAllStoneTypeInfo[nIndex].vtxMax = D3DXVECTOR3(CSTONE_INITVTXVALUE_MIN, CSTONE_INITVTXVALUE_MIN, CSTONE_INITVTXVALUE_MIN);
	m_pAllStoneTypeInfo[nIndex].vtxMin = D3DXVECTOR3(CSTONE_INITVTXVALUE_MAX, CSTONE_INITVTXVALUE_MAX, CSTONE_INITVTXVALUE_MAX);

	// 頂点数取得
	nNumVertices = m_pAllStoneTypeInfo[nIndex].pMesh->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(m_pAllStoneTypeInfo[nIndex].pMesh->GetFVF());

	// 頂点バッファをロック
	m_pAllStoneTypeInfo[nIndex].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

	// 頂点数分ループ
	for (int nCntModel = 0; nCntModel < nNumVertices; nCntModel++)
	{
		// 頂点位置を取得する
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		// 最小が大きいとき設定
		Mybfunc_WhenBiggerSet(m_pAllStoneTypeInfo[nIndex].vtxMin.x, vtx.x);
		Mybfunc_WhenBiggerSet(m_pAllStoneTypeInfo[nIndex].vtxMin.y, vtx.y);
		Mybfunc_WhenBiggerSet(m_pAllStoneTypeInfo[nIndex].vtxMin.z, vtx.z);
		// 最大が小さいとき設定
		Mybfunc_WhenSmallerSet(m_pAllStoneTypeInfo[nIndex].vtxMax.x, vtx.x);
		Mybfunc_WhenSmallerSet(m_pAllStoneTypeInfo[nIndex].vtxMax.y, vtx.y);
		Mybfunc_WhenSmallerSet(m_pAllStoneTypeInfo[nIndex].vtxMax.z, vtx.z);

		pVertexBuffer += sizeFVF;		// サイズ分ポインタを進める
	}

	// 頂点データをアンロックする
	m_pAllStoneTypeInfo[nIndex].pMesh->UnlockVertexBuffer();
}

#ifdef CSTONE_USE_TEXTURE
//-------------------------------------------------------------------------------------------------------------
// Xファイルから取得したテクスチャからIDの取得する
//-------------------------------------------------------------------------------------------------------------
void CStone::CreateTexturefrom_Xfile(const int nIndex, CONST STRING sFileName)
{
	// 変数宣言
	FILE *pFile;				// ファイルのポインタ
	int nNumTexture;			// テクスチャ数
	// 初期化
	nNumTexture = MYLIB_INT_UNSET;
	// ファイルを開く
	pFile = fopen(sFileName, "r");
	if (pFile != NULL)
	{
		// 変数宣言
		char cRead[MYLIB_STRINGSIZE];		// 読み込み用
		char cComp[MYLIB_STRINGSIZE];		// 比較用
		char cEmpty[MYLIB_STRINGSIZE];		// 要らないもの用
		// 初期化
		cRead[0] = MYLIB_CHAR_UNSET;
		cComp[0] = MYLIB_CHAR_UNSET;
		cEmpty[0] = MYLIB_CHAR_UNSET;
		do
		{	// 1行読み込む
			fgets(cRead, sizeof(cRead), pFile);
			// 読み込んど文字列代入
			sscanf(cRead, "%s", &cComp);
		} while (strcmp(cComp, "Mesh") != 0);

		if (strcmp(cComp, "Mesh") == 0)
		{
			cComp[0] = MYLIB_CHAR_UNSET;
			do
			{		// 1行読み込む
				fgets(cRead, sizeof(cRead), pFile);
				// 読み込んど文字列代入
				sscanf(cRead, "%s", &cComp);

				if (strcmp(cComp, "Material") == 0)
				{
					cComp[0] = MYLIB_CHAR_UNSET;
					do
					{	// 1行読み込む
						fgets(cRead, sizeof(cRead), pFile);
						// 読み込んど文字列代入
						sscanf(cRead, "%s", &cComp);
						if (strcmp(cComp, "TextureFilename") == 0)
						{
							cComp[0] = MYLIB_CHAR_UNSET;
							nNumTexture++;
						}
					} while (strcmp(cComp, "}") != 0);
					cComp[0] = MYLIB_CHAR_UNSET;
				}
			} while (strcmp(cComp, "}") != 0);
		}
	}
	// テクスチャが存在しない時処理を中断
	if (nNumTexture == 0)
	{
		// テクスチャ情報の初期化
		m_pAllStoneTypeInfo[nIndex].nNumTexture = MYLIB_INT_UNSET;
		m_pAllStoneTypeInfo[nIndex].ppTexture = NULL;
		m_pAllStoneTypeInfo[nIndex].pTextureID = NULL;
		return;
	}
	// テクスチャ数の設定
	m_pAllStoneTypeInfo[nIndex].nNumTexture = nNumTexture;
	// テクスチャIDの生成
	m_pAllStoneTypeInfo[nIndex].pTextureID = new int[nNumTexture];
	// テクスチャ情報の生成
	m_pAllStoneTypeInfo[nIndex].ppTexture = new LPDIRECT3DTEXTURE9[nNumTexture];

	// デバイスの取得
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();		// デバイスの取得

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL* materials = (D3DXMATERIAL*)m_pAllStoneTypeInfo[nIndex].pBuffMat->GetBufferPointer();

	int nCntxture = MYLIB_INT_UNSET;

	// カウントマテリアル
	for (int nCntMat = 0; nCntMat < (int)m_pAllStoneTypeInfo[nIndex].nNumMat; nCntMat++)
	{
		if (materials[nCntMat].pTextureFilename != NULL)
		{// ファイル名の取得
			STRING sName = strstr(materials[nCntMat].pTextureFilename, "DATA");
			if (sName != NULL)
			{
				if (D3DXCreateTextureFromFile(pDevice, sName, &m_pAllStoneTypeInfo[nIndex].ppTexture[nCntxture]) != D3D_OK)
				{
					// 失敗
				}
				else
				{
					// 成功
				}
			}
		}
	}
}
#endif