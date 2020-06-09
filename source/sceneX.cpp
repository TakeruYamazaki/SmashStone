//==================================================================================================================
//
// Xファイルの描画[sceneX.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "manager.h"
#include "sceneX.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "debugProc.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define COLOR_SHADOW		(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f))	// 影のカラー

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CSceneX::CSceneX()
{
}

CSceneX::CSceneX(PRIORITY type) : CScene(type)
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CSceneX::~CSceneX()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CSceneX::Init(void)
{
	// 初期化
	m_pos		= ZeroVector3;	// 位置
	m_move		= ZeroVector3;	// 移動量
	m_size		= OneVector3;	// 大きさ
	m_rot		= ZeroVector3;	// 回転
	m_vecAxis	= ZeroVector3;	// 回転軸
	m_fValueRot = 0.0f;			// 回転角（回転量）
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CSceneX::Uninit(void)
{

}


//==================================================================================================================
// 更新処理
//==================================================================================================================
void CSceneX::Update(void)
{
	// 回転しすぎた分を補正する
	CKananLibrary::InterpolationFloat(m_fValueRot);


}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CSceneX::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();								// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();							// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;										// 計算用マトリックス
	D3DXMATERIAL *pMat;															// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;														// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX mtxShadow;														// シャドウマトリックス
	D3DXPLANE planeField;														// 平面変数
	D3DXVECTOR4 vecLight;														// ライトの方向ベクトル
	D3DXVECTOR3 pos, normal;													// 影の位置、法線

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelInfo->matBuff->GetBufferPointer();

	// キャラクターの影描画
	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo->matNum; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// 描画
		m_pModelInfo->mesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pModelInfo->pTexture);

	//ポリゴン描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

}

//==================================================================================================================
// メッシュの描画
//==================================================================================================================
void CSceneX::DrawMesh(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef;

	// テクスチャの設定
	pDevice->SetTexture(0, m_pModelInfo->pTexture);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelInfo->matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo->matNum; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// 描画
		m_pModelInfo->mesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}


//=============================================================================
// 影の描画
//=============================================================================
void CSceneX::DrawShadow(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef, matBlack;

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	matBlack = matDef;

	// 色の設定
	matBlack.Diffuse = COLOR_SHADOW;

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelInfo->matBuff->GetBufferPointer();

	// ========================ステンシル============================
	// [ピクセル単位で参照値を設定し、								]
	// [その値と条件を比較してピクセルの描画を行うか判断する手法	]
	// ==============================================================

	// 参照値	: D3DRS_STENCILPASS						(ステンシルテスト・Zテストを両方成功していたら +1)
	// 条件		: D3DRS_STENCILREF, D3DRS_STENCILFUNC	(参照値が0のみステンシルテスト合格)
	// よって、参照値 = 0 だと描画する					(ステンシルテストを合格しているピクセルのみ、描画される)

	// ステンシルの値を0にする
	//pDevice->Clear(0, NULL, D3DCLEAR_STENCIL, 0, 1.0f, 0);
	//
	//pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);				// ステンシルテスト有効
	//pDevice->SetRenderState(D3DRS_STENCILREF, 0);					// ステンシルの条件の値
	//pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);		// (EQUAL : 条件と同じ)

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo->matNum; nCntMat++)
	{
		// インクリメント
		//pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

		// マテリアルの設定
		pDevice->SetMaterial(&matBlack);

		// 描画
		m_pModelInfo->mesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	// ステンシルテスト無効
	//pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//==================================================================================================================
// 矩形の当たり判定
//==================================================================================================================
bool CSceneX::SetCollisionBox(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 posOld)
{
	bool bHit = false;

	// モデルの横幅の範囲内にいるとき
	if (m_pos.x - size.x < posOld.x && m_pos.x + size.x > posOld.x)
	{
		// 下からモデルのぶつかった
		if (m_pos.z - size.z > posOld.z && m_pos.z - size.z <= pos.z)
		{
			// 当たったことにする
			bHit = true;
		}
	}

	return bHit;
}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CSceneX *CSceneX::Create(void)
{
	// シーン動的に確保
	CSceneX *pSceneX = new CSceneX(CScene::PRIORITY_FIELD);

	// シーン初期化
	pSceneX->Init();

	// 値を返す
	return pSceneX;
}