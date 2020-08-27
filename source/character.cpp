//=============================================================================
//
// キャラクター処理  [ character.cpp ]
// Author : Seiya Takahashi
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "character.h"
#include "Inputkeyboard.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "modelCharacter.h"
#include "debugProc.h"
#include "motion.h"
#include "game.h"
#include "meshField.h"
#include "modelParts.h"
#include "CapsuleCollider.h"

//=============================================================================
// マクロ定義
//=============================================================================	
#define SPEED_ROT			(0.1f)		// 回転のスピード
#define TIME_MAX_DOWN		(60)		// 最大までダウンできる時間

#define INERTIA_SMASH		(0.005f)	// スマッシュ吹き飛び時の慣性
#define INERTIA_BLOWAWAY	(0.02f)		// 通常吹き飛び時の慣性

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
int CCharacter::m_nNumCharacter = 0;

//=============================================================================
// コンストラクタ
//=============================================================================
CCharacter::CCharacter(PRIORITY nPriority) : CScene(nPriority)
{
	// 要素の初期化
	m_pModelCharacter	= nullptr;
	m_pos				= ZeroVector3;
	m_posOld			= ZeroVector3;
	m_posBegin			= ZeroVector3;
	m_rot				= ZeroVector3;
	m_rotDest			= ZeroVector3;
	m_rotDif			= ZeroVector3;
	m_move				= ZeroVector3;
	m_nCntTrans			= 0;
	m_nAttackFlow		= 0;
	m_nAttackFrame		= 0;
	m_nCntGap			= 0;
	m_nCntJump			= 0;
	m_nNumStone 		= 0;
	m_nLife				= 0;
	m_bDown				= false;
	m_bAttack			= false;
	m_bJump				= false;
	m_bWalk				= false;
	m_bBlowAway			= false;
	m_bSmashBlowAway	= false;
	m_bDaunted			= false;
	m_StateLift			= STATE_NONE;
	
	// 総数を加算
	m_nNumCharacter++;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCharacter::~CCharacter()
{
	// nullcheck
	if (m_pModelCharacter)
	{
		// モデルキャラクターを破棄
		delete m_pModelCharacter;
		m_pModelCharacter = nullptr;
	}
}

//=============================================================================
// 初期化
//=============================================================================
void CCharacter::Init()
{
}

//=============================================================================
// 終了
//=============================================================================
void CCharacter::Uninit()
{
	//モデルの終了処理
	m_pModelCharacter->Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CCharacter::Update()
{
	// 移動処理
	Move();

	// 回転処理
	Rot();

	// モーション処理
	Motion();

	// 変身処理
	Trans();

	// ワールドマトリックスの設定
	m_pModelCharacter->SetCharacterMtx(&m_mtxWorld);

	// モデルの更新
	m_pModelCharacter->Update();

	if (!GetbMotionAttack())
	{
		CDebugProc::Print("攻撃してない\n");
	}
	else
	{
		CDebugProc::Print("攻撃してる\n");
	}

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

#ifdef _DEBUG
	CInputKeyboard *pKeyboard = CManager::GetInputKeyboard();		// キーボードの取得

	// キーボードの[5]を押したとき
	if (pKeyboard->GetKeyboardPress(DIK_5))
	{
		// 体力を減らす
		m_nLife -= 1;
	}
	// キーボードの[6]を押したとき
	if (pKeyboard->GetKeyboardPress(DIK_6))
	{
		// 体力を増やす
		m_nLife += 1;
	}
	// キーボードの[6]を押したとき
	if (pKeyboard->GetKeyboardPress(DIK_7))
	{
		// 体力を増やす
		m_nLife = m_param.fMaxLife;
	}

#endif // _DEBUG

	if (m_nLife <= 0)
		m_nLife = 0;
	if (m_nLife >= m_param.fMaxLife)
		m_nLife = m_param.fMaxLife;
}

//=============================================================================
// 描画
//=============================================================================
void CCharacter::Draw()
{
	// モデルの描画
	m_pModelCharacter->Draw();
}

//=============================================================================
// モデルの設定
//=============================================================================
void CCharacter::SetModelType(CHARACTER_TYPE type)
{
	// モデルの生成
	m_pModelCharacter = CModelCharacter::Create(type);

	m_type = type;
	m_typeTrans = (CHARACTER_TYPE)(type + 1);

	// タイプごとに最大HP設定
	m_param = CCharaParam::GetPlayerParam((PARAM_TYPE)(m_type / 2));
	m_nLife = m_param.fMaxLife;

	// ワールドマトリックスの設定
	m_pModelCharacter->SetCharacterMtx(&m_mtxWorld);

	// シリンダーコライダーの設定
	SetCylinderCoillider();
}

//=============================================================================
// シリンダーコライダーの設定
//=============================================================================
void CCharacter::SetCylinderCoillider(void)
{
	// パーツのポインタ
	CModelParts *pParts = m_pModelCharacter->GetModelParts();

	// 右前腕の設定
	m_pCapColi[CCharacter::COLLIPARTS_FOREARM_R] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_LOWARM_R].GetMtx(), CCharacter::COLLIPARTS_FOREARM_R);
	// 右上腕の設定
	m_pCapColi[CCharacter::COLLIPARTS_UPPERARM_R] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_UPARM_R].GetMtx(), CCharacter::COLLIPARTS_UPPERARM_R);
	// 左前腕の設定
	m_pCapColi[CCharacter::COLLIPARTS_FOREARM_L] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_LOWARM_L].GetMtx(), CCharacter::COLLIPARTS_FOREARM_L);
	// 左上腕の設定
	m_pCapColi[CCharacter::COLLIPARTS_UPPERARM_L] =
		CCapsuleCollider::Create(this, pParts[CModelParts::PARTSNAME_UPARM_L].GetMtx(), CCharacter::COLLIPARTS_UPPERARM_L);

	// 体の設定
	m_pCapColi[CCharacter::COLLIPARTS_BODY] =
		CCapsuleCollider::Create(this, &m_mtxWorld, CCharacter::COLLIPARTS_BODY);

}

