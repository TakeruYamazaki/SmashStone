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
#include "game.h"
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
#ifdef _DEBUG
bool CKananLibrary::m_WireFrame = false;
int CKananLibrary::m_nCulling = 0;
#endif

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
// 影のマトリックス計算
//=============================================================================
void CKananLibrary::CalcShadowMatrix(D3DXMATRIX & mtxShadow, D3DXVECTOR3 const & pos, D3DXVECTOR3 const & nor)
{
	D3DXPLANE planeField;
	D3DXVECTOR3 LightVec = CManager::GetRenderer()->GetGame()->GetLight()->GetLightVec(0);	// ライトの取得
	D3DXVECTOR4 ShadowVec = ZeroVector4;

	// マトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);

	// 光と逆向きにベクトルを設定
	ShadowVec = D3DXVECTOR4(-LightVec.x, -LightVec.y, -LightVec.z, 0);

	// 平面の座標を決める
	D3DXPlaneFromPointNormal(&planeField, &pos, &nor);

	// 影のマトリックスを決定
	D3DXMatrixShadow(&mtxShadow, &ShadowVec, &planeField);
}

//=============================================================================
// メッシュから頂点座標の最大最小を出力
//=============================================================================
MODEL_VTX CKananLibrary::OutputModelVtxColl(LPD3DXMESH mesh)
{
	int			nNumVertices = mesh->GetNumVertices();					// 頂点数取得
	DWORD		sizeFVF = D3DXGetFVFVertexSize(mesh->GetFVF());		// 頂点フォーマットのサイズを取得
	BYTE		*pVertexBuffer;	// 頂点バッファのポインタ
	MODEL_VTX	outVtx;			// 出力する頂点情報

								// 頂点バッファをロック
	mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

	// 頂点数分繰り返す
	for (int nCntVtx = 0; nCntVtx < nNumVertices; nCntVtx++)
	{
		// バッファから頂点座標を取得
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		// 全ての頂点情報を比較して、最小値・最大値を抜き出す
		// 最小値
		if (outVtx.VtxMin.x > vtx.x)
			outVtx.VtxMin.x = vtx.x;
		if (outVtx.VtxMin.y > vtx.y)
			outVtx.VtxMin.y = vtx.y;
		if (outVtx.VtxMin.z > vtx.z)
			outVtx.VtxMin.z = vtx.z;
		// 最大値
		if (outVtx.VtxMax.x < vtx.x)
			outVtx.VtxMax.x = vtx.x;
		if (outVtx.VtxMax.y < vtx.y)
			outVtx.VtxMax.y = vtx.y;
		if (outVtx.VtxMax.z < vtx.z)
			outVtx.VtxMax.z = vtx.z;

		// サイズ分ポインタを進める
		pVertexBuffer += sizeFVF;
	}

	// 頂点バッファをアンロック
	mesh->UnlockVertexBuffer();

	// 値を返す
	return outVtx;
}

