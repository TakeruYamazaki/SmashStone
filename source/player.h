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
class CPlayer;

//==================================================================================================================
//
// プレイヤークラスの定義
//
//==================================================================================================================
class CPlayer : public CCharacter
{
public:
	// モーションのタイプ
	typedef enum
	{
		PLAYER_MOTION_NORMAL = 0,			// ニュートラル
		PLAYER_MOTION_WALK,					// 歩き
		PLAYER_MOTION_ACTION,				// アクション
		PLAYER_MOTION_JUMP,					// ジャンプ
		PLAYER_MOTION_LANDING,				// 着地
		PLAYER_MOTION_DAMAGE,				// ダメージ
		PLAYER_MOTION_MAX					// モーションの最大数
	}PLAYER_MOTION_STATE;

	CPlayer(PRIORITY type);							// コンストラクタ
	~CPlayer();										// デストラクタ
	void Init(void);								// 初期化処理
	void Uninit(void);								// 終了処理
	void Update(void);								// 更新処理
	void Draw(void);								// 描画処理

	static CPlayer *Create(int nPlayer, CHARACTER_TYPE type);	// 生成処理
	static CPlayer *GetPlayer(int nPlayer) { return m_pPlayer[nPlayer]; }// プレイヤー情報取得処理

	int GetNumStone(void)	{ return m_nNumStone; }	// 取得したストーンの数を取得
	int GetnPlayer(void) { return m_nPlayer; }		// プレイヤー番号取得
	inline int GetBoxColliderID(void) { return m_nBoxColliderID; }
protected:

private:
	void Control(void);								// 操作関数
	void Collision(void);							// 当たり判定関数
	void Attack(void);								// 攻撃関数

	CPlayer*GetAnotherPlayer(void);					// 違うプレイヤーの取得
	static CPlayer *m_pPlayer[MAX_PLAYER];			// プレイヤー情報
	static CHitPoint *m_pHitPoint;					// HP情報

	void ControlGamepad(CInputGamepad *pGamepad);	// ゲームパッド操作
	void ControlKeyboard(CInputKeyboard *pKeyboard);// キーボード操作
	void CatchStone(void);							// ストーンの取得判定
	void SetnPlayer(int nPlayerNum);				// プレイヤー番号設定処理

	inline bool BlowAway(CPlayer *pAnother);		// 吹き飛ぶ

	int m_nPlayer;			// プレイヤー番号
	int m_nBoxColliderID;	// ボックスコライダーID

#ifdef _DEBUG
	void ShowDebugInfo(void);									// ImGuiの更新
#endif
};
#endif