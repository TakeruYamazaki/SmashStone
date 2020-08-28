//==================================================================================================================
//
// UI[UI.h]
// Author : Seiya Takahashi
//
//==================================================================================================================
#ifndef _UI_H_
#define _UI_H_

//==================================================================================================================
// �C���N���[�h�t�@�C��
//==================================================================================================================
#include "main.h"
#include "scene.h"
#include "stone.h"

//==================================================================================================================
// �O���錾
//==================================================================================================================
class CScene2D;
class CInputKeyboard;
class CInputGamepad;

//==================================================================================================================
//
// ���S�N���X�̒�`
//
//==================================================================================================================
class CUI : public CScene
{
public:
	//=============================================================================
	// �񋓌^��`
	//=============================================================================
	typedef enum
	{
		LOGOTYPE_TITLE = 0,			// �^�C�g��
		LOGOTYPE_ENTER,				// �G���^�[���S
		LOGOTYPE_ARROW,				// �R���p�X���
		LOGOTYPE_SIGN,				// �Ŕ�
		LOGOTYPE_MODE00,			// ���[�h00
		LOGOTYPE_MODE01,			// ���[�h01
		LOGOTYPE_MODEFREAM,			// ���[�h�g��
		LOGOTYPE_GEAR0,				// ����0
		LOGOTYPE_CLOCK0,			// ���v�y��0
		LOGOTYPE_CLOCKHANDS0,		// ���v�̐j0
		LOGOTYPE_CLOCKGEAR0,		// ���v�̎���0
		LOGOTYPE_GEAR1,				// ����0
		LOGOTYPE_CLOCK1,			// ���v�y��0
		LOGOTYPE_CLOCKHANDS1,		// ���v�̐j0
		LOGOTYPE_CLOCKGEAR1,		// ���v�̎���0
		LOGOTYPE_CHARAFULL,			// �L�����N�^�[�S��
		LOGOTYPE_1PCHARA,			// 1P�L�����N�^�[
		LOGOTYPE_2PCHARA,			// 2P�L�����N�^�[
		LOGOTYPE_1PCHARA_FREAM,		// 1P�L�����N�^�[�g��
		LOGOTYPE_2PCHARA_FREAM,		// 2P�L�����N�^�[�g��
		LOGOTYPE_1PCHARA_NAME,		// 1P�L�����N�^�[�l�[��
		LOGOTYPE_2PCHARA_NAME,		// 2P�L�����N�^�[�l�[��
		LOGOTYPE_1PREADY,			// 1P�L�����N�^�[��������
		LOGOTYPE_2PREADY,			// 2P�L�����N�^�[��������
		LOGOTYPE_SELECTICON,		// �v���C���[�Z���N�g�A�C�R��
		LOGOTYPE_WORLDMAP,			// ���E�n�}
		LOGOTYPE_MAPFRAME,			// �}�b�v�I��g
		LOGOTYPE_MAP1,				// �}�b�v1
		LOGOTYPE_MAP2,				// �}�b�v2
		LOGOTYPE_MAP3,				// �}�b�v3
		LOGOTYPE_MAP4,				// �}�b�v4
		LOGOTYPE_MAPEXPLANATION1,	// �}�b�v����
		LOGOTYPE_MAPEXPLANATION2,	// �}�b�v����
		LOGOTYPE_MAPEXPLANATION3,	// �}�b�v����
		LOGOTYPE_MAPEXPLANATION4,	// �}�b�v����
		LOGOTYPE_GAMEBG,			// �Q�[���w�i
		LOGOTYPE_JEWELRYBG1P,		// ��Δw�i1P
		LOGOTYPE_JEWELRYBG2P,		// ��Δw�i2P
		LOGOTYPE_JEWELRYRED,		// ��ΐ�
		LOGOTYPE_JEWELRYBULE,		// ��ΐ�
		LOGOTYPE_JEWELRYGREEN,		// ��Η�
		LOGOTYPE_PLAYER1,			// �v���C���[�A�C�R��1P
		LOGOTYPE_PLAYER2,			// �v���C���[�A�C�R��2P
		LOGOTYPE_CHARANAME0,		// 1P�̃L�����N�^�[�l�[��
		LOGOTYPE_CHARANAME1,		// 2P�̃L�����N�^�[�l�[��
		LOGOTYPE_MAX				// �ő�
	} UITYPE;

	CUI(PRIORITY type);				// �R���X�g���N�^
	~CUI();							// �f�X�g���N�^
	void Init(void);				// ����������
	void Uninit(void);				// �I������
	void Update(void);				// �X�V����
	void Draw(void);				// �`�揈��

