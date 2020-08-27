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
#include "CylinderCollider.h"
#include "motion.h"
#include "Reflection.h"
#include "hitpoint.h"
#include "PolygonCollider.h"
#include "CapsuleCollider.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define HEIGHT_CEILING	(400.0f)			// 天井の高さ

#define BLOWAWAYFORCE_SMASH		(100.0f)	// 吹き飛ばし力(スマッシュ攻撃)
#define BLOWAWAYFORCE_NORMAL	(8.0f)		// 吹き飛ばし力(通常攻撃)

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================
CHitPoint *CPlayer::m_pHitPoint = NULL;				// HP情報

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
	for (int nCnt = 0; nCnt < CStone::STONE_ID_MAX; nCnt++)
	{
		m_bGetStoneType[nCnt] = false;
	}

	// 初期化
	CCharacter::Init();
	CCharacter::SetModelType(m_type);

	// 当たり判定の設定
	this->m_nBoxColliderID = C3DBoxCollider::SetColliderInfo(&this->GetPos(), this, C3DBoxCollider::COLLIDER_SUB_NORMAL, C3DBoxCollider::ID_CHARACTER);

	m_pHitPoint = CHitPoint::Create(m_nPlayer, m_param.fMaxLife);	// プレイヤーの生成処理
	m_pHitPoint->SetnPlayerNum(m_nPlayer);							// プレイヤー番号設定
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CPlayer::Uninit(void)
{
	CCharacter::Uninit();

	m_pHitPoint = nullptr;	// 変数NULL
}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CPlayer::Update(void)
{
	CGame::GAMESTATE gameState = CManager::GetRenderer()->GetGame()->GetGameState();
	if (m_bBlowAway == false && m_bDaunted == false &&
		(gameState == CGame::GAMESTATE_NORMAL || 
		gameState == CGame::GAMESTATE_BEFORE))
		// 操作
		Control();

	// 更新
	CCharacter::Update();

	// 当たり判定
	Collision();

	// ストーンの取得判定
	CatchStone();

	// プレイヤーの番号設定
	SetnPlayer(m_nPlayer);

	char cText[8];
	sprintf(cText, "%d / ", m_pModelCharacter->GetAllFrame() - m_nAttackFrame);
	CDebugProc::Print(cText);
	sprintf(cText, "%d\n", m_pModelCharacter->GetAllFrame());
	CDebugProc::Print(cText);
	 
#ifdef _DEBUG
	CDebugProc::Print("プレイヤーの位置 [%.4f][%.4f][%.4f]\n", m_pos.x, m_pos.y, m_pos.z);

	ShowDebugInfo();

	/*if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_2))
		m_bTrans = true;*/
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
	// プレイヤー番号の保存
	pPlayer->m_nPlayer = nPlayer;
	// 初期化
	pPlayer->Init();

	// 値を返す
	return pPlayer;
}

