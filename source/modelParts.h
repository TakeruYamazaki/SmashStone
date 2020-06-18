//=============================================================================
//
// モデルのパーツ処理  [ modelParts.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _MODELPARTS_H_
#define _MODELPARTS_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "sceneX.h"
#include "motion.h"

//=============================================================================
// クラス定義
//=============================================================================
class CModelParts
{
public:
	CModelParts();
	virtual ~CModelParts();

	void Init(void);																	// 初期化
	void Uninit(void);																	// 終了
	void Update(void);																	// 更新
	void Draw(void);																	// 描画
	void DrawShadow(void);																// 影の描画

	void SetPartsTexInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPDIRECT3DTEXTURE9 pTexture);		// パーツの情報設定
	void SetPartsInfo(int nIndex, int nParent, D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// パーツの情報設定
	void SetMotionRotDest(CMotion::MOTION_TYPE motiontype, int nKey);					// モーションの目的の回転の設定

	void SetMtxParent(D3DXMATRIX *pMtxParent)	{ m_mtxParent = pMtxParent; }		// 親のマトリックスの設定
	void BindModelInfo(MODELINFO *pModelInfo)	{ m_pModelInfo = pModelInfo; }	// モデル情報の設定

	int GetParent(void)			{ return m_nParent; }							// 親番号取得
	D3DXMATRIX *GetMtx(void)	{ return &m_mtxWorld; }							// ワールドマトリックスの取得

	void ShowModelPartsInfo(void);														// ImGuiの更新

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	// モデルのテクスチャ

	D3DXVECTOR3	m_pos;				// 座標
	D3DXVECTOR3	m_rot;				// 回転
	D3DXVECTOR3	m_rot1F;			// 回転の差分
	D3DXMATRIX	m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX	*m_mtxParent;		// 親のマトリックス
	MODELINFO	*m_pModelInfo;		// モデル情報
	int m_nIndex;					// 自分のID
	int m_nParent;					// 親の番号
};
#endif