//*************************************************************************************************************
//
// �����̃��C�u�����̏���[Mylibrary.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _MYLIBRARY_H_
#define _MYLIBRARY_H_

#define _CRT_SECURE_NO_WARNINGS

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"
#include "XAudio2.h"
#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <time.h>
#include <vector>
#include <list>
#include <iostream>
using namespace std;

//-------------------------------------------------------------------------------------------------------------
// ���C�u�����̃����N
//-------------------------------------------------------------------------------------------------------------
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")

//-------------------------------------------------------------------------------------------------------------
// �S�̃���
//-------------------------------------------------------------------------------------------------------------
/*
�ioperator�j�I�[�o�[���[�h�ł��鉉�Z�q
	+		-		*		/		%		^
	!		=		<		>		+=		-=
	^=		&=		|=		<<		>>		<<=
	<=		>=		&&		||		++		--
	( )		[ ]		new		delete	&		|
	~		*=		/=		%=		>>=		==
	!=		,		->		->*
*/
/*
	* [pIt] iterator�̗�(�|�C���^)
	* [src] source�̗�
	* [rhs] right-hand side�̗�
	* [lhs] left-hand side�̗�
	* [str] string�̗�
	* [tmp] temporary�̗�
	* [b]   bool�^�̗�
	* [t]   true(�^)�̗�
	* [f]   false(�U)�̗�
	* [o]   output�̗�
	* [r]   resource�̗�
*/

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define MYLIB_NORMAL			 0					// ����ɓ������Ƃ�
#define MYLIB_ERROR				-1					// ���s������
#define MYLIB_STRINGSIZE		128					// ������̊�{�̒���

#define MYLIB_PNG				".png" ,".PNG"
#define MYLIB_JPG				".jpg" ,".JPG"
#define MYLIB_JPEG				".jpeg" ,".JPEG"
#define MYLIB_IMAGE_EXTENSION	MYLIB_PNG ,MYLIB_JPG ,MYLIB_JPEG

#define MYLIB_TXT				".txt" ,".TXT"
#define MYLIB_INI				".ini" ,".INI"
#define MYLIB_TXT_EXTENSION		MYLIB_TXT ,MYLIB_INI

#define MYLIB_X					".x" ,".X"

#define MYLIB_CLEARARRAYONDECLA	{ NULL }						// �錾���̔z����N���A

#define MYLIB_INITSTRUCT_WITHCONST	{ }							// �R���X�g���N�^�̂���\���̂̏�����

#define MYLIB_D3DXCOR_UNSET			D3DXCOLOR(0.0f,0.0f,0.0f,0.0f)	// �F�ݒ�Ȃ�
#define MYLIB_D3DXCOR_SET			D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)	// �F�ݒ�

#define MYLIB_3DVECTOR_ZERO			D3DXVECTOR3(0.0f,0.0f,0.0f)		// 3D�x�N�g�����[���ɐݒ�
#define MYLIB_SCALE_UNSET			D3DXVECTOR3(1.0f,1.0f,1.0f)		// �X�P�[���̏�����
#define MYLIB_VEC3_XONE				D3DXVECTOR3(1.0f,0.0f,0.0f)		// 3D�x�N�g��X����1.0f
#define MYLIB_VEC3_YONE				D3DXVECTOR3(0.0f,1.0f,0.0f)		// 3D�x�N�g��Y����1.0f
#define MYLIB_VEC3_ZONE				D3DXVECTOR3(0.0f,0.0f,1.0f)		// 3D�x�N�g��Z����1.0f
#define MYLIB_VEC3_UNSET			D3DXVECTOR3(0.0f,0.0f,0.0f)		// 3D�x�N�g���̏�����

#define MYLIB_2DVECTOR_ZERO			D3DXVECTOR2(0.0f,0.0f)			// 2D�x�N�g�����[���ɐݒ�
#define MYLIB_VEC2_XONE				D3DXVECTOR2(1.0f,0.0f)			// 2D�x�N�g��X����1.0f
#define MYLIB_VEC2_YONE				D3DXVECTOR2(0.0f,1.0f)			// 2D�x�N�g��Y����1.0f
#define MYLIB_VEC2_UNSET			D3DXVECTOR2(0.0f,0.0f)			// 2D�x�N�g���̏�����

#define MYLIB_INT2_UNSET			INTEGER2(0,0)					// INT2�ݒ�Ȃ�

#define MYLIB_RANGE_UNSET			RANGE(0,0)						// �����W�̐ݒ�Ȃ�

#define MYLIB_FLOAT_UNSET			(0.0f)							// float�^�̏�����
#define MYLIB_INT_UNSET				(0)								// int�^�̏�����
#define MYLIB_INT_NOELEM			(-1)							// int�^�v�f�Ȃ�
#define MYLIB_CHAR_UNSET			'\0'							// char�^�̏�����(������̈�Ԗڂ̏�����)
#define MYLIB_STRING_UNSET			{ 0 }							// ������̏�����

#define MYLIB_HALF_SIZE				(0.5f)							// �����̃T�C�Y
#define MYLIB_ONEQUARTER_SIZE		(0.25f)							// 4����1�̃T�C�Y
#define MYLIB_THREEQUARTERS_SIZE	(0.75f)							// 4����3�̃T�C�Y

#define MYLIB_VSHORT_UNSET			(0x000)							// 1�o�C�g���̕ϐ��ݒ�Ȃ�

#define MYLIB_ERRORCOUNT_MAX		(1048576)						// �G���[�J�E���g

#define MYLIB_VSHORT_UNSET			(0x000)							// 

#define MYLIB_HALFSECONDCOUNT		(30)							// 1�t���[���J�E���g�����̃J�E���g

#define MYLIB_D3DXCORR_BIT			(0)								// ��
#define MYLIB_D3DXCORG_BIT			(8)								// ��
#define MYLIB_D3DXCORB_BIT			(16)							// ��
#define MYLIB_D3DXCORA_BIT			(24)							// �����x

#define MYLIB_S8BYTES				(64)							// ������8�o�C�g��
#define MYLIB_S4BYTES				(32)							// ������4�o�C�g��

#define MYLIB_BRACKETS_L					"========================================================== \n"		// ��؂�̊���(��)
#define MYLIB_BRACKETS_M					"---------------------------------------------------------- \n"		// ��؂�̊���(��)
#define MYLIB_BRACKETS_S					"#*\n"																// ��؂�̊���(��)

#define MYLIB_OX_EPSILON			0.000001f							// �덷

//-------------------------------------------------------------------------------------------------------------
// �}�N���֐�
//-------------------------------------------------------------------------------------------------------------
# ifdef _DEBUG
#define Mybfunc_Debug_brackets_L		cout << MYLIB_BRACKETS_L			// ��؂�̊���(��)�̏o��
#define Mybfunc_Debug_brackets_M		cout << MYLIB_BRACKETS_M			// ��؂�̊���(��)�̏o��
#define Mybfunc_Debug_brackets_S		cout << MYLIB_BRACKETS_S			// ��؂�̊���(��)�̏o��
#define Mybfunc_Debug_start(str)		cout << str << " == start!!\n"		// �J�n(start!!)�̏o��
#define Mybfunc_Debug_end(str)			cout << str << " == end!!\n"		// �I��(end!!)�̏o��
#define Mybfunc_Debug_open(str)			cout << str << " == open!!\n"		// �J����(open!!)�̏o��
#define Mybfunc_Debug_openfailed(str)	cout << str << " == open failed!!\n"// �J���̎��s(open failed!!)�̏o��
#define Mybfunc_Debug_close(str)		cout << str << " == close!!\n"		// ����(close!!)�̏o��
#endif
#define Mybfunc_gate(b)					if(b)								// ��:�^�̎� ���g��Ȃ�
#define Mybfunc_iif(b)					(b == true) ? true: false			// ����������^�U��Ԃ�
#define Mybfunc_iifEx(b ,t ,f)			(b == true) ? t: f					// ����������^�U��Ԃ�(�^�U����)
#define Mybfunc_WhenBiggerSet(o ,src)	if(o > src) {o = src;}				// s���o���傫���Ƃ��ݒ肷��
#define Mybfunc_WhenSmallerSet(o ,src)	if(o < src) {o = src;}				// s���o���������Ƃ��ݒ肷��
#define Mybfunc_swap(lhs,rhs,tmp)		do{tmp = rhs; rhs = lhs; lhs = tmp}while(false);	// ����

