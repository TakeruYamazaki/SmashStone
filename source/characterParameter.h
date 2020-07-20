// ===================================================================
//
// キャラクターパラメーター処理 [ characterParameter.h ]
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

class CCharacterParameter
{
public:
	typedef enum
	{	// パラメーターのタイプ
		PARAMETER_1YASU = 0,	// 1ヤス
		PARAMETER_2YASU,		// 2ヤス
		PARAMETER_MAX			// 最大数
	} PARAMETER_TYPE;

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
	} PARAMETER_ATTACK;

	typedef struct
	{	// 移動のパラメーター
		float fRunSpeed;	// 移動力
		float fJumpPower;	// ジャンプ力
	} PARAMETER_MOVE;


	CCharacterParameter();			// コンストラクタ
	~CCharacterParameter();			// デストラクタ

	void               Init(void);					// 初期化
	void               Uninit(void);				// 終了
	HRESULT            Load(PARAMETER_TYPE type);	// キャラクターごとのパラメーターロード

protected:
	int m_nMaxLife;		// 最大HP
	PARAMETER_ATTACK	*m_attackParameter;	// 攻撃のパラメーターのポインタ
	PARAMETER_MOVE		m_moveParameter;	// 移動のパラメーター

private:
	static char m_aFileName[PARAMETER_MAX][64];	// 変身前キャラ数分のパラメーターファイル名
};

#endif
