// ===================================================================
//
// キャラクターパラメーター処理 [ charaParame.h ]
// Author : KANAN NAGANAWA
//
// ===================================================================
#ifndef _CHARACTERPARAMETER_H_
#define _CHARACTERPARAMETER_H_

#define _CRT_SECURE_NO_WARNINGS // 警告除去

// ===================================================================
// インクルードファイル
// ===================================================================
#include "main.h"
#include "kananlibrary.h"
#include "player.h"

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
		PARAM_MAX			// 最大数
	} PARAM_TYPE;

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
		float					fAttackPower;	// 攻撃力
		INT_START_END			CancelFrame;	// キャンセルフレーム
		CPlayer::BLOWAWAY_TYPE	blowType;		// 吹っ飛びの種類
	} PARAM_ATTACK;

	typedef struct
	{	// 移動のパラメーター
		float fRunSpeed;	// 移動力
		float fJumpPower;	// ジャンプ力
	} PARAM_MOVE;


	CCharaParam();			// コンストラクタ
	~CCharaParam();			// デストラクタ

	static HRESULT         Load(void);		// キャラクターごとのパラメーターロード
	static void			   Unload(void);	// データの破棄

protected:
	static int			m_nMaxLife[PARAM_MAX];			// 最大HP
	static PARAM_MOVE	m_moveParam[PARAM_MAX];		// 移動のパラメーター
	static PARAM_ATTACK	*m_pAttackParam[PARAM_MAX];	// 攻撃のパラメーターのポインタ

private:
	static char m_aFileName[PARAM_MAX][64];	// 変身前キャラ数分のパラメーターファイル名
};

#endif
