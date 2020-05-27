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
#include <memory>	// スマートポインタ使用に必要

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// クラス定義
//=============================================================================
class CObject : public CSceneX
{
public:
	typedef struct MODEL_VTX
	{	// モデルの最大・最小頂点座標
		D3DXVECTOR3 VtxMax;	// 頂点の最大値
		D3DXVECTOR3 VtxMin;	// 頂点の最小値
		MODEL_VTX() {};		// コンストラクタ
		MODEL_VTX(const D3DXVECTOR3 Max, const D3DXVECTOR3 Min)
		{
			VtxMax = Max;
			VtxMin = Min;
		}
	} MODEL_VTX;

	typedef struct
	{	// モデル一つ一つの情報
		char			cModelName[128];	// モデルのファイル名
		LPD3DXMESH		mesh;				// メッシュ
		LPD3DXBUFFER	matBuff;			// 頂点情報
		DWORD			matNum;				// マテリアル数
		MODEL_VTX		modelVtx;			// モデルの最大・最小頂点座標
	} MODELINFO;

	CObject(PRIORITY type);		// コンストラクタ
	~CObject();					// デストラクタ

	void Init(void);			// 初期化
	void Uninit(void);			// 終了
	void Update(void);			// 更新
	void Draw(void);			// 描画

	static std::unique_ptr<CObject> Create(void);	// 生成

	void SetObjInfo(D3DXVECTOR3 pos,
		D3DXVECTOR3 rot);			// パーツの情報設定

	//bool CollObject(D3DXVECTOR3 *pos, D3DXVECTOR3 *posOld, D3DXVECTOR3 *move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax);

	bool GetRelease(void) { return m_bRelease; }	// リリースするかどうか

#ifdef _DEBUG
	//void ShowObjectInfo(char cPrintText[16]);
#endif

private:
	bool		m_bRelease;		// リリースするかどうか
	MODELINFO	m_modelInfo;	// モデル情報
};

#endif