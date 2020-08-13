//*************************************************************************************************************
//
// マイライブラリ[Mylibrary.inl]
// Contents : Mylibrary inline functions
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CBMPFILE_INL
#define _CBMPFILE_INL

/*
// struct inline func
// 構造体のインライン関数
*/

//----------------------------------------------------------------------------------------------------
// 3成分float
//----------------------------------------------------------------------------------------------------
inline FLOAT3
FLOAT3::operator +(const FLOAT3 &rhs) const
{
	return FLOAT3(x + rhs.x, y + rhs.y, z + rhs.z);
}

inline FLOAT3
FLOAT3::operator -(const FLOAT3 &rhs) const
{
	return FLOAT3(x - rhs.x, y - rhs.y, z - rhs.z);
}

inline FLOAT3
FLOAT3::operator -(void) const
{
	return FLOAT3(x * -1.0f, y * -1.0f, z * -1.0f);
}

inline FLOAT3
FLOAT3::operator *(const FLOAT3 &r) const
{
	return FLOAT3(x * r.x, y * r.y, z * r.z);
}

inline FLOAT3
FLOAT3::operator /(const FLOAT3 &r) const
{
	return FLOAT3(x / r.x, y / r.y, z / r.z);
}

inline FLOAT3
FLOAT3::operator *(float r) const
{
	return FLOAT3(x * r, y * r, z * r);
}

inline FLOAT3
FLOAT3::operator /(float r) const
{
	return FLOAT3(x / r, y / r, z / r);
}

//inline FLOAT3 &
//FLOAT3::operator=(D3DXVECTOR3 & rhs)
//{
//	FLOAT3(rhs);
//	return *this;
//}

inline float
FLOAT3::Dot(const FLOAT3 &r) const {
	return x * r.x + y * r.y + z * r.z;
}

inline FLOAT3
FLOAT3::Cross(const FLOAT3 &r) const {
	return FLOAT3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
}

inline float
FLOAT3::Length() const {
	return sqrtf(LengthSq());
}

inline float
FLOAT3::LengthSq() const {
	return x * x + y * y + z * z;
}

inline void
FLOAT3::Norm() {
	const float len = Length();
	if (len > 0.0f) {
		x /= len;
		y /= len;
		z /= len;
	}
}

inline FLOAT3
FLOAT3::GetNorm() const {
	const float len = Length();
	if (len > 0.0f) {
		return FLOAT3(x / len, y / len, z / len);
	}
	return FLOAT3(0.0f, 0.0f, 0.0f);
}

//----------------------------------------------------------------------------------------------------
// 3Dベクトル
//----------------------------------------------------------------------------------------------------
inline VEC3& VEC3::operator =(const FLOAT3 &r) {
	x = r.x;
	y = r.y;
	z = r.z;
	return *this;
}

inline void 
VEC3::norm(void) {
	const float len = Length();
	if (len > 0.0f) {
		x /= len;
		y /= len;
		z /= len;
	}
}

inline bool
VEC3::IsVertical(const VEC3 &r) const {
	float DotValue = Dot(r);
	return (-MYLIB_OX_EPSILON < DotValue && DotValue < MYLIB_OX_EPSILON);	// 誤差範囲内なら垂直と判定
}

inline bool
VEC3::IsParallel(const VEC3 &r) const {
	float CrossValue = Cross(r).LengthSq();
	return (-MYLIB_OX_EPSILON < CrossValue && CrossValue < MYLIB_OX_EPSILON);	// 誤差範囲内なら平行と判定
}

inline bool
VEC3::IsSharpAngle(const VEC3 &r) const {
	return (Dot(r) >= 0.0f);
}

//----------------------------------------------------------------------------------------------------
// 直線
//----------------------------------------------------------------------------------------------------
inline FLOAT3
LINE::GetPoint(float fCoffi) const
{
	return Point + fCoffi * Vec;
}

//----------------------------------------------------------------------------------------------------
// 線分
//----------------------------------------------------------------------------------------------------
inline FLOAT3
SEGMENT::GetEndPoint() const
{
	return Point + Vec;
}

//----------------------------------------------------------------------------------------------------
// 3Dの線分情報
//----------------------------------------------------------------------------------------------------
/* * コンストラクタ */
inline
LINESEGMENT3D::LINESEGMENT3D(D3DXVECTOR3 &start, D3DXVECTOR3 &vector)
{
	this->start = start;
	this->vector = vector;
}

/* * 単項演算子 + */
inline LINESEGMENT3D
LINESEGMENT3D::operator+(void) const
{
	return *this;
}

/* * 単項演算子 - */
inline LINESEGMENT3D
LINESEGMENT3D::operator-(void) const
{
	return LINESEGMENT3D(-start, -vector);
}

