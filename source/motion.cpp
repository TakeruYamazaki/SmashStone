//=============================================================================
//
// モーション処理  [ motion.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "motion.h"
#include "renderer.h"
#include "manager.h"
#include "Inputkeyboard.h"
#include "debugproc.h"
#include "modelcharacter.h"
#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
CMotion::MOTION_INFO *CMotion::m_pMotionInfo = nullptr;
char * CMotion::m_apFileName[CMotion::MOTION_MAX] =
{
	{ "data/MOTION/fokker/motion_neutral.txt" },
	{ "data/MOTION/fokker/motion_run.txt" },
	//{ "data/MOTION/fokker/motion_jump.txt" },
	{ "data/MOTION/fokker/motion_lift.txt" },
	{ "data/MOTION/fokker/motion_throw.txt" },
	{ "data/MOTION/fokker/motion_attack_0.txt" },
	{ "data/MOTION/fokker/motion_attack_1.txt" },
	{ "data/MOTION/fokker/motion_attack_2.txt" },
	{ "data/MOTION/fokker/motion_attack_3.txt" },
	{ "data/MOTION/fokker/motion_smashCharge.txt" },
	{ "data/MOTION/fokker/motion_smash.txt" },
	{ "data/MOTION/fokker/motion_daunted.txt" },
	{ "data/MOTION/fokker/motion_blowAway.txt" }
};

