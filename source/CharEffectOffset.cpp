//*************************************************************************************************************
//
// 文字エフェクトのオフセット処理[CharEffectOffset.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "CharEffectOffset.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define CHAREFFEOFF_OPNEMODE "r"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
CCharEffectOffset::OFFSET CCharEffectOffset::m_OffSet[CCharEffectOffset::OFFSETNAME::OFFSET_MAX] = {};		// オフセット情報

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Load(void)
{
#ifdef _DEBUG
	DWORD start = timeGetTime();			// 計測スタート時間
#endif // _DEBUG

	// ファイル名
	CONST_STRING pFileName[OFFSETNAME::OFFSET_MAX] = {
		{ "data/TEXT/Effect/CharEffectOffset/Offset_ドンッ.txt" },
	};

	for (int nCntOffSet = 0; nCntOffSet < OFFSETNAME::OFFSET_MAX; nCntOffSet++)
	{
		// ファイルから読み込む
		LoadFromFile(pFileName[nCntOffSet], nCntOffSet);
	}

#ifdef _DEBUG
	DWORD end = timeGetTime();			// 計測スタート時間

	cout << "\nCCharEffectOffset::Load オフセット情報の読み込み終了\n";
	cout << "CCharEffectOffset::Load オフセット情報の読み込み 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]\n";
#endif // _DEBUG


}

