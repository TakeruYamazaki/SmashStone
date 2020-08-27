//=============================================================================
//
// オブジェクトマネージャー処理  [ objectManager.h ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"
#include "object.h"
#include "objectLift.h"
#include "scene.h"
#include <vector>
#include <memory>							// スマートポインタの使用に必要
#include "kananlibrary.h"

//=============================================================================
// クラス定義
//=============================================================================
class CObject;

class CObjectManager : public CScene
{
public:
	typedef enum
	{	// ステージの種類
		STAGE_1 = 0,	// ステージ1
		STAGE_2,		// ステージ2
		STAGE_MAX		// 最大数
	} STAGETYPE;

	typedef struct
	{	// オブジェクトオフセット
		int nType;			// タイプ
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 回転
		D3DXVECTOR3 scale;	// 拡縮
	} OBJECT_OFFSET;

	typedef struct
	{
		int nNumObject;							// 配置するオブジェクト数
		OBJECT_OFFSET *objOffset;				// オブジェクトオフセット格納用
		std::vector<CObject*> pObject;			// オブジェクト情報
		std::vector<CObjectLift*> pObjLift;	// 持てるオブジェクト情報
		D3DXVECTOR3 stonePos[5]; // ストーンを配置する位置
	} STAGEINFO;

	CObjectManager(CScene::PRIORITY nPriority);	// コンストラクタ
	~CObjectManager();	// デストラクタ

	void Init();									// 初期化
	void Uninit();									// 終了
	void Update();									// 更新
	void Draw();									// 描画
	static CObjectManager *Create(STAGETYPE type);	// 生成

	static HRESULT Load(void);					// モデルのロード
	static void Unload(void);					// モデルの破棄

	static MODEL_VTX GetModelVtx(const int & nType) { return m_objInfo[nType].modelVtx; }	// タイプごとのモデル頂点情報の取得
	static STAGETYPE GetStageType(void) { return m_stageType; }								// ステージのタイプ取得
	static STAGEINFO GetStageInfo(void) { return m_stageInfo[m_stageType]; }				// ステージの情報取得

	std::vector<CObject*> GetObjAll(void) { return m_stageInfo[m_stageType].pObject; }		// オブジェクトの取得
	CObject* GetObj(int nIndex) { return m_stageInfo[m_stageType].pObject[nIndex]; }		// オブジェクトの取得
	int		GetNumObject(void) { return (int)m_stageInfo[m_stageType].pObject.size(); }		// オブジェクト総数の取得

	std::vector<CObjectLift*> GetObjLiftAll(void) { return m_stageInfo[m_stageType].pObjLift; }	// 持てるオブジェクトの取得
	CObjectLift* GetObjLift(int nIndex) { return m_stageInfo[m_stageType].pObjLift[nIndex]; }	// 持てるオブジェクトの取得
	int		GetNumObjLift(void) { return (int)m_stageInfo[m_stageType].pObjLift.size(); }		// 持てるオブジェクト総数の取得

	static D3DXVECTOR3 GetDefaultStonePos(int nPos) { return m_stageInfo[m_stageType].stonePos[nPos]; }	// ストーンの配置位置の取得

#ifdef _DEBUG
	void ShowObjectManagerInfo(void);			// ImGuiの更新
	static bool GetShowObjWindow(void) { return m_bShowAnother; }			// オブジェウィンドウ表示の有無を取得
	static void SetShowObjWindow(bool bShow) { m_bShowAnother = bShow; }	// 表示の設定
#endif

private:
	static void CreateTexture();	// テクスチャ生成
	static void CreateObjInfo();	// オブジェクト情報生成

	static HRESULT LoadModel(void);				// モデル読み込み
	static HRESULT LoadOffset(void);			// オフセット読み込み
	static HRESULT SetOffset(void);				// オフセット設定

	static char					m_cModelFile[MAX_TEXT];				// モデル情報のファイル名
	static char					m_aFileName[STAGE_MAX][MAX_TEXT];	// モデル配置のファイル名
	static STAGEINFO			m_stageInfo[STAGE_MAX];				// ステージの情報
	static STAGETYPE			m_stageType;						// ステージのタイプ
	static std::vector<OBJINFO>	m_objInfo;							// オブジェクトのモデル情報
	static int					m_nNumTexture;						// テクスチャ総数
	static int					*m_pModelIndex;						// テクスチャを割り当てるモデル番号

#ifdef _DEBUG
	// デバッグモードの構造体
	typedef enum
	{
		MODE_GAME = 0,	// ゲーム
		MODE_DEBUG		// デバッグ
	} DEBUG_MODE;

	HRESULT SaveOffset(void);						// オフセット情報のセーブ

	static int	m_stateMode;						// モードの状態
	static bool m_bShowAnother;						// 別のウィンドウ
	CObject		*m_pFakeObject;						// 偽のオブジェクト
	static bool m_bObjUse;							// 偽オブジェを使用しているか
	static int	m_nFakeType;						// 偽オブジェのタイプ
	static std::string	*m_pObjName;				// ImGui用のオブジェクト名
#endif
};
#endif