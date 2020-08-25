//=============================================================================
//
// オブジェクトマネージャー処理  [ objManager.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "objManager.h"
#include "manager.h"
#include "renderer.h"
#include "Inputkeyboard.h"
#include "debugproc.h"
#include "mouse.h"
#include "camera.h"
#include "game.h"
#include <iostream>
#include "ImGui/imgui.h"				// Imguiの実装に必要
#include "ImGui/imgui_impl_dx9.h"		// Imguiの実装に必要
#include "ImGui/imgui_impl_win32.h"		// Imguiの実装に必要

//=============================================================================
// マクロ定義
//=============================================================================
#define MODEL_PASS_CUT	(25)	// モデルのパスを切り取る数

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================
char						CObjectManager::m_cModelFile[MAX_TEXT] =
{
	"data/OBJECT/object_model.txt",
};

char						CObjectManager::m_aFileName[STAGE_MAX][MAX_TEXT] = 
{ 
	"data/OBJECT/offset_stage1.txt",
	"data/OBJECT/offset_stage2.txt",
};

CObjectManager::STAGETYPE	CObjectManager::m_stageType				= STAGE_1;
CObjectManager::STAGEINFO	CObjectManager::m_stageInfo[STAGE_MAX]	= {};
std::vector<OBJINFO>		CObjectManager::m_objInfo				= {};
int							CObjectManager::m_nNumTexture			= 0;
int							*CObjectManager::m_pModelIndex			= {};

#ifdef _DEBUG
int							CObjectManager::m_nFakeType = 0;
int							CObjectManager::m_stateMode = CObjectManager::MODE_GAME;
bool						CObjectManager::m_bObjUse = false;
bool						CObjectManager::m_bShowAnother = false;
std::string					*CObjectManager::m_pObjName = {};
#endif

//=============================================================================
// コンストラクタ
//=============================================================================
CObjectManager::CObjectManager(PRIORITY type = CScene::PRIORITY_OBJECT) : CScene(type)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectManager::~CObjectManager()
{

}

//=============================================================================
// 初期化
//=============================================================================
void CObjectManager::Init()
{

}

//=============================================================================
// 終了
//=============================================================================
void CObjectManager::Uninit()
{
#ifdef _DEBUG
	if (m_pFakeObject)
	{
		m_pFakeObject->Uninit();
		delete m_pFakeObject;
		m_pFakeObject = nullptr;
	}

#endif

	// サイズ分繰り返す (size_t = unsigned, vector.size()でメモリ数を返す)
	for (size_t nCnt = 0; nCnt < m_stageInfo[m_stageType].pObject.size(); nCnt++)
	{
		// メモリ削除
		delete m_stageInfo[m_stageType].pObject[nCnt];
	}
	// 中身をなくす
	m_stageInfo[m_stageType].pObject.clear();
}

//=============================================================================
// 更新
//=============================================================================
void CObjectManager::Update()
{
	// モデル数分繰り返す
	for (size_t nCnt = 0; nCnt < m_stageInfo[m_stageType].pObject.size(); nCnt++)
	{
		// nullcheck
		if (m_stageInfo[m_stageType].pObject[nCnt])
			m_stageInfo[m_stageType].pObject[nCnt]->Update();
	}

#ifdef _DEBUG
	if (m_pFakeObject)
	{
		m_pFakeObject->Update();
	}
	// ImGuiの更新
	ShowObjectManagerInfo();
#endif
}

//=============================================================================
// 描画	
//=============================================================================
void CObjectManager::Draw()
{
	// モデル数分繰り返す
	for (size_t nCnt = 0; nCnt < m_stageInfo[m_stageType].pObject.size(); nCnt++)
	{
		// nullcheck
		if (m_stageInfo[m_stageType].pObject[nCnt])
			m_stageInfo[m_stageType].pObject[nCnt]->Draw();
	}

#ifdef _DEBUG
	if (m_pFakeObject)
		m_pFakeObject->DrawAlpha();
#endif
}

//=============================================================================
// 生成
//=============================================================================
CObjectManager *CObjectManager::Create(STAGETYPE type)
{
	// メモリ確保
	CObjectManager *pMana = new CObjectManager();

	// nullcheck
	if (!pMana)
	{
		// 失敗
		std::cout << "オブジェクトマネージャー生成失敗" << std::endl;
		return nullptr;
	}

	// 初期化
	pMana->Init();
	// タイプを保存
	pMana->m_stageType = type;
	// ステージ配置
	pMana->SetOffset();

	// 成功
	return pMana;
}