namespace ikuminLib
{
//-------------------------------------------------------------------------------------------------------------
// �񋓌^��`
//-------------------------------------------------------------------------------------------------------------
// ���_�_�̌��_(�΃L�[�p�b�h)
typedef enum
{
	ORIGINVERTEXTYPE_NONE = 0,			// �Ȃ�
	ORIGINVERTEXTYPE_LOWERLEFT,			// ����
	ORIGINVERTEXTYPE_LOWERCENTER,		// ������
	ORIGINVERTEXTYPE_LOWERRIGHT,		// �E��
	ORIGINVERTEXTYPE_CENTERLEFT,		// ������
	ORIGINVERTEXTYPE_CENTER,			// ����
	ORIGINVERTEXTYPE_CENTERRIGHT,		// �E����
	ORIGINVERTEXTYPE_UPPERLEFT,			// ����
	ORIGINVERTEXTYPE_UPPERCENTER,		// ������
	ORIGINVERTEXTYPE_UPPERRIGHT,		// �E��
	ORIGINVERTEXTYPE_ROTATION = 10,		// ��]����
	ORIGINVERTEXTYPE_ROTLOWERLEFT,		// ��]����
	ORIGINVERTEXTYPE_ROTLOWERCENTER,	// ��]������
	ORIGINVERTEXTYPE_ROTLOWERRIGHT,		// ��]�E��
	ORIGINVERTEXTYPE_ROTCENTERLEFT,		// ��]������
	ORIGINVERTEXTYPE_ROTCENTER,			// ��]����
	ORIGINVERTEXTYPE_ROTCENTERRIGHT,	// ��]�E����
	ORIGINVERTEXTYPE_ROTUPPERLEFT,		// ��]����
	ORIGINVERTEXTYPE_ROTUPPERCENTER,	// ��]������
	ORIGINVERTEXTYPE_ROTUPPERRIGHT,		// ��]�E��
	ORIGINVERTEXTYPE_MAX				// �ő吔
} ORIGINVERTEXTYPE;

// ���Z�qID
typedef enum
{
	OPERATOR_ID_NONE = -1,				// ����
	OPERATOR_ID_ADDITION,				// ���Z
	OPERATOR_ID_SUBTRACTION,			// ���Z
	OPERATOR_ID_MULTIPLICATION,			// ��Z
	OPERATOR_ID_DIVISION,				// ���Z
	OPERATOR_ID_MAX						// �ő吔
} OPERATOR_ID;


//-------------------------------------------------------------------------------------------------------------
// �^�A�\���̒�`
//-------------------------------------------------------------------------------------------------------------
typedef const char *    CONST_STRING;	// �ύX�s������
typedef char *          STRING;			// ������

typedef signed char     VSHORT;			// ��������1�o�C�g���̕ϐ�
typedef unsigned char   UNVSHORT;		// �����Ȃ�1�o�C�g���̕ϐ�

// �r�b�g�t�B�[���h
typedef struct BITS_12
{
	unsigned char _1 : 1;
	unsigned char _2 : 1;
	unsigned char _3 : 1;
	unsigned char _4 : 1;
	unsigned char _5 : 1;
	unsigned char _6 : 1;
	unsigned char _7 : 1;
	unsigned char _8 : 1;
	unsigned char _9 : 1;
	unsigned char _10 : 1;
	unsigned char _11 : 1;
	unsigned char _12 : 1;
}BITS_12;

typedef union UBITS_12
{
	// max 4095
	UBITS_12() {}
	UBITS_12(unsigned char c) : cValue(c) {}
	unsigned short cValue;
	BITS_12 Bits;
	inline unsigned char operator[] (int nIndex)
	{
		return (cValue & (1 << nIndex));
	}
	inline bool comp(int nIndex)
	{
		return (cValue & (1 << nIndex)) != 0;
	}
	inline void clear(int nIndex)
	{
		cValue &= ~(1 << nIndex);
	}
	inline void set(int nIndex)
	{
		cValue |= (1 << nIndex);
	}
} UBITS_12;
// �r�b�g�t�B�[���h
typedef struct BITS_8
{
	unsigned char _1 : 1;
	unsigned char _2 : 1;
	unsigned char _3 : 1;
	unsigned char _4 : 1;
	unsigned char _5 : 1;
	unsigned char _6 : 1;
	unsigned char _7 : 1;
	unsigned char _8 : 1;

}BITS_8;

typedef union UBITS_8
{
	UBITS_8() {}
	UBITS_8(unsigned char c) : cValue (c){}
	unsigned char cValue;
	BITS_8 Bits;
	inline bool operator[] (int nIndex)
	{
		return (cValue & (1 << nIndex)) != 0;
	}
} UBITS_8;

// �r�b�g�t�B�[���h
typedef struct BITS_4
{
	unsigned char _1 : 1;
	unsigned char _2 : 1;
	unsigned char _3 : 1;
	unsigned char _4 : 1;
}BITS_4;

typedef union
{
	unsigned char cValue;
	BITS_4 Bits;
} UBITS_4;

// 3����float
struct FLOAT3 : public D3DXVECTOR3
{
	FLOAT3() {}																										// �R���X�g���N�^
	FLOAT3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}														// �R���X�g���N�^
	FLOAT3(CONST FLOAT3& rhs) : D3DXVECTOR3(rhs) {}																	// �R���X�g���N�^
	FLOAT3(CONST D3DXVECTOR3& rhs) : D3DXVECTOR3(rhs) {}															// �R���X�g���N�^
	~FLOAT3() {}																									// �f�X�g���N�^
	inline FLOAT3        operator +(const FLOAT3 &rhs) const;														// �l�����Z�q+
	inline FLOAT3        operator -(const FLOAT3 &rhs) const;														// �l�����Z�q-
	inline FLOAT3        operator -(void) const;																	// �l�����Z�q-
	inline FLOAT3        operator *(const FLOAT3 &rhs) const;														// �l�����Z�q*
	inline FLOAT3        operator /(const FLOAT3 &rhs) const;														// �l�����Z�q/
	inline FLOAT3        operator *(float rhs) const;																// �l�����Z�q*
	inline FLOAT3        operator /(float rhs) const;																// �l�����Z�q/
	inline friend FLOAT3 operator *(float l, const FLOAT3 &rhs) { return FLOAT3(rhs.x * l, rhs.y * l, rhs.z * l); }	// �l�����Z�q*�t�����h�֐�
	inline friend FLOAT3 operator /(float l, const FLOAT3 &rhs) { return FLOAT3(rhs.x / l, rhs.y / l, rhs.z / l); }	// �l�����Z�q/�t�����h�֐�

	inline float         Dot(const FLOAT3 &rhs) const;																// ����
	inline FLOAT3        Cross(const FLOAT3 &rhs) const;															// �O��
	inline float         Length(void) const;																		// ����
	inline float         LengthSq(void) const;																		// �ׂ��撷��
	inline void          Norm(void);																				// ���K��
	inline FLOAT3        GetNorm(void) const;																		// ���K�����擾
};

// �x�N�g��
struct VEC3 : public FLOAT3
{
	VEC3() {}												// �R���X�g���N�^
	VEC3(float x, float y, float z) : FLOAT3(x, y, z) {}	// �R���X�g���N�^
	VEC3(const FLOAT3 &r) : FLOAT3(r) {}					// �R���X�g���N�^
	~VEC3() {}												// �f�X�g���N�^
	inline VEC3& operator =(const FLOAT3 &r);				// ������Z�q
	inline void  norm(void);								// �W����
	inline bool  IsVertical(const VEC3 &r) const;			// �����֌W�ɂ���H
	inline bool  IsParallel(const VEC3 &r) const;			// ���s�֌W�ɂ���H
	inline bool  IsSharpAngle(const VEC3 &r) const;			// �s�p�֌W�H
};

