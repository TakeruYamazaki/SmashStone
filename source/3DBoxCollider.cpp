//*************************************************************************************************************
//
//�{�b�N�X�R���C�_�[����[3DBoxCollider.cpp]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#include "3DBoxCollider.h"
#include "DebugProc.h"
#include "mouse.h"
#include "game.h"
#include "MeshField.h"
#include "manager.h"
#include "renderer.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define _3DBOXCOLLIDER_HALF_SIZE	0.5f								// �T�C�Y�̔���
#define _3DBOXCOLLIDER_FILENAME		"data/TEXT/ColliInfo/3DBox.csv"		// �t�@�C����
#define _3DBOXCOLLIDER_OPENMODE		"r"									// �t�@�C���̊J�����[�h
// �t�@�C���ǂݍ������s����
#define _3DBOXCOLLIDER_LOADFLAG_
#undef _3DBOXCOLLIDER_LOADFLAG_

#define _3DBOXCOLLI_WORDSIZE					16		// ���[�h�T�C�Y

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9			   C3DBoxCollider::m_pVtxBuff = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9				   C3DBoxCollider::m_pTexture = NULL;					// �e�N�X�`���̏��̃|�C���^
LPDIRECT3DINDEXBUFFER9			   C3DBoxCollider::m_pIdxBuff = NULL;					// �C���f�b�N�X�̃o�b�t�@�̃|�C���^
C3DBoxCollider::READINFOFILEBUFFER C3DBoxCollider::m_ReadInfoFileBuff;					// �ǂ݂��񂾃t�@�C�����
C3DBoxCollider::_3DBOXCOLLIDER	   C3DBoxCollider::m_ColliderInfo[_3DBOXCOLLIDER_MAX];	// �R���C�_�[���

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
C3DBoxCollider::C3DBoxCollider(CScene::PRIORITY priority) : CScene(priority)
{
}

