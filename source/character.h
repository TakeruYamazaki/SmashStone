// ===================================================================
//
// キャラクター処理 [ character.h ]
// Author : Seiya Takahashi
//
// ===================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "scene.h"
#include "motion.h"
#include "charaParam.h"
#include "kananlibrary.h"

// ===================================================================
// マクロ定義
// ===================================================================
#define TIME_TRANS		(15 * ONE_SECOND_FPS)	// 変身の時間
#define LIFE_DEFAULT	(100.0f)				// ライフの初期値

// ===================================================================
// クラス定義
// ===================================================================
class CModelCharacter;
class CCapsuleCollider;
class CCharaParam;

class CCharacter : public CScene
{
public:
	enum COLLIPARTS
	{
		COLLIPARTS_NONE = -1,	// なし
		COLLIPARTS_BODY,		// 体
		COLLIPARTS_FOREARM_R,	// 右前腕
		COLLIPARTS_UPPERARM_R,	// 右上腕
		COLLIPARTS_FOREARM_L,	// 前腕
		COLLIPARTS_UPPERARM_L,	// 上腕
		COLLIPARTS_MAX			// 最大
	};

	typedef enum
	{	// 物持ちの状態
		STATE_NONE = 0,			// 何もない
		STATE_LIFT,				// 物持ち上げ
		STATE_LIFT_NEUTRAL,		// 物持ちニュートラル
		STATE_WALK,				// 物持ち歩き
		STATE_THROW,			// 物投げ
		STATE_LIFT_MAX			// 最大数
	} STATE_LIFT_OBJET;

	CCharacter(CScene::PRIORITY nPriority);			// コンストラクタ
	~CCharacter();									// デストラクタ

	void               Init(void);					// 初期化
	void               Uninit(void);				// 終了
	void               Update(void);				// 更新
	void               Draw(void);					// 描画

	inline void        SetPos(const D3DXVECTOR3 & pos)			{ m_pos = pos; }					// 位置の設定
	inline void        SetMove(const D3DXVECTOR3 & move)		{ m_move = move; }					// 移動値の設定
	inline void        SetRot(const D3DXVECTOR3 & rot)			{ m_rot = rot; }					// 回転の設定
	inline void        SetRotDest(const D3DXVECTOR3 & rotDest)	{ m_rotDest = rotDest; }			// 目的の回転の設定
	inline void        SetbWalk(const bool &bWalk)				{ m_bWalk = bWalk; }				// 歩きの設定
	inline void        SetbJump(const bool &bJump)				{ m_bJump = bJump; }				// ジャンプの設定
	inline void        SetLife(const float nLife)				{ m_nLife = nLife; }				// ライフの設定
	inline void        SetAttakHit(const bool bAttackHit)		{ m_bAttakHit = m_bAttack; }		// 攻撃を当てたフラグ
	void               SetModelType(CHARACTER_TYPE type);											// モデルの設定
	void               SetCylinderCoillider(void);													// シリンダーコライダーの設定


	inline CHARACTER_TYPE    GetCharaType(void)					{ return m_type; }					// キャラタイプの取得
	inline STATE_LIFT_OBJET  GetStateLift(void)					{ return m_StateLift; }				// 物持ち上げの状態取得
	inline D3DXVECTOR3       &GetPos(void)						{ return m_pos; }					// 位置の取得
	inline D3DXVECTOR3       GetMove(void)						{ return m_move; }					// 移動値の取得
	inline D3DXVECTOR3       GetRot(void)						{ return m_rot; }					// 回転の取得
	inline D3DXVECTOR3       GetRotDest(void)					{ return m_rotDest; }				// 回転先の取得
	inline float             GetMaxLife(void)					{ return m_param.fMaxLife; }		// 最大ライフ取得
	inline float             GetLife(void)						{ return m_nLife; }					// ライフ取得
	inline bool              GetbJump(void)						{ return m_bJump; }					// ジャンプ状態の取得
	inline void              Damage(const int nDamage)			{ m_nLife -= nDamage; }				// ダメージ処理
	inline CCapsuleCollider* GetCapCollider(int nPartsIndex)	{ return m_pCapColi[nPartsIndex]; }	// カプセルコライダーの取得

	void			   Daunted(const int nGap);						// 怯み処理
	void			   VictoryAction(void);							// 勝利行動

protected:
	CModelCharacter *m_pModelCharacter;					// モデルキャラクターのポインタ
	CHARACTER_TYPE	 m_type;							// キャラクターのタイプ
	CHARACTER_TYPE	 m_typeTrans;						// 変身時のタイプ

	LPD3DXMESH		m_pMesh;							// メッシュ情報
	LPD3DXBUFFER	m_pBuffMat;							// マテリアル情報
	DWORD			m_nNumMat;							// マテリアル情報の数

	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_posOld;								// 前回の位置
	D3DXVECTOR3 m_posBegin;								// 初期位置
	D3DXVECTOR3 m_move;									// 移動値
	D3DXVECTOR3 m_rot;									// 回転
	D3DXVECTOR3 m_rotDest;								// 目的の回転
	D3DXVECTOR3 m_rotDif;								// 回転の差
	D3DXMATRIX	m_mtxWorld;								// ワールドマトリックス
	STATE_LIFT_OBJET m_StateLift;						// 物持ち上げの状態
	CCapsuleCollider* m_pCapColi[COLLIPARTS_MAX];	// シリンダーコライダーポインタ

	CCharaParam::PLAYER_PARAM m_param;					// プレイヤーのパラメーター
	int						  m_nCntTrans;				// どれぐらいの時間変身しているか
	int						  m_nNumStone;				// 取得したストーンの数
	float					  m_nLife;					// ライフ
	bool					  m_bJump;					// ジャンプしたかどうか
	bool					  m_bWalk;					// 歩いてるかどうか
	bool					  m_bTrans;					// 変身しているか
	bool					  m_bAttack;				// 攻撃しているか
	bool					  m_bBlowAway;				// 吹き飛んでいるか
	bool					  m_bSmashBlowAway;			// スマッシュで吹き飛んでいるか
	bool					  m_bDaunted;				// ひるんでいるか
	bool					  m_bDown;					// ダウンしているかどうか
	bool					  m_bInvincible;			// 無敵かどうか
	static int				  m_nNumCharacter;			// キャラクターの総数
	int						  m_nCntJump;				// ジャンプモーション切り替えのカウンタ
	int						  m_nAttackFrame;			// 攻撃モーション切り替えのカウンタ
	int						  m_nAttackFlow;			// 攻撃の流れ
	int						  m_nCntGap;				// 隙のカウンタ
	int						  m_nCntDown;				// ダウンしている時間

	bool		m_bAttakHit;							// 攻撃を当てたフラグ

private:
	void Move(void);									// 移動関数
	void Rot(void);										// 回転関数
	void Motion(void);									// モーション関数
	void Trans(void);									// 変身関数
};

#endif
