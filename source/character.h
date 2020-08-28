// ===================================================================
//
// �L�����N�^�[���� [ character.h ]
// Author : Seiya Takahashi
//
// ===================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#define _CRT_SECURE_NO_WARNINGS // �x������

// ===================================================================
// �C���N���[�h�t�@�C��
// ===================================================================
#include "main.h"
#include "scene.h"
#include "motion.h"
#include "charaParam.h"
#include "kananlibrary.h"
#include "stone.h"

// ===================================================================
// �}�N����`
// ===================================================================
#define TIME_TRANS		(15 * ONE_SECOND_FPS)	// �ϐg�̎���
#define LIFE_DEFAULT	(100.0f)				// ���C�t�̏����l

// ===================================================================
// �N���X��`
// ===================================================================
class CModelCharacter;
class CCapsuleCollider;
class CCharaParam;

class CCharacter : public CScene
{
public:
	enum COLLIPARTS
	{
		COLLIPARTS_NONE = -1,	// �Ȃ�
		COLLIPARTS_BODY,		// ��
		COLLIPARTS_FOREARM_R,	// �E�O�r
		COLLIPARTS_UPPERARM_R,	// �E��r
		COLLIPARTS_FOREARM_L,	// �O�r
		COLLIPARTS_UPPERARM_L,	// ��r
		COLLIPARTS_MAX			// �ő�
	};

	typedef enum
	{	// �������̏��
		STATE_NONE = 0,			// �����Ȃ�
		STATE_LIFT,				// �������グ
		STATE_LIFT_NEUTRAL,		// �������j���[�g����
		STATE_WALK,				// ����������
		STATE_THROW,			// ������
		STATE_LIFT_MAX			// �ő吔
	} STATE_LIFT_OBJET;

	CCharacter(CScene::PRIORITY nPriority);			// �R���X�g���N�^
	~CCharacter();									// �f�X�g���N�^

	void               Init(void);					// ������
	void               Uninit(void);				// �I��
	void               Update(void);				// �X�V
	void               Draw(void);					// �`��

	inline void        SetPos(const D3DXVECTOR3 & pos)			{ m_pos = pos; }					// �ʒu�̐ݒ�
	inline void        SetMove(const D3DXVECTOR3 & move)		{ m_move = move; }					// �ړ��l�̐ݒ�
	inline void        SetRot(const D3DXVECTOR3 & rot)			{ m_rot = rot; }					// ��]�̐ݒ�
	inline void        SetRotDest(const D3DXVECTOR3 & rotDest)	{ m_rotDest = rotDest; }			// �ړI�̉�]�̐ݒ�
	inline void        SetbWalk(const bool &bWalk)				{ m_bWalk = bWalk; }				// �����̐ݒ�
	inline void        SetbJump(const bool &bJump)				{ m_bJump = bJump; }				// �W�����v�̐ݒ�
	inline void        SetLife(const float &nLife)				{ m_nLife = nLife; }				// ���C�t�̐ݒ�
	inline void        SetAttakHit(const bool &bAttackHit)		{ m_bAttakHit = m_bAttack; }		// �U���𓖂Ă��t���O
	inline void        SetAttakHitStone(const bool &bAttackHit)	{ m_bAttakHitStone = m_bAttack; }	// �X�g�[���ɍU���𓖂Ă��t���O
	void               SetModelType(CHARACTER_TYPE type);											// ���f���̐ݒ�
	void               SetCylinderCoillider(void);													// �V�����_�[�R���C�_�[�̐ݒ�

	inline CHARACTER_TYPE    GetCharaType(void)					{ return m_type; }					// �L�����^�C�v�̎擾
	inline STATE_LIFT_OBJET  GetStateLift(void)					{ return m_StateLift; }				// �������グ�̏�Ԏ擾
	inline D3DXVECTOR3       &GetPos(void)						{ return m_pos; }					// �ʒu�̎擾
	inline D3DXVECTOR3       &GetMove(void)						{ return m_move; }					// �ړ��l�̎擾
	inline D3DXVECTOR3       &GetRot(void)						{ return m_rot; }					// ��]�̎擾
	inline D3DXVECTOR3       &GetRotDest(void)					{ return m_rotDest; }				// ��]��̎擾
	inline float             &GetMaxLife(void)					{ return m_param.fMaxLife; }		// �ő僉�C�t�擾
	inline float             &GetLife(void)						{ return m_nLife; }					// ���C�t�擾
	inline bool              &GetbJump(void)					{ return m_bJump; }					// �W�����v��Ԃ̎擾
	inline void              Damage(const int nDamage)			{ m_nLife -= nDamage; }				// �_���[�W����
	inline CCapsuleCollider* GetCapCollider(int nPartsIndex)	{ return m_pCapColi[nPartsIndex]; }	// �J�v�Z���R���C�_�[�̎擾

