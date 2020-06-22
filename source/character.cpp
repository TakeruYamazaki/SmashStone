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

//=============================================================================
// マクロ定義
//=============================================================================	
#define SPEED_ROT		(0.1f)	// 回転のスピード
#define LIFE_DEFAULT	(100)	// ライフの初期値

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
	m_posBegin			= ZeroVector3;
	m_rot				= ZeroVector3;
	m_rotDest			= ZeroVector3;
	m_rotDif			= ZeroVector3;
	m_move				= ZeroVector3;
	m_nCntMove			= 0;
	m_bJump				= false;
	m_bWalk				= false;
	m_nMaxLife			= LIFE_DEFAULT;
	m_nLife				= m_nMaxLife;

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

	// ワールドマトリックスの計算
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);
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

	// ワールドマトリックスの設定
	m_pModelCharacter->SetCharacterMtx(&m_mtxWorld);
}

//=============================================================================
// 移動の処理
//=============================================================================
void CCharacter::Move(void)
{
	D3DXVECTOR3 difMove;	// 現在の移動値と目的の移動値の差

	// 慣性
	CKananLibrary::InertiaMove(&m_move);

	//移動量加算
	m_pos += m_move;
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
	if (!m_bWalk)
		m_pModelCharacter->SetMotion(CMotion::PLAYER_NEUTRAL);
	else
		m_pModelCharacter->SetMotion(CMotion::MOTION_NONE);
}

//=============================================================================
// 変身の処理
//=============================================================================
void CCharacter::Trans(void)
{
	if (m_bTrans)
	{
		m_pModelCharacter->ModelRebind(CHARACTER_1_TRANS);
	}
}