//=============================================================================
// ロード
//=============================================================================
HRESULT CObjectManager::Load()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// モデル名取得
	LoadModel();

	// オフセット読み込み
	LoadOffset();

	// テクスチャ生成
	CreateTexture();

	// オブジェクト情報生成
	CreateObjInfo();

	// 成功
	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CObjectManager::Unload(void)
{
	// nullcheck
	if (m_pModelIndex)
	{
		// 破棄
		delete m_pModelIndex;
		m_pModelIndex = nullptr;
	}

	for (int nCnt = 0; nCnt < (int)m_objInfo.size(); nCnt++)
	{
		m_pObjName[nCnt].erase(m_pObjName[nCnt].begin(), m_pObjName[nCnt].end());
		m_pObjName[nCnt].clear();
	}

	if (m_pObjName)
	{
		delete[] m_pObjName;
		m_pObjName = nullptr;
	}

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		// nullcheck
		if (m_stageInfo[nCnt].objOffset)
		{
			// 破棄
			delete[] m_stageInfo[nCnt].objOffset;
			m_stageInfo[nCnt].objOffset = nullptr;
		}
	}
}

//=============================================================================
// モデル配置
//=============================================================================
HRESULT CObjectManager::SetOffset(void)
{
	for (int nCnt = 0; nCnt < m_stageInfo[m_stageType].nNumObject; nCnt++)
	{
		m_stageInfo[m_stageType].pObject.push_back(CObject::Create());
	}

	for (int nCnt = 0; nCnt < (int)m_stageInfo[m_stageType].pObject.size(); nCnt++)
	{
		// オブジェクト情報の設定
		m_stageInfo[m_stageType].pObject[nCnt]->SetObjInfo(m_stageInfo[m_stageType].objOffset[nCnt].pos,
			m_stageInfo[m_stageType].objOffset[nCnt].rot,
			m_stageInfo[m_stageType].objOffset[nCnt].scale,
			&m_objInfo[m_stageInfo[m_stageType].objOffset[nCnt].nType].modelInfo,
			m_stageInfo[m_stageType].objOffset[nCnt].nType);
	}

	return S_OK;
}

//=============================================================================
// モデル読み込み
//=============================================================================
HRESULT CObjectManager::LoadModel(void)
{
	// 変数宣言
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";
	int nNumType = 0;

	// ファイルを開く
	pFile = fopen(&m_cModelFile[0], "r");

	CKananLibrary::StartBlockComment("オブジェクトファイルの読み込み開始。");

	// nullcheck
	if (!pFile)
	{
		// ブロックコメント
		CKananLibrary::EndBlockComment("ファイルを開けませんでした。");
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
							m_objInfo[nModel].modelInfo.bTex = true;
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
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	CKananLibrary::EndBlockComment("ファイルの読み込み終了成功");

	// 成功
	return S_OK;
}

//=============================================================================
// オブジェクトファイル読み込み
//=============================================================================
HRESULT CObjectManager::LoadOffset(void)
{
	// 変数宣言
	FILE *pFile = nullptr;
	char cReadText[MAX_TEXT] = "";
	char cHeadText[MAX_TEXT] = "";
	char cDieText[MAX_TEXT] = "";
	int nNumType = 0;

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		// 読み込んだモデル数格納用
		int nModel = 0;

		// ファイルを開く
		pFile = fopen(&m_aFileName[nCnt][0], "r");

		CKananLibrary::StartBlockComment("ステージファイルの読み込み開始。");

		// nullcheck
		if (!pFile)
		{
			// ブロックコメント
			CKananLibrary::EndBlockComment("ファイルを開けませんでした。");
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
				// オブジェクト数
				if (strcmp(cHeadText, "NUM_OBJECT") == 0)
				{
					// モデル数の取得
					sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_stageInfo[nCnt].nNumObject);
					// オフセットのメモリ確保
					if (!m_stageInfo[nCnt].objOffset)
						m_stageInfo[nCnt].objOffset = new OBJECT_OFFSET[m_stageInfo[nCnt].nNumObject];
				}
				// セットが来たら
				else if (strcmp(cHeadText, "OBJECTSET") == 0)
				{
					// エンドセットがくるまで繰り返す
					while (strcmp(cHeadText, "END_OBJECTSET") != 0)
					{
						fgets(cReadText, sizeof(cReadText), pFile);
						sscanf(cReadText, "%s", &cHeadText);
						// 種類
						if (strcmp(cHeadText, "TYPE") == 0)
							sscanf(cReadText, "%s %s %d", &cDieText, &cDieText, &m_stageInfo[nCnt].objOffset[nModel].nType);
						// 位置
						if (strcmp(cHeadText, "POS") == 0)
							sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, 
								&m_stageInfo[nCnt].objOffset[nModel].pos.x, 
								&m_stageInfo[nCnt].objOffset[nModel].pos.y,
								&m_stageInfo[nCnt].objOffset[nModel].pos.z);
						// 回転
						if (strcmp(cHeadText, "ROT") == 0)
							sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText, 
								&m_stageInfo[nCnt].objOffset[nModel].rot.x,
								&m_stageInfo[nCnt].objOffset[nModel].rot.y,
								&m_stageInfo[nCnt].objOffset[nModel].rot.z);
						// 拡大率
						if (strcmp(cHeadText, "SCALE") == 0)
							sscanf(cReadText, "%s %s %f %f %f", &cDieText, &cDieText,
								&m_stageInfo[nCnt].objOffset[nModel].scale.x,
								&m_stageInfo[nCnt].objOffset[nModel].scale.y,
								&m_stageInfo[nCnt].objOffset[nModel].scale.z);
					}

					// モデル数を加算
					nModel++;
				}
			}
		}

		// ファイルを閉じる
		fclose(pFile);

		// オブジェクト総数を表示
		printf("読み込んだオブジェクト総数 %d\n", m_stageInfo[nCnt].nNumObject);

		CKananLibrary::EndBlockComment("ファイルの読み込み終了成功");

	}
	// 成功
	return S_OK;
}

