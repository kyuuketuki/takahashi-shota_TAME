//=============================================================================
//
// �ʍs�l���� [passerby.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _PASSERBY_H_
#define _PASSERBY_H_

#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#include "main.h"
#include "scene.h"
#include "model.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MODEL_PARTS		(20)	// ���f���p�[�c�̍ő吔
#define TEXT_LENGTH		(128)	// �e�L�X�g�ǂݍ��݂̍ő吔
#define DATA_LENGTH		(64)	// �f�[�^�ǂݍ��݂̍ő吔

#define MAX_PASSERBY_PARTS (13)

//==============================================================================
// ���[�V�����̎��
//==============================================================================
typedef enum
{
	MOTIONTYPE_PASSERBY_NEUTRAL = 0,// �j���[�g�������[�V����
	MOTIONTYPE_PASSERBY_MOVE,		// �ړ����[�V����
	MOTIONTYPE_PASSERBY_LEFTMOVE,	// ���ړ����[�V����
	MOTIONTYPE_PASSERBY_RIGHTMOVE,	// �E�ړ����[�V����
	MOTIONTYPE_PASSERBY_JUMP,		// �W�����v���[�V����
	MOTIONTYPE_PASSERBY_MAX			// ���[�V�����̍ő吔
} MOTIONTYPE_PASSERBY;

//==============================================================================
// ���s�҂̎��
//==============================================================================
typedef enum
{
	PASSERBY_TYPE_MEN = 0,		// �j�̐l
	PASSERBY_TYPE_CAT,	        // �L
	PASSERBY_TYPE_CUSTOMER,     // ���q�l
	PASSERBY_TYPE_MAX			// ���[�V�����̍ő吔
} PASSERBY_TYPE;

//=============================================================================
// �ʍs�l�N���X�̒�`
//=============================================================================
class CPasserby : public CScene
{
public:
	CPasserby(int nPriority = 5, int nType = 0);								//�R���X�g���N�^
	virtual ~CPasserby();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//����������
	void Uninit(void);															//�I������
	void Update(void);															//�X�V����
	void Draw(void);															//�`�揈��

	static CPasserby *Create(D3DXVECTOR3 pos, D3DXVECTOR3 goalpos, D3DXVECTOR3 randpos, PASSERBY_TYPE passerbytype);						//��������
	void LoadMotion(PASSERBY_TYPE passerbytype);														//�O���ǂݍ���
	void UpdateMotion(MOTIONTYPE_PASSERBY passerby);
	void PasserbyMove(void);										//�ʍs�l�̓���
	void CustomerMove(void);													//���q�l�̓���
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	static bool GetUse(void) { return m_bUse; }
	static void SetUse(bool bUse);

private:
	CModel			*m_apModel[MAX_PASSERBY_PARTS];					//���f���ւ̃|�C���^
	char			*m_pTextName[MAX_PASSERBY_PARTS];				//���O�ۑ�
	int				m_aIndexParent[MAX_PASSERBY_PARTS];				//���f���̐e
	float			m_aOffset[MAX_PASSERBY_PARTS][6];				//���f���̈ʒu������ݒu
	float			m_aOffsetMemo[MAX_PASSERBY_PARTS][6];			//�ۑ��p���f���̈ʒu������ݒu

	//���[���h�}�g���b�N�X�֘A
	LPDIRECT3DTEXTURE9	m_pTexture;				//�e�N�X�`���̃|�C���^
	LPD3DXMESH				m_pMesh;			//���b�V��
	LPD3DXBUFFER			m_pBuffMat;			//�o�b�t�@
	DWORD					m_nNumMat;			//�}�g���b�N�X
	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3				m_pos;				//�ʒu
	D3DXVECTOR3				m_poskeep;          //�ʒu���L������ϐ�
	D3DXVECTOR3				m_rot;				//����
	D3DXVECTOR3             m_rotDest;          //�����̊���
	D3DXVECTOR3				m_RotDest;			//��]�l
	D3DXVECTOR3				m_move;				//�ړ���
	D3DXVECTOR3				m_MaxPasserby;		//�ő�l
	D3DXVECTOR3				m_MinPasserby;		//�ŏ��l
	D3DXVECTOR3				m_posold;			//�ʒu���
	D3DXVECTOR3				m_posold2;			//�ʒu���
	int						nCntPlayer;			//�v���C���[�̐�
	int						m_nNumVtx;			//���_��
	float					m_fRot;				//��]
	
	
	//�ÓI�����o�ϐ�
	static D3DXVECTOR3   m_posHoming;           //������ǔ�����

	//���[�V�����g�p
	bool					m_Loop[MOTIONTYPE_PASSERBY_MAX];
	int						m_NumKey[MOTIONTYPE_PASSERBY_MAX];
	int						m_SetParent[MAX_PASSERBY_PARTS];
	float					m_Passerby[MOTIONTYPE_PASSERBY_MAX][10][MAX_PASSERBY_PARTS][6];		//�e���[�V�����̈ʒu�������L���ꏊ
	int						m_nFrame[MOTIONTYPE_PASSERBY_MAX][10];								//�e���[�V�����̃t���[���L��
	int						m_nFrameTotal[MOTIONTYPE_PASSERBY_MAX];								//�e���[�V�����̃t���[���L��
	int						m_Key;																//���݂̃��[�V�������J�E���g
	float					m_fCounterMotion;													//���[�V�����̃J�E���^�[
	int						m_KeySet[MOTIONTYPE_PASSERBY_MAX];									//�ő�t���[�������L��
	MOTIONTYPE_PASSERBY		m_MotionType;														//���[�V�����^�C�v
	MOTIONTYPE_PASSERBY		m_MotionTypeOld;													//�O��̃��[�V�����^�C�v

	//�J�E���g�p
	int						m_nRand;													//�ʒu���Ɏg�p�p
	int                     m_nMoveCnt;                                                 //���[�V�����p�J�E���g�ϐ�
	int                     m_nNumModel;                                                //���f���̃p�[�c��
	int                     m_nNumKeyKeep;                                              //���f���̃L�[�ۑ��p
	int                     m_nBrendFrame;                                              //�u�����h���[�V�����t���[��
	int						m_nPasserbyCount;                                           //���s�҂̕ʁX�̓����ϐ�
	char                    m_pModelName[MAX_PASSERBY_PARTS][128];                      //X�t�@�C���ǂݍ��ݗp
	bool                    m_bGoal;													//�ړI�n����p
	bool                    m_bMotionChange;                                            //���[�V�����ω��p
	bool                    m_bLoopKeep;                                                //���[�v���邩���Ȃ����ۑ��p
	bool                    m_fUseScene3D;												//�����I���I�t
	D3DXVECTOR3             m_posMemo;                                                  //�ۑ��p
	D3DXVECTOR3             m_nRamdom[2];                                               //�����_������������
	D3DXVECTOR3          	m_aPos[10][4];                                              //�����蔻��̊O�ϗp
	D3DXVECTOR3				m_startpos;													//�X�^�[�g�n�_
	D3DXVECTOR3				m_goalpos;													//�������ꏊ
	D3DXVECTOR3				m_randpos;													//�����ꏊ
	PASSERBY_TYPE			m_passerbytype;                                             //���s�҂̎��
	
	//�ÓI�����o�ϐ�
	static bool             m_bUse;														//�g�p���Ă��邩�ǂ���
};

#endif
