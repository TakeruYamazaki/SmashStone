//==================================================================================================================
//
// UI処理[UI.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "scene2D.h"
#include "debugProc.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define START_SIZE 180.0f							// スタート時の大きさ
#define LOGO_START_SIZEX 400.0f						// スタートロゴの横の大きさ
#define LOGO_START_SIZEY 200.0f						// スタートロゴの縦の大きさ
#define TITLEFADE_POSX -1500.0f						// タイトルロゴの初期位置X
#define TITLEFADE_POSY 250.0f						// タイトルロゴの初期位置Y
#define TITLELOGO_SIZEX 600.0f						// タイトルロゴの大きさX
#define TITLELOGO_SIZEY 300.0f						// タイトルロゴの大きさY
#define TITLE_CENTER_POSX 620.0f					// タイトルロゴ中央の位置X
#define TITLELOGO_SPEEDX 5.9f						// タイトルロゴをずらすスピード
#define TUTORIALLOGO_POSX1 1395.0f					// チュートリアルロゴ位置X1
#define TUTORIALLOGO_POSX2 630.0f					// チュートリアルロゴ位置X2
#define TUTORIALLOGO_POSY 620.0f					// チュートリアルロゴ位置Y
#define TUTORIALLOGO_SIZEX 270.0f					// チュートリアルロゴ大きさX
#define TUTORIALLOGO_SIZEY 130.0f					// チュートリアルロゴ大きさY
#define ENTER_SIZEX 300.0f							// エンターロゴX
#define ENTER_SIZEY 150.0f							// エンターロゴY
#define TITLE_ENTER_POSY 600.0f						// タイトルエンターロゴ位置Y
#define TUTORIAL_FREAMLOGO_POSX 640.0f				// チュートリアルフレームロゴ位置X
#define TUTORIAL_FREAMLOGO_POSY 620.0f				// チュートリアルフレームロゴ位置Y
#define TUTORIAL_FREAMLOGO_SIZEX 1500.0f			// チュートリアルフレームロゴ大きさX
#define TUTORIAL_FREAMLOGO_SIZEY 130.0f				// チュートリアルフレームロゴ大きさY
#define TUTORIAL_LOGO_POSX 1200.0f					// チュートリアルロゴ位置X
#define TUTORIAL_LOGO_POSY 620.0f					// チュートリアルロゴ位置Y
#define TUTORIAL_LOGO_SIZEX 230.0f					// チュートリアルロゴ大きさX
#define TUTORIAL_LOGO_SIZEY 130.0f					// チュートリアルロゴ大きさY
#define TUTORIAL_LOGO2_POSX -40.0f					// チュートリアルロゴ2位置X
#define TUTORIAL_LOGO2_POSY 620.0f					// チュートリアルロゴ2位置Y
#define TUTORIAL_LOGO2_SIZEX 230.0f					// チュートリアルロゴ2大きさX
#define TUTORIAL_LOGO2_SIZEY 130.0f					// チュートリアルロゴ2大きさY
#define TUTORIAL_ENTERLOGO_POSX 500.0f				// チュートリアルエンターロゴ位置X
#define TUTORIAL_ENTERLOGO_POSY 670.0f				// チュートリアルエンターロゴ位置Y
#define TUTORIAL_ENTERLOGO_SIZEX 220.0f				// チュートリアルエンターロゴ大きさX
#define TUTORIAL_ENTERLOGO_SIZEY 120.0f				// チュートリアルエンターロゴ大きさY
#define TUTORIAL_DELOGO_POSX 640.0f					// チュートリアルでロゴ位置X
#define TUTORIAL_DELOGO_POSY 670.0f					// チュートリアルでロゴ位置Y
#define TUTORIAL_DELOGO_SIZEX 90.0f					// チュートリアルでロゴ大きさX
#define TUTORIAL_DELOGO_SIZEY 90.0f					// チュートリアルでロゴ大きさY
#define TUTORIAL_SKIPLOGO_POSX 790.0f				// チュートリアルスキップロゴ位置X
#define TUTORIAL_SKIPLOGO_POSY 670.0f				// チュートリアルスキップロゴ位置Y
#define TUTORIAL_SKIPLOGO_SIZEX 220.0f				// チュートリアルスキップロゴ大きさX
#define TUTORIAL_SKIPLOGO_SIZEY 120.0f				// チュートリアルスキップロゴ大きさY
#define TUTORIAL_ALOGO_POSX 150.0f					// チュートリアルAロゴ位置X
#define TUTORIAL_ALOGO_POSY 200.0f					// チュートリアルAロゴ位置Y
#define TUTORIAL_ALOGO_SIZEX 110.0f					// チュートリアルAロゴ大きさX
#define TUTORIAL_ALOGO_SIZEY 110.0f					// チュートリアルAロゴ大きさY
#define TUTORIAL_LEFTLOGO_POSX 150.0f				// チュートリアル左旋回ロゴ位置X
#define TUTORIAL_LEFTLOGO_POSY 250.0f				// チュートリアル左旋回ロゴ位置Y
#define TUTORIAL_LEFTLOGO_SIZEX 200.0f				// チュートリアル左旋回ロゴ大きさX
#define TUTORIAL_LEFTLOGO_SIZEY 100.0f				// チュートリアル左旋回ロゴ大きさY
#define TUTORIAL_DLOGO_POSX 1110.0f					// チュートリアルDロゴ位置X
#define TUTORIAL_DLOGO_POSY 200.0f					// チュートリアルDロゴ位置Y
#define TUTORIAL_DLOGO_SIZEX 110.0f					// チュートリアルDロゴ大きさX
#define TUTORIAL_DLOGO_SIZEY 110.0f					// チュートリアルDロゴ大きさY
#define TUTORIAL_RIGHTLOGO_POSX 1110.0f				// チュートリアル右旋回ロゴ位置X
#define TUTORIAL_RIGHTLOGO_POSY 250.0f				// チュートリアル右旋回ロゴ位置Y
#define TUTORIAL_RIGHTLOGO_SIZEX 200.0f				// チュートリアル右旋回ロゴ大きさX
#define TUTORIAL_RIGHTLOGO_SIZEY 100.0f				// チュートリアル右旋回ロゴ大きさY
#define TUTORIAL_SLOGO_POSX 300.0f					// チュートリアルSロゴ位置X
#define TUTORIAL_SLOGO_POSY 400.0f					// チュートリアルSロゴ位置Y
#define TUTORIAL_SLOGO_SIZEX 110.0f					// チュートリアルSロゴ大きさX
#define TUTORIAL_SLOGO_SIZEY 110.0f					// チュートリアルSロゴ大きさY
#define TUTORIAL_SPEEDLOGO_POSX 300.0f				// チュートリアル減速ロゴ位置X
#define TUTORIAL_SPEEDLOGO_POSY 450.0f				// チュートリアル減速ロゴ位置Y
#define TUTORIAL_SPEEDLOGO_SIZEX 130.0f				// チュートリアル減速ロゴ大きさX
#define TUTORIAL_SPEEDLOGO_SIZEY 100.0f				// チュートリアル減速ロゴ大きさY
#define TUTORIAL_SPACELOGO_POSX 950.0f				// チュートリアルスペースロゴ位置X
#define TUTORIAL_SPACELOGO_POSY 400.0f				// チュートリアルスペースロゴ位置Y
#define TUTORIAL_SPACELOGO_SIZEX 110.0f				// チュートリアルスペースロゴ大きさX
#define TUTORIAL_SPACELOGO_SIZEY 110.0f				// チュートリアルスペースロゴ大きさY
#define TUTORIAL_JUMPLOGO_POSX 950.0f				// チュートリアルジャンプロゴ位置X
#define TUTORIAL_JUMPLOGO_POSY 450.0f				// チュートリアルジャンプロゴ位置Y
#define TUTORIAL_JUMPLOGO_SIZEX 260.0f				// チュートリアルジャンプロゴ大きさX
#define TUTORIAL_JUMPLOGO_SIZEY 90.0f				// チュートリアルジャンプロゴ大きさY
#define GAME_TIMEFREAMELOGO_POSX 640.0f				// ゲームタイマーフレーム位置X
#define GAME_TIMEFREAMELOGO_POSY 650.0f				// ゲームタイマーフレーム位置Y
#define GAME_TIMEFREAMELOGO_SIZEX 400.0f			// ゲームタイマーフレーム大きさX
#define GAME_TIMEFREAMELOGO_SIZEY 150.0f			// ゲームタイマーフレーム大きさY
#define GAME_NOWTIMEFREAMELOGO_POSX 540.0f			// ゲーム現在のタイマーフレーム位置X
#define GAME_NOWTIMEFREAMELOGO_POSY 610.0f			// ゲーム現在のタイマーフレーム位置Y
#define GAME_NOWTIMEFREAMELOGO_SIZEX 100.0f			// ゲーム現在のタイマーフレーム大きさX
#define GAME_NOWTIMEFREAMELOGO_SIZEY 50.0f			// ゲーム現在のタイマーフレーム大きさY
#define GAME_BESTTIME_POSX 520.0f					// ゲーム最高のタイムロゴ位置X
#define GAME_BESTTIME_POSY 680.0f					// ゲーム最高のタイムロゴ位置Y
#define GAME_BESTTIME_SIZEX 140.0f					// ゲーム最高のタイムロゴ大きさX
#define GAME_BESTTIME_SIZEY 50.0f					// ゲーム最高のタイムロゴ大きさY
#define GAME_DOT0_POSX 677.0f						// ゲームタイムとタイムの間の点0ロゴ位置X
#define GAME_DOT0_POSY 620.0f						// ゲームタイムとタイムの間の点0ロゴ位置Y
#define GAME_DOT0_SIZEX 100.0f						// ゲームタイムとタイムの間の点0ロゴ大きさX
#define GAME_DOT0_SIZEY 100.0f						// ゲームタイムとタイムの間の点0ロゴ大きさY
#define GAME_DOT1_POSX 757.0f						// ゲームタイムとタイムの間の点1ロゴ位置X
#define GAME_DOT1_POSY 620.0f						// ゲームタイムとタイムの間の点1ロゴ位置Y
#define GAME_DOT1_SIZEX 100.0f						// ゲームタイムとタイムの間の点1ロゴ大きさX
#define GAME_DOT1_SIZEY 100.0f						// ゲームタイムとタイムの間の点1ロゴ大きさY
#define GAME_DOT2_POSX 677.0f						// ゲームタイムとタイムの間の点2ロゴ位置X
#define GAME_DOT2_POSY 690.0f						// ゲームタイムとタイムの間の点2ロゴ位置Y
#define GAME_DOT2_SIZEX 100.0f						// ゲームタイムとタイムの間の点2ロゴ大きさX
#define GAME_DOT2_SIZEY 100.0f						// ゲームタイムとタイムの間の点2ロゴ大きさY
#define GAME_DOT3_POSX 757.0f						// ゲームタイムとタイムの間の点3ロゴ位置X
#define GAME_DOT3_POSY 690.0f						// ゲームタイムとタイムの間の点3ロゴ位置Y
#define GAME_DOT3_SIZEX 100.0f						// ゲームタイムとタイムの間の点3ロゴ大きさX
#define GAME_DOT3_SIZEY 100.0f						// ゲームタイムとタイムの間の点3ロゴ大きさY
#define GAME_PAUSE_POSX 207.0f						// ゲームポーズロゴ位置X
#define GAME_PAUSE_POSY 50.0f						// ゲームポーズロゴ位置Y
#define GAME_PAUSE_SIZEX 400.0f						// ゲームポーズロゴ大きさX
#define GAME_PAUSE_SIZEY 130.0f						// ゲームポーズロゴ大きさY
#define RANKING_RANK_POSX 450.0f					// ランクロゴ位置X
#define RANKING_RANK_POSY 350.0f					// ランクロゴ位置Y
#define RANKING_RANK_SIZEX 180.0f					// ランクロゴ大きさX
#define RANKING_RANK_SIZEY 500.0f					// ランクロゴ大きさY
#define RANKING_LOGO_POSX SCREEN_WIDTH / 2			// ランキングロゴ位置X
#define RANKING_LOGO_POSY 50.0f						// ランキングロゴ位置Y
#define RANKING_LOGO_SIZEX 300.0f					// ランキングロゴ大きさX
#define RANKING_LOGO_SIZEY 100.0f					// ランキングロゴ大きさY
#define RANKING_ENTER_POSX 1000.0f					// ランキングエンターロゴ位置X
#define RANKING_ENTER_POSY 620.0f					// ランキングエンターロゴ位置Y
#define RANKING_ENTER_SIZEX 300.0f					// ランキングエンターロゴ大きさX
#define RANKING_ENTER_SIZEY 100.0f					// ランキングエンターロゴ大きさY
#define RANKING_DOT0_POSX 595.0f					// ランキング数字と数字の間0の点位置X
#define RANKING_DOT0_POSY 163.0f					// ランキング数字と数字の間0の点位置Y
#define RANKING_DOT_SIZE 130.0f						// ランキング数字と数字の間の点大きさ
#define RANKING_DOT1_POSX 703.0f					// ランキング数字と数字の間1の点位置X
#define RANKING_DOT1_POSY 163.0f					// ランキング数字と数字の間1の点位置Y
#define RANKING_DOT2_POSX 595.0f					// ランキング数字と数字の間2の点位置X
#define RANKING_DOT2_POSY 263.0f					// ランキング数字と数字の間2の点位置Y
#define RANKING_DOT3_POSX 703.0f					// ランキング数字と数字の間3の点位置X
#define RANKING_DOT3_POSY 263.0f					// ランキング数字と数字の間3の点位置Y
#define RANKING_DOT4_POSX 595.0f					// ランキング数字と数字の間4の点位置X
#define RANKING_DOT4_POSY 363.0f					// ランキング数字と数字の間4の点位置Y
#define RANKING_DOT5_POSX 703.0f					// ランキング数字と数字の間5の点位置X
#define RANKING_DOT5_POSY 363.0f					// ランキング数字と数字の間5の点位置Y
#define RANKING_DOT6_POSX 595.0f					// ランキング数字と数字の間6の点位置X
#define RANKING_DOT6_POSY 463.0f					// ランキング数字と数字の間6の点位置Y
#define RANKING_DOT7_POSX 703.0f					// ランキング数字と数字の間7の点位置X
#define RANKING_DOT7_POSY 463.0f					// ランキング数字と数字の間7の点位置Y
#define RANKING_DOT8_POSX 595.0f					// ランキング数字と数字の間8の点位置X
#define RANKING_DOT8_POSY 563.0f					// ランキング数字と数字の間8の点位置Y
#define RANKING_DOT9_POSX 703.0f					// ランキング数字と数字の間9の点位置X
#define RANKING_DOT9_POSY 563.0f					// ランキング数字と数字の間9の点位置Y
#define TUTORIAL_MOVE 2.0f							// チュートリアルロゴの移動量
#define TUTORIAL_PRESSJUMP_POSX 640.0f				// チュートリアル試し泳ぎロゴ位置X
#define TUTORIAL_PRESSJUMP_POSY 290.0f				// チュートリアル試し泳ぎロゴ位置Y
#define TUTORIAL_PRESSJUMP_SIZEX 300.0f				// チュートリアル試し泳ぎロゴ大きさX
#define TUTORIAL_PRESSJUMP_SIZEY 150.0f				// チュートリアル試し泳ぎロゴ大きさY
#define TUTORIAL_PLAYSWIM_POSX 640.0f				// チュートリアル試し泳ぎロゴ位置X
#define TUTORIAL_PLAYSWIM_POSY 380.0f				// チュートリアル試し泳ぎロゴ位置Y
#define TUTORIAL_PLAYSWIM_SIZEX 400.0f				// チュートリアル試し泳ぎロゴ大きさX
#define TUTORIAL_PLAYSWIM_SIZEY 210.0f				// チュートリアル試し泳ぎロゴ大きさY
#define GOAL_COUNT 4								// ゴールカウント
#define GOAL_COUNTMAX 100							// ゴールカウントの最大数
#define GOAL_LOGO_POSY 250.0f						// ゴールロゴの位置Y

