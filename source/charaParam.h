// ===================================================================
//
// キャラクターパラメーター処理 [ charaParam.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CHARAPARAM_H_
#define _CHARAPARAM_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "kananlibrary.h"

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CCharaParam
{
public:
	typedef enum
	{	// パラメーターのタイプ
		PARAM_1YASU = 0,	// 1ヤス
		PARAM_2YASU,		// 2ヤス
		PARAM_3YASU,		// 3ヤス
		PARAM_4YASU,		// 4ヤス
		PARAM_MAX			// 最大数
	} PARAM_TYPE;

	typedef enum
	{
		ATTACK_NORMAL_1 = 0,	// 通常攻撃1
		ATTACK_NORMAL_2,		// 通常攻撃2
		ATTACK_NORMAL_3,		// 通常攻撃3
		ATTACK_NORMAL_4,		// 通常攻撃4
		ATTACK_AIR_PUNCH,		// 空中パンチ攻撃
		ATTACK_AIR_KICK,		// 空中キック攻撃
		ATTACK_SMASH,			// スマッシュ攻撃
		ATTACK_MAX				// 全ての攻撃
	} PLAYER_ATTACK_TYPE;

	typedef enum
	{
		BLOWAWAY_DAUNTED = 0,	// 怯み
		BLOWAYAY_NORMAL,		// 吹き飛び
		BLOWAWAY_SMASH,			// スマッシュ吹き飛び
		BLOWAWAY_MAX			// 最大数
	} BLOWAWAY_TYPE;			// 吹き飛びのタイプ

	typedef struct INT_START_END
	{	// int型の開始～終了フレームの構造体
		int start;	// 開始
		int end;	// 終了

		// コンストラクタ
		INT_START_END() {};
		INT_START_END(int nStart, int nEnd)
		{
			start = nStart;
			end = nEnd;
		}
	} INT_START_END;

	typedef struct
	{	// 攻撃のパラメーター
		INT_START_END	CancelFrame;	// キャンセルフレーム
		BLOWAWAY_TYPE	blowType;		// 吹っ飛びの種類
	} MOTION_PARAM;

	typedef struct
	{	// 移動のパラメーター
		float fRunSpeed;	// 移動力
		float fJumpPower;	// ジャンプ力
	} MOVE_PARAM;

	typedef struct
	{	// プレイヤーのパラメーター
		float		 fMaxLife;					// 最大HP
		float		 fAttackPower;				// 攻撃力
		float		 fDefensePower;				// 防御力
		MOVE_PARAM	 moveParam;					// 移動のパラメーター
		MOTION_PARAM motionParam[ATTACK_MAX];	// モーションのパラメーター
	} PLAYER_PARAM;

	CCharaParam();			// コンストラクタ
	~CCharaParam();			// デストラクタ

	static HRESULT         Load(void);						// 全キャラクターのパラメーターロード
	static HRESULT		   Save(const PARAM_TYPE type);		// 各キャラクターのパラメーターセーブ
	static PLAYER_PARAM	   GetPlayerParam(PARAM_TYPE type) { return m_playerParam[type]; }	// パラメーターの取得
	
protected:
	static PLAYER_PARAM m_playerParam[PARAM_MAX];	// プレイヤーのパラメーター

private:
	static char m_aFileName[PARAM_MAX][64];	// 変身前キャラ数分のパラメーターファイル名
};

#endif
