//*************************************************************************************************************
//
// シリンダーコライダー処理[CylinderCoillider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CYLINDERCOILLIDER_H_
#define _CYLINDERCOILLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C3DBoxCollider;
class CModel;
class CCylinderCoillider :public CScene
{
public:
	typedef enum
	{
		TYPEID_NOEN = -1,	// 無し
		TYPEID_FOREARM_R,	// 右前腕
		TYPEID_UPPERARM_R,	// 右上腕
		TYPEID_FOREARM_L,	// 前腕
		TYPEID_UPPERARM_L,	// 上腕
		TYPEID_MAX			// 最大数
	}TYPEID;

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
	CCylinderCoillider();
	// デストラクタ
	~CCylinderCoillider();

	// 読み込み
	static HRESULT Load(void);
	// 開放
	static void Unload(void);
	// 生成
	static CCylinderCoillider *Create(TYPEID enmTypeId, D3DXMATRIX *pMtxWorld);
	// 初期化
	void Init(void);
	// 終了
	void Uninit(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 情報の設定と同期
	void InfoSetSync(float fRadius, float fLengthMax, float fLengthMin, D3DXVECTOR3 &diff);
	// 親の設定
	void SetPareMtxWorld(D3DXMATRIX* pPareMtxWorld);
	// ワールドマトリックスの取得
	D3DXMATRIX *GetMtxWorld(void);
	// 半径の取得
	float GetRadius(void);
	// 最大点の取得
	D3DXVECTOR3 *GetTopPoint(void);
	// 最小点の取得
	D3DXVECTOR3 *GetBottomPoint(void);
	// 底面の法線と1辺のベクトルの算出
	void CalculateBottomNormal(void);
	// 底面の法線の取得
	D3DXVECTOR3 *GetBottomNormal(void);
	// タイプIDの設定
	void SetTypeID(TYPEID enmTypeID);

	// 衝突
	bool Collision(int n3DBoxColliderID);
	// ボックス内にいるかテスト
	bool InsideBoxTest(int n3DBoxColliderID , D3DXVECTOR3 *pPos);

private:
	/* メンバ関数 */
	// 頂点の作成
	void MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	// インデックスの作成
	void MakeIndex(LPDIRECT3DDEVICE9 pDevice);
	/* メンバ変数 */
	static READINFOFILEBUFFER	m_ReadInfoFileBuff;		// 読みこんだファイル情報
	LPDIRECT3DVERTEXBUFFER9		m_pVtexBuff;			// 頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				// インデックスのバッファのポインタ
	D3DXMATRIX					m_mtxWorld;				// ワールドマトリックス
	D3DXMATRIX*					m_pPareMtxWorld;		// 親のワールドマトリックスポインタ
	D3DXVECTOR3					m_Difference;			// 差分
	float						m_fRadius;				// 半径
	D3DXVECTOR3					m_TopTransVec;			// 上面までの長さと向き
	D3DXVECTOR3					m_BottomTransVec;		// 底面までの長さと向き
	int							m_nNumVertex;			// 総頂点数
	int							m_nNumIndex;			// 総インデックス数
	int							m_nNumPolygon;			// 総ポリゴン数
	D3DXVECTOR3					m_TopPoint;				// 上面位置
	D3DXVECTOR3					m_BottomPoint;			// 底面位置
	D3DXVECTOR3					m_BottomNormal;			// 底面の法線
	TYPEID						m_enmTtpeID;			// タイプID

protected:
};

#endif