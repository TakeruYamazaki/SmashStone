//==================================================================================================================
//
// Game�̕`�� [game.cpp]
// Author : Seiya Takahashi
//
//==================================================================================================================

//==================================================================================================================
//	�C���N���[�h�t�@�C��
//==================================================================================================================
#include "game.h"
#include "number.h"
#include "fade.h"
#include "inputKeyboard.h"
#include "meshField.h"
#include "scene3D.h"
#include "camera.h"
#include "light.h"
#include "time.h"
#include "renderer.h"
#include "motionModel.h"
#include "pause.h"
#include "inputGamepad.h"
#include "meshSphere.h"
#include "player.h"
#include "modelCharacter.h"
#include "stone.h"
#include "3DBoxCollider.h"
#include "hitpoint.h"
#include "bar.h"
#include "wall.h"
#include "debugProc.h"
#include "objManager.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include "UI_KO.h"
#include "UI_gameStart.h"
#include "UI_gameResult.h"
#include "UI.h"
#include "3DEffect.h"
#include "CharEffectOffset.h"
#include "3DParticle.h"
#include "sound.h"

//==================================================================================================================
//	�}�N����`
//==================================================================================================================
#define MAX_ROUND			(3)										// �ő僉�E���h��
#define PLAYER_START_POS_X 390.0f									// �v���C���[�̏����ʒuX
#define PLAYER_START_POS_Z -585.0f									// �v���C���[�̏����ʒuZ
#define RESPAWN_SIZE 0.000001f										// ���X�|�[�����f���̑傫��

#define TIME_CREATE_STONE	(5 * ONE_SECOND_FPS)					// �X�g�[���𐶐����鎞��
#define TIME_KO_AFTER		(60)									// KO�̌�̎���

#define TIME_FADE_NEXTROUND	(10)									// ���E���h�؂�ւ��̃t�F�[�h����

#define DEFAULTPOS_1P		(D3DXVECTOR3(-160.0f, 0.0f, 100.0f))	// 1P�v���C���[�̏������W
#define DEFAULTPOS_2P		(D3DXVECTOR3(20.0f, 0.0f, -130.0f))		// 2P�v���C���[�̏������W

//==================================================================================================================
//	�ÓI�����o�ϐ��錾
//==================================================================================================================
CPlayer				*CGame::m_pPlayer[MAX_PLAYER]	= {};							// �L�����N�^�[���
CMeshField			*CGame::m_pMeshField			= NULL;							// ���b�V���t�B�[���h���
CCamera				*CGame::m_pCamera				= NULL;							// �J�������
CLight				*CGame::m_pLight				= NULL;							// ���C�g���
CLogo				*CGame::m_pLogo					= NULL;							// ���S���
CPause				*CGame::m_pPause				= NULL;							// �|�[�Y���
CMeshSphere			*CGame::m_pMeshSphere			= NULL;							// ���b�V�����̏��
CTime				*CGame::m_pTime					= NULL;							// �^�C�����
CWall				*CGame::m_pWall					= NULL;							// �ǂ̃|�C���^
CUI					*CGame::m_pUI					= NULL;							// UI�|�C���^
CUIKO				*CGame::m_pUIKO					= nullptr;						// KO�̃|�C���^
CUI_GameStart		*CGame::m_pUIGameStart			= nullptr;						// �Q�[���J�n����UI�̃|�C���^
CUI_GameResult		*CGame::m_pUIGameResult			= nullptr;						// �Q�[�����U���g��UI�̃|�C���^
CGame::GAMESTATE	CGame::m_gameState				= CGame::GAMESTATE_NONE;		// �Q�[�����
int					CGame::m_nCounterGameState		= NULL;							// �Q�[���̏�ԊǗ��J�E���^�[
int					CGame::m_nNumStone				= 0;							// ���������X�g�[���̐�
int					CGame::m_nCntDecide				= 0;							// �X�g�[�������̃^�C�~���O�����߂�J�E���^
int					CGame::m_nRound					= 0;							// ���E���h��
int					CGame::m_nRoundAll				= 0;							// �S���E���h��
NUM_PLAYER			CGame::m_winPlayer				= NUM_PLAYER::PLAYER_NONE;		// ���������v���C���[
NUM_PLAYER			CGame::m_losePlayer				= NUM_PLAYER::PLAYER_NONE;		// �������v���C���[
CObjectManager		*CGame::m_pObjMana				= nullptr;						// �I�u�W�F�N�g�}�l�[�W���[�̃|�C���^
bool				CGame::m_bSetPos[STONE_POS]		= {};							// �X�g�[���̐����ꏊ�ɐ�������Ă��邩
bool				CGame::m_bGetType[CStone::STONE_ID_MAX] = {};					// �X�g�[���̐����ꏊ�ɐ�������Ă��邩
int					CGame::m_nStageType				= 0;							// �X�e�[�W�̃^�C�v

