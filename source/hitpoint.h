//==================================================================================================================
//
// HPクラス[hitpoint.h]
// Author:Seiya Takahashi
//
//==================================================================================================================
#ifndef _HITPOINT_H
#define _HITPOINT_H

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"
#include "scene.h"

//==================================================================================================================
// マクロ定義
//==================================================================================================================
#define MAX_BAR 5			// バーの最大本数

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CBar;
class CScene;
class CPlayer;

//==================================================================================================================
//クラスの定義
//==================================================================================================================
class CHitPoint : public CScene
{
public:

	CHitPoint(PRIORITY type);			// コンストラクタ
	~CHitPoint();						// デストラクタ
	void Init(void);					// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static CHitPoint *Create(int nNumber);// 生成処理

	void SetPos(D3DXVECTOR3 pos);		// 位置設定処理
	void SetnPlayerNum(int nPlayerNum);	// プレイヤー番号設定処理

	D3DXVECTOR3 GetPos(void);			// 位置取得処理

protected:

private:

	void Bar0Update(float NowHP);	// バー0の更新処理
	void Bar1Update(float NowHP);	// バー1の更新処理
	void Bar2Update(float NowHP);	// バー2の更新処理
	void Bar3Update(float NowHP);	// バー3の更新処理
	void Bar4Update(float NowHP);	// バー4の更新処理

	static CPlayer *m_pPlayer;		// プレイヤーの情報ポインタ
	static int m_nInitCnt;					// 初期化用カウンタ

	CBar *m_pBar;					// バーの情報ポインタ

	D3DXVECTOR3 m_pos;				// 位置
	D3DXVECTOR3 m_rot0;				// 回転
	D3DXVECTOR3 m_rot1;				// 回転
	D3DXVECTOR3 m_rot2;				// 回転
	D3DXVECTOR3 m_rot3;				// 回転

	int m_nCntCol0;			// α値減少用カウンタ
	int m_nCntCol1;			// α値減少用カウンタ
	int m_nCntCol2;			// α値減少用カウンタ
	int m_nCntCol3;			// α値減少用カウンタ
	int m_nCntPos0;			// 位置変更用カウンタ
	int m_nCntPos1;			// 位置変更用カウンタ
	int m_nCntPos2;			// 位置変更用カウンタ
	int m_nCntPos3;			// 位置変更用カウンタ
	int m_nPlayerNum;		// プレイヤーの番号

	float m_fNowHP;			// 現在のHP
	float m_fMaxHP;			// 最大HP
	float m_fHeight;		// 縦の長さ

	bool m_bBar[MAX_BAR];	// バーがあるかどうか
};

#endif
