//=============================================================================
//
// シーンを使わない2Dポリゴン処理 [ polygon2D.cpp ]
// Author : KANAN NAGANAWA
//
//=============================================================================
#include "polygon2D.h"
#include "renderer.h"
#include "manager.h"
#include "kananlibrary.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数の初期化
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
CPolygon2D::CPolygon2D()
{
}

//=============================================================================
// デストラクタ
//=============================================================================
CPolygon2D::~CPolygon2D()
{
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CPolygon2D::Init()
{
	m_pos		= ZeroVector3;
	m_rot		= ZeroVector3;
	m_size		= ZeroVector3;
	m_col		= WhiteColor;
	m_fAngle	= 0.0f;
	m_fLength	= 0.0f;
	m_bShow		= true;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報の設定
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = ZeroVector3;
	pVtx[1].pos = ZeroVector3;
	pVtx[2].pos = ZeroVector3;
	pVtx[3].pos = ZeroVector3;

	//頂点の同次座標
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点の色
	pVtx[0].col = WhiteColor;
	pVtx[1].col = WhiteColor;
	pVtx[2].col = WhiteColor;
	pVtx[3].col = WhiteColor;

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
	
	// 成功
	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CPolygon2D::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CPolygon2D::Update(void)
{
	// 頂点座標の設定
	MakeVertexPos();
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CPolygon2D::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	if (m_bShow)
		// 描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,
			2);
}

//=============================================================================
// ユニークポインタで生成
//=============================================================================
CPolygon2D *CPolygon2D::Create(void)
{
	// メモリ確保
	CPolygon2D *pPolygon = new CPolygon2D;
	// 初期化
	pPolygon->Init();
	// 値を返す
	return pPolygon;
}

//=============================================================================
// サイズの設定
//=============================================================================
void CPolygon2D::SetSize(const D3DXVECTOR3 & size)
{
	m_size = size;

	// ポリゴン回転用の対角線を算出
	m_fAngle = atan2f(m_size.x, m_size.y);
	m_fLength = (float)sqrt(m_size.x * m_size.x + m_size.y * m_size.y) / 2;
}

//=============================================================================
// 色の設定
//=============================================================================
void CPolygon2D::SetCol(const D3DXCOLOR & col)
{
	m_col = col;

	// 頂点情報へのポインタ
	VERTEX_2D *pVtx;

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報(頂点座標)の更新
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	// 頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void CPolygon2D::SetAnim(const D3DXVECTOR2 & UV, const D3DXVECTOR2 & size)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(UV.x, UV.y);
	pVtx[1].tex = D3DXVECTOR2(UV.x + size.x, UV.y);
	pVtx[2].tex = D3DXVECTOR2(UV.x, UV.y + size.y);
	pVtx[3].tex = D3DXVECTOR2(UV.x + size.x, UV.y + size.y);

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void CPolygon2D::MakeVertexPos(void)
{
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_posStart)
	{
	case POSSTART_TOP_LEFT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f,		0.0f, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x, 0.0f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f,		m_size.y, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x, m_size.y, 0.0f);
		break;
	case POSSTART_TOP_CENTRAL:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f,	0.0f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f, m_size.y, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f,	m_size.y, 0.0f);
		break;
	case POSSTART_TOP_RIGHT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x,	0.0f, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(0.0f,			0.0f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x,	m_size.y, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(0.0f,			m_size.y, 0.0f);
		break;
	case POSSTART_CENTRAL_LEFT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f,		-m_size.y * 0.5f, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x, -m_size.y * 0.5f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f,		m_size.y * 0.5f, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x, m_size.y * 0.5f, 0.0f);
		break;
	case POSSTART_CENTRAL_CENTRAL:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f,-m_size.y * 0.5f,0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f, -m_size.y * 0.5f, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f,m_size.y * 0.5f,0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f, m_size.y * 0.5f, 0.0f);
		break;
	case POSSTART_CENTRAL_RIGHT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x,	-m_size.y * 0.5f,0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(0.0f,			-m_size.y * 0.5f,0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x,	m_size.y * 0.5f,0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(0.0f,			m_size.y * 0.5f,0.0f);
		break;
	case POSSTART_BOTTOM_LEFT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(0.0f,			-m_size.y, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x,		-m_size.y, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(0.0f,			0.0f, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x,		0.0f, 0.0f);
		break;
	case POSSTART_BOTTOM_CENTRAL:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f, -m_size.y, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f,	-m_size.y, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x * 0.5f, 0.0f, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(m_size.x * 0.5f,	0.0f, 0.0f);
		break;
	case POSSTART_BOTTOM_RIGHT:
		pVtx[0].pos = m_pos + D3DXVECTOR3(-m_size.x,	-m_size.y, 0.0f);
		pVtx[1].pos = m_pos + D3DXVECTOR3(0.0f,			-m_size.y, 0.0f);
		pVtx[2].pos = m_pos + D3DXVECTOR3(-m_size.x,	0.0f, 0.0f);
		pVtx[3].pos = m_pos + D3DXVECTOR3(0.0f,			0.0f, 0.0f);
		break;
	}

	//頂点データをアンロック
	m_pVtxBuff->Unlock();
}
