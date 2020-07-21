//=============================================================================
//
// オブジェクト処理 [ object.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "debugProc.h"
#include "objManager.h"
#include "3DBoxCollider.h"

#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================
#ifdef _DEBUG
#define COLOR_ALPHA		(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f))	// 透明なカラー
#endif

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject()
{
	// 要素の初期化
	m_bCollision	= false;
	m_pos			= ZeroVector3;
	m_posOld		= ZeroVector3;
	m_posBegin		= ZeroVector3;
	m_move			= ZeroVector3;
	m_rot			= ZeroVector3;
	m_rotBegin		= ZeroVector3;
	m_nColliderID	= -1;
#ifdef _DEBUG
	m_bRelease = false;
#endif
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
// 初期化
//=============================================================================
void CObject::Init(void)
{
}

//=============================================================================
// 終了
//=============================================================================
void CObject::Uninit(void)
{
}

//=============================================================================
// 更新
//=============================================================================
void CObject::Update(void)
{
	// 前回の位置を保存
	m_posOld = m_pos;

	// 位置更新
	m_pos += m_move;
}

//=============================================================================
// 描画
//=============================================================================
void CObject::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マテリアル格納用
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// テクスチャの設定
	if (m_pModelInfo.bTex)
		pDevice->SetTexture(0, m_pModelInfo.pTexture);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelInfo.matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo.matNum; nCntMat++)
	{
		// 半透明にする
		pMat[nCntMat].MatD3D.Diffuse.a = 1.0f;
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		// 描画
		m_pModelInfo.mesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

#ifdef _DEBUG
//=============================================================================
// 透明描画
//=============================================================================
void CObject::DrawAlpha(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// マテリアル格納用
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// テクスチャの設定
	if (m_pModelInfo.bTex)
		pDevice->SetTexture(0, m_pModelInfo.pTexture);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pModelInfo.matBuff->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModelInfo.matNum; nCntMat++)
	{
		// 半透明にする
		pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		// 描画
		m_pModelInfo.mesh->DrawSubset(nCntMat);
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}
#endif

//=============================================================================
// 生成
//=============================================================================
CObject * CObject::Create(void)
{
	// メモリ確保
	CObject *pObject = new CObject();

	// nullcheck
	if (!pObject)
		// 失敗
		return nullptr;

	// 初期化
	pObject->Init();

	// 値を返す
	return pObject;
}

//=============================================================================
// オブジェクトの情報設定
//=============================================================================
void CObject::SetObjInfo(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, MODELINFO *pModelInfo, const int & type, const bool & bCollision)
{
	m_pos = pos;
	m_posBegin = pos;
	m_rot = rot;
	m_rotBegin = rot;
	m_pModelInfo = *pModelInfo;
	m_nType = type;
	m_bCollision = bCollision;

	// コライダーの設定
	SetCollider();
}

//=============================================================================
// オブジェクトの当たり判定
//=============================================================================
bool CObject::CollObject(D3DXVECTOR3 *pos, const D3DXVECTOR3 & posOld, D3DXVECTOR3 *move, const MODEL_VTX & modelVtx)
{
	bool bLand = false;
	MODEL_VTX	Vtx = CObjectManager::GetModelVtx(m_nType);

	if (pos->x + modelVtx.VtxMax.x > m_pos.x + Vtx.VtxMin.x && pos->x + modelVtx.VtxMin.x < m_pos.x + Vtx.VtxMax.x &&
		pos->y + modelVtx.VtxMax.y >= m_pos.y + Vtx.VtxMin.y && pos->y + modelVtx.VtxMin.y < m_pos.y + Vtx.VtxMax.y)
	{
		// ブロック前部の判定
		if (pos->z + modelVtx.VtxMax.z >= m_pos.z + Vtx.VtxMin.z && posOld.z + modelVtx.VtxMax.z <= m_posOld.z + Vtx.VtxMin.z)
		{
			pos->z = m_pos.z + Vtx.VtxMin.z - modelVtx.VtxMax.z;
			move->z = 0.0f;
		}
		// ブロック後部の判定
		if (pos->z + modelVtx.VtxMin.z <= m_pos.z + Vtx.VtxMax.z && posOld.z + modelVtx.VtxMin.z >= m_posOld.z + Vtx.VtxMax.z)
		{
			pos->z = m_pos.z + Vtx.VtxMax.z - modelVtx.VtxMin.z;
			move->z = 0.0f;
		}
	}

	if (pos->z + modelVtx.VtxMax.z > m_pos.z + Vtx.VtxMin.z && pos->z + modelVtx.VtxMin.z < m_pos.z + Vtx.VtxMax.z &&
		pos->y + modelVtx.VtxMax.y >= m_pos.y + Vtx.VtxMin.y && pos->y + modelVtx.VtxMin.y < m_pos.y + Vtx.VtxMax.y)
	{
		// ブロック左部の判定
		if (pos->x + modelVtx.VtxMax.x >= m_pos.x + Vtx.VtxMin.x && posOld.x + modelVtx.VtxMax.x <= m_posOld.x + Vtx.VtxMin.x)
		{
			pos->x = m_pos.x + Vtx.VtxMin.x - modelVtx.VtxMax.x;
			move->x = 0.0f;
		}
		// ブロック右部の判定
		if (pos->x + modelVtx.VtxMin.x <= m_pos.x + Vtx.VtxMax.x && posOld.x + modelVtx.VtxMin.x >= m_posOld.x + Vtx.VtxMax.x)
		{
			pos->x = m_pos.x + Vtx.VtxMax.x - modelVtx.VtxMin.x;
			move->x = 0.0f;
		}
	}

	if (pos->x + modelVtx.VtxMax.x > m_pos.x + Vtx.VtxMin.x && pos->x + modelVtx.VtxMin.x < m_pos.x + Vtx.VtxMax.x &&
		pos->z + modelVtx.VtxMax.z > m_pos.z + Vtx.VtxMin.z && pos->z + modelVtx.VtxMin.z < m_pos.z + Vtx.VtxMax.z)
	{
		// ブロック上部の判定
		if (pos->y + modelVtx.VtxMin.y <= m_pos.y + Vtx.VtxMax.y && posOld.y + modelVtx.VtxMin.y >= m_posOld.y + Vtx.VtxMax.y)
		{
			pos->y = m_pos.y + Vtx.VtxMax.y - modelVtx.VtxMin.y;
			move->y = 0.0f;
			// 上に載っている
			bLand = true;
		}
		// ブロック下部の判定
		if (pos->y + modelVtx.VtxMax.y >= m_pos.y + Vtx.VtxMin.y && posOld.y + modelVtx.VtxMax.y <= m_posOld.y + Vtx.VtxMin.y)
		{
			pos->y = m_pos.y + Vtx.VtxMin.y - modelVtx.VtxMax.y;
			move->y = 0.0f;
		}
	}

	return bLand;
}

//=============================================================================
// コライダーの設定
//=============================================================================
void CObject::SetCollider(void)
{
	// 設定されているとき
	if (m_nColliderID != -1)
	{// 処理を抜ける
		return;
	}
	// コライダーIDの設定
	m_nColliderID = C3DBoxCollider::SetColliderInfo(&m_pos, NULL, C3DBoxCollider::COLLIDER_SUB_NORMAL, C3DBoxCollider::TOP_OBJECT + m_nType);
}

#ifdef _DEBUG
//=============================================================================
// ImGuiの更新
//=============================================================================
void CObject::ShowObjectInfo(char cPrintText[16])
{
	// ツリー
	if (ImGui::TreeNode(cPrintText))
	{
		// 偽オブジェクト以外
		if (cPrintText != "FakeObject")
			ImGui::Text("type : %d", m_nType);

		ImGui::DragFloat3("pos", m_pos, 0.5f);
		ImGui::DragFloat3("rot", m_rot, 0.05f, -D3DX_PI, D3DX_PI);
		ImGui::Checkbox("Collision", &m_bCollision);

		// 偽オブジェクト以外
		if (cPrintText != "FakeObject")
		{
			// 改行せずに間隔をあける
			ImGui::Dummy(ImVec2(200.0f, 0.0f));
			ImGui::SameLine();

			// 削除
			if (ImGui::Button("delete"))
				// リリースする
				m_bRelease = true;
		}

		// ツリーの最後に必ず書く
		ImGui::TreePop();
	}
}
#endif