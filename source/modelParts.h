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

	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }
	void SetUpdatePos(const D3DXVECTOR3 &pos) { m_UpdatePos = pos; }
	void SetUpdateRot(const D3DXVECTOR3 &rot) { m_UpdateRot = rot; }

	void SetMtxParent(D3DXMATRIX *pMtxParent)	{ m_mtxParent = pMtxParent; }		// 親のマトリックスの設定
	void BindModelInfo(MODELINFO *pModelInfo)	{ m_pModelInfo = pModelInfo; }	// モデル情報の設定

	int			GetParent(void)		{ return m_nParent; }						// 親番号取得
	int			GetIndex(void)		{ return m_nIndex; }						// パーツのインデックスを取得
	D3DXMATRIX	*GetMtx(void)		{ return &m_mtxWorld; }						// ワールドマトリックスの取得
	D3DXVECTOR3 *GetOffsetPos(void) { return &m_OffsetPos; }					// オフセット位置の取得
	D3DXVECTOR3 *GetOffsetRot(void) { return &m_OffsetRot; }					// オフセット回転の取得
	D3DXVECTOR3 *GetPos(void)		{ return &m_pos; }							// 位置の取得
	D3DXVECTOR3 *GetRot(void)		{ return &m_rot; }							// 回転の所得
	D3DXVECTOR3 *GetUpdatePos(void) { return &m_UpdatePos; }					// 差分の位置を取得
	D3DXVECTOR3 *GetUpdateRot(void) { return &m_UpdateRot; }					// 差分の回転を取得

	void ShowModelPartsInfo(void);														// ImGuiの更新

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;	// モデルのテクスチャ

	D3DXVECTOR3	m_pos;				// 座標
	D3DXVECTOR3	m_rot;				// 回転
	D3DXVECTOR3 m_OffsetPos;		// モデルのオフセット位置
	D3DXVECTOR3 m_OffsetRot;		// モデルのオフセット回転
	D3DXVECTOR3 m_UpdatePos;		// モーション用の差分の座標
	D3DXVECTOR3 m_UpdateRot;		// モーション用の差分の回転
	D3DXMATRIX	m_mtxWorld;			// ワールドマトリックス
	D3DXMATRIX	*m_mtxParent;		// 親のマトリックス
	MODELINFO	*m_pModelInfo;		// モデル情報
	int m_nIndex;					// 自分のインデックス
	int m_nParent;					// 親の番号

private:

};
#endif