//==================================================================================================================
// 静的メンバー変数の初期化
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI::m_pTexture[LOGOTYPE_MAX] = {};		// テクスチャ情報
char *CUI::m_apFileName[LOGOTYPE_MAX] =						// 読み込むモデルのソース先
{
	{ "data/TEXTURE/title.png" },				// タイトル
	{ "data/TEXTURE/PRESS START.png" },			// エンター
	{ "data/TEXTURE/frame.png" },				// タイマーフレーム
	{ "data/TEXTURE/tutorial.png" },			// チュートリアルロゴ0
	{ "data/TEXTURE/tutorial.png" },			// チュートリアルロゴ1
	{ "data/TEXTURE/PRESS START.png" },			// チュートリアルエンターロゴ
	{ "data/TEXTURE/de.png" },					// でロゴ
	{ "data/TEXTURE/skip.png" },				// スキップロゴ
	{ "data/TEXTURE/pressJump.png" },			// ジャンプボタンで
	{ "data/TEXTURE/playswim.png" },			// 試し泳ぎロゴ
	{ "data/TEXTURE/left.jpg" },				// 左
	{ "data/TEXTURE/left.png" },				// 左旋回
	{ "data/TEXTURE/right.jpg" },				// 右
	{ "data/TEXTURE/right.png" },				// 右旋回
	{ "data/TEXTURE/Aボタン.jpg" },				// Aボタン
	{ "data/TEXTURE/speed down.png" },			// 減速
	{ "data/TEXTURE/Bボタン.jpg" },				// Bボタン
	{ "data/TEXTURE/jump.png" },				// Bボタン
	{ "data/TEXTURE/time.png" },				// 現在のタイム
	{ "data/TEXTURE/besttime.png" },			// 最高のタイム
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点0
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点1
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点2
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点3
	{ "data/TEXTURE/pauselogo.png" },			// ポーズロゴ
	{ "data/TEXTURE/goal.png" },				// ゴール
	{ "data/TEXTURE/start01.png" },				// スタート
	{ "data/TEXTURE/1.png" },					// カウント1
	{ "data/TEXTURE/2.png" },					// カウント2
	{ "data/TEXTURE/3.png" },					// カウント3
	{ "data/TEXTURE/Rank01.png" },				// ランク
	{ "data/TEXTURE/RankingLogo.png" },			// ランキングロゴ
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点4
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点5
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点6
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点7
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点8
	{ "data/TEXTURE/dot.png" },					// タイムとタイムの間の点9
};

