//==================================================================================================================
//
// ���b�V���t�B�[���h����[meshField.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "renderer.h"
#include "meshField.h"
#include "inputKeyboard.h"
#include "debugProc.h"
#include "player.h"
#include "tutorial.h"
#include "game.h"
#include "title.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define WhileX			30.0f							// �C�`�}�X�̒�����
#define WhileY			15.0f							// �C�`�}�X�̒�������
#define WhileZ			30.0f							// �C�`�}�X�̒����c
#define MASS_WIDTH		(2)								// ���̃}�X
#define MASS_DEPTH		(2)								// �c�̃}�X

#define FIELD_ALPHA		1.0f							// ���b�V���t�B�[���h�A���t�@
#define FIELD_TEXTUE_TITLE	"data/TEXTURE/worldMap.jpg"	// �ǂݍ��ރe�N�X�`���̃\�[�X��
#define FIELD_TEXTUE_GAME	"data/TEXTURE/water4.png"	// �ǂݍ��ރe�N�X�`���̃\�[�X��
#define MESH_ANIM_MAX	(10000)							// �A�j���[�V�������Ԃ̍ő�

//==================================================================================================================
// �ÓI�����o�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CMeshField::m_pTexture = NULL;			// �e�N�X�`���ϐ�
CMeshField *CMeshField::m_pMeshField = NULL;				// ���b�V���t�B�[���h�̏��
CPlayer *CMeshField::m_pPlayer = NULL;						// �v���C���[�̏��

//==================================================================================================================
// �R���X�g���N�^
//==================================================================================================================
CMeshField::CMeshField(PRIORITY type = CScene::PRIORITY_FIELD) : CScene(type)
{

}