#ifdef _DEBUG
//=============================================================================
// ImGuiの更新
//=============================================================================
void CObjectManager::ShowObjectManagerInfo(void)
{
	// ウィンドウを表示するか
	ImGui::Checkbox("show object window", &m_bShowAnother);

	// 表示する
	if (!m_bShowAnother)
	{
		return;
	}

	// ImGuiの更新開始
	ImGui::Begin("object window", &m_bShowAnother);

	ImGui::Text("stage %d", m_stageType);

	// カメラの取得
	CCamera *pCamera = CManager::GetRenderer()->GetGame()->GetCamera();

	// セーブボタン
	if (ImGui::Button("save object offset"))
	{
		// セーブしたら
		if (SaveOffset() == S_OK)
		{
			// マウスの取得
			CMouse *pMouse = CManager::GetMouse();
			// ウィンドウハンドルの取得
			HWND hWnd = pMouse->GetWnd();
			// 確認のメッセージ
			MessageBox(hWnd, "オブジェクトファイルをセーブしました", "確認", MB_OK);
		}
	}

	// ゲームモードに切り替え
	if (ImGui::RadioButton("GAME", &m_stateMode, MODE_GAME))
	{
		if (m_bObjUse)
		{
			delete m_pFakeObject;
			m_pFakeObject = nullptr;
			m_bObjUse = false;
		}
	}
	// 改行しない
	ImGui::SameLine();
	// デバッグモードに切り替え
	if (ImGui::RadioButton("DEBUG", &m_stateMode, MODE_DEBUG))
	{
		// 偽オブジェを使っていない
		if (!m_bObjUse)
		{
			// 生成して使用
			m_pFakeObject = new CObject();
			m_bObjUse = true;
		}

		// 偽オブジェが存在する
		if (m_pFakeObject)
		{
			// デフォルトタイプとして初期化
			m_pFakeObject->Init();

			// 初期モデルを設定
			m_pFakeObject->SetObjInfo(*pCamera->GetPosR(), 
				ZeroVector3, 
				OneVector3, 
				&m_objInfo[0].modelInfo, 
				0);
		}
	}

	if (m_stateMode == MODE_GAME)
	{
		// モードの詳細を表示
		ImGui::Text("This mode displays the details of the object.");
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
	}
	else if (m_stateMode == MODE_DEBUG)
	{
		// モードの詳細を表示
		ImGui::Text("This mode allows you to place objects.");

		m_pFakeObject->ShowObjectInfo("FakeObject");

		// オブジェタイプが変わったら
		if (ImGui::InputInt("ObjectType", &m_nFakeType))
		{
			// タイプが超えないよう
			if (m_nFakeType < 0)
				m_nFakeType = 0;
			else if (m_nFakeType >= (int)m_objInfo.size())
				m_nFakeType = (int)m_objInfo.size() - 1;

			// モデル情報格納
			m_pFakeObject->SetObjInfo(*m_pFakeObject->GetPos(), 
				*m_pFakeObject->GetRot(), 
				*m_pFakeObject->GetScale(), 
				&m_objInfo[m_nFakeType].modelInfo,
				m_nFakeType);
		}

		// オブジェクトの生成
		if (ImGui::Button("Create"))
		{
			// メモリ確保
			m_stageInfo[m_stageType].pObject.push_back(CObject::Create());
			// 初期化・情報設定
			m_stageInfo[m_stageType].pObject[(int)m_stageInfo[m_stageType].pObject.size() - 1]->Init();
			// モデル情報格納
			m_stageInfo[m_stageType].pObject[(int)(m_stageInfo[m_stageType].pObject.size() - 1)]->SetObjInfo(*m_pFakeObject->GetPos(),
				*m_pFakeObject->GetRot(), 
				*m_pFakeObject->GetScale(), 
				&m_objInfo[m_nFakeType].modelInfo,
				m_nFakeType);
		}
	}

	// モデル数分繰り返す
	for (int nCnt = (int)m_stageInfo[m_stageType].pObject.size() - 1; nCnt > -1; nCnt--)
	{
		// nullcheck
		if (m_stageInfo[m_stageType].pObject[nCnt])
		{
			// パスをcharにキャスト
			char *cObjName = new char[m_pObjName[m_stageInfo[m_stageType].pObject[nCnt]->GetType()].size() + 1];
			// 文字列を複製
			strcpy(cObjName, m_pObjName[m_stageInfo[m_stageType].pObject[nCnt]->GetType()].c_str());
			// ツリー名の設定
			char cText[32] = {};
			sprintf(cText, "Object : %d (%s)", nCnt, cObjName);
			// ImGuiの更新
			m_stageInfo[m_stageType].pObject[nCnt]->ShowObjectInfo(cText);
			// メモリを破棄
			delete cObjName;
		}

		// リリースが有効
		if (m_stageInfo[m_stageType].pObject[nCnt]->GetRelease())
		{
			m_stageInfo[m_stageType].pObject[nCnt]->Uninit();
			delete m_stageInfo[m_stageType].pObject[nCnt];
			// 指定した番号のオブジェクトを削除 : vector.erase( vectorの始まり + 指定した番号 )
			m_stageInfo[m_stageType].pObject.erase(m_stageInfo[m_stageType].pObject.begin() + nCnt);
		}
	}

	// 閉じるボタン
	if (ImGui::Button("close this window"))
		m_bShowAnother = false;

	// ImGuiの更新終了
	ImGui::End();
}

