//=============================================================================
//
//	ライブラリ処理 [ kananlibrary.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "kananlibrary.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "inputKeyboard.h"
#include "inputGamepad.h"
#include "light.h"
#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 構造体定義
//=============================================================================

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
bool CKananLibrary::m_WireFrame = false;
int CKananLibrary::m_nCulling = 0;

//=============================================================================
// マトリックス計算
//=============================================================================
void CKananLibrary::CalcMatrix(D3DXMATRIX * pMtx, const D3DXVECTOR3 & pos, const D3DXVECTOR3 & rot)
{
	D3DXMATRIX	mtxRot, mtxTrans;			//計算用

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtx);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);
}

//=============================================================================
// 回転の補間
//=============================================================================
void CKananLibrary::InterpolationRot(D3DXVECTOR3 *rot)
{
	// 3.14以上
	if (rot->x > D3DX_PI)
	{
		rot->x -= D3DX_PI * 2;
	}
	// 3.14以下
	if (rot->x < -D3DX_PI)
	{
		rot->x += D3DX_PI * 2;
	}
	// 3.14以上
	if (rot->y > D3DX_PI)
	{
		rot->y -= D3DX_PI * 2;
	}
	// 3.14以下
	if (rot->y < -D3DX_PI)
	{
		rot->y += D3DX_PI * 2;
	}
	// 3.14以上
	if (rot->z > D3DX_PI)
	{
		rot->z -= D3DX_PI * 2;
	}
	// 3.14以下
	if (rot->z < -D3DX_PI)
	{
		rot->z += D3DX_PI * 2;
	}
}

//=============================================================================
// float型の回転の補間
//=============================================================================
void CKananLibrary::InterpolationFloat(float & Float)
{
	// 3.14以上
	if (Float > D3DX_PI)
	{
		Float -= D3DX_PI * 2;
	}
	// 3.14以下
	if (Float < -D3DX_PI)
	{
		Float += D3DX_PI * 2;
	}
}

//=============================================================================
// 平方根計算
//=============================================================================
float CKananLibrary::OutputSqrt(D3DXVECTOR3 difpos)
{
	float fSqrt = sqrt(difpos.x * difpos.x + difpos.y * difpos.y + difpos.z * difpos.z);
	return fSqrt;
}

//=============================================================================
// 距離計算
//=============================================================================
float CKananLibrary::OutputDistance(D3DXVECTOR3 difpos)
{
	return difpos.x * difpos.x + difpos.y * difpos.y + difpos.z * difpos.z;
}

//=============================================================================
// 慣性の処理
//=============================================================================
void CKananLibrary::InertiaMove(D3DXVECTOR3 *move)
{
	move->x += (0 - move->x) * INERTIA;
	move->y += (0 - move->y) * INERTIA;
	move->z += (0 - move->z) * INERTIA;
}

//=============================================================================
// ブロックコメント枠の表示
//=============================================================================
void CKananLibrary::PrintBlockCommentFrame(void)
{
	std::cout << "==================================================" << std::endl;
}

//=============================================================================
// 上入力
//=============================================================================
HRESULT CKananLibrary::Up(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// キーボード入力
	if (Key && Key->GetKeyboardTrigger(DIK_W))
		return TRUE;

	// ゲームパッド接続確認
	if (pGamepad->GetbConnect())
	{
		// 左スティック取得
		float fValueX, fValueZ;
		pGamepad->GetStickLeft(&fValueX, &fValueZ);

		// スティック入力
		if (fValueX <= 1.0f && fValueZ > 0.0f)
			return TRUE;

		// キー入力
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_UP))
			return TRUE;
	}

	// 入力無し
	return FALSE;
}

//=============================================================================
// 下入力
//=============================================================================
HRESULT CKananLibrary::Down(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// キーボード入力
	if (Key && Key->GetKeyboardTrigger(DIK_S))
		return TRUE;

	// ゲームパッド接続確認
	if (pGamepad->GetbConnect())
	{
		// 左スティック取得
		float fValueX, fValueZ;
		pGamepad->GetStickLeft(&fValueX, &fValueZ);

		// スティック入力
		if (fValueX >= -1.0f && fValueZ < 0.0f)
			return TRUE;

		// キー入力
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_DOWN))
			return TRUE;
	}

	// 入力無し
	return FALSE;
}