// ����
struct LINE
{
	FLOAT3 Point;													// �ʒu
	VEC3 Vec;														// �����x�N�g��
	LINE() : Point(0.0f, 0.0f, 0.0f), Vec(1.0f, 0.0f, 0.0f) {}		// �R���X�g���N�^
	LINE(const FLOAT3 &p, const VEC3 &v) : Point(p), Vec(v) {}		// �R���X�g���N�^
	~LINE() {}														// �f�X�g���N�^
	inline FLOAT3 GetPoint(float fCoffi) const;						// �_��̍��W���擾
};


// ����
struct SEGMENT : public LINE
{
	SEGMENT() {}														// �R���X�g���N�^
	SEGMENT(const FLOAT3 &p, const VEC3 &v) : LINE(p, v) {}				// �R���X�g���N�^
	SEGMENT(const FLOAT3 &p1, const FLOAT3 &p2) : LINE(p1, p2 - p1) {}	// �R���X�g���N�^
	~SEGMENT() {}														// �f�X�g���N�^
	inline FLOAT3 GetEndPoint(void) const;								// �I�_���擾
};

// ��
struct SPHERE
{
	FLOAT3 Point;														// ���S�_
	float fRadius;														// ���a
	SPHERE() : Point(0.0f, 0.0f, 0.0f), fRadius(0.5f) {}				// �R���X�g���N�^
	SPHERE(const FLOAT3 &p, float r) : Point(p), fRadius(r) {}			// �R���X�g���N�^
	~SPHERE() {}														// �f�X�g���N�^
};

// �J�v�Z��
struct CAPSULE
{
	SEGMENT Segment;																		// ����
	float fRadius;																			// ���a
	CAPSULE() : fRadius(0.5f) {}															// �R���X�g���N�^
	CAPSULE(const SEGMENT &s, float r) : Segment(s), fRadius(r) {}							// �R���X�g���N�^
	CAPSULE(const FLOAT3 &p1, const FLOAT3 &p2, float r) : Segment(p1, p2), fRadius(r) {}	// �R���X�g���N�^
	~CAPSULE() {}																			// �f�X�g���N�^
};

// AABB
struct AABB
{
	FLOAT3 Point;		// ���S�_
	FLOAT3 HalLength;	// �e���̕ӂ̒����̔���
	AABB() {}																// �R���X�g���N�^
	AABB(const FLOAT3 &p, const FLOAT3 &hl) : Point(p), HalLength(hl) {}	// �R���X�g���N�^
	inline float LenX(void) const { return HalLength.x * 2.0f; };			// X���ӂ̒������擾
	inline float LenY(void) const { return HalLength.y * 2.0f; };			// Y���ӂ̒������擾
	inline float LenZ(void) const { return HalLength.z * 2.0f; };			// Z���ӂ̒������擾
	inline float Len(int i)       { return *((&HalLength.x) + i) * 2.0f; }	// �ӂ̒������擾
};

/* * ���̓L�[�̃Z�� */
typedef struct _INPUTKEYCELL
{
	int             nKey;		// �L�[
	int             nData;		// �f�[�^
	_INPUTKEYCELL*  pNext;		// ���̃f�[�^�|�C���^
}INPUTKEYCELL;

/* * 3D�̐������ */
typedef struct LINESEGMENT3D
{
public:
	LINESEGMENT3D() {};
	LINESEGMENT3D(D3DXVECTOR3 &start, D3DXVECTOR3 &vector);
	/* * �P�����Z�q + */
	LINESEGMENT3D operator + (void) const;
	/* * �P�����Z�q - */
	LINESEGMENT3D operator - (void) const;

	/* * �񍀉��Z�q + */
	inline LINESEGMENT3D operator + (CONST LINESEGMENT3D& rhs) const;
	/* * �񍀉��Z�q - */
	inline LINESEGMENT3D operator - (CONST LINESEGMENT3D& rhs) const;
	/* * �񍀉��Z�q * */
	inline LINESEGMENT3D operator * (CONST LINESEGMENT3D& rhs) const;
	/* * �񍀉��Z�q / */
	inline LINESEGMENT3D operator / (CONST LINESEGMENT3D& rhs) const;

	/* * ��r���Z�q == */
	inline bool operator == (CONST LINESEGMENT3D& rhs) const;
	/* * ��r���Z�q != */
	inline bool operator != (CONST LINESEGMENT3D& rhs) const;

	/* * ������Z�q += */
	inline LINESEGMENT3D& operator += (CONST LINESEGMENT3D& rhs);
	/* * ������Z�q -= */
	inline LINESEGMENT3D& operator -= (CONST LINESEGMENT3D& rhs);
	/* * ������Z�q *= */
	inline LINESEGMENT3D& operator *= (CONST LINESEGMENT3D& rhs);
	/* * ������Z�q /= */
	inline LINESEGMENT3D& operator /= (CONST LINESEGMENT3D& rhs);

	/* �����o�ϐ�*/
	D3DXVECTOR3 start;			// �n�_
	D3DXVECTOR3 vector;			// �x�N�g��(�����A����)���K���_��

}LINESEGMENT3D, *PLINESEGMENT3D;

/* * 2D�̐������ */
typedef struct LINESEGMENT2D
{
public:
	LINESEGMENT2D() {}
	LINESEGMENT2D(D3DXVECTOR2 start, D3DXVECTOR2 vector)
	{
		this->start = start;
		this->vector = vector;
	}
	D3DXVECTOR2 start;		// �n�_
	D3DXVECTOR2 vector;		// �x�N�g��(�����A����)���K���_��
}LINESEGMENT2D, *PLINESEGMENT2D;

/* *�^�C�}�[��� */
typedef struct _TIMER_INFO
{
	_TIMER_INFO() {}
	int nStart;		// �J�n
	int nEnd;		// �I��
}TIMER_INFO;

/* *
* �e�N�X�`����UV���W�̐ݒ�p�̏��
* Previous version TEXTUREVARIABLES
*/
typedef struct _SETING_UV
{
	_SETING_UV() {}
	_SETING_UV(int nCntU, int nNumU, int nCntV, int nNumV)
	{
		this->nCntU = nCntU;
		this->nNumU = nNumU;
		this->nCntV = nCntV;
		this->nNumV = nNumV;
	}
	int nCntU;	// ���̃J�E���g
	int nNumU;	// ���̌�
	int nCntV;	// �c�̃J�E���g
	int nNumV;	// �c�̌�
}SETING_UV ;

/* *
* �e�N�X�`���A�j���[�V�������
* Previous version ANIMTEXVARIABLES
*/
typedef struct _TEXTUREANIMEINFO
{
	_TEXTUREANIMEINFO() {}
	_TEXTUREANIMEINFO(int nFrame, int nFrameMax, int nIteration)
	{
		this->nFrame = nFrame;
		this->nFrameMax = nFrameMax;
		this->nIteration = nIteration;
	}
	int nFrame;		// �t���[��
	int nFrameMax;	// �ő�t���[����
	int nIteration;	// �؂�Ԃ�(1��-1)
}TEXTUREANIMEINFO;

/* * �͈� */
typedef struct _RANGE
{
	_RANGE() {}
	_RANGE(int nMax, int nMin)
	{
		this->nMax = nMax;
		this->nMin = nMin;
	}
	//_RANGE(INTEGER2&)
	//RANGE& operator = (INTEGER2&);


	int nMax;	// �ő�l
	int nMin;	// �ŏ��l
}RANGE;

/* * int�^2�� */
typedef struct INTEGER2
{
public:
	INTEGER2() {}
	INTEGER2(int nX, int nY)
	{
		this->nX = nX;
		this->nY = nY;
	}

	int nX;	// �ő�l
	int nY;	// �ŏ��l
}INTEGER2, *PINTEGER2;