int					CGame::m_nPlayerType[MAX_PLAYER] = {};	// �L�����N�^�[�Z���N�g���̃^�C�v��ۑ�

CPolygonCollider* CGame::m_pPolyColli[POLYCOLLI_USE_TYPE] = { NULL };							// �|���S���R���C�_�[�̃|�C���^

//==================================================================================================================
//	�R���X�g���N�^
//==================================================================================================================
CGame::CGame()
{

}

//==================================================================================================================
//	�f�X�g���N�^
//==================================================================================================================
CGame::~CGame()
{

}

//==================================================================================================================
//	����������
//==================================================================================================================
void CGame::Init(void)
{
	/* ���[�h */
	CNumber::Load();						// �����e�N�X�`�����[�h
	CMeshField::Load();						// ���e�N�X�`�����[�h
	CMotionModel::Load();					// ���[�V�����p���f�����[�h
	CPause::Load();							// �|�[�Y�e�N�X�`�����[�h
	CMeshSphere::Load();					// ���b�V�����̃e�N�X�`�����[�h
	CStone::Load();							// �X�g�[���̓ǂݍ���
	CBar::Load();							// Bar�e�N�X�`�����[�h
	C3DBoxCollider::Load();					// 3D�{�b�N�X�R���C�_�[�̓ǂݍ���
	CWall::Load();							// �ǂ̃��[�h
	CObjectManager::Load();					// �I�u�W�F�N�g�}�l�[�W���[�̃��[�h
	CCapsuleCollider::Load();				// �J�v�Z���R���C�_�[�̃��[�h
	CUIKO::Load();							// KO�̃��[�h
	CUI_GameStart::Load();					// �J�nUI�̃��[�h
	CUI_GameResult::Load();
	CUI::Load();							// UI���[�h

	// 3D�G�t�F�N�g�̍쐬
	C3DEffect *p3DEffect;
	// ����
	p3DEffect = new C3DEffect;
	// �쐬
	p3DEffect->Make();

	/* ���� */
	C3DBoxCollider::Create();										// �{�b�N�X�R���C�_�[�̐���
	m_pObjMana    = CObjectManager::Create((CObjectManager::STAGETYPE)m_nStageType);// �I�u�W�F�N�g�}�l�[�W���[�̐���
	m_pWall       = CWall::Create(CWall::WALLTEX_FIELD);			// �ǂ̐���
	m_pCamera     = CCamera::Create();								// �J�����̐�������
	m_pLight      = CLight::Create();								// ���C�g�̐�������
	m_pMeshSphere = CMeshSphere::Create();							// ���b�V�����̐�������
	m_pPlayer[PLAYER_ONE] = CPlayer::Create(PLAYER_ONE, (CHARACTER_TYPE)m_nPlayerType[PLAYER_ONE]);	// �v���C���[����
	m_pPlayer[PLAYER_TWO] = CPlayer::Create(PLAYER_TWO, (CHARACTER_TYPE)m_nPlayerType[PLAYER_TWO]);	// �v���C���[����
	m_pPlayer[PLAYER_ONE]->SetPos(DEFAULTPOS_1P);
	m_pPlayer[PLAYER_TWO]->SetPos(DEFAULTPOS_2P);

	m_pMeshField  = CMeshField::Create(INTEGER2(4, 4), D3DXVECTOR3(600.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, -40.0f, 50.0f));// ���b�V���t�B�[���h����
	m_pUI         = CUI::Create();									// UI�̐�������
	m_pTime       = CTime::Create();								// �^�C������
	m_pPause      = CPause::Create();								// �|�[�Y�̐�������

	// �ɂ₩�ȊK�i
	m_pPolyColli[CPolygonCollider::POLYCOLLI_LONGSTAIRS] = CPolygonCollider::Create(CPolygonCollider::POLYCOLLI_LONGSTAIRS);
	// �K�i
	m_pPolyColli[CPolygonCollider::POLYCOLLI_STAIRS] = CPolygonCollider::Create(CPolygonCollider::POLYCOLLI_STAIRS);
	m_pPolyColli[CPolygonCollider::POLYCOLLI_STAIRS_TOP] = CPolygonCollider::Create(CPolygonCollider::POLYCOLLI_STAIRS_TOP);

	/* �Q�[���̏����� */
	m_gameState = GAMESTATE_BEFORE;
	m_nCounterGameState = 0;				// �Q�[���̏�ԊǗ��J�E���^�[��0�ɂ���
	m_nNumStone			= 0;				// �l��������
	m_nCntDecide		= 0;				// �l��������
	m_nCntAny			= 0;
	m_nRound			= 0;
	m_nRoundAll			= MAX_ROUND;
	m_roundPoint		= INTEGER2(0, 0);

	for (int nCnt = 0; nCnt < STONE_POS; nCnt++)
	{
		m_bSetPos[nCnt] = false;
	}

	for (int nCnt = 0; nCnt < CStone::STONE_ID_MAX; nCnt++)
	{
		m_bGetType[nCnt] = false;
	}
}

