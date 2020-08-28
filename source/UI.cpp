//==================================================================================================================
//
// UI����[UI.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================
#include "UI.h"
#include "renderer.h"
#include "inputKeyboard.h"
#include "fade.h"
#include "player.h"
#include "scene2D.h"
#include "debugProc.h"
#include "game.h"
#include "title.h"
#include "tutorial.h"
#include "kananlibrary.h"
#include "inputGamepad.h"
#include "sound.h"

//==================================================================================================================
// �}�N����`
//==================================================================================================================
#define ENTER_SIZEX 300.0f							// �G���^�[���SX
#define ENTER_SIZEY 150.0f							// �G���^�[���SY
#define TITLE_ENTER_POSY 600.0f						// �^�C�g���G���^�[���S�ʒuY
#define NORMAL_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)// �摜�̂܂܂̐F
#define TITLEUI_BEGIN_X 1760						// �^�C�g��UI�ŏ��̉��̑傫��
#define TITLEUI_BEGIN_Y 1600						// �^�C�g��UI�ŏ��̏c�̑傫��
#define TITLEUI_SMALL_SPEED 30						// �^�C�g��UI�̏��������鑬�x
#define TITLEUI_VALUE_Y	700							// �����T�C�Y�̒lY
#define TITLEUI_BOUND_SPEED 10						// �^�C�g��UI�̃o�E���h���x
#define TITLEUI_BOUND_COUNT 60						// �^�C�g��UI�o�E���h�J�E���^
#define TITLEUI_FINISH_Y 250						// �^�C�g��UI�̍Ō�̈ʒuY
#define TITLEUI_UP_SPEED 2							// �^�C�g��UI�オ�鑬�x
#define ENTERUI_SIZE_X 950							// �G���^�[UI�傫����
#define ENTERUI_SIZE_Y 80							// �G���^�[UI�傫���c
#define ENTERUI_POS_Y 600							// �G���^�[UI�ʒuY
#define ENTERUI_ALPHA 0.03f							// �G���^�[UI���l�ύX�l
#define SIGNUI_BEGINPOS_Y 800						// �Ŕ�UI�ŏ��̈ʒu
#define SIGNUI_LASTPOS_Y 600						// �Ŕ�UI�Ō�̈ʒu
#define SIGNUI_LASTPOS_Y 600						// �Ŕ�UI�Ō�̈ʒu
#define SIGNUI_MOVE_SPEED 8.0f						// �Ŕ�UI�������x
#define SIGNUI_SIZE_X 700							// �Ŕ�UI�傫��X
#define SIGNUI_SIZE_Y 500							// �Ŕ�UI�傫��Y
#define MODE00UI_BEGINPOS_X 480						// ���[�h00UI�ŏ��̈ʒuX
#define MODE01UI_BEGINPOS_X 810						// ���[�h01UI�ŏ��̈ʒuX
#define MODEUI_BEGINPOS_Y 780						// ���[�hUI�ŏ��̈ʒuY
#define MODEUI_SIZE_X 300							// ���[�hUI�傫��X
#define MODEUI_SIZE_Y 120							// ���[�hUI�傫��Y
#define CLOCKUI0_POS_X 290							// ���vUI�ʒuX
#define CLOCKUI1_POS_X 990							// ���vUI�ʒuX
#define CLOCKHANDSUI_WAITTIME 30					// ���v�̐jUI�ҋ@����
#define CHARATEX_SISE_X 350							// �L�����e�N�X�`���傫��X
#define CHARATEX_SISE_Y 450							// �L�����e�N�X�`���傫��Y
#define CHARAUI_POS_Y 600							// �S���L����UI�ʒuY
#define CHARAFULLUI_SIZE_X 720						// �S���L����UI�傫��X
#define CHARAFULLUI_SIZE_Y 200						// �S���L����UI�傫��Y
#define CHARAFREAMUI_SIZE_X 180						// �L����UI�傫��X
#define CHARA1PUI_POS D3DXVECTOR3(200, 230, 0)		// 1P�L����UI�̈ʒu
#define CHARA2PUI_POS D3DXVECTOR3(1080, 230, 0)		// 2P�L����UI�̈ʒu
#define GEAR_POS_Y 530								// ���Ԃ̈ʒuY
#define CLOCK_HANDS_DIFF 0.1f						// ��]�̏����l
#define READY1PUI_POS D3DXVECTOR3(200, 450, 0)		// 1P���������ʒu
#define READY2PUI_POS D3DXVECTOR3(1080, 450, 0)		// 2P���������ʒu
#define READYUI_SIZE_X 200							// ���������傫��X
#define READYUI_SIZE_Y 160							// ���������傫��X
#define CHARA_SELECTUI_POS D3DXVECTOR3(640, 60, 0)	// �L�����N�^�[�I��UI�ʒu
#define CHARA_SELECTUI_SIZE_X 400					// �L�����N�^�[�I��UI�傫��X
#define CHARA_SELECTUI_SIZE_Y 120					// �L�����N�^�[�I��UI�傫��Y
#define MAPUI_SIZE_X 300							// �}�b�vUI�傫��X
#define MAPUI_SIZE_Y 250							// �}�b�vUI�傫��Y
#define MAPUI_POS_X0 650							// �}�b�vUI�c�̈ʒu0���
#define MAPUI_POS_X1 1050							// �}�b�vUI�c�̈ʒu1���
#define MAPUI_POS_Y0 200							// �}�b�vUI���̈ʒu0���
#define MAPUI_POS_Y1 500							// �}�b�vUI���̈ʒu1���
#define MAPFRAMEUI_SIZE_X 360						// �}�b�v�g���傫��X
#define MAPFRAMEUI_SIZE_Y 290						// �}�b�v�g���傫��Y
#define MAPEXPLANATION_POS D3DXVECTOR3(250, SCREEN_HEIGHT / 2, 0)// �}�b�v����UI�ʒu
#define MAPEXPLANATION_SIZE_X 400					// �}�b�v����UI�傫��X
#define MAPEXPLANATION_SIZE_Y 600					// �}�b�v����UI�傫��Y
#define STONE_POS_Y 620								// �Έʒu�c
#define STONE_SIZE_X 60								// �Α傫��X
#define STONE_SIZE_Y 50								// �Α傫��Y

