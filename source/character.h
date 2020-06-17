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
#include "kananlibrary.h"

// ===================================================================
// マクロ定義
// ===================================================================

// ===================================================================
// クラス定義
// ===================================================================
class CModelCharacter;

class CCharacter : public CScene
{
public:
	CCharacter(CScene::PRIORITY nPriority);
	~CCharacter();

	void Init(void);								// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	void Draw(void);								// 描画

	void SetPos(const D3DXVECTOR3 & pos)			{ m_pos = pos; }			// 位置の設定
	void SetMove(const D3DXVECTOR3 & move)			{ m_move = move; }			// 移動値の設定
	void SetRot(const D3DXVECTOR3 & rot)			{ m_rot = rot; }			// 回転の設定
	void SetRotDest(const D3DXVECTOR3 & rotDest)	{ m_rotDest = rotDest; }	// 目的の回転の設定
	void SetModelType(CHARACTER_TYPE type);			// モデルの設定

	D3DXVECTOR3 GetPos(void)		{ return m_pos; }		// 位置の取得
	D3DXVECTOR3 GetMove(void)		{ return m_move; }		// 移動値の取得
	D3DXVECTOR3 GetRot(void)		{ return m_rot; }		// 回転の取得
	D3DXVECTOR3 GetRotDest(void)	{ return m_rotDest; }	// 回転先の取得

private:
	void Move(void);								// 移動関数
	void Rot(void);									// 回転関数

	CModelCharacter *m_pModelCharacter;				// モデルキャラクターのポインタ

	LPD3DXMESH		m_pMesh;						// メッシュ情報
	LPD3DXBUFFER	m_pBuffMat;						// マテリアル情報
	DWORD			m_nNumMat;						// マテリアル情報の数

	D3DXVECTOR3 m_pos;								// 位置
	D3DXVECTOR3 m_posBegin;							// 初期位置
	D3DXVECTOR3 m_move;								// 移動値
	D3DXVECTOR3 m_rot;								// 回転
	D3DXVECTOR3 m_rotDest;							// 目的の回転
	D3DXVECTOR3 m_rotDif;							// 回転の差
	D3DXMATRIX	m_mtxWorld;							// ワールドマトリックス

	int			m_nCntMove;							// どれぐらいの時間動いているか
	bool		m_bJump;							// ジャンプしたかどうか
	bool		m_bWalk;							// 歩いてるかどうか

	static int	m_nNumCharacter;					// キャラクターの総数
};

#endif