/* * int�^3�� */
typedef struct INTEGER3
{
public:
	INTEGER3() {}
	INTEGER3(int X, int Y, int Z)
	{
		nX = X;
		nY = Y;
		nZ = Z;
	}

	int nX,		// X������
		nY,		// Y������
		nZ;		// Z������
} INTEGER3, *PINTEGER3;

/* * int�^4�� */
typedef struct INTEGER4
{
public:
	INTEGER4() {}
	INTEGER4(int X, int Y, int Z, int W)
	{
		nX = X;
		nY = Y;
		nZ = Z;
		nW = W;
	}
	int nX,		// X������
		nY,		// Y������
		nZ,		// Z������
		nW;		// W������
} INTEGER4, *PINTEGER4;

/*�폜�\��*/
typedef struct
{
	char string[MYLIB_S8BYTES];
} ITEMTAG, *PITEMTAG;

typedef struct TRANSFORM
{
	TRANSFORM() :pos(MYLIB_3DVECTOR_ZERO), rot(MYLIB_3DVECTOR_ZERO), scal(MYLIB_SCALE_UNSET) {}
	TRANSFORM(D3DXVECTOR3 Possition, D3DXVECTOR3 Rotation, D3DXVECTOR3 scale)
	{
		pos = Possition;
		rot = Rotation;
		scal = scale;
	}
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 rot;		// ��]
	D3DXVECTOR3 scal;		// �X�P�[��
	D3DXMATRIX mtxWorld;	// �}�g���b�N�X
} TRANSFORM, *PTRANSFORM;


/*-- ���[�V�����L�[ --*/
typedef struct
{
	int				nFlame;		// �t���[����
	D3DXVECTOR3		*pos;		// �ʒu
	D3DXVECTOR3		*rot;		// ��]��
} MOTION_KEY;

/*-- ���[�V���� --*/
typedef struct
{
	int			nLoop;			// ���[�v���邩�ǂ���
	int			nNumKey;		// �L�[��
	MOTION_KEY	*MotionKey;		// ���[�V�����L�[
}MOTION_INFO;


// �Q�[�W���p�̌^
typedef struct GAGE_INFO
{
	GAGE_INFO() {}
	GAGE_INFO(RANGE SourceRange, int nSourceValue, float fSourceCoefficient)
	{
		range = SourceRange;
		nValue = nSourceValue;
		fCoefficient = fSourceCoefficient;
	}
	RANGE		range;			// �͈�
	int			nValue;			// ������l
	float		fCoefficient;	// �W��
} GAGE_INFO;


typedef struct
{
	UNVSHORT b;	// ��
	UNVSHORT g;	// ��
	UNVSHORT r;	// ��
} BGR;

typedef struct
{
	UNVSHORT b;	// ��
	UNVSHORT g;	// ��
	UNVSHORT r;	// ��
	UNVSHORT a;	// �����x
} BGRA;

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
/* ���쒆 �x�N�^�[�^*/
template<class T>
class MyVector
{
public:
	// 
	// �W����
	typedef T*       pIterator;
	// �W���́iconst�j
	typedef const T* pConstIterator;

	// �R���X�g���N�^
	MyVector()
	{
		this->m_nSize = 0;
		this->m_nCapacity = 0;
		this->m_pData = NULL;
	}
	// �R���X�g���N�^
	MyVector(const MyVector<T>& source)
	{
		this->m_nSize = 0;
		this->m_nCapacity = 0;
		this->m_pData = NULL;
		operator=(source);
	}
	// = �̃I�[�o�[���[�h
	inline MyVector<T>& operator=(const MyVector<T>& source)
	{
		clear();
		resize(source.this->m_nSize);
		memcpy(this->m_pData, source.m_pData, (size_t)this->m_nSize * sizeof(T));
		return *this;
	}

	// �f�X�g���N�^
	inline ~MyVector()
	{
		if (this->m_pData != NULL)
		{
			free(this->m_pData);
			this->m_pData = NULL;
		}
	}

	// �R���e�i���󂩂ǂ����𔻒肷��
	inline bool empty(void) const
	{
		return Mybfunc_iif(this->m_nSize == 0);
	}
	// �T�C�Y�̎擾
	inline int size(void) const
	{
		return this->m_nSize;
	}
	// ���e�ʂ̎擾
	inline int capacity(void) const
	{
		return this->m_nCapacity;
	}

	// []�̃I�[�o�[���[�h
	inline T& operator[](int nArray)
	{
		return this->m_pData[nArray];
	}
	// []�̃I�[�o�[���[�h
	inline const T& operator[](int nArray) const
	{
		return this->m_pData[nArray];
	}
	// �N���A
	inline void clear(void)
	{
		if (this->m_pData != NULL)
		{
			this->m_nSize = 0;
			this->m_nCapacity = 0;
			free(this->m_pData);
			this->m_pData = NULL;
		}
	}
	// �擪�v�f���w���C�e���[�^���擾����B
	inline pIterator begin(void)
	{
		return this->m_pData;
	}
	// �擪�v�f���w���C�e���[�^���擾����B
	inline const pIterator begin(void) const
	{
		return this->m_pData;
	}
	// �����̎����w���C�e���[�^���擾����
	inline pIterator end(void)
	{
		return this->m_pData + this->m_nSize;
	}
	// �����̎����w���C�e���[�^���擾����
	inline pConstIterator end(void) const
	{
		return this->m_pData + this->m_nSize;
	}
	// �擪�v�f�ւ̎Q�Ƃ��擾����B
	inline T& front(void)
	{
		return this->m_pData[0];
	}
	// �擪�v�f�ւ̎Q�Ƃ��擾����B
	inline const T& front(void) const
	{
		return this->m_pData[0];
	}
	// �����v�f�ւ̎Q�Ƃ��擾����
	inline T& back(void)
	{
		return this->m_pData[this->m_nSize - 1];
	}
	// �����v�f�ւ̎Q�Ƃ��擾����
	inline const T& back(void) const
	{
		return this->m_pData[this->m_nSize - 1];
	}
	// ����vector�I�u�W�F�N�g�ƃf�[�^�����ւ���B
	inline void swap(MyVector<T>& rhs)
	{
		// �ϐ��錾
		int       nRhs_size;		// �E�ӂ̃T�C�Y
		int       nRhs_cap;			// �E�ӂ̓��e��
		pIterator pRhs_data;		// �E�ӂ̃f�[�^

									// �T�C�Y�̌���
		Mybfunc_swap(this->m_nSize, rhs.m_nSize, nRhs_size);
		// ���e�ʂ̌���
		Mybfunc_swap(this->m_nCapacity, rhs.m_nCapacity, nRhs_cap);
		// �f�[�^�̌���
		Mybfunc_swap(this->m_pData, rhs.m_pData, pRhs_data);
	}
	// �v�f����ύX����
	inline void resize(int nNewSize)
	{
		// �V�����T�C�Y���傫���Ƃ�
		if (nNewSize > this->m_nCapacity)
		{
			// �]�T�����
			reserve(AddCapacity(nNewSize));
			// �T�C�Y��ݒ�
			this->m_nSize = nNewSize;
		}
	}
	// �v�f����ύX����
	inline void resize(int nNewSize, const T& tValue)
	{
		// ���e�ʂ��V�����T�C�Y���傫���Ƃ�
		if (nNewSize > this->m_nCapacity)
		{	// �]�T�����
			reserve(AddCapacity(nNewSize));
		}
		// �T�C�Y���V�����T�C�Y���傫���Ƃ�
		if (nNewSize > this->m_nSize)
		{
			// �V���������R�s�[����
			for (int nCntData = this->m_nSize; nCntData < nNewSize; nCntData++)
			{
				memcpy(&Data[nCntData], &tValue, sizeof(tValue));
			}
		}
		// �T�C�Y��ݒ�
		this->m_nSize = nNewSize;
	}
	// ���e�ʂ�ύX����
	inline void reserve(int nNewCapacity)
	{
		// �V�����e�ʂ����̗e�ʈȉ��̎��������Ȃ�
		if (nNewCapacity <= this->m_nCapacity)
		{
			return;
		}
		// �ϐ��錾
		pIterator pNewData;	// �V�����f�[�^
							// �̈�̊m��
		pNewData = (pIterator)malloc(nNewCapacity * sizeof(T));
		// �f�[�^����������
		if (this->m_pData != NULL)
		{// ���������R�s�[
			memcpy(pNewData, this->m_pData, this->m_nSize * sizeof(T));
			// �f�[�^���J��
			free(this->m_pData);
		}
		// �f�[�^�ɐV�����f�[�^����
		this->m_pData = pNewData;
		// ���e�ʂ�ݒ�
		this->m_nCapacity = nNewCapacity;
	}

