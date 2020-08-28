//*************************************************************************************************************
//
// �����G�t�F�N�g�̃I�t�Z�b�g����[CharEffectOffset.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "CharEffectOffset.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define CHAREFFEOFF_OPNEMODE "r"

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
CCharEffectOffset::OFFSET CCharEffectOffset::m_OffSet[CCharEffectOffset::OFFSETNAME::OFFSET_MAX] = {};		// �I�t�Z�b�g���

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Load(void)
{
#ifdef _DEBUG
	DWORD start = timeGetTime();			// �v���X�^�[�g����
#endif // _DEBUG

	// �t�@�C����
	CONST_STRING pFileName[OFFSETNAME::OFFSET_MAX] = {
		{ "data/TEXT/Effect/CharEffectOffset/Offset_�h���b.txt" },
		{ "data/TEXT/Effect/CharEffectOffset/Offset_�S�b.txt" },
		{ "data/TEXT/Effect/CharEffectOffset/Offset_�L�[��.txt" },

	};

	for (int nCntOffSet = 0; nCntOffSet < OFFSETNAME::OFFSET_MAX; nCntOffSet++)
	{
		// �t�@�C������ǂݍ���
		LoadFromFile(pFileName[nCntOffSet], nCntOffSet);
	}

#ifdef _DEBUG
	DWORD end = timeGetTime();			// �v���X�^�[�g����

	cout << "\nCCharEffectOffset::Load �I�t�Z�b�g���̓ǂݍ��ݏI��\n";
	cout << "CCharEffectOffset::Load �I�t�Z�b�g���̓ǂݍ��� �������x = " << (end - start) << "�@[" << (end - start) * 0.001f << "sec.]\n";
#endif // _DEBUG


}

