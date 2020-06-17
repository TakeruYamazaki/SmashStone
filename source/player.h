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

	static CPlayer *Create(void);					// 生成処理

protected:

private:
#ifdef _DEBUG
	void ShowDebugInfo(void);									// ImGuiの更新
#endif
};
#endif