	// �V���ȗv�f�𖖔��ɒǉ�����
	inline void push_back(const T& tValue)
	{
		// �T�C�Y�Ɠ��e�ʂ������Ƃ�
		if (this->m_nSize == this->m_nCapacity)
		{
			// �]�T�����
			reserve(AddCapacity(this->m_nSize + 1));
		}
		// �l���R�s�[����
		memcpy(&this->m_pData[this->m_nSize], &tValue, sizeof(tValue));
		// �T�C�Y���C���N�������g
		this->m_nSize++;
	}
	// �����v�f���폜����
	inline void pop_back()
	{
		// �傫�����f�N�������g
		this->m_nSize--;
	}

	// �w�肵���v�f���폜����B
	inline pIterator erase(const pIterator pIt)
	{
		// �ϐ��錾
		const int nOff; //  �폜����擪
		nOff = pIt - this->m_pData;
		// ���������ړ�����
		memmove(
			this->m_pData + nOff,
			this->m_pData + nOff + 1,
			((size_t)this->m_nSize - (size_t)nOff - 1) * sizeof(T)
		);
		// �T�C�Y���f�N�������g
		this->m_nSize--;
		// �폜�������̃C�e���[�^�[��Ԃ�
		return this->m_pData + nOff;
	}
	// �w�肵���͈̗͂v�f���폜
	inline pIterator erase(pConstIterator pIt, pConstIterator pIt_last)
	{
		// �ϐ��錾
		const int nCount = pIt_last - pIt;	// �폜����J�E���g
		const int nOff = pIt - Data;			// �폜����擪
												// ���������ړ�����
		memmove(
			this->m_pData + nOff,
			this->m_pData + nOff + nCount,
			((size_t)this->m_nSize - (size_t)nOff - nCount) * sizeof(T));
		// �T�C�Y�����炷
		this->m_nSize -= (int)nCount;
		// �폜�������̃|�C���^��n��
		return this->m_pData + nOff;
	}

	// �C�ӂ̈ʒu�ɐV���ȗv�f��}������
	inline pIterator insert(pConstIterator pIt, const T& tValue)
	{
		// �ϐ��錾
		const int nOff = pIt - this->m_pData;	// �ύX����擪
												// �T�C�Y�Ɠ��e�ʂ������Ƃ�
		if (this->m_nSize == this->m_nCapacity)
		{// ���e�ʂ�ύX����
			reserve(AddCapacity(this->m_nSize + 1));
		}
		// �폜����擪���T�C�Y�̕����傫���Ƃ�
		if (nOff < (int)this->m_nSize)
		{// ���������ړ�����
			memmove(
				this->m_pData + nOff + 1,
				this->m_pData + nOff,
				((size_t)this->m_nSize - (size_t)nOff) * sizeof(T)
			);
		}
		// �ύX���郁�����ɃR�s�[
		memcpy(&this->m_pData[nOff], &tValue, sizeof(tValue));
		// �T�C�Y���C���N�������g
		this->m_nSize++;
		// �}�������|�C���^��Ԃ�
		return this->m_pData + nOff;
	}

	// �C�ӂ̒l���v�f���Ɋ܂܂�Ă��邩
	inline bool contains(const T& tValue) const
	{
		// �ϐ��錾
		pConstIterator pData = this->m_pData;						// �f�[�^�|�C���^
		pConstIterator pData_end = this->m_pData + this->m_nSize;	// �f�[�^�̏I���
		while (pData < pData_end)
		{
			if (*pData++ == tValue)
			{
				return true;
			}
		}
		return false;
	}
	// �w�肳�ꂽ�l����������
	inline pIterator find(const T& tValue)
	{
		// �ϐ��錾
		pIterator pData = this->m_pData;							// �f�[�^�|�C���^
		pConstIterator pData_end = this->m_pData + this->m_nSize;	// �f�[�^�̏I���
		while (pData < pData_end)
		{
			if (*pData == tValue)
			{
				break;
			}
			else
			{
				++pData;
			}
		}
		return pData;
	}
	// �w�肳�ꂽ�l����������
	inline const pIterator find(const T& tValue) const
	{
		// �ϐ��錾
		pConstIterator pData = this->m_pData;						// �f�[�^�|�C���^
		pConstIterator pData_end = this->m_pData + this->m_nSize;	// �f�[�^�̏I���
		while (pData < pData_end)
		{
			if (*data == tValue)
			{
				break;
			}
			else
			{
				++pData;
			}
		}
		return pData;
	}
	// �w�肳�ꂽ�l���������폜����
	inline bool find_erase(const T& tValue)
	{
		// �ϐ��錾
		pConstIterator pIt = find(tValue);
		if (pIt < this->m_pData + this->m_nSize)
		{
			erase(pIt);
			return true;
		}
		return false;
	}
	// �C�e���[�^�[����C���f�b�N�X�ɂ���
	inline int index_from_ptr(pConstIterator pIt) const
	{
		// �ϐ��錾
		const int nOff = pIt - this->m_pData;
		return nOff;
	}
private:
	// ���e�ʂ𑝂₷_grow_capacity
	inline int AddCapacity(int nSize) const
	{
		// �ϐ��錾
		int nNewCapacity;	// �V�����e��
							// �V�����e�ʂ�ݒ�
		nNewCapacity = Mybfunc_iifEx(
			(this->m_nCapacity != 0),
			(this->m_nCapacity + this->m_nCapacity / 2),
			8);
		// �傫���ق���Ԃ�
		return Mybfunc_iifEx(
			(nNewCapacity > this->m_nSize),
			nNewCapacity,
			this->m_nSize);
	}

	int m_nSize;		// �T�C�Y
	int m_nCapacity;	// ���e��
	T*  m_pData;		// �f�[�^�|�C���^
};

class CInputkey
{
	/* * �R���X�g���N�^ */
	CInputkey() {}
	/* * �f�X�g���N�^ */
	~CInputkey() {}
	/* * ������ */
	void Init(void);
	/* * �L�[�f�[�^�̏����� */
	void KeyInit(void);
	/* * �L�[�f�[�^�̐��� */
	void KeyCreate(void);
	/* * �L�[�f�[�^���X�g�̊J�� */
	void KeyListRelease(void);
	/* * �L�[�f�[�^�̑}�� */
	void KeyInsert(int nData, int nKey);
	/* * �L�[�f�[�^�𔲂����@*/
	void KeyPullout(int nKey);
	/* * �L�[�f�[�^�̎擾 */
	int GetKeyData(int nKey);
	/* * �T�C�Y�̎擾 */
	int GetSize(void);
private:
	int           m_nSize;		// �T�C�Y�i�f�[�^��)
	INPUTKEYCELL* m_pKeyTop;	// �L�[�̐擪�|�C���^
};

class CString
{
public:
	/* * �R���X�g���N�^ */
	CString();
	/* * �f�X�g���N�^ */
	~CString() {}
	/* * ������ */
	inline void Init(void);
	/* * �J������ */
	inline void release(void);
	/* * ������̐ݒ�(�ݒ肵�Ă����ꍇ�͍Đݒ肷��) */
	inline void Set(STRING string);
	/* * ������̎擾 */
	inline STRING Get(void);
private:
	STRING m_string;		// ������|�C���^
	int m_nStringLength;	// ������̒���
};