//==================================================================================================================
// プレイヤーの操作
//==================================================================================================================
void CPlayer::Control(void)
{
	// ゲームパッド取得
	CInputGamepad *pGamepad = CManager::GetInputGamepad(m_nPlayer);
	// キーボードの取得
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();

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
	// 攻撃判定
	CollisionAttack();

	// 当たり判定位置の更新
	C3DBoxCollider::ChangePosition(this->m_nBoxColliderID, this->m_pos, MYLIB_3DVECTOR_ZERO);
	// 当たり判定
	if (C3DBoxCollider::CollisionBox(this->m_nBoxColliderID, this->m_pos, m_move))
		m_bJump = false;

	// 壁の取得
	CWall *pWall = CGame::GetWall();
	// 出力される交点
	D3DXVECTOR3 out_intersect = ZeroVector3;
	// 出力される法線
	D3DXVECTOR3 out_nor = ZeroVector3;
	// 壁との当たり判定
	if (pWall->Collision(&m_pos, &m_posOld, &out_intersect,&out_nor, m_bSmashBlowAway) == true)
	{
		// 反射フラグが立っているときかつ
		// 出力された法線がゼロじゃない時かつ
		// 出力された交点がゼロじゃない時
		if (m_bSmashBlowAway == true &&
			out_nor != ZeroVector3 &&
			out_intersect != ZeroVector3)
		{
			// ダメージ
			this->Damage(2);
			// 向きを決定
			this->m_rotDest.y =  atan2f(out_nor.x, out_nor.z);
			// 回転を補間
			CKananLibrary::InterpolationFloat(m_rotDest.y);
			// 一瞬で向きを変える
			this->m_rot.y = this->m_rotDest.y;
			CReflection::GetPlaneReflectingAfterPosAndVec(&this->m_pos,&this->m_move, &out_intersect, &this->m_move, &out_nor);
		}
	}

	for (int nCntPolyColli = 0; nCntPolyColli < CPolygonCollider::POLYCOLLI_MAX; nCntPolyColli++)
	{
		// ポリゴンコライダーの取得
		CPolygonCollider*pPolyColli = CGame::GetpolyColly(nCntPolyColli);

		// ポリゴンコライダーポインタがNULLだった時
		if (pPolyColli == NULL)
		{// スキップ
			continue;
		}

		// ポリゴンコライダーの衝突判定
		if (pPolyColli[0].Collision(&m_pos, &m_posOld, &m_move, &out_intersect, &out_nor, m_bSmashBlowAway) == true)
		{
			if (m_bSmashBlowAway == false)
			{
#ifdef _DEBUG
				CDebugProc::Print("乗っている\n");
#endif
				// 地面に乗っていたら、移動量をなくす
				if (m_move.y <= -5.0f)
					m_move.y = -5.0f;
				// ジャンプ解除
				m_bJump = false;
				// ジャンプカウンタを初期化
				m_nCntJump = 0;
			}
			else
			{
				CReflection::GetPlaneReflectingAfterPosAndVec(&this->m_pos, &this->m_move, &out_intersect, &this->m_move, &out_nor);
			}
		}
	}

	// 高さ制限
	if (m_pos.y > HEIGHT_CEILING)
	{
		m_pos.y = HEIGHT_CEILING;
		m_move.y *= -1;
	}
}

//==================================================================================================================
// スマッシュ
//==================================================================================================================
void CPlayer::Smash(void)
{
	// 条件を設定
	m_bWalk = false;
	m_bAttack = true;

	// スマッシュ
	if (m_pModelCharacter->GetMotion() == CMotion::PLAYER_SMASH_CHARGE)
		m_pModelCharacter->SetMotion(CMotion::PLAYER_SMASH);
	// スマッシュチャージ
	else if (m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH_CHARGE &&
		m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH)
		m_pModelCharacter->SetMotion(CMotion::PLAYER_SMASH_CHARGE);

	// 攻撃が当たったフラグをオフにする
	m_bAttakHit = false;
	// 攻撃フレームを設定
	m_nAttackFrame = m_pModelCharacter->GetAllFrame();
}

//==================================================================================================================
// 通常攻撃
//==================================================================================================================
void CPlayer::NormalAttack(void)
{
	int AllFrame = m_pModelCharacter->GetAllFrame();

	if (!m_bAttack && !m_bJump)
	{
		// 条件を設定
		m_bWalk = false;
		m_bAttack = true;
	}
	else if (m_bAttack)
	{
		if (AllFrame - m_nAttackFrame > AllFrame ||
			AllFrame - m_nAttackFrame < m_param.motionParam[(CCharaParam::PLAYER_ATTACK_TYPE)m_nAttackFlow - 1].CancelFrame.start)
			return;
	}

	// モーションの切り替え
	m_pModelCharacter->SetMotion((CMotion::MOTION_TYPE)(CMotion::PLAYER_ATTACK_0 + m_nAttackFlow));
	// 攻撃が当たったフラグをオフにする
	m_bAttakHit = false;
	// 攻撃フレームを設定
	m_nAttackFrame = m_pModelCharacter->GetAllFrame();
	// 攻撃の順番を設定
	m_nAttackFlow++;
	if (m_nAttackFlow >= 4)
		m_nAttackFlow = 0;

}