//-------------------------------------------------------------------------------------------------------------
// �ǂݍ���
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DBoxCollider::Load(void)
{
	// �ǂ݂��񂾃t�@�C�����̏�����
	m_ReadInfoFileBuff.nNumReadInfo	= MYLIB_INT_UNSET;
	m_ReadInfoFileBuff.pCell		= NULL;
	m_ReadInfoFileBuff.pSetThisID	= NULL;
#ifdef _DEBUG
	cout << "---------------------------------------------------------------------\n";
	cout << "C3DBoxCollider::Load �{�b�N�X�R���C�_�[�̓ǂݍ��݊J�n\n";
	DWORD start = timeGetTime();			// �v���X�^�[�g����
#endif
	// �ϐ��錾
	FILE *pFile = fopen(_3DBOXCOLLIDER_FILENAME, _3DBOXCOLLIDER_OPENMODE);
	// �J���Ȃ�����
	if (pFile == NULL)
	{
#ifdef _DEBUG
		cout << "C3DBoxCollider::Load �{�b�N�X�R���C�_�[�̓ǂݍ��݂̃t�@�C�����J���܂���ł���\n";
		cout << "---------------------------------------------------------------------\n";
#endif
		return E_FAIL;
	}

	// �ϐ��錾
	char aRead[MYLIB_STRINGSIZE];		// �ǂݍ��ݗp
	char aComp[MYLIB_STRINGSIZE];		// ��r�p
	char aEmpty[MYLIB_STRINGSIZE];		// �v��Ȃ����̗p
	int  nCntInfo;						// ���J�E���g

	aRead[0] = MYLIB_CHAR_UNSET;		// �ǂݍ��ݗp
	aComp[0] = MYLIB_CHAR_UNSET;		// ��r�p
	aEmpty[0] = MYLIB_CHAR_UNSET;		// �v��Ȃ����̗p
	nCntInfo = MYLIB_INT_UNSET;			// ���J�E���g

#ifdef _DEBUG
	int  nCntError;						// �G���[�J�E���g
	nCntError = MYLIB_INT_UNSET;		// �G���[�J�E���g
#endif
	// �X�N���v�g������܂łƂ΂�
	while (strcmp(aComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%[^,]s", &aComp);
#ifdef _DEBUG
		// �G���[�J�E���g���C���N�������g
		if (++nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
#endif
	}
	// END_SCRIPT�܂Ń��[�v
	while (strcmp(aComp, "END_SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(aRead, sizeof(aRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(aRead, "%[^,]s", &aComp);
#ifdef _DEBUG
		// �G���[�J�E���g���C���N�������g
		if (++nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			fclose(pFile);
			return E_FAIL;
		}
#endif
		if (m_ReadInfoFileBuff.nNumReadInfo <= 0)
		{
			// �ݒ肷���
			if (strcmp(aComp, "SET_NUM") == 0)
			{
				sscanf(aRead, "%[^,],%d", &aEmpty, &m_ReadInfoFileBuff.nNumReadInfo);
				// �P�̏�����������
				m_ReadInfoFileBuff.pCell = new READINFOFILE_CELL[m_ReadInfoFileBuff.nNumReadInfo];
				// ����������ID����������
				m_ReadInfoFileBuff.pSetThisID = new int[m_ReadInfoFileBuff.nNumReadInfo];
				// ������̏�����
				aComp[0] = '\0';
				cout << "�ǂ݂��݌����擾���܂���\n";
			}
		}
		else
		{
			// �ݒ肷�����
			if (strcmp(aComp, "SET") == 0)
			{
				// �ϐ��錾
				char aSizeWord[_3DBOXCOLLI_WORDSIZE];	// �傫���̃t���O����
				char aDiffWord[_3DBOXCOLLI_WORDSIZE];	// �����̃t���O����
				READINFOFILE_CELL *pCell;					// �Z���|�C���^

				// ������
				aSizeWord[0] = MYLIB_CHAR_UNSET;
				aDiffWord[0] = MYLIB_CHAR_UNSET;
				pCell = &m_ReadInfoFileBuff.pCell[nCntInfo];
				/* ��s���̏�����͂��� */
				//			   SET     ����   ID   �傫��X �傫��Y �傫��X ����X   ����Y   ����Z   �^�C�v
				sscanf(aRead, "%[^, ], %[^, ], %d, %[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %d",
					&aEmpty, &aEmpty,
					&m_ReadInfoFileBuff.pSetThisID[nCntInfo],
					&aSizeWord,
					&aEmpty, &aEmpty,
					&aDiffWord,
					&aEmpty, &aEmpty, 
					&pCell->nColliderType);

				// �T�C�Y��ݒ肷�鎞
				if (strcmp(aSizeWord, "UNSET") != 0)
				{
					// �T�C�Y�̐���
					pCell->pSize = new D3DXVECTOR3;
					// �傫����ǂݍ���
					sscanf(aRead, "%[^, ], %[^, ], %[^, ], %f, %f, %f, ",
						&aEmpty, &aEmpty, &aEmpty,
						&pCell->pSize->x,
						&pCell->pSize->y,
						&pCell->pSize->z);
				}
				else
				{// ����ȊO�̎�
					pCell->pSize = NULL;
				}
				// ������ݒ肷�鎞
				if (strcmp(aDiffWord, "UNSET") != 0)
				{
					// �����̐���
					pCell->pDifference = new D3DXVECTOR3;
					// ������ǂݍ���
					sscanf(aRead, "%[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %[^, ], %f, %f, %f, ",
						&aEmpty, &aEmpty, &aEmpty, &aEmpty, &aEmpty, &aEmpty,
						&pCell->pDifference->x,
						&pCell->pDifference->y,
						&pCell->pDifference->z);
				}
				else
				{// ����ȊO�̎�1
					pCell->pDifference = NULL;
				}
				// ���J�E���g�C���N�������g
				nCntInfo++;
				// ������̏�����
				aComp[0] = '\0';
#ifdef _DEBUG
				cout << "[" << nCntInfo << "]�ڂ̏����擾���܂���\n";
#endif
			}
		}
	}
	fclose(pFile);
#ifdef _DEBUG
	DWORD end = timeGetTime();			// �v���X�^�[�g����
	cout << "C3DBoxCollider::Load �{�b�N�X�R���C�_�[�̓ǂݍ��ݏI��\n";
	cout << "C3DBoxCollider::Load �{�b�N�X�R���C�_�[�̓ǂݍ��� �������x = " << (end - start) << "�@[" << (end - start) * 0.001f << "sec.]\n";
#endif
	cout << "---------------------------------------------------------------------\n";
	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------
// �J��
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Unload(void)
{
	// �P�̏��̌������[�v
	for (int nCntCell = 0; nCntCell < m_ReadInfoFileBuff.nNumReadInfo; nCntCell++)
	{
		// �傫���̔j��
		if (m_ReadInfoFileBuff.pCell[nCntCell].pSize != NULL)
		{
			delete m_ReadInfoFileBuff.pCell[nCntCell].pSize;
			m_ReadInfoFileBuff.pCell[nCntCell].pSize = NULL;
		}
		// �����̔j��
		if (m_ReadInfoFileBuff.pCell[nCntCell].pDifference != NULL)
		{
			delete m_ReadInfoFileBuff.pCell[nCntCell].pDifference;
			m_ReadInfoFileBuff.pCell[nCntCell].pDifference = NULL;
		}
	}
	// �P�̏���j��
	if (m_ReadInfoFileBuff.pCell != NULL)
	{
		delete[]m_ReadInfoFileBuff.pCell;
		m_ReadInfoFileBuff.pCell = NULL;
	}
	// ����������ID�̔j��
	if (m_ReadInfoFileBuff.pSetThisID != NULL)
	{
		delete[]m_ReadInfoFileBuff.pSetThisID;
		m_ReadInfoFileBuff.pSetThisID = NULL;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
C3DBoxCollider * C3DBoxCollider::Create(void)
{
	// �ϐ��錾
	C3DBoxCollider *p3DCollider = new C3DBoxCollider(CScene::PRIORITY_COLLISION);	// 3D�{�b�N�X�R���C�_�[�|�C���^

	// ������
	p3DCollider->Init();

	return p3DCollider;
}

//-------------------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Init(void)
{
	// �l���̏�����
	InitColliderInfo();

	// ���_�̍쐬
	MakeVertex();

	// �C���f�b�N�X�̍쐬
	MakeIndex();
}

//-------------------------------------------------------------------------------------------------------------
// �I��
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Uninit(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �X�V
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Update(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// �`��
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::Draw(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// ���_�o�b�t�@�𐶐�
	D3DXMATRIX mtxTrans, mtwWorld;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);						// ���C�e�B���O���[�h����

	int nCntVertexIndex = 0;
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++)
	{
		if (m_ColliderInfo[nCntCollider].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_ColliderInfo[nCntCollider].mtxWorld);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(
				&mtxTrans,
				m_ColliderInfo[nCntCollider].pos.x,
				m_ColliderInfo[nCntCollider].pos.y,
				m_ColliderInfo[nCntCollider].pos.z);

			D3DXMatrixMultiply(
				&m_ColliderInfo[nCntCollider].mtxWorld,
				&m_ColliderInfo[nCntCollider].mtxWorld,
				&mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &m_ColliderInfo[nCntCollider].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���X�g���[���ɐݒ�
			pDevice->SetIndices(m_pIdxBuff);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);
#ifdef _DEBUG
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_LINELIST,
				nCntVertexIndex,
				0,
				_3DBOXCOLLIDER_NUM_INDEX,
				0,
				_3DBOXCOLLIDER_NUM_PRIMITIVE);
#endif
		}
		nCntVertexIndex += _3DBOXCOLLIDER_USE_VERTEX;
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ���C�e�B���O���[�h����

}

//-------------------------------------------------------------------------------------------------------------
// �Փ�
//-------------------------------------------------------------------------------------------------------------
bool C3DBoxCollider::Collision(D3DXVECTOR3 & pos)
{
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX)
	{
		if (m_ColliderInfo[nCntCollider].bUse)
		{
			//if (CManager::GetMouse()->GetTrigger(0))
			//{
			//	bool bOk = false;
			//	if (CMylibrary::Test3DInsidePolygon(m_ColliderInfo[nCntCollider].Vertex[0], m_ColliderInfo[nCntCollider].Vertex[1], m_ColliderInfo[nCntCollider].Vertex[2], pos, m_ColliderInfo[nCntCollider].surfaceNom[C3DBoxCollider::SURFACE_TYPE_TOP]))
			//	{
			//		bOk = true;
			//	}
			//	if (CMylibrary::Test3DInsidePolygon(m_ColliderInfo[nCntCollider].Vertex[2], m_ColliderInfo[nCntCollider].Vertex[3], m_ColliderInfo[nCntCollider].Vertex[0], pos, m_ColliderInfo[nCntCollider].surfaceNom[C3DBoxCollider::SURFACE_TYPE_TOP]))
			//	{
			//		bOk = true;
			//	}
			//	if (bOk == false)
			//	{
			//		continue;
			//	}
			//	bOk = false;
			//	if (CMylibrary::Test3DInsidePolygon(m_ColliderInfo[nCntCollider].Vertex[4], m_ColliderInfo[nCntCollider].Vertex[5], m_ColliderInfo[nCntCollider].Vertex[6], pos, m_ColliderInfo[nCntCollider].surfaceNom[C3DBoxCollider::SURFACE_TYPE_BOTTOM]))
			//	{
			//		bOk = true;
			//	}
			//	else if (CMylibrary::Test3DInsidePolygon(m_ColliderInfo[nCntCollider].Vertex[6], m_ColliderInfo[nCntCollider].Vertex[7], m_ColliderInfo[nCntCollider].Vertex[4], pos, m_ColliderInfo[nCntCollider].surfaceNom[C3DBoxCollider::SURFACE_TYPE_BOTTOM]))
			//	{
			//		bOk = true;
			//	}
			//	if (bOk == false)
			//	{
			//		continue;
			//	}
			//}
			return true;
		}
	}
	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
	return false;
}

//-------------------------------------------------------------------------------------------------------------
// �_���[�W����
//-------------------------------------------------------------------------------------------------------------
bool C3DBoxCollider::CollisionDamage(int n3DBoxColliderID, int nNoneColisitionID)
{
	// 3D�{�b�N�X�R���C�_�[�̃|�C���^
	_3DBOXCOLLIDER *pOwnerCollider = &m_ColliderInfo[n3DBoxColliderID];	// ������̃|�C���^
	_3DBOXCOLLIDER *pOtherCollider = &m_ColliderInfo[0];				// ���̑��̃|�C���^
	bool		   bCollision	   = false;								// �Փ˃t���O
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �R���C�_�[�������[�v
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX, pOtherCollider++)
	{
		if (nCntCollider == n3DBoxColliderID ||
			nCntCollider == nNoneColisitionID)
		{// �������͏��O
			continue;
		}
		if (pOtherCollider->bUse == false)
		{// �g�p���Ă��Ȃ����̂͏��O
			continue;
		}
		if (pOtherCollider->pScene == NULL)
		{// Scene���ݒ肳��Ă��Ȃ����̂͏��O
			continue;
		}
		//if (pOtherCollider->pScene->GetObjectTyoe() != CScene::TYPE_ENEMY &&
		//	pOtherCollider->pScene->GetObjectTyoe() != CScene::TYPE_PLAYER)
		//{// �G�ƃv���C���[�ȊO�̎����O
		//	continue;
		//}
		if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_NORMAL)
		{
			// Y�����d�Ȃ��Ă���Ƃ�
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				// Z���͈̔�
				if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// ���̓����蔻��
					if (pOwnerCollider->pos.x + pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						return true;
					}
					// �E�̓����蔻��
					else if (pOwnerCollider->pos.x - pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						return true;
					}
				}
				// X���͈̔�
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// ���̓����蔻��
					if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						return true;
					}
					// �E�̓����蔻��
					else if (pOwnerCollider->pos.z - pOwnerCollider->size.z* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						return true;
					}
				}
			}
			if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
				pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
			{
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					if (pOwnerCollider->pos.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						return true;
					}
					else if (pOwnerCollider->pos.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						return true;
					}
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_CYLINDER)
		{
			// Y�����d�Ȃ��Ă���Ƃ�
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
				float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + pOtherCollider->size.x;
				if (diffPos.x*diffPos.x +
					diffPos.y*diffPos.y <=
					(fRadius*fRadius))
				{

					// ���_�f�[�^���A�����b�N
					m_pVtxBuff->Unlock();
					return true;
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_SPHERE)
		{
			// �G�ƃv���C���[�̍��������߂�
			D3DXVECTOR3 diffPos = pOwnerCollider->pos - pOtherCollider->pos;
			float fRadius = pOwnerCollider->size.x + pOtherCollider->size.x;

			// �O�����̒藝����G�ƃv���C���[�̋������r
			if (diffPos.x*diffPos.x +
				diffPos.y*diffPos.y +
				diffPos.z*diffPos.z <=
				(fRadius* fRadius))
			{
				// ���_�f�[�^���A�����b�N
				m_pVtxBuff->Unlock();
				return true;
			}
		}
	}
	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
	return bCollision;
}

//-------------------------------------------------------------------------------------------------------------
// �Փ˂�������������
//-------------------------------------------------------------------------------------------------------------
bool C3DBoxCollider::Collisionoverlap(int n3DBoxColliderID,int* pHitID, int nNoneColisitionID)
{
	// �ϐ��錾
	_3DBOXCOLLIDER *pOwnerCollider = &m_ColliderInfo[n3DBoxColliderID];	// ������̃|�C���^
	_3DBOXCOLLIDER *pOtherCollider = &m_ColliderInfo[0];				// ���̑��̃|�C���^
	bool		   bCollision	   = false;								// �Փ˃t���O
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �R���C�_�[�������[�v
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX, pOtherCollider++)
	{
		if (nCntCollider == n3DBoxColliderID ||
			nCntCollider == nNoneColisitionID)
		{// �������͏��O
			continue;
		}
		if (pOtherCollider->bUse == false)
		{// �g�p���Ă��Ȃ����̂͏��O
			continue;
		}
		if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_NORMAL)
		{
			// Y�����d�Ȃ��Ă���Ƃ�
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				// Z���͈̔�
				if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// ���̓����蔻��
					if (pOwnerCollider->pos.x + pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
					// �E�̓����蔻��
					else if (pOwnerCollider->pos.x - pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
				}
				// X���͈̔�
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// ���̓����蔻��
					if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
					// �E�̓����蔻��
					else if (pOwnerCollider->pos.z - pOwnerCollider->size.z* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
				}
			}
			if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
				pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
			{
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					if (pOwnerCollider->pos.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
					else if (pOwnerCollider->pos.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();
						*pHitID = nCntCollider;
						return true;
					}
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_CYLINDER)
		{
			// Y�����d�Ȃ��Ă���Ƃ�
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
				float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + pOtherCollider->size.x;
				if (diffPos.x*diffPos.x +
					diffPos.y*diffPos.y <=
					(fRadius*fRadius))
				{

					// ���_�f�[�^���A�����b�N
					m_pVtxBuff->Unlock();
					*pHitID = nCntCollider;
					return true;
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_SPHERE)
		{
			// �G�ƃv���C���[�̍��������߂�
			D3DXVECTOR3 diffPos = pOwnerCollider->pos - pOtherCollider->pos;
			float fRadius = pOwnerCollider->size.x + pOtherCollider->size.x;

			// �O�����̒藝����G�ƃv���C���[�̋������r
			if (diffPos.x*diffPos.x +
				diffPos.y*diffPos.y +
				diffPos.z*diffPos.z <=
				(fRadius* fRadius))
			{
				// ���_�f�[�^���A�����b�N
				m_pVtxBuff->Unlock();
				*pHitID = nCntCollider;
				return true;
			}
		}
	}
	//CDebugProc::print("PlayerColli = [%.4f] [%.4f] [%.4f]\n", pOwnerCollider->pos.x, pOwnerCollider->pos.y, pOwnerCollider->pos.z);

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
	*pHitID = -1;
	return bCollision;

}

//-------------------------------------------------------------------------------------------------------------
// �u���b�N��̏Փ˔���
//-------------------------------------------------------------------------------------------------------------
bool C3DBoxCollider::CollisionBox(int n3DBoxColliderID, D3DXVECTOR3 &pos, D3DXVECTOR3 &move, D3DXVECTOR3 *pOut_intersect, D3DXVECTOR3 *pOut_nor, bool bReflection)
{
	// �ϐ��錾
	_3DBOXCOLLIDER		 *pOwnerCollider = &m_ColliderInfo[n3DBoxColliderID];	// ������̃|�C���^
	_3DBOXCOLLIDER		 *pOtherCollider = &m_ColliderInfo[0];					// ���̑��̃|�C���^
	bool				 bCollision = false;									// �Փ˃t���O
	VERTEX_3D *pVtx;													// ���_���|�C���^

	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// �R���C�_�[�������[�v
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX, pOtherCollider++)
	{
		if (nCntCollider == n3DBoxColliderID)
		{// �������͏��O
			continue;
		}
		if (pOtherCollider->bUse == false)
		{// �g�p���Ă��Ȃ����̂͏��O
			continue;
		}
		if (pOtherCollider->ColliderSubType == C3DBoxCollider::COLLIDER_SUB_OVERRAP)
		{// �Փ˂��Ă��邩���ʂ����ނ͏���
			continue;
		}
		if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_NORMAL)
		{
			// Y�����d�Ȃ��Ă���Ƃ�
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				// Z���͈̔�
				if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// ���̓����蔻��
					if (pOwnerCollider->pos.x + pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.x = pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE - 0.01f;
						pos.x = pOwnerCollider->pos.x;
						move.x = 0.0f;
						if (bReflection == true)
						{
							*pOut_intersect = pos;
							*pOut_nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
						}
					}
					// �E�̓����蔻��
					else if (pOwnerCollider->pos.x - pOwnerCollider->size.x* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.x = pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE + 0.01f;
						pos.x = pOwnerCollider->pos.x;
						move.x = 0.0f;
						if (bReflection == true)
						{
							*pOut_intersect = pos;
							*pOut_nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
						}
					}
				}
				// X���͈̔�
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					// ���̓����蔻��
					if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.z = pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE - 0.01f;
						pos.z = pOwnerCollider->pos.z;
						move.z = 0.0f;
						if (bReflection == true)
						{
							*pOut_intersect = pos;
							*pOut_nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
						}
					}
					// �E�̓����蔻��
					else if (pOwnerCollider->pos.z - pOwnerCollider->size.z* _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.z = pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE + 0.01f;
						pos.z = pOwnerCollider->pos.z;
						move.z = 0.0f;
						if (bReflection == true)
						{
							*pOut_intersect = pos;
							*pOut_nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
						}
					}
				}
			}
			if (pOwnerCollider->pos.z + pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.z - pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE&&
				pOwnerCollider->pos.z - pOwnerCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.z + pOtherCollider->size.z * _3DBOXCOLLIDER_HALF_SIZE)
			{
				if (pOwnerCollider->pos.x + pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.x - pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->pos.x - pOwnerCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.x + pOtherCollider->size.x * _3DBOXCOLLIDER_HALF_SIZE)
				{
					if (pOwnerCollider->pos.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.y = pOtherCollider->pos.y - (pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + 0.0001f);
						pos.y = pOwnerCollider->pos.y - pOwnerCollider->difference.y;

						move.y = 0.0f;
						if (bReflection == true)
						{
							*pOut_intersect = pos;
							*pOut_nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
						}
					}
					else if (pOwnerCollider->pos.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
						pOwnerCollider->posOld.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
					{
						pOwnerCollider->pos.y = pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + 0.0001f;
						pos.y = pOwnerCollider->pos.y - pOwnerCollider->difference.y;
						move.y = 0.0f;
						bCollision = true;
						if (bReflection == true)
						{
							*pOut_intersect = pos;
							*pOut_nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
						}
					}
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_CYLINDER)
		{
			// Y�����d�Ȃ��Ă���Ƃ�
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
				float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + (sqrtf(pOtherCollider->size.x*pOtherCollider->size.x + pOtherCollider->size.z*pOtherCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE);
				if (diffPos.x*diffPos.x +
					diffPos.y*diffPos.y <=
					(fRadius*fRadius))
				{
					// �p�x��ݒ�
					float radian = atan2f(-diffPos.x, -diffPos.y);
					pOwnerCollider->pos.x = pOtherCollider->pos.x + sinf(radian)*(fRadius);
					pOwnerCollider->pos.z = pOtherCollider->pos.z + cosf(radian)*(fRadius);
					pos.x = pOwnerCollider->pos.x;
					pos.z = pOwnerCollider->pos.z;
				}
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_SPHERE)
		{
			// �G�ƃv���C���[�̍��������߂�
			D3DXVECTOR3 diffPos = pOwnerCollider->pos - pOtherCollider->pos;
			float fRadius = pOwnerCollider->size.x + pOtherCollider->size.x;

			// �O�����̒藝����G�ƃv���C���[�̋������r
			if (diffPos.x*diffPos.x +
				diffPos.y*diffPos.y +
				diffPos.z*diffPos.z <=
				(fRadius* fRadius))
			{
				// �p�x��ݒ�
				float fRadian = atan2f(diffPos.x, diffPos.z);
				pOwnerCollider->pos.x = pOtherCollider->pos.x + sinf(fRadian)*(fRadius);
				pOwnerCollider->pos.z = pOtherCollider->pos.z + cosf(fRadian)*(fRadius);
				pos.x = pOwnerCollider->pos.x;
				pos.z = pOwnerCollider->pos.z;
			}
		}
		else if (pOtherCollider->ColliderType == C3DBoxCollider::COLLIDER_TYPE_CYLINDER_CANRIDE)
		{
			// Y�����l�����Ȃ��œ����Ă����ꍇ
			D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
			float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + (sqrtf(pOtherCollider->size.x*pOtherCollider->size.x + pOtherCollider->size.z*pOtherCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE);
			if (diffPos.x*diffPos.x +
				diffPos.y*diffPos.y <=
				(fRadius*fRadius))
			{
				if (pOwnerCollider->pos.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->posOld.y + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE <= pOtherCollider->pos.y - pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
				{
					pOwnerCollider->pos.y = pOwnerCollider->posOld.y;
					pos.y = pOwnerCollider->pos.y;
					move.y = 0.0f;
				}
				else if (pOwnerCollider->pos.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE&&
					pOwnerCollider->posOld.y - pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE >= pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE)
				{
					pOwnerCollider->pos.y = pOtherCollider->pos.y + pOtherCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + pOwnerCollider->size.y * _3DBOXCOLLIDER_HALF_SIZE + 0.0001f;
					pos.y = pOwnerCollider->pos.y - pOwnerCollider->difference.y;
					// �ړ��ʂ̏�����
					move.y = 0.0f;
					bCollision = true;
					if (bReflection == true)
					{
						*pOut_intersect = pos;
						*pOut_nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
					}
				}
			}
			// Y�����d�Ȃ��Ă���Ƃ�
			if (pOwnerCollider->pos.y + pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE > pOtherCollider->pos.y - pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE &&
				pOwnerCollider->pos.y - pOwnerCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE < pOtherCollider->pos.y + pOtherCollider->size.y*_3DBOXCOLLIDER_HALF_SIZE)
			{
				D3DXVECTOR2 diffPos = D3DXVECTOR2(pOtherCollider->pos.x - pOwnerCollider->pos.x, pOtherCollider->pos.z - pOwnerCollider->pos.z);
				float fRadius = (sqrtf(pOwnerCollider->size.x*pOwnerCollider->size.x + pOwnerCollider->size.z*pOwnerCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE) + (sqrtf(pOtherCollider->size.x*pOtherCollider->size.x + pOtherCollider->size.z*pOtherCollider->size.z)*_3DBOXCOLLIDER_HALF_SIZE);
				if (diffPos.x*diffPos.x +
					diffPos.y*diffPos.y <=
					(fRadius*fRadius))
				{
					// �p�x��ݒ�
					float radian = atan2f(-diffPos.x, -diffPos.y);
					float fSinValue = sinf(radian)*fRadius;
					float fCosValue = cosf(radian)*fRadius;
					pOwnerCollider->pos.x = pOtherCollider->pos.x + fSinValue;
					pOwnerCollider->pos.z = pOtherCollider->pos.z + fCosValue;
					pos.x = pOwnerCollider->pos.x;
					pos.z = pOwnerCollider->pos.z;

					if (bReflection == true)
					{
						D3DXVECTOR3 norm;	// �ʂ����Ẵx�N�g��
						norm = D3DXVECTOR3(fSinValue, 0.0f, fCosValue);
						CMylibrary::CreateUnitVector(&norm, &norm);
						*pOut_intersect = pos;
						*pOut_nor = norm;
					}

				}
			}
		}
	}
	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
	return bCollision;
}

//-------------------------------------------------------------------------------------------------------------
// �ݒ�
//-------------------------------------------------------------------------------------------------------------
int C3DBoxCollider::Set(D3DXVECTOR3 &size, D3DXVECTOR3 &pos, D3DXVECTOR3 &rot, D3DXVECTOR3 &difference, COLLIDER_TYPE colliType, COLLIDER_SUBTYPE ColliderSubType,CScene * pScene)
{
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++)
	{
		if (m_ColliderInfo[nCntCollider].bUse == false)
		{
			// ���_���̐ݒ�
			VERTEX_3D *pVtx;
			// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
			// ���_���̍X�V
			pVtx += _3DBOXCOLLIDER_USE_VERTEX * nCntCollider;
			m_ColliderInfo[nCntCollider].difference		= difference;		// ����
			m_ColliderInfo[nCntCollider].size			= size;				// �傫��
			m_ColliderInfo[nCntCollider].pos			= pos + difference;	// �ʒu
			m_ColliderInfo[nCntCollider].rot			= rot;				// ��]
			m_ColliderInfo[nCntCollider].bUse			= true;				// �g�p�t���O
			m_ColliderInfo[nCntCollider].ColliderType	= colliType;		// �Փˎ��
			m_ColliderInfo[nCntCollider].pScene			= pScene;			// scene�̐ݒ�
			m_ColliderInfo[nCntCollider].ColliderSubType = ColliderSubType;	// �T�u�^�C�v�̐ݒ�
			// ���_�ʒu�̍X�V
			SetVertexPosition(pVtx, nCntCollider);
			SetVertexPosResult(pVtx, nCntCollider);
			SetSurfaceNom(pVtx, nCntCollider);
			// ���_�f�[�^���A�����b�N
			m_pVtxBuff->Unlock();

			return nCntCollider;
		}
	}
	return -1;
}

//-------------------------------------------------------------------------------------------------------------
// �V�[���|�C���^�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetScene(int n3DBoxColliderID, CScene * pScene)
{
	m_ColliderInfo[n3DBoxColliderID].pScene = pScene;
}

//-------------------------------------------------------------------------------------------------------------
// �V�[���|�C���^�̎擾
//-------------------------------------------------------------------------------------------------------------
CScene * C3DBoxCollider::GetScene(int n3DBoxColliderID)
{
	return m_ColliderInfo[n3DBoxColliderID].pScene;
}

//-------------------------------------------------------------------------------------------------------------
// ����ύX����
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::ChangeInfo(int nIndex, D3DXVECTOR3 & size, D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	if (m_ColliderInfo[nIndex].bUse == true)
	{
		// ���_���̐ݒ�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_���̍X�V
		pVtx += _3DBOXCOLLIDER_USE_VERTEX * nIndex;

		m_ColliderInfo[nIndex].size = size;		// �傫��
		m_ColliderInfo[nIndex].pos = pos;		// �ʒu
		m_ColliderInfo[nIndex].rot = rot;		// ��]

		// ���_�ʒu�̍X�V
		SetVertexPosition(pVtx, nIndex);
		SetVertexPosResult(pVtx, nIndex);
		SetSurfaceNom(pVtx, nIndex);

		// ���_�f�[�^���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//-------------------------------------------------------------------------------------------------------------
// ����ύX����
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::ChangeInfo(int nIndex, D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	if (m_ColliderInfo[nIndex].bUse == true)
	{
		// ���_���̐ݒ�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_���̍X�V
		pVtx += _3DBOXCOLLIDER_USE_VERTEX * nIndex;

		m_ColliderInfo[nIndex].pos = pos;		// �ʒu
		m_ColliderInfo[nIndex].rot = rot;		// ��]

		// ���_�ʒu�̍X�V
		SetVertexPosition(pVtx, nIndex);
		SetVertexPosResult(pVtx, nIndex);
		SetSurfaceNom(pVtx, nIndex);

		// ���_�f�[�^���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �ʒu�̕ύX
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::ChangePosition(int nID,D3DXVECTOR3 & pos, D3DXVECTOR3 & rot)
{
	if (m_ColliderInfo[nID].bUse == true)
	{
		// ���_���̐ݒ�
		VERTEX_3D *pVtx;
		// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_���̍X�V
		pVtx += _3DBOXCOLLIDER_USE_VERTEX * nID;
		m_ColliderInfo[nID].posOld = m_ColliderInfo[nID].pos;
		m_ColliderInfo[nID].pos = pos + m_ColliderInfo[nID].difference;		// �ʒu
		m_ColliderInfo[nID].rot = rot;		// ��]

		// ���_�ʒu�̍X�V
		SetVertexPosition(pVtx, nID);
		SetVertexPosResult(pVtx, nID);
		SetSurfaceNom(pVtx, nID);
		// ���_�f�[�^���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//-------------------------------------------------------------------------------------------------------------
// �ʂ̖@����ݒ肷��
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetSurfaceNom(VERTEX_3D *pVtx, int nIndex)
{
	{// ��̖ʂ̖@��
		// ���ꂼ��̃x�N�g�����쐬
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[1] - m_ColliderInfo[nIndex].Vertex[0];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[3] - m_ColliderInfo[nIndex].Vertex[0];
		// �ϐ��錾
		D3DXVECTOR3 nomA_B;						// �|���S��0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_TOP] = nomA_B;
	}

	{// ���̖ʂ̖@��
		// ���ꂼ��̃x�N�g�����쐬
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[7] - m_ColliderInfo[nIndex].Vertex[4];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[5] - m_ColliderInfo[nIndex].Vertex[4];
		// �ϐ��錾
		D3DXVECTOR3 nomA_B;						// �|���S��0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_BOTTOM] = nomA_B;
	}

	{// �E�̖ʂ̖@��
	 // ���ꂼ��̃x�N�g�����쐬
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[5] - m_ColliderInfo[nIndex].Vertex[1];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[2] - m_ColliderInfo[nIndex].Vertex[1];
		// �ϐ��錾
		D3DXVECTOR3 nomA_B;						// �|���S��0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_RIGHT] = nomA_B;
	}

	{// ���̖ʂ̖@��
	 // ���ꂼ��̃x�N�g�����쐬
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[3] - m_ColliderInfo[nIndex].Vertex[0];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[4] - m_ColliderInfo[nIndex].Vertex[0];
		// �ϐ��錾
		D3DXVECTOR3 nomA_B;						// �|���S��0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_LEFT] = nomA_B;
	}
	{//  �O�̖ʂ̖@��
	 // ���ꂼ��̃x�N�g�����쐬
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[4] - m_ColliderInfo[nIndex].Vertex[0];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[1] - m_ColliderInfo[nIndex].Vertex[0];
		// �ϐ��錾
		D3DXVECTOR3 nomA_B;						// �|���S��0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_FRONT] = nomA_B;
	}
	{//  ���̖ʂ̖@��
	 // ���ꂼ��̃x�N�g�����쐬
		D3DXVECTOR3 vecA = m_ColliderInfo[nIndex].Vertex[2] -m_ColliderInfo[nIndex].Vertex[3];
		D3DXVECTOR3 vecB = m_ColliderInfo[nIndex].Vertex[7] -m_ColliderInfo[nIndex].Vertex[3];
		// �ϐ��錾
		D3DXVECTOR3 nomA_B;						// �|���S��0
		D3DXVec3Cross(&nomA_B, &vecA, &vecB);
		D3DXVec3Normalize(&nomA_B, &nomA_B);

		m_ColliderInfo[nIndex].surfaceNom[C3DBoxCollider::SURFACE_TYPE_REAR] = nomA_B;
	}
}

//-------------------------------------------------------------------------------------------------------------
// �v�Z��̒��_�ʒu�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetVertexPosResult(int nIndex)
{
	D3DXMATRIX mtxRot, mtxTrans, mtwWorld;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixIdentity(&mtxTrans);
	D3DXMatrixIdentity(&mtwWorld);

	D3DXMatrixIdentity(&m_ColliderInfo[nIndex].mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		m_ColliderInfo[nIndex].rot.y,
		m_ColliderInfo[nIndex].rot.x,
		m_ColliderInfo[nIndex].rot.z);

	D3DXMatrixMultiply(
		&m_ColliderInfo[nIndex].mtxWorld,
		&m_ColliderInfo[nIndex].mtxWorld,
		&mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(
		&mtxTrans,
		m_ColliderInfo[nIndex].pos.x,
		m_ColliderInfo[nIndex].pos.y,
		m_ColliderInfo[nIndex].pos.z);

	D3DXMatrixMultiply(
		&m_ColliderInfo[nIndex].mtxWorld,
		&m_ColliderInfo[nIndex].mtxWorld,
		&mtxTrans);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;
	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += _3DBOXCOLLIDER_USE_VERTEX*nIndex;

	for (int nCntVert = 0; nCntVert < _3DBOXCOLLIDER_USE_VERTEX; nCntVert++)
	{
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(
			&mtxRot,
			0.0f,
			0.0f,
			0.0f);
		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(
			&mtxTrans,
			pVtx[nCntVert].pos.x,
			pVtx[nCntVert].pos.y,
			pVtx[nCntVert].pos.z);

		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&mtxTrans);

		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&m_ColliderInfo[nIndex].mtxWorld);

		m_ColliderInfo[nIndex].Vertex[nCntVert].x = mtwWorld._41;
		m_ColliderInfo[nIndex].Vertex[nCntVert].y = mtwWorld._42;
		m_ColliderInfo[nIndex].Vertex[nCntVert].z = mtwWorld._43;
	}
	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//-------------------------------------------------------------------------------------------------------------
// �v�Z��̒��_�ʒu�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetVertexPosResult(VERTEX_3D * pVtx, int nIndex)
{
	D3DXMATRIX mtxRot, mtxTrans, mtwWorld;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtwWorld);

	D3DXMatrixIdentity(&m_ColliderInfo[nIndex].mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		m_ColliderInfo[nIndex].rot.y,
		m_ColliderInfo[nIndex].rot.x,
		m_ColliderInfo[nIndex].rot.z);

	D3DXMatrixMultiply(
		&m_ColliderInfo[nIndex].mtxWorld,
		&m_ColliderInfo[nIndex].mtxWorld,
		&mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(
		&mtxTrans,
		m_ColliderInfo[nIndex].pos.x,
		m_ColliderInfo[nIndex].pos.y,
		m_ColliderInfo[nIndex].pos.z);

	D3DXMatrixMultiply(
		&m_ColliderInfo[nIndex].mtxWorld,
		&m_ColliderInfo[nIndex].mtxWorld,
		&mtxTrans);

	for (int nCntVert = 0; nCntVert < _3DBOXCOLLIDER_USE_VERTEX; nCntVert++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtwWorld);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(
			&mtxTrans,
			pVtx[nCntVert].pos.x,
			pVtx[nCntVert].pos.y,
			pVtx[nCntVert].pos.z);

		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&mtxTrans);

		D3DXMatrixMultiply(
			&mtwWorld,
			&mtwWorld,
			&m_ColliderInfo[nIndex].mtxWorld);

		m_ColliderInfo[nIndex].Vertex[nCntVert].x = mtwWorld._41;
		m_ColliderInfo[nIndex].Vertex[nCntVert].y = mtwWorld._42;
		m_ColliderInfo[nIndex].Vertex[nCntVert].z = mtwWorld._43;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ���̎擾
//-------------------------------------------------------------------------------------------------------------
C3DBoxCollider::_3DBOXCOLLIDER *C3DBoxCollider::GetInfo(int nIndex)
{
	return &m_ColliderInfo[nIndex];
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::unset(int nID)
{
	CMylibrary::Setdifferentvalue(m_ColliderInfo[nID].bUse, false);
	m_ColliderInfo[nID].bUse = false;
	m_ColliderInfo[nID].pScene = NULL;
}

//-------------------------------------------------------------------------------------------------------------
// ���̏�����
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::InitColliderInfo(void)
{
	// �R���C�_�[�̍ő�������[�v
	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++)
	{
		// �g�p�t���O�̏�����
		m_ColliderInfo[nCntCollider].bUse = false;
		// �ʒu�̏�����
		m_ColliderInfo[nCntCollider].pos = MYLIB_3DVECTOR_ZERO;
		// �O��̈ʒu�̏�����
		m_ColliderInfo[nCntCollider].posOld = MYLIB_3DVECTOR_ZERO;
		// ��]�ʂ̏�����
		m_ColliderInfo[nCntCollider].rot = MYLIB_3DVECTOR_ZERO;
		// �傫���̏�����
		m_ColliderInfo[nCntCollider].size = MYLIB_3DVECTOR_ZERO;
		// �ʂ̖@���̏�����
		for (int nCntVertex = 0; nCntVertex < _3DBOXCOLLIDER_NUM_SURFACE; nCntVertex++)
		{
			m_ColliderInfo[nCntCollider].surfaceNom[nCntVertex] = MYLIB_3DVECTOR_ZERO;
		}
		// ���_�ʒu�̏�����
		for (int nCntVertex = 0; nCntVertex < _3DBOXCOLLIDER_USE_VERTEX; nCntVertex++)
		{
			m_ColliderInfo[nCntCollider].Vertex[0] = MYLIB_3DVECTOR_ZERO;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// �R���C�_�[���̐ݒ�
//-------------------------------------------------------------------------------------------------------------
int C3DBoxCollider::SetColliderInfo(D3DXVECTOR3 *pPos, CScene *pScene, COLLIDER_SUBTYPE ColliderSubType, int nID)
{
	// �R���C�_�[ID�ɕϊ�����
	ConvertColliderID(&nID);
	// �ǂ݂��ތ������[�v
	for (int nCntRead = 0; nCntRead < m_ReadInfoFileBuff.nNumReadInfo; nCntRead++)
	{
		// �ݒ肷��ID�ƔC�ӂ�ID�������Ƃ��ݒ肷��
		if (m_ReadInfoFileBuff.pSetThisID[nCntRead] == nID)
		{
			return C3DBoxCollider::Set(
				Mybfunc_iifEx(m_ReadInfoFileBuff.pCell[nCntRead].pSize != NULL, *m_ReadInfoFileBuff.pCell[nCntRead].pSize, MYLIB_3DVECTOR_ZERO),
				*pPos,
				MYLIB_3DVECTOR_ZERO,
				Mybfunc_iifEx(m_ReadInfoFileBuff.pCell[nCntRead].pDifference != NULL, *m_ReadInfoFileBuff.pCell[nCntRead].pDifference, MYLIB_3DVECTOR_ZERO),
				(COLLIDER_TYPE)m_ReadInfoFileBuff.pCell[nCntRead].nColliderType, ColliderSubType,
				pScene);
		}
	}
	return -1;
}


//-------------------------------------------------------------------------------------------------------------
// �C�ӂ�ID���R���C�_�[ID�ɕϊ�
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::ConvertColliderID(int * pID)
{
	switch (*pID)
	{
		/* ID�Ɠ����Ȃ牽�����Ȃ� */
	case ID_CHARACTER: return; break;
	case ID_STONE:   return; break;
		/* ID�ƈႢ���Ƃ��X���[ */
	default:
		break;
	}
}

//-------------------------------------------------------------------------------------------------------------
// ���_���̍쐬
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DBoxCollider::MakeVertex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// ���_�o�b�t�@�𐶐�

	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * _3DBOXCOLLIDER_MAX * _3DBOXCOLLIDER_USE_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,									// ���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔̓��b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCollider = 0; nCntCollider < _3DBOXCOLLIDER_MAX; nCntCollider++, pVtx += _3DBOXCOLLIDER_USE_VERTEX)
	{
		for (int nCntVert = 0; nCntVert < _3DBOXCOLLIDER_USE_VERTEX; nCntVert++)
		{
			// ���_�̐ݒ�
			pVtx[nCntVert].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// �@���x�N�g��
			pVtx[nCntVert].nor = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			// ���_�J���[
			pVtx[nCntVert].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			// texture���W�̐ݒ�
			pVtx[nCntVert].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();


	return E_NOTIMPL;
}
//-------------------------------------------------------------------------------------------------------------
// �C���f�b�N�X���̍쐬
//-------------------------------------------------------------------------------------------------------------
HRESULT C3DBoxCollider::MakeIndex(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// ���_�o�b�t�@�𐶐�

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(WORD) *_3DBOXCOLLIDER_NUM_INDEX*_3DBOXCOLLIDER_MAX,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �C���f�b�N�X�f�[�^�ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�o�b�t�@�̃|�C���^�擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntSurface = 0; nCntSurface < 2; nCntSurface++)
	{
		for (int nCntTopBottom = 0; nCntTopBottom < 4; nCntTopBottom++)
		{
			pIdx[0] = nCntTopBottom + (nCntSurface * 4);
			if (nCntTopBottom != 3)
			{
				pIdx[1] = 1 + nCntTopBottom + (nCntSurface * 4);
			}
			else
			{
				pIdx[1] = 0 + (nCntSurface * 4);
			}
			pIdx += 2;
		}
	}
	for (int nCntMidle = 0; nCntMidle < 4; nCntMidle++)
	{
		pIdx[0] = 0 + nCntMidle;
		pIdx[1] = 4 + nCntMidle;
		pIdx += 2;
	}
	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
	return E_NOTIMPL;
}

//-------------------------------------------------------------------------------------------------------------
// ���_�ʒu�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
void C3DBoxCollider::SetVertexPosition(VERTEX_3D *pVtx, int nIndex)
{
	pVtx[0].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[1].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[2].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*-_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[3].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*-_3DBOXCOLLIDER_HALF_SIZE);

	pVtx[4].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[5].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[6].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*-_3DBOXCOLLIDER_HALF_SIZE);
	pVtx[7].pos = D3DXVECTOR3(m_ColliderInfo[nIndex].size.x*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.y*-_3DBOXCOLLIDER_HALF_SIZE, m_ColliderInfo[nIndex].size.z*-_3DBOXCOLLIDER_HALF_SIZE);
}
