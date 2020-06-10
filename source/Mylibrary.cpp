//*************************************************************************************************************
//
// 発光の処理[flash.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Mylibrary.h"
#include "DebugProc.h"

/*
// class CMylibrary func
// ライブラリクラス
// *MEMO*
// [contents] 処理内容または関数名
// [in]       入力の簡単な説明
// [out]      出力の簡単な説明
// [return]   返り値の簡単な説明
*/
//----------------------------------------------------------------------------------------------------
// 2D
//----------------------------------------------------------------------------------------------------

//* [contents] 2Dベクトルを生成する{ B - A }
//* [in] 3DposA、3DposB
//* [out] 生成した2Dベクトル
void CMylibrary::CreateVector2(D3DXVECTOR2 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB)
{
	*output = D3DXVECTOR2(posB->x - posA->x, posB->y - posA->y);
}

//* [contents] 2Dベクトルを生成する{ B - A }
//* [in] 2DposA、2DposB
//* [out] 生成した2Dベクトル
void CMylibrary::CreateVector2(D3DXVECTOR2 * output, CONST D3DXVECTOR2 * posA, CONST D3DXVECTOR2 * posB)
{
	*output = *posB - *posA;
}

//* [contents] 2Dベクトルを生成する{ B - A }
//* [in] 3DposA、2DposB
//* [out] 生成した2Dベクトル
void CMylibrary::CreateVector2(D3DXVECTOR2 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR2 * posB)
{
	*output = D3DXVECTOR2(posB->x - posA->x, posB->y - posA->y);
}

//* [contents] 2Dベクトルを生成する{ B - A }
//* [in] 2DposA、3DposB
//* [out] 生成した2Dベクトル
void CMylibrary::CreateVector2(D3DXVECTOR2 * output, CONST D3DXVECTOR2 * posA, CONST D3DXVECTOR3 * posB)
{
	*output = D3DXVECTOR2(posB->x - posA->x, posB->y - posA->y);
}

//* [contents] 2Dの外積を生成する
//* [in] 3DvecA、3DvecB
//* [return] 生成した2D外積
float CMylibrary::Create2DCross(CONST D3DXVECTOR3 *vecA, CONST D3DXVECTOR3 *vecB)
{
	return vecA->x*vecB->y - vecA->y*vecB->x;
}

//* [contents] 2Dの外積を生成する
//* [in] 2DvecA、2DvecB
//* [return] 生成した2D外積
float CMylibrary::Create2DCross(CONST D3DXVECTOR2 *vecA, CONST D3DXVECTOR2 *vecB)
{
	return vecA->x*vecB->y - vecA->y*vecB->x;
}

//* [contents] 2Dの内積を生成する
//* [in] 3DvecA、3DvecB
//* [return] 生成した2D外積
float CMylibrary::Create2DDot(CONST D3DXVECTOR3* vecA, CONST D3DXVECTOR3* vecB)
{
	return vecA->x* vecB->x + vecA->y*vecB->y;
}

//* [contents] 2Dの内積を生成する
//* [in] 2DvecA、2DvecB
//* [return] 生成した2D外積
float CMylibrary::Create2DDot(CONST D3DXVECTOR2* vecA, CONST D3DXVECTOR2* vecB)
{
	return vecA->x* vecB->x + vecA->y*vecB->y;
}

//* [contents] 2Dのベクトルの長さを求める
//* [in] 2Dvec
//* [return] ベクトルの長さ
float CMylibrary::Get2DVecLength(D3DXVECTOR2 *vec)
{
	return sqrtf(vec->x*vec->x + vec->y*vec->y);
}

//* [contents] 2Dの差分の距離
//* [in] 3D差分
//* [return] 計算した距離
float CMylibrary::Get2DToPointDistance(D3DXVECTOR2 *diffPos)
{
	return sqrtf(diffPos->x*diffPos->x + diffPos->y*diffPos->y);
}

//* [contents] 2Dの単位ベクトルを生成
//* [in] 2Dvec
//* [out] 生成した単位ベクトル
void CMylibrary::CreateUnitVec2D(D3DXVECTOR2 * Output, D3DXVECTOR2 *Vec)
{
	float fLength = CMylibrary::Get2DVecLength(Vec);
	D3DXVECTOR2 Unit =
	{
		Vec->x / fLength,
		Vec->y / fLength
	};
	*Output = Unit;
}

//* [contents] 2Dの差分の角度
//* [in] 3DposA, 3DposB
//* [out] 計算した角度
void CMylibrary::Get2DToPointAngle(float * pOutput, D3DXVECTOR3 * posA, D3DXVECTOR3 * posB)
{
	*pOutput = atan2f((posA->x - posB->x), (posA->y - posB->y));
}

//* [contents] 2Dの差分の角度
//* [in] 3D差分
//* [out] 計算した角度
void CMylibrary::Get2DToPointAngle(float * pOutput, D3DXVECTOR3* diffPos)
{
	*pOutput = atan2f(diffPos->x, diffPos->y);
}

//* [contents] 2D内積を用いて直線同士の交点を求める
//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D
//* [out] 交点
//* [return] bool （true 交わっている、false 交わっていない）
bool CMylibrary::Get2DIntersectionOfStraightLines(D3DXVECTOR3 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB, CONST D3DXVECTOR3 * posC, CONST D3DXVECTOR3 * posD)
{
	// ベクトルの生成
	// プレイヤーの進行ベクトル
	D3DXVECTOR3 vecA_B;
	CMylibrary::CreateVector3(&vecA_B, posA, posB);

	// ベクトルが生成できなければ処理しない
	if (CMylibrary::Get3DToDiffDistance(&vecA_B) == 0.0f) {
		return false;
	}

	{// 交差しているかどうか
	 // 変数宣言
		D3DXVECTOR2 vecC_D2D;// C->Dのベクトル
		D3DXVECTOR2 vecC_A2D;// C->Aのベクトル
		D3DXVECTOR2 vecC_B2D;// C->Bのベクトル
		CMylibrary::CreateVector2(&vecC_D2D, posC, posD);
		CMylibrary::CreateVector2(&vecC_A2D, posC, posA);
		CMylibrary::CreateVector2(&vecC_B2D, posC, posB);

		float fCrossValue;												// 外積値
																		//外積で表裏の判定を取る
		fCrossValue = CMylibrary::Create2DCross(&vecC_D2D, &vecC_A2D);
		if (fCrossValue < 0)
		{// 裏に居なかった
			return false;
		}
		fCrossValue = CMylibrary::Create2DCross(&vecC_D2D, &vecC_B2D);
		if (fCrossValue > 5000.0f)
		{// 表に居なかったとき
			return false;
		}
	}

	// ブロック一辺のベクトル生成
	D3DXVECTOR3 vecC_D;
	CMylibrary::CreateVector3(&vecC_D, posC, posD);

	// 正規化したベクトル情報
	D3DXVECTOR3 NomA_B, NomC_D;
	// ベクトルの正規化
	CMylibrary::CreateUnitVector(&NomA_B, &vecA_B);
	CMylibrary::CreateUnitVector(&NomC_D, &vecC_D);

	float fWork1 = CMylibrary::Create3DDot(&NomA_B, &NomC_D);
	float fWork2 = 1 - fWork1 * fWork1;

	// 平行だと0.0fになる
	if (fWork2 == 0.0f) {
		return false;
	}

	// A～Cのベクトルを求める
	D3DXVECTOR3 vecA_C;
	CMylibrary::CreateVector3(&vecA_C, posA, posC);
	float fD1 = (CMylibrary::Create3DDot(&vecA_C, &NomA_B) - fWork1*CMylibrary::Create3DDot(&vecA_C, &NomC_D)) / fWork2;
	float fD2 = (fWork1*CMylibrary::Create3DDot(&vecA_C, &NomA_B) - CMylibrary::Create3DDot(&vecA_C, &NomC_D)) / fWork2;

	D3DXVECTOR3 Result0 =
	{ // AB上の最近点
		posA->x + fD1*NomA_B.x,
		posA->y + fD1*NomA_B.y,
		posA->z + fD1*NomA_B.z,
	};

	D3DXVECTOR3 Result1 =
	{ // BC上の最近点
		posC->x + fD2*NomC_D.x,
		posC->y + fD2*NomC_D.y,
		posC->z + fD2*NomC_D.z,
	};

	if (CMylibrary::OneSideDotRelationship(Result1, *posC, *posD))
	{
		*output = Result0;
		return true;
	}
	return false;
}

