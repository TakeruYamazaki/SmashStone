//=============================================================================
//
// オブジェクト処理 [ object.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "sceneX.h"
#include "kananlibrary.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// クラス定義
//=============================================================================
class CObject
{
public:
	CObject();
	~CObject();

	void Init(void);								// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	void Draw(void);								// 描画
	void DrawAlpha(void);							// 透明描画

	static CObject *Create(void);					// 生成

	void SetObjInfo(const D3DXVECTOR3 & pos,
		const D3DXVECTOR3 & rot,
		const D3DXVECTOR3 & scale,
		MODELINFO *pModelInfo,
		const int & type);					// オブジェクトの情報設定

	bool CollObject(D3DXVECTOR3 *pos,
		const D3DXVECTOR3 & posOld,
		D3DXVECTOR3 *move,
		const MODEL_VTX & modelVtx);				// オブジェクトの衝突処理


	void SetCollider(void);							// コライダーの設定

	D3DXVECTOR3 *GetPos(void)		{ return &m_pos; }			// 位置の取得
	D3DXVECTOR3 *GetPosOld(void)	{ return &m_posOld; }		// 前回の入りの取得
	D3DXVECTOR3 *GetRot(void)		{ return &m_rot; }			// 向きの取得
	D3DXVECTOR3 *GetMove(void)		{ return &m_move; }			// 前回の向きの取得
	D3DXVECTOR3 *GetScale(void)		{ return &m_scale; }		// 拡大率の取得
	D3DXMATRIX	*GetMtx(void)		{ return &m_mtxWorld; }		// ワールドマトリックスの取得
	bool GetbColl(void)				{ return m_bCollision; }	// 当たり判定を行うか
	int	 GetType(void)				{ return m_nType; }			// タイプ取得

#ifdef _DEBUG
	bool GetRelease(void) { return m_bRelease; }		// リリースするかどうか
	void ShowObjectInfo(char cPrintText[16]);
#endif

private:
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_posOld;		// 前回の位置
	D3DXVECTOR3 m_posBegin;		// 開始位置
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_rotBegin;		// 開始向き
	D3DXVECTOR3 m_scale;		// モデルの拡大率
	D3DXMATRIX	m_mtxWorld;		// ワールドマトリックス
	MODELINFO	m_pModelInfo;	// モデル情報
	int			m_nType;		// オブジェクトタイプ
	bool		m_bCollision;	// 当たり判定処理を行うか
	int			m_nColliderID;	// コライダーID

#ifdef _DEBUG
	bool	m_bRelease;		// リリースするかどうか
#endif
};

#endif