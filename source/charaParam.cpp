//=============================================================================
//
// キャラクターパラメーター処理  [ charaParam.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "charaParam.h"

//=============================================================================
// マクロ定義
//=============================================================================	

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
CCharaParam::PLAYER_PARAM	CCharaParam::m_playerParam[PARAM_MAX] = {};

char						CCharaParam::m_aFileName[PARAM_MAX][64] =
{
	"data/PARAMETER/parameter_fokker.txt",
	"data/PARAMETER/parameter_niyasu.txt",
	"data/PARAMETER/parameter_sanyasu.txt",
	"data/PARAMETER/parameter_yonyasu.txt"
};

//=============================================================================
// コンストラクタ
//=============================================================================
CCharaParam::CCharaParam() {}
//=============================================================================
// デストラクタ
//=============================================================================
CCharaParam::~CCharaParam() {}

//=============================================================================
// キャラクターごとのパラメーターロード
//=============================================================================
HRESULT CCharaParam::Load(void)
{
	// 変数宣言
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";

	CKananLibrary::StartBlockComment("パラメーターファイルの読み込み開始");

	for (int type = 0; type < PARAM_MAX; type++)
	{
		// キャラ名を表示
		printf("キャラクター %d を読み込み (%s)\n", type, m_aFileName[type]);
		// ファイルを開く
		pFile = fopen(m_aFileName[type], "r");

		// nullcheck
		if (!pFile)
		{
			// 失敗
			printf("ファイルを開けませんでした\n");
			continue;
		}

		// スクリプトがくるまで繰り返す
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			// 一行ずつ読み込み
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
		}

		// エンドスクリプトが来るまで繰り返す
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// 一行ずつ読み込み
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);

			// 攻撃読み込みのカウンタ
			int nCntAttack = 0;

			// 改行
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// 最大HP
			else if (strcmp(cHeadText, "MAX_LIFE") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].fMaxLife);
			// 走る速度
			else if (strcmp(cHeadText, "RUN_SPEED") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].moveParam.fRunSpeed);
			// ジャンプ力
			else if (strcmp(cHeadText, "JUMP_POWER") == 0)
				sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &m_playerParam[type].moveParam.fJumpPower);
			// 攻撃
			else if (strcmp(cHeadText, "SET_ATTACKPARAM") == 0)
			{
				// エンドスクリプトが来るまで繰り返す
				while (strcmp(cHeadText, "END_ATTACKPARAM") != 0)
				{
					// 一行ずつ読み込み
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// 攻撃力
					if (strcmp(cHeadText, "ATTACK_POWER") == 0)
						sscanf(cReadText, "%s %s %f",
							&cDieText,
							&cDieText,
							&m_playerParam[type].attackParam[nCntAttack].fAttackPower);
					// 攻撃のキャンセルフレーム
					if (strcmp(cHeadText, "CANCEL_FRAME") == 0)
						sscanf(cReadText, "%s %s %d %s %d",
							&cDieText,
							&cDieText,
							&m_playerParam[type].attackParam[nCntAttack].CancelFrame.start,
							&cDieText,
							&m_playerParam[type].attackParam[nCntAttack].CancelFrame.end);
					// 吹き飛び方
					if (strcmp(cHeadText, "BLOWAWAY_TYPE") == 0)
						sscanf(cReadText, "%s %s %d",
							&cDieText,
							&cDieText,
							&m_playerParam[type].attackParam[nCntAttack].blowType);
				}
				// 攻撃の加算
				nCntAttack++;
			}
		}
		// ファイルを閉じる
		fclose(pFile);
	}

	// 成功
	CKananLibrary::EndBlockComment("パラメーターファイルの読み込み終了");
	return S_OK;
}

//=============================================================================
// データの破棄
//=============================================================================
void CCharaParam::Unload(void)
{
	// 種類数分繰り返す
	/*for (int type = 0; type < PARAM_MAX; type++)
	{
		// nullcheck
		if (m_pAttackParam[type])
		{
			// 破棄
			delete m_pAttackParam[type];
			m_pAttackParam[type] = nullptr;
		}
	}*/
}