//==================================================================================================================
//
// コンストラクタ
//
//==================================================================================================================
CUI::CUI(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// デストラクタ
//
//==================================================================================================================
CUI::~CUI()
{

}

//==================================================================================================================
// 初期化処理
//==================================================================================================================
void CUI::Init(void)
{
	m_nCounterAnim = 0;							// アニメーションカウンタ

	// ロゴの最大枚数カウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// タイトルのとき
		if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
		{
			// タイトルで使うロゴのとき
			if (nCnt <= LOGOTYPE_ENTER)
			{
				// 生成処理
				m_pScene2D[nCnt] = CScene2D::Create();

				// テクスチャを貼る
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}

			// ロゴによって切り替える
			switch (nCnt)
			{
				// タイトルロゴ
			case LOGOTYPE_TITLE:
				break;
				// エンター
			case LOGOTYPE_ENTER:
				SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, TITLE_ENTER_POSY, 0.0f), ENTER_SIZEX, ENTER_SIZEY, LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			}
		}

		// チュートリアルのとき
		if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
		{
			// チュートリアルで使うロゴのとき
			if (nCnt <= LOGOTYPE_JUMP && nCnt >= LOGOTYPE_TIMEFRAME)
			{
				// 生成処理
				m_pScene2D[nCnt] = CScene2D::Create();

				// テクスチャを貼る
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}

			// ロゴによって切り替える
			switch (nCnt)
			{
				// フレームロゴ
			case LOGOTYPE_TIMEFRAME:
				SetUI(D3DXVECTOR3(TUTORIAL_FREAMLOGO_POSX, TUTORIAL_FREAMLOGO_POSY, 0.0f), TUTORIAL_FREAMLOGO_SIZEX, TUTORIAL_FREAMLOGO_SIZEY, LOGOTYPE_TIMEFRAME, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// チュートリアルロゴ
			case LOGOTYPE_TUTORIAL0:
				SetUI(D3DXVECTOR3(TUTORIAL_LOGO_POSX, TUTORIAL_LOGO_POSY, 0.0f), TUTORIAL_LOGO_SIZEX, TUTORIAL_LOGO_SIZEY, LOGOTYPE_TUTORIAL0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// チュートリアルロゴ
			case LOGOTYPE_TUTORIAL1:
				SetUI(D3DXVECTOR3(TUTORIAL_LOGO2_POSX, TUTORIAL_LOGO2_POSY, 0.0f), TUTORIAL_LOGO2_SIZEX, TUTORIAL_LOGO2_SIZEY, LOGOTYPE_TUTORIAL1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// チュートリアルエンターロゴ
			case LOGOTYPE_TUTORIALENTER:
				SetUI(D3DXVECTOR3(TUTORIAL_ENTERLOGO_POSX, TUTORIAL_ENTERLOGO_POSY, 0.0f), TUTORIAL_ENTERLOGO_SIZEX, TUTORIAL_ENTERLOGO_SIZEY, LOGOTYPE_TUTORIALENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// でロゴ
			case LOGOTYPE_DE:
				SetUI(D3DXVECTOR3(TUTORIAL_DELOGO_POSX, TUTORIAL_DELOGO_POSY, 0.0f), TUTORIAL_DELOGO_SIZEX, TUTORIAL_DELOGO_SIZEY, LOGOTYPE_DE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// スキップロゴ
			case LOGOTYPE_SKIP:
				SetUI(D3DXVECTOR3(TUTORIAL_SKIPLOGO_POSX, TUTORIAL_SKIPLOGO_POSY, 0.0f), TUTORIAL_SKIPLOGO_SIZEX, TUTORIAL_SKIPLOGO_SIZEY, LOGOTYPE_SKIP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// キーボードのA
			case LOGOTYPE_A:
				SetUI(D3DXVECTOR3(TUTORIAL_ALOGO_POSX, TUTORIAL_ALOGO_POSY, 0.0f), TUTORIAL_ALOGO_SIZEX, TUTORIAL_ALOGO_SIZEY, LOGOTYPE_A, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 左旋回
			case LOGOTYPE_LEFT:
				SetUI(D3DXVECTOR3(TUTORIAL_LEFTLOGO_POSX, TUTORIAL_LEFTLOGO_POSY, 0.0f), TUTORIAL_LEFTLOGO_SIZEX, TUTORIAL_LEFTLOGO_SIZEY, LOGOTYPE_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// キーボードのD
			case LOGOTYPE_D:
				SetUI(D3DXVECTOR3(TUTORIAL_DLOGO_POSX, TUTORIAL_DLOGO_POSY, 0.0f), TUTORIAL_DLOGO_SIZEX, TUTORIAL_DLOGO_SIZEY, LOGOTYPE_D, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 右旋回
			case LOGOTYPE_RIGHT:
				SetUI(D3DXVECTOR3(TUTORIAL_RIGHTLOGO_POSX, TUTORIAL_RIGHTLOGO_POSY, 0.0f), TUTORIAL_RIGHTLOGO_SIZEX, TUTORIAL_RIGHTLOGO_SIZEY, LOGOTYPE_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// キーボードのS
			case LOGOTYPE_S:
				SetUI(D3DXVECTOR3(TUTORIAL_SLOGO_POSX, TUTORIAL_SLOGO_POSY, 0.0f), TUTORIAL_SLOGO_SIZEX, TUTORIAL_SLOGO_SIZEY, LOGOTYPE_S, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 減速ロゴ
			case LOGOTYPE_REDUCE_SPEED:
				SetUI(D3DXVECTOR3(TUTORIAL_SPEEDLOGO_POSX, TUTORIAL_SPEEDLOGO_POSY, 0.0f), TUTORIAL_SPEEDLOGO_SIZEX, TUTORIAL_SPEEDLOGO_SIZEY, LOGOTYPE_REDUCE_SPEED, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// キーボードのスペース
			case LOGOTYPE_SPACE:
				SetUI(D3DXVECTOR3(TUTORIAL_SPACELOGO_POSX, TUTORIAL_SPACELOGO_POSY, 0.0f), TUTORIAL_SPACELOGO_SIZEX, TUTORIAL_SPACELOGO_SIZEY, LOGOTYPE_SPACE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// ジャンプロゴ
			case LOGOTYPE_JUMP:
				SetUI(D3DXVECTOR3(TUTORIAL_JUMPLOGO_POSX, TUTORIAL_JUMPLOGO_POSY, 0.0f), TUTORIAL_JUMPLOGO_SIZEX, TUTORIAL_JUMPLOGO_SIZEY, LOGOTYPE_JUMP, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			}
		}

		// ゲームのとき
		if (CRenderer::GetMode() == CRenderer::MODE_GAME)
		{
			// ゲームで使うロゴのとき
			if (nCnt >= LOGOTYPE_NOW_TIME && nCnt <= LOGOTYPE_THREE || nCnt == LOGOTYPE_TIMEFRAME)
			{
				// 生成処理
				m_pScene2D[nCnt] = CScene2D::Create();

				// テクスチャを貼る
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}

			// ロゴによって切り替える
			switch (nCnt)
			{
				// タイマーフレーム
			case LOGOTYPE_TIMEFRAME:
				SetUI(D3DXVECTOR3(GAME_TIMEFREAMELOGO_POSX, GAME_TIMEFREAMELOGO_POSY, 0.0f), GAME_TIMEFREAMELOGO_SIZEX, GAME_TIMEFREAMELOGO_SIZEY, LOGOTYPE_TIMEFRAME, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 現在のタイム
			case LOGOTYPE_NOW_TIME:
				SetUI(D3DXVECTOR3(GAME_NOWTIMEFREAMELOGO_POSX, GAME_NOWTIMEFREAMELOGO_POSY, 0.0f), GAME_NOWTIMEFREAMELOGO_SIZEX, GAME_NOWTIMEFREAMELOGO_SIZEY, LOGOTYPE_NOW_TIME, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 最高のタイム
			case LOGOTYPE_BEST_TIME:
				SetUI(D3DXVECTOR3(GAME_BESTTIME_POSX, GAME_BESTTIME_POSY, 0.0f), GAME_BESTTIME_SIZEX, GAME_BESTTIME_SIZEY, LOGOTYPE_BEST_TIME, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// タイムとタイムの間の点
			case LOGOTYPE_DOT0:
				SetUI(D3DXVECTOR3(GAME_DOT0_POSX, GAME_DOT0_POSY, 0.0f), GAME_DOT0_SIZEX, GAME_DOT0_SIZEY, LOGOTYPE_DOT0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// タイムとタイムの間の点
			case LOGOTYPE_DOT1:
				SetUI(D3DXVECTOR3(GAME_DOT1_POSX, GAME_DOT1_POSY, 0.0f), GAME_DOT1_SIZEX, GAME_DOT1_SIZEY, LOGOTYPE_DOT1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// タイムとタイムの間の点
			case LOGOTYPE_DOT2:
				SetUI(D3DXVECTOR3(GAME_DOT2_POSX, GAME_DOT2_POSY, 0.0f), GAME_DOT2_SIZEX, GAME_DOT2_SIZEY, LOGOTYPE_DOT2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// タイムとタイムの間の点
			case LOGOTYPE_DOT3:
				SetUI(D3DXVECTOR3(GAME_DOT3_POSX, GAME_DOT3_POSY, 0.0f), GAME_DOT3_SIZEX, GAME_DOT3_SIZEY, LOGOTYPE_DOT3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// ポーズロゴ
			case LOGOTYPE_PAUSE:
				SetUI(D3DXVECTOR3(GAME_PAUSE_POSX, GAME_PAUSE_POSY, 0.0f), GAME_PAUSE_SIZEX, GAME_PAUSE_SIZEY, LOGOTYPE_PAUSE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			}
		}

		// ランキングのとき
		if (CRenderer::GetMode() == CRenderer::MODE_RANKING)
		{
			// ランキングで使うロゴのとき
			if (nCnt >= LOGOTYPE_RANK && nCnt <= LOGOTYPE_DOT9 || nCnt == LOGOTYPE_ENTER || nCnt >= LOGOTYPE_DOT0 && nCnt <= LOGOTYPE_DOT3)
			{
				// 生成処理
				m_pScene2D[nCnt] = CScene2D::Create();

				// テクスチャを貼る
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}

			// ロゴによって切り替える
			switch (nCnt)
			{
				// ランクのとき
			case LOGOTYPE_RANK:
				SetUI(D3DXVECTOR3(RANKING_RANK_POSX, RANKING_RANK_POSY, 0.0f), RANKING_RANK_SIZEX, RANKING_RANK_SIZEY, LOGOTYPE_RANK, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// ランキングロゴのとき
			case LOGOTYPE_RANKING_LOGO:
				SetUI(D3DXVECTOR3(RANKING_LOGO_POSX, RANKING_LOGO_POSY, 0.0f), RANKING_LOGO_SIZEX, RANKING_LOGO_SIZEY, LOGOTYPE_RANKING_LOGO, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// エンター
			case LOGOTYPE_ENTER:
				SetUI(D3DXVECTOR3(RANKING_ENTER_POSX, RANKING_ENTER_POSY, 0.0f), RANKING_ENTER_SIZEX, RANKING_ENTER_SIZEY, LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点0
			case LOGOTYPE_DOT0:
				SetUI(D3DXVECTOR3(RANKING_DOT0_POSX, RANKING_DOT0_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点1
			case LOGOTYPE_DOT1:
				SetUI(D3DXVECTOR3(RANKING_DOT1_POSX, RANKING_DOT1_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT1, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点2
			case LOGOTYPE_DOT2:
				SetUI(D3DXVECTOR3(RANKING_DOT2_POSX, RANKING_DOT2_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT2, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点3
			case LOGOTYPE_DOT3:
				SetUI(D3DXVECTOR3(RANKING_DOT3_POSX, RANKING_DOT3_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点4
			case LOGOTYPE_DOT4:
				SetUI(D3DXVECTOR3(RANKING_DOT4_POSX, RANKING_DOT4_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点5
			case LOGOTYPE_DOT5:
				SetUI(D3DXVECTOR3(RANKING_DOT5_POSX, RANKING_DOT5_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT5, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点6
			case LOGOTYPE_DOT6:
				SetUI(D3DXVECTOR3(RANKING_DOT6_POSX, RANKING_DOT6_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点7
			case LOGOTYPE_DOT7:
				SetUI(D3DXVECTOR3(RANKING_DOT7_POSX, RANKING_DOT7_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT7, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点8
			case LOGOTYPE_DOT8:
				SetUI(D3DXVECTOR3(RANKING_DOT8_POSX, RANKING_DOT8_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT8, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
				// 数字と数字の間の点9
			case LOGOTYPE_DOT9:
				SetUI(D3DXVECTOR3(RANKING_DOT9_POSX, RANKING_DOT9_POSY, 0.0f), RANKING_DOT_SIZE, RANKING_DOT_SIZE, LOGOTYPE_DOT9, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				break;
			}
		}
	}
}

//==================================================================================================================
// 終了処理
//==================================================================================================================
void CUI::Uninit(void)
{

}

//==================================================================================================================
// 更新処理
//==================================================================================================================
void CUI::Update(void)
{

}

//==================================================================================================================
// 描画処理
//==================================================================================================================
void CUI::Draw(void)
{

}

//==================================================================================================================
// 生成処理
//==================================================================================================================
CUI *CUI::Create(void)
{
	// シーン動的に確保
	CUI *pUI = new CUI(CScene::PRIORITY_UI);

	// シーン初期化
	pUI->Init();

	// 値を返す
	return pUI;
}

//==================================================================================================================
// テクスチャロード
//==================================================================================================================
HRESULT CUI::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// レンダラー情報取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// デバイスの取得

	//==============================テクスチャの読み込み==============================//
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
	}

	// 値を返す
	return S_OK;
}

//==================================================================================================================
// 破棄
//==================================================================================================================
void CUI::Unload(void)
{
	// ロゴの最大種類までカウント
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		m_pTexture[nCnt]->Release();		// 開放
		m_pTexture[nCnt] = NULL;			// NULLにする
	}
}

//==================================================================================================================
// 種類取得
//==================================================================================================================
CUI::UITYPE CUI::GetType(void)
{
	return m_type;
}

//==================================================================================================================
// ロゴ作成
//==================================================================================================================
void CUI::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);												// 位置設定
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));				// 大きさ設定
	m_pScene2D[nCnt]->SetCol(col);												// 色設定
}