//==================================================================================================================
// �f�X�g���N�^
//==================================================================================================================
CMeshField::~CMeshField()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CMeshField::Init(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// �f�o�C�X�̎擾

	// �ʒu�E��]�̏����ݒ�
	m_rot		= ZeroVector3;		// ��]
	m_vectorA	= ZeroVector3;		// ��ڂ̊O�ϗp�ϐ�
	m_vectorB	= ZeroVector3;		// ��ڂ̊O�ϗp�ϐ�
	m_vectorC	= ZeroVector3;		// �O�ڂ̊O�ϗp�ϐ�
	nNumber		= 0;				// �z��̔ԍ�
	StartBox	= m_nWidth + 1;		// �n�܂锠
	EndBox		= 0;				// ������锠
	fDivide		= 0;				// sin�̒��g������ϐ�
	m_nCntAnim	= 0;

	m_aVecA = new D3DXVECTOR3[m_nWidth * m_nDepth * 2];		// �@���x�N�g����ʂ̐����ꎞ�I�Ɋi�[
	m_aVecB = new D3DXVECTOR3[m_nWidth * m_nDepth * 2];		// �@���x�N�g����ʂ̐����ꎞ�I�Ɋi�[

	m_nNumVertex = ((m_nWidth + 1) * m_nDepth * 2) + (m_nDepth - 1) * 2;	// �����_��
	m_nNumIndex = (m_nDepth + 1) * (m_nWidth + 1);							// ���C���f�b�N�X��
	m_nNumPolygon = m_nNumVertex - 2;										// �O�p�`�̐�

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�𐶐�
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// �c���J�E���g
	for (int nDepth = 0; nDepth < m_nDepth + 1; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < m_nWidth + 1; nWide++)
		{
			///////////////////////////////////////////////�i�~�i�~�`�`///////////////////////////////////////////////
			// ���_���W�̐ݒ�
			//m_pVtx[0].pos = D3DXVECTOR3((-WhileX * Width) / 2 + WhileX * nWide, cosf(D3DX_PI / 6 * nDepth + fDivide) * WhileY, (WhileZ / 2 * Depth) - WhileZ * nDepth);

			// ���_���W�̐ݒ�
			m_pVtx[0].pos.x = (-m_size.x * m_nWidth) / 2 + m_size.x * nWide;
			m_pVtx[0].pos.y = m_pos.y;
			m_pVtx[0].pos.z = (m_size.z / 2 * m_nDepth) - m_size.z * nDepth;

			// ���_�J���[
			m_pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`���`�ʂ̈ʒu
			m_pVtx[0].tex = D3DXVECTOR2(1.0f * nWide, 1.0f * nDepth);

			m_pVtx++;
		}
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	WORD *pIdx;				// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	// �C���f�b�N�X�o�b�t�@�����b�N���C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �c���J�E���g
	for (int nCntY = 0; nCntY < m_nDepth; nCntY++)
	{
		// ��s�ڂ���Ȃ��Ƃ�
		if (nCntY != 0)
		{
			pIdx[0] = (m_nWidth + 1) + nCntY * (m_nWidth + 1);
			pIdx++;
		}
		// �����J�E���g
		for (int nCntX = 0; nCntX < m_nWidth + 1; nCntX++)
		{
			pIdx[0] = (m_nWidth + 1) + nCntX + nCntY * (m_nWidth + 1);
			pIdx[1] = nCntX + nCntY * (m_nWidth + 1);
			pIdx += 2;
		}

		// �k�ރ|���S��
		if (nCntY + 1 < m_nDepth)
		{
			pIdx[0] = m_nWidth + nCntY * (m_nWidth + 1);
			pIdx++;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();

	// �@���̐ݒ�
	SetNor();
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CMeshField::Uninit(void)
{
	// ���_�o�b�t�@��NULL����Ȃ��Ƃ�
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();				// ���_�o�b�t�@�̊J��
		m_pVtxBuff = NULL;					// NULL�ɂ���
	}

	// �C���f�b�N�X�o�b�t�@��NULL����Ȃ��Ƃ�
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();				// �C���f�b�N�X�o�b�t�@�̊J��
		m_pIdxBuff = NULL;					// NULL�ɂ���
	}

	// nullcheck
	if (m_aVecA)
	{
		// �j��
		delete[] m_aVecA;
		m_aVecA = nullptr;
	}

	// nullcheck
	if (m_aVecB)
	{
		// �j��
		delete[] m_aVecB;
		m_aVecB = nullptr;
	}
}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CMeshField::Update(void)
{
	m_nCntAnim++;
	if (m_nCntAnim > MESH_ANIM_MAX)
	{
		m_nCntAnim = 0;
	}

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	// �^�C�g���̂Ƃ� ���� �`���[�g���A���̂Ƃ�
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE ||
		CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{
		// ������
		fDivide -= 0.05f;

		// �c���J�E���g
		for (int nDepth = 0; nDepth < m_nDepth + 1; nDepth++)
		{
			// �����J�E���g
			for (int nWide = 0; nWide < m_nWidth + 1; nWide++)
			{
				// ���_���W�̐ݒ�
				m_pVtx[0].pos = D3DXVECTOR3((-WhileX * m_nWidth) / 2 + WhileX * nWide, 
					cosf(D3DX_PI / 3 * nDepth + fDivide) * WhileY, 
					(WhileZ / 2 * m_nDepth) - WhileZ * nDepth);

				// �e�N�X�`���`�ʂ̈ʒu
				m_pVtx[0].tex = D3DXVECTOR2((1.0f / m_nWidth) * nWide, (1.0f / m_nDepth) * nDepth);

				m_pVtx++;
			}
		}

		// �@���̐ݒ�
		SetNor();
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_GAME)
	{// �Q�[���̂Ƃ�
		// �c���J�E���g
		for (int nDepth = 0; nDepth < m_nDepth + 1; nDepth++)
		{
			// �����J�E���g
			for (int nWide = 0; nWide < m_nWidth + 1; nWide++)
			{
				// �e�N�X�`���`�ʂ̈ʒu
				m_pVtx[0].tex = D3DXVECTOR2(1.0f * nWide + (0.0001f * m_nCntAnim), 1.0f * nDepth + (0.0001f * m_nCntAnim));

				m_pVtx++;
			}
		}
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CMeshField::Draw(void)
{
	// �����_���[���擾
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[�̏��擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;							// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���[���h�}�g���b�N�X�̌v�Z
	CKananLibrary::CalcMatrix(&m_mtxWorld, m_pos, m_rot);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
}

//==================================================================================================================
// �ʒu�ݒ�
//==================================================================================================================
void CMeshField::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//==================================================================================================================
// �|���S������
//==================================================================================================================
CMeshField *CMeshField::Create(const INTEGER2 & block, const D3DXVECTOR3 & size, const D3DXVECTOR3 & pos)
{
	// �������𓮓I�Ɋm��
	m_pMeshField = new CMeshField(CScene::PRIORITY_FIELD);

	// ���b�V����������Ƃ�
	if (m_pMeshField != NULL)
	{
		// ���b�V���̃}�X����ݒ�
		m_pMeshField->m_nWidth = block.nX;
		m_pMeshField->m_nDepth = block.nY;

		// ���b�V���̃}�X�̒����ݒ�
		m_pMeshField->m_size = size;

		// ���b�V���̈ʒu�ݒ�
		m_pMeshField->m_pos = pos;

		// �V�[��������
		m_pMeshField->Init();
	}

	// �l��Ԃ�
	return m_pMeshField;
}

//==================================================================================================================
// �e�N�X�`����񃍁[�h
//==================================================================================================================
HRESULT CMeshField::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();						// �����_���[�̏��擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();					// �f�o�C�X���擾����

	// �^�C�g���̂Ƃ�
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE ||
		CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			FIELD_TEXTUE_TITLE,						// �t�@�C���̖��O
			&m_pTexture);							// �ǂݍ��ރ������[
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_GAME)
	{
		// �e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
			FIELD_TEXTUE_GAME,						// �t�@�C���̖��O
			&m_pTexture);							// �ǂݍ��ރ������[
	}

	// �l��Ԃ�
	return S_OK;
}

