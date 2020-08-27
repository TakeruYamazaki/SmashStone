//*************************************************************************************************************
//
// 文字エフェクトのオフセット処理[CharEffectOffset.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CHAREFFECTOFFSET_H_
#define _CHAREFFECTOFFSET_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "3DEffect.h"

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class CCharEffectOffset : public CScene
{
public:
	// 列挙型定義
	typedef enum
	{
		OFFSET_NONE = -1,
		STR_ドンッ,
		OFFSET_MAX
	} OFFSETNAME;

	typedef struct PARAMCELL : public C3DEffect::SETINGPARAM
	{
		PARAMCELL() : nTime(MYLIB_INT_UNSET), SETINGPARAM() {}
		int nTime;		// 出現時間
	} PARAMCELL;

	typedef struct
	{
		int          nNumParam;		// 使うパラメータ数
		bool         bParent;		// 親フラグ
		PARAMCELL*   pCell;			// パラメータ単体の位置
	} OFFSET;

	CCharEffectOffset() : CScene(PRIORITY_EFFECT) {}											// コンストラクタ
	~CCharEffectOffset() {}																		// デストラクタ

	static void Load(void);																		// 読み込み
	static void LoadFromFile(CONST_STRING pFileName, const int &nCntOffSet);					// ファイルから読み込む
	static void LoadParamFromFile(FILE *pFile, const int &nCntOffSet, const int &nCntParam);	// ファイルからパラメータを読み込む

	static void Unload(void);																	// 開放

	void Init(void);																			// 初期化処理
	void Uninit(void);																			// 終了処理
	void Update(void);																			// 更新処理
	void Draw(void);																			// 描画処理

	static void Set(D3DXVECTOR3* pParent, CONST OFFSETNAME & name);	// 設定

private:
	bool ReleaseCheck(void);

	void SetPos(D3DXVECTOR3* pParent);
	void SetName(CONST OFFSETNAME& name);

	static OFFSET m_OffSet[OFFSETNAME::OFFSET_MAX];		// オフセット情報
	D3DXVECTOR3* m_pParent;		// 親の位置
	D3DXVECTOR3  m_pos;			// 位置
	OFFSETNAME   m_Name;		// オフセット名
	int          m_nFlame;		// フレームカウント
};

#endif