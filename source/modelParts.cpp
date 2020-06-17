//=============================================================================
//
// モデルのパーツ処理  [ modelParts.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "modelparts.h"
#include "renderer.h"
#include "manager.h"
#include "Inputkeyboard.h"
#include "ImGui/imgui.h"
#include "debugproc.h"
#include "player.h"
#include "sceneX.h"
#include "kananlibrary.h"

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CModelParts::CModelParts()
{
	//初期化
	m_nIndex = 0;
	m_nParent = 0;
	m_rot1F = ZeroVector3;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModelParts::~CModelParts()
{

}

//=============================================================================
// 初期化
//=============================================================================
void CModelParts::Init(void)
{

}

//=============================================================================
// 終了
//=============================================================================
void CModelParts::Uninit(void)
{

}

//=============================================================================
//更新
//=============================================================================
void CModelParts::Update(void)
{
	// 回転に加算
	//m_rot += m_rot1F;
}

//=============================================================================
// 描画
//=============================================================================
void CModelParts::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX		mtxShadow;		// 影のワールドマトリックス
	D3DXMATRIX		DrawMtx;		// 描画用のワールドマトリックス
	D3DXMATERIAL	*pMat;
	D3DMATERIAL9	matDef;

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// 親の情報をいれる
	if (m_nParent >= 0)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, m_mtxParent);

	// テクスチャの設定
	if (m_pModelInfo->bTex)
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

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// モデルのワールド座標取得
	pDevice->GetTransform(D3DTS_WORLD, &DrawMtx);

	// 影のマトリックス計算
	CKananLibrary::CalcShadowMatrix(mtxShadow, D3DXVECTOR3(0.0f, 1.0f, 0.0f), AboveNormal);

	// 描画用のマトリックスにかけ合わせる
	D3DXMatrixMultiply(&DrawMtx, &DrawMtx, &mtxShadow);

	// モデル座標からワールド座標に設定
	pDevice->SetTransform(D3DTS_WORLD, &DrawMtx);

	// 影の描画
	//CSceneX::DrawShadow();
}

//=============================================================================
// いろんな情報セット (テクスチャ付き)
//=============================================================================
void CModelParts::SetPartsTexInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture)
{
	m_pos = pos;
	m_rot = rot;
	m_nParent = nParent;
	m_nIndex = nIndex;
	m_pTexture = pTexture;
}

//=============================================================================
// いろんな情報セット
//=============================================================================
void CModelParts::SetPartsInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;
	m_rot = rot;
	m_nParent = nParent;
	m_nIndex = nIndex;
}

//=============================================================================
// 目的の回転の設定
//=============================================================================
void CModelParts::SetMotionRotDest(CMotion::MOTION_TYPE motiontype, int nKey)
{
	D3DXVECTOR3 rotDest = CMotion::GetRotDest(motiontype, nKey, m_nIndex);		// 目的の回転
	D3DXVECTOR3 rot = m_rot;													// 現在の回転
	int nFrame = CMotion::GetFrame(motiontype, nKey);							// かかるフレーム数

	// 1Fあたりの回転量を取得
	m_rot1F = (rotDest - rot) / (float)nFrame;
}