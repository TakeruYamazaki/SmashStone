//*************************************************************************************************************
//
// 3Dパーティクル処理[3DParticle.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _3DPARTICLE_H_
#define _3DPARTICLE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "3DEffect.h"


//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C3DParticle : public CScene
{
public:
	typedef enum
	{
		OFFSET_NONE = -1,	// 無し
		HIT,				// 当たった
		OFFSET_MAX			// 最大
	}OFFSETNAME;

	typedef enum
	{
		BITS_NONE = -1,
		TARGET,			// 目標点有効フラグ
		LIFE_AUTO,		// ライフ自動フラグ
		ALPHA_AUTO,		// アルファ値自動フラグ
		RADIUS_AUTO,	// 半径変化値自動フラグ
		MOVE_AUTO,		// 移動量自動フラグ
		DIR_REVERSE,	// 移動方向逆フラグ
		PARENT,			// 親フラグ
		CLING_PARENT,	// 親にくっつくフラグ
		MOVE_CHANGE,	// 移動量変化フラグ
		DIREQUAPOS,		// 方向と噴射位置を同じにする
		BITS_MAX		// 最大数
	} BIT_INDEX;

	typedef struct PARAM
	{
		PARAM();							// コンストラクタ
		int				nTimer;				// タイマー
		int				nFrame;				// フレーム数
		int				nNumber;			// 個数
		int				nType;				// 種類
		int				nTexType;			// テクスチャの種類
		D3DXVECTOR3*	pPos;				// 位置のポインタ
		RANGE*			pPosXRand;			// 位置X座標ランダム用のポインタ
		RANGE*			pPosYRand;			// 位置Y座標ランダム用のポインタ
		RANGE*			pPosZRand;			// 位置Y座標ランダム用のポインタ
		float			fLength;			// 原点からの距離
		RANGE*			pLengthRand;		// 原点からの距離ランダム用のポインタ
		D3DXCOLOR		col;				// 色
		float			fRadius;			// 半径
		RANGE*			pRadiusRand;		// 半径のランダム用のポインタ
		float			fRadiusValue;		// 半径の変化量
		float			fAngle;				// 噴射角度
		RANGE*			pAngleRand;			// 噴射角度のランダム用のポインタ
		float			fSpeed;				// 速度
		RANGE*			pSpeedRand;			// 速度のランダム用のポインタ
		int				nLife;				// ライフ
		RANGE*			pLifeRand;			// ライフのランダム用のポインタ
		float			fGravity;			// 重力
		D3DXVECTOR3		rot;				// 向き
		D3DXVECTOR3		Target;				// 目標地点
		float			fInertiaCoeffi;		// 慣性係数
		D3DXCOLOR		colValue;			// 色の変化値
		RANGE*			pColRRand;			// 色のRランダム用のポインタ
		RANGE*			pColGRand;			// 色のGランダム用のポインタ
		RANGE*			pColBRand;			// 色のBランダム用のポインタ
		RANGE*			pColARand;			// 色のAランダム用のポインタ
		float			fScalCol;			// 色の縮小値
		UBITS_12		Flag;				// フラグ

		bool			bParent;			// 親フラグ
		bool			bDirEquaPos;		// 方向と噴射位置を同じにする

	}PARAM;

	typedef struct OFFSET
	{
		OFFSET();					// コンストラクタ
		int          nNumParam;		// 使うパラメータ数
		PARAM*       pParam;		// パラメータ単体の位置
	} OFFSET;

	C3DParticle() : CScene(CScene::PRIORITY_EFFECT) {}
	~C3DParticle() {}

	static void Load(void);																				// 読み込み
	static void Unload(void);																			// 開放
	static void ReleaseParam(OFFSET *pOffset);															// パラメータの開放
	static void ReleaseParamMemb(PARAM * pParam);														// パラメータメンバの開放

	static void LoadFromFile(CONST_STRING pFileName, const int &nCntOffSet);							// ファイルから読み込む
	static void LoadParamFromFile(FILE *pFile, const int &nCntOffSet, const int &nCntParam);			// ファイルからパラメータを読み込む

	void Init(void);																					// 初期化処理
	void Uninit(void);																					// 終了処理
	void Update(void);																					// 更新処理
	void Draw(void);																					// 描画処理

	void SetEffectFromParam(PARAM * pParam);															// パラメータからエフェクトを設定

	static C3DParticle* Set(D3DXVECTOR3* pPos, D3DXVECTOR3* pRot, CONST OFFSETNAME & name);			// 設定

private:
	void GetRadiuFromParam(CONST PARAM * pParam, float &fRadius);										// パラメータから半径を取得
	void GetLifeFromParam(CONST PARAM * pParam, int &nLife);											// パラメータからライフを取得
	D3DXVECTOR3 * GetParentFromParam(CONST PARAM * pParam);												// パラメータから親を取得
	void GetPosFromParam(CONST PARAM * pParam, D3DXVECTOR3 &pos);										// パラメータから位置を取得
	void GetMoveFromParam(CONST PARAM * pParam, D3DXVECTOR3 &move);										// パラメータから移動量を取得
	void GetSpeedFromParam(CONST PARAM * pParam, float &fSpeed);										// パラメータから速度を取得
	void GetMoveAndPosAccordingDirFromParam(CONST PARAM * pParam, D3DXVECTOR3 &move, D3DXVECTOR3 &pos);	// パラメータから移動量と方向に倣った位置の取得
	void GetLengthFromParam(CONST PARAM * pParam, float &fLength);										// パラメータから原点からの距離を取得
	bool ReleaseCheck(void);																			// 開放できるかチェックする
	void SetPos(D3DXVECTOR3* pPos);																		// 位置の設定
	void SetRot(D3DXVECTOR3* pRot);																		// 向きの設定
	void SetName(CONST OFFSETNAME& name);																// オフセット名の設定
	void StandbyFrame(const int& nNumParam);															// フレームポインタの準備
	static OFFSET m_Offset[OFFSET_MAX];																	// オフセット情報
	D3DXVECTOR3* m_pPosParent;																			// 親の位置
	D3DXVECTOR3  m_pos;																					// 位置
	D3DXVECTOR3* m_pRotParent;																			// 親の向き
	D3DXVECTOR3  m_rot;																					// 向き
	OFFSETNAME   m_Name;																				// オフセット名
	int          m_nTimer;																				// タイマーカウント
	int*         m_pFrame;																				// フレームポインタ
};










#endif