//==================================================================================================================
// �e�N�X�`�����j��
//==================================================================================================================
void CMeshField::Unload(void)
{
	// �e�N�X�`����񂪂���Ƃ�
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();			// �e�N�X�`�����̊J��
		m_pTexture = NULL;				// NULL�ɂ���
	}
}

//==================================================================================================================
// ���b�V���t�B�[���h���擾
//==================================================================================================================
CMeshField * CMeshField::GetMeshField(void)
{
	return m_pMeshField;
}

//==================================================================================================================
// �ʒu�擾
//==================================================================================================================
D3DXVECTOR3 CMeshField::GetPos(void)
{
	return m_pos;
}

//==================================================================================================================
// �ړ��ʎ擾
//==================================================================================================================
D3DXVECTOR3 CMeshField::GetMove(void)
{
	return m_move;
}

//==================================================================================================================
// �����̎擾
//==================================================================================================================
float CMeshField::GetHeight(D3DXVECTOR3 &pos)
{
	// �ď�����
	nNumber = 0;				// �z��̔ԍ�
	StartBox = m_nWidth + 1;	// �n�܂锠
	EndBox = 0;					// ������锠
	float VecY;					// ���������p
	int nCntVec = 0;			// �@���̃J�E���^

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	//�c���J�E���g
	for (int nDepth = 0; nDepth < m_nDepth + 1; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < m_nWidth + 1; nWide++, nCntVec++)
		{
				// �[�͌v�Z�����Ȃ�
				if (nWide != m_nWidth && nDepth != m_nDepth)
				{
					// �v�Z�p�̍��W
					D3DXVECTOR3 VtxPos0 = m_pVtx[0].pos;
					D3DXVECTOR3 VtxPos1 = m_pVtx[1].pos;
					D3DXVECTOR3 VtxPos3 = m_pVtx[1 + m_nWidth].pos;
					D3DXVECTOR3 VtxPos4 = m_pVtx[2 + m_nWidth].pos;
					
					// �S�Ă̊O�ς������ƁA���̃|���S���̏�ɂ���
					if (GetNor(pos, VtxPos4, VtxPos3, VtxPos0) && GetPos().y >= pos.y)
					{
						VecY =	VtxPos3.y +
								(m_aVecB[nCntVec].x  *	(pos.x - VtxPos3.x)
								- m_aVecB[nCntVec].z *	(pos.z - VtxPos3.z))
								/ m_aVecB[nCntVec].y;

						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();

						// �������C��
						return GetPos().y + VecY;
					}

					// 2���ڂ̃|���S��
					if (GetNor(pos, VtxPos0, VtxPos1, VtxPos4) && GetPos().y >= pos.y)
					{
						VecY =	VtxPos1.y +
								(m_aVecA[nCntVec].x	 *	(pos.x - VtxPos1.x)
								- m_aVecA[nCntVec].z *	(pos.z - VtxPos1.z))
								/ m_aVecA[nCntVec].y;

						// ���_�f�[�^���A�����b�N
						m_pVtxBuff->Unlock();

						// �������C��
						return GetPos().y + VecY;
					}

					// �z��̔ԍ�1�v���X����
					StartBox++;
					EndBox++;
					nNumber++;
			}
		}
		// ���I�������z��̔ԍ���1�v���X����
		StartBox++;
		EndBox++;
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();

	return m_pos.y;
}

