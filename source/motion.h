//=============================================================================
//
// モーション処理  [ motion.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// クラス定義
//=============================================================================
class CModelCharacter;

class CMotion
{
public:
	// キー
	typedef struct
	{
		D3DXVECTOR3 posDest;		// 目的の位置
		D3DXVECTOR3 rotDest;		// 目的の回転
	} KEY;

	// キー情報
	typedef struct
	{
		KEY *pKey;					// キーのポインタ
		int nFrame;					// フレーム数
	} KEY_INFO;

	// モーションの情報
	typedef struct
	{
		KEY_INFO *pKeyInfo;			// キー情報のポインタ
		int nNumKey;				// キー数
		bool bLoop;					// ループするかどうか
	} MOTION_INFO;

	// モーション
	typedef enum
	{
		MOTION_NONE,
		PLAYER_NEUTRAL = 0,		// プレイヤー ニュートラル
		PLAYER_RUN,				// プレイヤー 走る
		MOTION_MAX				// 最大値
	} MOTION_TYPE;

	CMotion();
	~CMotion();

	static HRESULT Load();						// モーションのロード
	static void UnLoad();						// モーションのアンロード

	static D3DXVECTOR3 GetRotDest(MOTION_TYPE motiontype, int nKey, int nIndex)
		{ return m_pMotionInfo[motiontype].pKeyInfo[nKey].pKey[nIndex].rotDest; }	// モーションの回転のゴール取得
	static D3DXVECTOR3 GetPosDest(MOTION_TYPE motiontype, int nKey, int nIndex)
		{ return m_pMotionInfo[motiontype].pKeyInfo[nKey].pKey[nIndex].posDest; }	// モーションの回転のゴール取得
	static int GetFrame(MOTION_TYPE motiontype, int nKey) 
		{ return m_pMotionInfo[motiontype].pKeyInfo[nKey].nFrame; }					// モーションの総フレーム数の朱徳
	static int GetNumKey(MOTION_TYPE motiontype) 
		{ return m_pMotionInfo[motiontype].nNumKey; }								// モーションの総キー数の取得
	static bool GetLoop(MOTION_TYPE motiontype) 
		{ return m_pMotionInfo[motiontype].bLoop; }									// モーションがループするかどうか取得

private:
	static MOTION_INFO *m_pMotionInfo;					// モーション情報のポインタ
	static HRESULT LoadMotion(MOTION_TYPE motiontype);	// モーション読み込み	
	static int CheckCharacter(MOTION_TYPE motiontype);	// どのキャラクターか確認

	static char *m_apFileName[CMotion::MOTION_MAX];
};
#endif