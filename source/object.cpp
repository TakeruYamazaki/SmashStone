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
#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject(PRIORITY nPriority) : CSceneX(nPriority)
{

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
	// 要素の初期化
	m_bRelease = false;

	CSceneX::Init();
}

//=============================================================================
// 終了
//=============================================================================
void CObject::Uninit(void)
{
	CSceneX::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CObject::Update(void)
{
	CSceneX::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CObject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	D3DXMATRIX *pMtx = GetMatrix();;

	CKananLibrary::CalcMatrix(pMtx, *GetPos(), *GetRot());

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, pMtx);

	CSceneX::DrawMesh();
}

//=============================================================================
// 生成
//=============================================================================
CObject * CObject::Create(void)
{
	// メモリ確保
	CObject *pObject = new CObject(CScene::PRIORITY_OBJECT);

	// nullcheck
	if (!pObject)
	{
		// 失敗
		return nullptr;
	}

	// 初期化
	pObject->Init();

	// 値を返す
	return pObject;
}

//=============================================================================
// オブジェクトの情報設定
//=============================================================================
void CObject::SetObjInfo(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot, const LPDIRECT3DTEXTURE9 & pTexture, const int & type, const bool & bCollision)
{
	SetPos(pos);
	SetRot(rot);
	BindTex(pTexture);
	m_nType = type;
	m_bCollision = bCollision;
}

//=============================================================================
// オブジェクトの当たり判定
//=============================================================================
bool CObject::CollObject(D3DXVECTOR3 *pos, const D3DXVECTOR3 & posOld, D3DXVECTOR3 *move, const MODEL_VTX & modelVtx)
{
	bool bLand = false;
	D3DXVECTOR3 objPos = *GetPos();
	D3DXVECTOR3 objPosOld = *GetPosOld();
	MODEL_VTX	Vtx = CObjectManager::GetModelVtx(m_nType);

	if (pos->x + modelVtx.VtxMax.x > objPos.x + Vtx.VtxMin.x && pos->x + modelVtx.VtxMin.x < objPos.x + Vtx.VtxMax.x &&
		pos->y + modelVtx.VtxMax.y >= objPos.y + Vtx.VtxMin.y && pos->y + modelVtx.VtxMin.y < objPos.y + Vtx.VtxMax.y)
	{
		// ブロック前部の判定
		if (pos->z + modelVtx.VtxMax.z >= objPos.z + Vtx.VtxMin.z && posOld.z + modelVtx.VtxMax.z <= objPosOld.z + Vtx.VtxMin.z)
		{
			pos->z = objPos.z + Vtx.VtxMin.z - modelVtx.VtxMax.z;
			move->z = 0.0f;
			bLand = true;
			CDebugProc::Print("前\n");
		}
		// ブロック後部の判定
		if (pos->z + modelVtx.VtxMin.z <= objPos.z + Vtx.VtxMax.z && posOld.z + modelVtx.VtxMin.z >= objPosOld.z + Vtx.VtxMax.z)
		{
			pos->z = objPos.z + Vtx.VtxMax.z - modelVtx.VtxMin.z;
			move->z = 0.0f;
			bLand = true;
			CDebugProc::Print("後\n");
		}
	}

	if (pos->z + modelVtx.VtxMax.z > objPos.z + Vtx.VtxMin.z && pos->z + modelVtx.VtxMin.z < objPos.z + Vtx.VtxMax.z &&
		pos->y + modelVtx.VtxMax.y >= objPos.y + Vtx.VtxMin.y && pos->y + modelVtx.VtxMin.y < objPos.y + Vtx.VtxMax.y)
	{
		// ブロック左部の判定
		if (pos->x + modelVtx.VtxMax.x >= objPos.x + Vtx.VtxMin.x && posOld.x + modelVtx.VtxMax.x <= objPosOld.x + Vtx.VtxMin.x)
		{
			pos->x = objPos.x + Vtx.VtxMin.x - modelVtx.VtxMax.x;
			move->x = 0.0f;
			bLand = true;
			CDebugProc::Print("左\n");
		}
		// ブロック右部の判定
		if (pos->x + modelVtx.VtxMin.x <= objPos.x + Vtx.VtxMax.x && posOld.x + modelVtx.VtxMin.x >= objPosOld.x + Vtx.VtxMax.x)
		{
			pos->x = objPos.x + Vtx.VtxMax.x - modelVtx.VtxMin.x;
			move->x = 0.0f;
			bLand = true;
			CDebugProc::Print("右\n");
		}
	}

	if (pos->x + modelVtx.VtxMax.x > objPos.x + Vtx.VtxMin.x && pos->x + modelVtx.VtxMin.x < objPos.x + Vtx.VtxMax.x &&
		pos->z + modelVtx.VtxMax.z > objPos.z + Vtx.VtxMin.z && pos->z + modelVtx.VtxMin.z < objPos.z + Vtx.VtxMax.z)
	{
		// ブロック上部の判定
		if (pos->y + modelVtx.VtxMin.y <= objPos.y + Vtx.VtxMax.y && posOld.y + modelVtx.VtxMin.y >= objPosOld.y + Vtx.VtxMax.y)
		{
			pos->y = objPos.y + Vtx.VtxMax.y - modelVtx.VtxMin.y;
			move->y = 0.0f;
			bLand = true;
			CDebugProc::Print("上\n");
		}
		// ブロック下部の判定
		if (pos->y + modelVtx.VtxMax.y >= objPos.y + Vtx.VtxMin.y && posOld.y + modelVtx.VtxMax.y <= objPosOld.y + Vtx.VtxMin.y)
		{
			pos->y = objPos.y + Vtx.VtxMin.y - modelVtx.VtxMax.y;
			move->y = 0.0f;
			bLand = true;
			CDebugProc::Print("下\n");
		}
	}

	return bLand;
}

#ifdef _DEBUG
//=============================================================================
// ImGuiの更新
//=============================================================================
void CObject::ShowObjectInfo(char cPrintText[16])
{
	// 情報取得
	D3DXVECTOR3 pos = *GetPos();
	D3DXVECTOR3 rot = *GetRot();

	// ツリー
	if (ImGui::TreeNode(cPrintText))
	{
		// 偽オブジェクト以外
		if (cPrintText != "FakeObject")
		{
			ImGui::Text("type : %d", m_nType);
		}

		ImGui::DragFloat3("pos", pos, 0.5f);
		ImGui::DragFloat3("rot", rot, 0.05f, -D3DX_PI, D3DX_PI);
		ImGui::Checkbox("Collision", &m_bCollision);

		// 偽オブジェクト以外
		if (cPrintText != "FakeObject")
		{
			// 改行せずに間隔をあける
			ImGui::Dummy(ImVec2(200.0f, 0.0f));
			ImGui::SameLine();

			// 削除
			if (ImGui::Button("delete"))
			{
				// リリースする
				m_bRelease = true;
			}
		}

		// ツリーの最後に必ず書く
		ImGui::TreePop();
	}

	SetPos(pos);
	SetRot(rot);
}
#endif