//==================================================================================================================
//
// カメラ[camera.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//==================================================================================================================
// インクルードファイル
//==================================================================================================================
#include "main.h"

//==================================================================================================================
// 前方宣言
//==================================================================================================================
class CInputKeyboard;

//==================================================================================================================
//
// カメラクラスの定義
//
//==================================================================================================================
class CCamera
{
public:
	typedef enum
	{	// カメラの状態
		CAMERA_GAME = 0,	// ゲームカメラ
		CAMERA_DEBUG	// デバッグカメラ
	} CAMERA_MODE;

	CCamera();							// コンストラクタ
	~CCamera();							// デストラクタ
	void Init();						// 初期化処理
	void Uninit(void);					// 終了処理
	void Update(void);					// 更新処理
	void Draw(void);					// 描画処理

	static CCamera *Create(void);		// 生成処理

	void SetCamera(void);				// カメラの設定処理
	void SetCameraPos(const D3DXVECTOR3 posV, const D3DXVECTOR3 posR);	// カメラの位置設定
	void SetRot(const D3DXVECTOR3 &rot)		{ m_rot = rot; }		// 回転設定処理
	void SetPosV(const D3DXVECTOR3 &posV)	{ m_posV = posV; }		// 視点位置設定処理
	void SetPosR(const D3DXVECTOR3 &posR)	{ m_posR = posR; }		// 注視点の設定

	D3DXVECTOR3 *GetRot(void)	{ return &m_rot; }	// 回転取得処理
	D3DXVECTOR3 *GetPosV(void)	{ return &m_posV; }	// 視点位置設定処理
	D3DXVECTOR3 *GetPosR(void)	{ return &m_posR; }	// 注視点の取得
	float		GetRotY(void)	{ return m_rot.y; }	// 回転(Y軸)の取得
	D3DXVECTOR3 *GetVec(void)	{ return &m_vec; }	// ベクトルの取得

protected:

private:
	void ShowDebugInfo(void);			// ImGuiの更新
	void GameCamera(void);				// ゲームのカメラ
	void DebugControl(void);			// デバッグ時の操作
	void ControlMouse(void);			// マウスでの操作
	void ControlKeyboard(CInputKeyboard *pKeyboard);	// キーボードでの操作
	static CCamera *m_pCamera;			// カメラ情報
	CAMERA_MODE m_mode;					// カメラの状態

	D3DXMATRIX m_mtxProjection;			// プロジェクションマトリックス
	D3DXMATRIX m_mtxView;				// ビューマトリックス

	D3DXVECTOR3 m_posV;					// 視点
	D3DXVECTOR3 m_posVDest;				// 視点の目標地点
	D3DXVECTOR3 m_posR;					// 注視点(参照点)
	D3DXVECTOR3 m_posRDest;				// 注視点(参照点)の目標地点
	D3DXVECTOR3 m_posDebug;				// デバッグ用の位置
	D3DXVECTOR3 m_posU;					// 上方向ベクトル
	D3DXVECTOR3 m_rot;					// 回転
	D3DXVECTOR3 m_rotDest;				// 回転の目標地点
	D3DXVECTOR3 m_vec;					// カメラの向いているベクトル
	D3DXVECTOR3 m_mousePos;				// マウス操作用

	int nCntRot;						// 回転を始めるカウンタ
	int m_nCntTitleFade;				// タイトルフェードのカウンタ

	float m_fDistance;					// 距離
	float m_fDisScale;					// 拡張可能な距離

	bool m_bCameraMode;					// カメラモードかどうか
};
#endif