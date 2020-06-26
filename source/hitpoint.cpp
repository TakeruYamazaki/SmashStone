//==================================================================================================================
//
// HPの描画 [hitpoint.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS

//==================================================================================================================
//	インクルードファイル
//==================================================================================================================
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "scene2D.h"
#include "manager.h"
#include "inputKeyboard.h"
#include "hitpoint.h"
#include "bar.h"
#include "player.h"
#include "game.h"
#include "character.h"

//==================================================================================================================
//	コンストラクタ
//==================================================================================================================
CHitPoint::CHitPoint(PRIORITY type = PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//	初期化処理
//==================================================================================================================
void CHitPoint::Init(void)
{
	// プレイヤーの情報ポインタ
	CPlayer *pPlayer = CGame::GetPlayer(0);

	// 初期化
	m_pBar = NULL;						// バーの情報ポインタ
	m_pos = D3DXVECTOR3(0, 0, 0);		// 位置
	m_fMaxHP = pPlayer->GetLife();		// 最大HP
	m_fNowHP = m_fMaxHP;				// 現在のHP
	m_fHeight = MAX_HEIGHT;				// 高さ

	// Barの生成
	m_pBar = CBar::Create();

	// Barがあるとき
	if (m_pBar != NULL)
	{
		// BarHP
		m_pBar->SetVertexBar(0, D3DXVECTOR3(250, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(1, D3DXVECTOR3(250, 354, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT + 5);

		// BarHP
		m_pBar->SetVertexBar(2, D3DXVECTOR3(200, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(3, D3DXVECTOR3(200, 354, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT + 5);

		// BarHP
		m_pBar->SetVertexBar(4, D3DXVECTOR3(150, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(5, D3DXVECTOR3(150, 354, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT + 5);

		// BarHP
		m_pBar->SetVertexBar(6, D3DXVECTOR3(100, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(7, D3DXVECTOR3(100, 354, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT + 5);

		// BarHP
		m_pBar->SetVertexBar(8, D3DXVECTOR3(50, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(9, D3DXVECTOR3(50, 354, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT + 5);

	}
}

//==================================================================================================================
//	終了処理
//==================================================================================================================
void CHitPoint::Uninit(void)
{
	// バーがあるとき
	if (m_pBar != NULL)
	{
		//Barの終了処理
		m_pBar->Uninit();
	}
}

//==================================================================================================================
//	更新処理
//==================================================================================================================
void CHitPoint::Update(void)
{
	// Barの更新処理
	m_pBar->Update();

	// プレイヤー情報取得
	CPlayer *pPlayer = CGame::GetPlayer(0);

	float NowHP = pPlayer->GetLife();				// 現在のHP

	// HPバーが一本目のとき
	if (NowHP >= m_fMaxHP / 5 * 4)
	{
		float fHeight = 0.0f;									// 現在の画像の高さ
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 4) / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// BarHPの設定
		m_pBar->SetVertexBar(0, D3DXVECTOR3(250, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
	}
	else if (NowHP >= m_fMaxHP / 5 * 3)
	{// HPバーが二本目のとき

		float fHeight = 0.0f;									// 現在の画像の高さ
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 3) / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// BarHPの設定
		m_pBar->SetVertexBar(2, D3DXVECTOR3(200, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
	}
	else if (NowHP >= m_fMaxHP / 5 * 2)
	{// HPバーが三本目のとき

		float fHeight = 0.0f;									// 現在の画像の高さ
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 2) / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// BarHPの設定
		m_pBar->SetVertexBar(4, D3DXVECTOR3(150, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
	}
	else if (NowHP >= m_fMaxHP / 5 * 1)
	{// HPバーが四本目のとき

		float fHeight = 0.0f;									// 現在の画像の高さ
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 1) / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// BarHPの設定
		m_pBar->SetVertexBar(6, D3DXVECTOR3(100, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
	}
	else
	{// HPバーが五本目のとき

		float fHeight = 0.0f;				// 現在の画像の高さ
		m_fNowHP = NowHP / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;		// 描画すべき画像幅がいくらなのか

		// BarHPの設定
		m_pBar->SetVertexBar(8, D3DXVECTOR3(50, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
	}
}

//==================================================================================================================
//	描画処理
//==================================================================================================================
void CHitPoint::Draw(void)
{
	//Barの描画処理
	m_pBar->Draw();
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CHitPoint * CHitPoint::Create(void)
{
	CHitPoint *pHitPoint = NULL;				// ポインタ変数

	pHitPoint = new CHitPoint(PRIORITY_UI);		// メモリを動的確保
	pHitPoint->Init();							// 初期化処理

	return pHitPoint;							// 値を返す
}

//==================================================================================================================
//	位置の設定
//==================================================================================================================
void CHitPoint::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==================================================================================================================
//	位置の取得
//==================================================================================================================
D3DXVECTOR3 CHitPoint::GetPos(void)
{
	return m_pos;
}

//==================================================================================================================
//	デストラクタ
//==================================================================================================================
CHitPoint::~CHitPoint(void)
{

}