//=============================================================================
// モーションの攻撃判定の取得
//=============================================================================
inline bool CCharacter::GetbMotionAttack(void)
{
	// 現在のモーションキーが攻撃中かを取得
	bool bMotionAttack = CMotion::GetbAttack((PARAM_TYPE)(m_type / 2), m_pModelCharacter->GetMotion(), m_pModelCharacter->GetNowKey());
	// 値を返す
	return bMotionAttack;
}

//=============================================================================
// ひるみ処理
//=============================================================================
void CCharacter::Daunted(const int nGap)
{
	// カウントを設定し、怯み状態に移行
	m_pModelCharacter->ResetMotion();
	m_nCntGap = nGap;
	m_bDaunted = true;
	m_pModelCharacter->SetMotion(CMotion::PLAYER_DAUNTED);
}

//=============================================================================
// 勝利行動
//=============================================================================
void CCharacter::VictoryAction(void)
{
}

//=============================================================================
// 移動の処理
//=============================================================================
void CCharacter::Move(void)
{
	// 前回の位置を保存
	m_posOld = m_pos;

	D3DXVECTOR3 difMove;	// 現在の移動値と目的の移動値の差
	
	// 慣性
	if (m_bSmashBlowAway == true)
	{
		CMylibrary::SlowingMove(&m_move.x, INERTIA_SMASH);
		CMylibrary::SlowingMove(&m_move.y, INERTIA_SMASH);
		CMylibrary::SlowingMove(&m_move.z, INERTIA_SMASH);
	}
	else if (m_bBlowAway == true)
	{
		CMylibrary::SlowingMove(&m_move.x, INERTIA_BLOWAWAY);
		CMylibrary::SlowingMove(&m_move.y, INERTIA_BLOWAWAY);
		CMylibrary::SlowingMove(&m_move.z, INERTIA_BLOWAWAY);
	}
	else
		CKananLibrary::InertiaMoveXZ(&m_move);

	// 重力
	CKananLibrary::Gravity(m_move.y);

	//移動量加算
	m_pos += m_move;

	// 地面との高さを比較し、修正
	if (CKananLibrary::FloatLowerLimit(&m_pos.y, CManager::GetRenderer()->GetGame()->GetMeshField()->GetHeight(m_pos)))
	{
		// スマッシュ被弾時は反射
		if (m_bSmashBlowAway)
			m_move.y *= -1;
		// 地面に乗っていたら、移動量をなくす
		if (m_move.y <= -10.0f)
			m_move.y = -10.0f;
		// ジャンプ解除
		m_bJump = false;
		// ジャンプカウンタを初期化
		m_nCntJump = 0;
	}
}

//=============================================================================
// 回転の処理
//=============================================================================
void CCharacter::Rot(void)
{
	// 回転の差分を求める
	m_rotDif = m_rotDest - m_rot;

	// 回転の補間
	CKananLibrary::InterpolationRot(&m_rotDif);

	// モデルの回転
	m_rot += m_rotDif * SPEED_ROT;

	// 回転の補間
	CKananLibrary::InterpolationRot(&m_rot);
}

