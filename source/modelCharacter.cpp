//=============================================================================
//
// モデルキャラクター処理  [ modelCharacter.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "modelcharacter.h"
#include "modelparts.h"
#include "manager.h"
#include "renderer.h"
#include "Inputkeyboard.h"
#include "debugproc.h"
#include "sceneX.h"
#include "motion.h"
#include "kananlibrary.h"
#include "motion.h"
#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
MODELCHARACTER	CModelCharacter::m_pModelCharacter[CHARACTER_MAX] = {};
char			CModelCharacter::m_aFileName[CHARACTER_MAX][64] =
{
	{ "data/OFFSET/offset_fokker.txt" },
};

//=============================================================================
// コンストラクタ
//=============================================================================
CModelCharacter::CModelCharacter()
{
	// 初期化
	m_pModelParts = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CModelCharacter::~CModelCharacter()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CModelCharacter::Init()
{
	// モデルを個数分生成
	m_pModelParts = new CModelParts[m_pModelCharacter[m_type].nNumParts];

	if (m_pModelParts)
	{
		for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
		{
			// 初期化
			m_pModelParts[nCnt].Init();
			// モデル情報設定
			m_pModelParts[nCnt].BindModelInfo(&m_pModelCharacter[m_type].pModelInfo[nCnt]);
		}
		// パーツのオフセット取得
		LoadOffset(m_type);
	}

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CModelCharacter::Uninit()
{
	// nullcheck
	if (m_pModelParts)
	{
		// 破棄
		delete[] m_pModelParts;
		m_pModelParts = nullptr;
	}
}

//=============================================================================
// 更新
//=============================================================================
void CModelCharacter::Update()
{
	// フレーム加算
	m_nFrame++;

	// フレームが一定値まで来た時
	if (m_nFrame >= CMotion::GetFrame(m_motion, m_nKey))
	{
		// フレーム初期化
		m_nFrame = 0;

		// キー加算
		m_nKey++;

		// キーが一定値まで来た時
		if (m_nKey >= CMotion::GetNumKey(m_motion))
		{
			//フレームとキー初期化
			ResetMotion();

			// ループしない時
			if (!CMotion::GetLoop(m_motion))
			{
				// ニュートラルに戻す
				m_motion = CMotion::PLAYER_NEUTRAL;
			}
		}

		// 次のモーション情報をセット
		SetMotion(m_motion);
	}

	// モデル数分繰り返す
	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		// nullcheck
		if (m_pModelParts)
			// 更新
			m_pModelParts[nCnt].Update();
	}

#ifdef _DEBUG
	// ImGuiの更新
	ShowModelCharaInfo();
#endif
}

//=============================================================================
// 描画	
//=============================================================================
void CModelCharacter::Draw()
{
	// モデル数分繰り返す
	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		// nullcheck
		if (m_pModelParts)
			// 描画
			m_pModelParts[nCnt].Draw();
	}
}

//=============================================================================
// 生成
//=============================================================================
CModelCharacter *CModelCharacter::Create(CHARACTER_TYPE modeltype)
{
	// メモリ確保
	CModelCharacter *pModelCharacter = new CModelCharacter;

	// 失敗
	if (!pModelCharacter)
		return nullptr;

	// タイプの設定
	pModelCharacter->m_type = modeltype;

	// 初期化
	pModelCharacter->Init();

	// 値を返す
	return pModelCharacter;
}

