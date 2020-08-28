//==================================================================================================================
//
// tutorial�̕`�� [tutorial.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS											// �x�����

//==================================================================================================================
//	�C���N���[�h�t�@�C��
//==================================================================================================================
#include "tutorial.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "sound.h"
#include "scene2D.h"
#include "scene3D.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "player.h"
#include "motionModel.h"
#include "inputGamepad.h"
#include "meshSphere.h"
#include "UI.h"
#include "game.h"

//==================================================================================================================
//	�ÓI�����o�ϐ��錾
//==================================================================================================================
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture = NULL;		// �e�N�X�`�����
CCamera *CTutorial::m_pCamera = NULL;					// �J�������
CLight *CTutorial::m_pLight = NULL;						// ���C�g���
CPolygon *CTutorial::m_pPolygon = NULL;					// �|���S�����
CMeshField *CTutorial::m_pMeshField = NULL;				// ���b�V���t�B�[���h���
CCharacter *CTutorial::m_pCharacter = NULL;				// �L�����N�^�[���
CUI *CTutorial::m_pUI = NULL;							// UI���
CMeshSphere *CTutorial::m_pMeshSphere = NULL;			// ���b�V�������
CModel *CTutorial::m_pModel[TUTORIAL_MAX_MODEL] = {};	// ���f�����

//==================================================================================================================
//	�R���X�g���N�^
//==================================================================================================================
CTutorial::CTutorial()
{

}

//==================================================================================================================
//	�f�X�g���N�^
//==================================================================================================================
CTutorial::~CTutorial()
{

}

//==================================================================================================================
//	����������
//==================================================================================================================
void CTutorial::Init(void)
{
	m_bCharaDecide[MAX_PLAYER] = false;	// �L�����N�^�[���I�΂ꂽ�ǂ���

	CMeshField::Load();				// ���b�V���t�B�[���h�e�N�X�`�����[�h
	CMotionModel::Load();			// ���f�����[�h
	CMeshSphere::Load();			// ���b�V�����̃e�N�X�`�����[�h
	CUI::Load();					// UI�e�N�X�`�����[�h

	// �J�����̐�������
	m_pCamera = CCamera::Create();

	// ���C�g�̐�������
	m_pLight = CLight::Create();

	// ���b�V�����̐�������
	m_pMeshSphere = CMeshSphere::Create();

	// ���b�V���t�B�[���h��������
	m_pMeshField = CMeshField::Create(INTEGER2(10, 10), D3DXVECTOR3(250.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// UI��������
	m_pUI = CUI::Create();
}

//==================================================================================================================
//	�I������
//==================================================================================================================
void CTutorial::Uninit(void)
{
	// scene2D�̔j������
	CScene2D::ReleaseAll();

	// scene3D�̔j������
	CScene3D::ReleaseAll();

	CMeshField::Unload();			// ���b�V���t�B�[���h�e�N�X�`���A�����[�h
	CMotionModel::Unload();			// �v���C���[�e�N�X�`�����f���A�����[�h
	CUI::Unload();					// UI�e�N�X�`���A�����[�h

	delete m_pLight;				// �������폜
	m_pLight = nullptr;				// �|�C���^NULL

	delete m_pCamera;				// �������폜
	m_pCamera = nullptr;			// �|�C���^NULL
}

//==================================================================================================================
//	�X�V����
//==================================================================================================================
void CTutorial::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �t�F�[�h�擾
	CFade::FADE fade = CFade::GetFade();

	// �ő�l���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// �L�����N�^�[���I�΂ꂽ�ǂ����擾
		m_bCharaDecide[nCnt] = m_pUI->GetCharaDecide(nCnt);
	}

	// �J�����̍X�V����
	m_pCamera->Update();

	// ���C�g�̍X�V����
	m_pLight->Update();

	// 1P��2P���L�����N�^�[��I�񂾂Ƃ�
	if (m_bCharaDecide[0] && m_bCharaDecide[1])
	{
		// �t�F�[�h�������Ȃ���
		if (fade == CFade::FADE_NONE)
		{
			// ���ꂼ��̃^�C�v��ۑ�
			CGame::SetPlayerType(PLAYER_ONE, m_pUI->GetCharaNum(PLAYER_ONE));
			CGame::SetPlayerType(PLAYER_TWO, m_pUI->GetCharaNum(PLAYER_TWO));

			// �t�F�[�h�̐ݒ�
			CFade::SetFade(CRenderer::MODE_MAPSELECT, DEFAULT_FADE_TIME);
		}
	}
}

//==================================================================================================================
//	�`�揈��
//==================================================================================================================
void CTutorial::Draw(void)
{
	// �J�����̐ݒ�
	m_pCamera->SetCamera();

	// �J�����̕`�揈��
	m_pCamera->Draw();
}

//==================================================================================================================
// �I�u�W�F�N�g�̐���
//==================================================================================================================
CTutorial * CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;		// �`���[�g���A������NULL�ɂ���

	pTutorial = new CTutorial;			// ���I�Ɋm��
	pTutorial->Init();					// ����������

	return pTutorial;					// �l��Ԃ�
}

//==================================================================================================================
// �L�����N�^�[���擾����
//==================================================================================================================
CCharacter * CTutorial::GetCharacter(void)
{
	return m_pCharacter;
}