//==================================================================================================================
// ジャンプ
//==================================================================================================================
void CPlayer::Jump(void)
{
	// 条件を設定
	m_bJump = true;
	m_bWalk = false;

	// 移動値を設定
	m_move.y = m_param.moveParam.fJumpPower;
}

//==================================================================================================================
// 物持ち処理
//==================================================================================================================
void CPlayer::Lift(void)
{
	// 持ち上げていなければ、処理しない
	if (m_StateLift == STATE_NONE)
	{
		return;
	}
}

//==================================================================================================================
// 攻撃判定
//==================================================================================================================
void CPlayer::CollisionAttack(void)
{
	// 違うプレイヤーの取得
	CPlayer *pAnother = GetAnotherPlayer();

	// 違うプレイヤーが攻撃を当てたフラグが立ってない時
	if (pAnother->m_bAttakHit == false)
	{
		// 別のプレイヤーのモーションを比較
		switch (pAnother->m_pModelCharacter->GetMotion())
		{
		case CMotion::PLAYER_ATTACK_0:
			// 別のプレイヤーが攻撃0をしている時
			this->AnotherPlayerAttack0(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_1:
			// 別のプレイヤーが攻撃1をしている時
			this->AnotherPlayerAttack1(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_2:
			// 別のプレイヤーが攻撃2をしている時
			this->AnotherPlayerAttack2(pAnother);
			break;
		case CMotion::PLAYER_ATTACK_3:
			// 別のプレイヤーが攻撃3をしている時
			this->AnotherPlayerAttack3(pAnother);
			break;
		case CMotion::PLAYER_SMASH:
			// 別のプレイヤーがスマッシュ攻撃しているとき
			this->AnotherPlayerSmash(pAnother);
			break;
		}
	}
}

//==================================================================================================================
// 攻撃当てる準備かできているか
//==================================================================================================================
bool CPlayer::ReadyToHit(const int &nCapColliID)
{
	// 違うプレイヤーが攻撃を当てたフラグが立ってない時
	if (this->m_bAttakHit == false)
	{
		// 別のプレイヤーのモーションを比較
		switch (this->m_pModelCharacter->GetMotion())
		{
		case CMotion::PLAYER_ATTACK_0: return HitConditionAttack0(nCapColliID);
		case CMotion::PLAYER_ATTACK_1: return HitConditionAttack1(nCapColliID);
		case CMotion::PLAYER_ATTACK_2: return HitConditionAttack2(nCapColliID);
		case CMotion::PLAYER_ATTACK_3: return HitConditionAttack3(nCapColliID);
		case CMotion::PLAYER_SMASH:    return HitConditionSmash(nCapColliID);
		}
	}
	return false;
}

//==================================================================================================================
//　攻撃0を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionAttack0(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_L) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_L);
}

//==================================================================================================================
//　攻撃1を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionAttack1(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
//　攻撃2を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionAttack2(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_L) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_L);
}

//==================================================================================================================
//　攻撃3を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionAttack3(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
//　スマッシュ攻を当てる条件
//==================================================================================================================
bool CPlayer::HitConditionSmash(const int &nCapColliID)
{
	return (nCapColliID == CCapsuleCollider::TYPEID_FOREARM_R) || (nCapColliID == CCapsuleCollider::TYPEID_UPPERARM_R);
}