//=============================================================================
// ロード
//=============================================================================
HRESULT CModelCharacter::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// キャラクター数分回す
	for (int nCntModel = 0; nCntModel < CHARACTER_MAX; nCntModel++)
	{
		// モデル名取得
		LoadFileName((CHARACTER_TYPE)nCntModel);

		// ブロックコメント
		char cComment[MAX_TEXT];
		sprintf(cComment, "キャラクター %d のモデル読み込み開始", nCntModel);
		CKananLibrary::StartBlockComment(cComment);

		// テクスチャ数分回す
		for (int nCntTex = 0; nCntTex < m_pModelCharacter[nCntModel].nNumTexture; nCntTex++)
		{
			// パーツ数分回す
			for (int nCntParts = 0; nCntParts < m_pModelCharacter[nCntModel].nNumParts; nCntParts++)
			{
				// テクスチャ生成
				if (FAILED(D3DXCreateTextureFromFile(pDevice,
					&m_pModelCharacter[nCntModel].pModelInfo[nCntTex].cTextureName[0],
					&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].pTexture)))
				{
					// できなければ失敗
					printf("テクスチャ生成失敗 (テクスチャ %d)\n", nCntTex);
					m_pModelCharacter[nCntModel].pModelInfo[nCntParts].bTex = false;
				}
			}
		}

		// パーツ数分回す
		for (int nCntParts = 0; nCntParts < m_pModelCharacter[nCntModel].nNumParts; nCntParts++)
		{
			// モデル生成
			if (FAILED(D3DXLoadMeshFromX(&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].cModelName[0],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].matBuff,
				NULL,
				&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].matNum,
				&m_pModelCharacter[nCntModel].pModelInfo[nCntParts].mesh)))
			{
				// できなければ失敗
				printf("モデル生成失敗 (パーツ %d)\n", nCntParts);
			}
		}

		// ブロックコメント
		sprintf(cComment, "キャラクター %d のモデル読み込み終了", nCntModel);
		CKananLibrary::EndBlockComment(cComment);
	}

	// 成功
	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CModelCharacter::Unload(void)
{
	// キャラ数分繰り返す
	for (int nCntModel = 0; nCntModel < CHARACTER_MAX; nCntModel++)
	{
		// リリースを行う
		for (int nCnt = 0; nCnt < m_pModelCharacter[nCntModel].nNumParts; nCnt++)
			CKananLibrary::ReleaseModelInfo(&m_pModelCharacter[nCntModel].pModelInfo[nCnt]);

		// nullcheck
		if (m_pModelCharacter[nCntModel].pModelInfo)
		{
			// 破棄
			delete[] m_pModelCharacter[nCntModel].pModelInfo;
			m_pModelCharacter[nCntModel].pModelInfo = nullptr;
		}
	}
}

//=============================================================================
// キャラクターのマトリックス設定
//=============================================================================
void CModelCharacter::SetCharacterMtx(D3DXMATRIX *mtx)
{
	// モデル総数分
	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		if (m_pModelParts)
		{
			// 親がいない時
			if (m_pModelParts[nCnt].GetParent() == -1)
				// 大元のマトリックス設定
				m_pModelParts[nCnt].SetMtxParent(mtx);
			else
			{
				// 親パーツのマトリックス設定
				m_pModelParts[nCnt].SetMtxParent(m_pModelParts[m_pModelParts[nCnt].GetParent()].GetMtx());
			}
		}
	}
}

//=============================================================================
// モーションセット
//=============================================================================
void CModelCharacter::SetMotion(CMotion::MOTION_TYPE motiontype)
{
	m_motion = motiontype;

	// モデル数分繰り返す
	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		// nullcheck
		if (&m_pModelParts[nCnt])
			m_pModelParts[nCnt].SetMotionRotDest(motiontype, m_nKey);
	}
}

