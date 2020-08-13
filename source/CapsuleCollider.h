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
		LPDIRECT3DVERTEXBUFFER9		pVtexBuff;		// 頂点バッファのポインタ
		LPDIRECT3DINDEXBUFFER9		pIdxBuff;			// インデックスのバッファのポインタ

		float			fLength;			// 長さ
		int				nSlices;			// 軸を中心とした分割数
		int				nStacks_1_2;		// 半球部分の分割数

		int				nNumVertex;			// 頂点数
		int				nNumindex;			// インデックス数
		int				nNumPolygon;		// ポリゴン数

		D3DXVECTOR3		Difference;			// 差分
		D3DXVECTOR3		TopTransVec;		// 上面までの長さと向き
		D3DXVECTOR3		BottomTransVec;		// 底面までの長さと向き

		CAPSULE			Capsule;			// カプセル情報
		TYPEID			enmTtpeID;			// タイプID

		TRANSFORM		trans;				// トランス情報
		D3DXMATRIX*		pMtxParent;			// 親のマトリックスポインタ
		CScene*			pScene;				// 親のシーンポインタ
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

	void CalCapPosition(void);																// カプセル位置の計算

	//* [contents] ∠p1p2p3は鋭角かどうか
	//* [in] Point（点）, Line（直線）
	//* [return] 鋭角かどうか（true : 鋭角, false : 鋭角ではない）
	static bool IsSharpAngle(CONST FLOAT3 &Point1, CONST FLOAT3 &Point2, CONST FLOAT3 &Point3);

	//* [contents] 点と直線の最短距離
	//* [in] Point（点）, Line（直線）
	//* [out] Perp（垂線）, fVecCoeffi（ベクトル係数）
	//* [return] 最短距離
	float calcPointLineDist(const FLOAT3 &Point, const LINE &Line, FLOAT3 &Perp, float &fVecCoeffi);

	//* [contents] 点と線分の最短距離
	//* [in] Point（点）, Seg（線分）
	//* [out] EndPtShortdist（最短距離となる端点）, EndPoint（端点）
	//* [return] 最短距離
	float calcPointSegmentDist(const FLOAT3 &Point, const SEGMENT &Seg, FLOAT3 &EndPtShortdist, float &EndPoint);

	//* [contents] 2直線の最短距離
	//* [in] Line1（直線1）, Line2（直線2）
	//* [out] PerpendFoot1（直線1側の垂線の足）,PerpendFoot2（直線2側の垂線の足）,fVecCoeffi1（直線1側のベクトル係数）, fVecCoeffi2（直線2側のベクトル係数）
	//* [return] 最短距離
	float calcLineLineDist(const LINE &Line1, const LINE &Line2, FLOAT3 &PerpendFoot1, FLOAT3 &PerpendFoot2, float &fVecCoeffi1, float &fVecCoeffi2);

	//* [contents] 0～1の間に制限する
	//* [out] fValue（制限する値）
	void Limit0to1(float &fValue);

	//* [contents] 2直線の最短距離
	//* [in] Seg1（線分1）, Seg2（線分2）
	//* [out] PerpendFoot1（線分1側の垂線の足）,PerpendFoot2（線分2側の垂線の足）,fVecCoeffi1（線分1側のベクトル係数）, fVecCoeffi2（線分2側のベクトル係数）
	//* [return] 最短距離
	float calcSegmentSegmentDist(const SEGMENT &Seg1, const SEGMENT &Seg2, FLOAT3 &PerpendFoot1, FLOAT3 &PerpendFoot2, float &fVecCoeffi1, float &fVecCoeffi2);

	//* [contents] カプセル同士の衝突判定
	//* [in] Cap1（カプセル1）, Cap2（カプセル2）
	//* [return] 衝突している時true
	bool colCapsuleCapsule(const CAPSULE &Cap1, const CAPSULE &Cap2);

private:
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);		// 頂点の作成
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);		// インデックスの作成

	static READINFOFILEBUFFER	m_ReadInfoFileBuff;		// 読みこんだファイル情報
	CAPSULECOLLIDERINFO m_ColliderInfo;				// 情報
};

#endif