//==================================================================================================================
//	�I������
//==================================================================================================================
void CGame::Uninit(void)
{
	// �Q�[����Ԃ�ʏ�ɂ���
	m_gameState = GAMESTATE_NORMAL;

	// �S�Ă̊J��
	CScene2D::ReleaseAll();				// 2D�̂��̑S�Ĕj��
	CScene3D::ReleaseAll();				// 3D�̂��̑S�Ĕj��

	/* �A�����[�h */
	CCapsuleCollider::Unload();
	CWall::Unload();					// �ǂ̊J��
	C3DBoxCollider::Unload();			// 3D�{�b�N�X�R���C�_�[�̊J��
	CStone::Unload();					// �X�g�[���̊J��
	CNumber::Unload();					// �����e�N�X�`���A�����[�h
	CMeshField::Unload();				// ���e�N�X�`���A�����[�h
	CMotionModel::Unload();				// ���[�V�����p���f���A�����[�h
	CBar::Unload();						// Bar�e�N�X�`���A�����[�h
	CObjectManager::Unload();			// �I�u�W�F�N�g�}�l�[�W���[�̃A�����[�h
	CUIKO::Unload();					// KO�̃A�����[�h
	CUI_GameStart::Unload();			// �J�n����UI�̃A�����[�h
	CUI_GameResult::Unload();
	CUI::Unload();						// UI�A�����[�h

	// ������c���Ă����ꍇ
	if (m_pUIGameStart)
	{
		// �j��
		m_pUIGameStart->Uninit();
		delete m_pUIGameStart;
		m_pUIGameStart = nullptr;
	}

	// ������c���Ă����ꍇ
	if (m_pUIGameResult)
	{
		// �j��
		m_pUIGameResult->Uninit();
		delete m_pUIGameResult;
		m_pUIGameResult = nullptr;
	}

	// ������c���Ă����ꍇ
	if (m_pUIKO)
	{
		// �j��
		m_pUIKO->Uninit();
		delete m_pUIKO;
		m_pUIKO = nullptr;
	}

	// �|�[�Y�̏I������
	m_pPause->Uninit();

	m_pObjMana = nullptr;				// �|�C���^NULL

	delete m_pPause;					// �������폜
	m_pPause = nullptr;					// �|�C���^NULL

	delete m_pCamera;					// �������폜
	m_pCamera = nullptr;				// �|�C���^NULL

	delete m_pLight;					// �������폜
	m_pLight = nullptr;					// �|�C���^NULL

	m_pTime = nullptr;					// �|�C���^NULL
}