//==================================================================================================================
// �ÓI�����o�[�ϐ��̏�����
//==================================================================================================================
LPDIRECT3DTEXTURE9 CUI::m_pTexture[LOGOTYPE_MAX] = {};		// �e�N�X�`�����
char *CUI::m_apFileName[LOGOTYPE_MAX] =						// �ǂݍ��ރ��f���̃\�[�X��
{
	{ "data/TEXTURE/title.png" },		// �^�C�g��
	{ "data/TEXTURE/PressStart.png" },	// �G���^�[
	{ "data/TEXTURE/arrow.png" },		// �R���p�X���
	{ "data/TEXTURE/sign00.png" },		// �Ŕ�
	{ "data/TEXTURE/Mode00.png" },		// ���[�h00
	{ "data/TEXTURE/Mode01.png" },		// ���[�h01
	{ "data/TEXTURE/ModeFream.png" },	// ���[�h�g��
	{ "data/TEXTURE/gear.png" },		// ����0
	{ "data/TEXTURE/clock.png" },		// ���v�y��0
	{ "data/TEXTURE/Clock hands.png" },	// ���v�̐j0
	{ "data/TEXTURE/clock gear.png" },	// ���v�̎���0
	{ "data/TEXTURE/gear.png" },		// ����1
	{ "data/TEXTURE/clock.png" },		// ���v�y��1
	{ "data/TEXTURE/Clock hands.png" },	// ���v�̐j1
	{ "data/TEXTURE/clock gear.png" },	// ���v�̎���1
	{ "data/TEXTURE/FULLchara.png" },	// �L�����N�^�[�S��
	{ "data/TEXTURE/FULLchara.png" },	// 1P�L�����N�^�[
	{ "data/TEXTURE/FULLchara.png" },	// 2P�L�����N�^�[
	{ "data/TEXTURE/1Pchara.png" },		// 1P�L�����N�^�[�g
	{ "data/TEXTURE/2Pchara.png" },		// 2P�L�����N�^�[�g
	{ "data/TEXTURE/charaName.png" },	// 1P�L�����N�^�[�l�[��
	{ "data/TEXTURE/charaName.png" },	// 2P�L�����N�^�[�l�[��
	{ "data/TEXTURE/Ready.png" },		// 1P�L�����N�^�[��������
	{ "data/TEXTURE/Ready.png" },		// 2P�L�����N�^�[��������
	{ "data/TEXTURE/PlayerSelect.png" },// �v���C���[�Z���N�g�A�C�R��
	{ "data/TEXTURE/worldMap.jpg" },	// ���E�n�}
	{ "data/TEXTURE/MapFrame.jpg" },	// �}�b�v�I��g
	{ "data/TEXTURE/map1.jpg" },		// �}�b�v1
	{ "data/TEXTURE/map2.jpg" },		// �}�b�v2
	{ "data/TEXTURE/coming soon.jpg" },	// �}�b�v3(coming soon)
	{ "data/TEXTURE/coming soon.jpg" },	// �}�b�v4(coming soon)
	{ "data/TEXTURE/MapExplanation.jpg" },// �}�b�v����
	{ "data/TEXTURE/MapExplanation.jpg" },// �}�b�v����
	{ "data/TEXTURE/MapExplanation.jpg" },// �}�b�v����
	{ "data/TEXTURE/MapExplanation.jpg" },// �}�b�v����
	{ "data/TEXTURE/gameBG.png" },		// �Q�[���w�i
	{ "data/TEXTURE/jewelryBG.png" },	// ��Δw�i1P
	{ "data/TEXTURE/jewelryBG.png" },	// ��Δw�i2P
	{ "data/TEXTURE/jewelryRed.png" },	// ��ΐ�
	{ "data/TEXTURE/jewelryBule.png" },	// ��ΐ�
	{ "data/TEXTURE/jewelryGreen.png" },// ��Η�
	{ "data/TEXTURE/FULLchara.png" },	// �L����1P
	{ "data/TEXTURE/FULLchara.png" },	// �L����2P
	{ "data/TEXTURE/charaName0.png" },	// 1P�̃L�����N�^�[�l�[��
	{ "data/TEXTURE/charaName1.png" },	// 2P�̃L�����N�^�[�l�[��
};

int CUI::m_nMapID = 0;	// �}�b�vID

//==================================================================================================================
//
// �R���X�g���N�^
//
//==================================================================================================================
CUI::CUI(PRIORITY type = CScene::PRIORITY_UI) : CScene(type)
{

}

//==================================================================================================================
//
// �f�X�g���N�^
//
//==================================================================================================================
CUI::~CUI()
{

}

//==================================================================================================================
// ����������
//==================================================================================================================
void CUI::Init(void)
{
	TitlePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);// �^�C�g���̈ʒu
	m_nCntBound = 0;			// �^�C�g��UI�o�E���h�J�E���^
	m_nMode = 0;				// ���[�h�ԍ�
	m_nCharaNum[0] = 0;			// �L�����ԍ�
	m_nCharaNum[1] = 3;			// �L�����ԍ�
	m_nCntRot[MAX_PLAYER] = 0;	// ���v�̐j�̉�]�p�J�E���^
	m_nCntWait[MAX_PLAYER] = 0;	// �ҋ@���ԗp�J�E���^
	m_nCntMove[MAX_PLAYER] = 0;	// �ړ��p�J�E���^
	m_nMapID = 0;				// �}�b�v�ԍ�
	m_fCntUITitle0 = 0;			// �^�C�g��UI�p�J�E���^0
	m_fCntUITitle1 = 0;			// �^�C�g��UI�p�J�E���^1
	m_fCntEnter = 0;			// �G���^�[�p�J�E���^
	m_fCntUISign = 0;			// �Ŕp�J�E���^
	m_nPlayer = 0;				// �v���C���[�ԍ�
	m_fPosMove[MAX_PLAYER] = 0;	// �ʒu�ړ��ϐ�
	m_fPos[0] = 370;			// ���݂̘g���e�N�X�`���̈ʒuX
	m_fPos[1] = 910;			// ���݂̘g���e�N�X�`���̈ʒuX
	m_fPosDiff[MAX_PLAYER] = 0;	// �ڕW�̘g���e�N�X�`���̈ʒuX
	m_fPosOld[MAX_PLAYER] = 0;	// �O��̘g���e�N�X�`���̈ʒuX
	m_fRotGear[MAX_PLAYER] = 0;	// ���Ԃ̉�]�i�[�ϐ�
	m_fPosCul[MAX_PLAYER] = 0;	// �ʒu�v�Z�p�ϐ�
	m_fDiff[MAX_PLAYER] = 0;	// 1�t���[���O�Ƃ̋���
	m_fAngle[MAX_PLAYER] = 0;	// ���Ԃ̉�]�p�x
	m_fRad[MAX_PLAYER] = 0;		// ���W�A���l
	m_bUITitle0 = false;		// �^�C�g���𓮂������ǂ���
	m_bUITitle1 = false;		// �^�C�g���𓮂������ǂ���
	m_bUIEnter = false;			// �G���^�[�̃��l�p�ϐ�
	m_bUIClockHands[MAX_PLAYER] = false;// ���v�̐j�����������ǂ���0
	m_bCharaDecide[MAX_PLAYER] = false;// �����̃L�����N�^�[��I���������ǂ���
	m_bStickReturn[MAX_PLAYER] = false;// �p�b�h�X�e�B�b�N��߂������ǂ���
	m_bMapSelect = false;		// �}�b�v��I���������ǂ���
	m_bTransform[MAX_PLAYER] = false;// �ϐg�������ǂ���
	m_bStoneID[MAX_PLAYER][CStone::STONE_ID_MAX] = false;// �X�g�[�����擾�������ǂ���

	// ���S�̍ő喇���J�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// �^�C�g���̂Ƃ�
		if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
		{
			// �^�C�g���Ŏg��UI�̂Ƃ�
			if (nCnt <= LOGOTYPE_MODEFREAM)
			{
				// ��������
				m_pScene2D[nCnt] = CScene2D::Create();

				// �e�N�X�`����\��
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
		{// �`���[�g���A���̂Ƃ�
			// �`���[�g���A���Ŏg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_MODEFREAM && nCnt <= LOGOTYPE_SELECTICON)
			{
				// ��������
				m_pScene2D[nCnt] = CScene2D::Create();

				// �e�N�X�`����\��
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
		{// �}�b�v�I����ʂ̂Ƃ�
			// �}�b�v�I����ʂŎg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_SELECTICON && nCnt <= LOGOTYPE_MAPEXPLANATION4)
			{
				// ��������
				m_pScene2D[nCnt] = CScene2D::Create();

				// �e�N�X�`����\��
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_GAME)
		{// �Q�[���̂Ƃ�
			// �Q�[���Ŏg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_MAPEXPLANATION4 && nCnt <= LOGOTYPE_CHARANAME1)
			{
				// ��������
				m_pScene2D[nCnt] = CScene2D::Create();

				// �e�N�X�`����\��
				m_pScene2D[nCnt]->BindTex(m_pTexture[nCnt]);
			}
		}
	}
}

//==================================================================================================================
// �I������
//==================================================================================================================
void CUI::Uninit(void)
{

}

//==================================================================================================================
// �X�V����
//==================================================================================================================
void CUI::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�ϐ�
	CInputGamepad *pGamepad[MAX_PLAYER];

	// �ő�l���܂ŃJ�E���g
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		// �Q�[���p�b�h�擾
		pGamepad[nCnt] = CManager::GetInputGamepad(nCnt);
	}

	// UI�^�C�g���̍X�V����
	TitleUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);

	// UI�`���[�g���A���X�V����
	TutorialUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);

	// UI�}�b�v�I����ʍX�V����
	MapSelectUpdate(pInputKeyboard, pGamepad[0], pGamepad[1]);

	// UI�Q�[���X�V����
	GameUpdate();
}

