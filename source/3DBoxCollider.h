//*************************************************************************************************************
//
//ボックスコライダー処理[3DBoxCollider.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _3DBOXCOLLIDER_H_
#define _3DBOXCOLLIDER_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define _3DBOXCOLLIDER_MAX				128		// ボックスコライダーの個数
#define _3DBOXCOLLIDER_NUM_INDEX		24
#define _3DBOXCOLLIDER_NUM_PRIMITIVE	12
#define _3DBOXCOLLIDER_USE_VERTEX		8
#define _3DBOXCOLLIDER_NUM_SURFACE		6


//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C3DBoxCollider :public CScene
{
public:
	enum
	{
		SURFACE_TYPE_TOP =0,	// 上
		SURFACE_TYPE_FRONT,		// 前
		SURFACE_TYPE_REAR,		// 後ろ
		SURFACE_TYPE_RIGHT,		// 右
		SURFACE_TYPE_LEFT,		// 左
		SURFACE_TYPE_BOTTOM,	// 下
		SURFACE_TYPE_MAX		// 最大
	};

	enum
	{
		ID_NOEN = -1,			// 無し
		ID_CHARACTER,			// キャラクター
		ID_STONE,				// ストーン

		ID_FLOWER,				// 植木鉢
		ID_ROOF,				// 屋根
		ID_STAIRS,				// 階段
		ID_WOODENBOX,			// 木の箱
		ID_LOOOSESTAIRS,		// 緩い階段
		ID_FOUNTAIN_DOWN,		// 噴水の下
		ID_FOUNTAIN_W_UP,		// 噴水の水がある上の部分
		ID_FOUNTAIN_IRON,		// 鉄

		ID_MAX					// 最大数
	};

	enum
	{
		TOP_OBJECT = ID_FLOWER,	// オブジェクトの先頭ID
	};

	typedef enum
	{
		COLLIDER_TYPE_NORMAL = 0,			// 通常
		COLLIDER_TYPE_CYLINDER,				// 円柱
		COLLIDER_TYPE_SPHERE,				// 球体
		COLLIDER_TYPE_CYLINDER_CANRIDE,		// 円柱の乗れるやつ
		COLLIDER_TYPE_MAX					// 最大
	} COLLIDER_TYPE;

	typedef enum
	{
		COLLIDER_SUB_NORMAL = 0,	// 通常
		COLLIDER_SUB_OVERRAP,		// 重なっているかだけ
		COLLIDER_SUB_MAX			// 最大
	} COLLIDER_SUBTYPE;

	typedef struct
	{
		D3DXVECTOR3			difference;		// 差分
		D3DXVECTOR3			size;			// 大きさ
		D3DXVECTOR3			pos;			// 位置
		D3DXVECTOR3			posOld;			// 前回の位置
		D3DXVECTOR3			Vertex[_3DBOXCOLLIDER_USE_VERTEX];	// 頂点位置
		D3DXVECTOR3			rot;			// 回転
		D3DXMATRIX			mtxWorld;		// ワールドマトリックス
		D3DXVECTOR3			surfaceNom[_3DBOXCOLLIDER_NUM_SURFACE];	// 面の法線
		bool				bUse;			// 使用フラグ
		COLLIDER_TYPE		ColliderType;	// 衝突種類
		COLLIDER_SUBTYPE	ColliderSubType;// 衝突サブタイプ
		CScene				*pScene;		// なにものか判別するため
	} _3DBOXCOLLIDER;

	// 読みこんだファイルの単体の情報
	typedef struct
	{
		D3DXVECTOR3	*pSize;			// 大きさのポインタ
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
	C3DBoxCollider(CScene::PRIORITY priority);
	// 読み込み
	static HRESULT Load(void);
	// 開放
	static void Unload(void);
	// 生成
	static C3DBoxCollider*Create(void);
	// 初期化
	void Init(void);
	// 終了
	void Uninit(void);
	// 更新
	void Update(void);
	// 描画
	void Draw(void);
	// 衝突判定
	static bool Collision(D3DXVECTOR3 &pos);
	// ダメージ判定判定
	static bool CollisionDamage(int n3DBoxColliderID, int nNoneColisitionID = -1);
	// 衝突したかだけ判定
	static bool Collisionoverlap(int n3DBoxColliderID,int *pHitID, int nNoneColisitionID = -1);
	// ブロック状の衝突判定
	static bool CollisionBox(int n3DBoxColliderID, D3DXVECTOR3 &pos, D3DXVECTOR3 &move);
	// 設定
	static int Set(D3DXVECTOR3 &size, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &difference, COLLIDER_TYPE colliType, COLLIDER_SUBTYPE ColliderSubType, CScene * pScene);
	// シーンのポインタを設定
	static void SetScene(int n3DBoxColliderID, CScene *pScene = NULL);
	// シーンのポインタを取得
	static CScene *GetScene(int n3DBoxColliderID);

	// 情報の変更
	static void ChangeInfo(int nIndex, D3DXVECTOR3 &size, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);
	// 情報の変更
	static void ChangeInfo(int nIndex, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);
	// 位置の変更
	static void ChangePosition(int nID,D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);
	// 面の法線の設定
	static void SetSurfaceNom(VERTEX_3D *pVtx, int nIndex);
	static void SetVertexPosResult(int nIndex);
	// 計算後の頂点位置の設定
	static void SetVertexPosResult(VERTEX_3D *pVtx, int nIndex);
	// コライダー情報の取得
	static _3DBOXCOLLIDER *GetInfo(int nIndex = 0);
	// 設定解除
	static void unset(int nID);
	// 情報の初期化
	void InitColliderInfo(void);
	// コライダー情報の読み込み
	//static int LoadColliderLoad(TRANSFORM *trans, int nID = -1, D3DXVECTOR3 *vtxMax = NULL, D3DXVECTOR3 *vtxMin = NULL, bool bLoad = true, COLLIDER_TYPE Collidertype = COLLIDER_TYPE::COLLIDER_TYPE_OVERRAP);
	// コライダー情報を設定する
	static int SetColliderInfo(D3DXVECTOR3 *pPos, CScene *pScene, COLLIDER_SUBTYPE ColliderSubType = COLLIDER_SUB_NORMAL, int nID = ID_CHARACTER);
	// 任意のIDをコライダーIDに変換
	static void ConvertColliderID(int *pID);
private:
	/* メンバ関数 */
	// 頂点の作成
	HRESULT MakeVertex(void);
	// インデックスの作成
	HRESULT MakeIndex(void);
	// 頂点位置の設定
	static void SetVertexPosition(VERTEX_3D *pVtx,int nIndex);
	/* メンバ変数 */
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;							// 頂点バッファへのポインタ
	static LPDIRECT3DTEXTURE9		m_pTexture;							// テクスチャ情報のポインタ
	static LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;							// インデックスのバッファのポインタ
	static READINFOFILEBUFFER		m_ReadInfoFileBuff;					// 読みこんだファイル情報
	static _3DBOXCOLLIDER			m_ColliderInfo[_3DBOXCOLLIDER_MAX];	// コライダー情報
};
#endif