//==================================================================================================================
//	�X�V����
//==================================================================================================================
void CGame::Update(void)
{
	// �J�n�O
	if (m_gameState == GAMESTATE_BEFORE)
		GameBefore();
	// �ʏ�̂Ƃ�
	if (m_gameState == GAMESTATE_NORMAL)
		GameNormal();
	// �Q�[����Ԃ��|�[�Y�̂Ƃ�
	else if (m_gameState == GAMESTATE_PAUSE)
		GamePause();
	// KO�̂Ƃ�
	else if (m_gameState == GAMESTATE_KO)
		GameKO();
	// KO�̌�
	else if (m_gameState == GAMESTATE_KO_AFTER)
		GameKOAfter();
	// ���̃��E���h
	else if (m_gameState == GAMESTATE_NEXTROUND)
		NextRound();
	// ���U���g
	else if (m_gameState == GAMESTATE_RESULT)
		GameResult();

#ifdef _DEBUG
	// �L�[�{�[�h��[0]���������Ƃ�
	if (m_gameState != GAMESTATE_RESULT && CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_RETURN) && CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LSHIFT))
	{
		// �t�F�[�h�擾
		CFade::FADE fade = CFade::GetFade();

		// �t�F�[�h�������Ȃ���
		if (fade == CFade::FADE_NONE)
			// �t�F�[�h��ݒ肷��
			CFade::SetFade(CRenderer::MODE_TITLE, DEFAULT_FADE_TIME);
	}

	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_LSHIFT))
	{
		C3DParticle::Set(&m_pPlayer[PLAYER_ONE]->GetPos(), &m_pPlayer[PLAYER_ONE]->GetRot(), C3DParticle::HIT);
	}
#endif // _DEBUG
}

//==================================================================================================================
//	�`�揈��
//==================================================================================================================
void CGame::Draw(void)
{
	// �J�����̐ݒ�
	m_pCamera->SetCamera();

	// �J�����̕`�揈��
	m_pCamera->Draw();

	// �|�[�Y��Ԃ�true�̂Ƃ�
	if (m_pPause->GetPause() == true)
		// �|�[�Y�̍X�V����
		m_pPause->Draw();

	// �Q�[���J�n����UI�̕`��
	if (m_pUIGameStart)
		m_pUIGameStart->Draw();

	// KO�̕`��
	if (m_pUIKO)
		m_pUIKO->Draw();

	// ���U���g��UI�̕`��
	if (m_pUIGameResult)
		m_pUIGameResult->Draw();
}

//==================================================================================================================
//	��������
//==================================================================================================================
CGame * CGame::Create(void)
{
	// �������m��
	CGame *pGame = new CGame;
	// ������
	pGame->Init();
	// �l��Ԃ�
	return pGame;
}

//==================================================================================================================
//	���̃��[�h�ֈڍs
//==================================================================================================================
void CGame::SetNextMode(const int nextMode)
{
	// nullcheck
	if (m_pUIGameResult)
	{
		// �j��
		m_pUIGameResult->Uninit();
		delete m_pUIGameResult;
		m_pUIGameResult = nullptr;
	}

	m_gameState = GAMESTATE_END;

	// �t�F�[�h�擾
	CFade::FADE fade = CFade::GetFade();

	// �t�F�[�h�������Ȃ���
	if (fade == CFade::FADE_NONE)
		// �t�F�[�h��ݒ肷��
		CFade::SetFade((CRenderer::MODE)nextMode, DEFAULT_FADE_TIME);
}

//==================================================================================================================
//	�ǂ�����ł��Ăяo����X�g�[����������
//==================================================================================================================
void CGame::AppearStone(void)
{
	// �����_���Ń|�C���g�����߂�
	int RandPos = CKananLibrary::DecideRandomValue(STONE_POS, m_bSetPos);
	// �����_���Ń|�C���g�����߂�
	int RandType = CKananLibrary::DecideRandomValue(CStone::STONE_ID_MAX, m_bGetType);
	// ���߂�ꂽ�ʒu���烉���_���Ő���
	CStone::Create(RandPos, (CStone::STONE_ID)RandType, CObjectManager::GetDefaultStonePos(RandPos));
	// �������ꂽ
	m_bSetPos[RandPos] = true;
	// �������ꂽ
	m_bGetType[RandType] = true;
	// �o���������Z
	m_nNumStone++;
}

