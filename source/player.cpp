//==================================================================================================================
//
// プレイヤー処理[player.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "inputKeyboard.h"
#include "renderer.h"
#include "player.h"
#include "debugProc.h"
#include "camera.h"
#include "meshField.h"
#include "motionModel.h"
#include "time.h"
#include "title.h"
#include <initializer_list>
#include "tutorial.h"
#include "game.h"
#include "inputGamepad.h"
#include "character.h"
#include "scene.h"
#include "modelCharacter.h"
#include "motion.h"
#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要
#include "3DBoxCollider.h"
#include "stone.h"
#include "wall.h"

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================
#define VALUE_MOVE_PLAYER	(1.0f)	// プレイヤーの移動値
#define VALUE_JUMP			(5.0f)	// ジャンプ力の値

#define POS_1P	(D3DXVECTOR3(0.0f, 0.0f, 100.0f))	// 1Pプレイヤーの初期座標
#define POS_2P	(D3DXVECTOR3(0.0f, 0.0f, -100.0f))	// 2Pプレイヤーの初期座標

//==================================================================================================================
// マクロ定義
//==================================================================================================================

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CPlayer::CPlayer(PRIORITY type = CScene::PRIORITY_PLAYER) : CCharacter(type)
{

}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CPlayer::~CPlayer()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CPlayer::Init(void)
{
	// 要素の初期化
	m_bTrans = false;

	// 初期化
	CCharacter::Init();
	CCharacter::SetModelType(m_type);

	// 当たり判定の設定
	this->m_nBoxColliderID = C3DBoxCollider::SetColliderInfo(&this->GetPos(), this, C3DBoxCollider::COLLIDER_SUB_NORMAL, C3DBoxCollider::ID_CHARACTER);
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CPlayer::Update(void)
{
	// 操作
	Control();

	// 更新
	CCharacter::Update();

	// 当たり判定
	Collision();

	// ストーンの取得判定
	CatchStone();
	
#ifdef _DEBUG
	ShowDebugInfo();

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_2))
	{
		m_bTrans = true;
	}
#endif
}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CPlayer::Draw(void)
{
	CCharacter::Draw();
}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CPlayer *CPlayer::Create(int nPlayer, CHARACTER_TYPE type)
{
	// シーン動的に確保
	CPlayer *pPlayer = new CPlayer(CScene::PRIORITY_PLAYER);

	// 失敗
	if (!pPlayer)
		return nullptr;

	// プレイヤーのキャラタイプを設定
	pPlayer->m_type = type;
	// 初期化
	pPlayer->Init();
	// プレイヤー番号の保存
	pPlayer->m_nPlayer = nPlayer;

	// プレイヤー番号によって座標を再設定
	if (nPlayer == 0)
		pPlayer->SetPos(POS_1P);
	if (nPlayer == 1)
		pPlayer->SetPos(POS_2P);

	// 値を返す
	return pPlayer;
}

//==================================================================================================================
// プレイヤーの操作
//==================================================================================================================
void CPlayer::Control(void)
{
	CInputGamepad *pGamepad = CManager::GetInputGamepad(m_nPlayer);	// ゲームパッド取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// キーボードの取得

	if (m_nPlayer == 0)
		CDebugProc::Print("プレイヤー1操作 : WASD, SPACEキー\n");
	if (m_nPlayer == 1)
		CDebugProc::Print("プレイヤー2操作 : 矢印, 0(テンキー)\n");

	// ゲームパッド有効時
	if (pGamepad->GetbConnect())
		// ゲームパッド操作
		ControlGamepad(pGamepad);
	// ゲームパッド無効時
	else
		// キーボード操作
		ControlKeyboard(pKeyboard);
}

//==================================================================================================================
// 当たり判定処理
//==================================================================================================================
void CPlayer::Collision(void)
{
	// 当たり判定位置の更新
	C3DBoxCollider::ChangePosition(this->m_nBoxColliderID, this->m_pos, MYLIB_3DVECTOR_ZERO);
	// 当たり判定
	C3DBoxCollider::CollisionBox(this->m_nBoxColliderID, this->m_pos, m_move);


	// 壁の取得
	CWall *pWall = CGame::GetWall();

	// 出力される交点
	D3DXVECTOR3 out_intersect = ZeroVector3;
	// スマッシュによる跳ね返りを受けているか
	bool bReflection = false;

	// 壁との当たり判定
	pWall->Collision(&m_pos, &m_posOld, &out_intersect, bReflection);
}

//==================================================================================================================
// 攻撃の処理
//==================================================================================================================
void CPlayer::Attack(void)
{
	if (!m_bAttack)
		return;
}

