//==================================================================================================================
//
// プレイヤー[player.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"
#include "sceneX.h"
#include "fade.h"
#include "character.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define TEXTURE_PLAYER "data/TEXTURE/field000.jpg"		// 読み込むテクスチャのソース先
#define PLAYER_MODEL "data/MODEL/testmodel.x"			// 読み込むモデルのソース先
#define PLAYER_MAX_MODEL (6)							// player.cpp内のモデルのパーツ数

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CCamera;
class CMotionModel;
class CMeshField;
class CInputKeyboard;
class CInputGamepad;
class CFade;
class CHitPoint;

//==================================================================================================================
//
// プレイヤークラスの定義
//
//==================================================================================================================
class CPlayer : public CCharacter
{
public:
	CPlayer(PRIORITY type);							// コンストラクタ
	~CPlayer();										// デストラクタ
	void Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

	static CPlayer *Create(int nPlayer, CHARACTER_TYPE type);	// 生成処理

	int GetNumStone(void)	{ return m_nNumStone; }	// 取得したストーンの数を取得
	int GetnPlayer(void) { return m_nPlayer; }		// プレイヤー番号取得
	inline int GetBoxColliderID(void) { return m_nBoxColliderID; }
	CPlayer*GetAnotherPlayer(void);					// 違うプレイヤーの取得
protected:

private:
	void Control(void);								// 操作関数
	void Collision(void);							// 当たり判定関数
	void Smash(void);								// スマッシュ処理
	void NormalAttack(void);						// 通常攻撃処理
	void Jump(void);								// ジャンプ処理
	void Lift(void);								// 物持ち処理

	void CollisionAttack(void);						// 攻撃判定

	static CHitPoint *m_pHitPoint;					// HP情報

	void ControlGamepad(CInputGamepad *pGamepad);	// ゲームパッド操作
	void ControlKeyboard(CInputKeyboard *pKeyboard);// キーボード操作
	void CatchStone(void);							// ストーンの取得判定
	void SetnPlayer(int nPlayerNum);				// プレイヤー番号設定処理

	void AnotherPlayerAttack0(CPlayer *pAnother);	// 別のプレイヤーが攻撃0している時
	void AnotherPlayerAttack1(CPlayer *pAnother);	// 別のプレイヤーが攻撃1している時
	void AnotherPlayerAttack2(CPlayer *pAnother);	// 別のプレイヤーが攻撃2している時
	void AnotherPlayerAttack3(CPlayer *pAnother);	// 別のプレイヤーが攻撃3している時
	void AnotherPlayerSmash(CPlayer *pAnother);		// 別のプレイヤーがスマッシュ攻撃している時

	void TakeDamage(CPlayer * pAnother);							// ダメージを受ける
	void TakeAttack3Damage(CPlayer * pAnother);					// 攻撃3のダメージを受ける
	void TakeSmashDamage(CPlayer * pAnother);						// 攻撃3のダメージを受ける


	bool BlowAway(CPlayer *pAnother, const float MoveVecY, const float fBlowAwayForce);		// 吹き飛ぶ

	int m_nPlayer;			// プレイヤー番号
	int m_nBoxColliderID;	// ボックスコライダーID

#ifdef _DEBUG
	void ShowDebugInfo(void);									// ImGuiの更新
#endif
};
#endif