//* [contents] 2D外積を用いて線分同士の交点を求める
//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D, 法線
//* [out] 交点
//* [return] bool （true 交わっている、false 交わっていない）
bool CMylibrary::Get2DIntersectionOfLineSegments(D3DXVECTOR3 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB, CONST D3DXVECTOR3 * posC, CONST D3DXVECTOR3 * posD)
{
	D3DXVECTOR3 vecA_C = *posC - *posA;
	D3DXVECTOR3 vecA_B = *posB - *posA;
	D3DXVECTOR3 vecC_D = *posD - *posC;

	float fCrosvecA_BvecC_D = Create2DCross(&vecA_B, &vecC_D);
	if (fCrosvecA_BvecC_D == 0.0f)
	{// 平行だったら処理しない
		return false;
	}

	float fCrosvecA_CvecA_B = Create2DCross(&vecA_C, &vecA_B);
	float fCrosvecA_CvecC_D = Create2DCross(&vecA_C, &vecC_D);

	float fT1 = fCrosvecA_CvecC_D / fCrosvecA_BvecC_D;
	float fT2 = fCrosvecA_CvecA_B / fCrosvecA_BvecC_D;

	const float eps0 = 0.002f;
	const float eps1 = 0.00000002f;
	if (fT1 + eps0 < 0 || fT1 - eps0 > 1 ||
		fT2 + eps1 < 0 || fT2 - eps1 > 1) {
		// 交差していない
		return false;
	}
	if (output != NULL)
	{
		*output = vecA_B * (fT1);
	}

	return true;
}

//* [contents] 2D直線同士が交差しているか求める
//* [in] 頂点位置A, 頂点位置B, 頂点位置C, 頂点位置D
//* [out] 交点
//* [return] bool （true 交わっている、false 交わっていない）
bool CMylibrary::Get2DIntersectionOfStraightLineSegments(D3DXVECTOR3 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB, CONST D3DXVECTOR3 * posC, CONST D3DXVECTOR3 * posD)
{
	// ベクトルの生成
	// プレイヤーの進行ベクトル
	D3DXVECTOR3 vecA_B;
	CMylibrary::CreateVector3(&vecA_B, posA, posB);

	// ベクトルが生成できなければ処理しない
	if (CMylibrary::Get3DToDiffDistance(&vecA_B) == 0.0f) {
		return false;
	}

	{// 交差しているかどうか
	 // 変数宣言
		D3DXVECTOR2 vecC_D2D;	// C->Dのベクトル
		D3DXVECTOR2 vecC_A2D;	// C->Aのベクトル
		D3DXVECTOR2 vecC_B2D;	// C->Bのベクトル
		CMylibrary::CreateVector2(&vecC_D2D, posC, posD);
		CMylibrary::CreateVector2(&vecC_A2D, posC, posA);
		CMylibrary::CreateVector2(&vecC_B2D, posC, posB);


		float fCrossValue;												// 外積値
																		//外積で表裏の判定を取る
		fCrossValue = CMylibrary::Create2DCross(&vecC_D2D, &vecC_A2D);
		if (fCrossValue < 0)
		{// 裏に居なかった
			return false;
		}
		fCrossValue = CMylibrary::Create2DCross(&vecC_D2D, &vecC_B2D);
		if (fCrossValue > 5000.0f)
		{// 表に居なかったとき
			return false;
		}
		if (CMylibrary::OneSideDotRelationship(*posA, *posC, *posD))
		{
			return true;
		}

	}

	return false;

}

//* [contents] 2D四角形の当たり判定
//* [in] 位置A,位置B, 大きさA, 大きさB
//* [return] bool （true 接触している、false 接触していない）
bool CMylibrary::Test2DCollisionRect(D3DXVECTOR3 * posA, D3DXVECTOR3 * posB, D3DXVECTOR2 * sizeA, D3DXVECTOR2 * sizeB)
{
	if (sizeA != NULL&&sizeB != NULL)
	{
		if (posA->x - sizeA->x < posB->x - sizeB->x&&
			posA->x + sizeA->x > posB->x + sizeB->x&&
			posA->y - sizeA->y < posB->y - sizeB->y&&
			posA->y + sizeA->y > posB->y + sizeB->y)
		{
			return true;
		}
	}
	if (sizeA != NULL)
	{
		if (posA->x - sizeA->x < posB->x&&
			posA->x + sizeA->x > posB->x&&
			posA->y - sizeA->y < posB->y&&
			posA->y + sizeA->y > posB->y)
		{
			return true;
		}
	}
	else if (sizeB != NULL)
	{
		if (posB->x - sizeB->x < posA->x&&
			posB->x + sizeB->x > posA->x&&
			posB->y - sizeB->y < posA->y&&
			posB->y + sizeB->y > posA->y)
		{
			return true;
		}
	}
	return false;
}

