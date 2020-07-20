//=============================================================================
//
// キャラクターパラメーター処理  [ characterParameter.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// インクルード
//=============================================================================
#include "characterParameter.h"

//=============================================================================
// マクロ定義
//=============================================================================	

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
char CCharacterParameter::m_aFileName[PARAMETER_MAX][64] =
{
	"data/PARAMETER/parameter_fokker.txt",
	"data/PARAMETER/parameter_niyasu.txt"
};

//=============================================================================
// コンストラクタ
//=============================================================================
CCharacterParameter::CCharacterParameter() {}
//=============================================================================
// デストラクタ
//=============================================================================
CCharacterParameter::~CCharacterParameter() {}
//=============================================================================
// 初期化
//=============================================================================
void CCharacterParameter::Init(void)
{

}

//=============================================================================
// 終了
//=============================================================================
void CCharacterParameter::Uninit(void)
{

}

//=============================================================================
// キャラクターごとのパラメーターロード
//=============================================================================
HRESULT CCharacterParameter::Load(PARAMETER_TYPE type)
{
	// 変数宣言
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";

	// ファイルを開く
	pFile = fopen(m_aFileName[type], "r");

	CKananLibrary::StartBlockComment("パラメーターファイルの読み込み開始");

	if (!pFile)
	{
		// ブロックコメント
		CKananLibrary::EndBlockComment("パラメーターファイルを開けませんでした");
		// 失敗
		return E_FAIL;
	}

	return S_OK;
}

/*HRESULT CObjectManager::LoadFileName(void)
{
	// 変数宣言
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";
	int nNumType = 0;
	int nNumObj = 0;

	// ファイルを開く
	pFile = fopen(&m_aFileName[0], "r");

	CKananLibrary::StartBlockComment("オブジェクトファイルの読み込み開始。");

	// nullcheck
	if (!pFile)
	{
		// ブロックコメント
		CKananLibrary::EndBlockComment("オブジェクトファイルを開けませんでした。");
		// 失敗
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
		// エンドスクリプトが来るまで繰り返す
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			fgets(cReadText, sizeof(cReadText), pFile);
			sscanf(cReadText, "%s", &cHeadText);
			// 改行
			if (strcmp(cHeadText, "\n") == 0)
				continue;
			// モデル番号が来たら
			else if (strcmp(cHeadText, "NUM_TYPE") == 0)
			{
				// モデルタイプ数読み込み
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nNumType);
				printf("読み込んだオブジェクトタイプ数 %d\n", nNumType);

				OBJINFO objInfo;
				int nCntModel = 0;
				while (nCntModel != nNumType)
				{
					// 一行読み込み
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);

					// ファイル名読み込み
					if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{
						// モデル名取得
						if (SUCCEEDED(sscanf(cReadText, "%s %s %s",
							&cDieText,
							&cDieText,
							&objInfo.modelInfo.cModelName[0])))
						{
							// 読み込んだファイル名を表示
							printf("モデル %s を読み込み\n", &objInfo.modelInfo.cModelName[0]);
							// テクスチャ無し
							objInfo.modelInfo.bTex = false;
							// オブジェクト情報を保存
							m_objInfo.push_back(objInfo);
							// モデル数を加算
							nCntModel++;
						}
					}
				}
			}
			// テクスチャ番号が来たら
			else if (strcmp(cHeadText, "NUM_TEXTURE") == 0)
			{
				// テクスチャ数読み込み
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_nNumTexture);
				printf("読み込んだテクスチャ数 %d\n", m_nNumTexture);

				if (m_nNumTexture > 0)
					m_pModelIndex = new int[m_nNumTexture];	// テクスチャを割り当てるモデル番号

				int nCntTex = 0;
				// 読み込んだテクスチャ数が設定数になるまで繰り返す
				while (nCntTex != m_nNumTexture)
				{
					// 一行読み込み
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);

					// ファイル名読み込み
					if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
					{
						int nModel = 0;
						char cFileName[MAX_TEXT];
						// モデル番号取得
						if (SUCCEEDED(sscanf(cReadText, "%s %s %s : %s %s %d",
							&cDieText,		// TEX_FILE
							&cDieText,		// =
							&cFileName[0],	// テクスチャ名
							&cDieText,		// MODEL
							&cDieText,		// =
							&nModel)))		// モデル番号
						{
							// テクスチャ名を保存
							strcpy(m_objInfo[nModel].modelInfo.cTextureName, cFileName);
							// モデル番号を保存
							m_pModelIndex[nCntTex] = nModel;
							// 読み込んだファイル名を表示
							printf("テクスチャ %s を読み込み\n", &m_objInfo[nModel].modelInfo.cTextureName[0]);
							// テクスチャ数を加算
							nCntTex++;
						}
					}
				}
			}
			// キャラクターセットが来たら
			else if (strcmp(cHeadText, "NUM_OBJECT") == 0)
				// モデルタイプ数の取得
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nNumObj);
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	// オブジェクト総数を表示
	printf("読み込んだオブジェクト総数 %d\n", nNumObj);
	// モデル数分繰り返す
	for (int nCntModel = 0; nCntModel < nNumObj; nCntModel++)
		// 総数分のメモリ確保
		m_pObject.push_back(CObject::Create());

	CKananLibrary::EndBlockComment("オブジェクトファイルの読み込み終了成功");

	// 成功
	return S_OK;
}*/