//==================================================================================================================
// �@���̎擾
//==================================================================================================================
bool CMeshField::GetNor(const D3DXVECTOR3 & pos, const D3DXVECTOR3 & VtxPos0, const D3DXVECTOR3 & VtxPos1, const D3DXVECTOR3 & VtxPos2)
{
	D3DXVECTOR3 nor0 = ZeroVector3;
	D3DXVECTOR3 nor1 = ZeroVector3;
	D3DXVECTOR3 nor2 = ZeroVector3;

	// 1���_��
	D3DXVECTOR3 Vec0_P;
	D3DXVECTOR3 Vec0_1;
	// 2
	D3DXVECTOR3 Vec1_P;
	D3DXVECTOR3 Vec1_2;
	// 3
	D3DXVECTOR3 Vec2_P;
	D3DXVECTOR3 Vec2_0;

	// 1���_��
	Vec0_P = pos - VtxPos0;
	Vec0_1 = VtxPos1 - VtxPos0;

	// 2
	Vec1_P = pos - VtxPos1;
	Vec1_2 = VtxPos2 - VtxPos1;

	// 3
	Vec2_P = pos - VtxPos2;
	Vec2_0 = VtxPos0 - VtxPos2;

	// 1�ڂ̖@��
	D3DXVec3Cross(&nor0, &Vec0_1, &Vec0_P);

	// 2
	D3DXVec3Cross(&nor1, &Vec1_2, &Vec1_P);

	// 3
	D3DXVec3Cross(&nor2, &Vec2_0, &Vec2_P);

	if (nor0.y >= 0 && nor1.y >= 0 && nor2.y >= 0)
		return true;
	else
		return false;
}

//==================================================================================================================
// �ړ��ʐݒ�
//==================================================================================================================
void CMeshField::SetMove(D3DXVECTOR3 move)
{
	m_move = move;
}