//==================================================================================================================
// 違うプレイヤーの取得
//==================================================================================================================
CPlayer * CPlayer::GetAnotherPlayer(void)
{
	return (this->m_nPlayer == PLAYER_ONE) ? CGame::GetPlayer(PLAYER_TWO) : CGame::GetPlayer(PLAYER_ONE);
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

	if (CGame::GetGameState() == CGame::GAMESTATE_NORMAL)
	{
		// 変身中、スマッシュ入力
		if (m_bTrans &&
			(pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_B)))
			// スマッシュ
			Smash();

		// スマッシュ系モーション中は以降の処理をしない
		if (m_pModelCharacter->GetMotion() == CMotion::PLAYER_SMASH_CHARGE ||
			m_pModelCharacter->GetMotion() == CMotion::PLAYER_SMASH)
			// 処理を終える
			return;

		// 攻撃入力
		if (pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_X) &&
			(m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH_CHARGE &&
				m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH))
		{
			// 通常攻撃
			NormalAttack();
			// 処理を終える
			return;
		}
	}

	CCamera *pCamera = CManager::GetRenderer()->GetGame()->GetCamera();	// カメラ取得

	D3DXVECTOR3 rotDest = GetRotDest();				// 目的の向きを格納する変数
	D3DXVECTOR3 *vecCamera = pCamera->GetVec();		// カメラの向いている方向の取得
	float		CameraRotY = pCamera->GetRotY();	// カメラのY軸回転の取得
	float		fSpeed = 0.0f;						// プレイヤーの速度
	float		fAngle;								// スティック角度の計算用変数

	if (!m_bJump && !m_bAttack && 
		pGamepad->GetTrigger(CInputGamepad::JOYPADKEY_A))
	{
		// ジャンプ
		Jump();
	}

	if (m_bAttack)
	{
		m_bWalk = false;
		return;
	}

	if (m_bDown)
	{
		m_bWalk = false;
		return;
	}

	// 角度の計算して補正
	fAngle = atan2f(fValueX, fValueY);
	CKananLibrary::InterpolationFloat(fAngle);

	// スティックの倒れ具合でスピードを決定
	if (abs(fValueX) > abs(fValueY))
		fSpeed = (abs(fValueX));		// 横の倒れ具合
	else
		fSpeed = (abs(fValueY));		// 縦の倒れ具合

	// スティックの角度によってプレイヤー移動
	m_move.x += sinf(fAngle + CameraRotY) * fSpeed * m_param.moveParam.fRunSpeed;
	m_move.z += cosf(fAngle + CameraRotY) * fSpeed * m_param.moveParam.fRunSpeed;

	// 目的の向きを決定
	if (fValueX != 0 || fValueY != 0)
	{
		rotDest.y = D3DX_PI + fAngle + CameraRotY;
		m_bWalk = true;
	}

	if (!m_bJump)
	{
		if (!m_bWalk)
			// 歩き始めはモーションリセット
			m_pModelCharacter->ResetMotion();
		// 歩いている
		m_bWalk = true;
	}

	// 回転の補正
	CKananLibrary::InterpolationRot(&rotDest);

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

	// 変数宣言
	CCamera *pCamera = CManager::GetRenderer()->GetGame()->GetCamera();	// カメラ取得
	D3DXVECTOR3 rotDest = GetRotDest();										// 目的の向きを格納する変数
	float		CameraRotY = pCamera->GetRotY();								// カメラのY軸回転の取得

	if (CGame::GetGameState() == CGame::GAMESTATE_NORMAL)
	{
		// 変身中、スマッシュ入力
		if (m_bTrans &&
			((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardTrigger(ONE_SMASH)) ||
			(m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardTrigger(TWO_SMASH))))
			// スマッシュ
			Smash();

		// スマッシュ系モーション中は以降の処理をしない
		if (m_pModelCharacter->GetMotion() == CMotion::PLAYER_SMASH_CHARGE ||
			m_pModelCharacter->GetMotion() == CMotion::PLAYER_SMASH)
			// 処理を終える
			return;

		// 攻撃入力
		if ((((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardTrigger(ONE_ATTACK)) ||
			(m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardTrigger(TWO_ATTACK))) &&
			m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH_CHARGE &&
			m_pModelCharacter->GetMotion() != CMotion::PLAYER_SMASH))
		{
			// 通常攻撃
			NormalAttack();
			// 処理を終える
			return;
		}
	}

	if (m_StateLift == STATE_NONE &&
		((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardTrigger(ONE_LIFT)) ||
		(m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardTrigger(TWO_LIFT))))
	{
		m_StateLift = STATE_LIFT;
	}

	// 物持ち上げの処理
	Lift();

	if (!m_bJump && !m_bAttack &&
		(m_nPlayer == PLAYER_ONE && (pKeyboard->GetKeyboardTrigger(ONE_JUMP)) ||
			m_nPlayer == PLAYER_TWO && (pKeyboard->GetKeyboardTrigger(TWO_JUMP))))
	{
		// ジャンプ
		Jump();
	}

	if (m_bAttack)
	{
		m_bWalk = false;
		return;
	}

	if (m_bDown)
	{
		m_bWalk = false;
		return;
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
			m_move.x += sinf(-D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = D3DX_PI * 0.75f + CameraRotY;
		}
		// Sキー長押し
		else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_DOWN)) ||
			m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_DOWN))
		{
			// 左下移動
			m_move.x += sinf(-D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = D3DX_PI * 0.25f + CameraRotY;
		}
		// Aキーのみ
		else
		{
			// 左移動
			m_move.x += sinf(-D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(-D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
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
			m_move.x += sinf(D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.75f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.75f + CameraRotY;
		}
		// Sキー長押し
		else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_DOWN)) ||
			m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_DOWN))
		{
			// 右下移動
			m_move.x += sinf(D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.25f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.25f + CameraRotY;
		}
		// Dキーのみ
		else
		{
			// 右移動
			m_move.x += sinf(D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			m_move.z -= cosf(D3DX_PI * 0.5f - CameraRotY) * m_param.moveParam.fRunSpeed;
			// 目的の向きを決定
			rotDest.y = -D3DX_PI * 0.5f + CameraRotY;
		}
	}
	// Wキー長押し
	else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_UP)) ||
		m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_UP))
	{
		// 上移動
		m_move.x += sinf(D3DX_PI * 1.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		m_move.z -= cosf(D3DX_PI * 1.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		// 目的の向きを決定
		rotDest.y = -D3DX_PI * 1.0f + CameraRotY;
	}
	// Sキー長押し
	else if ((m_nPlayer == PLAYER_ONE && pKeyboard->GetKeyboardPress(ONE_DOWN)) ||
		m_nPlayer == PLAYER_TWO && pKeyboard->GetKeyboardPress(TWO_DOWN))
	{
		// 下移動
		m_move.x += sinf(D3DX_PI * 0.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		m_move.z -= cosf(D3DX_PI * 0.0f - CameraRotY) * m_param.moveParam.fRunSpeed;
		// 目的の向きを決定
		rotDest.y = CameraRotY;
	}

	if (!m_bJump)
	{
		if (!m_bWalk)
			// 歩き始めはモーションリセット
			m_pModelCharacter->ResetMotion();
		// 歩いている
		m_bWalk = true;
	}

	// 回転の補正
	CKananLibrary::InterpolationRot(&rotDest);

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
		// 出現ストーン数を減算
		CGame::RemoveNumStone(((CStone *)pScene)->GetIndexPos());
		// 取得ストーンのタイプを有効
		int nStoneID = ((CStone *)pScene)->GetStoneID();
		m_bGetStoneType[nStoneID] = true;
		// ストーンの取得
		((CStone *)pScene)->Catch();

		// ストーンの取得数を加算
		m_nNumStone++;
		// 3つ取得している
		if (m_nNumStone >= 3)
			// 変身
			this->m_bTrans = true;
	}
}

//==================================================================================================================
// 別のプレイヤーが攻撃0している時
//==================================================================================================================
void CPlayer::AnotherPlayerAttack0(CPlayer * pAnother)
{
	// if (pAnother->m_pCapColi[CCharacter::COLLIPARTS_FOREARM_L]->Collision(this->m_nBoxColliderID) == true ||
	// 	pAnother->m_pCapColi[CCharacter::COLLIPARTS_UPPERARM_L]->Collision(this->m_nBoxColliderID) == true)
	// {
	// 	// ダメージを受ける
	// 	TakeDamage();
	// 	// 当てたフラグを立てる
	// 	pAnother->m_bAttakHit = true;
	// }
}

//==================================================================================================================
// 別のプレイヤーが攻撃1している時
//==================================================================================================================
void CPlayer::AnotherPlayerAttack1(CPlayer * pAnother)
{
	//if (pAnother->m_pCyliColi[CCharacter::COLLIPARTS_FOREARM_R]->Collision(this->m_nBoxColliderID) == true ||
	//	pAnother->m_pCyliColi[CCharacter::COLLIPARTS_UPPERARM_R]->Collision(this->m_nBoxColliderID) == true)
	//{
	// ダメージを受ける
	//	TakeDamage();
	//	// 当てたフラグを立てる
	//	pAnother->m_bAttakHit = true;
	//}
}

//==================================================================================================================
// 別のプレイヤーが攻撃2している時
//==================================================================================================================
void CPlayer::AnotherPlayerAttack2(CPlayer * pAnother)
{
	//if (pAnother->m_pCyliColi[CCharacter::COLLIPARTS_FOREARM_L]->Collision(this->m_nBoxColliderID) == true ||
	//	pAnother->m_pCyliColi[CCharacter::COLLIPARTS_UPPERARM_L]->Collision(this->m_nBoxColliderID) == true)
	//{
	// ダメージを受ける
	//	TakeDamage();
	//	// 当てたフラグを立てる
	//	pAnother->m_bAttakHit = true;
	//}
}

//==================================================================================================================
// 別のプレイヤーが攻撃3している時
//==================================================================================================================
void CPlayer::AnotherPlayerAttack3(CPlayer * pAnother)
{
	//if (pAnother->m_pCyliColi[CCharacter::COLLIPARTS_FOREARM_R]->Collision(this->m_nBoxColliderID) == true ||
	//	pAnother->m_pCyliColi[CCharacter::COLLIPARTS_UPPERARM_R]->Collision(this->m_nBoxColliderID) == true)
	//{
	//	// ダメージ
	//	this->Damage(2);
	//	// 変身中以外は吹き飛ぶ
	//	if (!m_bTrans)
	//	{
	//		// 吹き飛び
	//		BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_NORMAL);
	//		// 吹き飛びを有効
	//		m_bBlowAway = true;
	//		if (m_nNumStone > 0)
	//		{
	//			// 所持ストーンを一つ減らすn
	//			m_nNumStone--;
	//			// 減らしたストーンを即生成
	//			CGame::AppearStone();
	//		}
	//	}
	//	// 当てたフラグを立てる
	//	pAnother->m_bAttakHit = true;
	//}
}

//==================================================================================================================
// 別のプレイヤーがスマッシュ攻撃している時
//==================================================================================================================
void CPlayer::AnotherPlayerSmash(CPlayer * pAnother)
{
	//// シリンダーコライダーの衝突判定
	//if (pAnother->m_pCyliColi[CCharacter::COLLIPARTS_FOREARM_R]->Collision(this->m_nBoxColliderID) == true ||
	//	pAnother->m_pCyliColi[CCharacter::COLLIPARTS_UPPERARM_R]->Collision(this->m_nBoxColliderID) == true)
	//{
	//	// ダメージ
	//	this->Damage(2);
	//	// 変身中以外は吹き飛ぶ
	//	BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_SMASH);
	//	// スマッシュによる吹き飛びを実行
	//	m_bSmashBlowAway = true;
	//	// 当てたフラグを立てる
	//	pAnother->m_bAttakHit = true;
	//}
}

//==================================================================================================================
//ダメージを受ける
//==================================================================================================================
void CPlayer::TakeDamage(CPlayer * pAnother)
{
	// ダメージ
	this->Damage(2);
	if (!m_bTrans)
		// 怯み
		this->Daunted(20);

	// 当てたフラグを立てる
	pAnother->m_bAttakHit = true;
}

//==================================================================================================================
// 攻撃3のダメージを受ける
//==================================================================================================================
void CPlayer::TakeAttack3Damage(CPlayer * pAnother)
{
	// ダメージ
	this->Damage(2);
	// 変身中以外は吹き飛ぶ
	if (!m_bTrans)
	{
		// 吹き飛び
		BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_NORMAL);
		// 吹き飛びを有効
		m_bBlowAway = true;
		if (m_nNumStone > 0)
		{
			// 所持ストーンを一つ減らす
			m_nNumStone--;
			// 再配置できるようストーンを使用されていない状態にする
			CGame::RemoveTypeStone(CKananLibrary::DecideRandomValue(m_nNumStone + 1, m_bGetStoneType));
			// 減らしたストーンを即生成
			CGame::AppearStone();
		}
	}
	// 当てたフラグを立てる
	pAnother->m_bAttakHit = true;
}

