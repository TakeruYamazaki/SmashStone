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
#include "character.h"
#include "debugProc.h"
#include "game.h"

//==================================================================================================================
//	マクロ定義
//==================================================================================================================
#define ROT_ANGLE 0.08f			// 回転させる角度
#define ALPHA_DEGRESS 19.0f		// α値の減少量
#define MAX_HPBAR 5				// HPバーの最大本数
#define BAR_HP_HEIGHT 110		// HPバーの高さ
#define BAR_HPFRAME_HEIGHT 163	// HPフレームの高さ
#define DROP_SPEED 8.0f			// バーの落ちるスピード
#define DROP_DIFFRENCE 2.0f		// 落ちるときのずれ修正
#define ROT_DIFF 53.0f			// 回転の差
#define BAR0_LEFT_POS D3DXVECTOR3(250, BAR_HP_HEIGHT, 0)		// 0番目BarHP
#define BAR1_LEFT_POS D3DXVECTOR3(250, BAR_HPFRAME_HEIGHT, 0)	// 1番目Barフレーム
#define BAR2_LEFT_POS D3DXVECTOR3(210, BAR_HP_HEIGHT, 0)		// 2番目BarHP
#define BAR3_LEFT_POS D3DXVECTOR3(210, BAR_HPFRAME_HEIGHT, 0)	// 3番目Barフレーム
#define BAR4_LEFT_POS D3DXVECTOR3(170, BAR_HP_HEIGHT, 0)		// 4番目BarHP
#define BAR5_LEFT_POS D3DXVECTOR3(170, BAR_HPFRAME_HEIGHT, 0)	// 5番目Barフレーム
#define BAR6_LEFT_POS D3DXVECTOR3(130, BAR_HP_HEIGHT, 0)		// 6番目BarHP
#define BAR7_LEFT_POS D3DXVECTOR3(130, BAR_HPFRAME_HEIGHT, 0)	// 7番目Barフレーム
#define BAR8_LEFT_POS D3DXVECTOR3(90, BAR_HP_HEIGHT, 0)			// 8番目BarHP
#define BAR9_LEFT_POS D3DXVECTOR3(90, BAR_HPFRAME_HEIGHT, 0)	// 9番目Barフレーム
#define BAR0_RIGHT_POS D3DXVECTOR3(1010, BAR_HP_HEIGHT, 0)		// 0番目BarHP
#define BAR1_RIGHT_POS D3DXVECTOR3(1010, BAR_HPFRAME_HEIGHT, 0)	// 1番目Barフレーム
#define BAR2_RIGHT_POS D3DXVECTOR3(1050, BAR_HP_HEIGHT, 0)		// 2番目BarHP
#define BAR3_RIGHT_POS D3DXVECTOR3(1050, BAR_HPFRAME_HEIGHT, 0)	// 3番目Barフレーム
#define BAR4_RIGHT_POS D3DXVECTOR3(1090, BAR_HP_HEIGHT, 0)		// 4番目BarHP
#define BAR5_RIGHT_POS D3DXVECTOR3(1090, BAR_HPFRAME_HEIGHT, 0)	// 5番目Barフレーム
#define BAR6_RIGHT_POS D3DXVECTOR3(1130, BAR_HP_HEIGHT, 0)		// 6番目BarHP
#define BAR7_RIGHT_POS D3DXVECTOR3(1130, BAR_HPFRAME_HEIGHT, 0)	// 7番目Barフレーム
#define BAR8_RIGHT_POS D3DXVECTOR3(1170, BAR_HP_HEIGHT, 0)		// 8番目BarHP
#define BAR9_RIGHT_POS D3DXVECTOR3(1170, BAR_HPFRAME_HEIGHT, 0)	// 9番目Barフレーム
#define NORMAL_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	// 普通の色

//==================================================================================================================
//	静的メンバ変数の初期化
//==================================================================================================================
CPlayer *CHitPoint::m_pPlayer = NULL;		// プレイヤーの情報
int CHitPoint::m_nInitCnt = 0;				// 初期化用カウンタ

//==================================================================================================================
//	コンストラクタ
//==================================================================================================================
CHitPoint::CHitPoint(PRIORITY type = PRIORITY_UI) : CScene(type)
{
	m_pBar = NULL;				// バーの情報ポインタ
}