//=============================================================================
// オフセット読み込み
//=============================================================================
HRESULT CModelCharacter::LoadOffset(CHARACTER_TYPE type)
{
	// 変数宣言
	FILE *pFile;
	char cReadText[MAX_TEXT];
	char cHeadText[MAX_TEXT];
	char cDieText[MAX_TEXT];
	int nCntParts = 0;

	// 格納用
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nParent;

	// ファイルを開く
	pFile = fopen(&m_aFileName[type][0], "r");

	// nullcheck
	if (pFile)
	{
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
				// キャラクターセットが来たら
				else if (strcmp(cHeadText, "CHARACTERSET") == 0)
				{
					// エンドキャラクターセットがくるまで繰り返す
					while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);
						// 改行
						if (strcmp(cHeadText, "\n") == 0)
						{
						}
						// パーツセットが来たら
						else if (strcmp(cHeadText, "PARTSSET") == 0)
						{
							// エンドパーツセットがくるまで繰り返す
							while (strcmp(cHeadText, "END_PARTSSET") != 0)
							{
								fgets(cReadText, sizeof(cReadText), pFile);
								sscanf(cReadText, "%s", &cHeadText);

								int nBeWeapon = 0;
								float fRadius = 0.0f;
								//	ペアレント
								if (strcmp(cHeadText, "PARENT") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nParent);
								}
								//	武器かどうか
								if (strcmp(cHeadText, "WEAPON") == 0)
								{
									sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nBeWeapon);
								}
								//	当たり判定の半径
								if (strcmp(cHeadText, "RADIUS") == 0)
								{
									sscanf(cReadText, "%s %s %f", &cDieText, &cDieText, &fRadius);
								}
								// 位置
								if (strcmp(cHeadText, "POS") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &pos.x, &pos.y, &pos.z);
								}
								// 回転
								if (strcmp(cHeadText, "ROT") == 0)
								{
									sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, &rot.x, &rot.y, &rot.z);
								}
								if (strcmp(cHeadText, "END_PARTSSET") == 0)
								{
									if (m_pModelCharacter[type].pModelInfo[nCntParts].bTex)
									{
										// テクスチャ付きのパーツ情報格納
										m_pModelParts[nCntParts].SetPartsTexInfo(nCntParts, nParent, pos, rot, m_pModelCharacter[type].pModelInfo[nCntParts].pTexture);
									}
									else
									{
										// テクスチャなしのパーツ情報格納
										m_pModelParts[nCntParts].SetPartsInfo(nCntParts, nParent, pos, rot);
									}

									if (nBeWeapon == 1)
									{

									}

									// パーツ数加算
									nCntParts++;
								}
							}
						}
					}
				}
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}

	// ファイルを開けなかった時
	else
	{
		printf("オフセットファイルを開けませんでした。\n");
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
// モデル数・モデル名読み込み
//=============================================================================
HRESULT CModelCharacter::LoadFileName(CHARACTER_TYPE type)
{
	// 変数宣言
	FILE *pFile;
	char cReadText[MAX_TEXT];
	char cHeadText[MAX_TEXT];
	char cDieText[MAX_TEXT];
	int nNumModel = 0;
	int nNumTexture = 0;

	// ファイルを開く
	pFile = fopen(&m_aFileName[type][0], "r");

	CKananLibrary::StartBlockComment("モデルファイル読み込み開始");

	// nullcheck
	if (!pFile)
	{
		// ファイル読み込み失敗
		CKananLibrary::EndBlockComment("モデルファイルを開けませんでした");
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
			{
			}
			// モデル番号が来たら
			else if (strcmp(cHeadText, "NUM_MODEL") == 0)
			{
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nNumModel);
				m_pModelCharacter[type].pModelInfo = new MODELINFO[nNumModel];
				m_pModelCharacter[type].nNumParts = nNumModel;
				printf("読み込んだパーツ数 : %d\n", nNumModel);

				// 読み込んだモデル数
				int nCntModel = 0;
				// モデル数分繰り返す
				while (nCntModel < nNumModel)
				{
					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// ファイル名読み込み
					if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
					{
						// モデル名取得
						sscanf(cReadText, "%s %s %s", &cDieText, &cDieText, m_pModelCharacter[type].pModelInfo[nCntModel].cModelName);
						printf("ファイル %s を取得\n", m_pModelCharacter[type].pModelInfo[nCntModel].cModelName);
						m_pModelCharacter[type].pModelInfo[nCntModel].bTex = false;

						// モデルカウント加算
						nCntModel++;
					}
				}
			}
			// テクスチャ番号が来たら
			else if (strcmp(cHeadText, "NUM_TEXTURE") == 0)
			{
				sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nNumTexture);
				m_pModelCharacter[type].nNumTexture = nNumTexture;

				// 読み込んだテクスチャ数
				int nCntTexture = 0;
				// 初期化
				sprintf(m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName, "");

				// テクスチャ数分繰り返す
				while (nCntTexture < nNumTexture)
				{
					// 初期化
					m_pModelCharacter[type].pModelInfo[nCntTexture].bTex = false;
					sprintf(m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName, "");

					fgets(cReadText, sizeof(cReadText), pFile);
					sscanf(cReadText, "%s", &cHeadText);
					// ファイル名読み込み
					if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
					{
						// テクスチャ名取得
						sscanf(cReadText, "%s %s %s", &cDieText, &cDieText, m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName);
						printf("ファイル %s を取得\n", m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName);
						m_pModelCharacter[type].pModelInfo[nCntTexture].bTex = true;

						// テクスチャカウント加算
						nCntTexture++;
					}
				}
			}
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	CKananLibrary::EndBlockComment("モデルファイル読み込み終了");

	return S_OK;
}

//=============================================================================
// モーション情報リセット
//=============================================================================
void CModelCharacter::ResetMotion()
{
	m_nFrame = 0;
	m_nKey = 0;
}

#ifdef _DEBUG
//=============================================================================
// ImGuiの更新
//=============================================================================
void CModelCharacter::ShowModelCharaInfo(void)
{
	// 現在のフレーム数を表示
	/*ImGui::Text("nFrame : %d / %d", m_nFrame, CMotion::GetFrame(m_motion, m_nKey));
	// 現在のキー数を表示
	ImGui::Text("nKey : %d / %d", m_nKey, CMotion::GetNumKey(m_motion));*/
}
#endif