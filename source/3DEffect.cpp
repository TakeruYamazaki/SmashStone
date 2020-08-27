//*************************************************************************************************************
//
// 3Dエフェクト処理[3DEffect.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "3DEffect.h"
#include "renderer.h"
#include "manager.h"
#include "debugProc.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define _3DEFFE_FILENAME "data/TEXT/Effect/Texture/EffectTexture.txt"
#define _3DEFFE_OPNEMODE "r"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9     C3DEffect::m_pVtxBuff                        = nullptr;						// バッファ情報
LPDIRECT3DTEXTURE9          C3DEffect::m_pTexInfo[C3DEffect::TEXTYPE_MAX]= MYLIB_INITSTRUCT_WITHCONST;	// テクスチャ情報
const float                 C3DEffect::m_cfBaseAngle                     = D3DX_PI * 0.25f;				// 基本角度
int                         C3DEffect::m_nNumTextureMax                  = MYLIB_INT_UNSET;				// テクスチャの最大数
C3DEffect::PARAMETER        C3DEffect::m_EffectPram[_3DEFFE_USEQUANTITY] = MYLIB_INITSTRUCT_WITHCONST;	// エフェクトのパラメータ
C3DEffect::VIBRATION_OPTION C3DEffect::m_VibrationOption                 = MYLIB_INITSTRUCT_WITHCONST;	// 振動のオプション

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Load(void)
{
#ifdef _DEBUG
	DWORD start = timeGetTime();			// 計測スタート時間
#endif // _DEBUG

	// 変数宣言
	FILE *pFile;						// ファイルポインタ
	char aRead[MYLIB_STRINGSIZE];		// 読み込み用
	char aComp[MYLIB_STRINGSIZE];		// 比較用
	char aWork[MYLIB_STRINGSIZE];		// 作業用
	int  nCntInfo;						// 情報カウント
	LPDIRECT3DDEVICE9 pDevice;			// デバイスの取得
	VIBRATION_OPTION SetingOption;		// 設定用オプション

	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	aWork[0] = MYLIB_CHAR_UNSET;
	nCntInfo = MYLIB_INT_UNSET;
	pDevice = CManager::GetRenderer()->GetDevice();
	SetingOption = MYLIB_INITSTRUCT_WITHCONST;

	// ファイルを開く
	if ((pFile = fopen(_3DEFFE_FILENAME, _3DEFFE_OPNEMODE)) == NULL)
	{// 失敗した時
		return;
	}

	// エンドスクリプト
	while (strcmp(aComp,"END_SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(aRead, sizeof(aRead), pFile);
		// 読み込んど文字列代入
		sscanf(aRead, "%s", &aComp);

		// ファイル名
		if (sscanf(aRead, "FILENAME = %s", aWork) == 1)
		{
			// テクスチャの作成
			CreateTexture(aWork, nCntInfo, pDevice);
			// 情報カウントを進める
			nCntInfo++;
		}
		// 距離の最大数
		else if (sscanf(aRead, "MAXDIST = %d", &SetingOption.nMaxDist) == 1)
		{
			m_VibrationOption.nMaxDist = SetingOption.nMaxDist;
		}
		// 距離の縮小値
		else if (sscanf(aRead, "SCALEDIST = %f", &SetingOption.fScalDistValue) == 1)
		{
			m_VibrationOption.fScalDistValue = SetingOption.fScalDistValue;
		}
		// 方向の最大値
		else if (sscanf(aRead, "MAXDIVIDIR = %d", &SetingOption.nMaxDiviDir) == 1)
		{
			m_VibrationOption.nMaxDiviDir = SetingOption.nMaxDiviDir;
		}
		// 方向の縮小値
		else if (sscanf(aRead, "SCALDIR = %f", &SetingOption.fScalDirValue) == 1)
		{
			m_VibrationOption.fScalDirValue = SetingOption.fScalDirValue;
		}

		// 情報カウントがテクスチャタイプ以上になったとき
		if (nCntInfo >= TEXTYPE_MAX)
		{// 処理を抜ける
			break;
		}
	}

	fclose(pFile);
#ifdef _DEBUG
	DWORD end = timeGetTime();			// 計測スタート時間

	cout << "\nC3DEffect::Load テクスチャ情報の読み込み終了\n";
	cout << "C3DEffect::Load テクスチャ情報の読み込み 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]\n";
#endif // _DEBUG

};

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Unload(void)
{
	// テクスチャタイプの最大数ループ
	for (int nCntTex = 0; nCntTex < TEXTYPE_MAX; nCntTex++)
	{// ポインタがnullだった時
		if (m_pTexInfo[nCntTex] != nullptr)
		{// テクスチャ情報の開放処理
			m_pTexInfo[nCntTex]->Release();
			m_pTexInfo[nCntTex] = nullptr;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 作成
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Make(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;			// デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点情報の作成
	MakeVertex(pDevice);

	// 初期化
	Init();
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Init(void)
{
	// パラメータポインタ
	PARAMETER* pEffePram = &m_EffectPram[0];

	for (int nCntPrame = 0; nCntPrame < _3DEFFE_USEQUANTITY; nCntPrame++)
	{
		pEffePram[nCntPrame].bUse             = false;								// 使用フラグ
		pEffePram[nCntPrame].bDisp            = true;								// 描画フラグ
		pEffePram[nCntPrame].bBillBoard       = true;								// ビルボードフラグ
		pEffePram[nCntPrame].Trans            = TRANSFORM();						// トランス情報
		pEffePram[nCntPrame].pParent          = nullptr;							// 親の位置
		pEffePram[nCntPrame].move             = MYLIB_VEC3_UNSET;					// 移動量
		pEffePram[nCntPrame].col              = MYLIB_D3DXCOR_UNSET;				// 色
		pEffePram[nCntPrame].fRadius          = MYLIB_FLOAT_UNSET;					// 半径
		pEffePram[nCntPrame].fAngle           = D3DX_PI * 0.5f;						// 角度
		pEffePram[nCntPrame].fRadiusValue     = MYLIB_FLOAT_UNSET;					// 半径の変化値
		pEffePram[nCntPrame].fAlphaValue      = MYLIB_FLOAT_UNSET;					// アルファ値の変化値
		pEffePram[nCntPrame].nLife            = MYLIB_INT_UNSET;					// 持ち時間
		pEffePram[nCntPrame].nTexType         = MYLIB_INT_UNSET;					// テクスチャの種類
		pEffePram[nCntPrame].fGravity         = MYLIB_FLOAT_UNSET;					// 重力処理
		pEffePram[nCntPrame].type             = TYPE_NONE;							// 種類
		pEffePram[nCntPrame].Vibrat           = VIBRATION();						// 振動

		
		if (pEffePram[nCntPrame].Vibrat.bRandDist == false)
		{
			pEffePram[nCntPrame].Vibrat.fDist = m_VibrationOption.nMaxDist * m_VibrationOption.fScalDistValue;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Update(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;				// 頂点情報のポインタ
	PARAMETER* pEffePram;			// パラメータポインタ
	pEffePram = &m_EffectPram[0];	// ポインタの初期化

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPrame = 0; nCntPrame < _3DEFFE_USEQUANTITY; nCntPrame++, pVtx += _3DEFFE_USEVERTEX)
	{
		// 使用されていない時
		if (pEffePram[nCntPrame].bUse == false)
		{// 処理をスキップ
			continue;
		}
		// エフェクトのライフが0以下になったら処理を抜ける
		if (pEffePram[nCntPrame].nLife <= MYLIB_INT_UNSET)
		{// 使用フラグをオフにする
			pEffePram[nCntPrame].bUse = false;
			// 処理をスキップ
			continue;
		}
		//　ライフを減らす
		pEffePram[nCntPrame].nLife--;

		// 重力をかける
		pEffePram[nCntPrame].move.y += pEffePram[nCntPrame].fGravity;

		// 慣性処理
		CMylibrary::SlowingMove(&pEffePram[nCntPrame].move, 0.2f);

		// 移動量を加算
		pEffePram[nCntPrame].Trans.pos += pEffePram[nCntPrame].move;

		// 文字タイプの時
		if (pEffePram[nCntPrame].type == TYPE::CHAR)
		{
			// 振動の更新処理
			pEffePram[nCntPrame].Vibrat.Update();
			CDebugProc::Print("エフェクト振動した\n");

		}

		// 半径を変化させる
		pEffePram[nCntPrame].fRadius += pEffePram[nCntPrame].fRadiusValue;

		// 半径が0.0f以下の時
		if (pEffePram[nCntPrame].fRadius <= 0.0f)
		{// 半径を0.0fに固定
			pEffePram[nCntPrame].fRadius = 0.0f;
			// 使用フラグをオフにする
			pEffePram[nCntPrame].bUse = false;
			// 頂点サイズの設定
			SetVartexSize(pVtx, pEffePram[nCntPrame]);
		}
		// 頂点サイズの設定
		SetVartexSize(pVtx, pEffePram[nCntPrame]);

		// アルファ値を変化させる
		pEffePram[nCntPrame].col.a -= pEffePram[nCntPrame].fAlphaValue;

		// アルファ値が0.0f以下の時
		if (pEffePram[nCntPrame].col.a <= 0.0f)
		{// アルファ値を0.0fに固定
			pEffePram[nCntPrame].col.a = 0.0f;
			// 使用フラグをオフにする
			pEffePram[nCntPrame].bUse = false;
			// 頂点カラーの設定
			SetVetexColor(pVtx, pEffePram[nCntPrame]);
		}
		// 頂点カラーの設定
		SetVetexColor(pVtx, pEffePram[nCntPrame]);
	}

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

#ifdef _DEBUG
	pEffePram = &m_EffectPram[0];	// ポインタの初期化
	int nCntUse = 0;
	for (int nCntPrame = 0; nCntPrame < _3DEFFE_USEQUANTITY; nCntPrame++)
	{
		// 使用されていない時
		if (pEffePram[nCntPrame].bUse == true)
		{// 処理をスキップ
			nCntUse++;
		}
	}
	CDebugProc::Print("生きているエフェクト数 == [%d]\n", nCntUse);

#endif
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;						// デバイスのポインタ
	D3DXMATRIX mtxTrans, mtxParent;					// 計算用マトリックス
	PARAMETER* pEffePram;							// パラメータポインタ

	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	pEffePram = &m_EffectPram[0];					// ポインタの初期化

	// ライティングモード無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zバッファ　有効　無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	// 裏面(左回り)をカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntPrame = 0; nCntPrame < _3DEFFE_USEQUANTITY; nCntPrame++)
	{
		// 使用されていない時または、描画フラグが立っていない時
		if (pEffePram[nCntPrame].bUse == false ||
			pEffePram[nCntPrame].bDisp == false)
		{// 処理をスキップ
			continue;
		}

		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pEffePram[nCntPrame].Trans.mtxWorld);
		
		if (pEffePram[nCntPrame].type == C3DEffect::TYPE::CHAR)
		{
			// 通常ブレンド
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}

		if (pEffePram[nCntPrame].type == C3DEffect::TYPE::CHAR &&
			pEffePram[nCntPrame].bBillBoard == true &&
			pEffePram[nCntPrame].pParent != nullptr)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&mtxParent);
			// ビルボードの設定
			CMylibrary::SetBillboard(pDevice, &mtxParent);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans
				, pEffePram[nCntPrame].pParent->x
				, pEffePram[nCntPrame].pParent->y
				, pEffePram[nCntPrame].pParent->z);

			D3DXMatrixMultiply(&mtxParent
				, &mtxParent
				, &mtxTrans);
		}
		// ビルボードフラグが立っていた時
		else if (pEffePram[nCntPrame].bBillBoard == true)
			{// ビルボードの設定
				CMylibrary::SetBillboard(pDevice, &pEffePram[nCntPrame].Trans.mtxWorld);
			}

		// 位置を反映
		D3DXMatrixTranslation(
			&mtxTrans,
			pEffePram[nCntPrame].Trans.pos.x,
			pEffePram[nCntPrame].Trans.pos.y,
			pEffePram[nCntPrame].Trans.pos.z);

		D3DXMatrixMultiply(
			&pEffePram[nCntPrame].Trans.mtxWorld,
			&pEffePram[nCntPrame].Trans.mtxWorld,
			&mtxTrans);

		if (pEffePram[nCntPrame].type == C3DEffect::TYPE::CHAR &&
			pEffePram[nCntPrame].pParent != nullptr)
		{
			// マトリックスのずれと掛ける
			D3DXMatrixMultiply(&pEffePram[nCntPrame].Trans.mtxWorld
				, &pEffePram[nCntPrame].Trans.mtxWorld
				, &mtxParent);
		}

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pEffePram[nCntPrame].Trans.mtxWorld);

		// 頂点バッファをデータストリームにバインド
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexInfo[pEffePram[nCntPrame].nTexType]);
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPrame * _3DEFFE_USEVERTEX, 2);

		if (pEffePram[nCntPrame].type == C3DEffect::TYPE::CHAR)
		{
			// レンダーステート(加算合成処理)
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
	}

	//アルファテスト戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//Zバッファ　有効　無効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
	// ライティングモード有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	// 裏面(左回り)をカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//-------------------------------------------------------------------------------------------------------------
// 設定
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Set(SETINGPARAM & Seting)
{
	// 変数宣言
	PARAMETER* pEffePram;							// パラメータポインタ
	pEffePram = &m_EffectPram[0];					// ポインタの初期化

	// 最大数ループ
	for (int nCntEffect = 0; nCntEffect < _3DEFFE_USEQUANTITY; nCntEffect++)
	{
		// 使用フラグオンの個体の時
		if (pEffePram[nCntEffect].bUse == true)
		{// 処理をスキップ
			continue;
		}
		// 変数定義
		VERTEX_3D *pVtx;		// 頂点ポインタ
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点ポインタの更新
		pVtx += nCntEffect * 4;

		// 親の位置の設定
		pEffePram[nCntEffect].pParent = Seting.pParent;
		// 描画フラグを立てる
		pEffePram[nCntEffect].bDisp = true;
		// ビルボードの設定
		pEffePram[nCntEffect].bBillBoard = Seting.bBillBoard;
		// 重力の設定
		pEffePram[nCntEffect].fGravity = Seting.fGravity;
		// ライフの設定
		pEffePram[nCntEffect].nLife = Seting.nLife;
		// テクスチャ種類の設定
		pEffePram[nCntEffect].nTexType = Seting.nTexType;
		// エフェクト種類の設定
		pEffePram[nCntEffect].type = (C3DEffect::TYPE)Seting.type;
		// 移動量の設定
		pEffePram[nCntEffect].move = Seting.move;
		// 半径の設定
		pEffePram[nCntEffect].fRadius = Seting.fRadius;
		// 位置の設定
		pEffePram[nCntEffect].Trans.pos = Seting.pos;
		// 色の設定
		pEffePram[nCntEffect].col = Seting.col;

		if (pEffePram[nCntEffect].type == TYPE::CHAR)
		{
			pEffePram[nCntEffect].Vibrat.bRandDist = true;
			// 半径変化値の設定
			pEffePram[nCntEffect].fRadiusValue = 0.0f;
			// アルファ変化値の設定
			pEffePram[nCntEffect].fAlphaValue = 0.0f;
		}
		else
		{
			pEffePram[nCntEffect].Vibrat.bRandDist = false;
			// アルファ変化値の設定
			pEffePram[nCntEffect].fAlphaValue = pEffePram[nCntEffect].col.a / pEffePram[nCntEffect].nLife;

			// 半径変化値の設定
			pEffePram[nCntEffect].fRadiusValue = -(pEffePram[nCntEffect].fRadius / pEffePram[nCntEffect].nLife);
			pEffePram[nCntEffect].fRadiusValue = Seting.fRadiusValue;
		}
		SetVartexSize(pVtx, pEffePram[nCntEffect]);
		SetVetexColor(pVtx, pEffePram[nCntEffect]);

		// 使用フラグをオン
		pEffePram[nCntEffect].bUse = true;
		//頂点データをアンロック
		m_pVtxBuff->Unlock();
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// テクスチャの作成
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::CreateTexture(CONST_STRING SetingStr, const int &nCntInfo, LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	if (D3DXCreateTextureFromFile(pDevice, SetingStr, &m_pTexInfo[nCntInfo]) != D3D_OK)
	{
#ifdef _DEBUG
		MessageBox(NULL, "テクスチャの生成に失敗しました", "警告！", MB_ICONWARNING);
		cout << nCntInfo << "目の3Dエフェクトのテクスチャの生成に失敗しました。";
#endif
	}
}

//-------------------------------------------------------------------------------------------------------------
// 頂点の作成
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DEffect::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	VERTEX_3D *pVtx;		// 頂点情報のポインタ

	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * _3DEFFE_USEVERTEX * _3DEFFE_USEQUANTITY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntEffect = 0; nCntEffect < _3DEFFE_USEQUANTITY; nCntEffect++, pVtx += _3DEFFE_USEVERTEX)
	{
		pVtx[0].pos = MYLIB_3DVECTOR_ZERO;
		pVtx[1].pos = MYLIB_3DVECTOR_ZERO;
		pVtx[2].pos = MYLIB_3DVECTOR_ZERO;
		pVtx[3].pos = MYLIB_3DVECTOR_ZERO;
		// 法線ベクトル
		pVtx[0].nor = MYLIB_VEC3_ZONE;
		pVtx[1].nor = MYLIB_VEC3_ZONE;
		pVtx[2].nor = MYLIB_VEC3_ZONE;
		pVtx[3].nor = MYLIB_VEC3_ZONE;
		// 頂点カラー
		pVtx[0].col = MYLIB_D3DXCOR_UNSET;
		pVtx[1].col = MYLIB_D3DXCOR_UNSET;
		pVtx[2].col = MYLIB_D3DXCOR_UNSET;
		pVtx[3].col = MYLIB_D3DXCOR_UNSET;
		// texture座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;

}

//-------------------------------------------------------------------------------------------------------------
// 頂点サイズの設定
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVartexSize(VERTEX_3D * pVtx, PARAMETER & Effect)
{
	// 頂点の設定
	pVtx[0].pos.x = Effect.Vibrat.Pos.x + -sinf(m_cfBaseAngle + Effect.fAngle) * Effect.fRadius;
	pVtx[0].pos.y = Effect.Vibrat.Pos.y + -cosf(m_cfBaseAngle + Effect.fAngle) * Effect.fRadius;
	pVtx[1].pos.x = Effect.Vibrat.Pos.x + -sinf(m_cfBaseAngle - Effect.fAngle) * Effect.fRadius;
	pVtx[1].pos.y = Effect.Vibrat.Pos.y + cosf(m_cfBaseAngle - Effect.fAngle) * Effect.fRadius;
	pVtx[2].pos.x = Effect.Vibrat.Pos.x + sinf(m_cfBaseAngle - Effect.fAngle) * Effect.fRadius;
	pVtx[2].pos.y = Effect.Vibrat.Pos.y + -cosf(m_cfBaseAngle - Effect.fAngle) * Effect.fRadius;
	pVtx[3].pos.x = Effect.Vibrat.Pos.x + sinf(m_cfBaseAngle + Effect.fAngle) * Effect.fRadius;
	pVtx[3].pos.y = Effect.Vibrat.Pos.y + cosf(m_cfBaseAngle + Effect.fAngle) * Effect.fRadius;
}

//-------------------------------------------------------------------------------------------------------------
// 頂点カラーの設定
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::SetVetexColor(VERTEX_3D * pVtx, PARAMETER & Effect)
{
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = Effect.col;
}

//-------------------------------------------------------------------------------------------------------------
// 更新処理
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::VIBRATION::Update(void)
{
	// 方向ベクトルの取得
	this->GetVecDir();
	// 距離の取得
	this->GetDist();
	// 位置の計算
	this->CalPos();
}

//-------------------------------------------------------------------------------------------------------------
// 方向ベクトルの取得
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::VIBRATION::GetVecDir(void)
{
	// 相殺分
	int nOffsetting = m_VibrationOption.nMaxDiviDir * 2;

	this->VecDir.x = ((rand() % nOffsetting) - m_VibrationOption.nMaxDiviDir)*m_VibrationOption.fScalDirValue;
	this->VecDir.y = ((rand() % nOffsetting) - m_VibrationOption.nMaxDiviDir)*m_VibrationOption.fScalDirValue;

	// 正規化する
	CMylibrary::CreateUnitVector(&VecDir, &VecDir);
}

//-------------------------------------------------------------------------------------------------------------
// 距離の取得
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::VIBRATION::GetDist(void)
{
	if (bRandDist == true)
	{
		this->fDist = (rand() % m_VibrationOption.nMaxDist) * m_VibrationOption.fScalDistValue;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 位置の計算
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::VIBRATION::CalPos(void)
{
	this->Pos = this->VecDir * this->fDist;
}
