//=============================================================================
//
// �v���C���[3D���� [player3D.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _PLAYER3D_H_
#define _PLAYER3D_H_

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "model.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MODEL_PARTS			(20)	// ���f���p�[�c�̍ő吔
#define TEXT_LENGTH			(128)	// �e�L�X�g�ǂݍ��݂̍ő吔
#define DATA_LENGTH			(64)	// �f�[�^�ǂݍ��݂̍ő吔
#define PLAYER_SPEED		(30)	// �f�[�^�ǂݍ��݂̍ő吔
#define MAX_PLAYER_PARTS	(30)	//�g�p�ł���p�[�c��
#define MAX_MOTION          (20)    //���[�V�����̍ő吔

#define	MAX_COUNT_VIOLATION	(120)	// �ᔽ�s�ׂ�����false�ɂ���܂ł̃t���[����

//==============================================================================
// ���[�V�����̎��
//==============================================================================
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,	// �j���[�g�������[�V����
	MOTIONTYPE_MOVE,		// �ړ����[�V����
	MOTIONTYPE_LEFTMOVE,	// ���ړ����[�V����
	MOTIONTYPE_RIGHTMOVE,	// �E�ړ����[�V����
	MOTIONTYPE_FALLMOVE,	// �]�ԃ��[�V����
	MOTIONTYPE_MAX			// ���[�V�����̍ő吔
} MOTIONTYPE;

//=============================================================================
// �v���C���[3D�N���X�̒�`
//=============================================================================
class CPlayer3D : public CScene
{
public:
	CPlayer3D(int nPriority = 5, int nType = 0);								//�R���X�g���N�^
	virtual ~CPlayer3D();														//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);					//����������
	void Uninit(void);															//�I������
	void Update(void);															//�X�V����
	void Draw(void);															//�`�揈��

	static CPlayer3D *Create();													//��������
	void LoadMotion(void);														//�O���ǂݍ���
	void CPlayer3D::UpdateMotion(int MOTIONTYPE, bool bUse);                    //���[�V�����X�V�p

	static D3DXVECTOR3	GetPosition(void);										//�ʒu�擾�p
	static D3DXVECTOR3	GetRotation(void);										//�����擾�p

	//�ړ������̋���(�I���I�t)
	static void SetMoveUse(bool bMoveUse);

	static bool GetViolation(void) { return m_bViolation; }
	static int GetMotionType(void) { return m_MotionType; }

private:
	//�e�L�X�g�p
	CModel			*m_apModel[MAX_PLAYER_PARTS];				//���f���ւ̃|�C���^
	char			*m_pTextName[MAX_PLAYER_PARTS];				//���O�ۑ�
	int				m_aIndexParent[MAX_PLAYER_PARTS];			//���f���̐e
	float			m_aOffset[MAX_PLAYER_PARTS][6];				//���f���̈ʒu������ݒu
	float			m_aOffsetMemo[MAX_PLAYER_PARTS][6];			//�ۑ��p���f���̈ʒu������ݒu

	D3DXMATRIX				m_mtxWorld;			//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9		m_pTexture;			//�e�N�X�`���̃|�C���^
	LPD3DXMESH				m_pMesh;			//���b�V��
	LPD3DXBUFFER			m_pBuffMat;			//�o�b�t�@
	DWORD					m_nNumMat;			//�}�g���b�N�X

	D3DXVECTOR3				m_move;				//�ړ���
	D3DXVECTOR3				m_MaxPlayer;		//�ő�l
	D3DXVECTOR3				m_MinPlayer;		//�ŏ��l
	D3DXVECTOR3				m_posold;			//�ʒu�t���[���O�̈ʒu���
	D3DXVECTOR3             m_rotDest;          //�����̊���
	D3DXVECTOR3				m_RotDest;			//�p�x�v�Z�p
	int						m_nNumVtx;			//���_��
	float					m_fRot;				//��]

												//�ÓI�����o�ϐ�
	static D3DXVECTOR3		m_pos;				//�ʒu
	static D3DXVECTOR3		m_rot;				//����

												//���[�V�����g�p
	int						m_NumKey[MAX_MOTION];									//�e���[�V�����̃t���[����
	int						m_KeySet[MAX_MOTION];									//�ő�t���[�������L��
	int                     m_nMoveCnt;                                                 //���[�V�����p�J�E���g�ϐ�
	int						m_SetParent[MAX_PLAYER_PARTS];								//�e�p�[�c�̐e�q�֌W
	int						m_nFrame[MAX_MOTION][10];								//�e���[�V�����̃t���[���L��
	int						m_nFrameTotal[MAX_MOTION];								//�e���[�V�����̃t���[���L��
	int						m_Key;														//���݂̃��[�V�������J�E���g
	float					m_Player3D[MAX_MOTION][10][MAX_PLAYER_PARTS][6];		//�e���[�V�����̈ʒu�������L���ꏊ
	int 					m_fCounterMotion;											//���[�V�����̃J�E���^�[
	bool					m_Loop[MAX_MOTION];										//�e���[�V�����̃��[�v����
	static int 				    m_MotionType;												//���[�V�����u�����h�@����
	int 					m_MotionTypeOld;											//���[�V�����u�����h�@�ߋ�

																						//���f���g�p
	static int              m_nNumModel;                                                //���f���̃p�[�c��
	int                     m_nNumKeyKeep;                                              //���f���̃L�[�ۑ��p
	int                     m_nBrendCount;												//���[�V�����u�����h�J�E���g
	int                     m_nBrendFrame;                                              //�u�����h���[�V�����t���[��
	char                    m_pModelName[MAX_PLAYER_PARTS][128];                        //X�t�@�C���ǂݍ��ݗp	
	bool                    m_bLoopKeep;                                                //���[�v���邩���Ȃ����ۑ��p
	bool                    m_bMotionChange;                                            //���[�V�����ω��p
	D3DXVECTOR3             m_posMemo;                                                  //�ۑ��p

																						//�p�g�J�[���ǐՏ������邽�߂̕ϐ�
	static bool				m_bViolation;												//���݈ᔽ�s�ׂ�������
	int						m_nCntViolation;											//�ᔽ�s�ה���̎��Ԍv��
	bool					m_bInterSectionTurn;										//�����_�Ȃ�������
	int						m_nCntInterSectionTurn;										//�Ȃ�������̔���p
																						//�M���@�̊m�F�����邽�߂̕ϐ�
	float					m_fVecLightX;
	float					m_fVecLightZ;
	float					m_fDisLightZX;
	float					m_fAngleLight;

	bool                    m_nNextKey;
	int                     m_TotalKey;
	int                     m_GetKey;
	bool                    m_bMove;
	int                     m_MotionChange;
	int                     m_nCounter;
	int                     m_nCnt;

	static bool				m_bMoveUse;
};

#endif