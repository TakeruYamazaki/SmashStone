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
		PLAYER_NEUTRAL,			// プレイヤー　ニュートラル
		PLAYER_SWIM_BEGIN,		// プレイヤー　泳ぎ始め
		PLAYER_SWIM,			// プレイヤー　泳ぎ
		FISH000_NEUTRAL,		// 魚000　ニュートラル
		FISH001_NEUTRAL,		// 魚001　ニュートラル
		MOTION_MAX				// 最大値
	} MOTION_TYPE;

	CMotion();
	~CMotion();

	static HRESULT Load();						// モーションのロード
	static void UnLoad();						// モーションのアンロード

	static D3DXVECTOR3 GetRotDest(MOTION_TYPE motiontype, int nKey, int nIndex);			// モーションの回転のゴール取得
	static int GetFrame(MOTION_TYPE motiontype, int nKey);
	static int GetNumKey(MOTION_TYPE motiontype);
	static bool GetLoop(MOTION_TYPE motiontype);

private:
	static MOTION_INFO *m_pMotionInfo;					// モーション情報のポインタ
	static HRESULT LoadMotion(MOTION_TYPE motiontype);	// モーション読み込み	
	static int CheckCharacter(MOTION_TYPE motiontype);	// どのキャラクターか確認

	static char *m_apFileName[CMotion::MOTION_MAX];
};
#endif