class CMylibrary
{
public:
	//* [contents] ��p1p2p3�͉s�p���ǂ���
	//* [in] Point�i�_�j, Line�i�����j
	//* [return] �s�p���ǂ����itrue : �s�p, false : �s�p�ł͂Ȃ��j
	static bool IsSharpAngle(CONST FLOAT3 &Point1, CONST FLOAT3 &Point2, CONST FLOAT3 &Point3);

	//----------------------------------------------------------------------------------------------------
	// �J�v�Z��
	//----------------------------------------------------------------------------------------------------

	//* [contents] �_�ƒ����̍ŒZ����
	//* [in] Point�i�_�j, Line�i�����j
	//* [out] Perp�i�����j, fVecCoeffi�i�x�N�g���W���j
	//* [return] �ŒZ����
	static float calcPointLineDist(const FLOAT3 &Point, const LINE &Line, FLOAT3 &Perp, float &fVecCoeffi);

	//* [contents] �_�Ɛ����̍ŒZ����
	//* [in] Point�i�_�j, Seg�i�����j
	//* [out] EndPtShortdist�i�ŒZ�����ƂȂ�[�_�j, EndPoint�i�[�_�j
	//* [return] �ŒZ����
	static float calcPointSegmentDist(const FLOAT3 &Point, const SEGMENT &Seg, FLOAT3 &EndPtShortdist, float &EndPoint);

	//* [contents] 2�����̍ŒZ����
	//* [in] Line1�i����1�j, Line2�i����2�j
	//* [out] PerpendFoot1�i����1���̐����̑��j,PerpendFoot2�i����2���̐����̑��j,fVecCoeffi1�i����1���̃x�N�g���W���j, fVecCoeffi2�i����2���̃x�N�g���W���j
	//* [return] �ŒZ����
	static float calcLineLineDist(const LINE &Line1, const LINE &Line2, FLOAT3 &PerpendFoot1, FLOAT3 &PerpendFoot2, float &fVecCoeffi1, float &fVecCoeffi2);

	//* [contents] 0�`1�̊Ԃɐ�������
	//* [out] fValue�i��������l�j
	static void Limit0to1(float &fValue);

	//* [contents] 2�����̍ŒZ����
	//* [in] Seg1�i����1�j, Seg2�i����2�j
	//* [out] PerpendFoot1�i����1���̐����̑��j,PerpendFoot2�i����2���̐����̑��j,fVecCoeffi1�i����1���̃x�N�g���W���j, fVecCoeffi2�i����2���̃x�N�g���W���j
	//* [return] �ŒZ����
	static float calcSegmentSegmentDist(const SEGMENT &Seg1, const SEGMENT &Seg2, FLOAT3 &PerpendFoot1, FLOAT3 &PerpendFoot2, float &fVecCoeffi1, float &fVecCoeffi2);

	//* [contents] �J�v�Z�����m�̏Փ˔���
	//* [in] Cap1�i�J�v�Z��1�j, Cap2�i�J�v�Z��2�j
	//* [return] �Փ˂��Ă��鎞true
	static bool colCapsuleCapsule(const CAPSULE &Cap1, const CAPSULE &Cap2);

	//* [contents] �J�v�Z�����m�̏Փ˔���
	//* [in] Cap1�i�J�v�Z��1�j, Cap2�i�J�v�Z��2�j
	//* [return] �Փ˂��Ă��鎞true
	static bool colCapsuleCapsule(const CAPSULE &Cap1, const CAPSULE &Cap2,D3DXVECTOR3 &HitPos);

	//* [contents] �����Ƌ��̋���
	//* [in] Seg1�i����1�j, Seg2�i����2�j
	//* [out] PerpendFoot1�i����1���̐����̑��j,PerpendFoot2�i����2���̐����̑��j,fVecCoeffi1�i����1���̃x�N�g���W���j, fVecCoeffi2�i����2���̃x�N�g���W���j
	//* [return] �ŒZ����
	static float calcSegmentPosDist(const SEGMENT &Seg1, const D3DXVECTOR3 &pos, FLOAT3 &PerpendFoot1, float &fVecCoeffi1);

	//* [contents] �J�v�Z�����m�̏Փ˔���
	//* [in] Cap1�i�J�v�Z��1�j, Cap2�i�J�v�Z��2�j
	//* [return] �Փ˂��Ă��鎞true
	static bool colCapsuleSphere(const CAPSULE &Cap1, const D3DXVECTOR3 &pos, const float &fRadius, D3DXVECTOR3 & HitPos);

	//----------------------------------------------------------------------------------------------------
	// 2D
	//----------------------------------------------------------------------------------------------------

	//* [contents] 2D�x�N�g���𐶐�����{ B - A }
	//* [in] 3DposA�A3DposB
	//* [out] ��������2D�x�N�g��
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 2D�x�N�g���𐶐�����{ B - A }
	//* [in] 2DposA�A2DposB
	//* [out] ��������2D�x�N�g��
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 2D�x�N�g���𐶐�����{ B - A }
	//* [in] 3DposA�A2DposB
	//* [out] ��������2D�x�N�g��
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 2D�x�N�g���𐶐�����{ B - A }
	//* [in] 2DposA�A3DposB
	//* [out] ��������2D�x�N�g��
	static void CreateVector2(D3DXVECTOR2 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 2D�̊O�ς𐶐�����
	//* [in] 3DvecA�A3DvecB
	//* [return] ��������2D�O��
	static float Create2DCross(CONST D3DXVECTOR3 *vecA, CONST D3DXVECTOR3 *vecB);

	//* [contents] 2D�̊O�ς𐶐�����
	//* [in] 2DvecA�A2DvecB
	//* [return] ��������2D�O��
	static float Create2DCross(CONST D3DXVECTOR2 *vecA, CONST D3DXVECTOR2 *vecB);

	//* [contents] 2D�̓��ς𐶐�����
	//* [in] 3DvecA�A3DvecB
	//* [return] ��������2D�O��
	static float Create2DDot(CONST D3DXVECTOR3* vecA, CONST D3DXVECTOR3* vecB);

	//* [contents] 2D�̓��ς𐶐�����
	//* [in] 2DvecA�A2DvecB
	//* [return] ��������2D�O��
	static float Create2DDot(CONST D3DXVECTOR2* vecA, CONST D3DXVECTOR2* vecB);

	//* [contents] 2D�̃x�N�g���̒��������߂�
	//* [in] 2Dvec
	//* [return] �x�N�g���̒���
	static float Get2DVecLength(D3DXVECTOR2 *vec);

	//* [contents] 2D�̍����̋���
	//* [in] 3D����
	//* [return] �v�Z��������
	static float Get2DToPointDistance(D3DXVECTOR2 *diffPos);

	//* [contents] 2D�̒P�ʃx�N�g���𐶐�
	//* [in] 2Dvec
	//* [out] ���������P�ʃx�N�g��
	static void CreateUnitVec2D(D3DXVECTOR2 *Output, D3DXVECTOR2 *Vec);

	//* [contents] 2D�̍����̊p�x
	//* [in] 3DposA, 3DposB
	//* [out] �v�Z�����p�x
	static void Get2DToPointAngle(float *pOutput, D3DXVECTOR3 *posA, D3DXVECTOR3 *posB);

	//* [contents] 2D�̍����̊p�x
	//* [in] 3D����
	//* [out] �v�Z�����p�x
	static void Get2DToPointAngle(float *pOutput, D3DXVECTOR3 *diffPos);

	//* [contents] 2D���ς�p���Ē������m�̌�_�����߂�
	//* [in] ���_�ʒuA, ���_�ʒuB, ���_�ʒuC, ���_�ʒuD
	//* [out] ��_
	//* [return] bool �itrue ������Ă���Afalse ������Ă��Ȃ��j
	static bool Get2DIntersectionOfStraightLines(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB, CONST D3DXVECTOR3 *posC, CONST D3DXVECTOR3 *posD);

	//* [contents] 2D�O�ς�p���Đ������m�̌�_�����߂�
	//* [in] ���_�ʒuA, ���_�ʒuB, ���_�ʒuC, ���_�ʒuD, �@��
	//* [out] ��_
	//* [return] bool �itrue ������Ă���Afalse ������Ă��Ȃ��j
	static bool Get2DIntersectionOfLineSegments(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB, CONST D3DXVECTOR3 *posC, CONST D3DXVECTOR3 *posD);

	//* [contents] 2D�������m���������Ă��邩���߂�
	//* [in] ���_�ʒuA, ���_�ʒuB, ���_�ʒuC, ���_�ʒuD
	//* [out] ��_
	//* [return] bool �itrue ������Ă���Afalse ������Ă��Ȃ��j
	static bool Get2DIntersectionOfStraightLineSegments(D3DXVECTOR3 * output, CONST D3DXVECTOR3 * posA, CONST D3DXVECTOR3 * posB, CONST D3DXVECTOR3 * posC, CONST D3DXVECTOR3 * posD);

	//* [contents] 2D�l�p�`�̓����蔻��
	//* [in] �ʒuA,�ʒuB, �傫��A, �傫��B
	//* [return] bool �itrue �ڐG���Ă���Afalse �ڐG���Ă��Ȃ��j
	static bool Test2DCollisionRect(D3DXVECTOR3 *posA, D3DXVECTOR3 *posB, D3DXVECTOR2 *sizeA = NULL, D3DXVECTOR2 *sizeB = NULL);

	//* [contents] ���_�ʒu�̐ݒ�
	//* [in] ���_�^�C�v, �ʒu, �傫��, ���a, ��]��
	//* [out] ���_�ʒu
	static void SetVatexPostion(D3DXVECTOR3 **ppOut,ORIGINVERTEXTYPE type, D3DXVECTOR3 *pPos, D3DXVECTOR2 *pSize, float fRadius = 0.0f, float fRotation = 0.0f);

	//-------------------------------------------------------------------------------------------------------------
	// 3D
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] 3D�x�N�g���𐶐�����
	//* [in] 3DposA�A3DposB
	//* [out] ��������3D�x�N�g��
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3D�x�N�g���𐶐�����
	//* [in] 2DposA�A2DposB
	//* [out] ��������3D�x�N�g��
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 3D�x�N�g���𐶐�����
	//* [in] 2DposA�A3DposB
	//* [out] ��������3D�x�N�g��
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR2 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3D�x�N�g���𐶐�����
	//* [in] 3DposA�A2DposB
	//* [out] ��������3D�x�N�g��
	static void CreateVector3(D3DXVECTOR3 *output, CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR2 *posB);