//==================================================================================================================
//	�Q�[���̑O�̍X�V
//==================================================================================================================
void CGame::GameBefore(void)
{
	if (!m_pUIGameStart)
		m_pUIGameStart = CUI_GameStart::Create();

	// �J�����̍X�V����
	m_pCamera->Update();

	// ���C�g�̍X�V����
	m_pLight->Update();

	if (m_pUIGameStart)
		m_pUIGameStart->Update();
}

//==================================================================================================================
//	�ʏ�̍X�V
//==================================================================================================================
void CGame::GameNormal(void)
{
	// nullcheck
	if (m_pUIGameStart)
	{
		// �j��
		m_pUIGameStart->Uninit();
		delete m_pUIGameStart;
		m_pUIGameStart = nullptr;
	}

	// �J�����̍X�V����
	m_pCamera->Update();

	// ���C�g�̍X�V����
	m_pLight->Update();

	// �X�g�[���𐶐����邩���߂�
	DecideCreateStone();

	// �ǂ��炩�̃v���C���[�̃��C�t��0
	if (GetPlayer(PLAYER_ONE)->GetLife() <= 0 ||
		GetPlayer(PLAYER_TWO)->GetLife() <= 0)
	{
		// KO
		m_gameState = GAMESTATE_KO;
		// ���ʉ����Đ�
		CRenderer::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FINISH);
	}

	// �|�[�Y�̐؂�ւ�
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_P))
		SwitchPause();
}

//==================================================================================================================
//	�|�[�Y�̍X�V
//==================================================================================================================
void CGame::GamePause(void)
{
	// �|�[�Y�̍X�V����
	m_pPause->Update();

	// ���g���C
	if (m_gameState == GAMESTATE_START_OVER)
		CFade::SetFade(CRenderer::MODE_GAME, DEFAULT_FADE_TIME);
	// �^�C�g���ɖ߂�
	else if (m_gameState == GAMESTATE_BREAK)
		CFade::SetFade(CRenderer::MODE_TITLE, DEFAULT_FADE_TIME);
}

//==================================================================================================================
//	KO�̍X�V
//==================================================================================================================
void CGame::GameKO(void)
{
	// KO��UI�𐶐�
	if (!m_pUIKO)
		m_pUIKO = CUIKO::Create();

	// KO�̍X�V
	if (m_pUIKO)
		m_pUIKO->Update();
}

//==================================================================================================================
//	KO�̌�̍X�V
//==================================================================================================================
void CGame::GameKOAfter(void)
{
	// �J�E���^�̉��Z
	m_nCntAny++;
	// ��莞�ԂŎ���
	if (m_nCntAny >= TIME_KO_AFTER &&
		!m_pPlayer[PLAYER_ONE]->GetbJump() && !m_pPlayer[PLAYER_TWO]->GetbJump())
	{
		m_nCntAny = 0;
		m_gameState = GAMESTATE_NEXTROUND;
		return;
	}

	// �J�����̍X�V����
	m_pCamera->Update();
	// ���C�g�̍X�V����
	m_pLight->Update();
}

//==================================================================================================================
//	�|�[�Y�̐؂�ւ�
//==================================================================================================================
void CGame::SwitchPause(void)
{
	// �Q�[����Ԃ��|�[�Y�̂Ƃ�
	if (m_gameState == GAMESTATE_PAUSE)
	{
		// �Q�[����Ԃ�NORMAL�ɂ���
		m_gameState = GAMESTATE_NORMAL;
		// �|�[�Y��Ԃ̐ݒ�
		m_pPause->SetPause(false);
	}
	else
	{// �Q�[����Ԃ��|�[�Y����Ȃ��Ƃ�
	 // �Q�[����Ԃ��|�[�Y�ɂ���
		m_gameState = GAMESTATE_PAUSE;
		// �|�[�Y��Ԃ̐ݒ�
		m_pPause->SetPause(true);
	}
}

