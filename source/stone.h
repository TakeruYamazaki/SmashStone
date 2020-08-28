//*************************************************************************************************************
//
// �X�g�[������[stone.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _STONE_H_
#define _STONE_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "sceneX.h"
#include "Mylibrary.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#ifndef CSTONE_USE_TEXTURE
//#define CSTONE_USE_TEXTURE	// �e�N�X�`�����g��
#endif // !CSTONE_USE_TEXTURE

#ifndef CSTONE_USE_NULLPTR
#define CSTONE_USE_NULLPTR		// nullptr���g��
#endif // CSTONE_USE_NULL

#ifndef CSTONE_UPDATE
#define CSTONE_UPDATE
#endif // !CSTONE_UPDATE

#ifndef CSTONE_DRAW
#define CSTONE_DRAW
#endif // !CSTONE_DRAW


// DEBUG��
#ifdef _DEBUG

#ifndef CSTONE_DEBUG_DRAW
//#define CSTONE_DEBUG_DRAW
#endif // !CSTONE_DEBUG_DRAW

#ifndef CSTONE_LAOD_DEBUG_DRAW
#define CSTONE_LAOD_DEBUG_DRAW
#endif // !CSTONE_LAOD_DEBUG_DRAW

//#undef CSTONE_UPDATE
//#undef CSTONE_DRAW
#endif // _DEBUG


//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CStone : public CSceneX
{
public:
	typedef enum
	{
		STONE_ID_NONE = -1,
		STONE_ID_RED,
		STONE_ID_GREEN,
		STONE_ID_BLUE,
		STONE_ID_MAX
	} STONE_ID;

	// ���f�����
	typedef struct
	{
		LPD3DXMESH			pMesh;			// ���b�V�����̃|�C���^
		LPD3DXBUFFER		pBuffMat;		// �}�e���A�����̃|�C���^
		DWORD				nNumMat;		// �}�e���A���̐�
		D3DXVECTOR3			vtxMax;			// �ő咸�_
		D3DXVECTOR3			vtxMin;			// �ŏ����_
		float				*pAlphaValue;	// �A���t�@�l�̃|�C���^
#ifdef CSTONE_USE_TEXTURE
		int					*pTextureID;	// �e�N�X�`��ID
		LPDIRECT3DTEXTURE9	*ppTexture;		// �e�N�X�`�����
		int					nNumTexture;	// �e�N�X�`����
#endif
	} MODEL_INFO;

	// �R���X�g���N�^
	CStone();
	// ��������R���X�g���N�^
	CStone(PRIORITY type);
	// �f�X�g���N�^
	~CStone() {};
	// �ǂݍ���(�}�l�[�W���[�̏������ɓ����)
	static HRESULT Load(void);
	// �J��(�}�l�[�W���[�̏I�������ɓ����)
	static void Unload(void);
	// �����̒��B
	static CString *GetResource(void);

	// ����
	static CStone* Create(
		CONST int nIndexPos,
		CONST STONE_ID     eumID = STONE_ID_NONE,
		CONST D3DXVECTOR3  &pos = MYLIB_3DVECTOR_ZERO
	);

	// ������
	void Init(void);
	// �I��
	void Uninit(void);
	// �X�V
	void Update(void);
	// �`��
	void Draw(void);

	// ���肷��
	void Catch(void);
	// �X�g�[���ւ̃_���[�W
	void Damege(int nDamege) { m_nLife -= nDamege; }
	// �����ʒu�̎擾
	int GetIndexPos(void) { return m_nIndexPos; }
	// �X�g�[����ID�̎擾
	int GetStoneID(void) { return m_enmStoneID; }
	// ���C�t�̎擾
	int GetLife(void) { return m_nLife; }
	// �_���[�W�̔������C�t0�̎� true
	bool ApplyDamage(void);


private:
	/* �����o�֐� */
	// �ő�ŏ��̒��_�ʒu�̎擾
	static void GetMaxMinVertices(int nIndex);
#ifdef CSTONE_USE_TEXTURE
	// X�t�@�C������擾�����e�N�X�`������ID�̎擾����
	static void CreateTexturefrom_Xfile(const int nIndex, CONST STRING sFileName);
#endif
	/* �����o�ϐ��@*/
	static MODEL_INFO* m_pAllStoneTypeInfo;		// �X�g�[���̑S�Ẵ��f�����
	static int         m_nNumTypeAll;			// �X�g�[���̎�ސ�
	static const float m_fDoublePi;				// �~������2�{
	STONE_ID           m_enmStoneID;			// �X�g�[��ID
	unsigned int       m_fCntShake;				// ����J�E���g
	MODELINFO          m_pModelInfo;			// ���f�����Q�Ƃ��邽�߂̏��
	int                m_nBoxClliderID;			// �{�b�N�X�R���C�_�[ID
	int				   m_nIndexPos;				// �������W�̔ԍ�
	int				   m_nLife;					// ���C�t
	bool			   m_bDamage;				// �_���[�W���󂯂�t���O
#ifdef CSTONE_DEBUG_DRAW
	static int         m_nNumAll;				// �S�Ă̌�
	int                m_nNumID;				// ��ID(����)
#endif // CSTONE_DEBUG_DRAW
};




#endif