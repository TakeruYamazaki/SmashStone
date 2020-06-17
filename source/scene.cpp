//==================================================================================================================
//
// 描画するものを管理する[scene.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "scene.h"
#include "renderer.h"

//==================================================================================================================
// 静的メンバ変数の初期化
//==================================================================================================================
CScene *CScene::m_pTop[PRIORITY_MAX] = {};		// 先頭オブジェクトのポインタ
CScene *CScene::m_pCur[PRIORITY_MAX] = {};		// 現在(最後尾)のオブジェクトのポインタ

//==================================================================================================================
// コンストラクタ
//==================================================================================================================
CScene::CScene(PRIORITY type)
{
	// 先頭オブジェクトが無い時
	if (m_pTop[type] == NULL)
	{
		m_pPrev[type] = NULL;					// 前のオブジェクトのポインタ
		m_pTop[type] = this;					// 先頭オブジェクトのポインタをこれにする
		m_pCur[type] = this;
		m_pNext[type] = NULL;
	}
	else
	{
		// 最後尾の要素を設定する
		if (m_pCur[type] != NULL)
		{
			m_pCur[type]->m_pNext[type] = this;
			this->m_pPrev[type] = m_pCur[type];
			m_pCur[type] = this;
		}
	}

	m_bDeth = false;						// 死亡フラグは立っていない状態にする
}

//==================================================================================================================
// デストラクタ
//==================================================================================================================
CScene::~CScene()
{

}

//==================================================================================================================
// 全ての更新
//==================================================================================================================
void CScene::UpdateAll(void)
{
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		// 更新
		if (m_pTop[nCnt])
		{
			CScene *pScene = m_pTop[nCnt];
			while (pScene)
			{
				CScene *pNextScene = pScene->m_pNext[nCnt];
				pScene->Update();
				pScene = pNextScene;
			}
		}

		// 開放判定を行う
		if (m_pTop[nCnt])
		{
			CScene *pScene = m_pTop[nCnt];
			while (pScene)
			{
				CScene *pNextScene = pScene->m_pNext[nCnt];
				pScene->Deleate(nCnt);
				pScene = pNextScene;
			}
		}
	}
}

//==================================================================================================================
// 全ての描画
//==================================================================================================================
void CScene::DrawAll(void)
{
	// 描画順番のfor文
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		CScene *pScene = m_pTop[nCnt];				// 変数を作り初期化する
		while (pScene)								// pSceneがNULLになるまでエンドレス
		{
			CScene *pSceneNext = pScene->m_pNext[nCnt];	// 次の情報格納	
			pScene->Draw();							// 今の情報の描画処理
			pScene = pSceneNext;					// pSceneに次の情報代入
		}
	}
}

//==================================================================================================================
// すべて破棄
//==================================================================================================================
void CScene::ReleaseAll(void)
{
	// 描画順番のfor文
	for (int nCnt = 0; nCnt < PRIORITY_MAX; nCnt++)
	{
		// nullcheck
		if (m_pTop[nCnt])
		{
			// pSceneが空になるまで繰り返す
			CScene *pScene = m_pTop[nCnt];

			// nullcheck
			while (pScene)
			{
				// 次のポインタを保存
				CScene *pSceneNext = pScene->m_pNext[nCnt];

				// 今のポインタをリリース
				pScene->Release();

				// 今のポインタを破棄
				pScene->Deleate(nCnt);

				// 今のポインタを次のポインタに上書きする
				pScene = pSceneNext;
			}
		}
	}
}

//==================================================================================================================
// 死亡フラグを立てる
//==================================================================================================================
void CScene::Release(void)
{
	m_bDeth = true;				// 死亡フラグを立てる
}

//==================================================================================================================
// 取得
//==================================================================================================================
CScene *CScene::GetScene(PRIORITY nPriority, int nCntScene)
{
	CScene *pScene = m_pTop[nPriority];					// 変数を作り初期化する

	// 先頭からnCntScene分次のオブジェクトにポインタを渡す
	for (int nCnt = 0; nCnt < nCntScene; nCnt++)
	{
		CScene *pSceneNext = pScene->m_pNext[nPriority];	// 次の情報格納	
		pScene = pSceneNext;								// pSceneに次の情報代入
	}

	return pScene;										// 値を返す
}

//==================================================================================================================
// 破棄
//==================================================================================================================
void CScene::Deleate(int type)
{
	// 死亡フラグが立っているとき
	if (m_bDeth)
	{
		this->Uninit();

		// 削除時に配列に穴抜けができるので、順を入れ替える
		if (this == m_pTop[type] && this == m_pCur[type])
		{
			m_pTop[type] = m_pCur[type] = nullptr;
		}
		else if (this == m_pTop[type])
		{
			m_pTop[type] = m_pTop[type]->m_pNext[type];
			m_pTop[type]->m_pPrev[type] = nullptr;
		}
		else if (this == m_pCur[type])
		{
			m_pCur[type] = m_pCur[type]->m_pPrev[type];
			m_pCur[type]->m_pNext[type] = nullptr;
		}
		else
		{
			m_pPrev[type]->m_pNext[type] = this->m_pNext[type];
			m_pNext[type]->m_pPrev[type] = this->m_pPrev[type];
		}
		delete this;
	}
}