//=============================================================================
// テクスチャ生成
//=============================================================================
void CObjectManager::CreateTexture()
{
	// テクスチャ数分回す
	for (int nCnt = 0; nCnt < m_nNumTexture; nCnt++)
	{
		// テクスチャを生成
		CKananLibrary::CreateTexture(&m_objInfo[m_pModelIndex[nCnt]].modelInfo.pTexture,
										m_objInfo[m_pModelIndex[nCnt]].modelInfo.cTextureName);
	}
}

//=============================================================================
// オブジェクト情報生成
//=============================================================================
void CObjectManager::CreateObjInfo()
{
#ifdef _DEBUG
	// オブジェクト名のメモリ確保
	m_pObjName = new std::string[(int)m_objInfo.size()];
#endif

	// 種類数分回す
	for (size_t nCnt = 0; nCnt < m_objInfo.size(); nCnt++)
	{
		// モデル情報の生成
		CKananLibrary::CreateModelInfo(&m_objInfo[nCnt].modelInfo);

		// モデルの頂点座標の最大・最小を求める
		m_objInfo[nCnt].modelVtx =
		CKananLibrary::OutputModelVtxColl(m_objInfo[nCnt].modelInfo.mesh);

#ifdef _DEBUG
		// モデルのパスを取得
		std::string modelName = m_objInfo[nCnt].modelInfo.cModelName;
		// モデル名のみ保存
		m_pObjName[nCnt] = modelName.substr(MODEL_PASS_CUT);
#endif
	}
}