//* [contents] 頂点位置の設定
//* [in] 原点タイプ, 位置, 大きさ, 半径, 回転量
//* [out] 頂点位置
void CMylibrary::SetVatexPostion(D3DXVECTOR3 ** ppOut, ORIGINVERTEXTYPE type, D3DXVECTOR3 * pPos, D3DXVECTOR2 * pSize, float fRadius, float fRotation)
{
	switch (type)
	{
	case ORIGINVERTEXTYPE_LOWERLEFT:
		*ppOut[0] = D3DXVECTOR3(pPos->x, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x + pSize->x, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x + pSize->x, pPos->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_LOWERCENTER:
		*ppOut[0] = D3DXVECTOR3(pPos->x - pSize->x*MYLIB_HALF_SIZE, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x + pSize->x*MYLIB_HALF_SIZE, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x - pSize->x*MYLIB_HALF_SIZE, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x + pSize->x*MYLIB_HALF_SIZE, pPos->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_LOWERRIGHT:
		*ppOut[0] = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x, pPos->y - pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x - pSize->x, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x, pPos->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTERLEFT:
		*ppOut[0] = D3DXVECTOR3(pPos->x, pPos->y - pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x + pSize->x, pPos->y - pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x, pPos->y + pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTER:
		*ppOut[0] = D3DXVECTOR3(pPos->x - pSize->x*MYLIB_HALF_SIZE, pPos->y - pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x + pSize->x*MYLIB_HALF_SIZE, pPos->y - pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x - pSize->x*MYLIB_HALF_SIZE, pPos->y + pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x + pSize->x*MYLIB_HALF_SIZE, pPos->y + pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_CENTERRIGHT:
		*ppOut[0] = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x, pPos->y - pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x - pSize->x, pPos->y - pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x, pPos->y - pSize->y*MYLIB_HALF_SIZE, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERLEFT:
		*ppOut[0] = D3DXVECTOR3(pPos->x, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x + pSize->x, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERCENTER:
		*ppOut[0] = D3DXVECTOR3(pPos->x - pSize->x, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x + pSize->x, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x - pSize->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x + pSize->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_UPPERRIGHT:
		*ppOut[0] = D3DXVECTOR3(pPos->x - pSize->x, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[1] = D3DXVECTOR3(pPos->x, pPos->y, MYLIB_FLOAT_UNSET);
		*ppOut[2] = D3DXVECTOR3(pPos->x - pSize->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		*ppOut[3] = D3DXVECTOR3(pPos->x, pPos->y + pSize->y, MYLIB_FLOAT_UNSET);
		break;
	case ORIGINVERTEXTYPE_ROTATION:
		// 頂点の設定
		ppOut[0]->x = pPos->x + sinf(D3DX_PI*-MYLIB_THREEQUARTERS_SIZE)*fRadius;
		ppOut[0]->y = pPos->y + cosf(D3DX_PI*-MYLIB_THREEQUARTERS_SIZE)*fRadius;
		ppOut[0]->z = MYLIB_FLOAT_UNSET;
		ppOut[1]->x = pPos->x + sinf(D3DX_PI*MYLIB_THREEQUARTERS_SIZE)*fRadius;
		ppOut[1]->y = pPos->y + cosf(D3DX_PI*MYLIB_THREEQUARTERS_SIZE)*fRadius;
		ppOut[1]->z = MYLIB_FLOAT_UNSET;
		ppOut[2]->x = pPos->x + sinf(D3DX_PI*-MYLIB_ONEQUARTER_SIZE)*fRadius;
		ppOut[2]->y = pPos->y + cosf(D3DX_PI*-MYLIB_ONEQUARTER_SIZE)*fRadius;
		ppOut[2]->z = MYLIB_FLOAT_UNSET;
		ppOut[3]->x = pPos->x + sinf(D3DX_PI*MYLIB_ONEQUARTER_SIZE)*fRadius;
		ppOut[3]->y = pPos->y + cosf(D3DX_PI*MYLIB_ONEQUARTER_SIZE)*fRadius;
		ppOut[3]->z = MYLIB_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTCENTER:
		ppOut[0]->x = pPos->x + sinf(D3DX_PI + fRotation)*pSize->y + sinf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[0]->y = pPos->y + cosf(D3DX_PI + fRotation)*pSize->y + cosf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[0]->z = MYLIB_FLOAT_UNSET;
		ppOut[1]->x = pPos->x + sinf(D3DX_PI + fRotation)*pSize->y + sinf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[1]->y = pPos->y + cosf(D3DX_PI + fRotation)*pSize->y + cosf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[1]->z = MYLIB_FLOAT_UNSET;
		ppOut[2]->x = pPos->x + sinf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y + sinf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[2]->y = pPos->y + cosf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y + cosf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[2]->z = MYLIB_FLOAT_UNSET;
		ppOut[3]->x = pPos->x + sinf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y + sinf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[3]->y = pPos->y + cosf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y + cosf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[3]->z = MYLIB_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTCENTERLEFT:
		ppOut[0]->x = pPos->x + sinf(D3DX_PI + fRotation)*pSize->y;
		ppOut[0]->y = pPos->y + cosf(D3DX_PI + fRotation)*pSize->y;
		ppOut[0]->z = MYLIB_FLOAT_UNSET;
		ppOut[1]->x = pPos->x + sinf(D3DX_PI + fRotation)*pSize->y + sinf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*(pSize->x*2.0f);
		ppOut[1]->y = pPos->y + cosf(D3DX_PI + fRotation)*pSize->y + cosf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*(pSize->x*2.0f);
		ppOut[1]->z = MYLIB_FLOAT_UNSET;
		ppOut[2]->x = pPos->x + sinf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y;
		ppOut[2]->y = pPos->y + cosf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y;
		ppOut[2]->z = MYLIB_FLOAT_UNSET;
		ppOut[3]->x = pPos->x + sinf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y + sinf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*(pSize->x*2.0f);
		ppOut[3]->y = pPos->y + cosf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y + cosf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*(pSize->x*2.0f);
		ppOut[3]->z = MYLIB_FLOAT_UNSET;
		break;

	case ORIGINVERTEXTYPE_ROTLOWERCENTER:
		ppOut[0]->x = pPos->x + sinf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x + sinf(D3DX_PI + fRotation)*(pSize->y*2.0f);
		ppOut[0]->y = pPos->y + cosf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x + cosf(D3DX_PI + fRotation)*(pSize->y*2.0f);
		ppOut[0]->z = MYLIB_FLOAT_UNSET;
		ppOut[1]->x = pPos->x + sinf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x + sinf(D3DX_PI + fRotation)*(pSize->y*2.0f);
		ppOut[1]->y = pPos->y + cosf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x + cosf(D3DX_PI + fRotation)*(pSize->y*2.0f);
		ppOut[1]->z = MYLIB_FLOAT_UNSET;
		ppOut[2]->x = pPos->x + sinf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[2]->y = pPos->y + cosf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[2]->z = MYLIB_FLOAT_UNSET;
		ppOut[3]->x = pPos->x + sinf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[3]->y = pPos->y + cosf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[3]->z = MYLIB_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTCENTERRIGHT:
		ppOut[0]->x = pPos->x + sinf(D3DX_PI + fRotation)*pSize->y + sinf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*(pSize->x)*2.0f;
		ppOut[0]->y = pPos->y + cosf(D3DX_PI + fRotation)*pSize->y + cosf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*(pSize->x)*2.0f;
		ppOut[0]->z = MYLIB_FLOAT_UNSET;
		ppOut[1]->x = pPos->x + sinf(D3DX_PI + fRotation)*pSize->y;
		ppOut[1]->y = pPos->y + cosf(D3DX_PI + fRotation)*pSize->y;
		ppOut[1]->z = MYLIB_FLOAT_UNSET;
		ppOut[2]->x = pPos->x + sinf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y + sinf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*(pSize->x*2.0f);
		ppOut[2]->y = pPos->y + cosf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y + cosf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*(pSize->x*2.0f);
		ppOut[2]->z = MYLIB_FLOAT_UNSET;
		ppOut[3]->x = pPos->x + sinf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y;
		ppOut[3]->y = pPos->y + cosf(MYLIB_FLOAT_UNSET + fRotation)*pSize->y;
		ppOut[3]->z = MYLIB_FLOAT_UNSET;
		break;
	case ORIGINVERTEXTYPE_ROTUPPERCENTER:
		ppOut[0]->x = pPos->x + sinf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[0]->y = pPos->y + cosf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[0]->z = MYLIB_FLOAT_UNSET;
		ppOut[1]->x = pPos->x + sinf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[1]->y = pPos->y + cosf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x;
		ppOut[1]->z = MYLIB_FLOAT_UNSET;
		ppOut[2]->x = pPos->x + sinf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x + sinf(MYLIB_FLOAT_UNSET + fRotation)*(pSize->y*2.0f);;
		ppOut[2]->y = pPos->y + cosf(D3DX_PI*-MYLIB_HALF_SIZE + fRotation)*pSize->x + cosf(MYLIB_FLOAT_UNSET + fRotation)*(pSize->y*2.0f);;
		ppOut[2]->z = MYLIB_FLOAT_UNSET;
		ppOut[3]->x = pPos->x + sinf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x + sinf(MYLIB_FLOAT_UNSET + fRotation)*(pSize->y*2.0f);;
		ppOut[3]->y = pPos->y + cosf(D3DX_PI*MYLIB_HALF_SIZE + fRotation)*pSize->x + cosf(MYLIB_FLOAT_UNSET + fRotation)*(pSize->y*2.0f);;
		ppOut[3]->z = MYLIB_FLOAT_UNSET;
		break;
	}
}

//----------------------------------------------------------------------------------------------------
// 3D
//----------------------------------------------------------------------------------------------------

//* [contents] 3Dベクトルを生成する
//* [in] 3DposA、3DposB
//* [out] 生成した3Dベクトル
void CMylibrary::CreateVector3(D3DXVECTOR3 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB)
{
	*output = *posB - *posA;
}

//* [contents] 3Dベクトルを生成する
//* [in] 2DposA、2DposB
//* [out] 生成した3Dベクトル
void CMylibrary::CreateVector3(D3DXVECTOR3 * output, CONST D3DXVECTOR2 * posA, CONST D3DXVECTOR2 * posB)
{
	*output = D3DXVECTOR3(posB->x - posA->x, posB->y - posA->y, 0.0f);
}

//* [contents] 3Dベクトルを生成する
//* [in] 2DposA、3DposB
//* [out] 生成した3Dベクトル
void  CMylibrary::CreateVector3(D3DXVECTOR3 * output, CONST D3DXVECTOR2 * posA, CONST D3DXVECTOR3 * posB)
{
	*output = D3DXVECTOR3(posB->x - posA->x, posB->y - posA->y, posB->z - 0.0f);
}

//* [contents] 3Dベクトルを生成する
//* [in] 3DposA、2DposB
//* [out] 生成した3Dベクトル
void CMylibrary::CreateVector3(D3DXVECTOR3 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR2 * posB)
{
	*output = D3DXVECTOR3(posB->x - posA->x, posB->y - posA->y, 0.0f - posA->z);
}

//* [contents] 3Dの外積を生成する
//* [in] 3DvecA、3DvecB
//* [out] 生成した3D外積
void CMylibrary::Create3DCross(D3DXVECTOR3 * resulut, D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB)
{
	*resulut = D3DXVECTOR3(vecA->y*vecB->z - vecA->z*vecB->y, vecA->z*vecB->x - vecA->x*vecB->z, vecA->x*vecB->y - vecA->y*vecB->x);
}

//* [contents] 3Dの平面外積を生成する
//* [in] 3DvecA、3DvecB
//* [return] 生成した平面3D外積
float CMylibrary::CreateflatCross(CONST D3DXVECTOR3 *vecA, CONST D3DXVECTOR3 *vecB)
{
	return vecA->x*vecB->z - vecA->z*vecB->x;
}

//* [contents] 3Dの内積を生成する
//* [in] 3DvecA、3DvecB
//* [return] 生成した3D内積
float CMylibrary::Create3DDot(D3DXVECTOR3 * vecA, D3DXVECTOR3 * vecB)
{
	return vecA->x* vecB->x + vecA->y*vecB->y + vecA->z*vecB->z;
}

//* [contents] 3Dの内積を生成する
//* [in] 3DvecA、3DvecB
//* [out] 生成した3D内積
void CMylibrary::Create3DDot(float * resulut, D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB)
{
	*resulut = CMylibrary::Create3DDot(vecA, vecB);
}

//* [contents] 3Dの2D上のベクトルの長さを求める
//* [in] 3DvecA、3DvecB
//* [out] 生成した3D内積
float CMylibrary::GetFlatVecLength(D3DXVECTOR3 * vec)
{
	return sqrtf(vec->x*vec->x + vec->z*vec->z) *0.5f;
}

//* [contents] 3Dの2D上のベクトルの長さを求める
//* [in] 3Dvec
//* [out] 生成したベクトルの長さ
float CMylibrary::Get3DVecLength(D3DXVECTOR3 * vec)
{
	return sqrtf(vec->x*vec->x + vec->y*vec->y + vec->z*vec->z);
}

//* [contents] 3Dの2点のベクトルの長さを求める
//* [in] 3DvecA、3DvecB
//* [return] 生成したベクトルの長さ
float CMylibrary::Get3DClossLength(D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB)
{
	D3DXVECTOR3 CrossVec;
	Create3DCross(&CrossVec, vecA, vecB);
	return CMylibrary::Get3DVecLength(&CrossVec);
}

//* [contents] 3Dの2点のベクトルの長さを求める
//* [in] 3Dvec
//* [out] 生成したベクトルの長さ
void CMylibrary::Get3DVecLength(float * resulut, D3DXVECTOR3 *vec)
{
	*resulut = sqrtf(vec->x*vec->x + vec->y*vec->y + vec->z*vec->z);
}

//* [contents] 3Dの2点の外積の長さを求める
//* [in] 3DposA、3DposB
//* [out] 生成したベクトルの長さ
void CMylibrary::Get3DClossLength(float * resulut, D3DXVECTOR3 *posA, D3DXVECTOR3 *posB)
{
	D3DXVECTOR3 CrossVec;
	Create3DCross(&CrossVec, posA, posB);
	float fLength = CMylibrary::Get3DVecLength(&CrossVec);
	*resulut = fLength;
}

//* [contents] 3Dの単位ベクトルを生成
//* [in] 3Dvec
//* [out] 生成した単位ベクトル
void CMylibrary::CreateUnitVector(D3DXVECTOR3 * Output, D3DXVECTOR3 *Vec)
{
	float fLength = CMylibrary::Get3DVecLength(Vec);
	D3DXVECTOR3 Unit =
	{
		Vec->x / fLength,
		Vec->y / fLength,
		Vec->z / fLength
	};
	*Output = Unit;
}

//* [contents] 3Dの頂点間の距離の取得
//* [in] 3DposA、3DposB
//* [return] 計算した距離
float CMylibrary::Get3DToPointDistance(CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB)
{
	return sqrtf(
		(posB->x - posA->x) * (posB->x - posA->x) +
		(posB->y - posA->y) * (posB->y - posA->y) +
		(posB->z - posA->z) * (posB->z - posA->z));
}

//* [contents] 3Dの差分の距離
//* [in] 3D差分
//* [return] 計算した距離
float CMylibrary::Get3DToDiffDistance(D3DXVECTOR3 *diffPos)
{
	return sqrtf(
		diffPos->x*diffPos->x +
		diffPos->y*diffPos->y +
		diffPos->z*diffPos->z);
}

//* [contents] 3Dの差分の距離
//* [in] 3D差分
//* [out] 計算した距離
void CMylibrary::Get3DToPointDistance(float *Length, D3DXVECTOR3 *diffPos)
{
	*Length = sqrtf(diffPos->x*diffPos->x + diffPos->y*diffPos->y + diffPos->z*diffPos->z);
}

//* [contents] ポリゴンの範囲にいるかテスト
//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P)
//* [return] bool （範囲内 == true 範囲外 == false）
bool CMylibrary::Test3DInsidePolygon(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP)
{
	// 変数宣言
	D3DXVECTOR3 vecBP;			// BP辺
	D3DXVECTOR3 vecCP;			// CP辺
	D3DXVECTOR3 vecAP;			// AP辺
	D3DXVECTOR3 CrossResult1;	// 外積の計算結果
	D3DXVECTOR3 CrossResult2;	// 外積の計算結果
	D3DXVECTOR3 CrossResult3;	// 外積の計算結果
	// ベクトルの生成
	vecBP = posP - posB;		// BP辺
	vecCP = posP - posC;		// CP辺
	vecAP = posP - posA;		// AP辺

	// 外積計算
	// 変数定義
	D3DXVECTOR3 vecAB;		// AB辺
	D3DXVECTOR3 vecBC;		// BC辺
	D3DXVECTOR3 vecCA;		// CA辺
	// ベクトルの生成
	vecAB = posB - posA;	// AB辺
	vecBC = posC - posB;	// BC辺
	vecCA = posA - posC;	// CA辺

	// 外積の計算
	D3DXVec3Cross(&CrossResult1, &vecAB, &vecBP);
	D3DXVec3Cross(&CrossResult2, &vecBC, &vecCP);
	D3DXVec3Cross(&CrossResult3, &vecCA, &vecAP);
	if ((CrossResult1.y >= 0 && CrossResult2.y >= 0 && CrossResult3.y >= 0))
	{
		return true;
	}
	else
	{
		return false;
	}

}

//* [contents] ポリゴンの範囲にいるかテスト
//* [in] 頂点位置A, 頂点位置B, 頂点位置C , 位置P(点P), ポリゴンの法線
//* [return] bool （範囲内 == true 範囲外 == false）
bool CMylibrary::Test3DInsidePolygon(CONST D3DXVECTOR3 & posA, CONST D3DXVECTOR3 & posB, CONST D3DXVECTOR3 & posC, CONST D3DXVECTOR3 & posP, CONST D3DXVECTOR3 & nom)
{
#if 0
	// 変数宣言
	D3DXVECTOR3 vecBP;			// BP辺
	D3DXVECTOR3 vecCP;			// CP辺
	D3DXVECTOR3 vecAP;			// AP辺
	D3DXVECTOR3 CrossResult1;	// 外積の計算結果
	D3DXVECTOR3 CrossResult2;	// 外積の計算結果
	D3DXVECTOR3 CrossResult3;	// 外積の計算結果
	// ベクトルの生成
	vecBP = posP - posB;		// BP辺
	vecCP = posP - posC;		// CP辺
	vecAP = posP - posA;		// AP辺

	// 外積計算
	 // 変数定義
	D3DXVECTOR3 vecAB;		// AB辺
	D3DXVECTOR3 vecBC;		// BC辺
	D3DXVECTOR3 vecCA;		// CA辺
	// ベクトルの生成
	vecAB = posB - posA;	// AB辺
	vecBC = posC - posB;	// BC辺
	vecCA = posA - posC;	// CA辺

	// 外積の計算
	D3DXVec3Cross(&CrossResult1, &vecAB, &vecBP);
	D3DXVec3Cross(&CrossResult2, &vecBC, &vecCP);
	D3DXVec3Cross(&CrossResult3, &vecCA, &vecAP);
	//　単位ベクトルの作成
	D3DXVec3Normalize(&CrossResult1, &CrossResult1);
	D3DXVec3Normalize(&CrossResult2, &CrossResult2);
	D3DXVec3Normalize(&CrossResult3, &CrossResult3);

	// 外積の計算結果でポリゴン内外を判定
	if ((CrossResult1.y >= 0.0f && CrossResult2.y >= 0.0f && CrossResult3.y >= 0.0f))
	{
		// 変数定義
		float fDotResult1;	// 内積の結果

		// 内積
		fDotResult1 = D3DXVec3Dot(&nom, &vecBP);

		// 内積結果で表裏判定
		if (fDotResult1 <= 0.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
#else
	// 変数宣言
	D3DXVECTOR3 vecBP;			// BP辺
	D3DXVECTOR3 vecAB;			// AB辺
	D3DXVECTOR3 CrossResult1;	// 外積の計算結果
	vecBP = posP - posB;		// BP辺
	vecAB = posB - posA;		// AB辺
	// 外積計算
	D3DXVec3Cross(&CrossResult1, &vecAB, &vecBP);
	// 単位ベクトルの生成
	D3DXVec3Normalize(&CrossResult1, &CrossResult1);
	if (CrossResult1.y < 0.0f)
	{
		return false;
	}

	// 変数宣言
	D3DXVECTOR3 vecCP;			// CP辺
	D3DXVECTOR3 vecBC;			// BC辺
	D3DXVECTOR3 CrossResult2;	// 外積の計算結果
	vecCP = posP - posC;		// CP辺
	vecBC = posC - posB;		// BC辺
	// 外積計算
	D3DXVec3Cross(&CrossResult2, &vecBC, &vecCP);
	// 単位ベクトルの生成
	D3DXVec3Normalize(&CrossResult2, &CrossResult2);
	if (CrossResult2.y < 0.0f)
	{
		return false;
	}

	// 変数宣言
	D3DXVECTOR3 vecAP;			// AP辺
	D3DXVECTOR3 vecCA;			// CA辺
	D3DXVECTOR3 CrossResult3;	// 外積の計算結果
	vecAP = posP - posA;		// AP辺
	vecCA = posA - posC;		// CA辺
	// 外積計算
	D3DXVec3Cross(&CrossResult3, &vecCA, &vecAP);
	// 単位ベクトルの生成
	D3DXVec3Normalize(&CrossResult3, &CrossResult3);
	if (CrossResult3.y < 0.0f)
	{
		return false;
	}

	// 変数宣言
	float fDotResult;	// 内積の結果
	// 内積計算
	fDotResult = D3DXVec3Dot(&nom, &vecBP);
	// 内積結果で表裏判定
	if (fDotResult <= 0.0f)
	{
		return true;
	}
	else
	{
		return false;
	}

#endif
}

//* [contents] 直線同士の距離と最短点座標の取得
//* [in] 位置A , ベクトルA, 位置B, ベクトルB
//* [out] 距離, 最短点座標A, 最短点座標B
bool CMylibrary::Calc2LineNearestDistAndPos(
	D3DXVECTOR3 * posA, D3DXVECTOR3 * vecA, D3DXVECTOR3 * posB, D3DXVECTOR3 * vecB,
	float * fOutDist, D3DXVECTOR3 * pOutPosA, D3DXVECTOR3 * pOutPosB)
{
	// 変数宣言
	D3DXVECTOR3 &pPos1 = *posA;
	D3DXVECTOR3 &pPos2 = *posB;
	D3DXVECTOR3 nom1;
	D3DXVECTOR3 nom2;

	D3DXVec3Normalize(&nom1, vecA);
	D3DXVec3Normalize(&nom2, vecB);

	float Dot1 = D3DXVec3Dot(&(pPos2 - pPos1), &nom1);
	float Dot2 = D3DXVec3Dot(&(pPos2 - pPos1), &nom2);

	{// 計算できるか判定
		D3DXVECTOR3 cross;
		float fVecsqLength = D3DXVec3LengthSq(D3DXVec3Cross(&cross, &nom1, &nom2));

		if (fVecsqLength < 0.0f)
		{// 結果がマイナスの時(誤差分)
			if (fOutDist != NULL)
			{
				D3DXVECTOR3 work;
				*fOutDist = D3DXVec3Length(D3DXVec3Cross(&work, &(pPos2 - pPos1), &nom1));
			}
			return false;
		}
	}

	float Dot1_2 = D3DXVec3Dot(&nom1, &nom2);
	float t1 = (Dot1 - Dot2 * Dot1_2) / (1.0f - Dot1_2 * Dot1_2);
	float t2 = (Dot2 - Dot1 * Dot1_2) / (Dot1_2 * Dot1_2 - 1.0f);

	D3DXVECTOR3
		Q1 = pPos1 + t1 * nom1,
		Q2 = pPos2 + t2 * nom2;

	if (fOutDist != NULL)
	{
		*fOutDist = D3DXVec3Length(&(Q2 - Q1));
	}

	if (pOutPosA != NULL)
	{
		*pOutPosA = Q1;
	}

	if (pOutPosB != NULL)
	{
		*pOutPosB = Q2;
	}
	return true;
}


//* [contents] 外積で内外位置関係を調べる
//* [in] 位置P(点P), 頂点位置A, 頂点位置B
//* [return] bool （内 == true 外 == false）
bool CMylibrary::CrossRelationship(D3DXVECTOR3 &posP, D3DXVECTOR3 &posA, D3DXVECTOR3 &posB)
{
	// 変数宣言
	D3DXVECTOR3 posA_posB = posB - posA;
	D3DXVECTOR3 posA_Dot = posP - posA;

	float fSurface = CMylibrary::CreateflatCross(&posA_posB, &posA_Dot);

	if (fSurface > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//* [contents] 内積で表裏位置関係を調べる
//* [in] 位置P(点P), 頂点位置A, 頂点位置B
//* [return] bool （表 == true 裏 == false）
bool CMylibrary::DotRelationship(CONST D3DXVECTOR3 &posP, CONST D3DXVECTOR3& posA, CONST D3DXVECTOR3 &posB, float fTolerance)
{
	// 変数宣言
	D3DXVECTOR3 posA_posB = posB - posA;
	D3DXVECTOR3 posA_posP = posP - posA;
	// 表裏を決める値を内積計算
	float fJudgValue = CMylibrary::Create3DDot(&posA_posB, &posA_posP);
	if (fJudgValue < fTolerance)
	{// 裏
		return false;
	}
	else
	{// 表
		return true;
	}
}

//* [contents] 2D一辺を内積で位置関係を調べる
//* [in] 位置P(点P), 頂点位置A, 頂点位置B
//* [return] bool （表 == true 裏 == false）
bool CMylibrary::OneSideDotRelationship(CONST D3DXVECTOR3 &posP, CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, float fTolerance)
{
	if (!CMylibrary::DotRelationship(posP, posA, posB, fTolerance))
	{
		return false;
	}
	if (!CMylibrary::DotRelationship(posP, posB, posA, fTolerance))
	{
		return false;
	}
	return true;
}

//* [contents] 3D点と直線の距離
//* [in] 位置P(点P), 頂点位置A, 頂点位置B
//* [return] 計算した距離
float CMylibrary::Get3DLengthDotAndLine(CONST D3DXVECTOR3 * posP, CONST D3DXVECTOR3 * pPosA, CONST D3DXVECTOR3 * pPosB)
{
	// 変数宣言
	D3DXVECTOR3 posA_posB = *pPosB - *pPosA;
	D3DXVECTOR3 posA_Dot = *posP - *pPosA;

	// 外積でベクトルの長さを求め面積値を出す
	D3DXVECTOR3 CrossVec;
	Create3DCross(&CrossVec, &posA_posB, &posA_Dot);
	float fAreavAlue = CMylibrary::Get3DVecLength(&CrossVec);

	// 線の長さを求める
	float Line = CMylibrary::Get3DToPointDistance(pPosA, pPosB);

	float fLength = fAreavAlue / Line;

	return fLength;
}

//* [contents] 3D線上の最近点を算出
//* [in] 位置P(点P), 頂点位置A, 頂点位置B
//* [out] 最近点
void CMylibrary::Get3DNearestPointOnline(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 * posP, CONST D3DXVECTOR3 * pPosA, CONST D3DXVECTOR3 * pPosB)
{
	// 変数宣言
	D3DXVECTOR3 vecA_B;				// A->Bのベクトル
	D3DXVECTOR3 vecA_Dot;			// A->Dotのベクトル

	vecA_B		= *pPosB - *pPosA;	// A->Bのベクトル作成
	vecA_Dot	= *posP - *pPosA;	// A->Dotのベクトル作成

	// 単位ベクトルの作成
	CMylibrary::CreateUnitVector(&vecA_B, &vecA_B);

	// 内積でposAから最近点までの距離を算出
	float fDistance = CMylibrary::Create3DDot(&vecA_B, &vecA_Dot);

	// 最近点を算出
	pOut->x = pPosA->x + (vecA_B.x * fDistance);
	pOut->y = pPosA->y + (vecA_B.y * fDistance);
	pOut->z = pPosA->z + (vecA_B.z * fDistance);
}

//* [contents] 3D平面上の点と直線のの距離
//* [in] 位置P(点P), 頂点位置A, 頂点位置B
//* [return] 計算した距離
float CMylibrary::Get3DFlatLengthDotAndLine(CONST D3DXVECTOR3 * posP, CONST D3DXVECTOR3 * pPosA, CONST D3DXVECTOR3 * pPosB)
{
	// 変数宣言
	D3DXVECTOR3 posA_posB = D3DXVECTOR3(pPosB->x - pPosA->x,0.0f, pPosB->z - pPosA->z);
	D3DXVECTOR3 posA_Dot = D3DXVECTOR3(posP->x - pPosA->x, 0.0f, posP->z - pPosA->z);

	// 外積でベクトルの長さを求め面積値を出す
	D3DXVECTOR3 CrossVec;
	Create3DCross(&CrossVec, &posA_posB, &posA_Dot);
	float fAreavAlue = CMylibrary::Get3DVecLength(&CrossVec);

	// 線の長さを求める
	float Line = CMylibrary::Get3DToPointDistance(pPosA, pPosB);

	float fLength = fAreavAlue / Line;

	return fLength;
}

//* [contents] ビルボードの設定
//* [in] デバイスポインタ
//* [out] マトリックス
void CMylibrary::SetBillboard(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX * mtxOutput)
{
	// 変数宣言
	D3DXMATRIX mtxView;
	// ヴュー情報を取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//逆行列　ビルボード
	mtxOutput->_11 = mtxView._11;
	mtxOutput->_12 = mtxView._21;
	mtxOutput->_13 = mtxView._31;
	mtxOutput->_21 = mtxView._12;
	mtxOutput->_22 = mtxView._22;
	mtxOutput->_23 = mtxView._32;
	mtxOutput->_31 = mtxView._13;
	mtxOutput->_32 = mtxView._23;
	mtxOutput->_33 = mtxView._33;
}

//* [contents] ビルボードロー回転の打ち消しの設定
//* [in] デバイスポインタ
//* [out] マトリックス
void CMylibrary::SetBillboardNotRoll(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX * mtxOutput)
{
	// 変数宣言
	D3DXMATRIX mtxView;
	// ヴュー情報を取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//逆行列　ビルボード
	mtxOutput->_11 = mtxView._11;
	mtxOutput->_12 = mtxView._21;
	mtxOutput->_13 = mtxView._31;
	mtxOutput->_31 = mtxView._13;
	mtxOutput->_32 = mtxView._23;
	mtxOutput->_33 = mtxView._33;
}

//-------------------------------------------------------------------------------------------------------------
// 2,3D
//-------------------------------------------------------------------------------------------------------------

//* [contents] 減速運動処理
//* [in] (減速)係数
//* [out] 3DVec移動量
void CMylibrary::SlowingMove(D3DXVECTOR3 * pMove, const float fCoefficient)
{
	pMove->x += pMove->x * -fCoefficient;
	pMove->y += pMove->y * -fCoefficient;
	pMove->z += pMove->z * -fCoefficient;
}

//* [contents] 減速運動処理
//* [in] (減速)係数
//* [out] 移動量
void CMylibrary::SlowingMove(float * pMove, const float fCoefficient)
{
	*pMove += *pMove * -fCoefficient;
}

//* [contents] 追従移動処理
//* [in] 追う位置, 追われる位置, (追従)係数
//* [out] 3DVec移動量
void CMylibrary::FollowingMove(D3DXVECTOR3 * move, const D3DXVECTOR3 * ChasPos, const D3DXVECTOR3 * ChasedPos, const float * SpeedFactor)
{
	move->x = (ChasedPos->x - ChasPos->x) * *SpeedFactor;
	move->y = (ChasedPos->y - ChasPos->y) * *SpeedFactor;
	move->z = (ChasedPos->z - ChasPos->z) * *SpeedFactor;
}

//* [contents] 点と点の距離を比較する
//* [in] 距離, ソース（差分の場合追加ソースなし）, 追加ソース
//* [return] bool （true 設定した距離より近い、false 設定した距離より遠い）
bool CMylibrary::TestToPointDistance(float fDistance, CONST D3DXVECTOR3 * Source, CONST D3DXVECTOR3 * AddSource)
{
	if (AddSource == NULL)
	{
		if (Source->x*Source->x +
			Source->y*Source->y +
			Source->z*Source->z <=
			fDistance*fDistance)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		D3DXVECTOR3 diffPos = *Source - *AddSource;

		if (diffPos.x*diffPos.x +
			diffPos.y*diffPos.y +
			diffPos.z*diffPos.z <=
			fDistance*fDistance)
		{
			return true;
		}
		else
		{
			return false;
		}

	}
}

//* [contents] 回転量を直す
//* [out] 向きor回転量
void CMylibrary::SetFixTheRotation(float * Rotation)
{
	if (*Rotation < -D3DX_PI)
	{
		*Rotation += D3DX_PI * 2;
	}
	else if (*Rotation > D3DX_PI)
	{
		*Rotation -= D3DX_PI * 2;
	}
}

//* [contents] 回転量を直す
//* [out] Vec3向きor回転量
void CMylibrary::SetVec3FixTheRotation(D3DXVECTOR3 * Rotation)
{
	if (Rotation->x < -D3DX_PI)
	{
		Rotation->x += D3DX_PI * 2;
	}
	else if (Rotation->x > D3DX_PI)
	{
		Rotation->x -= D3DX_PI * 2;
	}

	if (Rotation->y < -D3DX_PI)
	{
		Rotation->y += D3DX_PI * 2;
	}
	else if (Rotation->y > D3DX_PI)
	{
		Rotation->y -= D3DX_PI * 2;
	}

	if (Rotation->z < -D3DX_PI)
	{
		Rotation->z += D3DX_PI * 2;
	}
	else if (Rotation->z > D3DX_PI)
	{
		Rotation->z -= D3DX_PI * 2;
	}
}

//-------------------------------------------------------------------------------------------------------------
// COLLAR
//-------------------------------------------------------------------------------------------------------------

//* [contents] COLORREFからD3DXCOLORに変換する(アルファ値なし)
//* [in] システムカラー
//* [out] DirectX3Dカラー
void CMylibrary::SetD3DXCOLORfromREF_WithoutAlpha(COLORREF * SystemColor, D3DXCOLOR * col)
{
	col->r = (BYTE)(*SystemColor) / 255.0f;
	col->g = (BYTE)((*SystemColor) >> MYLIB_D3DXCORG_BIT) / 255.0f;
	col->b = (BYTE)((*SystemColor) >> MYLIB_D3DXCORB_BIT) / 255.0f;
}

//* [contents]  COLORREFからD3DXCOLORに変換する
//* [in] システムカラー
//* [out] DirectX3Dカラー
void CMylibrary::SetD3DXCOLORfromREF(COLORREF * SystemColor, D3DXCOLOR * col)
{
	col->r = (BYTE)(*SystemColor) / 255.0f;
	col->g = (BYTE)((*SystemColor) >> MYLIB_D3DXCORG_BIT) / 255.0f;
	col->b = (BYTE)((*SystemColor) >> MYLIB_D3DXCORB_BIT) / 255.0f;
	col->a = (BYTE)((*SystemColor) >> MYLIB_D3DXCORA_BIT) / 255.0f;
}

//-------------------------------------------------------------------------------------------------------------
// Misc
//-------------------------------------------------------------------------------------------------------------

//* [contents] 桁数を取得する
//* [in] ソース（数字）
//* [out] 桁数
void CMylibrary::GetDigits(int & out, int nSource)
{
	do
	{
		out++;
	} while ((nSource /= 10) != 0);
}

//* [contents] 桁数を取得する
//* [in] ソース（数字）
//* [return] 桁数
int CMylibrary::GetDigits(int nSource)
{
	// 変数宣言
	int nResult = 0;	// 計算結果
	do
	{
		nResult++;
	} while ((nSource /= 10) != 0);

	return nResult;
}

//* [contents] 桁数を取得する(引数あり)
//* [in] ソース, 数字, 基数（進数）
//* [out] 桁数
void CMylibrary::GetDigitsEx(int &out, int nSource, int nRadix)
{
	do
	{
		nSource /= nRadix;
		out++;
	} while (nSource >= nRadix);
}

//* [contents] 桁数を取得する(引数あり)
//* [in] ソース, 数字, 基数（進数）
//* [return] 桁数
int CMylibrary::GetDigitsEx(int nSource, int nRadix)
{
	// 変数宣言
	int nResult = 0;	// 計算結果
	do
	{
		nSource /= nRadix;
		nResult++;
	} while (nSource >= nRadix);

	return nResult;
}

//* [contents] キーIDの参照したキー名を取得
//* [in] キーID
//* [out] キー名
void CMylibrary::GetKeyInputInfo(int nKeyID, STRING KeyName)
{
	if (KeyName != NULL)
	{
		switch (nKeyID)
		{
		case DIK_0:		   strcpy(KeyName, "0");        break;
		case DIK_1:		   strcpy(KeyName, "1");        break;
		case DIK_2:		   strcpy(KeyName, "2");        break;
		case DIK_3:		   strcpy(KeyName, "3");        break;
		case DIK_4:		   strcpy(KeyName, "4");        break;
		case DIK_5:		   strcpy(KeyName, "5");        break;
		case DIK_6:		   strcpy(KeyName, "6");        break;
		case DIK_7:		   strcpy(KeyName, "7");        break;
		case DIK_8:		   strcpy(KeyName, "8");        break;
		case DIK_9:		   strcpy(KeyName, "9");        break;
		case DIK_Q:		   strcpy(KeyName, "Q");        break;
		case DIK_W:		   strcpy(KeyName, "W");        break;
		case DIK_E:		   strcpy(KeyName, "E");        break;
		case DIK_R:		   strcpy(KeyName, "R");        break;
		case DIK_T:		   strcpy(KeyName, "T");        break;
		case DIK_Y:		   strcpy(KeyName, "Y");        break;
		case DIK_U:		   strcpy(KeyName, "U");        break;
		case DIK_I:		   strcpy(KeyName, "I");        break;
		case DIK_O:		   strcpy(KeyName, "O");        break;
		case DIK_P:		   strcpy(KeyName, "P");        break;
		case DIK_A:		   strcpy(KeyName, "A");        break;
		case DIK_S:		   strcpy(KeyName, "S");        break;
		case DIK_D:		   strcpy(KeyName, "D");        break;
		case DIK_F:		   strcpy(KeyName, "F");        break;
		case DIK_G:		   strcpy(KeyName, "G");        break;
		case DIK_H:		   strcpy(KeyName, "H");        break;
		case DIK_J:		   strcpy(KeyName, "J");        break;
		case DIK_K:		   strcpy(KeyName, "K");        break;
		case DIK_L:		   strcpy(KeyName, "L");        break;
		case DIK_Z:		   strcpy(KeyName, "Z");        break;
		case DIK_X:		   strcpy(KeyName, "X");        break;
		case DIK_C:		   strcpy(KeyName, "C");        break;
		case DIK_V:		   strcpy(KeyName, "V");        break;
		case DIK_B:		   strcpy(KeyName, "B");        break;
		case DIK_N:		   strcpy(KeyName, "N");        break;
		case DIK_M:		   strcpy(KeyName, "M");        break;
		case DIK_LSHIFT:   strcpy(KeyName, "LSHIFT");   break;
		case DIK_LCONTROL: strcpy(KeyName, "LCONTROL"); break;
		case DIK_RSHIFT:   strcpy(KeyName, "RSHIFT");   break;
		case DIK_RCONTROL: strcpy(KeyName, "RCONTROL"); break;
		case DIK_RETURN:   strcpy(KeyName, "RETURN");   break;
		case DIK_SPACE:    strcpy(KeyName, "SPACE");    break;
		}
	}
}

//* [contents] 繰り上げた数を取得
//* [in] ソース, 基数
//* [return] 繰り上げた数
int CMylibrary::GetRoundedUp(int nSource, int nBase)
{
	// 基数より１小さい数を加算してから基数で割って、基数をかける。
	return (nSource + (nBase - 1)) / nBase * nBase;
}

//-------------------------------------------------------------------------------------------------------------
// FILE
//-------------------------------------------------------------------------------------------------------------

//* [contents] テキストファイルから個数を取得する
//* [in] ファイル名, 項目名
//* [return] 個数
int CMylibrary::GetNumberFromText(CONST_STRING TextFile, CONST_STRING SetItem)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ
	char cRead[128];				// 読み込み用
	char cComp[128];				// 比較用
	char cEmpty[128];				// 要らないもの用
	int nNumber = MYLIB_ERROR;	// 個数
	int nCntError = 0;				// エラー用

									// ファイルが開かれていなかったら
	if ((pFile = fopen(TextFile, "r")) == NULL)
	{// メッセージの表示
		return 12;
	}
	// スクリプトが存在しているかチェック
	if (!CMylibrary::CheckforScript(pFile))
	{
		return 12;
	}
	cRead[0] = '\0';
	cComp[0] = '\0';
	// END_SCRIPTが来るまで
	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPTまでループ
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > MYLIB_ERRORCOUNT_MAX)
		{// エラー
			nCntError = 0;

		}
		if (strcmp(cComp, SetItem) == 0)
		{
			sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &nNumber);
			cComp[0] = '\0';
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	return nNumber;
}

//* [contents] テキストファイルにスクリプトが存在するかチェック
//* [in] ファイルポインタ
//* [return] bool (true 存在した、false 存在しない)
bool CMylibrary::CheckforScript(FILE *pFile)
{
	// 変数宣言
	char cRead[MYLIB_STRINGSIZE] = {};	// 読み込み用
	char cComp[MYLIB_STRINGSIZE] = {};	// 比較用
	int nCntError = 0;					// エラー用
	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > MYLIB_ERRORCOUNT_MAX)
		{// エラー
			nCntError = 0;
			return false;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------------------
// STRING
//-------------------------------------------------------------------------------------------------------------

//* [contents] 文字列の確保、設定
//* [in] ソース(文字列)
//* [return] 文字列ポインタ
STRING CMylibrary::SetStringAlloc(CONST_STRING source)
{
	// 文字列の長さを取得
	int nLength = strlen(source);
	// メモリ確保[長さ + NULL文字]
	STRING work = new char[nLength + 1];
	// 文字の代入
	sprintf(work, "%s", source);

	return work;
}

//* [contents] 文字列からint型に変換する(文字列から数字を抜き取る)
//* [in] 文字列
//* [return] 変換した数字
int CMylibrary::GetConvertStoI(STRING string)
{
	// 変数宣言
	int nNum = 0;			// 数
	bool bType = false;		// 符号の種類
	
	// 先頭に+付いてた時スキップ
	if (*string == '+')
	{
		string++;
	}

	// 先頭に-付いてたら符号フラグを立てておく
	else if (*string == '-')
	{
		bType = true;
		string++;
	}

	while (*string != '\0')
	{
		// 0〜9以外の文字列の時
		if (*string < 48 ||
			*string > 57)
		{
			string++;
			continue;
		}
		nNum += *string - 48;
		nNum *= 10;
		string++;
	}

	nNum /= 10;

	// フラグが立っていた時
	if (bType)
	{
		nNum = 0 - nNum;
	}

	return nNum;

}

//* [contents] int型から文字列に変換する
//* [in] 変換する値、基数（進数）
//* [out] 変換した文字列
void CMylibrary::ConvertItoS(STRING pOut, int nVal, int nRadix)
{
	// 変数宣言
	int  nDigits;		// 桁数 + 1
	char *pCur;			// 現在のポインタ

	// 桁数 + NULL文字分
	nDigits = GetDigitsEx(nVal, nRadix) + 1;
	// 後ろからの設定する
	pCur = pOut + nDigits;
	// 文字列の最後を設定
	*pCur = '\0';

	do {
		// ポインタをデクリメント
		--pCur;
		// 一桁分の数値を文字に変換
		*pCur = nVal % nRadix + '0';
		// 変換した文字が10進数以上の時
		if (*pCur > '9')
		{
			// アルファベットに変換
			*pCur = nVal % nRadix - 10 + 'A';
		}
		// 桁をずらす
		nVal /= nRadix;
	} while (pCur != pOut);

}

//* [contents] int型から文字列を生成し、変換する
//* [in] 基数（進数）
//* [return] 変換した文字列
STRING CMylibrary::ConvertItoSAlloc(int nVal, int nRadix)
{
	// 変数宣言
	int    nDigits;		// 桁数
	STRING pOut;		// 返す値

	// 桁数(+ NULL文字)を取得
	nDigits = GetDigitsEx(nVal, nRadix) + 1;

	// 生成
	pOut = new char[nDigits];
	// 生成に失敗した時
	if (pOut == NULL)
	{
		return NULL;
	}

	// 変数宣言
	char *pFinish;		// 終了するポインタ

	// アドレスを設定
	pFinish = pOut;
	// 後ろからの設定する
	pOut += nDigits;
	// 最後の文字を設定
	*pOut = '\0';

	do {
		// ポインタをデクリメント
		--pOut;
		// 一桁分の数値を文字に変換
		*pOut = nVal % nRadix + '0';
		// 変換した文字が10進数以上の時
		if (*pOut > '9')
		{
			// アルファベットに変換
			*pOut = nVal % nRadix - 10 + 'A';
		}
		// 桁をずらす
		nVal /= nRadix;
	} while (pOut != pFinish);

	return pOut;
}

//* [contents] int型から文字列を生成し、変換する
//* [in] 変換する値、基数（進数）
//* [out] 変換した文字列
void CMylibrary::ConvertItoSAlloc(char ** ppOut, int nVal, int nRadix)
{
	// 変数宣言
	int nDigits;			// 桁数

							// 桁数を取得
	nDigits = GetDigitsEx(nVal, nRadix) + 1;

	// 生成
	*ppOut = new char[nDigits];
	// 生成に失敗した時
	if (*ppOut == NULL)
	{
		return;
	}

	// 変数宣言
	char *pFinish;		// 終了ポインタ

						// アドレスを設定
	pFinish = *ppOut;
	// 後ろからの設定にする
	*ppOut += nDigits;
	// 最後の文字を設定
	**ppOut = '\0';

	do {
		// ポインタをデクリメント
		--*ppOut;
		// 一桁分の数値を文字に変換
		**ppOut = nVal % nRadix + '0';
		// 変換した文字が10進数以上の時
		if (**ppOut > '9')
		{
			// アルファベットに変換する
			**ppOut = nVal % nRadix - 10 + 'A';
		}
		// 桁をずらす
		nVal /= nRadix;
	} while (*ppOut != pFinish);

}
