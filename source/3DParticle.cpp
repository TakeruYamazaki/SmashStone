//*************************************************************************************************************
//
// 3Dパーティクル処理[3DParticle.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "3DParticle.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
C3DParticle::OFFSET C3DParticle::m_Offset[C3DParticle::OFFSETNAME::OFFSET_MAX] = {};

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Load(void)
{
#ifdef _DEBUG
	DWORD start = timeGetTime();			// 計測スタート時間
#endif // _DEBUG
	UBITS_12 test;
	test.cValue = 4095;
	for (int nCnt = 0; nCnt < 12; nCnt++)
	{
		test.clear(nCnt);
	}
	// ファイル名
	CONST_STRING pFileName[OFFSETNAME::OFFSET_MAX] = {
		{ "data/TEXT/Effect/ParticleOffset/HitOffset.txt" },
	};

	for (int nCntOffset = 0; nCntOffset < OFFSETNAME::OFFSET_MAX; nCntOffset++)
	{
		// ファイルから読み込む
		LoadFromFile(pFileName[nCntOffset], nCntOffset);
	}

#ifdef _DEBUG
	DWORD end = timeGetTime();			// 計測スタート時間

	cout << "\nC3DParticle::Load オフセット情報の読み込み終了\n";
	cout << "C3DParticle::Load オフセット情報の読み込み 処理速度 = " << (end - start) << "　[" << (end - start) * 0.001f << "sec.]\n";
#endif // _DEBUG
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Unload(void)
{
	// オフセットのポインタ
	OFFSET *pOffset = &m_Offset[0];
	for (int nCntOffset = 0; nCntOffset < OFFSETNAME::OFFSET_MAX; nCntOffset++)
	{
		ReleaseParam(&pOffset[nCntOffset]);
	}
}

//-------------------------------------------------------------------------------------------------------------
// パラメータの開放
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::ReleaseParam(OFFSET *pOffset)
{
	for (int nCntParam = 0; nCntParam < pOffset->nNumParam; nCntParam++)
	{
		if (&pOffset->pParam[nCntParam] != nullptr)
		{
			// パラメータのメンバの開放
			ReleaseParamMemb(&pOffset->pParam[nCntParam]);
		}
	}
	delete[]pOffset->pParam;
	pOffset->pParam = nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// パラメータメンバの開放
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::ReleaseParamMemb(PARAM * pParam)
{
	if (pParam->pPos != nullptr)
	{
		delete pParam->pPos;
		pParam->pPos = nullptr;
	}
	if (pParam->pPosXRand != nullptr)
	{
		delete pParam->pPosXRand;
		pParam->pPosXRand = nullptr;
	}
	if (pParam->pPosYRand != nullptr)
	{
		delete pParam->pPosYRand;
		pParam->pPosYRand = nullptr;
	}
	if (pParam->pPosZRand != nullptr)
	{
		delete pParam->pPosZRand;
		pParam->pPosZRand = nullptr;
	}
	if (pParam->pLengthRand != nullptr)
	{
		delete pParam->pLengthRand;
		pParam->pLengthRand = nullptr;
	}
	if (pParam->pRadiusRand != nullptr)
	{
		delete pParam->pRadiusRand;
		pParam->pRadiusRand = nullptr;
	}
	if (pParam->pSpeedRand != nullptr)
	{
		delete pParam->pSpeedRand;
		pParam->pSpeedRand = nullptr;
	}
	if (pParam->pAngleRand != nullptr)
	{
		delete pParam->pAngleRand;
		pParam->pAngleRand = nullptr;
	}
	if (pParam->pLifeRand != nullptr)
	{
		delete pParam->pLifeRand;
		pParam->pLifeRand = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ファイルから読み込む
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::LoadFromFile(CONST_STRING pFileName, const int & nCntOffSet)
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
	nParent = MYLIB_INT_UNSET;


	// ファイルを開く
	if ((pFile = fopen(pFileName, "r")) == NULL)
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
			m_Offset[nCntOffSet].nNumParam = nNumParam;
			m_Offset[nCntOffSet].pParam = new PARAM[m_Offset[nCntOffSet].nNumParam];
			cout << "NUMPARAM 取得\n";
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
// ファイルからパラメータを読み込む
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::LoadParamFromFile(FILE * pFile, const int & nCntOffSet, const int & nCntParam)
{
	// 変数宣言
	char aRead[MYLIB_STRINGSIZE];	// 読み込み用
	char aComp[MYLIB_STRINGSIZE];	// 比較用
	int nIntWork;					// intの作業用
	D3DXVECTOR3 Float3Work;			// float3の作業用
	RANGE RangeWork;				// rangeの作業用
	float FlaotWork;				// floatの作業用
	D3DXCOLOR col;					// 色
	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	nIntWork = MYLIB_INT_UNSET;
	Float3Work = MYLIB_VEC3_UNSET;
	RangeWork = MYLIB_RANGE_UNSET;
	FlaotWork = MYLIB_FLOAT_UNSET;
	col = MYLIB_D3DXCOR_UNSET;

	cout << "パラム == " << nCntParam << "\n";
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

		if (sscanf(pFIleRead, "TIMER = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nTimer = nIntWork;
			cout << "TIMER 取得\n";
		}
		else if (sscanf(pFIleRead, "FRAME = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nFrame = nIntWork;
			cout << "FRAME 取得\n";
		}
		else if (sscanf(pFIleRead, "NUMBER = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nNumber = nIntWork;
			cout << "NUMBER 取得\n";
		}
		else if (sscanf(pFIleRead, "TYPE = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nType = nIntWork;
			cout << "TYPE 取得\n";
		}
		else if (sscanf(pFIleRead, "TEXTYPE = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nTexType = nIntWork;
			cout << "TEXTYPE 取得\n";
		}
		else if (sscanf(pFIleRead, "POS = %f %f %f", &Float3Work.x, &Float3Work.y, &Float3Work.z) == 3)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pPos = new D3DXVECTOR3;
			*m_Offset[nCntOffSet].pParam[nCntParam].pPos = Float3Work;
			cout << "POS 取得\n";
		}
		else if (sscanf(pFIleRead, "POSXRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pPosXRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pPosXRand = RangeWork;
			cout << "POSXRAND 取得\n";
		}
		else if (sscanf(pFIleRead, "POSYRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pPosYRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pPosYRand = RangeWork;
			cout << "POSYRAND 取得\n";
		}
		else if (sscanf(pFIleRead, "POSZRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pPosZRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pPosZRand = RangeWork;
			cout << "POSZRAND 取得\n";
		}
		else if (sscanf(pFIleRead, "LENGTH = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fLength = FlaotWork;
			cout << "LENGTH 取得\n";
		}
		else if (sscanf(pFIleRead, "LENGTHRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pLengthRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pLengthRand = RangeWork;
			cout << "LENGTHRAND 取得\n";
		}
		else if (sscanf(pFIleRead, "COL = %f %f %f %f", &col.r, &col.g, &col.b, &col.a) == 4)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].col = col;
			cout << "COL 取得\n";
		}
		else if (sscanf(pFIleRead, "RADIUS = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fRadius = FlaotWork;
			cout << "RADIUS 取得\n";
		}
		else if (sscanf(pFIleRead, "RADIUSRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pRadiusRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pRadiusRand = RangeWork;
			cout << "RADIUSRAND 取得\n";
		}
		else if (sscanf(pFIleRead, "RADIUSVALUE = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fRadiusValue = FlaotWork;
			cout << "RADIUSVALUE 取得\n";
		}
		else if (sscanf(pFIleRead, "ANGLE = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fAngle = FlaotWork;
			cout << "ANGLE 取得\n";
		}
		else if (sscanf(pFIleRead, "ANGLERAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pAngleRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pAngleRand = RangeWork;
			cout << "ANGLERAND 取得\n";
		}
		else if (sscanf(pFIleRead, "SPEED = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fSpeed = FlaotWork;
			cout << "SPEED 取得\n";
		}
		else if (sscanf(pFIleRead, "SPEEDRAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pSpeedRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pSpeedRand = RangeWork;
			cout << "SPEEDRAND 取得\n";
		}
		else if (sscanf(pFIleRead, "LIFE = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].nLife = nIntWork;
			cout << "LIFE 取得\n";
		}
		else if (sscanf(pFIleRead, "LIFERAND = %d %d", &RangeWork.nMin, &RangeWork.nMax) == 2)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].pLifeRand = new RANGE;
			*m_Offset[nCntOffSet].pParam[nCntParam].pLifeRand = RangeWork;
			cout << "LIFERAND 取得\n";
		}
		else if (sscanf(pFIleRead, "GRAVITY = %f", &FlaotWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].fGravity = FlaotWork;
			cout << "GRAVITY 取得\n";
		}
		else if (sscanf(pFIleRead, "ROT = %f %f %f", &Float3Work.x, &Float3Work.y, &Float3Work.z) == 3)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].rot = Float3Work;
			cout << "ROT 取得\n";
		}
		else if (sscanf(pFIleRead, "PARENT = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].bParent = nIntWork != 0;
			(nIntWork == 1) ?
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.set(BIT_INDEX::PARENT) :
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.clear(BIT_INDEX::PARENT);
			cout << "PARENT 取得\n";
		}
		else if (sscanf(pFIleRead, "DIREQUAPOS = %d", &nIntWork) == 1)
		{
			m_Offset[nCntOffSet].pParam[nCntParam].bDirEquaPos = nIntWork != 0;
			(nIntWork == 1) ?
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.set(BIT_INDEX::DIREQUAPOS) :
				m_Offset[nCntOffSet].pParam[nCntParam].Flag.clear(BIT_INDEX::DIREQUAPOS);
			cout << "DIREQUAPOS 取得\n";
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Init(void)
{
	m_nTimer = MYLIB_INT_UNSET;
	m_pFrame = nullptr;
	m_pPosParent = nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Uninit(void)
{
	m_pPosParent = nullptr;
	m_pRotParent = nullptr;

	if (m_pFrame != nullptr)
	{
		delete[] m_pFrame;
		m_pFrame = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Update(void)
{
	// 変数宣言
	PARAM * pParam = &m_Offset[m_Name].pParam[0];	// パラムポインタ

	// パラメータ数ループ
	for (int nCntParam = 0; nCntParam < m_Offset[m_Name].nNumParam; nCntParam++)
	{
		// ポインタがnullの時
		if (&pParam[nCntParam] == nullptr)
		{
			continue;
		}

		// タイマーよりも小さい時またはフレーム数以上の時
		if (pParam[nCntParam].nTimer > m_nTimer ||
			pParam[nCntParam].nFrame <= m_pFrame[nCntParam])
		{// 処理をスキップ
			continue;
		}
		// フレーム数を進める
		m_pFrame[nCntParam]++;

		// パラメータからエフェクトを設定
		SetEffectFromParam(&pParam[nCntParam]);

	}
	// 開放できるかチェック
	if (ReleaseCheck() == true)
	{
		this->Release();
	}
	// タイマーを加算する
	m_nTimer++;
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// パラメータからエフェクトw設定
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetEffectFromParam(PARAM * pParam)
{
	for (int nNumEffect = 0; nNumEffect < pParam->nNumber; nNumEffect++)
	{
		// 設定用
		C3DEffect::SETINGPARAM Seting;
		// ビルボードの設定
		Seting.bBillBoard = true;
		// 色の設定
		Seting.col = pParam->col;
		// 重力の設定
		Seting.fGravity = pParam->fGravity;
		// 半径の設定
		GetRadiuFromParam(pParam, Seting.fRadius);
		// 半径の変化値の設定
		Seting.fRadiusValue = pParam->fRadiusValue;
		// テクスチャタイプの設定
		Seting.nTexType = pParam->nTexType;
		// タイプの設定
		Seting.type = pParam->nType;
		// ライフの設定
		GetLifeFromParam(pParam, Seting.nLife);
		// 親ポインタの設定
		Seting.pParent = GetParentFromParam(pParam);
		// 方向と噴射位置を合わせるフラグが立っている時
		if (pParam->bDirEquaPos == true)
		{// 移動量と方向に倣った位置の取得
			GetMoveAndPosAccordingDirFromParam(pParam, Seting.move, Seting.pos);
		}
		else
		{// 位置の設定
			GetPosFromParam(pParam, Seting.pos);
			// 移動量の設定
			GetMoveFromParam(pParam, Seting.move);
		}
		// エフェクトの設定
		C3DEffect::Set(Seting);
	}
}

//-------------------------------------------------------------------------------------------------------------
// 設定
//------------------------------------------------------------------------------------------------------------
C3DParticle* C3DParticle::Set(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pRot, CONST OFFSETNAME & name)
{
	// パーティクルの生成
	C3DParticle * pParticle = new C3DParticle;
	// 初期化
	pParticle->Init();
	// 向きの設定
	pParticle->SetRot(pRot);
	// 位置の設定
	pParticle->SetPos(pPos);
	// オフセット名の設定
	pParticle->SetName(name);
	// フレームの準備
	pParticle->StandbyFrame(m_Offset[name].nNumParam);
	return pParticle;
}

//-------------------------------------------------------------------------------------------------------------
// パラメータから半径を取得
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetRadiuFromParam(CONST PARAM * pParam, float &fRadius)
{
	fRadius =
		(pParam->pRadiusRand != nullptr) ?
		(float)(rand() % pParam->pRadiusRand->nMax + pParam->pRadiusRand->nMin) :
		pParam->fRadius;
}

//-------------------------------------------------------------------------------------------------------------
// パラメータからライフを取得
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetLifeFromParam(CONST PARAM * pParam, int & nLife)
{
	nLife = 
		(pParam->pLifeRand != nullptr) ?
		(rand() % pParam->pLifeRand->nMax + pParam->pLifeRand->nMin) :
		pParam->nLife;
}

//-------------------------------------------------------------------------------------------------------------
// パラメータから親を取得
//-------------------------------------------------------------------------------------------------------------
D3DXVECTOR3 * C3DParticle::GetParentFromParam(CONST PARAM * pParam)
{
	return (pParam->bParent == true) ? m_pPosParent : nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// パラメータから位置を取得
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetPosFromParam(CONST PARAM * pParam, D3DXVECTOR3 & pos)
{
	if (pParam->pPos != nullptr)
	{
		pos = (pParam->bParent == true) ?
			*pParam->pPos + m_pos :
			*pParam->pPos;

	}
	else if (pParam->pPosXRand != nullptr &&
		pParam->pPosYRand != nullptr &&
		pParam->pPosZRand != nullptr)
	{
		if (pParam->bParent == true)
		{
			pos.x = (float)(rand() % pParam->pPosXRand->nMax + pParam->pPosXRand->nMin) + m_pos.x;
			pos.y = (float)(rand() % pParam->pPosYRand->nMax + pParam->pPosYRand->nMin) + m_pos.y;
			pos.z = (float)(rand() % pParam->pPosZRand->nMax + pParam->pPosZRand->nMin) + m_pos.z;
		}
		else
		{
			pos.x = (float)(rand() % pParam->pPosXRand->nMax + pParam->pPosXRand->nMin);
			pos.y = (float)(rand() % pParam->pPosYRand->nMax + pParam->pPosYRand->nMin);
			pos.z = (float)(rand() % pParam->pPosZRand->nMax + pParam->pPosZRand->nMin);

		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// パラメータから移動量を取得
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetMoveFromParam(CONST PARAM * pParam, D3DXVECTOR3 & move)
{
	// 変数宣言
	float       fSpeed;		// 速度

	// 速度の取得
	GetSpeedFromParam(pParam, fSpeed);

	move.x = pParam->rot.x + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	move.y = pParam->rot.y + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	move.z = pParam->rot.z + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	// 正規化
	CMylibrary::CreateUnitVector(&move, &move);

	move *= fSpeed;
}

//-------------------------------------------------------------------------------------------------------------
// パラメータから速度を取得
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetSpeedFromParam(CONST PARAM * pParam, float & fSpeed)
{
	fSpeed =
		(pParam->pSpeedRand != nullptr) ?
		(float)(rand() % pParam->pSpeedRand->nMax + pParam->pSpeedRand->nMin) :
		pParam->fSpeed;
}

//-------------------------------------------------------------------------------------------------------------
// パラメータから移動量と方向に倣った位置の取得
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetMoveAndPosAccordingDirFromParam(CONST PARAM * pParam, D3DXVECTOR3 & move, D3DXVECTOR3 & pos)
{
	// 変数宣言
	float fSpeed;		// 速度
	float fLength;		// 原点からの距離

	// 速度の取得
	GetSpeedFromParam(pParam, fSpeed);

	// 原点からの距離を取得
	GetLengthFromParam(pParam, fLength);

	// 方向ベクトルを計算
	move.x = pParam->rot.x + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	move.y = pParam->rot.y + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	move.z = pParam->rot.z + (rand() % 628 - 314) * 0.01f *pParam->fAngle;
	// 正規化
	CMylibrary::CreateUnitVector(&move, &move);

	// 位置を計算する
	if (pParam->bParent == true)
	{
		pos.x = move.x * fLength + m_pos.x;
		pos.y = move.y * fLength + m_pos.y;
		pos.z = move.z * fLength + m_pos.z;
	}
	else
	{
		pos.x = move.x * fLength;
		pos.y = move.y * fLength;
		pos.z = move.z * fLength;
	}


	// 移動量を計算する
	move *= fSpeed;
}

//-------------------------------------------------------------------------------------------------------------
// パラメータから原点からの距離を取得
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::GetLengthFromParam(CONST PARAM * pParam, float & fLength)
{
	fLength =
		(pParam->pLengthRand != nullptr) ?
		(float)(rand() % pParam->pLengthRand->nMax + pParam->pLengthRand->nMin) :
		pParam->fLength;
}

//-------------------------------------------------------------------------------------------------------------
// 開放できるかチェック
//-------------------------------------------------------------------------------------------------------------
bool C3DParticle::ReleaseCheck(void)
{
	// 変数宣言
	PARAM * pParam = &m_Offset[m_Name].pParam[0];	// パラムポインタ
	int nCntApea   = MYLIB_INT_UNSET;				// 出現カウント
	// パラメータ数ループ
	for (int nCntParam = 0; nCntParam < m_Offset[m_Name].nNumParam; nCntParam++)
	{
		if (pParam[nCntParam].nFrame <= m_pFrame[nCntParam])
		{
			nCntApea++;
		}
	}
	if (m_Offset[m_Name].nNumParam <= nCntApea)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// 位置の設定
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetPos(D3DXVECTOR3 * pPos)
{
	m_pPosParent = pPos;
	m_pos = *pPos;
}

//-------------------------------------------------------------------------------------------------------------
// 向きの設定
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetRot(D3DXVECTOR3 * pRot)
{
	m_pRotParent = pRot;
	m_rot = *pRot;
}

//-------------------------------------------------------------------------------------------------------------
// オフセット名の設定
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetName(CONST OFFSETNAME & name)
{
	m_Name = name;
}

//-------------------------------------------------------------------------------------------------------------
// フレームの準備
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::StandbyFrame(const int & nNumParam)
{
	// フレームの生成
	m_pFrame = new int[nNumParam];

	for (int nCntFrame = 0; nCntFrame <nNumParam; nCntFrame++)
	{
		m_pFrame[nCntFrame] = MYLIB_INT_UNSET;
	}
}

//-------------------------------------------------------------------------------------------------------------
// パラム構造体のコンストラクタ
//-------------------------------------------------------------------------------------------------------------
C3DParticle::PARAM::PARAM()
{
	nTimer       = MYLIB_INT_UNSET;							// タイマー
	nFrame       = MYLIB_INT_UNSET;							// フレーム数
	nNumber      = MYLIB_INT_UNSET;							// 個数
	nType        = C3DEffect::TYPE::TYPE_NONE;				// 種類
	nTexType     = C3DEffect::TEXTURETYPE::TEXTYPE_NONE;	// テクスチャの種類
	pPos         = nullptr;									// 位置のポインタ
	pPosXRand    = nullptr;									// 位置X座標ランダム用のポインタ
	pPosYRand    = nullptr;									// 位置Y座標ランダム用のポインタ
	pPosZRand    = nullptr;									// 位置Y座標ランダム用のポインタ
	fLength      = MYLIB_FLOAT_UNSET;						// 原点からの距離
	pLengthRand  = nullptr;									// 原点からの距離ランダム用のポインタ
	col          = MYLIB_D3DXCOR_UNSET;						// 色のポインタ
	fRadius      = MYLIB_FLOAT_UNSET;						// 半径
	pRadiusRand  = nullptr;									// 半径のランダム用のポインタ
	fRadiusValue = MYLIB_FLOAT_UNSET;						// 半径の変化量
	fAngle       = MYLIB_FLOAT_UNSET;						// 噴射角度
	pAngleRand   = nullptr;									// 噴射角度のランダム用のポインタ
	fSpeed       = MYLIB_FLOAT_UNSET;						// 速度
	pSpeedRand   = nullptr;									// 速度のランダム用のポインタ
	nLife        = MYLIB_INT_UNSET;							// ライフ
	pLifeRand    = nullptr;									// ライフのランダム用のポインタ
	fGravity     = MYLIB_FLOAT_UNSET;						// 重力
	rot          = MYLIB_VEC3_UNSET;						// 向き
	bParent      = false;									// 親フラグ
	bDirEquaPos  = false;									// 方向と噴射位置を同じにする
}

//-------------------------------------------------------------------------------------------------------------
// オフセット構造体のコンストラクタ
//-------------------------------------------------------------------------------------------------------------
C3DParticle::OFFSET::OFFSET()
{
	int    nNumParam = MYLIB_INT_UNSET;	// 使うパラメータ数
	PARAM* pParam    = nullptr;			// パラメータ単体の位置
}
