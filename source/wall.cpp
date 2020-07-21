//*************************************************************************************************************
//
// 壁の処理[wall.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "wall.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define WALL_FILENAME	"data/TEXT/wall/WallInfo.txt"	// ファイル名
#define WALL_OPENMODE	"r"								// 開くモード

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 CWall::m_pAllTexture[WALLTEX::WALLTEX_MAX] = {};					// 全てのテクスチャ情報
D3DXVECTOR2        CWall::m_Length                            = MYLIB_VEC2_UNSET;	// 中心からの距離
D3DXVECTOR3        CWall::m_CenterPos                         = MYLIB_VEC3_UNSET;	// 中心位置
float              CWall::m_fHeight                           = MYLIB_FLOAT_UNSET;	// 高さ

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
HRESULT CWall::Load(void)
{
	// 変数宣言
	FILE*             pFile;						// ファイルのポインタ
	char              aRead[MYLIB_STRINGSIZE];		// 読み込み用
	char              aComp[MYLIB_STRINGSIZE];		// 比較用
	char              aEmpty[MYLIB_STRINGSIZE];		// 要らないもの用
	char              aFileName[MYLIB_STRINGSIZE];	// ファイル名
	int               nCntFileName;					// ファイル名カウント
	D3DXVECTOR3       CenterPos;					// 中心位置
	D3DXVECTOR2       Length;						// 中心からの距離
	float             fHeight;						// 高さ
	LPDIRECT3DDEVICE9 pDevice;						// デバイスのポインタ

	// 初期化
	aRead[0]     = MYLIB_CHAR_UNSET;
	aComp[0]     = MYLIB_CHAR_UNSET;
	aEmpty[0]    = MYLIB_CHAR_UNSET;
	aFileName[0] = MYLIB_CHAR_UNSET;
	nCntFileName = MYLIB_INT_UNSET;
	fHeight      = MYLIB_FLOAT_UNSET;

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// ファイルを開く
	pFile = fopen(WALL_FILENAME, WALL_OPENMODE);

	// 開けなかった時
	if (pFile == NULL)
	{
		return E_FAIL;
	}

#ifdef _DEBUG
	int  nCntError;						// エラーカウント
	nCntError = MYLIB_INT_UNSET;		// エラーカウントの初期化
#endif
	// スクリプトが来るまでとばす
	while (strcmp(aComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(aRead, sizeof(aRead), pFile);
		// 読み込んど文字列代入
		sscanf(aRead, "%s", &aComp);
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
		sscanf(aRead, "%s", &aComp);
#ifdef _DEBUG
		// エラーカウントをインクリメント
		if (++nCntError > 1048576)
		{// エラー
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
#endif
		// ファイル名からテクスチャを読み込む
		if (sscanf(aRead, "FILENAME = %s", &aFileName) == 1)
		{
			// テクスチャの読み込み
			if (D3DXCreateTextureFromFile(pDevice, &aFileName[0], &m_pAllTexture[nCntFileName]) != D3D_OK)
			{// 失敗

			}
			else
			{// 成功

			}
			// ファイル名をインクリメント
			nCntFileName++;
		}
		// 中心位置の読み込み
		else if (sscanf(aRead, "CENTER = %f %f %f", &CenterPos.x, &CenterPos.y, &CenterPos.z) == 3)
		{
			CWall::m_CenterPos = CenterPos;
		}
		// 中心からの距離の読み込み
		else if (sscanf(aRead, "LENGTH = %f %f", &Length.x, &Length.y) == 2)
		{
			CWall::m_Length = Length;
		}
		// 高さの読み込み
		else if (sscanf(aRead, "HEIGHT = %f", &fHeight) == 1)
		{
			CWall::m_fHeight = fHeight;
		}
	}

	// ファイルを閉じる
	fclose(pFile);
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CWall::Unload(void)
{
	// テクスチャ情報の開放
	for (int nCntTexture = 0; nCntTexture < WALLTEX::WALLTEX_MAX; nCntTexture++)
	{
		if (m_pAllTexture[nCntTexture] != NULL)
		{
			m_pAllTexture[nCntTexture]->Release();
			m_pAllTexture[nCntTexture] = NULL;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
CWall * CWall::Create(WALLTEX enmWallTex)
{
	// 壁クラスの生成
	CWall *pWall = new CWall;
	// 失敗した時
	if (pWall == NULL)
	{
		return NULL;
	}
	// 初期化
	pWall->Init();
	// テクスチャの連結
	pWall->BindTexType(enmWallTex);
	return pWall;
}

//-------------------------------------------------------------------------------------------------------------
// 衝突判定
//-------------------------------------------------------------------------------------------------------------
bool CWall::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pOut_Intersect, D3DXVECTOR3* SurfaceNor, bool bReflection)
{
	// 変数宣言
	SINGLEINFO *pSingleInfo = &m_SingleInfo[0];	// 単体情報のポインタ
	bool bColli = false;						// 衝突フラグ

	// +Xの位置の時
	if (pSingleInfo[CWall::SETINGPOS_POSIX].trans.pos.x <= pPos->x)
	{
		// 衝突フラグを立てる
		bColli = true;
		
		// 反射フラグが立っているとき
		if (bReflection == true)
		{// 交点を求める
			if (GetIntersection(pPos, pPosOld, pOut_Intersect, &m_SingleInfo[SETINGPOS_POSIX]) == true)
			{
				*SurfaceNor = pSingleInfo[CWall::SETINGPOS_POSIX].nor;
			}
		}
		else
		{// 位置を戻す
			pPos->x = pSingleInfo[CWall::SETINGPOS_POSIX].trans.pos.x;
		}
	}
	// -Xの位置の時
	else if (pSingleInfo[CWall::SETINGPOS_NEGX].trans.pos.x >= pPos->x)
	{
		// 衝突フラグを立てる
		bColli = true;

		// 反射フラグが立っているとき
		if (bReflection == true)
		{// 交点を求める
			if (GetIntersection(pPos, pPosOld, pOut_Intersect, &m_SingleInfo[SETINGPOS_NEGX]) == true)
			{
				*SurfaceNor = pSingleInfo[CWall::SETINGPOS_NEGX].nor;
			}
		}
		else
		{// 位置を戻す
			pPos->x = pSingleInfo[CWall::SETINGPOS_NEGX].trans.pos.x;
		}
	}
	// +Yの位置の時
	if (pSingleInfo[CWall::SETINGPOS_POSIZ].trans.pos.z <= pPos->z)
	{
		// 衝突フラグを立てる
		bColli = true;

		// 反射フラグが立っているとき
		if (bReflection == true)
		{// 交点を求める
			if (GetIntersection(pPos, pPosOld, pOut_Intersect, &pSingleInfo[SETINGPOS_POSIZ]) == true)
			{
				*SurfaceNor = pSingleInfo[CWall::SETINGPOS_POSIZ].nor;
			}
		}
		else
		{// 位置を戻す
			pPos->z = pSingleInfo[CWall::SETINGPOS_POSIZ].trans.pos.z;
		}
	}
	// -Y位置の時
	else if (pSingleInfo[CWall::SETINGPOS_NEGZ].trans.pos.z >= pPos->z)
	{
		// 衝突フラグを立てる
		bColli = true;

		// 反射フラグが立っているとき
		if (bReflection == true)
		{// 交点を求める
			if (GetIntersection(pPos, pPosOld, pOut_Intersect, &pSingleInfo[SETINGPOS_NEGZ]) == true)
			{
				*SurfaceNor = pSingleInfo[CWall::SETINGPOS_NEGZ].nor;
			}
			
		}
		else
		{// 位置を戻す
			 pPos->z = pSingleInfo[CWall::SETINGPOS_NEGZ].trans.pos.z;
		}
	}
	// 衝突フラグを返す
	return bColli;
}

//-------------------------------------------------------------------------------------------------------------
// 交点を求める
//-------------------------------------------------------------------------------------------------------------
bool CWall::GetIntersection(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, D3DXVECTOR3 * pOut_Intersect, CONST SINGLEINFO * pSingleInfo)
{
	// 変数宣言
	D3DXVECTOR3 PlanePoint;			// 平面上の点
	D3DXVECTOR3 VecPlane_pos;		// 平面上の点から位置のベクトル
	D3DXVECTOR3 VecPlane_posOld;	// 平面上の点から前回の位置のベクトル
	float DotPlane_pos;				// 位置のベクトル平面法線内積
	float DotPlane_posOld;			// 前回の位置のベクトル平面法線内積

	// 平面上の点を算出
	PlanePoint = pSingleInfo->trans.pos;

	// ベクトルの生成
	CMylibrary::CreateVector3(&VecPlane_pos, pPos, &PlanePoint);
	CMylibrary::CreateVector3(&VecPlane_posOld, pPosOld, &PlanePoint);

	// 内積計算
	DotPlane_pos =
		VecPlane_pos.x * pSingleInfo->plane.a +
		VecPlane_pos.y * pSingleInfo->plane.b +
		VecPlane_pos.z * pSingleInfo->plane.c;
	DotPlane_posOld =
		VecPlane_posOld.x * pSingleInfo->plane.a +
		VecPlane_posOld.y * pSingleInfo->plane.b +
		VecPlane_posOld.z * pSingleInfo->plane.c;

	// 交差判定
	if (DotPlane_pos == 0.0f &&
		DotPlane_posOld == 0.0f)
	{
		// 両端が平面上にあり、交点を計算できない。

		return false;
	}

	// 交点を求める 
	D3DXVECTOR3 Pos_posOld = D3DXVECTOR3(pPosOld->x - pPos->x, pPosOld->y - pPos->y, pPosOld->z - pPos->z);

	//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB
	float fLength = abs(DotPlane_pos) / (abs(DotPlane_pos) + abs(DotPlane_posOld));

	pOut_Intersect->x = pPos->x + (Pos_posOld.x * fLength);
	pOut_Intersect->y = pPos->y + (Pos_posOld.y * fLength);
	pOut_Intersect->z = pPos->z + (Pos_posOld.z * fLength);
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CWall::Init(void)
{
	// 位置の設定
	m_SingleInfo[SETINGPOS_POSIX].trans.pos = D3DXVECTOR3(m_CenterPos.x + m_Length.x, m_CenterPos.y, m_CenterPos.z);	// +Xの位置
	m_SingleInfo[SETINGPOS_NEGX].trans.pos  = D3DXVECTOR3(m_CenterPos.x - m_Length.x, m_CenterPos.y, m_CenterPos.z);	// -Xの位置
	m_SingleInfo[SETINGPOS_POSIZ].trans.pos = D3DXVECTOR3(m_CenterPos.x, m_CenterPos.y, m_CenterPos.z + m_Length.y);	// +Zの位置
	m_SingleInfo[SETINGPOS_NEGZ].trans.pos  = D3DXVECTOR3(m_CenterPos.x, m_CenterPos.y, m_CenterPos.z - m_Length.y);	// -Zの地位

	// 大きさの設定
	m_SingleInfo[SETINGPOS_POSIX].size = D3DXVECTOR3(0.0f, m_fHeight, m_Length.y);
	m_SingleInfo[SETINGPOS_NEGX].size = D3DXVECTOR3(0.0f, m_fHeight, m_Length.y);
	m_SingleInfo[SETINGPOS_POSIZ].size = D3DXVECTOR3(m_Length.x, m_fHeight, 0.0f);
	m_SingleInfo[SETINGPOS_NEGZ].size = D3DXVECTOR3(m_Length.x, m_fHeight,0.0f);

	// 法線ベクトルの設定
	m_SingleInfo[SETINGPOS_POSIX].nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	m_SingleInfo[SETINGPOS_NEGX].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_SingleInfo[SETINGPOS_POSIZ].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	m_SingleInfo[SETINGPOS_NEGZ].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// 壁の数ループ
	for (int nCntSingle = 0; nCntSingle < WALL_MAX; nCntSingle++)
	{
		// 回転の設定
		m_SingleInfo[nCntSingle].trans.rot = MYLIB_VEC3_UNSET;
		// 平面の生成
		D3DXPlaneFromPointNormal(&m_SingleInfo[nCntSingle].plane, &m_SingleInfo[nCntSingle].trans.pos, &m_SingleInfo[nCntSingle].nor);
	}


	// 頂点情報の作成
	MakeVertex();
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CWall::Uninit(void)
{
	if (m_pVtexBuff != NULL)
	{
		m_pVtexBuff->Unlock();
		m_pVtexBuff->Release();
		m_pVtexBuff = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CWall::Update(void)
{
#ifdef WALL_DEBUG
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		CDebugProc::Print("壁の位置 %.3f,%.3f,%.3f\n",
			m_SingleInfo[nCntWall].trans.pos.x,
			m_SingleInfo[nCntWall].trans.pos.y,
			m_SingleInfo[nCntWall].trans.pos.z);
		CDebugProc::Print("壁の大きさ %.3f,%.3f,%.3f\n",
			m_SingleInfo[nCntWall].size.x,
			m_SingleInfo[nCntWall].size.y,
			m_SingleInfo[nCntWall].size.z);
	}
#endif
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CWall::Draw(void)
{
		// 変数宣言
	LPDIRECT3DDEVICE9	pDevice;	// デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	SINGLEINFO *pSingleInfo;		// エフェクトのポインタ

	pDevice = CManager::GetRenderer()->GetDevice();		// デバイスの取得
	pSingleInfo = &m_SingleInfo[0];						// ポインタの初期化

	// ライティングモード無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングしない
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pSingleInfo[nCntWall].trans.mtxWorld);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pSingleInfo[nCntWall].trans.mtxWorld);

		// 頂点バッファをデータストリームにバインド
		pDevice->SetStreamSource(0, m_pVtexBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWall * 4, 2);
	}

	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 裏面(左回り)をカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャ情報の連結
//-------------------------------------------------------------------------------------------------------------
void CWall::BindTexType(WALLTEX enmTexType)
{
	// ポインタを代入
	this->m_pTexture = CWall::m_pAllTexture[enmTexType];
}

//-------------------------------------------------------------------------------------------------------------
// 頂点情報の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT CWall::MakeVertex(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;			// 頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice;	// デバイスのポインタ

	// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * WALL_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtexBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 単体情報のポインタ
	SINGLEINFO *pSingleInfo = &m_SingleInfo[0];

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtexBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++, pSingleInfo++)
	{
		// 頂点位置
		pVtx[0].pos = D3DXVECTOR3(pSingleInfo->trans.pos.x - pSingleInfo->size.x, pSingleInfo->trans.pos.y - pSingleInfo->size.y, pSingleInfo->trans.pos.z - pSingleInfo->size.z);
		pVtx[1].pos = D3DXVECTOR3(pSingleInfo->trans.pos.x + pSingleInfo->size.x, pSingleInfo->trans.pos.y - pSingleInfo->size.y, pSingleInfo->trans.pos.z + pSingleInfo->size.z);
		pVtx[2].pos = D3DXVECTOR3(pSingleInfo->trans.pos.x - pSingleInfo->size.x, pSingleInfo->trans.pos.y + pSingleInfo->size.y, pSingleInfo->trans.pos.z - pSingleInfo->size.z);
		pVtx[3].pos = D3DXVECTOR3(pSingleInfo->trans.pos.x + pSingleInfo->size.x, pSingleInfo->trans.pos.y + pSingleInfo->size.y, pSingleInfo->trans.pos.z + pSingleInfo->size.z);
		// 法線ベクトル
		pVtx[0].nor = pSingleInfo->nor;
		pVtx[1].nor = pSingleInfo->nor;
		pVtx[2].nor = pSingleInfo->nor;
		pVtx[3].nor = pSingleInfo->nor;
		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
		// texture座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		// 頂点ポインタの更新
		pVtx += 4;
	}
	// 頂点データをアンロックする
	m_pVtexBuff->Unlock();

	return S_OK;

}