/* * 二項演算子 + */
inline LINESEGMENT3D
LINESEGMENT3D::operator + (CONST LINESEGMENT3D& rhs) const
{
	return LINESEGMENT3D(this->start + rhs.start, this->vector + rhs.vector);
}
/* * 二項演算子 - */
inline LINESEGMENT3D
LINESEGMENT3D::operator - (CONST LINESEGMENT3D& rhs) const
{
	return LINESEGMENT3D(this->start - rhs.start, this->vector - rhs.vector);
}

/* * 二項演算子 * */
inline LINESEGMENT3D
LINESEGMENT3D::operator * (CONST LINESEGMENT3D& rhs) const
{
	return
		LINESEGMENT3D(
			D3DXVECTOR3(
				this->start.x * rhs.start.x,
				this->start.y * rhs.start.y,
				this->start.z * rhs.start.z),
			D3DXVECTOR3(
				this->vector.x * rhs.vector.x,
				this->vector.y * rhs.vector.y,
				this->vector.z * rhs.vector.z));
}

/* * 二項演算子 / */
inline LINESEGMENT3D
LINESEGMENT3D::operator / (CONST LINESEGMENT3D& rhs) const
{
	return
		LINESEGMENT3D(
			D3DXVECTOR3(
				this->start.x / rhs.start.x,
				this->start.y / rhs.start.y,
				this->start.z / rhs.start.z),
			D3DXVECTOR3(
				this->vector.x / rhs.vector.x,
				this->vector.y / rhs.vector.y,
				this->vector.z / rhs.vector.z));
}

/* * 比較演算子 == */
inline bool 
LINESEGMENT3D::operator == (CONST LINESEGMENT3D& rhs) const
{
	return this->start == rhs.start && this->vector == this->vector;
}

/* * 比較演算子 != */
inline bool
LINESEGMENT3D::operator != (CONST LINESEGMENT3D& rhs) const
{
	return this->start != rhs.start || this->vector != this->vector;
}

/* * 代入演算子 += */
inline LINESEGMENT3D&
LINESEGMENT3D::operator += (CONST LINESEGMENT3D& rhs)
{
	this->start += rhs.start;
	this->vector += rhs.vector;
	return *this;
}

/* * 代入演算子 -= */
inline LINESEGMENT3D&
LINESEGMENT3D::operator -= (CONST LINESEGMENT3D& rhs)
{
	this->start -= rhs.start;
	this->vector -= rhs.vector;
	return *this;
}

/* * 代入演算子 *= */
inline LINESEGMENT3D&
LINESEGMENT3D::operator *= (CONST LINESEGMENT3D& rhs)
{
	this->start.x *= rhs.start.x;
	this->start.y *= rhs.start.y;
	this->start.z *= rhs.start.z;
	this->vector.x *= rhs.vector.x;
	this->vector.y *= rhs.vector.y;
	this->vector.z *= rhs.vector.z;

	return *this;
}

/* * 代入演算子 /= */
inline LINESEGMENT3D&
LINESEGMENT3D::operator /= (CONST LINESEGMENT3D& rhs)
{
	this->start.x /= rhs.start.x;
	this->start.y /= rhs.start.y;
	this->start.z /= rhs.start.z;
	this->vector.x /= rhs.vector.x;
	this->vector.y /= rhs.vector.y;
	this->vector.z /= rhs.vector.z;

	return *this;
}

/*
// class inline func
// クラスのインライン関数
*/
//----------------------------------------------------------------------------------------------------
// class CInputkey 入力キークラス
//----------------------------------------------------------------------------------------------------

/* * 初期化 */
inline void
CInputkey::Init(void)
{
	this->m_nSize = 0;		// サイズ
	this->m_pKeyTop = NULL;		// キーの先頭ポインタ
}

/* * キーデータの取得 */
inline void
CInputkey::KeyInit(void)
{
	this->m_pKeyTop->nData = 0;		// データ
	this->m_pKeyTop->nKey = 0;		// キー
	this->m_pKeyTop->pNext = NULL;	// キーの先頭ポインタ
}

/* * キーデータの生成 */
inline void
CInputkey::KeyCreate(void)
{
	// 生成
	this->m_pKeyTop = new INPUTKEYCELL;
	// 生成失敗
	if (this->m_pKeyTop == NULL)
	{
		return;
	}
	// キーの初期化
	this->KeyInit();
}

