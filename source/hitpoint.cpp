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
#include "debugProc.h"

//==================================================================================================================
//	マクロ定義
//==================================================================================================================
#define ROT_ANGLE 0.08f			// 回転させる角度
#define ALPHA_DEGRESS 21.875f		// α値の減少量
#define MAX_HPBAR 5				// HPバーの最大本数
#define BAR0_POS D3DXVECTOR3(250, 350, 0)	// 0番目BarHP
#define BAR1_POS D3DXVECTOR3(250, 354, 0)	// 1番目Barフレーム
#define BAR2_POS D3DXVECTOR3(200, 350, 0)	// 2番目BarHP
#define BAR3_POS D3DXVECTOR3(200, 354, 0)	// 3番目Barフレーム
#define BAR4_POS D3DXVECTOR3(150, 350, 0)	// 4番目BarHP
#define BAR5_POS D3DXVECTOR3(150, 354, 0)	// 5番目Barフレーム
#define BAR6_POS D3DXVECTOR3(100, 350, 0)	// 6番目BarHP
#define BAR7_POS D3DXVECTOR3(100, 354, 0)	// 7番目Barフレーム
#define BAR8_POS D3DXVECTOR3(50, 350, 0)	// 8番目BarHP
#define BAR9_POS D3DXVECTOR3(50, 354, 0)	// 9番目Barフレーム
#define NORMAL_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// 普通の色

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
	m_rot0 = D3DXVECTOR3(0, 0, 0);		// 回転
	m_rot1 = D3DXVECTOR3(0, 0, 0);		// 回転
	m_rot2 = D3DXVECTOR3(0, 0, 0);		// 回転
	m_rot3 = D3DXVECTOR3(0, 0, 0);		// 回転
	m_nCntCol0 = 0;						// 回転用カウンタ
	m_nCntCol1 = 0;						// 回転用カウンタ
	m_nCntCol2 = 0;						// 回転用カウンタ
	m_nCntCol3 = 0;						// 回転用カウンタ
	m_fMaxHP = pPlayer->GetLife();		// 最大HP
	m_fNowHP = m_fMaxHP;				// 現在のHP
	m_fHeight = MAX_HEIGHT;				// 高さ
	m_bBar[5] = false;					// バーがあるかどうか

	// Barの生成
	m_pBar = CBar::Create();

	// Barがあるとき
	if (m_pBar != NULL)
	{
		// BarHP
		m_pBar->SetVertexBar(0, BAR0_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(1, BAR1_POS, NORMAL_COLOR, MAX_WIDTH, MAX_HEIGHT + 5);

		// BarHP
		m_pBar->SetVertexBar(2, BAR2_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(3, BAR3_POS, NORMAL_COLOR, MAX_WIDTH, MAX_HEIGHT + 5);

		// BarHP
		m_pBar->SetVertexBar(4, BAR4_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(5, BAR5_POS, NORMAL_COLOR, MAX_WIDTH, MAX_HEIGHT + 5);

		// BarHP
		m_pBar->SetVertexBar(6, BAR6_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(7, BAR7_POS, NORMAL_COLOR, MAX_WIDTH, MAX_HEIGHT + 5);

		// BarHP
		m_pBar->SetVertexBar(8, BAR8_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		// Barフレーム
		m_pBar->SetVertexBar(9, BAR9_POS, NORMAL_COLOR, MAX_WIDTH, MAX_HEIGHT + 5);

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
		m_bBar[0] = true;										// バー0がある状態にする

		// 最大本数までカウント
		for (int nCnt = 0; nCnt < MAX_HPBAR; nCnt++)
		{
			m_bBar[nCnt] = true;			// 全てtureにする
		}

		// BarHPの設定
		m_pBar->SetVertexBar(0, BAR0_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
	}
	else if (NowHP >= m_fMaxHP / 5 * 3)
	{// HPバーが二本目のとき

		float fHeight = 0.0f;									// 現在の画像の高さ
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 3) / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// 最大本数までカウント
		for (int nCnt = 1; nCnt < MAX_HPBAR; nCnt++)
		{
			m_bBar[0] = false;			// 0番目はfalse
			m_bBar[nCnt] = true;		// それ以外tureにする
		}

		// BarHPの設定
		m_pBar->SetVertexBar(2, BAR2_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);

		// HPバー0番目がfalseのとき
		if (!m_bBar[0])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot0.z <= D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(0, BAR0_POS, ROT_ANGLE, MAX_HEIGHT / 2);
				m_pBar->RotBar(1, BAR1_POS, ROT_ANGLE, MAX_HEIGHT / 2);

				// バー色設定
				m_pBar->SetColBar(0, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot0.z += ROT_ANGLE;

				// α値加算
				m_nCntCol0++;
			}
		}
	}
	else if (NowHP >= m_fMaxHP / 5 * 2)
	{// HPバーが三本目のとき

		float fHeight = 0.0f;									// 現在の画像の高さ
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 2) / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// 最大本数までカウント
		for (int nCnt = 2; nCnt < MAX_HPBAR; nCnt++)
		{
			m_bBar[0] = false;			// 0番目はfalse
			m_bBar[1] = false;			// 1番目はfalse
			m_bBar[nCnt] = true;		// それ以外tureにする
		}

		// BarHPの設定
		m_pBar->SetVertexBar(4, BAR4_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);

		// HPバー1番目がfalseのとき
		if (!m_bBar[1])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot1.z <= D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(2, BAR2_POS, ROT_ANGLE, MAX_HEIGHT / 2);
				m_pBar->RotBar(3, BAR3_POS, ROT_ANGLE, MAX_HEIGHT / 2);

				// バー色設定
				m_pBar->SetColBar(2, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot1.z += ROT_ANGLE;

				// α値加算
				m_nCntCol1++;
			}
		}
	}
	else if (NowHP >= m_fMaxHP / 5 * 1)
	{// HPバーが四本目のとき

		float fHeight = 0.0f;									// 現在の画像の高さ
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 1) / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか

		// 最大本数までカウント
		for (int nCnt = 3; nCnt < MAX_HPBAR; nCnt++)
		{
			m_bBar[0] = false;			// 0番目はfalse
			m_bBar[1] = false;			// 1番目はfalse
			m_bBar[2] = false;			// 2番目はfalse
			m_bBar[nCnt] = true;		// それ以外tureにする
		}

		// BarHPの設定
		m_pBar->SetVertexBar(6, D3DXVECTOR3(100, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);

		// HPバー2番目がfalseのとき
		if (!m_bBar[2])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot2.z <= D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(4, BAR4_POS, ROT_ANGLE, MAX_HEIGHT / 2);
				m_pBar->RotBar(5, BAR5_POS, ROT_ANGLE, MAX_HEIGHT / 2);

				// バー色設定
				m_pBar->SetColBar(4, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot2.z += ROT_ANGLE;

				// α値加算
				m_nCntCol2++;
			}
		}
	}
	else
	{// HPバーが五本目のとき

		float fHeight = 0.0f;				// 現在の画像の高さ
		m_fNowHP = NowHP / (m_fMaxHP / 5);	// HPの比率計算
		fHeight = m_fNowHP * m_fHeight;		// 描画すべき画像幅がいくらなのか

		// 最大本数までカウント
		for (int nCnt = 4; nCnt < MAX_HPBAR; nCnt++)
		{
			m_bBar[0] = false;			// 0番目はfalse
			m_bBar[1] = false;			// 1番目はfalse
			m_bBar[2] = false;			// 2番目はfalse
			m_bBar[3] = false;			// 3番目はfalse
			m_bBar[nCnt] = true;		// それ以外tureにする
		}

		// BarHPの設定
		m_pBar->SetVertexBar(8, D3DXVECTOR3(50, 350, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);

		// HPバー3番目がfalseのとき
		if (!m_bBar[3])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot3.z <= D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(6, BAR6_POS, ROT_ANGLE, MAX_HEIGHT / 2);
				m_pBar->RotBar(7, BAR7_POS, ROT_ANGLE, MAX_HEIGHT / 2);

				// バー色設定
				m_pBar->SetColBar(6, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(7, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot3.z += ROT_ANGLE;

				// α値加算
				m_nCntCol3++;
			}
		}
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