//==================================================================================================================
//	デストラクタ
//==================================================================================================================
CHitPoint::~CHitPoint(void)
{
	m_nInitCnt = 0;				// 初期化用カウンタ初期化
}

//==================================================================================================================
//	初期化処理
//==================================================================================================================
void CHitPoint::Init(void)
{
	// 初期化
	m_pos = D3DXVECTOR3(0, 0, 0);	// 位置
	m_rot0 = D3DXVECTOR3(0, 0, 0);	// 回転
	m_rot1 = D3DXVECTOR3(0, 0, 0);	// 回転
	m_rot2 = D3DXVECTOR3(0, 0, 0);	// 回転
	m_rot3 = D3DXVECTOR3(0, 0, 0);	// 回転
	m_nCntCol0 = 0;					// 回転用カウンタ
	m_nCntCol1 = 0;					// 回転用カウンタ
	m_nCntCol2 = 0;					// 回転用カウンタ
	m_nCntCol3 = 0;					// 回転用カウンタ
	m_nCntPos0 = 21;				// 位置変更用カウンタ
	m_nCntPos1 = 21;				// 位置変更用カウンタ
	m_nCntPos2 = 21;				// 位置変更用カウンタ
	m_nCntPos3 = 21;				// 位置変更用カウンタ
	m_fNowHP = m_fMaxHP;			// 現在のHP
	m_fHeight = MAX_HEIGHT;			// 高さ

	// 最大本数までカウント
	for (int nCnt = 0; nCnt < MAX_HPBAR; nCnt++)
	{
		m_bBar[nCnt] = true;			// 全てtureにする
	}

	// Barの生成
	m_pBar = CBar::Create();

	// Barがあるとき
	if (m_pBar != NULL)
	{
		// 初期化用カウンタが0のとき
		if (m_nInitCnt == 0)
		{
			// Barフレーム
			m_pBar->SetVertexBar(0, BAR1_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(1, BAR0_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Barフレーム
			m_pBar->SetVertexBar(2, BAR3_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(3, BAR2_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Barフレーム
			m_pBar->SetVertexBar(4, BAR5_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(5, BAR4_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Barフレーム
			m_pBar->SetVertexBar(6, BAR7_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(7, BAR6_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Barフレーム
			m_pBar->SetVertexBar(8, BAR9_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(9, BAR8_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		}
		else if (m_nInitCnt == 1)
		{// 初期化用カウンタが1のとき
			// Barフレーム
			m_pBar->SetVertexBar(0, BAR1_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(1, BAR0_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Barフレーム
			m_pBar->SetVertexBar(2, BAR3_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(3, BAR2_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Barフレーム
			m_pBar->SetVertexBar(4, BAR5_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(5, BAR4_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Barフレーム
			m_pBar->SetVertexBar(6, BAR7_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(7, BAR6_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);

			// Barフレーム
			m_pBar->SetVertexBar(8, BAR9_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			// BarHP
			m_pBar->SetVertexBar(9, BAR8_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, MAX_HEIGHT);
		}
	}

	// 初期化用カウンタ加算
	m_nInitCnt++;
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

	delete m_pBar;					// メモリ削除
	m_pBar = nullptr;				// ポインタNULL
}

//==================================================================================================================
//	更新処理
//==================================================================================================================
void CHitPoint::Update(void)
{
	// Barの更新処理
	m_pBar->Update();

	// プレイヤーの情報ポインタ取得
	m_pPlayer = CGame::GetPlayer(m_nPlayerNum);

	float NowHP = m_pPlayer->GetLife();		// 現在のHP

	// HPバーが一本目のとき
	if (NowHP > (m_fMaxHP / 5) * 4)
	{
		// バー0の更新処理
		Bar0Update(NowHP);
	}

	// HPバーが二本目のとき
	if (NowHP > (m_fMaxHP / 5) * 3)
	{
		// バー1の更新処理
		Bar1Update(NowHP);
	}

	// HPバーが三本目のとき
	if (NowHP > (m_fMaxHP / 5) * 2)
	{

		// バー2の更新処理
		Bar2Update(NowHP);
	}

	// HPバーが四本目のとき
	if (NowHP > (m_fMaxHP / 5) * 1)
	{
		// バー3の更新処理
		Bar3Update(NowHP);
	}

	// HPバーが五本目のとき
	if (NowHP >= 0)
	{
		// バー4の更新処理
		Bar4Update(NowHP);
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
CHitPoint * CHitPoint::Create(int nNumber, float fMaxHP)
{
	CHitPoint *pHitPoint = NULL;				// ポインタ変数

	pHitPoint = new CHitPoint(PRIORITY_UI);		// メモリを動的確保
	pHitPoint->m_fMaxHP = fMaxHP;				// プレイヤー最大HP代入
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
//	プレイヤー番号設定処理
//==================================================================================================================
void CHitPoint::SetnPlayerNum(int nPlayerNum)
{
	m_nPlayerNum = nPlayerNum;
}

//==================================================================================================================
//	位置の取得
//==================================================================================================================
D3DXVECTOR3 CHitPoint::GetPos(void)
{
	return m_pos;
}

//==================================================================================================================
//	バー0の更新処理
//==================================================================================================================
void CHitPoint::Bar0Update(float NowHP)
{
	float fHeight = 0.0f;									// 現在の画像の高さ
	m_fNowHP = (NowHP - m_fMaxHP / 5 * 4) / (m_fMaxHP / 5);	// HPの比率計算
	fHeight = m_fNowHP * m_fHeight;							// 描画すべき画像幅がいくらなのか
	m_rot0.z = 0.0f;										// 回転Z初期化
	m_nCntCol0 = 0;											// α値減少カウンタ初期化
	m_nPlayerNum = m_pPlayer->GetnPlayer();					// プレイヤー番号取得

	// プレイヤー番号が0のとき
	if (m_nPlayerNum == 0)
	{
		// バーの0番目がtrueのとき
		if (m_bBar[0])
		{
			// バーの位置Yが規定値より以下のとき
			if (DROP_SPEED * m_nCntPos0 < BAR_HPFRAME_HEIGHT)
			{
				// BarHPの設定
				m_pBar->SetVertexBar(1, D3DXVECTOR3(250, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos0, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(0, D3DXVECTOR3(250, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos0, 0), NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// 位置加算
				m_nCntPos0++;
			}
			else
			{
				// BarHPの設定
				m_pBar->SetVertexBar(1, BAR0_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Barフレーム
				m_pBar->SetVertexBar(0, BAR1_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// バー0番目がfalseのとき
			m_nCntPos0 = 0;		// 位置変更用カウンタ初期化
			m_bBar[0] = true;	// 0番目はtrue;
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤーの番号が1のとき
	 // バーの0番目がtrueのとき
		if (m_bBar[0])
		{
			// バーの位置Yが規定値より以下のとき
			if (DROP_SPEED * m_nCntPos0 < BAR_HPFRAME_HEIGHT)
			{
				// BarHPの設定
				m_pBar->SetVertexBar(1, D3DXVECTOR3(1010, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos0, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(0, D3DXVECTOR3(1010, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos0, 0), NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// 位置加算
				m_nCntPos0++;
			}
			else
			{
				// BarHPの設定
				m_pBar->SetVertexBar(1, BAR0_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Barフレーム
				m_pBar->SetVertexBar(0, BAR1_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// バー0番目がfalseのとき
			m_nCntPos0 = 0;		// 位置変更用カウンタ初期化
			m_bBar[0] = true;	// 0番目はtrue;
		}
	}
}

//==================================================================================================================
//	バー1の更新処理
//==================================================================================================================
void CHitPoint::Bar1Update(float NowHP)
{
	float fHeight = 0.0f;			// 現在の画像の高さ
	m_rot1.z = 0.0f;				// 回転Z初期化
	m_nCntCol1 = 0;					// α値減少カウンタ初期化

	// 現在のHP本数が四本以下のとき
	if (NowHP < (m_fMaxHP / 5) * 4)
	{
		m_bBar[0] = false;			// 0番目はfalse
	}

	// HPバー一本目があるとき
	if (m_bBar[0])
	{
		// HPの比率計算
		m_fNowHP = 1.0f;
	}
	else
	{// HPバー一本目が無いとき
		// HPの比率計算
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 3) / (m_fMaxHP / 5);
	}
	fHeight = m_fNowHP * m_fHeight;	// 描画すべき画像幅がいくらなのか

	// プレイヤーの番号が0番目のとき
	if (m_nPlayerNum == 0)
	{
		// バーの1番目がtrueのとき
		if (m_bBar[1])
		{
			// バーの位置Yが規定値より以下のとき
			if (DROP_SPEED * m_nCntPos1 < BAR_HPFRAME_HEIGHT)
			{
				// BarHPの設定
				m_pBar->SetVertexBar(3, D3DXVECTOR3(210, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos1, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(2, D3DXVECTOR3(210, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos1, 0), NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// 位置加算
				m_nCntPos1++;
			}
			else
			{
				// BarHPの設定
				m_pBar->SetVertexBar(3, BAR2_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Barフレーム
				m_pBar->SetVertexBar(2, BAR3_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// バーの1番目がfalseのとき
			m_nCntPos1 = 0;		// 位置変更用カウンタ初期化
			m_bBar[1] = true;	// 1番目はtrue;
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤーの番号が1番目のとき
	 // バーの1番目がtrueのとき
		if (m_bBar[1])
		{
			// バーの位置Yが規定値より以下のとき
			if (DROP_SPEED * m_nCntPos1 < BAR_HPFRAME_HEIGHT)
			{
				// BarHPの設定
				m_pBar->SetVertexBar(3, D3DXVECTOR3(1050, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos1, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(2, D3DXVECTOR3(1050, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos1, 0), NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// 位置加算
				m_nCntPos1++;
			}
			else
			{
				// BarHPの設定
				m_pBar->SetVertexBar(3, BAR2_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Barフレーム
				m_pBar->SetVertexBar(2, BAR3_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// バーの1番目がfalseのとき
			m_nCntPos1 = 0;		// 位置変更用カウンタ初期化
			m_bBar[1] = true;	// 1番目はtrue;

		}

	}

	// プレイヤーの番号が0番目のとき
	if (m_nPlayerNum == 0)
	{
		// HPバー0番目がfalseのとき
		if (!m_bBar[0])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot0.z <= D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(1, BAR0_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(0, BAR1_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// バー色設定
				m_pBar->SetColBar(1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot0.z += ROT_ANGLE;

				// α値加算
				m_nCntCol0++;
			}
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤーの番号が1番目のとき
	 // HPバー0番目がfalseのとき
		if (!m_bBar[0])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot0.z >= -D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(1, BAR0_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(0, BAR1_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// バー色設定
				m_pBar->SetColBar(1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol0 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot0.z -= ROT_ANGLE;

				// α値加算
				m_nCntCol0++;
			}
		}
	}
}

//==================================================================================================================
//	バー2の更新処理
//==================================================================================================================
void CHitPoint::Bar2Update(float NowHP)
{
	float fHeight = 0.0f;	// 現在の画像の高さ
	m_rot2.z = 0.0f;		// 回転Z初期化
	m_nCntCol2 = 0;			// α値減少カウンタ初期化

	// 現在のHP本数が三本以下のとき
	if (NowHP < (m_fMaxHP / 5) * 3)
	{
		m_bBar[0] = false;			// 0番目はfalse
		m_bBar[1] = false;			// 1番目はfalse
	}

	// HPバー二本目があるとき
	if (m_bBar[1])
	{
		// HPの比率計算
		m_fNowHP = 1.0f;
	}
	else
	{// HPバー二本目が無いとき
		// HPの比率計算
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 2) / (m_fMaxHP / 5);
	}
	fHeight = m_fNowHP * m_fHeight;	// 描画すべき画像幅がいくらなのか

	// プレイヤーの番号が0番目のとき
	if (m_nPlayerNum == 0)
	{
		// バー1がtrueのとき
		if (m_bBar[2])
		{
			// バーの位置Yが規定値より以下のとき
			if (DROP_SPEED * m_nCntPos2 < BAR_HPFRAME_HEIGHT)
			{
				// BarHPの設定
				m_pBar->SetVertexBar(5, D3DXVECTOR3(170, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos2, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(4, D3DXVECTOR3(170, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos2, 0), NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// 位置加算
				m_nCntPos2++;
			}
			else
			{
				// BarHPの設定
				m_pBar->SetVertexBar(5, BAR4_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Barフレーム
				m_pBar->SetVertexBar(4, BAR5_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// バー1がfalseのとき
			m_nCntPos2 = 0;		// 位置変更用カウンタ初期化
			m_bBar[2] = true;	// 2番目はtrue;
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤーの番号が1番目のとき
	 // バー1がtrueのとき
		if (m_bBar[2])
		{
			// バーの位置Yが規定値より以下のとき
			if (DROP_SPEED * m_nCntPos2 < BAR_HPFRAME_HEIGHT)
			{
				// BarHPの設定
				m_pBar->SetVertexBar(5, D3DXVECTOR3(1090, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos2, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(4, D3DXVECTOR3(1090, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos2, 0), NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// 位置加算
				m_nCntPos2++;
			}
			else
			{
				// BarHPの設定
				m_pBar->SetVertexBar(5, BAR4_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Barフレーム
				m_pBar->SetVertexBar(4, BAR5_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// バー1がfalseのとき
			m_nCntPos2 = 0;		// 位置変更用カウンタ初期化
			m_bBar[2] = true;	// 2番目はtrue;
		}
	}

	// プレイヤーの番号が0番目のとき
	if (m_nPlayerNum == 0)
	{
		// HPバー1番目がfalseのとき
		if (!m_bBar[1])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot1.z <= D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(3, BAR2_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(2, BAR3_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// バー色設定
				m_pBar->SetColBar(3, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot1.z += ROT_ANGLE;

				// α値加算
				m_nCntCol1++;
			}
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤーの番号が1番目のとき
	 // HPバー1番目がfalseのとき
		if (!m_bBar[1])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot1.z >= -D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(3, BAR2_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(2, BAR3_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// バー色設定
				m_pBar->SetColBar(3, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol1 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot1.z -= ROT_ANGLE;

				// α値加算
				m_nCntCol1++;
			}
		}
	}
}

//==================================================================================================================
//	バー3の更新処理
//==================================================================================================================
void CHitPoint::Bar3Update(float NowHP)
{
	float fHeight = 0.0f;	// 現在の画像の高さ
	m_rot3.z = 0.0f;		// 回転Z初期化
	m_nCntCol3 = 0;			// α値減少カウンタ初期化

	// 現在のHP本数が二本以下のとき
	if (NowHP < (m_fMaxHP / 5) * 2)
	{
		m_bBar[0] = false;			// 0番目はfalse
		m_bBar[1] = false;			// 1番目はfalse
		m_bBar[2] = false;			// 2番目はfalse
	}

	// HPバー二本目があるとき
	if (m_bBar[2])
	{
		// HPの比率計算
		m_fNowHP = 1.0f;
	}
	else
	{// HPバー二本目が無いとき
		// HPの比率計算
		m_fNowHP = (NowHP - m_fMaxHP / 5 * 1) / (m_fMaxHP / 5);
	}
	fHeight = m_fNowHP * m_fHeight;	// 描画すべき画像幅がいくらなのか

	// プレイヤー番号が0番目のとき
	if (m_nPlayerNum == 0)
	{
		// バー3がtrueのとき
		if (m_bBar[3])
		{
			// バーの位置Yが規定値より以下のとき
			if (DROP_SPEED * m_nCntPos3 < BAR_HPFRAME_HEIGHT)
			{
				// BarHPの設定
				m_pBar->SetVertexBar(7, D3DXVECTOR3(130, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos3, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(6, D3DXVECTOR3(130, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos3, 0), NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// 位置加算
				m_nCntPos3++;
			}
			else
			{
				// BarHPの設定
				m_pBar->SetVertexBar(7, BAR6_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Barフレーム
				m_pBar->SetVertexBar(6, BAR7_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// バー3がfalseのとき
			m_nCntPos3 = 0;		// 位置変更用カウンタ初期化
			m_bBar[3] = true;	// 3番目はtrue;
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤー番号が1番目のとき
	 // バー3がtrueのとき
		if (m_bBar[3])
		{
			// バーの位置Yが規定値より以下のとき
			if (DROP_SPEED * m_nCntPos3 < BAR_HPFRAME_HEIGHT)
			{
				// BarHPの設定
				m_pBar->SetVertexBar(7, D3DXVECTOR3(1130, DROP_DIFFRENCE - ROT_DIFF + DROP_SPEED * m_nCntPos3, 0), D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				m_pBar->SetVertexBar(6, D3DXVECTOR3(1130, DROP_DIFFRENCE + DROP_SPEED * m_nCntPos3, 0), NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);

				// 位置加算
				m_nCntPos3++;
			}
			else
			{
				// BarHPの設定
				m_pBar->SetVertexBar(7, BAR6_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
				// Barフレーム
				m_pBar->SetVertexBar(6, BAR7_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
			}
		}
		else
		{// バー3がfalseのとき
			m_nCntPos3 = 0;		// 位置変更用カウンタ初期化
			m_bBar[3] = true;	// 3番目はtrue;
		}
	}

	// プレイヤーの番号が0番目のとき
	if (m_nPlayerNum == 0)
	{
		// HPバー2番目がfalseのとき
		if (!m_bBar[2])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot2.z <= D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(5, BAR4_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(4, BAR5_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// バー色設定
				m_pBar->SetColBar(5, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot2.z += ROT_ANGLE;

				// α値加算
				m_nCntCol2++;
			}
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤーの番号が1番目のとき
	 // HPバー2番目がfalseのとき
		if (!m_bBar[2])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot2.z >= -D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(5, BAR4_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(4, BAR5_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// バー色設定
				m_pBar->SetColBar(5, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol2 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot2.z -= ROT_ANGLE;

				// α値加算
				m_nCntCol2++;
			}
		}
	}
}

//==================================================================================================================
//	バー4の更新処理
//==================================================================================================================
void CHitPoint::Bar4Update(float NowHP)
{
	float fHeight = 0.0f;	// 現在の画像の高さ

	// 現在のHP本数が一本以下のとき
	if (NowHP < (m_fMaxHP / 5) * 1)
	{
		m_bBar[0] = false;			// 0番目はfalse
		m_bBar[1] = false;			// 1番目はfalse
		m_bBar[2] = false;			// 2番目はfalse
		m_bBar[3] = false;			// 3番目はfalse
		m_bBar[4] = true;			// 4番目はtrue
	}

	// HPバー二本目があるとき
	if (m_bBar[3])
	{
		// HPの比率計算
		m_fNowHP = 1.0f;
	}
	else
	{// HPバー二本目が無いとき
	 // HPの比率計算
		m_fNowHP = NowHP / (m_fMaxHP / 5);
	}
	fHeight = m_fNowHP * m_fHeight;	// 描画すべき画像幅がいくらなのか

	// プレイヤーの番号が0番目のとき
	if (m_nPlayerNum == 0)
	{
		// バーの4番目がtrueのとき
		if (m_bBar[4])
		{
			// BarHPの設定
			m_pBar->SetVertexBar(9, BAR8_LEFT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
			// Barフレーム
			m_pBar->SetVertexBar(8, BAR9_LEFT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤーの番号が1番目のとき
	 // バーの4番目がtrueのとき
		if (m_bBar[4])
		{
			// BarHPの設定
			m_pBar->SetVertexBar(9, BAR8_RIGHT_POS, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), MAX_WIDTH, fHeight);
			// Barフレーム
			m_pBar->SetVertexBar(8, BAR9_RIGHT_POS, NORMAL_COLOR, MAX_WIDTH_FREAM, MAX_HEIGHT_FREAME);
		}

	}

	// プレイヤーの番号が0番目のとき
	if (m_nPlayerNum == 0)
	{
		// HPバー3番目がfalseのとき
		if (!m_bBar[3])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot3.z <= D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(7, BAR6_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(6, BAR7_LEFT_POS, ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// バー色設定
				m_pBar->SetColBar(7, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot3.z += ROT_ANGLE;

				// α値加算
				m_nCntCol3++;
			}
		}
	}
	else if (m_nPlayerNum == 1)
	{// プレイヤーの番号が1番目のとき
	 // HPバー3番目がfalseのとき
		if (!m_bBar[3])
		{
			// 角度ZがD3DX_PI/2以下のとき
			if (m_rot3.z >= -D3DX_PI / 2)
			{
				// バー回転設定
				m_pBar->RotBar(7, BAR6_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, ROT_DIFF, 0));
				m_pBar->RotBar(6, BAR7_RIGHT_POS, -ROT_ANGLE, D3DXVECTOR3(0, 0, 0));

				// バー色設定
				m_pBar->SetColBar(7, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));
				m_pBar->SetColBar(6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_nCntCol3 * (1.0f / ALPHA_DEGRESS)));

				// 角度加算
				m_rot3.z -= ROT_ANGLE;

				// α値加算
				m_nCntCol3++;
			}
		}
	}
}