/* * キーデータリストの開放 */
inline void
CInputkey::KeyListRelease(void)
{
	// 変数宣言
	INPUTKEYCELL* pTemp;	// 一時保存用
	INPUTKEYCELL* pSwap;	// 交換用

							// 先頭ポインタ
	pTemp = this->m_pKeyTop;
	// 存在している時
	if (pTemp != NULL)
	{
		// リストの開放
		while (pTemp != NULL)
		{
			// 交換用に設定
			pSwap = pTemp->pNext;
			// 保存用を破棄
			delete pTemp;
			// 保存用に設定
			pTemp = pSwap;
		}
	}
	this->m_pKeyTop = NULL;
}

/* * キーデータの挿入 */
inline void
CInputkey::KeyInsert(int nData, int nKey)
{
	// 変数宣言
	INPUTKEYCELL * pInsert;			// 挿入するポインタ
									// 生成
	pInsert = new INPUTKEYCELL;
	// 生成に失敗
	if (pInsert == NULL)
	{
		return;
	}
	// データの設定
	pInsert->nData = nData;
	// キーの設定
	pInsert->nKey = nKey;
	// 挿入する次のポインタの設定
	pInsert->pNext = this->m_pKeyTop;
	// 先頭ポインタの設定
	this->m_pKeyTop = pInsert;
	// サイズをインクリメント
	m_nSize++;
}

/* * キーデータを抜き取る */
inline void
CInputkey::KeyPullout(int nKey)
{
	// 変数宣言
	INPUTKEYCELL *pTarget = NULL;	// ターゲット用ポインタ
	INPUTKEYCELL *pChain = NULL;	// つなぎのポインタ

									// ターゲットの設定
	pTarget = this->m_pKeyTop;
	// ターゲットがいない時
	if (pTarget == NULL)
	{
		return;
	}
	// つなぎのポインタの設定
	pChain = pTarget->pNext;

	// 先頭を抜き取る場合
	if (pTarget->nKey == nKey)
	{
		// サイズをデクリメント
		m_nSize--;
		// つなぎのポインタを先頭にする
		this->m_pKeyTop = pChain;
		// ターゲットを破棄
		delete pTarget;
		pTarget = NULL;
		return;
	}

	// 先頭以外を抜き取る場合
	while (pTarget != NULL)
	{
		// ターゲットのキーと任意のキーが同じとき
		if (pTarget->nKey == nKey)
		{
			// サイズをデクリメント
			m_nSize--;
			// つなぎの次にターゲットの次のポインタを設定
			pChain->pNext = pTarget->pNext;
			// ターゲットの破棄
			delete pTarget;
			pTarget = NULL;
			return;
		}
		// つなぎにターゲットポインタを設定
		pChain = pTarget;
		// ターゲットにターゲットの次を設定
		pTarget = pTarget->pNext;
	}
}

/* * キーデータの取得 */
inline int
CInputkey::GetKeyData(int nKey)
{
	// 変数宣言
	INPUTKEYCELL *pTarget = NULL;	// ターゲットのポインタ
	INPUTKEYCELL *pChain = NULL;	// つなぎのポインタ

									// ターゲットの設定
	pTarget = this->m_pKeyTop;
	// ターゲットがNULLじゃない時ループ
	while (pTarget != NULL)
	{
		// ターゲットのキーと任意のキーが同じとき
		if (pTarget->nKey == nKey)
		{// データを返す
			return pTarget->nData;
		}
		// ターゲットをつなぎに設定
		pChain = pTarget;
		// ターゲットにターゲットに次を設定
		pTarget = pTarget->pNext;
	}
	return -1;
}

/* * サイズの取得 */
inline int
CInputkey::GetSize(void)
{
	return m_nSize;
}

//----------------------------------------------------------------------------------------------------
// class CString 文字列クラス
//----------------------------------------------------------------------------------------------------

/* * コンストラクタ */
inline
CString::CString()
{
	this->Init();
}

/* * 初期化 */
inline void
CString::Init(void)
{
	this->m_string = NULL;				// 文字列ポインタ
	this->m_nStringLength = MYLIB_INT_UNSET;	// 文字列の長さ
}

/* * 開放処理 */
inline void
CString::release(void)
{
	// NULL開放されていない時
	if (this->m_string != NULL)
	{
		// 文字列の破棄
		delete[this->m_nStringLength]this->m_string;
		// 初期化
		this->Init();
	}
}

/* * 文字列の設定（設定していた場合再設定する） */
inline void
CString::Set(STRING string)
{
	// 開放処理
	this->release();
	// 生成
	if (this->m_string == NULL)
	{
		this->m_nStringLength = (int)strlen(string) + 1;
		this->m_string = new char[this->m_nStringLength];
		strcpy(this->m_string, string);
	}
}

/* * 文字列の取得 */
inline STRING
CString::Get(void)
{
	return this->m_string;
}



#endif