//=============================================================================
// モーションの処理
//=============================================================================
void CCharacter::Motion(void)
{
	if (!m_bWalk && !m_bAttack && !m_bJump && !m_bDaunted && !m_bBlowAway && !m_bDown && m_StateLift == STATE_NONE)
		m_pModelCharacter->SetMotion(CMotion::PLAYER_NEUTRAL);	// ニュートラルモーション
	if (m_bWalk && !m_bAttack && !m_bJump && !m_bDaunted && !m_bBlowAway && !m_bDown && m_StateLift == STATE_NONE)
		m_pModelCharacter->SetMotion(CMotion::PLAYER_RUN);	// 移動モーション

	switch (m_StateLift)
	{
		// 持ち上げ
	case STATE_LIFT:
		m_pModelCharacter->SetMotion(CMotion::PLAYER_LIFT);
		break;
	case STATE_LIFT_NEUTRAL:
		m_pModelCharacter->SetMotion(CMotion::PLAYER_LIFT_NEUTRAL);
		break;
	case STATE_WALK:
		m_pModelCharacter->SetMotion(CMotion::PLAYER_LIFT);
		break;
	case STATE_THROW:
		break;
	}

	// ジャンプニュートラル
	if (m_bJump && !m_bDaunted && !m_bBlowAway && !m_bDown)
	{
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
		// ジャンプカウンタを加算
		m_nCntJump++;
		// 最初はジャンプモーション
		if (m_nCntJump <= 15)
			m_pModelCharacter->SetMotion(CMotion::PLAYER_JUMP);
		// 以降は落下モーション
		else
			m_pModelCharacter->SetMotion(CMotion::PLAYER_FALL);
	}
	// ダウン中
	if (m_bDown)
	{
		// ダウンカウント加算
		m_nCntDown++;
		m_pModelCharacter->SetMotion(CMotion::PLAYER_DOWN);
		// ダウン終了
		if (m_nCntDown >= TIME_MAX_DOWN)
		{
			// ダウン状態解除
			m_bDown = false;
			// カウント初期化
			m_nCntDown = 0;
		}
	}

	// 攻撃中
	if (m_bAttack)
	{
		// 攻撃フレームを減算
		m_nAttackFrame--;
		// 攻撃終了後
		if (m_nAttackFrame <= 0)
		{
			// 攻撃解除
			m_bAttack = false;
			// 攻撃の状態を初期化
			m_nAttackFlow = 0;
			// 攻撃フレームを初期化
			m_nAttackFrame = 0;
		}
	}

	// 怯み中
	if (m_bDaunted)
	{
		// 後隙フレーム減算
		m_nCntGap--;
		// 攻撃の状態を初期化
		m_nAttackFlow = 0;
		// 怯み終了
		if (m_nCntGap <= 0)
		{
			// 怯み解除
			m_bDaunted = false;
			// 後隙フレームを初期化
			m_nCntGap = 0;
		}
	}

	// 吹っ飛び中
	if (m_bBlowAway)
	{
		// 地面に着く
		if (m_move.y <= -3.0f)
		{
			// 吹き飛び終了
			m_bBlowAway = false;
			// ダウン開始
			m_bDown = true;
		}
	}

	// スマッシュ吹き飛び中
	if (m_bSmashBlowAway)
	{
		// 吹き飛びの威力が落ちる
		if (abs(m_move.x) <= 5.0f &&
			abs(m_move.z) <= 5.0f &&
			abs(m_move.y) <= 5.0f)
		{
			// 吹き飛び終了
			m_bSmashBlowAway = false;
			// ダウン開始
			m_bDown = true;
		}
	}
}

//=============================================================================
// 変身の処理
//=============================================================================
void CCharacter::Trans(void)
{
	if (!m_bTrans)
	{
		// 変身時間を初期化
		m_nCntTrans = 0;
		return;
	}

	// 変身時間を加算
	m_nCntTrans++;
	if (m_nCntTrans < TIME_TRANS)
	{
		// モデルを変身用にバインド
		m_pModelCharacter->ModelRebind(m_typeTrans);
		// モデルタイプを再設定
		m_pModelCharacter->SetModelType(m_typeTrans);
		return;
	}

	// ストーンの取得数を初期化
	m_nNumStone = 0;
	// ストーンの出現数を初期化
	CGame::SetNumStone(0);	
	for (int nCnt = 0; nCnt < CStone::STONE_ID_MAX; nCnt++)
	{
		m_bGetStoneType[nCnt] = false;
		// 再配置できるようストーンを使用されていない状態にする
		CGame::RemoveTypeStone(nCnt);
	}
	// 変身時間を初期化
	m_nCntTrans = 0;
	// 変身を解除
	m_bTrans = false;
	// モデルの再バインド
	m_pModelCharacter->ModelRebind(m_type);
	// モデルタイプを再設定
	m_pModelCharacter->SetModelType(m_type);
}
