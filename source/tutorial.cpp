//==================================================================================================================
//
// tutorialの描画 [tutorial.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS											// 警告回避文

//==================================================================================================================
//	インクルードファイル
//==================================================================================================================
#include "tutorial.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "sound.h"
#include "scene2D.h"
#include "scene3D.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "player.h"
#include "motionModel.h"
#include "inputGamepad.h"
#include "meshSphere.h"
#include "UI.h"
#include "game.h"

//==================================================================================================================
//	静的メンバ変数宣言
//==================================================================================================================
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture = NULL;		// テクスチャ情報
CCamera *CTutorial::m_pCamera = NULL;					// カメラ情報
CLight *CTutorial::m_pLight = NULL;						// ライト情報
CPolygon *CTutorial::m_pPolygon = NULL;					// ポリゴン情報
CMeshField *CTutorial::m_pMeshField = NULL;				// メッシュフィールド情報
CCharacter *CTutorial::m_pCharacter = NULL;				// キャラクター情報
CUI *CTutorial::m_pUI = NULL;							// UI情報
CMeshSphere *CTutorial::m_pMeshSphere = NULL;			// メッシュ球情報
CModel *CTutorial::m_pModel[TUTORIAL_MAX_MODEL] = {};	// モデル情報

//==================================================================================================================
//	コンストラクタ
//==================================================================================================================
CTutorial::CTutorial()
{

}

//==================================================================================================================
//	デストラクタ
//==================================================================================================================
CTutorial::~CTutorial()
{

}

//==================================================================================================================
//	初期化処理
//==================================================================================================================
void CTutorial::Init(void)
{
	m_bCharaDecide[MAX_PLAYER] = false;	// キャラクターが選ばれたどうか

	CMeshField::Load();				// メッシュフィールドテクスチャロード
	CMotionModel::Load();			// モデルロード
	CMeshSphere::Load();			// メッシュ球のテクスチャロード
	CUI::Load();					// UIテクスチャロード

	// カメラの生成処理
	m_pCamera = CCamera::Create();

	// ライトの生成処理
	m_pLight = CLight::Create();

	// メッシュ球の生成処理
	m_pMeshSphere = CMeshSphere::Create();

	// メッシュフィールド生成処理
	m_pMeshField = CMeshField::Create(INTEGER2(10, 10), D3DXVECTOR3(250.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// UI生成処理
	m_pUI = CUI::Create();
}

//==================================================================================================================
//	終了処理
//==================================================================================================================
void CTutorial::Uninit(void)
{
	// scene2Dの破棄処理
	CScene2D::ReleaseAll();

	// scene3Dの破棄処理
	CScene3D::ReleaseAll();

	CMeshField::Unload();			// メッシュフィールドテクスチャアンロード
	CMotionModel::Unload();			// プレイヤーテクスチャモデルアンロード
	CUI::Unload();					// UIテクスチャアンロード

	delete m_pLight;				// メモリ削除
	m_pLight = nullptr;				// ポインタNULL

	delete m_pCamera;				// メモリ削除
	m_pCamera = nullptr;			// ポインタNULL
}

//==================================================================================================================
//	更新処理
//==================================================================================================================
void CTutorial::Update(void)
{
	// キーボード取得
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// フェード取得
	CFade::FADE fade = CFade::GetFade();

	// 最大人数までカウント
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// キャラクターが選ばれたどうか取得
		m_bCharaDecide[nCnt] = m_pUI->GetCharaDecide(nCnt);
	}

	// カメラの更新処理
	m_pCamera->Update();

	// ライトの更新処理
	m_pLight->Update();

	// 1Pと2Pがキャラクターを選んだとき
	if (m_bCharaDecide[0] && m_bCharaDecide[1])
	{
		// フェードが何もない時
		if (fade == CFade::FADE_NONE)
		{
			// それぞれのタイプを保存
			CGame::SetPlayerType(PLAYER_ONE, m_pUI->GetCharaNum(PLAYER_ONE));
			CGame::SetPlayerType(PLAYER_TWO, m_pUI->GetCharaNum(PLAYER_TWO));

			// フェードの設定
			CFade::SetFade(CRenderer::MODE_MAPSELECT, DEFAULT_FADE_TIME);
		}
	}
}

//==================================================================================================================
//	描画処理
//==================================================================================================================
void CTutorial::Draw(void)
{
	// カメラの設定
	m_pCamera->SetCamera();

	// カメラの描画処理
	m_pCamera->Draw();
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CTutorial * CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;		// チュートリアル情報をNULLにする

	pTutorial = new CTutorial;			// 動的に確保
	pTutorial->Init();					// 初期化処理

	return pTutorial;					// 値を返す
}

//==================================================================================================================
// キャラクター情報取得処理
//==================================================================================================================
CCharacter * CTutorial::GetCharacter(void)
{
	return m_pCharacter;
}