//==================================================================================================================
// ゲームパッドの操作
//==================================================================================================================
void CPlayer::ControlGamepad(CInputGamepad * pGamepad)
{
	float fValueX, fValueY;	// ゲームパッドのスティック情報の取得用

	// 左スティック取得
	pGamepad->GetStickLeft(&fValueX, &fValueY);

	// 何も入力されていなければ、処理しない
	if (FAILED(CKananLibrary::GetMoveByGamepad(pGamepad)) && 
		fValueX == 0 && fValueY == 0)
	{
		m_bWalk = false;
		return;
	}

	// 歩いている
	SetbWalk(true);

	CCamera *pCamera = CManager::GetRenderer()->GetGame()->GetCamera();	// カメラ取得

	D3DXVECTOR3 move = GetMove();					// 移動値取得
	D3DXVECTOR3 rotDest = GetRotDest();				// 目的の向きを格納する変数
	D3DXVECTOR3 *vecCamera = pCamera->GetVec();		// カメラの向いている方向の取得
	float		CameraRotY = pCamera->GetRotY();	// カメラのY軸回転の取得
	float		fSpeed = 0.0f;						// プレイヤーの速度
	float		fAngle;								// スティック角度の計算用変数

	// 角度の計算して補正
	fAngle = atan2f(fValueX, fValueY);
	CKananLibrary::InterpolationFloat(fAngle);

	// スティックの倒れ具合でスピードを決定
	if (abs(fValueX) > abs(fValueY))
		fSpeed = (abs(fValueX));		// 横の倒れ具合
	else
		fSpeed = (abs(fValueY));		// 縦の倒れ具合

	// スティックの角度によってプレイヤー移動
	move.x += sinf(fAngle + CameraRotY) * fSpeed;
	move.z += cosf(fAngle + CameraRotY) * fSpeed;

	// 目的の向きを決定
	if (fValueX != 0 || fValueY != 0)
	{
		rotDest.y = D3DX_PI + fAngle + CameraRotY;
		m_bWalk = true;
	}

	// ジャンプ処理
	if (!m_bJump &&
		pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_A))
	{
		move.y += VALUE_JUMP;
		m_bJump = true;
	}

	if (m_bJump)
		m_bWalk = false;

	// 回転の補正
	CKananLibrary::InterpolationRot(&rotDest);

	// 移動値の設定
	SetMove(move);

	// 目的の回転の設定
	SetRotDest(rotDest);
}