//==================================================================================================================
// �`�揈��
//==================================================================================================================
void CUI::Draw(void)
{

}

//==================================================================================================================
// ��������
//==================================================================================================================
CUI *CUI::Create(void)
{
	// �V�[�����I�Ɋm��
	CUI *pUI = new CUI(CScene::PRIORITY_UI);

	// �V�[��������
	pUI->Init();

	// �l��Ԃ�
	return pUI;
}

//==================================================================================================================
// �e�N�X�`�����[�h
//==================================================================================================================
HRESULT CUI::Load(void)
{
	CRenderer *pRenderer = CManager::GetRenderer();			// �����_���[���擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		// �f�o�C�X�̎擾

	//==============================�e�N�X�`���̓ǂݍ���==============================//
	// �e�N�X�`���̍ő吔�܂ŃJ�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// ���[�h���^�C�g���̂Ƃ�
		if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
		{
			// �^�C�g���Ŏg��UI�̂Ƃ�
			if (nCnt <= LOGOTYPE_MODEFREAM)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
		{// ���[�h���`���[�g���A���̂Ƃ�
			// �`���[�g���A���Ŏg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_MODEFREAM && nCnt <= LOGOTYPE_SELECTICON)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
		{// ���[�h���}�b�v�I����ʂ̂Ƃ�
			// �}�b�v�I����ʂŎg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_SELECTICON && nCnt <= LOGOTYPE_MAPEXPLANATION4)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_GAME)
		{// ���[�h���Q�[���̂Ƃ�
			// �Q�[���Ŏg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_MAPEXPLANATION4 && nCnt <= LOGOTYPE_CHARANAME1)
			{
				// �e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice, m_apFileName[nCnt], &m_pTexture[nCnt]);
			}
		}
	}

	// �l��Ԃ�
	return S_OK;
}

//==================================================================================================================
// �j��
//==================================================================================================================
void CUI::Unload(void)
{
	// ���S�̍ő��ނ܂ŃJ�E���g
	for (int nCnt = 0; nCnt < LOGOTYPE_MAX; nCnt++)
	{
		// ���[�h���^�C�g���̂Ƃ�
		if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
		{
			// �^�C�g���Ŏg�����S�̂Ƃ�
			if (nCnt <= LOGOTYPE_MODEFREAM)
			{
				m_pTexture[nCnt]->Release();		// �J��
				m_pTexture[nCnt] = NULL;			// NULL�ɂ���
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
		{// ���[�h���`���[�g���A���̂Ƃ�
			// �`���[�g���A���Ŏg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_MODEFREAM && nCnt <= LOGOTYPE_SELECTICON)
			{
				m_pTexture[nCnt]->Release();		// �J��
				m_pTexture[nCnt] = NULL;			// NULL�ɂ���
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
		{// ���[�h���}�b�v�I����ʂ̂Ƃ�
			// �}�b�v�I����ʂŎg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_SELECTICON && nCnt <= LOGOTYPE_MAPEXPLANATION4)
			{
				m_pTexture[nCnt]->Release();		// �J��
				m_pTexture[nCnt] = NULL;			// NULL�ɂ���
			}
		}
		else if (CRenderer::GetMode() == CRenderer::MODE_GAME)
		{// ���[�h���Q�[���̂Ƃ�
			// �Q�[���Ŏg��UI�̂Ƃ�
			if (nCnt > LOGOTYPE_MAPEXPLANATION4 && nCnt <= LOGOTYPE_CHARANAME1)
			{
				m_pTexture[nCnt]->Release();		// �J��
				m_pTexture[nCnt] = NULL;			// NULL�ɂ���
			}
		}
	}
}

//==================================================================================================================
// ��ގ擾
//==================================================================================================================
CUI::UITYPE CUI::GetType(void)
{
	return m_type;
}

//==================================================================================================================
// �^�C�g���X�V����
//==================================================================================================================
void CUI::TitleUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1)
{
	// �^�C�g���̂Ƃ�
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
	{
		// ���̃��[�h�ɂ������ǂ�������擾
		bool bModeNext = CTitle::GetbModeNext();

		if (!bModeNext)
		{
			// �^�C�g��UI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + TitlePos.y, 0.0f), TITLEUI_BEGIN_X + m_fCntUITitle0 - m_fCntUITitle1,
				TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1, LOGOTYPE_TITLE, NORMAL_COLOR);

			// �o�E���h�J�E���g���K��l�ȉ��̂Ƃ�
			if (m_nCntBound < TITLEUI_BOUND_COUNT)
			{
				// �^�C�g���𓮂����Ă��ȂƂ�
				if (!m_bUITitle0)
				{
					// �^�C�g��UI�̏c�̒������K��l�ȉ��̂Ƃ�
					if (TITLEUI_BEGIN_Y + m_fCntUITitle0 <= TITLEUI_VALUE_Y)
					{
						// �^�C�g���𓮂�����Ԃɂ���
						m_bUITitle0 = true;

						// �^�C�g��UI�c�����K��l�ɂ���
						m_fCntUITitle0 = TITLEUI_VALUE_Y - TITLEUI_BEGIN_Y;
					}
					else
					{
						// �^�C�g���J�E���^���Z
						m_fCntUITitle0 -= TITLEUI_SMALL_SPEED;
					}
				}
				else
				{// �^�C�g���𓮂����Ă����Ƃ�
					// �^�C�g��UI�̏c�̒�����[370]�ȏ�̂Ƃ�
					if (TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1 >= TITLEUI_VALUE_Y + 40)
					{
						// �^�C�g�����ő�܂Ŋg�傳����
						m_bUITitle1 = true;

					}
					else if (TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1 <= TITLEUI_VALUE_Y - 40)
					{// �^�C�g��UI�̏c�̒�����[330]�ȉ��̂Ƃ�
						// �^�C�g�����ŏ��܂Ŋg��������
						m_bUITitle1 = false;
					}

					// �^�C�g�����ő�܂Ŋg�傳�����Ƃ�
					if (m_bUITitle1)
					{
						// �^�C�g���J�E���^���Z
						m_fCntUITitle1 -= TITLEUI_BOUND_SPEED;
					}
					else
					{// �^�C�g�����ŏ��܂Ŋg���������Ƃ�
						// �^�C�g���J�E���^���Z
						m_fCntUITitle1 += TITLEUI_BOUND_SPEED;
					}

					// �^�C�g��UI�o�E���h�J�E���^���Z
					m_nCntBound++;
				}
			}
			else
			{// �o�E���h�J�E���g���K��l�𒴂����Ƃ�
				// �^�C�g��UI�̈ʒuY���K��l�ȉ��̂Ƃ�
				if (SCREEN_HEIGHT / 2 + TitlePos.y < TITLEUI_FINISH_Y)
				{
					// �G���^�[UI
					SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, ENTERUI_POS_Y, 0.0f), ENTERUI_SIZE_X, ENTERUI_SIZE_Y,
						LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f + m_fCntEnter));

					// �G���^�[UI�̃��l��1.0�ȏ�̂Ƃ�
					if (1.0f + m_fCntEnter >= 1.0f)
					{
						// �G���^�[�p���l�����炷��Ԃɂ���
						m_bUIEnter = true;
					}
					else if (1.0f + m_fCntEnter <= 0.0f)
					{
						// �G���^�[�p���l�𑝂₷��Ԃɂ���
						m_bUIEnter = false;
					}

					// �G���^�[UI�����炷��Ԃ̂Ƃ�
					if (m_bUIEnter)
					{
						// �J�E���^���Z
						m_fCntEnter -= ENTERUI_ALPHA;
					}
					else
					{// �G���^�[UI�𑝂₷��Ԃ̂Ƃ�
					 // �J�E���^���Z
						m_fCntEnter += ENTERUI_ALPHA;
					}
				}
				else
				{
					// �^�C�g���ʒu���Z
					TitlePos.y -= TITLEUI_UP_SPEED;
				}
			}
		}
		else
		{
			// �^�C�g��UI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, TITLEUI_FINISH_Y + m_fCntUISign / 5, 0.0f), TITLEUI_BEGIN_X + m_fCntUITitle0 - m_fCntUITitle1,
				TITLEUI_BEGIN_Y + m_fCntUITitle0 + m_fCntUITitle1, LOGOTYPE_TITLE, NORMAL_COLOR);

			// �G���^�[UI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, ENTERUI_POS_Y, 0.0f), ENTERUI_SIZE_X, ENTERUI_SIZE_Y,
				LOGOTYPE_ENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			// �Ŕ�UI
			SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SIGNUI_BEGINPOS_Y + m_fCntUISign, 0.0f), SIGNUI_SIZE_X, SIGNUI_SIZE_Y, LOGOTYPE_SIGN, NORMAL_COLOR);

			// ���[�h00UI
			SetUI(D3DXVECTOR3(MODE00UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_fCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODE00, NORMAL_COLOR);
			// ���[�h01UI
			SetUI(D3DXVECTOR3(MODE01UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_fCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODE01, NORMAL_COLOR);

			// �Ŕ�UI���K��l���傫����
			if (SIGNUI_BEGINPOS_Y + m_fCntUISign > SIGNUI_LASTPOS_Y)
			{
				// �Ŕp�J�E���^���Z
				m_fCntUISign -= SIGNUI_MOVE_SPEED;
			}

			// ���[�h�ԍ���[0]�̂Ƃ�
			if (m_nMode == 0)
			{
				// ���[�h�g��UI
				SetUI(D3DXVECTOR3(MODE00UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_fCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODEFREAM, NORMAL_COLOR);
			}
			else if (m_nMode == 1)
			{
				// ���[�h�g��UI
				SetUI(D3DXVECTOR3(MODE01UI_BEGINPOS_X, MODEUI_BEGINPOS_Y + m_fCntUISign, 0.0f), MODEUI_SIZE_X, MODEUI_SIZE_Y, LOGOTYPE_MODEFREAM, NORMAL_COLOR);
			}

			// �Q�[���p�b�h�L����
			if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
				// �Q�[���p�b�h����
				ControlGamepad(pGamepad0, pGamepad1);
			// �Q�[���p�b�h������
			else
				// �L�[�{�[�h����
				ControlKeyboard(pKeyboard);
		}
	}
}

//==================================================================================================================
// �`���[�g���A���X�V����
//==================================================================================================================
void CUI::TutorialUpdate(CInputKeyboard * pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1)
{
	// �`���[�g���A���̂Ƃ�
	if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{
		// �L�����N�^�[�S��UI
		SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, CHARAUI_POS_Y, 0.0f), CHARAFULLUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_CHARAFULL, NORMAL_COLOR);

		// �Q�[���p�b�h�L����
		if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
			// �Q�[���p�b�h����
			ControlGamepad(pGamepad0, pGamepad1);
		// �Q�[���p�b�h������
		else
			// �L�[�{�[�h����
			ControlKeyboard(pKeyboard);

		// ���v�y��0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCK0, NORMAL_COLOR);
		// ���v�̐j0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKHANDS0, NORMAL_COLOR);
		// ���v�̐j��]�ݒ�
		m_pScene2D[LOGOTYPE_CLOCKHANDS0]->SetRot(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0), -D3DX_PI / 6 - m_nCntRot[0] * CLOCK_HANDS_DIFF, D3DXVECTOR3(0, 0, 0));
		// ���v�̐j0UI
		SetUI(D3DXVECTOR3(CLOCKUI0_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKGEAR0, NORMAL_COLOR);

		// ���v�̐j�������Ă�����Ԃ̂Ƃ�
		if (m_bUIClockHands[0])
		{
			// ���v�̐j���ő��]�l���傫���Ƃ�
			if ((-D3DX_PI / 6) * 5 < (-D3DX_PI / 6) - m_nCntRot[0] * CLOCK_HANDS_DIFF)
			{
				// ��]�����Ă���
				m_nCntRot[0]++;
			}
			else
			{
				// �ҋ@���ԉ��Z
				m_nCntWait[0]++;

				// �ҋ@���Ԃ�0.5�b�o�����Ƃ�
				if (m_nCntWait[0] >= CLOCKHANDSUI_WAITTIME)
				{
					m_bUIClockHands[0] = false;
				}
			}
		}
		else
		{
			// �ҋ@���ԗp�J�E���^������
			m_nCntWait[0] = 0;

			// ��]�����Ă���
			m_nCntRot[0]--;

			// ��]�p�J�E���^��[0]�ȉ��ɂȂ����Ƃ�
			if (m_nCntRot[0] <= 0)
			{
				// ��]�p�J�E���^������
				m_nCntRot[0] = 0;
			}
		}

		// ���v�y��1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCK1, NORMAL_COLOR);
		// ���v�̐j1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKHANDS1, NORMAL_COLOR);
		m_pScene2D[LOGOTYPE_CLOCKHANDS1]->SetRot(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0), D3DX_PI / 6 + m_nCntRot[1] * CLOCK_HANDS_DIFF, D3DXVECTOR3(0, 0, 0));
		// ���v�̐j1UI
		SetUI(D3DXVECTOR3(CLOCKUI1_POS_X, CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFREAMUI_SIZE_X, LOGOTYPE_CLOCKGEAR1, NORMAL_COLOR);

		// ���v�̐j�������Ă�����Ԃ̂Ƃ�
		if (m_bUIClockHands[1])
		{
			// ���v�̐j���ő��]�l���傫���Ƃ�
			if ((D3DX_PI / 6) * 5 > (D3DX_PI / 6) + m_nCntRot[1] * CLOCK_HANDS_DIFF)
			{
				// ��]�����Ă���
				m_nCntRot[1]++;
			}
			else
			{
				// �ҋ@���ԉ��Z
				m_nCntWait[1]++;

				// �ҋ@���Ԃ�0.5�b�o�����Ƃ�
				if (m_nCntWait[1] >= CLOCKHANDSUI_WAITTIME)
				{
					m_bUIClockHands[1] = false;
				}
			}
		}
		else
		{
			// �ҋ@���ԗp�J�E���^������
			m_nCntWait[1] = 0;

			// ��]�����Ă���
			m_nCntRot[1]--;

			// ��]�p�J�E���^��[0]�ȉ��ɂȂ����Ƃ�
			if (m_nCntRot[1] <= 0)
			{
				// ��]�p�J�E���^������
				m_nCntRot[1] = 0;
			}
		}

		// �ő�l���܂ŃJ�E���g
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// ���݂̈ʒu��O��̈ʒu�ɑ��
			m_fPosOld[nCnt] = m_fPos[nCnt];

			// �L�����ԍ���0��菬�����Ȃ����Ƃ�
			if (m_nCharaNum[nCnt] < 0)
			{
				// 0�ɖ߂�
				m_nCharaNum[nCnt] = 0;
			}

			// �L�����ԍ���3���傫���Ȃ����Ƃ�
			if (m_nCharaNum[nCnt] > 3)
			{
				// 3�ɖ߂�
				m_nCharaNum[nCnt] = 3;
			}
			// �ڕW�ʒu�i�[�ϐ�
			m_fPosDiff[nCnt] = 370 + (float)m_nCharaNum[nCnt] * CHARAFREAMUI_SIZE_X;
		}

		// �ő�l���܂ŃJ�E���g
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			float fRadius = CHARAFULLUI_SIZE_Y / 2;											// ���ԃe�N�X�`���̔��a
			m_fPosCul[nCnt] = ((m_fPosDiff[nCnt] - m_fPos[nCnt]) / 60) * m_fPosMove[nCnt];	// �v�Z�p�ϐ�
			m_fDiff[nCnt] = fabsf(m_fPos[nCnt] + m_fPosCul[nCnt] - m_fPosOld[nCnt]);		// 1�t���[����̋���
			m_fAngle[nCnt] = 180 * m_fDiff[nCnt] / (D3DX_PI * fRadius);						// �p�x�Z�o
			m_fRad[nCnt] = D3DX_PI * m_fAngle[nCnt] / 360;					// ���W�A���l

			// �ʒu�̍������̂Ƃ�
			if (m_fPosCul[nCnt] > 0)
			{
				m_fRotGear[nCnt] = m_fRotGear[nCnt] + m_fRad[nCnt];					// �p�x�i�[(���Z)
			}
			else if (m_fPosCul[nCnt] < 0)
			{// �ʒu�̍������̂Ƃ�
				m_fRotGear[nCnt] = m_fRotGear[nCnt] - m_fRad[nCnt];					// �p�x�i�[(���Z)
			}
		}

		// 1P�L�����N�^�[�g��UI
		SetUI(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_1PCHARA_FREAM, NORMAL_COLOR);
		// ����0UI
		SetUI(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], GEAR_POS_Y, 0.0f), CHARAFULLUI_SIZE_Y, CHARAFULLUI_SIZE_Y, LOGOTYPE_GEAR0, NORMAL_COLOR);
		// ��]�ݒ�
		m_pScene2D[LOGOTYPE_GEAR0]->SetRot(D3DXVECTOR3(m_fPos[0] + m_fPosCul[0], GEAR_POS_Y, 0), m_fRotGear[0], D3DXVECTOR3(0, 0, 0));

		// 2P�L�����N�^�[�g��UI
		SetUI(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], CHARAUI_POS_Y, 0.0f), CHARAFREAMUI_SIZE_X, CHARAFULLUI_SIZE_Y, LOGOTYPE_2PCHARA_FREAM, NORMAL_COLOR);
		// ����1UI
		SetUI(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], GEAR_POS_Y, 0.0f), CHARAFULLUI_SIZE_Y, CHARAFULLUI_SIZE_Y, LOGOTYPE_GEAR1, NORMAL_COLOR);
		// ��]�ݒ�
		m_pScene2D[LOGOTYPE_GEAR1]->SetRot(D3DXVECTOR3(m_fPos[1] + m_fPosCul[1], GEAR_POS_Y, 0), m_fRotGear[1], D3DXVECTOR3(0, 0, 0));

		// �ő�l���܂ŃJ�E���g
		for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
		{
			// �ʒu����������Ȃ��Ƃ�
			if (m_fPosDiff[nCnt] != m_fPos[nCnt])
			{
				// �ʒu�ړ��p�J�E���^���Z
				m_fPosMove[nCnt]++;
			}

			// ���݂̈ʒu�ݒ�
			m_fPos[nCnt] = m_fPos[nCnt] + m_fPosCul[nCnt];

			// ���݂̈ʒu��O��̈ʒu�ɑ��
			m_fPosOld[nCnt] = m_fPos[nCnt] + m_fPosCul[nCnt];

		}

		// 1P�L�����N�^�[UI
		SetUI(CHARA1PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, LOGOTYPE_1PCHARA, NORMAL_COLOR);
		// �e�N�X�`���ݒ�
		m_pScene2D[LOGOTYPE_1PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0]);
		// 2P�L�����N�^�[UI
		SetUI(CHARA2PUI_POS, CHARATEX_SISE_X, CHARATEX_SISE_Y, LOGOTYPE_2PCHARA, NORMAL_COLOR);
		// �e�N�X�`���ݒ�
		m_pScene2D[LOGOTYPE_2PCHARA]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1]);

		// 1P�L�����N�^�[�l�[��UI
		SetUI(READY1PUI_POS, 400, 120, LOGOTYPE_1PCHARA_NAME, NORMAL_COLOR);
		// �e�N�X�`���ݒ�
		m_pScene2D[LOGOTYPE_1PCHARA_NAME]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[0]) * 0.25f, 0.0f + (m_nCharaNum[0]) * 0.25f, 0);
		// 2P�L�����N�^�[�l�[��UI
		SetUI(READY2PUI_POS, 400, 120, LOGOTYPE_2PCHARA_NAME, NORMAL_COLOR);
		// �e�N�X�`���ݒ�
		m_pScene2D[LOGOTYPE_2PCHARA_NAME]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[1]) * 0.25f, 0.0f + (m_nCharaNum[1]) * 0.25f, 0);

		// 1P�L�����N�^�[�I������Ă���Ƃ�
		if (m_bCharaDecide[0])
		{
			// 1P�L�����N�^�[��������
			SetUI(READY1PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, LOGOTYPE_1PREADY, NORMAL_COLOR);
		}
		else
		{// 1P�L�����N�^�[�I������Ă��Ȃ��Ƃ�
			// 1P�L�����N�^�[��������
			SetUI(READY1PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, LOGOTYPE_1PREADY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// 2P�L�����N�^�[�I������Ă���Ƃ�
		if (m_bCharaDecide[1])
		{
			// 2P�L�����N�^�[UI
			SetUI(READY2PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, LOGOTYPE_2PREADY, NORMAL_COLOR);
		}
		else
		{// 2P�L�����N�^�[�I������Ă��Ȃ��Ƃ�
			// 2P�L�����N�^�[��������
			SetUI(READY2PUI_POS, READYUI_SIZE_X, READYUI_SIZE_Y, LOGOTYPE_2PREADY, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}

		// �L�����N�^�[�I���A�C�R��
		SetUI(CHARA_SELECTUI_POS, CHARA_SELECTUI_SIZE_X, CHARA_SELECTUI_SIZE_Y, LOGOTYPE_SELECTICON, NORMAL_COLOR);
	}
}

//==================================================================================================================
// �}�b�v�I����ʂ̍X�V����
//==================================================================================================================
void CUI::MapSelectUpdate(CInputKeyboard * pKeyboard, CInputGamepad * pGamepad0, CInputGamepad * pGamepad1)
{
	// �}�b�v�I����ʂ̂Ƃ�
	if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
	{
		// ���E�n�}
		SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, LOGOTYPE_WORLDMAP, NORMAL_COLOR);

		// �}�b�v1
		SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP1, NORMAL_COLOR);

		// �}�b�v2
		SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP2, NORMAL_COLOR);

		// �}�b�v3
		SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y1, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP3, NORMAL_COLOR);

		// �}�b�v4
		SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y1, 0.0f), MAPUI_SIZE_X, MAPUI_SIZE_Y, LOGOTYPE_MAP4, NORMAL_COLOR);

		// �}�b�vID��0��菬������
		if (m_nMapID < 0)
		{
			// 0�ɂ���
			m_nMapID = 0;
		}

		// �܂���ID��3���傫����
		if (m_nMapID > 3)
		{
			// 3�ɂ���
			m_nMapID = 3;
		}

		switch (m_nMapID)
		{
		case 0:
			// �}�b�v�g��
			SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, NORMAL_COLOR);
			// �}�b�v����
			SetUI(MAPEXPLANATION_POS, MAPEXPLANATION_SIZE_X, MAPEXPLANATION_SIZE_Y, LOGOTYPE_MAPEXPLANATION1, NORMAL_COLOR);
			break;
		case 1:
			// �}�b�v�g��
			SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y0, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, NORMAL_COLOR);
			// �}�b�v����
			SetUI(MAPEXPLANATION_POS, MAPEXPLANATION_SIZE_X, MAPEXPLANATION_SIZE_Y, LOGOTYPE_MAPEXPLANATION2, NORMAL_COLOR);
			break;
		case 2:
			// �}�b�v�g��
			SetUI(D3DXVECTOR3(MAPUI_POS_X0, MAPUI_POS_Y1, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, NORMAL_COLOR);
			// �}�b�v����
			SetUI(MAPEXPLANATION_POS, MAPEXPLANATION_SIZE_X, MAPEXPLANATION_SIZE_Y, LOGOTYPE_MAPEXPLANATION3, NORMAL_COLOR);
			break;
		case 3:
			// �}�b�v�g��
			SetUI(D3DXVECTOR3(MAPUI_POS_X1, MAPUI_POS_Y1, 0.0f), MAPFRAMEUI_SIZE_X, MAPFRAMEUI_SIZE_Y, LOGOTYPE_MAPFRAME, NORMAL_COLOR);
			// �}�b�v����
			SetUI(MAPEXPLANATION_POS, MAPEXPLANATION_SIZE_X, MAPEXPLANATION_SIZE_Y, LOGOTYPE_MAPEXPLANATION4, NORMAL_COLOR);
			break;
		}

		// �Q�[���p�b�h�L����
		if (pGamepad0->GetbConnect() || pGamepad1->GetbConnect())
			// �Q�[���p�b�h����
			ControlGamepad(pGamepad0, pGamepad1);
		// �Q�[���p�b�h������
		else
			// �L�[�{�[�h����
			ControlKeyboard(pKeyboard);
	}
}

