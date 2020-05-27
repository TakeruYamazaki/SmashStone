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
//#include "ImGui/imgui.h"				// Imguiの実装に必要
//#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
//#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CObject::CObject(PRIORITY type = CSceneX::PRIORITY_PLAYER) :CSceneX(type)
{
	// 要素の初期化
	m_bRelease = false;
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
	CSceneX::Draw();
}

//=============================================================================
// 生成
//=============================================================================
std::unique_ptr<CObject> CObject::Create(void)
{
	// メモリ確保
	std::unique_ptr<CObject> pObject = std::make_unique<CObject>();

	// 初期化
	pObject->Init();

	// 値を返す
	return pObject;
}

//=============================================================================
// モデル情報の格納
//=============================================================================
void CObject::SetObjInfo(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	SetPos(pos);
	SetRot(rot);
}

//=============================================================================
// オブジェクトの当たり判定
//=============================================================================
/*bool CObject::CollObject(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	bool bLand = false;
	D3DXVECTOR3 objPos = GetPos();
	D3DXVECTOR3 objPosOld = GetPosOld();

	if (pos->x + vtxMax.x > objPos.x + m_vtxMin.x && pos->x + vtxMin.x < objPos.x + m_vtxMax.x &&
		pos->y + vtxMax.y >= objPos.y + m_vtxMin.y && pos->y + vtxMin.y < objPos.y + m_vtxMax.y)
	{
		// ブロック前部の判定
		if (pos->z + vtxMax.z >= objPos.z + m_vtxMin.z && posOld->z + vtxMax.z <= objPosOld.z + m_vtxMin.z)
		{
			pos->z = objPos.z + m_vtxMin.z - vtxMax.z;
			move->z = 0.0f;
			bLand = true;
			CDebugProc::Print("前\n");
		}
		// ブロック後部の判定
		if (pos->z + vtxMin.z <= objPos.z + m_vtxMax.z && posOld->z + vtxMin.z >= objPosOld.z + m_vtxMax.z)
		{
			pos->z = objPos.z + m_vtxMax.z - vtxMin.z;
			move->z = 0.0f;
			bLand = true;
			CDebugProc::Print("後\n");
		}
	}

	if (pos->z + vtxMax.z > objPos.z + m_vtxMin.z && pos->z + vtxMin.z < objPos.z + m_vtxMax.z &&
		pos->y + vtxMax.y >= objPos.y + m_vtxMin.y && pos->y + vtxMin.y < objPos.y + m_vtxMax.y)
	{
		// ブロック左部の判定
		if (pos->x + vtxMax.x >= objPos.x + m_vtxMin.x && posOld->x + vtxMax.x <= objPosOld.x + m_vtxMin.x)
		{
			pos->x = objPos.x + m_vtxMin.x - vtxMax.x;
			move->x = 0.0f;
			bLand = true;
			CDebugProc::Print("左\n");
		}
		// ブロック右部の判定
		if (pos->x + vtxMin.x <= objPos.x + m_vtxMax.x && posOld->x + vtxMin.x >= objPosOld.x + m_vtxMax.x)
		{
			pos->x = objPos.x + m_vtxMax.x - vtxMin.x;
			move->x = 0.0f;
			bLand = true;
			CDebugProc::Print("右\n");
		}
	}

	if (pos->x + vtxMax.x > objPos.x + m_vtxMin.x && pos->x + vtxMin.x < objPos.x + m_vtxMax.x &&
		pos->z + vtxMax.z > objPos.z + m_vtxMin.z && pos->z + vtxMin.z < objPos.z + m_vtxMax.z)
	{
		// ブロック上部の判定
		if (pos->y + vtxMin.y <= objPos.y + m_vtxMax.y && posOld->y + vtxMin.y >= objPosOld.y + m_vtxMax.y)
		{
			pos->y = objPos.y + m_vtxMax.y - vtxMin.y;
			move->y = 0.0f;
			bLand = true;
			CDebugProc::Print("上\n");
		}
		// ブロック下部の判定
		if (pos->y + vtxMax.y >= objPos.y + m_vtxMin.y && posOld->y + vtxMax.y <= objPosOld.y + m_vtxMin.y)
		{
			pos->y = objPos.y + m_vtxMin.y - vtxMax.y;
			move->y = 0.0f;
			bLand = true;
			CDebugProc::Print("下\n");
		}
	}

	return bLand;
}*/

/*#ifdef _DEBUG
//=============================================================================
// ImGuiの更新
//=============================================================================
void CObject::ShowObjectInfo(char cPrintText[16])
{
	// 情報取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	int nType = m_type;

	// ツリー
	if (ImGui::TreeNode(cPrintText))
	{
		// 偽オブジェクト以外
		if (cPrintText != "FakeObject")
		{
			ImGui::Text("type : %d", m_type);
		}

		ImGui::DragFloat3("pos", pos, 0.5f);
		ImGui::DragFloat3("rot", rot, 0.05f, -D3DX_PI, D3DX_PI);

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

	// 座標・回転の更新
	SetPos(pos);
	SetRot(rot);
}
#endif*/