//=============================================================================
// コンストラクタ
//=============================================================================
CMotion::CMotion()
{
	m_pMotionInfo = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
// モーションのロード
//=============================================================================
HRESULT CMotion::Load()
{
	// メモリ確保
	m_pMotionInfo = new CMotion::MOTION_INFO[MOTION_MAX];

	// ブロックコメント
	CKananLibrary::StartBlockComment("モーションファイルの読み込み開始");

	for (int nCnt = 0; nCnt < MOTION_MAX; nCnt++)
	{
		// モデル読み込み
		if (FAILED(LoadMotion((CMotion::MOTION_TYPE)nCnt)))
			// 失敗
			return E_FAIL;
	}

	// ブロックコメント
	CKananLibrary::EndBlockComment("モーションファイルの読み込み終了");

	// 成功
	return S_OK;
}

//=============================================================================
// モーションのアンロード
//=============================================================================
void CMotion::UnLoad()
{
	// モーション数回す
	for (int nCntMotion = 0; nCntMotion < MOTION_MAX; nCntMotion++)
	{
		// キー数回す
		for (int nCntKeyInfo = 0; nCntKeyInfo < m_pMotionInfo[nCntMotion].nNumKey; nCntKeyInfo++)
		{
			// nullcheck
			if (m_pMotionInfo[nCntMotion].pKeyInfo[nCntKeyInfo].pKey)
			{
				// 現在のキーのメモリ開放
				delete[] m_pMotionInfo[nCntMotion].pKeyInfo[nCntKeyInfo].pKey;
				m_pMotionInfo[nCntMotion].pKeyInfo[nCntKeyInfo].pKey = nullptr;
			}
		}
		// nullcheck
		if (m_pMotionInfo[nCntMotion].pKeyInfo)
		{
			// 現在のキーインフォのメモリ開放
			delete[] m_pMotionInfo[nCntMotion].pKeyInfo;
			m_pMotionInfo[nCntMotion].pKeyInfo = nullptr;
		}
	}

	// nullcheck
	if (m_pMotionInfo)
	{
		// メモリ開放
		delete[] m_pMotionInfo;
		m_pMotionInfo = nullptr;

	}
}

//=============================================================================
// テキストファイルからモーションロード
//=============================================================================
HRESULT CMotion::LoadMotion(MOTION_TYPE motiontype)
{
	// 変数宣言
	FILE *pFile;
	char cReadText[MAX_TEXT] = {};
	char cHeadText[MAX_TEXT] = {};
	char cDieText[MAX_TEXT] = {};
	int nLoop = 0;
	int nCntkeyInfo = 0;
	int nCntKey = 0;
	bool bInfo = false;	// なぜかキー数が二回入るので、この変数で一回にする

	// ファイルを開く
	pFile = fopen(m_apFileName[motiontype], "r");

	// nullcheck
	if (!pFile)
	{
		// ファイル読み込み失敗
		std::cout << m_apFileName[motiontype] << " の読み込み失敗" << std::endl;
		return E_FAIL;
	}

	// スクリプトがくるまで繰り返す
	while (strcmp(cHeadText, "SCRIPT") != 0)
	{
		fgets(cReadText, sizeof(cReadText), pFile);
		sscanf(cReadText, "%s", &cHeadText);
	}
	// スクリプトが来たら
	if (strcmp(cHeadText, "SCRIPT") == 0)
	{
		// エンドスクリプトがくるまで繰り返す
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
			// 改行
			if (strcmp(cHeadText, "\n") == 0)
			{
			}
			// モーションセット
			else if (strcmp(cHeadText, "MOTIONSET") == 0)
			{
				// エンドモーションセットがくるまで繰り返す
				while (strcmp(cHeadText, "END_MOTIONSET") != 0)
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// 改行
					if (strcmp(cHeadText, "\n") == 0)
					{
					}
					// ループするか
					else if (strcmp(cHeadText, "LOOP") == 0)
					{
						// 一度int型で格納する
						sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nLoop);

						// 1か0でboolとして判断する
						m_pMotionInfo[motiontype].bLoop = nLoop ? true : false;
					}
					// キー数
					else if (strcmp(cHeadText, "NUM_KEY") == 0 && !bInfo)
					{
						sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfo[motiontype].nNumKey);

						// キー数分メモリ確保
						m_pMotionInfo[motiontype].pKeyInfo = new KEY_INFO[m_pMotionInfo[motiontype].nNumKey];

						// キーセットを行うので、キー情報カウントを初期化する
						nCntkeyInfo = 0;

						bInfo = true;
					}
					// キーセット
					else if (strcmp(cHeadText, "KEYSET") == 0)
					{
						// パーツ数分のキーを宣言
						int nKey = CModelCharacter::GetPartsNum((CHARACTER_TYPE)CheckCharacter(motiontype));

						// キー数分メモリ確保
						m_pMotionInfo[motiontype].pKeyInfo[nCntkeyInfo].pKey = new KEY[nKey];

						// キーカウントを初期化する
						nCntKey = 0;

						// エンドキーセットがくるまで繰り返す
						while (strcmp(cHeadText, "END_KEYSET") != 0)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);
							// フレーム数
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_pMotionInfo[motiontype].pKeyInfo[nCntkeyInfo].nFrame);
							}
							// キー
							if (strcmp(cHeadText, "KEY") == 0)
							{
								// エンドキーがくるまで繰り返す
								while (strcmp(cHeadText, "END_KEY") != 0)
								{
									fgets(cReadText, sizeof(cReadText), pFile);
									sscanf(cReadText, "%s", &cHeadText);
									// 位置
									if (strcmp(cHeadText, "POS") == 0)
									{
										sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &m_pMotionInfo[motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].posDest.x,
											&m_pMotionInfo[motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].posDest.y, &m_pMotionInfo[motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].posDest.z);
									}
									// 回転
									if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &m_pMotionInfo[motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].rotDest.x,
											&m_pMotionInfo[motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].rotDest.y, &m_pMotionInfo[motiontype].pKeyInfo[nCntkeyInfo].pKey[nCntKey].rotDest.z);
									}
									// 終了
									if (strcmp(cHeadText, "END_KEY") == 0)
									{
										// キー加算
										nCntKey++;
									}
								}
							}
							// 終了
							if (strcmp(cHeadText, "END_KEYSET") == 0)
							{
								// キー情報加算
								nCntkeyInfo++;
								bInfo = false;
							}
						}
					}
				}
			}
		}
	}
	// ファイルを閉じる
	fclose(pFile);

	// デバッグコメント表示
	std::cout << m_apFileName[motiontype] << " の読み込み完了" << std::endl;

	return S_OK;
}

//=============================================================================
// キャラクターがどれか確認
//=============================================================================
int CMotion::CheckCharacter(MOTION_TYPE type)
{
	// モデルの種類
	CHARACTER_TYPE CharacterType = CHARACTER_NONE;

	// モーションの種類で判断
	/*switch (type)
	{
		// プレイヤー
	case CMotion::PLAYER_NEUTRAL:
	case CMotion::PLAYER_SWIM:
	case CMotion::PLAYER_SWIM_BEGIN:
		CharacterType = CHARACTER_PLAYER;
		break;
	case CMotion::FISH000_NEUTRAL:
		CharacterType = CHARACTER_FISH;
		break;
		case CMotion::FISH001_NEUTRAL:
		CharacterType = CHARACTER_FISH2;
		break;
	}*/

	return CharacterType;
}
