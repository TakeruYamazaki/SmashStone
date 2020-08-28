//*************************************************************************************************************
//
// カプセルコライダー処理[PolygonCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CAPSULECOLLIDER_H_
#define _CAPSULECOLLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#define POLYCOLLI_USE_VTX	4

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CCapsuleCollider : public CScene
{
public:
	// 列挙型定義
	typedef enum
	{
		TYPEID_NOEN = -1,	// 無し
		COLLIPARTS_BODY,	// 体
		TYPEID_FOREARM_R,	// 右前腕
		TYPEID_UPPERARM_R,	// 右上腕
		TYPEID_FOREARM_L,	// 前腕
		TYPEID_UPPERARM_L,	// 上腕
		TYPEID_MAX			// 最大数
	}TYPEID;

	// 構造体定義
	typedef struct
	{
		LPDIRECT3DVERTEXBUFFER9		pVtexBuff;			// 頂点バッファのポインタ
		LPDIRECT3DINDEXBUFFER9		pIdxBuff;			// インデックスのバッファのポインタ
		int							nSlices;			// 軸を中心とした分割数
		int							nStacks_1_2;		// 半球部分の分割数
		int							nNumVertex;			// 頂点数
		int							nNumindex;			// インデックス数
		int							nNumPolygon;		// ポリゴン数
		D3DXVECTOR3					TopTransVec;		// 上面までの長さと向き
		D3DXVECTOR3					BottomTransVec;		// 底面までの長さと向き
		CAPSULE						Capsule;			// カプセル情報
		TYPEID						enmTtpeID;			// タイプID
		TRANSFORM					trans;				// トランス情報
		D3DXMATRIX*					pMtxParent;			// 親のマトリックスポインタ
		CScene*						pScene;				// 親のシーンポインタ
	}CAPSULECOLLIDERINFO;

	// シリンダーの大きさを設定する情報
	typedef struct
	{
		float fTop;		// 上面までのサイズ
		float fBottom;	// 底面までのサイズ
		float fRadius;	// 半径
	} SIZE_INFO;

	// 読みこんだファイルの単体の情報
	typedef struct
	{
		SIZE_INFO	*pSizeInfo;		// サイズ情報のポインタ
		D3DXVECTOR3	*pDifference;	// 差分のポインタ
		int			nColliderType;	// 衝突種類
	} READINFOFILE_CELL;

	// 読みこんだファイルの全体情報
	typedef struct
	{
		int				   nNumReadInfo;			// 読みこむ情報の個数
		int*			   pSetThisID;				// 初期化するIDのポインタ
		READINFOFILE_CELL* pCell;					// 単体情報ポインタ
	} READINFOFILEBUFFER;
	

	// コンストラクタ
	CCapsuleCollider();
	// デストラクタ
	~CCapsuleCollider() {}


	// 読み込み
	static HRESULT Load(void);
	// 開放
	static void Unload(void);

	void Init(void);																		// 初期化
	void Uninit(void);																		// 終了
	void Update(void);																		// 更新
	void Draw(void);																		// 描画
	void SetMtxParent(D3DXMATRIX *pMtxParent);												// 親のマトリックスポインタの設定
	void SetScene(CScene *pScene);															// シーンの設定

	static CCapsuleCollider *Create(CScene *pScene,D3DXMATRIX *pMtxParent, int nTypeID);	// 生成
	void InfoSetSync(float fRadius, float fLengthMax, float fLengthMin, D3DXVECTOR3 &diff);	// 情報の設定と同期
	void InitCapsInfo(void);																// カプセル情報の初期化
	bool Collision(void);																	// 衝突判定
	bool CollisionStone(void);																// ストーンとの衝突判定

	void CalCapPosition(void);																// カプセル位置の計算

	inline void SeteumTypeID(const int &nTypeID) { m_ColliderInfo.enmTtpeID = (TYPEID)nTypeID;}

private:
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);		// 頂点の作成
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);		// インデックスの作成

	static READINFOFILEBUFFER	m_ReadInfoFileBuff;	// 読みこんだファイル情報
	CAPSULECOLLIDERINFO m_ColliderInfo;				// 情報
};

#endif