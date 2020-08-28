//*************************************************************************************************************
//
// �����G�t�F�N�g�̃I�t�Z�b�g����[CharEffectOffset.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _CHAREFFECTOFFSET_H_
#define _CHAREFFECTOFFSET_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "3DEffect.h"

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CCharEffectOffset : public CScene
{
public:
	// �񋓌^��`
	typedef enum
	{
		OFFSET_NONE = -1,
		STR_�h���b,
		STR_�S�b,
		STR_�L�[��,
		OFFSET_MAX
	} OFFSETNAME;

	typedef struct PARAMCELL : public C3DEffect::SETINGPARAM
	{
		PARAMCELL() : nTime(MYLIB_INT_UNSET), SETINGPARAM() {}
		int nTime;		// �o������
	} PARAMCELL;

	typedef struct
	{
		int          nNumParam;		// �g���p�����[�^��
		bool         bParent;		// �e�t���O
		PARAMCELL*   pCell;			// �p�����[�^�P�̂̈ʒu
	} OFFSET;

	CCharEffectOffset() : CScene(PRIORITY_EFFECT) {}											// �R���X�g���N�^
	~CCharEffectOffset() {}																		// �f�X�g���N�^

	static void Load(void);																		// �ǂݍ���
	static void LoadFromFile(CONST_STRING pFileName, const int &nCntOffSet);					// �t�@�C������ǂݍ���
	static void LoadParamFromFile(FILE *pFile, const int &nCntOffSet, const int &nCntParam);	// �t�@�C������p�����[�^��ǂݍ���

	static void Unload(void);																	// �J��

	void Init(void);																			// ����������
	void Uninit(void);																			// �I������
	void Update(void);																			// �X�V����
	void Draw(void);																			// �`�揈��

	static void Set(D3DXVECTOR3* pParent, CONST OFFSETNAME & name);	// �ݒ�

private:
	bool ReleaseCheck(void);

	void SetPos(D3DXVECTOR3* pParent);
	void SetName(CONST OFFSETNAME& name);

	static OFFSET m_OffSet[OFFSETNAME::OFFSET_MAX];		// �I�t�Z�b�g���
	D3DXVECTOR3* m_pParent;		// �e�̈ʒu
	D3DXVECTOR3  m_pos;			// �ʒu
	OFFSETNAME   m_Name;		// �I�t�Z�b�g��
	int          m_nFlame;		// �t���[���J�E���g
};

#endif