//==================================================================================================================
// スマッシュ攻撃のダメージを受ける
//==================================================================================================================
void CPlayer::TakeSmashDamage(CPlayer * pAnother)
{
	// ダメージ
	this->Damage(2);
	// 変身中以外は吹き飛ぶ
	BlowAway(pAnother, 0.5f, BLOWAWAYFORCE_SMASH);
	// スマッシュによる吹き飛びを実行
	m_bSmashBlowAway = true;
	// 当てたフラグを立てる
	pAnother->m_bAttakHit = true;
}

//==================================================================================================================
// プレイヤー番号設定処理
//==================================================================================================================
void CPlayer::SetnPlayer(int nPlayerNum)
{
	m_nPlayer = nPlayerNum;
}

//==================================================================================================================
// 吹き飛ぶ
//==================================================================================================================
inline bool CPlayer::BlowAway(CPlayer *pAnother, const float MoveVecY, const float fBlowAwayForce)
{
	// NULLだった時処理しない
	if (pAnother == NULL)
	{
		return false;
	}
	// 変数宣言
	D3DXVECTOR3 MoveVec;	// 移動ベクトル

	MoveVec.x = sinf(pAnother->m_rot.y + D3DX_PI);
	MoveVec.y = MoveVecY;
	MoveVec.z = cosf(pAnother->m_rot.y + D3DX_PI);

	// ぶっ飛びモーション
	m_pModelCharacter->ResetMotion();
	m_pModelCharacter->SetMotion(CMotion::PLAYER_BLOWAWAY);
	// 向きを決定
	m_rotDest.y = pAnother->m_rot.y + D3DX_PI;
	// 回転の補間
	CKananLibrary::InterpolationFloat(m_rotDest.y);
	// 一瞬で向きを変える
	m_rot.y = m_rotDest.y;

	// 移動値に加算
	this->m_move.x = MoveVec.x * fBlowAwayForce;
	this->m_move.z = MoveVec.z * fBlowAwayForce;
	this->m_move.y = MoveVec.y * fBlowAwayForce;

	return true;
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
		int nAllFrame = m_pModelCharacter->GetAllFrame();
		// 情報の表示
		CKananLibrary::ShowOffsetInfo(GetPos(), GetRot(), GetMove());
		ImGui::Text("nLife       : %f", m_nLife);
		ImGui::Text("bJump       : %d", m_bJump);
		ImGui::Text("bWalk       : %d", m_bWalk);
		ImGui::Text("bDown       : %d", m_bDown);
		ImGui::Text("bAttack     : %d", m_bAttack);
		if (m_bAttack)
		{
			ImGui::Text("AttackFlow  : %d", m_nAttackFlow);
			ImGui::Text("AttackFrame : %d / %d", nAllFrame - m_nAttackFrame, nAllFrame);
		}
		ImGui::Text("GetNumStone : %d", m_nNumStone);
		if (m_bTrans)
			ImGui::Text("TransTime   : %d", TIME_TRANS - m_nCntTrans);
	}
}
#endif