//=============================================================================
// オブジェクト情報のセーブ
//=============================================================================
HRESULT CObjectManager::SaveOffset(void)
{
	// 変数宣言
	FILE *pFile;

	// テキストファイルを見やすくするコメント
	char cEqual[2] = "=";

	// 変数名用と書き込み用の変数
	char cHeadText[MAX_TEXT];
	char cWriteText[MAX_TEXT];

	// 位置・回転の取得用
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;

	// ファイルを開く
	pFile = fopen(&m_aFileName[m_stageType][0], "w");

	// nullcheck
	if (pFile)
	{
		fputs(COMMENT_BLOCK, pFile);														// #=====================================================
		fputs(COMMENT_BLOCK_LINE, pFile);													// #

		strcpy(cWriteText, "# オブジェクトのオフセット\n");
		fputs(cWriteText, pFile);															// # オブジェクトのオフセット

		strcpy(cWriteText, COMMENT_AUTHOR);
		fputs(cWriteText, pFile);															// # Author : KANAN NAGANAWA
		fputs(COMMENT_BLOCK_LINE, pFile);													// #
		fputs(COMMENT_BLOCK, pFile);														// #=====================================================

		strcpy(cHeadText, "SCRIPT");
		fputs(cHeadText, pFile);															// SCRIPT
		fputs(COMMENT_NEW_LINE, pFile);														// \n
		fputs(COMMENT_NEW_LINE, pFile);														// \n

		// スクリプトが来たら続ける
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			strcpy(cWriteText, "# オブジェクト総数\n");
			fputs(cWriteText, pFile);														// # オブジェクト総数
			fputs(COMMENT_BLOCK, pFile);													// #=====================================================

			strcpy(cHeadText, "NUM_OBJECT");
			sprintf(cWriteText, "%s %s %d\n",
				&cHeadText,
				&cEqual,
				(int)m_stageInfo[m_stageType].pObject.size());
			fputs(cWriteText, pFile);														// NUM_OBJECT = m_nNumObject
			fputs(COMMENT_NEW_LINE, pFile);													// \n
			fputs(COMMENT_BLOCK, pFile);													// #=====================================================

			strcpy(cWriteText, "# オブジェクト情報\n");
			fputs(cWriteText, pFile);														// # オブジェクト情報
			fputs(COMMENT_BLOCK, pFile);													// #=====================================================

			for (size_t nCnt = 0; nCnt < m_stageInfo[m_stageType].pObject.size(); nCnt++)
			{
				strcpy(cWriteText, "OBJECTSET\n");
				fputs(cWriteText, pFile);													// OBJECTSET

				strcpy(cHeadText, "TYPE");
				sprintf(cWriteText, "	%s %s %d",
					&cHeadText,
					&cEqual,
					(int)m_stageInfo[m_stageType].pObject[nCnt]->GetType());
				fputs(cWriteText, pFile);													//	TYPE = GetType()
				fputs(COMMENT_NEW_LINE, pFile);												//	\n

				strcpy(cHeadText, "POS");
				sprintf(cWriteText, "	%s %s %.3f %.3f %.3f",
					&cHeadText,
					&cEqual,
					m_stageInfo[m_stageType].pObject[nCnt]->GetPos()->x,
					m_stageInfo[m_stageType].pObject[nCnt]->GetPos()->y,
					m_stageInfo[m_stageType].pObject[nCnt]->GetPos()->z);
				fputs(cWriteText, pFile);											//	POS = GetPos()
				fputs(COMMENT_NEW_LINE, pFile);											//	\n

				strcpy(cHeadText, "ROT");
				sprintf(cWriteText, "	%s %s %.3f %.3f %.3f",
					&cHeadText,
					&cEqual,
					m_stageInfo[m_stageType].pObject[nCnt]->GetRot()->x,
					m_stageInfo[m_stageType].pObject[nCnt]->GetRot()->y,
					m_stageInfo[m_stageType].pObject[nCnt]->GetRot()->z);
				fputs(cWriteText, pFile);											//	ROT = GetRot()
				fputs(COMMENT_NEW_LINE, pFile);											//	\n
				
				strcpy(cHeadText, "SCALE");
				sprintf(cWriteText, "	%s %s %.3f %.3f %.3f",
					&cHeadText,
					&cEqual,
					m_stageInfo[m_stageType].pObject[nCnt]->GetScale()->x,
					m_stageInfo[m_stageType].pObject[nCnt]->GetScale()->y,
					m_stageInfo[m_stageType].pObject[nCnt]->GetScale()->z);
				fputs(cWriteText, pFile);											//	SCALE = GetScale()
				fputs(COMMENT_NEW_LINE, pFile);											//	\n

				strcpy(cWriteText, "END_OBJECTSET\n");
				fputs(cWriteText, pFile);													// END_OBJECTSET
				fputs(COMMENT_NEW_LINE, pFile);												// \n
			}
			fputs(COMMENT_NEW_LINE, pFile);													// \n

			strcpy(cWriteText, "END_SCRIPT\n");
			fputs(cWriteText, pFile);														// END_SCRIPT
		}
		// ファイルを閉じる
		fclose(pFile);
	}

	// ファイルを開けなかった時
	else
	{
		// コンソールに表示
		printf("オブジェクトファイルを開けませんでした。\n");
		// 失敗
		return E_FAIL;
	}

	// 成功
	return S_OK;
}

#endif