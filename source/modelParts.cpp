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
}

CModelParts::CModelParts(PRIORITY nPriority) : CSceneX(nPriority)
{
	//初期化
	m_pParentMtx = nullptr;
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
	// シーンの初期化
	CSceneX::Init();
}

//=============================================================================
// 終了
//=============================================================================
void CModelParts::Uninit(void)
{
	// シーンの終了
	CSceneX::Uninit();
}

//=============================================================================
//更新
//=============================================================================
void CModelParts::Update(void)
{
	//現在の回転首都kj
	D3DXVECTOR3 rot = *GetRot();

	//回転に加算
	rot += m_rot1F;

	//更新した回転情報を代入
	CSceneX::SetRot(rot);

	//更新
	CSceneX::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CModelParts::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxShadow;		// 影のワールドマトリックス
	D3DXMATRIX			DrawMtx;		// 描画用のワールドマトリックス
	D3DXMATRIX			*mtxWorld = GetMatrix();	// モデルのマトリックス

	// マトリックス計算
	CKananLibrary::CalcMatrix(mtxWorld, *GetPos(), *GetRot());
	D3DXMatrixMultiply(mtxWorld, mtxWorld, m_pParentMtx);

	// モデルの描画
	CSceneX::DrawMesh();

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
// 親のマトリックス設定
//=============================================================================
void CModelParts::SetParentMtx(D3DXMATRIX *mtx)
{
	m_pParentMtx = mtx;
}

//=============================================================================
// いろんな情報セット (テクスチャ付き)
//=============================================================================
void CModelParts::SetPartsTexInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture)
{
	CSceneX::SetPos(pos);
	CSceneX::SetRot(rot);
	m_nParent = nParent;
	m_nIndex = nIndex;
	CSceneX::BindTex(pTexture);
}

//=============================================================================
// いろんな情報セット
//=============================================================================
void CModelParts::SetPartsInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CSceneX::SetPos(pos);
	CSceneX::SetRot(rot);
	m_nParent = nParent;
	m_nIndex = nIndex;
}

//=============================================================================
// 目的の回転の設定
//=============================================================================
void CModelParts::SetMotionRotDest(CMotion::MOTION_TYPE motiontype, int nKey)
{
	D3DXVECTOR3 rotDest = CMotion::GetRotDest(motiontype, nKey, m_nIndex);		// 目的の回転
	D3DXVECTOR3 rot = *GetRot();													// 現在の回転
	int nFrame = CMotion::GetFrame(motiontype, nKey);							// かかるフレーム数

	// 1Fあたりの回転量を取得
	m_rot1F = (rotDest - rot) / (float)nFrame;
}