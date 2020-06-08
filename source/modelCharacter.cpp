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
	{ "data/OFFSET/offset_man.txt" },
	{ "data/OFFSET/offset_fish00.txt" },
	{ "data/OFFSET/offset_rob.txt" }
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

	for (int nCnt = 0; nCnt < m_pModelCharacter[m_type].nNumParts; nCnt++)
	{
		// 初期化
		m_pModelParts[nCnt].Init();

		// モデル情報設定
		m_pModelParts[nCnt].BindModel(m_pModelCharacter[m_type].pModelInfo[nCnt].matBuff, m_pModelCharacter[m_type].pModelInfo[nCnt].matNum, m_pModelCharacter[m_type].pModelInfo[nCnt].mesh);
	}
	// パーツのオフセット取得
	LoadOffset(m_type);

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
		if (&m_pModelParts[nCnt])
		{
			// 更新
			m_pModelParts[nCnt].Update();
		}
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
		if (&m_pModelParts[nCnt])
		{
			// 描画
			m_pModelParts[nCnt].Draw();
		}
	}
}

//=============================================================================
// 生成
//=============================================================================
CModelCharacter *CModelCharacter::Create(CHARACTER_TYPE modeltype)
{
	// メモリ確保
	CModelCharacter *pModelCharacter = new CModelCharacter;

	// タイプの設定
	pModelCharacter->m_type = modeltype;

	// 初期化
	pModelCharacter->Init();

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
					char cText[128];
					sprintf(cText, "テクスチャ生成失敗 (キャラクター %d, テクスチャ %d)", nCntModel, nCntTex);
					printf("%s\n", &cText[0]);
					return E_FAIL;
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
				char cText[128];
				sprintf(cText, "モデル生成失敗 (キャラクター %d, パーツ %d)", nCntModel, nCntParts);
				printf("%s\n", &cText[0]);
				return E_FAIL;
			}
		}
	}

	// 成功
	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CModelCharacter::Unload(void)
{
	for (int nCntModel = 0; nCntModel < CHARACTER_MAX; nCntModel++)
	{
		// リリースを行う
		for (int nCnt = 0; nCnt < m_pModelCharacter[nCntModel].nNumParts; nCnt++)
		{
			CKananLibrary::ReleaseModelInfo(&m_pModelCharacter[nCntModel].pModelInfo[nCnt]);
		}

		if (m_pModelCharacter[nCntModel].pModelInfo)
		{
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
		// 親がいない時
		if (m_pModelParts[nCnt].GetParent() == -1)
		{
			// 大元のマトリックス設定
			m_pModelParts[nCnt].SetParentMtx(mtx);
		}
		else
		{
			// 親パーツのマトリックス設定	
			m_pModelParts[nCnt].SetParentMtx(m_pModelParts[m_pModelParts[nCnt].GetParent()].GetMatrix());
		}
	}
}

//=============================================================================
// モーション情報取得
//=============================================================================
CMotion::MOTION_TYPE CModelCharacter::GetMotion(void)
{
	return m_motion;
}

//=============================================================================
// モデルパーツの取得
//=============================================================================
CModelParts *CModelCharacter::GetModelParts(void)
{
	return m_pModelParts;
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
		{
			m_pModelParts[nCnt].SetMotionRotDest(motiontype, m_nKey);
		}
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
									if (m_pModelCharacter[type].nNumTexture > 0)
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
	int nCntModel = 0;
	int nCntTexture = 0;

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

					// モデルがあれば
					if (nNumModel > 0)
					{
						// モデル数分繰り返す
						while (1)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);
							// ファイル名読み込み
							if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
							{
								// モデル名取得
								sscanf(cReadText, "%s %s %s %s %s", &cDieText, &cDieText, m_pModelCharacter[type].pModelInfo[nCntModel].cModelName, &cDieText, &cDieText);

								// モデルカウント加算
								nCntModel++;

								// モデルカウントがモデル数分になったら
								if (nCntModel == nNumModel)
								{
									// ループを抜ける
									break;
								}
							}
						}
					}
				}
				// テクスチャ番号が来たら
				else if (strcmp(cHeadText, "NUM_TEXTURE") == 0)
				{
					sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &nNumTexture);

					m_pModelCharacter[type].nNumTexture = nNumTexture;

					// テクスチャがあれば
					if (nNumTexture > 0)
					{
						// テクスチャ数分繰り返す
						while (1)
						{
							fgets(cReadText, sizeof(cReadText), pFile);
							sscanf(cReadText, "%s", &cHeadText);
							// ファイル名読み込み
							if (strcmp(cHeadText, "TEXTURE_FILENAME") == 0)
							{
								// テクスチャ名取得
								sscanf(cReadText, "%s %s %s %s %s", &cDieText, &cDieText, m_pModelCharacter[type].pModelInfo[nCntTexture].cTextureName, &cDieText, &cDieText);

								// テクスチャカウント加算
								nCntTexture++;

								// テクスチャカウントがテクスチャ数分になったら
								if (nCntTexture == nNumTexture)
								{
									// ループを抜ける
									break;
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

	// ファイルを開けなかったとき
	else
	{
		printf("モデルファイルを開けませんでした。\n");
		return E_FAIL;
	}

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

//=============================================================================
// キャラクターのモデル数を取得
//=============================================================================
int CModelCharacter::GetPartsNum(CHARACTER_TYPE type)
{
	return m_pModelCharacter[type].nNumParts;
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