//==================================================================================================================
//
// 棒の描画 [bar.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS

//==================================================================================================================
//	インクルードファイル
//==================================================================================================================
#include <stdio.h>
#include <stdlib.h>
#include "renderer.h"
#include "manager.h"
#include "bar.h"
#include "debugProc.h"

//==================================================================================================================
//	マクロ定義
//==================================================================================================================
#define SizeX 500			// 横のサイズ
#define SizeY 60			// 縦のサイズ

//==================================================================================================================
//	静的メンバ変数宣言
//==================================================================================================================
LPDIRECT3DTEXTURE9 CBar::m_pTexture[BARTYPE_MAX] = {};		// テクスチャ情報
char *CBar::m_apFileName[BARTYPE_MAX] =						// 読み込むモデルのソース先
{
	{ "data/TEXTURE/HPbar.png" },				// 0番目の体力バー
	{ "data/TEXTURE/HPframe.png" },				// 0番目のフレーム
	{ "data/TEXTURE/HPbar.png" },				// 1番目の体力バー
	{ "data/TEXTURE/HPframe.png" },				// 1番目のフレーム
	{ "data/TEXTURE/HPbar.png" },				// 2番目の体力バー
	{ "data/TEXTURE/HPframe.png" },				// 2番目のフレーム
	{ "data/TEXTURE/HPbar.png" },				// 3番目の体力バー
	{ "data/TEXTURE/HPframe.png" },				// 3番目のフレーム
	{ "data/TEXTURE/HPbar.png" },				// 4番目の体力バー
	{ "data/TEXTURE/HPframe.png" },				// 4番目のフレーム
};

//==================================================================================================================
//	コンストラクタ
//==================================================================================================================
CBar::CBar()
{

}

//==================================================================================================================
//	デストラクタ
//==================================================================================================================
CBar::~CBar(void)
{

}

//==================================================================================================================
//	初期化処理
//==================================================================================================================
void CBar::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// デバイスを取得する

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TEXTURE_BAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
}

//==================================================================================================================
//	終了処理
//==================================================================================================================
void CBar::Uninit(void)
{
	// 頂点バッファがあるとき
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();	// 頂点バッファの開放
		m_pVtxBuff = NULL;		// NULLにする
	}
}

//==================================================================================================================
//	更新処理
//==================================================================================================================
void CBar::Update(void)
{

}

//==================================================================================================================
//	描画処理
//==================================================================================================================
void CBar::Draw(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// デバイスを取得する

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの最大数までカウント
	for (int nCntTex = 0; nCntTex < TEXTURE_BAR; nCntTex++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCntTex]);

		// ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTex * 4, 2);
	}

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);
}

//==================================================================================================================
// オブジェクトの生成
//==================================================================================================================
CBar *CBar::Create(void)
{
	CBar *pBar = NULL;			// 変数初期化

	pBar = new CBar;			// メモリを動的に確保

	pBar->Init();				// 初期化処理

	return pBar;				// 値を返す
}

//==================================================================================================================
// テクスチャ情報ロード
//==================================================================================================================
HRESULT CBar::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// レンダラーの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// デバイスを取得する

	//==============================テクスチャの読み込み==============================//
	// テクスチャの最大数までカウント
	for (int nCnt = 0; nCnt < BARTYPE_MAX; nCnt++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
	}

	// 値を返す
	return S_OK;
}

//==================================================================================================================
// テクスチャ情報破棄
//==================================================================================================================
void CBar::Unload(void)
{
	// テクスチャの最大数までカウント
	for (int nCntTex = 0; nCntTex < TEXTURE_BAR; nCntTex++)
	{
		// テクスチャがあるとき
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();		// テクスチャの開放
			m_pTexture[nCntTex] = NULL;			// NULLにする
		}
	}
}

//==================================================================================================================
// 頂点の設定
//==================================================================================================================
void CBar::SetVertexBar(int index, D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタ取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx += index * 4;	// 頂点を4つずつ加算

	// 頂点座標の設定(右回りで設定する)
	m_pVtx[0].pos.x = pos.x - fWidth / 2;
	m_pVtx[0].pos.y = pos.y - fHeight;
	m_pVtx[0].pos.z = 0.0f;

	m_pVtx[1].pos.x = pos.x + fWidth / 2;
	m_pVtx[1].pos.y = pos.y - fHeight;
	m_pVtx[1].pos.z = 0.0f;

	m_pVtx[2].pos.x = pos.x - fWidth / 2;
	m_pVtx[2].pos.y = pos.y;
	m_pVtx[2].pos.z = 0.0f;

	m_pVtx[3].pos.x = pos.x + fWidth / 2;
	m_pVtx[3].pos.y = pos.y;
	m_pVtx[3].pos.z = 0.0f;

	// 同次座標(1.0で固定)
	m_pVtx[0].rhw = 1.0f;
	m_pVtx[1].rhw = 1.0f;
	m_pVtx[2].rhw = 1.0f;
	m_pVtx[3].rhw = 1.0f;

	// 色の設定
	m_pVtx[0].col = col;
	m_pVtx[1].col = col;
	m_pVtx[2].col = col;
	m_pVtx[3].col = col;

	// テクスチャ座標の設定
	m_pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	m_pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	m_pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	m_pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();

}

//==================================================================================================================
// バー回転処理
//==================================================================================================================
void CBar::RotBar(int index, D3DXVECTOR3 pos, float fAngle, float fLength)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx += index * 4;					// 頂点を4つずつ加算

	D3DXVECTOR3 originPos0 = m_pVtx[0].pos - pos;
	D3DXVECTOR3 originPos1 = m_pVtx[1].pos - pos;
	D3DXVECTOR3 originPos2 = m_pVtx[2].pos - pos;
	D3DXVECTOR3 originPos3 = m_pVtx[3].pos - pos;

	// 移動座標の設定
	m_pVtx[0].pos.x = originPos0.x * cosf(fAngle) - originPos0.y * sinf(fAngle) + pos.x;
	m_pVtx[0].pos.y = originPos0.x * sinf(fAngle) + originPos0.y * cosf(fAngle) + pos.y;
	m_pVtx[0].pos.z = 0.0f;

	m_pVtx[1].pos.x = originPos1.x * cosf(fAngle) - originPos1.y * sinf(fAngle) + pos.x;
	m_pVtx[1].pos.y = originPos1.x * sinf(fAngle) + originPos1.y * cosf(fAngle) + pos.y;
	m_pVtx[1].pos.z = 0.0f;

	m_pVtx[2].pos.x = originPos2.x * cosf(fAngle) - originPos2.y * sinf(fAngle) + pos.x;
	m_pVtx[2].pos.y = originPos2.x * sinf(fAngle) + originPos2.y * cosf(fAngle) + pos.y;
	m_pVtx[2].pos.z = 0.0f;

	m_pVtx[3].pos.x = originPos3.x * cosf(fAngle) - originPos3.y * sinf(fAngle) + pos.x;
	m_pVtx[3].pos.y = originPos3.x * sinf(fAngle) + originPos3.y * cosf(fAngle) + pos.y;
	m_pVtx[3].pos.z = 0.0f;

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//==================================================================================================================
// バー色設定処理
//==================================================================================================================
void CBar::SetColBar(int index, D3DXCOLOR col)
{
	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	m_pVtx += index * 4;					// 頂点を4つずつ加算

	// 頂点カラー
	m_pVtx[0].col = col;
	m_pVtx[1].col = col;
	m_pVtx[2].col = col;
	m_pVtx[3].col = col;

	// 頂点データをアンロックする
	m_pVtxBuff->Unlock();

}