//==================================================================================================================
// �@���̐ݒ�
//==================================================================================================================
void CMeshField::SetNor(void)
{
	nNumber = 0;									// �z��̔ԍ�
	StartBox = m_nWidth + 1;						// �n�܂锠
	EndBox = 0;										// ������锠

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^�擾
	m_pVtxBuff->Lock(0, 0, (void**)&m_pVtx, 0);

	//////////////////////////////�@���x�N�g��//////////////////////////////
	// �c���J�E���g
	for (int nDepth = 0; nDepth < m_nDepth + 1; nDepth++)
	{
		// �����J�E���g
		for (int nWide = 0; nWide < m_nWidth + 1; nWide++)
		{
			// �C���f�b�N�X�ԍ�
			int nIdx = nDepth * (m_nWidth + 1) + nWide;

			// �[�͌v�Z�����Ȃ�
			if (nWide != m_nWidth && nDepth != m_nDepth)
			{
				m_vectorA = m_pVtx[StartBox].pos - m_pVtx[EndBox].pos;										// ��ڂ̃x�N�g��
				m_vectorB = m_pVtx[StartBox + 1].pos - m_pVtx[EndBox].pos;									// ��ڂ̃x�N�g��
				m_vectorC = m_pVtx[EndBox + 1].pos - m_pVtx[EndBox].pos;									// �O�ڂ̃x�N�g��
				D3DXVec3Cross(&m_aVecA[nNumber], &m_vectorA, &m_vectorB);									// ��̃x�N�g���̒����x�N�g��
				D3DXVec3Normalize(&m_aVecA[nNumber], &m_aVecA[nNumber]);									// ���K������
				D3DXVec3Cross(&m_aVecB[nNumber], &m_vectorB, &m_vectorC);									// ��̃x�N�g���̒����x�N�g��
				D3DXVec3Normalize(&m_aVecB[nNumber], &m_aVecB[nNumber]);									// ���K������

				// �z��̔ԍ�1�v���X����
				StartBox++;
				EndBox++;
				nNumber++;
			}

			if (nIdx == m_nWidth)											// �E�ゾ�����Ƃ�
			{
				// �@���x�N�g��
				m_pVtx[nIdx].nor = m_aVecB[m_nWidth - 1];
			}
			else if (nIdx == m_nDepth * (m_nWidth + 1))						// �����������Ƃ�
			{
				// �@���x�N�g��
				m_pVtx[nIdx].nor = m_aVecA[(m_nDepth - 1) * m_nWidth];
			}
			else if (nIdx == 0)											// ���ゾ�����Ƃ�
			{
				// �@���x�N�g��
				m_pVtx[nIdx].nor = (m_aVecA[nIdx] + m_aVecB[nIdx]) / 2;
			}
			else if (nIdx == (m_nDepth + 1) * (m_nWidth + 1) - 1)				// �E���������Ƃ�
			{
				// �@���x�N�g��
				m_pVtx[nIdx].nor = (m_aVecA[m_nDepth * m_nWidth - 1] + m_aVecB[m_nDepth * m_nWidth - 1]) / 2;
			}
			else if (nWide != 0 && nWide != m_nWidth)						// ���̗񂪒[����Ȃ��Ƃ�
			{
				if (nDepth == 0)										// �c�����ڂ������Ƃ�
				{
					// �@���x�N�g��
					m_pVtx[nIdx].nor = (m_aVecB[nWide - 1] + m_aVecA[nWide] + m_aVecB[nWide]) / 3;
				}
				else if (nDepth == m_nDepth)								// �c���Ō���������Ƃ�
				{
					// �@���x�N�g��
					m_pVtx[nIdx].nor = (m_aVecB[(m_nDepth - 1) * m_nWidth - 1 + nWide] +
										m_aVecA[(m_nDepth - 1) * m_nWidth + nWide] +
										m_aVecB[(m_nDepth - 1) * m_nWidth + nWide]) / 3;
				}
				else													// ����ȊO
				{
					// �@���x�N�g��
					m_pVtx[nIdx].nor = (m_aVecA[(nWide - 1) + (nDepth - 1) * m_nWidth] +
										m_aVecB[(nWide - 1) + (nDepth - 1) * m_nWidth] +
										m_aVecA[(nWide - 1) + (nDepth - 1) * m_nWidth + 1] +
										m_aVecB[(nWide - 1) + (nDepth - 1) * m_nWidth + m_nWidth] +
										m_aVecA[(nWide - 1) + (nDepth - 1) * m_nWidth + m_nWidth + 1] +
										m_aVecB[(nWide - 1) + (nDepth - 1) * m_nWidth + m_nWidth + 1]) / 6;
				}
			}
			else if (nDepth != 0 && nDepth != m_nDepth)					// �c�̗񂪒[����Ȃ��Ƃ�
			{
				if (nWide == 0)											// �擪�̂Ƃ�
				{
					// �@���x�N�g��
					m_pVtx[nIdx].nor = (m_aVecA[(nDepth - 1) * m_nWidth] +
						m_aVecB[m_nWidth + (nDepth - 1) * m_nWidth] +
						m_aVecA[m_nWidth + (nDepth - 1) * m_nWidth]) / 3;
				}
				else if (nWide == m_nWidth)								// �Ō���̂Ƃ�
				{
					// �@���x�N�g��
					m_pVtx[nIdx].nor = (m_aVecB[m_nWidth - 1 + m_nWidth * (nDepth - 1)] +
						m_aVecA[m_nWidth - 1 + m_nWidth * (nDepth - 1)] +
						m_aVecB[m_nWidth - 1 + m_nWidth * nDepth]) / 3;
				}
			}
		}
		// �z��̔ԍ�1�v���X����
		StartBox++;
		EndBox++;
	}

	// ���_�f�[�^���A�����b�N
	m_pVtxBuff->Unlock();
}