//==================================================================================================================
// キーボードの操作
//==================================================================================================================
void CPlayer::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// 入力されていなければ処理を終える
	if (FAILED(CKananLibrary::GetMoveByKeyboard(pKeyboard, m_nPlayer)))
	{
		m_bWalk = false;
		return;
	}

	CCamera *pCamera = CManager::GetRenderer()->GetGame()->GetCamera();	// カメラ取得

	D3DXVECTOR3 move		= GetMove();			// 移動値取得
	D3DXVECTOR3 rotDest		= GetRotDest();			// 目的の向きを格納する変数
	float		CameraRotY	= pCamera->GetRotY();	// カメラのY軸回転の取得

	if (!m_bAttack &&
		(m_nPlayer == PLAYER_ONE && (pKeyboard->GetKeyboardTrigger(ONE_ATTACK)) ||
		m_nPlayer == PLAYER_TWO && (pKeyboard->GetKeyboardTrigger(TWO_ATTACK))))
	{
		m_bAttack = true;
	}

	if (!m_bJump && 
		(m_nPlayer == PLAYER_ONE && (pKeyboard->GetKeyboardTrigger(ONE_JUMP)) || 
		m_nPlayer == PLAYER_TWO && (pKeyboard->GetKeyboardTrigger(TWO_JUMP))))
	{
		move.y += VALUE_JUMP;
		m_bJump = true;
		m_bWalk = false;
	}

	if (m_bTrans && m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
	{
		m_bWalk = false;

		if (m_pModelCharacter->GetMotion() == CMotion::PLAYER_SMASH_CHARGE)
		{
			m_pModelCharacter->ResetMotion();
			m_pModelCharacter->SetMotion(CMotion::PLAYER_SMASH);
		}
		else if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH_CHARGE)
		{
			m_pModelCharacter->ResetMotion();
			m_pModelCharacter->SetMotion(CMotion::PLAYER_SMASH_CHARGE);
		}
	}

	// Aキー長押し
	if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_LEFT)) ||
		m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_LEFT))
	{
		// Wキー長押し
		if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_UP)) ||
			m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_UP))
		{
			// 左上移動
			move.x += sinf(-D3DX_PI * 0.75f - CameraRotY) * VALUE_MOVE_PLAYER;
			move.z -= cosf(-D3DX_PI * 0.75f - CameraRotY) * VALUE_MOVE_PLAYER;
			// 目的の向きを決定
			rotDest.y = D3DX_PI * 0.75f + CameraRotY;
		}
		// Sキー長押し
		else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_DOWN)) ||
			m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_DOWN))
		{
			// 左下移動
			move.x += sinf(-D3DX_PI * 0.25f - CameraRotY) * VALUE_MOVE_PLAYER;
			move.z -= cosf(-D3DX_PI * 0.25f - CameraRotY) * VALUE_MOVE_PLAYER;
			// 目的の向きを決定
			rotDest.y = D3DX_PI * 0.25f + CameraRotY;
		}
		// Aキーのみ
		else
		{
			// 左移動
			move.x += sinf(-D3DX_PI * 0.5f - CameraRotY) * VALUE_MOVE_PLAYER;
			move.z -= cosf(-D3DX_PI * 0.5f - CameraRotY) * VALUE_MOVE_PLAYER;
			// 目的の向きを決定
			rotDest.x = 0.0f;
			rotDest.y = D3DX_PI * 0.5f + CameraRotY;
		}
	}
	// Dキー長押し
	else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_RIGHT)) ||
		m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_RIGHT))
	{
		// Wキー長押し
		if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_UP)) ||
			m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_UP))
		{
			// 右上移動
			move.x += sinf(D3DX_PI * 0.75f - CameraRotY) * VALUE_MOVE_PLAYER;
			move.z -= cosf(D3DX_PI * 0.75f - CameraRotY) * VALUE_MOVE_PLAYER;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.75f + CameraRotY;
		}
		// Sキー長押し
		else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_DOWN)) ||
				m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_DOWN))
		{
			// 右下移動
			move.x += sinf(D3DX_PI * 0.25f - CameraRotY) * VALUE_MOVE_PLAYER;
			move.z -= cosf(D3DX_PI * 0.25f - CameraRotY) * VALUE_MOVE_PLAYER;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.25f + CameraRotY;
		}
		// Dキーのみ
		else
		{
			// 右移動
			move.x += sinf(D3DX_PI * 0.5f - CameraRotY) * VALUE_MOVE_PLAYER;
			move.z -= cosf(D3DX_PI * 0.5f - CameraRotY) * VALUE_MOVE_PLAYER;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.5f + CameraRotY;
		}
	}
	// Wキー長押し
	else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_UP)) ||
			m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_UP))
	{
		// 上移動
		move.x += sinf(D3DX_PI * 1.0f - CameraRotY) * VALUE_MOVE_PLAYER;
		move.z -= cosf(D3DX_PI * 1.0f - CameraRotY) * VALUE_MOVE_PLAYER;
		// 目的の向きを決定
		rotDest.y = -D3DX_PI * 1.0f + CameraRotY;
	}
	// Sキー長押し
	else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_DOWN)) ||
			m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_DOWN))
	{
		// 下移動
		move.x += sinf(D3DX_PI * 0.0f - CameraRotY) * VALUE_MOVE_PLAYER;
		move.z -= cosf(D3DX_PI * 0.0f - CameraRotY) * VALUE_MOVE_PLAYER;
		// 目的の向きを決定
		rotDest.y = CameraRotY;
	}

	if (!m_bJump)
		// 歩いている
		m_bWalk = true;

	/*if (pKeyboard->GetKeyboardTrigger(DIK_M))
	{
		m_bAttack = true;
	}*/

	// 回転の補正
	CKananLibrary::InterpolationRot(&rotDest);

	// 移動値の設定
	SetMove(move);

	// 目的の回転の設定
	SetRotDest(rotDest);
}

//==================================================================================================================
// ストーンの取得判定
//==================================================================================================================
void CPlayer::CatchStone(void)
{
	// 変数宣言
	int nHitID = MYLIB_INT_NOELEM;	// 当たったID
	// 当たったコライダのIDを取得
	if (C3DBoxCollider::Collisionoverlap(this->m_nBoxColliderID, &nHitID) == false)
	{
		return;
	}

	// scene取得
	CScene *pScene = C3DBoxCollider::GetScene(nHitID);	// シーンポインタ
	// 取得に失敗したとき
	if (pScene == NULL)
	{
		return;
	}
	// プライオリティがストーンのとき
	if (pScene->GetPriority() == CScene::PRIORITY_STONE)
	{
		// ストーンの取得
		((CStone *)pScene)->Catch();

		// ストーンの取得数を加算
		m_nNumStone++;
		// 出現ストーン数を減算
		CGame::RemoveNumStone();
		// 3つ取得している
		if (m_nNumStone >= 3)
			// 変身
			this->m_bTrans = true;
	}
}

#ifdef _DEBUG
//==================================================================================================================
// ImGuiの更新
//==================================================================================================================
void CPlayer::ShowDebugInfo()
{
	char cHead[8];
	sprintf(cHead, "Player%d", m_nPlayer);			// プレイヤー番号を文字列に追加

	if (ImGui::CollapsingHeader(cHead))
	{
		// 情報の表示
		CKananLibrary::ShowOffsetInfo(GetPos(), GetRot(), GetMove());
		ImGui::Text("nLife       : %f", m_nLife);
		ImGui::Text("bJump       : %d", m_bJump);
		ImGui::Text("GetNumStone : %d", m_nNumStone);
		if (m_bTrans)
			ImGui::Text("TransTime   : %d", TIME_TRANS - m_nCntTrans);
	}
}
#endif