//-------------------------------------------------------------------------------------------------------------
// ファイルから読み込む
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::LoadFromFile(CONST_STRING pFileName, const int & nCntOffSet)
{
	// 変数宣言
	FILE *pFile;						// ファイルポインタ
	char aRead[MYLIB_STRINGSIZE];		// 読み込み用
	char aComp[MYLIB_STRINGSIZE];		// 比較用
	char aWork[MYLIB_STRINGSIZE];		// 作業用
	int  nCntParam;						// 情報カウント
	int  nNumParam;						// 使うパラメータ数
	int  nParent;						// 親フラグ

	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	aWork[0] = MYLIB_CHAR_UNSET;
	nCntParam = MYLIB_INT_UNSET;
	nNumParam = MYLIB_INT_UNSET;
	nParent  = MYLIB_INT_UNSET;


	// ファイルを開く
	if ((pFile = fopen(pFileName, CHAREFFEOFF_OPNEMODE)) == NULL)
	{// 失敗した時
		return;
	}

	// スクリプト
	while (strcmp(aComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(aRead, sizeof(aRead), pFile);
		// 読み込んど文字列代入
		sscanf(aRead, "%s", &aComp);
	}

	// エンドスクリプト
	while (strcmp(aComp, "END_SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(aRead, sizeof(aRead), pFile);
		// 読み込んど文字列代入
		sscanf(aRead, "%s", &aComp);

		
		if (sscanf(aRead, "NUMPARAM = %d", &nNumParam) == 1)
		{
			m_OffSet[nCntOffSet].nNumParam = nNumParam;
			m_OffSet[nCntOffSet].pCell = new PARAMCELL[m_OffSet[nCntOffSet].nNumParam];
		}
		else if (sscanf(aRead, "PARENT = %d", &nParent) == 1)
		{
			m_OffSet[nCntOffSet].bParent = (nParent != 0);
		}
		else if (strcmp(aComp, "SETPARAM") == 0)
		{
			LoadParamFromFile(pFile, nCntOffSet, nCntParam);
			nCntParam++;
			aComp[0] = MYLIB_CHAR_UNSET;
		}
	}
	fclose(pFile);
}

//-------------------------------------------------------------------------------------------------------------
//　ファイルからパラメータを読みこむ
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::LoadParamFromFile(FILE * pFile, const int & nCntOffSet, const int &nCntParam)
{
	// 変数宣言
	PARAMCELL Seting;					// 設定用オプション
	char aRead[MYLIB_STRINGSIZE];		// 読み込み用
	char aComp[MYLIB_STRINGSIZE];		// 比較用
	char aWork[MYLIB_STRINGSIZE];		// 作業用
	int nBillBoard;						// ビルボードフラグ
	Seting = MYLIB_INITSTRUCT_WITHCONST;
	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	aWork[0] = MYLIB_CHAR_UNSET;
	nBillBoard = MYLIB_INT_UNSET;

	// エンドパラム
	while (strcmp(aComp, "END_SETPARAM") != 0)
	{
		STRING pFIleRead = &aRead[0];
		// 1行読み込む
		fgets(pFIleRead, MYLIB_STRINGSIZE, pFile);
		// 読み込んど文字列代入
		sscanf(pFIleRead, "%s", &aComp);

		while (*pFIleRead == '\t')
		{
			pFIleRead++;
		}

		if (sscanf(pFIleRead, "TIME = %d", &Seting.nTime) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].nTime = Seting.nTime;
		}
		else if (sscanf(pFIleRead, "BILLBOARD = %d", &nBillBoard) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].bBillBoard = (nBillBoard != 0);
		}
		else if (sscanf(pFIleRead, "TYPE = %d", &Seting.type) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].type = Seting.type;
		}
		else if (sscanf(pFIleRead, "TEXTYPE = %d", &Seting.nTexType) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].nTexType = Seting.nTexType;
		}
		else if (sscanf(pFIleRead, "POS = %f %f %f", &Seting.pos.x, &Seting.pos.y, &Seting.pos.z) == 3)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].pos = Seting.pos;
		}
		else if (sscanf(pFIleRead, "MOVE = %f %f %f", &Seting.move.x, &Seting.move.y, &Seting.move.z) == 3)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].move = Seting.move;
		}
		else if (sscanf(pFIleRead, "COL = %f %f %f %f", &Seting.col.r, &Seting.col.g, &Seting.col.b, &Seting.col.a) == 4)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].col = Seting.col;
		}
		else if (sscanf(pFIleRead, "RADIUS = %f", &Seting.fRadius) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].fRadius = Seting.fRadius;
		}
		else if (sscanf(pFIleRead, "LIFE = %d", &Seting.nLife) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].nLife = Seting.nLife;
		}
		else if (sscanf(pFIleRead, "GRAVITY = %f", &Seting.fGravity) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].fGravity = Seting.fGravity;
		}
		else if (sscanf(pFIleRead, "RADIUSVALUE = %f", &Seting.fRadiusValue) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].fRadiusValue = Seting.fRadiusValue;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Unload(void)
{
	for (int nCntOffSet = 0; nCntOffSet < OFFSETNAME::OFFSET_MAX; nCntOffSet++)
	{
		if (m_OffSet[nCntOffSet].pCell != nullptr)
		{
			delete[] m_OffSet[nCntOffSet].pCell;
			m_OffSet[nCntOffSet].pCell = nullptr;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Init(void)
{
	m_nFlame = MYLIB_INT_UNSET;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Update(void)
{
	// 変数宣言
	PARAMCELL* pParamCell = &m_OffSet[m_Name].pCell[0];		// パラメータ単体のポインタ

	for (int nCntParam = 0; nCntParam < m_OffSet[m_Name].nNumParam; nCntParam++)
	{
		if (pParamCell[nCntParam].nTime != m_nFlame)
		{
			continue;
		}

		if (m_pParent != nullptr)
		{
			pParamCell[nCntParam].pParent = m_pParent;
		}
		else
		{
			pParamCell[nCntParam].pos += m_pos;
		}
		
		C3DEffect::Set(pParamCell[nCntParam]);

		if (m_pParent != nullptr)
		{
			pParamCell[nCntParam].pParent = nullptr;
		}
		else
		{
			pParamCell[nCntParam].pos -= m_pos;
		}
	}

	// 開放確認
	if (ReleaseCheck() == true)
	{
		CScene::Release();
	}

	// フレームを進める
	m_nFlame++;
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 設定
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Set(D3DXVECTOR3 * pParent, CONST OFFSETNAME & name)
{
	CCharEffectOffset *pOffset = new CCharEffectOffset;
	pOffset->Init();
	pOffset->SetName(name);
	pOffset->SetPos(pParent);
}

//-------------------------------------------------------------------------------------------------------------
// 開放確認
//-------------------------------------------------------------------------------------------------------------
bool CCharEffectOffset::ReleaseCheck(void)
{
	// 出現カウント
	int nCntApea = MYLIB_INT_UNSET;
	for (int nCntParam = 0; nCntParam < m_OffSet[m_Name].nNumParam; nCntParam++)
	{
		if (m_OffSet[m_Name].pCell[nCntParam].nTime < m_nFlame)
		{
			nCntApea++;
		}
	}

	if (nCntApea == m_OffSet[m_Name].nNumParam)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// 位置の設定
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::SetPos(D3DXVECTOR3 * pParent)
{
	if (m_OffSet[m_Name].bParent == true)
	{
		m_pParent = pParent;
		m_pos = MYLIB_VEC3_UNSET;
	}
	else
	{
		m_pParent = nullptr;
		m_pos = *(D3DXVECTOR3 *)pParent;
	}
	
}

//-------------------------------------------------------------------------------------------------------------
// 名前の設定
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::SetName(CONST OFFSETNAME & name)
{
	m_Name = name;
}