//==================================================================================================================
// �Q�[���X�V����
//==================================================================================================================
void CUI::GameUpdate(void)
{
	// �Q�[���̂Ƃ�
	if (CRenderer::GetMode() == CRenderer::MODE_GAME)
	{
		CPlayer *pPlayer0, *pPlayer1;

		// �v���C���[���擾
		pPlayer0 = CGame::GetPlayer(0);
		pPlayer1 = CGame::GetPlayer(1);

		// �v���C���[��2�̂���Ƃ�
		if (pPlayer0 != NULL && pPlayer1 != NULL)
		{
			// �v���C���[�ԍ��擾
			m_nCharaNum[0] = pPlayer0->GetCharaType();
			m_nCharaNum[1] = pPlayer1->GetCharaType();

			// �ő�΂܂ŃJ�E���g
			for (int nCntStone = 0; nCntStone < CStone::STONE_ID_MAX; nCntStone++)
			{
				// �e�v���C���[���ǂ̎�ނ̐΂��l��������
				m_bStoneID[0][nCntStone] = pPlayer0->GetStoneType(nCntStone);
				m_bStoneID[1][nCntStone] = pPlayer1->GetStoneType(nCntStone);
			}
		}

		// �Q�[���w�i
		SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, LOGOTYPE_GAMEBG, NORMAL_COLOR);

		if (!m_bTransform[0])
		{
			// ��Δw�i1P
			SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG1P, NORMAL_COLOR);

			// 1P���Ԑ΂�������Ƃ�
			if (m_bStoneID[0][0])
			{
				// ��ΐ�
				SetUI(D3DXVECTOR3(125, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, NORMAL_COLOR);
			}

			// 1P���ΐ΂�������Ƃ�
			if (m_bStoneID[0][1])
			{
				// ��ΐ�
				SetUI(D3DXVECTOR3(200, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, NORMAL_COLOR);
			}

			// 1P���΂�������Ƃ�
			if (m_bStoneID[0][2])
			{
				// ��Η�
				SetUI(D3DXVECTOR3(275, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, NORMAL_COLOR);
			}
		}

		if (!m_bTransform[1])
		{
			// ��Δw�i2P
			SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), 210, STONE_SIZE_Y, LOGOTYPE_JEWELRYBG2P, NORMAL_COLOR);

			// 2P���Ԑ΂�������Ƃ�
			if (m_bStoneID[1][0])
			{
				// ��ΐ�
				SetUI(D3DXVECTOR3(1005, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYRED, NORMAL_COLOR);
			}

			// 2P���ΐ΂�������Ƃ�
			if (m_bStoneID[1][1])
			{
				// ��ΐ�
				SetUI(D3DXVECTOR3(1080, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYGREEN, NORMAL_COLOR);
			}

			// 2P���΂�������Ƃ�
			if (m_bStoneID[1][2])
			{
				// ��Η�
				SetUI(D3DXVECTOR3(1155, STONE_POS_Y, 0), STONE_SIZE_X, STONE_SIZE_Y, LOGOTYPE_JEWELRYBULE, NORMAL_COLOR);
			}

		}

		// 1P�L�����N�^�[UI
		SetUI(D3DXVECTOR3(55, 70, 0), 110, 130, LOGOTYPE_PLAYER1, NORMAL_COLOR);
		// �e�N�X�`���ݒ�
		m_pScene2D[LOGOTYPE_PLAYER1]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[0] / 2);
		// 2P�L�����N�^�[UI
		SetUI(D3DXVECTOR3(1225, 70, 0), 110, 130, LOGOTYPE_PLAYER2, NORMAL_COLOR);
		// �e�N�X�`���ݒ�
		m_pScene2D[LOGOTYPE_PLAYER2]->SetAnimation(0.25f, 1.0f, 0.0f, m_nCharaNum[1] / 2);

		// 1P�L�����N�^�[�l�[��UI
		SetUI(D3DXVECTOR3(205, 155, 0), 400, 50, LOGOTYPE_CHARANAME0, NORMAL_COLOR);
		// �e�N�X�`���ݒ�
		m_pScene2D[LOGOTYPE_CHARANAME0]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[0] / 2) * 0.25f, 0.0f + (m_nCharaNum[0] / 2) * 0.25f, 0);
		// 2P�L�����N�^�[�l�[��UI
		SetUI(D3DXVECTOR3(1075, 155, 0), 400, 50, LOGOTYPE_CHARANAME1, NORMAL_COLOR);
		// �e�N�X�`���ݒ�
		m_pScene2D[LOGOTYPE_CHARANAME1]->SetAnimation(1.0f, 0.25f + (m_nCharaNum[1] / 2) * 0.25f, 0.0f + (m_nCharaNum[1] / 2) * 0.25f, 0);
	}
}