	//* [contents] 3D�̊O�ς𐶐�����
	//* [in] 3DvecA�A3DvecB
	//* [out] ��������3D�O��
	static void Create3DCross(D3DXVECTOR3 *resulut, D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3D�̕��ʊO�ς𐶐�����
	//* [in] 3DvecA�A3DvecB
	//* [return] ������������3D�O��
	static float CreateflatCross(CONST D3DXVECTOR3 *vecA, CONST D3DXVECTOR3 *vecB);

	//* [contents] 3D�̓��ς𐶐�����
	//* [in] 3DvecA�A3DvecB
	//* [return] ��������3D����
	static float Create3DDot(D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3D�̓��ς𐶐�����
	//* [in] 3DvecA�A3DvecB
	//* [out] ��������3D����
	static void Create3DDot(float *out, D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3D��2D��̃x�N�g���̒��������߂�
	//* [in] 3DvecA�A3DvecB
	//* [out] ��������3D����
	static float GetFlatVecLength(D3DXVECTOR3 *vec);

	//* [contents] 3D��2D��̃x�N�g���̒��������߂�
	//* [in] 3Dvec
	//* [out] ���������x�N�g���̒���
	static float Get3DVecLength(D3DXVECTOR3 *vec);

	//* [contents] 3D��2�_�̃x�N�g���̒��������߂�
	//* [in] 3DvecA�A3DvecB
	//* [return] ���������x�N�g���̒���
	static float Get3DClossLength(D3DXVECTOR3 *vecA, D3DXVECTOR3 *vecB);

	//* [contents] 3D��2�_�̃x�N�g���̒��������߂�
	//* [in] 3Dvec
	//* [out] ���������x�N�g���̒���
	static void Get3DVecLength(float *out, D3DXVECTOR3 * vec);

	//* [contents] 3D��2�_�̊O�ς̒��������߂�
	//* [in] 3DposA�A3DposB
	//* [out] ���������x�N�g���̒���
	static void Get3DClossLength(float *out, D3DXVECTOR3 *posA, D3DXVECTOR3 *posB);

	//* [contents] 3D�̒P�ʃx�N�g���𐶐�
	//* [in] 3Dvec
	//* [out] ���������P�ʃx�N�g��
	static void CreateUnitVector(D3DXVECTOR3 *out, D3DXVECTOR3 *Vec);

	//* [contents] 3D�̒��_�Ԃ̋����̎擾
	//* [in] 3DposA�A3DposB
	//* [return] �v�Z��������
	static float Get3DToPointDistance(CONST D3DXVECTOR3 *posA, CONST D3DXVECTOR3 *posB);

	//* [contents] 3D�̍����̋���
	//* [in] 3D����
	//* [return] �v�Z��������
	static float Get3DToDiffDistance(D3DXVECTOR3 *diffPos);

	//* [contents] 3D�̍����̋���
	//* [in] 3D����
	//* [out] �v�Z��������
	static void Get3DToPointDistance(float *Length, D3DXVECTOR3 *diffPos);

	//* [contents] �|���S���̗��ɂ��邩�e�X�g
	//* [in] ���_�ʒuA, ���_�ʒuB, ���_�ʒuC , �ʒuP(�_P)
	//* [return] bool �i�͈͓� == true �͈͊O == false�j
	static bool TestIfPolygonBack(CONST D3DXVECTOR3 & posA, CONST D3DXVECTOR3 & posP, CONST D3DXVECTOR3 & nom);

	//* [contents] �|���S���͈̔͂ɂ��邩�e�X�g
	//* [in] ���_�ʒuA, ���_�ʒuB, ���_�ʒuC , �ʒuP(�_P)
	//* [return] bool �i�͈͓� == true �͈͊O == false�j
	static bool Test3DInsidePolygon(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP);

	//* [contents] �|���S���͈̔͂ɂ��邩�e�X�g
	//* [in] ���_�ʒuA, ���_�ʒuB, ���_�ʒuC , �ʒuP(�_P), �|���S���̖@��
	//* [return] bool �i�͈͓� == true �͈͊O == false�j
	static bool Test3DInsidePolygon(CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, CONST D3DXVECTOR3 &posC, CONST D3DXVECTOR3 &posP, CONST D3DXVECTOR3 & nom);

	//* [contents] �������m�̋����ƍŒZ�_���W�̎擾
	//* [in] �ʒuA , �x�N�g��A, �ʒuB, �x�N�g��B
	//* [out] ����, �ŒZ�_���WA, �ŒZ�_���WB
	static bool Calc2LineNearestDistAndPos(D3DXVECTOR3* posA, D3DXVECTOR3* vecA, D3DXVECTOR3* posB, D3DXVECTOR3* vecB, float* fOutDist, D3DXVECTOR3* pOutPosA, D3DXVECTOR3* pOutPosB);

	//* [contents] �O�ςœ��O�ʒu�֌W�𒲂ׂ�
	//* [in] �ʒuP(�_P), ���_�ʒuA, ���_�ʒuB
	//* [return] bool �i�� == true �O == false�j
	static bool CrossRelationship(D3DXVECTOR3 &posP, D3DXVECTOR3 &posA, D3DXVECTOR3 &posB);

	//* [contents] ���ςŕ\���ʒu�֌W�𒲂ׂ�
	//* [in] �ʒuP(�_P), ���_�ʒuA, ���_�ʒuB
	//* [return] bool �i�\ == true �� == false�j
	static bool DotRelationship(CONST D3DXVECTOR3 &Dot, CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, float fTolerance = 0.0f);

	//* [contents] 2D��ӂ���ςňʒu�֌W�𒲂ׂ�
	//* [in] �ʒuP(�_P), ���_�ʒuA, ���_�ʒuB
	//* [return] bool �i�\ == true �� == false�j
	static bool OneSideDotRelationship(CONST D3DXVECTOR3 &Dot, CONST D3DXVECTOR3 &posA, CONST D3DXVECTOR3 &posB, float fTolerance = 0.0f);

	//* [contents] 3D�_�ƒ����̋���
	//* [in] �ʒuP(�_P), ���_�ʒuA, ���_�ʒuB
	//* [return] �v�Z��������
	static float Get3DLengthDotAndLine(CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] 3D����̍ŋߓ_���Z�o
	//* [in] �ʒuP(�_P), ���_�ʒuA, ���_�ʒuB
	//* [out] �ŋߓ_
	static void Get3DNearestPointOnline(D3DXVECTOR3 *pOut, CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] 3D���ʏ�̓_�ƒ����̂̋���
	//* [in] �ʒuP(�_P), ���_�ʒuA, ���_�ʒuB
	//* [return] �v�Z��������
	static float Get3DFlatLengthDotAndLine(CONST D3DXVECTOR3 *posP, CONST D3DXVECTOR3 *pPosA, CONST D3DXVECTOR3 *pPosB);

	//* [contents] �r���{�[�h�̐ݒ�
	//* [in] �f�o�C�X�|�C���^
	//* [out] �}�g���b�N�X
	static void SetBillboard(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX *mtxOutput);

	//* [contents] �r���{�[�h���[��]�̑ł������̐ݒ�
	//* [in] �f�o�C�X�|�C���^
	//* [out] �}�g���b�N�X
	static void SetBillboardNotRoll(LPDIRECT3DDEVICE9 pDevice, D3DXMATRIX *mtxOutput);

	//-------------------------------------------------------------------------------------------------------------
	// 2,3D
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] �����^������
	//* [in] (����)�W��
	//* [out] 3DVec�ړ���
	static void SlowingMove(D3DXVECTOR3 *pMove, const float fCoefficient);

	//* [contents] �����^������
	//* [in] (����)�W��
	//* [out] �ړ���
	static void SlowingMove(float *pMove, const float fCoefficient);

	//* [contents] �Ǐ]�ړ�����
	//* [in] �ǂ��ʒu, �ǂ���ʒu, (�Ǐ])�W��
	//* [out] 3DVec�ړ���
	static void FollowingMove(D3DXVECTOR3 *move, const D3DXVECTOR3 *ChasPos, const D3DXVECTOR3 *ChasedPos, const float *SpeedFactor);

	//* [contents] �_�Ɠ_�̋������r����
	//* [in] ����, �\�[�X�i�����̏ꍇ�ǉ��\�[�X�Ȃ��j, �ǉ��\�[�X
	//* [return] bool �itrue �ݒ肵���������߂��Afalse �ݒ肵��������艓���j
	static bool TestToPointDistance(float fDistance, CONST D3DXVECTOR3 *Source, CONST D3DXVECTOR3 *AddSource = NULL);

	//* [contents] ��]�ʂ𒼂�
	//* [out] ����or��]��
	static void SetFixTheRotation(float *Rotation);

	//* [contents] ��]�ʂ𒼂�
	//* [out] Vec3����or��]��
	static void SetVec3FixTheRotation(D3DXVECTOR3 *Rotation);

	//-------------------------------------------------------------------------------------------------------------
	// COLLAR
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] COLORREF����D3DXCOLOR�ɕϊ�����(�A���t�@�l�Ȃ�)
	//* [in] �V�X�e���J���[
	//* [out] DirectX3D�J���[
	static void SetD3DXCOLORfromREF_WithoutAlpha(COLORREF *SystemColor, D3DXCOLOR *col);

	//* [contents]  COLORREF����D3DXCOLOR�ɕϊ�����
	//* [in] �V�X�e���J���[
	//* [out] DirectX3D�J���[
	static void SetD3DXCOLORfromREF(COLORREF *SystemColor, D3DXCOLOR *col);

	//-------------------------------------------------------------------------------------------------------------
	// Misc
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] �������擾����
	//* [in] �\�[�X�i�����j
	//* [out] ����
	static void GetDigits(int &out, int nSource);