//=============================================================================
// 左入力
//=============================================================================
HRESULT CKananLibrary::Left(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// キーボード入力
	if (Key && Key->GetKeyboardTrigger(DIK_A))
		return TRUE;

	// ゲームパッド接続確認
	if (pGamepad->GetbConnect())
	{
		// 左スティック取得
		float fValueX, fValueZ;
		pGamepad->GetStickLeft(&fValueX, &fValueZ);

		// スティック入力
		if (fValueX < 0 && fValueZ >= -1.0f)
			return TRUE;

		// キー入力
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_LEFT))
			return TRUE;
	}

	// 入力無し
	return FALSE;
}

//=============================================================================
// 右入力
//=============================================================================
HRESULT CKananLibrary::Right(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// キーボード入力
	if (Key && Key->GetKeyboardTrigger(DIK_D))
		return TRUE;

	// ゲームパッド接続確認
	if (pGamepad->GetbConnect())
	{
		// 左スティック取得
		float fValueX, fValueZ;
		pGamepad->GetStickLeft(&fValueX, &fValueZ);

		// スティック入力
		if (fValueX < 0 && fValueZ <= 1.0f)
			return TRUE;

		// キー入力
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_RIGHT))
			return TRUE;
	}

	// 入力無し
	return FALSE;
}

//=============================================================================
// 決定入力
//=============================================================================
HRESULT CKananLibrary::Decide(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// キーボード入力
	if (Key && Key->GetKeyboardTrigger(DIK_RETURN))
		return TRUE;

	// ゲームパッド接続確認
	if (pGamepad->GetbConnect())
	{
		// スティック入力
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_A))
			return TRUE;
	}

	// 入力無し
	return FALSE;
}

//=============================================================================
// 戻り入力
//=============================================================================
HRESULT CKananLibrary::Return(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// キーボード入力
	if (Key && Key->GetKeyboardTrigger(DIK_BACKSPACE))
		return TRUE;

	// ゲームパッド接続確認
	if (pGamepad->GetbConnect())
	{
		// スティック入力
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_B))
			return TRUE;
	}

	// 入力無し
	return FALSE;
}

//=============================================================================
// ポーズ入力
//=============================================================================
HRESULT CKananLibrary::Pause(CInputKeyboard * Key, CInputGamepad * pGamepad)
{
	// キーボード入力
	if (Key && Key->GetKeyboardTrigger(DIK_P))
		return TRUE;

	// ゲームパッド接続確認
	if (pGamepad->GetbConnect())
	{
		// スティック入力
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_START))
			return TRUE;
	}

	// 入力無し
	return FALSE;
}

#ifdef _DEBUG
//=============================================================================
// ImGuiの更新
//=============================================================================
void CKananLibrary::ShowDebugInfo(void)
{
	// ツリーのヘッダー
	if (ImGui::CollapsingHeader("DebugInfo"))
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// FPSの表示
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		//// Scene2D総数の表示
		//ImGui::Text("NumScene2D : %d", CScene2D::GetNumScene2D());
		//// Scene2D総数の表示
		//ImGui::Text("NumScene3D : %d", CScene3D::GetNumScene3D());
		//// Scene2D総数の表示
		//ImGui::Text("NumSceneX  : %d", CSceneX::GetNumSceneX());

		// ワイヤーフレーム
		if (ImGui::Checkbox("WireFrame", &m_WireFrame))
		{
			// ? A : B (条件演算子 : 真(A)偽(B)どちらかが当てはまっていれば、それを実行する)
			m_WireFrame ?
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME) :	//ワイヤー
				pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);			//通常
		}

		// 全体のカリング
		if (ImGui::TreeNode("Culling"))
		{
			// カリング
			if (ImGui::RadioButton("CCW", &m_nCulling, 0))
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// 裏面(左回り)をカリングする
			// 改行しない
			ImGui::SameLine();
			if (ImGui::RadioButton("CW", &m_nCulling, 1))
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);		// 表面(右回り)をカリングする
			// 改行しない
			ImGui::SameLine();
			if (ImGui::RadioButton("NONE", &m_nCulling, 2))
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);		// 両面カリング

			// ツリーの最後に必ず書く
			ImGui::TreePop();
		}
	}
}
#endif