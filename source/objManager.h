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
	CObjectManager(CScene::PRIORITY nPriority);	// コンストラクタ
	~CObjectManager();	// デストラクタ

	void Init();								// 初期化
	void Uninit();								// 終了
	void Update();								// 更新
	void Draw();								// 描画
	static CObjectManager *Create(void);		// 生成

	static HRESULT Load(void);					// モデルのロード
	static void Unload(void);					// モデルの破棄

	static MODEL_VTX GetModelVtx(const int & nType) { return m_objInfo[nType].modelVtx; }	// タイプごとのモデル頂点情報の取得

	std::vector<CObject*> GetObj(void) { return m_pObject; }					// オブジェクトの取得
	int		GetNumObject(void) { return (int)m_pObject.size(); }		// オブジェクト総数の取得

#ifdef _DEBUG
	void ShowObjectManagerInfo(void);			// ImGuiの更新
#endif

private:
	HRESULT LoadOffset(void);					// オフセット読み込み
	static HRESULT LoadFileName(void);			// モデル名読み込み

	static std::vector<CObject*>	m_pObject;				// オブジェクトのポインタ
	static std::vector<OBJINFO>		m_objInfo;				// モデルの詳細
	static char						m_aFileName[MAX_TEXT];	// モデル配置のファイル名
	static int	m_nNumTexture;					// テクスチャの総数
	static int  *m_pModelIndex;					// テクスチャを割り当てるモデルの番号

#ifdef _DEBUG
	// デバッグモードの構造体
	typedef enum
	{
		MODE_GAME = 0,	// ゲーム
		MODE_DEBUG		// デバッグ
	} DEBUG_MODE;

	HRESULT SaveObject(void);						// オブジェクト情報のセーブ

	static int	m_stateMode;						// モードの状態
	static bool m_bShowAnother;						// 別のウィンドウ
	CObject		*m_pFakeObject;						// 偽のオブジェクト
	static bool m_bObjUse;							// 偽オブジェを使用しているか
	static int	m_nFakeType;						// 偽オブジェのタイプ
	static std::string	*m_pObjName;				// ImGui用のオブジェクト名
#endif
};
#endif