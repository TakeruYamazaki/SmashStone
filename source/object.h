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

class CObject : public CSceneX
{
public:
	CObject(CSceneX::PRIORITY nPriority);
	~CObject();

	void Init(void);								// 初期化
	void Uninit(void);								// 終了
	void Update(void);								// 更新
	void Draw(void);								// 描画

	static CObject *Create(void);

	void SetObjInfo(const D3DXVECTOR3 & pos,
		const D3DXVECTOR3 & rot,
		const LPDIRECT3DTEXTURE9 & pTexture,
		const int & type,
		const bool & bCollision);						// オブジェクトの情報設定

	bool CollObject(D3DXVECTOR3 *pos,
		const D3DXVECTOR3 & posOld,
		D3DXVECTOR3 *move,
		const MODEL_VTX & modelVtx);

	bool GetRelease(void)	{ return m_bRelease; }		// リリースするかどうか
	bool GetbColl(void)		{ return m_bCollision; }	// 当たり判定を行うか
	int	 GetType(void)		{ return m_nType; }			// タイプ取得

#ifdef _DEBUG
	void ShowObjectInfo(char cPrintText[16]);
#endif

private:
	int		m_nType;		// オブジェクトタイプ
	bool	m_bRelease;		// リリースするかどうか
	bool	m_bCollision;	// 当たり判定処理を行うか
};

#endif