//==================================================================================================================
//	���̃��E���h��
//==================================================================================================================
void CGame::NextRound(void)
{
	// nullcheck
	if (m_pUIKO)
	{
		// �j��
		m_pUIKO->Uninit();
		delete m_pUIKO;
		m_pUIKO = nullptr;
	}

	// ���E���h�������Z
	m_nRound++;
	// �v���C���[�̃��E���h�|�C���g�����Z
	if (GetPlayer(PLAYER_ONE)->GetLife() <= 0 &&
		GetPlayer(PLAYER_TWO)->GetLife() <= 0)
		m_nRound--;
	else if (GetPlayer(PLAYER_ONE)->GetLife() > 0)
		m_roundPoint.nX++;
	else if (GetPlayer(PLAYER_TWO)->GetLife() > 0)
		m_roundPoint.nY++;

	// �ǂ��炩���ő�܂œ��_������I��
	if (m_roundPoint.nX == (int)((float)m_nRoundAll / 2.0f + 0.5f) ||
		m_roundPoint.nY == (int)((float)m_nRoundAll / 2.0f + 0.5f))
	{
		// ���҂����߂�
		if (m_roundPoint.nX > m_roundPoint.nY)
		{
			m_winPlayer = PLAYER_ONE;
			m_losePlayer = PLAYER_TWO;
		}
		else
		{
			m_winPlayer = PLAYER_TWO;
			m_losePlayer = PLAYER_ONE;
		}		// ���U���g��
		m_gameState = GAMESTATE_RESULT;
	}
	// ����ȊO�͍ăo�g��
	else
	{
		// �t�F�[�h�擾
		CFade *pFade = CManager::GetRenderer()->GetFade();
		// �t�F�[�h���Ԃ�ݒ肵�A��̃t�F�[�h���s
		pFade->SetFade(CRenderer::MODE_NONE, TIME_FADE_NEXTROUND);
		// �o�g���O��
		m_gameState = GAMESTATE_BEFORE;
		m_pPlayer[PLAYER_ONE]->SetPos(DEFAULTPOS_1P);
		m_pPlayer[PLAYER_TWO]->SetPos(DEFAULTPOS_1P);
		m_pPlayer[PLAYER_ONE]->SetLife(m_pPlayer[PLAYER_ONE]->GetMaxLife());
		m_pPlayer[PLAYER_TWO]->SetLife(m_pPlayer[PLAYER_TWO]->GetMaxLife());
	}
}

//==================================================================================================================
//	�Q�[���̃��U���g
//==================================================================================================================
void CGame::GameResult(void)
{
	// ����
	if (!m_pUIGameResult)
		m_pUIGameResult = CUI_GameResult::Create();

	// �������̍s��
	m_pPlayer[m_winPlayer]->VictoryAction();

	// �X�V
	if (m_pUIGameResult)
		m_pUIGameResult->Update();
}

//==================================================================================================================
//	�X�g�[���𐶐����邩���߂�
//==================================================================================================================
void CGame::DecideCreateStone(void)
{
	// �J�E���^�����Z
	m_nCntDecide++;

#ifdef _DEBUG
	// �������E�o�����̍��v��3����
	if (CManager::GetInputKeyboard()->GetKeyboardTrigger(DIK_1) && CManager::GetInputKeyboard()->GetKeyboardPress(DIK_LSHIFT) && 
		m_nNumStone + GetPlayer(PLAYER_ONE)->GetNumStone() + GetPlayer(PLAYER_TWO)->GetNumStone() < 3)
		// �X�g�[������
		AppearStone();
#endif

	// ���Ԉȓ�
	if (m_nCntDecide <= TIME_CREATE_STONE)
		// �������I����
		return;

	// �������E�o�����̍��v��3����
	if (m_nNumStone + GetPlayer(PLAYER_ONE)->GetNumStone() + GetPlayer(PLAYER_TWO)->GetNumStone() < 3)
		// �X�g�[������
		AppearStone();

	// �J�E���^��������
	m_nCntDecide = 0;
}