//==================================================================================================================
// �Q�[���p�b�h����
//==================================================================================================================
void CUI::ControlGamepad(CInputGamepad * pGamepad0, CInputGamepad *pGamepad1)
{
	float fValueX0, fValueY0 = 0;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p
	float fValueX1, fValueY1 = 0;	// �Q�[���p�b�h�̃X�e�B�b�N���̎擾�p

	// ���X�e�B�b�N�擾
	pGamepad0->GetStickLeft(&fValueX0, &fValueY0);
	pGamepad1->GetStickLeft(&fValueX1, &fValueY1);

	//// �������͂���Ă��Ȃ���΁A�������Ȃ�
	//if (FAILED(CKananLibrary::GetMoveByGamepad(pGamepad0)) &&
	//	fValueX0 == 0 && fValueY0 == 0 &&
	//	FAILED(CKananLibrary::GetMoveByGamepad(pGamepad1)) &&
	//	fValueX1 == 0 && fValueY1 == 0)
	//{
	//	return;
	//}

	// �^�C�g���̂Ƃ�
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
	{
		// ���ɌX�����Ƃ�
		if (fValueX0 < 0 || fValueX1 < 0)
		{
			// ���[�h0
			m_nMode = 0;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (fValueX0 > 0 || fValueX1 > 0)
		{// �E�ɌX�����Ƃ�
			// ���[�h1
			m_nMode = 1;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{// �`���[�g���A���̂Ƃ�
		// �����̃L�����N�^�[��I�����ĂȂ��Ƃ�
		if (!m_bCharaDecide[0])
		{
			// �X�e�B�b�N��߂��Ă���Ƃ�
			if (m_bStickReturn[0])
			{
				// ���ɌX�����Ƃ�
				if (fValueX0 < 0)
				{
					// �L�����ԍ����Z
					m_nCharaNum[0] -= 1;

					// ���v�̐j�������Ă�����Ԃɂ���
					m_bUIClockHands[0] = true;

					// �ʒu�ړ��p�J�E���^������
					m_fPosMove[0] = 0;

					// �X�e�B�b�N��߂������ǂ���
					m_bStickReturn[0] = false;

					// ���ʉ��Đ�
					CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
				}
				else if (fValueX0 > 0)
				{// �E�ɌX�����Ƃ�
					// �L�����ԍ����Z
					m_nCharaNum[0] += 1;

					// ���v�̐j�������Ă�����Ԃɂ���
					m_bUIClockHands[0] = true;

					// �ʒu�ړ��p�J�E���^������
					m_fPosMove[0] = 0;

					// �X�e�B�b�N��߂������ǂ���
					m_bStickReturn[0] = false;

					// ���ʉ��Đ�
					CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
				}

				// ����{�^�����������Ƃ�
				if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_B))
				{
					// �L�����N�^�[��I��������Ԃɂ���
					m_bCharaDecide[0] = true;
					// ���ʉ��Đ�
					CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
				}
			}

			// �X�e�B�b�N�p�x0�̂Ƃ�
			if (fValueX0 == 0)
			{
				// �X�e�B�b�N��߂��Ă����Ԃɂ���
				m_bStickReturn[0] = true;
			}
		}
		else
		{// 1P�������̃L�����N�^�[���I������Ă���Ƃ�
			// �߂�{�^�����������Ƃ�
			if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_A))
			{
				// �L�����N�^�[��I�����Ă��Ȃ���Ԃɂ���
				m_bCharaDecide[0] = false;
			}
		}


		// 2P�̃L�����N�^�[���I�΂�Ă��Ȃ��Ƃ�
		if (!m_bCharaDecide[1])
		{
			// �X�e�B�b�N��߂��Ă���Ƃ�
			if (m_bStickReturn[1])
			{
				// ���ɌX�����Ƃ�
				if (fValueX1 < 0)
				{
					// �L�����ԍ����Z
					m_nCharaNum[1] -= 1;

					// ���v�̐j�������Ă�����Ԃɂ���
					m_bUIClockHands[1] = true;

					// �ʒu�ړ��p�J�E���^������
					m_fPosMove[1] = 0;

					// �X�e�B�b�N���߂��Ă��Ȃ���Ԃɂ���
					m_bStickReturn[1] = false;

					// ���ʉ��Đ�
					CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
				}
				else if (fValueX1 > 0)
				{
					// �L�����ԍ����Z
					m_nCharaNum[1] += 1;

					// ���v�̐j�������Ă�����Ԃɂ���
					m_bUIClockHands[1] = true;

					// �ʒu�ړ��p�J�E���^������
					m_fPosMove[1] = 0;

					// �X�e�B�b�N���߂��Ă��Ȃ���Ԃɂ���
					m_bStickReturn[1] = false;

					// ���ʉ��Đ�
					CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
				}

				// ����{�^�����������Ƃ�
				if (pGamepad1->GetTrigger(CInputGamepad::JOYPADKEY_B))
				{
					// �L�����N�^�[��I��������Ԃɂ���
					m_bCharaDecide[1] = true;
					// ���ʉ��Đ�
					CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
				}
			}

			// �X�e�B�b�N���X���Ă��Ȃ��Ƃ�
			if (fValueX1 == 0)
			{
				// �X�e�B�b�N���߂��Ă����Ԃɂ���
				m_bStickReturn[1] = true;
			}
		}
		else
		{// 2P�������̃L�����N�^�[��I�����Ă���Ƃ�
			// �߂�{�^�����������Ƃ�
			if (pGamepad1->GetTrigger(CInputGamepad::JOYPADKEY_A))
			{
				// �L�����N�^�[��I�����Ă��Ȃ���Ԃɂ���
				m_bCharaDecide[1] = false;
			}
		}
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
	{// �}�b�v�I����ʂ̂Ƃ�
		// �}�b�v���I������Ă��Ȃ��Ƃ�
		if (!m_bMapSelect)
		{
			// ���ɌX�����Ƃ�
			if (fValueX0 < 0)
			{
				// �}�b�v�ԍ�1���Z
				m_nMapID -= 1;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (fValueX0 > 0)
			{// �E�ɌX�����Ƃ�
				// �}�b�v�ԍ�1���Z
				m_nMode += 1;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// ��ɌX�����Ƃ�
			if (fValueY0 < 0)
			{
				// �}�b�v�ԍ�-2
				m_nMapID -= 2;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (fValueY0 > 0)
			{// ���ɌX�����Ƃ�
				// �}�b�v�ԍ�+2
				m_nMapID += 2;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// ����{�^�����������Ƃ�
			if (pGamepad0->GetTrigger(CInputGamepad::JOYPADKEY_B))
			{
				// �}�b�v��I��������Ԃɂ���
				m_bMapSelect = true;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
			}
		}
	}
}
//==================================================================================================================
// �L�[�{�[�h����
//==================================================================================================================
void CUI::ControlKeyboard(CInputKeyboard * pKeyboard)
{
	// �^�C�g���̂Ƃ�
	if (CRenderer::GetMode() == CRenderer::MODE_TITLE)
	{
		// �L�[�{�[�h��[��]���������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(DIK_RIGHTARROW))
		{
			m_nMode = 1;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_LEFTARROW))
		{// �L�[�{�[�h��[��]���������Ƃ�
			m_nMode = 0;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}

		// �L�[�{�[�h��[D]���������Ƃ�
		if (pKeyboard->GetKeyboardTrigger(DIK_D))
		{
			m_nMode = 1;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
		else if (pKeyboard->GetKeyboardTrigger(DIK_A))
		{// �L�[�{�[�h��[A]���������Ƃ�
			m_nMode = 0;
			// ���ʉ��Đ�
			CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
		}
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_TUTORIAL)
	{// �`���[�g���A���̂Ƃ�

		// �����̃L�����N�^�[��I�����ĂȂ��Ƃ�
		if (!m_bCharaDecide[0])
		{
			// �L�[�{�[�h��[A]�������ꂽ�Ƃ�
			if (pKeyboard->GetKeyboardTrigger(ONE_LEFT))
			{
				// �L�����ԍ����Z
				m_nCharaNum[0] -= 1;

				// ���v�̐j�������Ă�����Ԃɂ���
				m_bUIClockHands[0] = true;

				// �ʒu�ړ��p�J�E���^������
				m_fPosMove[0] = 0;

				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (pKeyboard->GetKeyboardTrigger(ONE_RIGHT))
			{// �L�[�{�[�h��[D]�������ꂽ�Ƃ�
			 // �L�����ԍ����Z
				m_nCharaNum[0] += 1;

				// ���v�̐j�������Ă�����Ԃɂ���
				m_bUIClockHands[0] = true;

				// �ʒu�ړ��p�J�E���^������
				m_fPosMove[0] = 0;

				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// 1P������{�^�����������Ƃ�
			if (pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
			{
				// �L�����N�^�[��I��������Ԃɂ���
				m_bCharaDecide[0] = true;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
			}
		}
		else
		{// 1P�������̃L�����N�^�[��I�����Ă���Ƃ�
			// 1P������{�^�����������Ƃ�
			if (pKeyboard->GetKeyboardTrigger(ONE_JUMP))
			{
				// �L�����N�^�[��I�����Ă��Ȃ���Ԃɂ���
				m_bCharaDecide[0] = false;
			}
		}

		// 2P�̃L�����N�^�[���I�΂�Ă��Ȃ��Ƃ�
		if (!m_bCharaDecide[1])
		{
			// �L�[�{�[�h��[��]�������ꂽ�Ƃ�
			if (pKeyboard->GetKeyboardTrigger(TWO_LEFT))
			{
				// �L�����ԍ����Z
				m_nCharaNum[1] -= 1;

				// ���v�̐j�������Ă�����Ԃɂ���
				m_bUIClockHands[1] = true;

				// �ʒu�ړ��p�J�E���^������
				m_fPosMove[1] = 0;

				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (pKeyboard->GetKeyboardTrigger(TWO_RIGHT))
			{// �L�[�{�[�h��[��]�������ꂽ�Ƃ�
			 // �L�����ԍ����Z
				m_nCharaNum[1] += 1;

				// ���v�̐j�������Ă�����Ԃɂ���
				m_bUIClockHands[1] = true;

				// �ʒu�ړ��p�J�E���^������
				m_fPosMove[1] = 0;

				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// 2P������{�^�����������Ƃ�
			if (pKeyboard->GetKeyboardTrigger(TWO_ATTACK))
			{
				// �L�����N�^�[��I�񂾏�Ԃɂ���
				m_bCharaDecide[1] = true;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
			}
		}
		else
		{// 2P�������̃L�����N�^�[��I�����Ă���Ƃ�
			// 2P������{�^�����������Ƃ�
			if (pKeyboard->GetKeyboardTrigger(TWO_JUMP))
			{
				// �L�����N�^�[��I�����Ă��Ȃ���Ԃɂ���
				m_bCharaDecide[1] = false;
			}
		}
	}
	else if (CRenderer::GetMode() == CRenderer::MODE_MAPSELECT)
	{// �}�b�v�I����ʂ̂Ƃ�

		// �}�b�v���I������Ă��Ȃ��Ƃ�
		if (!m_bMapSelect)
		{
			// 1P�������͂������Ƃ�
			if (pKeyboard->GetKeyboardTrigger(ONE_LEFT))
			{
				// �}�b�vID�������Ȃ�
				m_nMapID -= 1;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (pKeyboard->GetKeyboardTrigger(ONE_RIGHT))
			{// 1P���E���͂������Ƃ�
				// �}�b�vID�傫���Ȃ�
				m_nMapID += 1;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (pKeyboard->GetKeyboardTrigger(ONE_UP))
			{// 1P������͂������Ƃ�
				// �}�b�vID-2����
				m_nMapID -= 2;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}
			else if (pKeyboard->GetKeyboardTrigger(ONE_DOWN))
			{// 1P���������͂������Ƃ�
				// �܂���ID+2����
				m_nMapID += 2;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CURSORMOVE);
			}

			// 1P������{�^�����������Ƃ�
			if (pKeyboard->GetKeyboardTrigger(ONE_ATTACK))
			{
				// �}�b�v��I�񂾏�Ԃɂ���
				m_bMapSelect = true;
				// ���ʉ��Đ�
				CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DECISION);
			}
		}
	}
}

//==================================================================================================================
// ���S�쐬
//==================================================================================================================
void CUI::SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col)
{
	m_pScene2D[nCnt]->SetPos(pos);									// �ʒu�ݒ�
	m_pScene2D[nCnt]->SetSize(D3DXVECTOR3(fSizeX, fSizeY, 0.0f));	// �傫���ݒ�
	m_pScene2D[nCnt]->SetCol(col);									// �F�ݒ�
}