	inline bool				 GetbMotionAttack(void);				// ���[�V�����̍U������̎擾
	inline bool				 GetStoneType(int nID)				{ return m_bGetStoneType[nID]; }	// �擾�����X�g�[���^�C�v�擾

	void			   Daunted(const int nGap);						// ���ݏ���
	void			   VictoryAction(void);							// �����s��

protected:
	CModelCharacter *m_pModelCharacter;					// ���f���L�����N�^�[�̃|�C���^
	CHARACTER_TYPE	 m_type;							// �L�����N�^�[�̃^�C�v
	CHARACTER_TYPE	 m_typeTrans;						// �ϐg���̃^�C�v

	LPD3DXMESH		m_pMesh;							// ���b�V�����
	LPD3DXBUFFER	m_pBuffMat;							// �}�e���A�����
	DWORD			m_nNumMat;							// �}�e���A�����̐�

	D3DXVECTOR3 m_pos;									// �ʒu
	D3DXVECTOR3 m_posOld;								// �O��̈ʒu
	D3DXVECTOR3 m_posBegin;								// �����ʒu
	D3DXVECTOR3 m_move;									// �ړ��l
	D3DXVECTOR3 m_rot;									// ��]
	D3DXVECTOR3 m_rotDest;								// �ړI�̉�]
	D3DXVECTOR3 m_rotDif;								// ��]�̍�
	D3DXMATRIX	m_mtxWorld;								// ���[���h�}�g���b�N�X
	STATE_LIFT_OBJET m_StateLift;						// �������グ�̏��
	CCapsuleCollider* m_pCapColi[COLLIPARTS_MAX];	// �V�����_�[�R���C�_�[�|�C���^

	CCharaParam::PLAYER_PARAM m_param;					// �v���C���[�̃p�����[�^�[
	int						  m_nCntTrans;				// �ǂꂮ�炢�̎��ԕϐg���Ă��邩
	int						  m_nNumStone;				// �擾�����X�g�[���̐�
	float					  m_nLife;					// ���C�t
	bool					  m_bJump;					// �W�����v�������ǂ���
	bool					  m_bWalk;					// �����Ă邩�ǂ���
	bool					  m_bTrans;					// �ϐg���Ă��邩
	bool					  m_bAttack;				// �U�����Ă��邩
	bool					  m_bBlowAway;				// �������ł��邩
	bool					  m_bSmashBlowAway;			// �X�}�b�V���Ő������ł��邩
	bool					  m_bDaunted;				// �Ђ��ł��邩
	bool					  m_bDown;					// �_�E�����Ă��邩�ǂ���
	bool					  m_bInvincible;			// ���G���ǂ���
	static int				  m_nNumCharacter;			// �L�����N�^�[�̑���
	int						  m_nCntJump;				// �W�����v���[�V�����؂�ւ��̃J�E���^
	int						  m_nAttackFrame;			// �U�����[�V�����؂�ւ��̃J�E���^
	int						  m_nAttackFlow;			// �U���̗���
	int						  m_nCntGap;				// ���̃J�E���^
	int						  m_nCntDown;				// �_�E�����Ă��鎞��

	bool		m_bAttakHit;							// �U���𓖂Ă��t���O
	bool		m_bAttakHitStone;						// �X�g�[���ɍU���𓖂Ă��t���O
	bool m_bGetStoneType[CStone::STONE_ID_MAX];	// �擾�����X�g�[���̃^�C�v

private:
	void Move(void);									// �ړ��֐�
	void Rot(void);										// ��]�֐�
	void Motion(void);									// ���[�V�����֐�
	void Trans(void);									// �ϐg�֐�
};

#endif
