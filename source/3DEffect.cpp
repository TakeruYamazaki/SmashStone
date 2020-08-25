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

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define _3DEFFE_FILENAME "data/TEXT/Effect/Texture/EffectTexture.txt"
#define _3DEFFE_OPNEMODE "r"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9     C3DEffect::m_pVtxBuff                        = nullptr;						// バッファ情報
LPDIRECT3DTEXTURE9          C3DEffect::m_pTexInfo[C3DEffect::TYPE_MAX]   = MYLIB_INITSTRUCT_WITHCONST;	// テクスチャ情報
const float                 C3DEffect::m_cfBaseAngle                     = D3DX_PI * 0.25f;				// 基本角度
int                         C3DEffect::m_nNumTextureMax                  = MYLIB_INT_UNSET;				// テクスチャの最大数
C3DEffect::PARAMETER        C3DEffect::m_EffectPram[_3DEFFE_USEQUANTITY] = MYLIB_INITSTRUCT_WITHCONST;	// エフェクトのパラメータ
C3DEffect::VIBRATION_OPTION C3DEffect::m_VibrationOption                 = MYLIB_INITSTRUCT_WITHCONST;	// 振動のオプション

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Load(void)
{
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

		if (sscanf(aRead, "FILENAME = %s", aWork) == 1)
		{
			// テクスチャの作成
			CreateTexture(aWork, nCntInfo, pDevice);
			// 情報カウントを進める
			nCntInfo++;
		}
		else if (sscanf(aRead, "MAXDIST = %d", &SetingOption.nMaxDist) == 1)
		{
			m_VibrationOption.nMaxDist = SetingOption.nMaxDist;
		}
		else if (sscanf(aRead, "SCALEDIST = %f", &SetingOption.fScalDistValue) == 1)
		{
			m_VibrationOption.fScalDistValue = SetingOption.fScalDistValue;
		}
		else if (sscanf(aRead, "MAXDIVIDIR = %d", &SetingOption.nMaxDiviDir) == 1)
		{
			m_VibrationOption.nMaxDiviDir = SetingOption.nMaxDiviDir;
		}
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
		pEffePram[nCntPrame].bDisp            = false;								// 描画フラグ
		pEffePram[nCntPrame].bBillBoard       = true;								// ビルボードフラグ
		pEffePram[nCntPrame].Trans            = TRANSFORM();						// トランス情報
		pEffePram[nCntPrame].move             = MYLIB_VEC3_UNSET;					// 移動量
		pEffePram[nCntPrame].col              = MYLIB_D3DXCOR_UNSET;				// 色
		pEffePram[nCntPrame].fRadius          = MYLIB_FLOAT_UNSET;					// 半径
		pEffePram[nCntPrame].fAngle           = MYLIB_FLOAT_UNSET;					// 角度
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
		if (pEffePram[nCntPrame].type == TYPE_CHAR)
		{
			// 振動の更新処理
			pEffePram[nCntPrame].Vibrat.Update();
		}

		// 半径を変化させる
		pEffePram[nCntPrame].fRadius -= pEffePram[nCntPrame].fRadiusValue;

		// 半径が0.0f以下の時
		if (pEffePram[nCntPrame].fRadius <= 0.0f)
		{// 半径を0.0fに固定
			pEffePram[nCntPrame].fRadius = 0.0f;
			// 使用フラグをオフにする
			pEffePram[nCntPrame].bUse = false;
			// 頂点サイズの設定
			SetVartexSize(pVtx, pEffePram[nCntPrame]);
			// 処理をスキップ
			continue;
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
			// 処理を抜ける
			continue;
		}
		// 頂点カラーの設定
		SetVetexColor(pVtx, pEffePram[nCntPrame]);
	}

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void C3DEffect::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;						// デバイスのポインタ
	D3DXMATRIX mtxTrans, mtxView;					// 計算用マトリックス
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
		
		// ビルボードフラグが立っていた時
		if (pEffePram[nCntPrame].bBillBoard == true)
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
	this->VecDir.z = ((rand() % nOffsetting) - m_VibrationOption.nMaxDiviDir)*m_VibrationOption.fScalDirValue;

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