//=============================================================================
// モデル情報の破棄
//=============================================================================
void CKananLibrary::ReleaseModelInfo(MODELINFO *pModelInfo)
{
	// バッファの開放
	if (pModelInfo->matBuff)
	{
		pModelInfo->matBuff->Release();
		pModelInfo->matBuff = nullptr;
	}
	// メッシュの開放
	if (pModelInfo->mesh)
	{
		pModelInfo->mesh->Release();
		pModelInfo->mesh = nullptr;
	}
	if (pModelInfo->pTexture)
	{
		//pModelInfo->pTexture->Release();
		pModelInfo->pTexture = nullptr;
	}

	// マテリアル数の初期化
	pModelInfo->matNum = 0;
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
// 距離計算ん
//=============================================================================
float CKananLibrary::OutputDistance(D3DXVECTOR3 difpos)
{
	return difpos.x * difpos.x + difpos.y * difpos.y + difpos.z * difpos.z;
}

//=============================================================================
// ビルボード化処理
//=============================================================================
void CKananLibrary::Billboard(D3DXMATRIX *mtxWorld, const D3DXMATRIX mtxView)
{
	mtxWorld->_11 = mtxView._11;
	mtxWorld->_12 = mtxView._21;
	mtxWorld->_13 = mtxView._31;
	mtxWorld->_21 = mtxView._12;
	mtxWorld->_22 = mtxView._22;
	mtxWorld->_23 = mtxView._32;
	mtxWorld->_31 = mtxView._13;
	mtxWorld->_32 = mtxView._23;
	mtxWorld->_33 = mtxView._33;
}

//=============================================================================
// 慣性の処理
//=============================================================================
void CKananLibrary::InertiaMove(D3DXVECTOR3 *move)
{
	move->x += (0 - move->x) * 0.3f;
	move->y += (0 - move->y) * 0.3f;
	move->z += (0 - move->z) * 0.3f;
}

void CKananLibrary::InertiaMoveXZ(D3DXVECTOR3 * move)
{
	move->x += (0 - move->x) * 0.3f;
	move->z += (0 - move->z) * 0.3f;
}

//=============================================================================
// 重力の処理
//=============================================================================
void CKananLibrary::Gravity(float & fMoveY)
{
	fMoveY -= GRAVITY;
}

//=============================================================================
// int型の上限設定
//=============================================================================
void CKananLibrary::IntegerUpperLimit(int * nValue, int upperLimit)
{
	if (*nValue > upperLimit)
	{
		*nValue = upperLimit;
	}
}

//=============================================================================
// int型の下限設定
//=============================================================================
void CKananLibrary::IntegerLowerLimit(int *nValue, int lowerLimit)
{
	if (*nValue < lowerLimit)
	{
		*nValue = lowerLimit;
	}
}

//=============================================================================
// float型の下限設定
//=============================================================================
bool CKananLibrary::FloatLowerLimit(float * fValue, const float & lowerLimit)
{
	bool bLimit = false;

	if (*fValue < lowerLimit)
	{
		*fValue = lowerLimit;
		bLimit = true;
	}
	return bLimit;
}

//=============================================================================
// 上限加減の制限処理
//=============================================================================
bool CKananLibrary::LimitVector3(D3DXVECTOR3 &Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit)
{
	bool bLimit = false;

	if (Value.x > upperLimit.x)
	{
		Value.x = upperLimit.x;
		bLimit = true;
	}
	if (Value.x < lowerLimit.x)
	{
		Value.x = lowerLimit.x;
		bLimit = true;
	}
	if (Value.y > upperLimit.y)
	{
		Value.y = upperLimit.y;
	}
	if (Value.y < lowerLimit.y)
	{
		Value.y = lowerLimit.y;
	}
	if (Value.z > upperLimit.z)
	{
		Value.z = upperLimit.z;
		bLimit = true;
	}
	if (Value.z < lowerLimit.z)
	{
		Value.z = lowerLimit.z;
		bLimit = true;
	}

	return bLimit;
}

bool CKananLibrary::LimitVector2(D3DXVECTOR2 & Value, const D3DXVECTOR2 lowerLimit, const D3DXVECTOR2 upperLimit)
{
	bool bLimit = false;

	if (Value.x > upperLimit.x)
	{
		Value.x = upperLimit.x;
		bLimit = true;
	}
	if (Value.x < lowerLimit.x)
	{
		Value.x = lowerLimit.x;
		bLimit = true;
	}
	if (Value.y > upperLimit.y)
	{
		Value.y = upperLimit.y;
	}
	if (Value.y < lowerLimit.y)
	{
		Value.y = lowerLimit.y;
	}

	return bLimit;
}

//=============================================================================
// 上限加減のループ処理
//=============================================================================
void CKananLibrary::LimitLoopVector3(D3DXVECTOR3 * Value, const D3DXVECTOR3 lowerLimit, const D3DXVECTOR3 upperLimit)
{
	if (Value->x > upperLimit.x)
	{
		Value->x = lowerLimit.x;
	}
	if (Value->x < lowerLimit.x)
	{
		Value->x = upperLimit.x;
	}
	if (Value->y > upperLimit.y)
	{
		Value->y = lowerLimit.y;
	}
	if (Value->y < lowerLimit.y)
	{
		Value->y = upperLimit.y;
	}
	if (Value->z > upperLimit.z)
	{
		Value->z = lowerLimit.z;
	}
	if (Value->z < lowerLimit.z)
	{
		Value->z = upperLimit.z;
	}
}

//=============================================================================
// フォグの生成
//=============================================================================
void CKananLibrary::SetFog(const float fStartPos, const float fEndPos, const D3DXCOLOR col)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);						// フォグon
	pDevice->SetRenderState(D3DRS_FOGCOLOR, col);						// カラー設定
	pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);			// 頂点モード
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);			// テーブルモード
	pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&fStartPos));		// 開始位置
	pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&fEndPos));			// 終了位置
}

