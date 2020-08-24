//=============================================================================
//
// モデルのキャラクター処理  [ modelCharacter.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _MODELCHARACTER_H_
#define _MODELCHARACTER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "sceneX.h"
#include "motion.h"
#include "kananlibrary.h"

//=============================================================================
// クラス定義
//=============================================================================
class CModelParts;

class CModelCharacter
{
public:

	CModelCharacter();
	~CModelCharacter();

	HRESULT Init();													// 初期化
	void Uninit();													// 終了
	void Update();													// 更新
	void Draw();													// 描画
	static CModelCharacter *Create(CHARACTER_TYPE type);			// 生成
	static HRESULT Load(void);										// モデルのロード
	static void Unload(void);										// モデルの破棄

	void SetCharacterMtx(D3DXMATRIX *mtx);							// マトリックスの設定
	void SetModelType(CHARACTER_TYPE type) { m_type = type; }							// キャラクターのタイプ
	void SetMotion(CMotion::MOTION_TYPE motiontype);				// モーションを設定
	void ResetMotion();												// モーションのリセット

	void ModelRebind(CHARACTER_TYPE type);							// モデルの再バインド

	static int GetPartsNum(CHARACTER_TYPE type) { return m_pModelCharacter[type].nNumParts; }	// キャラクターのパーツ数を取得
	CMotion::MOTION_TYPE GetMotion(void)		{ return m_motion; }							// モーションを取得
	CModelParts *GetModelParts(void)			{ return m_pModelParts; }						// モデルパーツの取得
	int GetAllFrame(void)						{ return m_nAllFrame; }							// 現在のモーションの総フレーム数を取得

private:
	static MODELCHARACTER m_pModelCharacter[CHARACTER_MAX];			// 全てのモデルキャラクタの情報格納
	CModelParts *m_pModelParts;										// モデルのパーツのポインタ
	CHARACTER_TYPE m_type;											// モデルの種類
	D3DXMATRIX *m_CharacterMtx;										// キャラクターのマトリックス
	CMotion::MOTION_TYPE m_motion;									// モーション情報
	int m_nFrame;													// 現在のフレーム
	int m_nKey;														// 現在のキー
	int m_nAllFrame;												// 現在のモーションの総フレーム

	HRESULT LoadOffset(CHARACTER_TYPE type);						// オフセット読み込み
	static HRESULT LoadFileName(CHARACTER_TYPE type);				// モデル名読み込み

#ifdef _DEBUG
	void ShowModelCharaInfo(void);									// ImGuiの更新
#endif

	static char m_aFileName[CHARACTER_MAX][64];
};
#endif