	static CUI *Create(void);		// ��������
	static HRESULT Load(void);		// �e�N�X�`����񃍁[�h����
	static void Unload(void);		// �e�N�X�`�����A�����[�h����

	void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nCnt, D3DXCOLOR col);	// ���S�ݒ菈��

	static int GetMapID(void) { return m_nMapID; }	// �}�b�vID�̎擾
	UITYPE GetType(void);			// ���S�^�C�v�擾����
	int GetCharaNum(int nPlayer) { return m_nCharaNum[nPlayer]; }	// �v���C���[�̃^�C�v���擾
	bool GetCharaDecide(int nPlayer) { return m_bCharaDecide[nPlayer]; }
	bool GetSelectMap(void) { return m_bMapSelect; }	// �}�b�v�̑I���󋵂��擾

protected:

private:
	void TitleUpdate(CInputKeyboard *pKeyboard, CInputGamepad *Gamepad0, CInputGamepad *pGamepad1);	// �^�C�g���̍X�V����
	void TutorialUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// �`���[�g���A���̍X�V����
	void MapSelectUpdate(CInputKeyboard *pKeyboard, CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// �}�b�v�I����ʂ̍X�V����
	void GameUpdate(void);								// �Q�[���̍X�V����
	void ControlGamepad(CInputGamepad *pGamepad0, CInputGamepad *pGamepad1);// �Q�[���p�b�h����
	void ControlKeyboard(CInputKeyboard *pKeyboard);	// �L�[�{�[�h����

	static LPDIRECT3DTEXTURE9 m_pTexture[LOGOTYPE_MAX];	// �e�N�X�`�����
	static char *m_apFileName[LOGOTYPE_MAX];			// ���S�̃t�@�C����

	UITYPE m_type;										// ���S�^�C�v�ϐ�
	CScene2D *m_pScene2D[LOGOTYPE_MAX];					// scene2D�̏��

	D3DXVECTOR3 TitlePos;								// �^�C�g���̈ʒu

	int m_nCntBound;									// �^�C�g��UI�o�E���h�J�E���^
	int m_nMode;										// ���[�h�ԍ�
	int m_nCharaNum[MAX_PLAYER];						// �L�����ԍ�
	int m_nCntRot[MAX_PLAYER];							// ���v�̐j��]�p�J�E���^
	int m_nCntWait[MAX_PLAYER];							// �ҋ@���ԗp�J�E���^
	int m_nCntMove[MAX_PLAYER];							// �g���ړ��p�J�E���^
	int m_nPlayer;										// �v���C���[�ԍ�
	static int m_nMapID;								// �}�b�v�ԍ�

	float m_fCntUITitle0;								// �^�C�g��UI�p�J�E���^0
	float m_fCntUITitle1;								// �^�C�g��UI�p�J�E���^1
	float m_fCntEnter;									// �G���^�[�p�J�E���^
	float m_fCntUISign;									// �Ŕp�J�E���^
	float m_fPosMove[MAX_PLAYER];						// �ʒu�ړ��p�J�E���^
	float m_fPos[MAX_PLAYER];							// ���݂̘g���e�N�X�`���ʒuX
	float m_fPosDiff[MAX_PLAYER];						// �ڕW�̘g���e�N�X�`���ʒuX
	float m_fPosOld[MAX_PLAYER];						// �O��̘g���e�N�X�`���ʒuX
	float m_fRotGear[MAX_PLAYER];						// ���Ԃ̉�]�i�[�ϐ�
	float m_fPosCul[MAX_PLAYER];						// �ʒu�v�Z�p�ϐ�
	float m_fDiff[MAX_PLAYER];							// 1�t���[���O�Ƃ̋���
	float m_fAngle[MAX_PLAYER];							// ���Ԃ̉�]�p�x
	float m_fRad[MAX_PLAYER];							// ���W�A���l

	bool m_bUITitle0;									// �^�C�g���𓮂������ǂ���0
	bool m_bUITitle1;									// �^�C�g���𓮂������ǂ���1
	bool m_bUIEnter;									// �G���^�[�̃��l�p�ϐ�
	bool m_bUIClockHands[MAX_PLAYER];					// ���v�̐j�������Ă��邩�ǂ���
	bool m_bCharaDecide[MAX_PLAYER];					// �����̃L�����N�^�[��I���������ǂ���
	bool m_bStickReturn[MAX_PLAYER];					// �p�b�h�̃X�e�B�b�N��߂������ǂ���
	bool m_bMapSelect;									// �}�b�v��I���������ǂ���
	bool m_bTransform[MAX_PLAYER];						// �ϐg�������ǂ���
	bool m_bStoneID[MAX_PLAYER][CStone::STONE_ID_MAX];	// �X�g�[��ID

};
#endif