//=============================================================================
// ブロックコメント枠の表示
//=============================================================================
void CKananLibrary::PrintBlockCommentFrame(void)
{
	std::cout << "==================================================" << std::endl;
}

//=============================================================================
// ブロックコメントの開始
//=============================================================================
void CKananLibrary::StartBlockComment(const std::string & str)
{
	PrintBlockCommentFrame();
	std::cout << str << std::endl;
}

//=============================================================================
// ブロックコメントの終了
//=============================================================================
void CKananLibrary::EndBlockComment(const std::string & str)
{
	std::cout << str << std::endl;
	PrintBlockCommentFrame();
}

//=============================================================================
// 上限加減の処理
//=============================================================================
void CKananLibrary::LimitColor(D3DXCOLOR & col, const D3DXCOLOR lowerLimit, const D3DXCOLOR upperLimit)
{
	if (col.r > upperLimit.r)
	{
		col.r = upperLimit.r;
	}
	if (col.r < lowerLimit.r)
	{
		col.r = lowerLimit.r;
	}
	if (col.g > upperLimit.g)
	{
		col.g = upperLimit.g;
	}
	if (col.g < lowerLimit.g)
	{
		col.g = lowerLimit.g;
	}
	if (col.b > upperLimit.b)
	{
		col.b = upperLimit.b;
	}
	if (col.b < lowerLimit.b)
	{
		col.b = lowerLimit.b;
	}
	if (col.a > upperLimit.a)
	{
		col.a = upperLimit.a;
	}
	if (col.a < lowerLimit.a)
	{
		col.a = lowerLimit.a;
	}
}

//=============================================================================
// モデル生成
//=============================================================================
void CKananLibrary::CreateModelInfo(MODELINFO * pModelInfo)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// モデル生成
	D3DXLoadMeshFromX(pModelInfo->cModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pModelInfo->matBuff,
		NULL,
		&pModelInfo->matNum,
		&pModelInfo->mesh);
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

//=============================================================================
// キーボードで移動入力しているかの確認
//=============================================================================
HRESULT CKananLibrary::GetMoveByKeyboard(CInputKeyboard * pKey, int nPlayer)
{
	// 入力されていない
	if (	(nPlayer == PLAYER_ONE && 
		(!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_UP)	 &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_DOWN)	 &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_RIGHT) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_ONE::ONE_LEFT) &&
		!pKey->GetKeyboardTrigger(PLAYER_MOVE_KEY_ONE::ONE_JUMP) && 
		!pKey->GetKeyboardTrigger(PLAYER_MOVE_KEY_ONE::ONE_ATTACK) )) ||
			(nPlayer == PLAYER_TWO &&
		(!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_UP) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_DOWN) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_RIGHT) &&
		!pKey->GetKeyboardPress(PLAYER_MOVE_KEY_TWO::TWO_LEFT) &&
		!pKey->GetKeyboardTrigger(PLAYER_MOVE_KEY_TWO::TWO_JUMP) && 
		!pKey->GetKeyboardTrigger(PLAYER_MOVE_KEY_TWO::TWO_ATTACK))))
	{
		// 移動していない
		return E_FAIL;
	}

	// 移動している
	return S_OK;
}

//=============================================================================
// ゲームパッドで移動入力しているかの確認
//=============================================================================
HRESULT CKananLibrary::GetMoveByGamepad(CInputGamepad * pGamepad)
{
	// 入力されていない
	if ((!pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_A) &&
		!pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_B) &&
		!pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_X) &&
		!pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_Y)))
	{
		// 操作していない
		return E_FAIL;
	}

	// 操作している
	return S_OK;
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
//=============================================================================
// 座標などの情報
//=============================================================================
void CKananLibrary::ShowOffsetInfo(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move)
{
	ImGui::Text("pos (%.4f, %.4f, %.4f)", pos.x, pos.y, pos.z);
	ImGui::Text("rot (%.4f, %.4f, %.4f)", rot.x, rot.y, rot.z);
	ImGui::Text("move (%.4f, %.4f, %.4f)", move.x, move.y, move.z);
}
#endif