//-------------------------------------------------------------------------------------------------------------
// �t�@�C������ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::LoadFromFile(CONST_STRING pFileName, const int & nCntOffSet)
{
	// �ϐ��錾
	FILE *pFile;						// �t�@�C���|�C���^
	char aRead[MYLIB_STRINGSIZE];		// �ǂݍ��ݗp
	char aComp[MYLIB_STRINGSIZE];		// ��r�p
	char aWork[MYLIB_STRINGSIZE];		// ��Ɨp
	int  nCntParam;						// ���J�E���g
	int  nNumParam;						// �g���p�����[�^��
	int  nParent;						// �e�t���O

	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	aWork[0] = MYLIB_CHAR_UNSET;
	nCntParam = MYLIB_INT_UNSET;
	nNumParam = MYLIB_INT_UNSET;
	nParent  = MYLIB_INT_UNSET;


	// �t�@�C�����J��
	if ((pFile = fopen(pFileName, CHAREFFEOFF_OPNEMODE)) == NULL)
	{// ���s������
		return;
	}

	// �X�N���v�g
	while (strcmp(aComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%s", &aComp);
	}

	// �G���h�X�N���v�g
	while (strcmp(aComp, "END_SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%s", &aComp);

		
		if (sscanf(aRead, "NUMPARAM = %d", &nNumParam) == 1)
		{
			m_OffSet[nCntOffSet].nNumParam = nNumParam;
			m_OffSet[nCntOffSet].pCell = new PARAMCELL[m_OffSet[nCntOffSet].nNumParam];
		}
		else if (sscanf(aRead, "PARENT = %d", &nParent) == 1)
		{
			m_OffSet[nCntOffSet].bParent = (nParent != 0);
		}
		else if (strcmp(aComp, "SETPARAM") == 0)
		{
			LoadParamFromFile(pFile, nCntOffSet, nCntParam);
			nCntParam++;
			aComp[0] = MYLIB_CHAR_UNSET;
		}
	}
	fclose(pFile);
}

//-------------------------------------------------------------------------------------------------------------
//�@�t�@�C������p�����[�^��ǂ݂���
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::LoadParamFromFile(FILE * pFile, const int & nCntOffSet, const int &nCntParam)
{
	// �ϐ��錾
	PARAMCELL Seting;					// �ݒ�p�I�v�V����
	char aRead[MYLIB_STRINGSIZE];		// �ǂݍ��ݗp
	char aComp[MYLIB_STRINGSIZE];		// ��r�p
	char aWork[MYLIB_STRINGSIZE];		// ��Ɨp
	int nBillBoard;						// �r���{�[�h�t���O
	Seting = MYLIB_INITSTRUCT_WITHCONST;
	aRead[0] = MYLIB_CHAR_UNSET;
	aComp[0] = MYLIB_CHAR_UNSET;
	aWork[0] = MYLIB_CHAR_UNSET;
	nBillBoard = MYLIB_INT_UNSET;

	// �G���h�p����
	while (strcmp(aComp, "END_SETPARAM") != 0)
	{
		STRING pFIleRead = &aRead[0];
		// 1�s�ǂݍ���
		fgets(pFIleRead, MYLIB_STRINGSIZE, pFile);
		// �ǂݍ���Ǖ�������
		sscanf(pFIleRead, "%s", &aComp);

		while (*pFIleRead == '\t')
		{
			pFIleRead++;
		}

		if (sscanf(pFIleRead, "TIME = %d", &Seting.nTime) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].nTime = Seting.nTime;
		}
		else if (sscanf(pFIleRead, "BILLBOARD = %d", &nBillBoard) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].bBillBoard = (nBillBoard != 0);
		}
		else if (sscanf(pFIleRead, "TYPE = %d", &Seting.type) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].type = Seting.type;
		}
		else if (sscanf(pFIleRead, "TEXTYPE = %d", &Seting.nTexType) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].nTexType = Seting.nTexType;
		}
		else if (sscanf(pFIleRead, "POS = %f %f %f", &Seting.pos.x, &Seting.pos.y, &Seting.pos.z) == 3)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].pos = Seting.pos;
		}
		else if (sscanf(pFIleRead, "MOVE = %f %f %f", &Seting.move.x, &Seting.move.y, &Seting.move.z) == 3)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].move = Seting.move;
		}
		else if (sscanf(pFIleRead, "COL = %f %f %f %f", &Seting.col.r, &Seting.col.g, &Seting.col.b, &Seting.col.a) == 4)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].col = Seting.col;
		}
		else if (sscanf(pFIleRead, "RADIUS = %f", &Seting.fRadius) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].fRadius = Seting.fRadius;
		}
		else if (sscanf(pFIleRead, "LIFE = %d", &Seting.nLife) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].nLife = Seting.nLife;
		}
		else if (sscanf(pFIleRead, "GRAVITY = %f", &Seting.fGravity) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].fGravity = Seting.fGravity;
		}
		else if (sscanf(pFIleRead, "RADIUSVALUE = %f", &Seting.fRadiusValue) == 1)
		{
			m_OffSet[nCntOffSet].pCell[nCntParam].fRadiusValue = Seting.fRadiusValue;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Unload(void)
{
	for (int nCntOffSet = 0; nCntOffSet < OFFSETNAME::OFFSET_MAX; nCntOffSet++)
	{
		if (m_OffSet[nCntOffSet].pCell != nullptr)
		{
			delete[] m_OffSet[nCntOffSet].pCell;
			m_OffSet[nCntOffSet].pCell = nullptr;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Init(void)
{
	m_nFlame = MYLIB_INT_UNSET;
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Update(void)
{
	// �ϐ��錾
	PARAMCELL* pParamCell = &m_OffSet[m_Name].pCell[0];		// �p�����[�^�P�̂̃|�C���^

	for (int nCntParam = 0; nCntParam < m_OffSet[m_Name].nNumParam; nCntParam++)
	{
		if (pParamCell[nCntParam].nTime != m_nFlame)
		{
			continue;
		}
		pParamCell[nCntParam].pos += m_pos;
		C3DEffect::Set(pParamCell[nCntParam]);

		pParamCell[nCntParam].pos -= m_pos;
	}

	// �J���m�F
	if (ReleaseCheck() == true)
	{
		CScene::Release();
	}

	// �t���[����i�߂�
	m_nFlame++;
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �ݒ�
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::Set(D3DXVECTOR3 * pParent, CONST OFFSETNAME & name)
{
	CCharEffectOffset *pOffset = new CCharEffectOffset;
	pOffset->Init();
	pOffset->SetName(name);
	pOffset->SetPos(pParent);
}

//-------------------------------------------------------------------------------------------------------------
// �J���m�F
//-------------------------------------------------------------------------------------------------------------
bool CCharEffectOffset::ReleaseCheck(void)
{
	// �o���J�E���g
	int nCntApea = MYLIB_INT_UNSET;
	for (int nCntParam = 0; nCntParam < m_OffSet[m_Name].nNumParam; nCntParam++)
	{
		if (m_OffSet[m_Name].pCell[nCntParam].nTime < m_nFlame)
		{
			nCntApea++;
		}
	}

	if (nCntApea == m_OffSet[m_Name].nNumParam)
	{
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// �ʒu�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::SetPos(D3DXVECTOR3 * pParent)
{
	if (m_OffSet[m_Name].bParent == true)
	{
		m_pParent = pParent;
		m_pos = MYLIB_VEC3_UNSET;
	}
	else
	{
		m_pParent = nullptr;
		m_pos = *(D3DXVECTOR3 *)pParent;
	}
	m_pos = *(D3DXVECTOR3 *)pParent;
	m_pParent = &m_pos;
}

//-------------------------------------------------------------------------------------------------------------
// ���O�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void CCharEffectOffset::SetName(CONST OFFSETNAME & name)
{
	m_Name = name;
}