	//* [contents] �������擾����
	//* [in] �\�[�X�i�����j
	//* [return] ����
	static int GetDigits(int nSource);

	//* [contents] �������擾����(��������)
	//* [in] �\�[�X, ����, ��i�i���j
	//* [out] ����
	static void GetDigitsEx(int &out, int nSource, int nRadix);

	//* [contents] �������擾����(��������)
	//* [in] �\�[�X, ����, ��i�i���j
	//* [return] ����
	static int GetDigitsEx(int nSource, int nRadix);

	//* [contents] �L�[ID�̎Q�Ƃ����L�[�����擾
	//* [in] �L�[ID
	//* [out] �L�[��
	static void GetKeyInputInfo(int nKeyID, STRING KeyName = NULL);

	//* [contents] �J��グ�������擾
	//* [in] �\�[�X, �
	//* [return] �J��グ����
	static int GetRoundedUp(int nSource, int nBase);

	//-------------------------------------------------------------------------------------------------------------
	// FILE
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] �e�L�X�g�t�@�C����������擾����
	//* [in] �t�@�C����, ���ږ�
	//* [return] ��
	static int GetNumberFromText(CONST_STRING TextFile, CONST_STRING SetItem);

	//* [contents] �e�L�X�g�t�@�C���ɃX�N���v�g�����݂��邩�`�F�b�N
	//* [in] �t�@�C���|�C���^
	//* [return] bool (true ���݂����Afalse ���݂��Ȃ�)
	static bool CheckforScript(FILE *pFile);

	//-------------------------------------------------------------------------------------------------------------
	// STRING
	//-------------------------------------------------------------------------------------------------------------

	//* [contents] ������̊m�ہA�ݒ�
	//* [in] �\�[�X(������)
	//* [return] ������|�C���^
	static STRING SetStringAlloc(CONST_STRING source);

	//* [contents] �����񂩂�int�^�ɕϊ�����(�����񂩂琔���𔲂����)
	//* [in] ������
	//* [return] �ϊ���������
	static int GetConvertStoI(STRING str);

	//* [contents] int�^���當����ɕϊ�����
	//* [in] �ϊ�����l�A��i�i���j
	//* [out] �ϊ�����������
	void ConvertItoS(STRING pOut, int nVal, int nRadix);

	//* [contents] int�^���當����𐶐����A�ϊ�����
	//* [in] ��i�i���j
	//* [return] �ϊ�����������
	STRING ConvertItoSAlloc(int nVal, int nRadix);

	//* [contents] int�^���當����𐶐����A�ϊ�����
	//* [in] �ϊ�����l�A��i�i���j
	//* [out] �ϊ�����������
	void ConvertItoSAlloc(char **ppOut, int nVal, int nRadix);

	//-------------------------------------------------------------------------------------------------------------
	// TEMPLATE
	//-------------------------------------------------------------------------------------------------------------
	/* MEMO
	* �G���[��������邽�߂�inline�֐��ɂ���
	*/

	// �Ⴄ�l�̎��ɐݒ�
	// input : (�ύX����f�[�^�A)
	template <class T>
	inline static bool Setdifferentvalue(T &tData, CONST T tSource)
	{
		if (tData != tSource)
		{
			tData = tSource;
			return true;
		}
		return false;
	}
};

}// namespace ikuminLib

using namespace ikuminLib;

// �C�����C���֐��p�t�@�C�����C���N���[�h
#include "Mylibrary.inl"

#endif