//==================================================================================================================
//
// タイトル処理[title.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _TITLE_H
#define _TITLE_H

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CCamera;
class CLight;
class CPolygon;
class CMeshField;
class CMeshSphere;
class CModel;
class CUI;
class CCharacter;

//==================================================================================================================
//
// タイトルクラスの定義
//
//==================================================================================================================
class CTitle
{
public:
	
	CTitle();									// コンストラクタ
	~CTitle();									// デストラクタ
	void Init(void);							// 初期化処理
	void Uninit(void);							// 終了処理
	void Update(void);							// 更新処理
	void Draw(void);							// 描画処理

	static CTitle *Create(void);				// 生成処理
	static CCharacter *GetCharacter(void);		// キャラクターの情報取得処理
	static CCamera *GetCamera(void);			// カメラの情報取得処理
	static bool GetbModeNext(void) { return m_bNextScreen; }// 次の画面に行くかどうか取得処理

	void SetNextMode(int nNextMode);			// 次のモード設定処理

protected:

private:

	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ情報
	static CCamera *m_pCamera;				// カメラの情報
	static CLight *m_pLight;				// ライトの情報
	static CPolygon *m_pPolygon;			// ポリゴン情報
	static CMeshField *m_pMeshField;		// メッシュフィールドの情報
	static CMeshSphere *m_pMeshSphere;		// メッシュ球の情報ポインタ
	static CUI *m_pUI;						// UIの情報ポインタ
	static CCharacter *m_pCharacter;		// キャラクターの情報ポインタ
	static bool m_bNextScreen;				// 次のモードにいくかどうか

	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuff;	// 頂点バッファ

	int m_nNextMode;						// 次のモード

};

#endif
