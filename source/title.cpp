//==================================================================================================================
//
// title�̕`�� [title.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#define _CRT_SECURE_NO_WARNINGS			// �x�����

//==================================================================================================================
//	�C���N���[�h�t�@�C��
//==================================================================================================================
#include "title.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "sound.h"
#include "scene2D.h"
#include "scene3D.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "motionModel.h"
#include "character.h"
#include "inputGamepad.h"
#include "meshSphere.h"
#include "UI.h"

//==================================================================================================================
//	�}�N����`
//==================================================================================================================
#define MAX_FADE_COUNT 800			// �t�F�[�h�̂̃J�E���g�̍ő吔

//==================================================================================================================
//	�ÓI�����o�ϐ��錾
//==================================================================================================================
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;		// �e�N�X�`�����
CCamera *CTitle::m_pCamera = NULL;					// �J�������
CLight *CTitle::m_pLight = NULL;					// ���C�g���
CPolygon *CTitle::m_pPolygon = NULL;				// �|���S�����
CMeshField *CTitle::m_pMeshField = NULL;			// ���b�V���t�B�[���h���
CMeshSphere *CTitle::m_pMeshSphere = NULL;			// ���b�V�������
CUI *CTitle::m_pUI = NULL;							// UI���
CCharacter *CTitle::m_pCharacter = NULL;			// �L�����N�^�[���
bool CTitle::m_bNextScreen = false;					// ���̃��[�h�ɂ������ǂ���

//==================================================================================================================
//	�R���X�g���N�^
//==================================================================================================================
CTitle::CTitle()
{

}

//==================================================================================================================
//	�f�X�g���N�^
//==================================================================================================================
CTitle::~CTitle()
{

}

//==================================================================================================================
//	����������
//==================================================================================================================
void CTitle::Init(void)
{
	// ������
	m_nNextMode = 0;			// ���̃��[�h�ԍ�
	m_bNextScreen = false;		// ���̉�ʂɍs�����ǂ���

	CMeshField::Load();			// ���b�V���t�B�[���h���[�h
	CMotionModel::Load();		// ���[�V�����p���f�����[�h
	CMeshSphere::Load();		// ���b�V�����̃e�N�X�`�����[�h
	CUI::Load();				// UI�̃e�N�X�`�����[�h

	// ���C�g�̐�������
	m_pLight = CLight::Create();

	// �J�����̐�������
	m_pCamera = CCamera::Create();

	// ���b�V�����̐�������
	m_pMeshSphere = CMeshSphere::Create();

	// ���b�V���t�B�[���h��������
	m_pMeshField = CMeshField::Create(INTEGER2(10, 10), D3DXVECTOR3(250.0f, 0.0f, 250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// UI�̐�������
	m_pUI = CUI::Create();
}

//==================================================================================================================
//	�I������
//==================================================================================================================
void CTitle::Uninit(void)
{
	// scene2D�̔j������
	CScene2D::ReleaseAll();

	// scene3D�̔j������
	CScene3D::ReleaseAll();

	CMeshField::Unload();			// ���e�N�X�`���A�����[�h
	CMotionModel::Unload();			// ���[�V�����p���f���A�����[�h
	CUI::Unload();					// UI�e�N�X�`���A�����[�h

	delete m_pLight;				// �������폜
	m_pLight = nullptr;				// �|�C���^NULL

	delete m_pCamera;				// �������폜
	m_pCamera = nullptr;			// �|�C���^NULL
}

//==================================================================================================================
//	�X�V����
//==================================================================================================================
void CTitle::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = nullptr;

	// �Q�[���p�b�h�ϐ�
	CInputGamepad *pInputGamepad[MAX_PLAYER];

	// �ő�l���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// �Q�[���p�b�h�擾
		pInputGamepad[nCnt] = CManager::GetInputGamepad(nCnt);
	}
	if (!pInputGamepad[PLAYER_ONE]->GetbConnect() ||
		!pInputGamepad[PLAYER_TWO]->GetbConnect())
			pInputKeyboard = CManager::GetInputKeyboard();

	// �t�F�[�h�擾
	CFade::FADE fade = CFade::GetFade();

	// �J�����̍X�V����
	m_pCamera->Update();

	// ���C�g�̍X�V����
	m_pLight->Update();

	// �L�[�{�[�h��[Enter] ���� �R���g���[���[��[START]���������Ƃ�
	if ((pInputGamepad[PLAYER_ONE]->GetTrigger(CInputGamepad::JOYPADKEY_START)) ||
		(pInputGamepad[PLAYER_TWO]->GetTrigger(CInputGamepad::JOYPADKEY_START)) || 
		(pInputKeyboard && pInputKeyboard->GetKeyboardTrigger(DIK_RETURN)))
	{
		// ���ʉ��Đ�
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
		// ���̉�ʂ̂Ƃ�
		if (!m_bNextScreen)
		{
			// �t�F�[�h�������Ȃ���
			if (fade == CFade::FADE_NONE)
			{
				// �t�F�[�h�̐ݒ�
				CFade::SetFade(CRenderer::MODE_TUTORIAL, DEFAULT_FADE_TIME);
			}
		}
		else
		{// ���̉�ʂɂ������ǂ���
			// ���̉�ʂɐi�ޏ�Ԃɂ���
			m_bNextScreen = true;
		}
	}
}

//==================================================================================================================
//	�`�揈��
//==================================================================================================================
void CTitle::Draw(void)
{
	// �J����������Ƃ�
	if (m_pCamera != NULL)
	{
		m_pCamera->SetCamera();			// �J�����̐ݒ�
		m_pCamera->Draw();				// �J�����̕`�揈��
	}
}

//==================================================================================================================
// �I�u�W�F�N�g�̐���
//==================================================================================================================
CTitle * CTitle::Create(void)
{
	CTitle *pTitle = NULL;			// �`���[�g���A������NULL�ɂ���

	pTitle = new CTitle;			// ���I�Ɋm��
	pTitle->Init();					// ����������

	return pTitle;					// �l��Ԃ�
}

//==================================================================================================================
// �L�����N�^�[���擾
//==================================================================================================================
CCharacter * CTitle::GetCharacter(void)
{
	return m_pCharacter;
}

//==================================================================================================================
// �J�������擾
//==================================================================================================================
CCamera * CTitle::GetCamera(void)
{
	return m_pCamera;
}

//==================================================================================================================
// ���̃��[�h�ݒ菈��
//==================================================================================================================
void CTitle::SetNextMode(int nNextMode)
{
	m_nNextMode = nNextMode;
}
