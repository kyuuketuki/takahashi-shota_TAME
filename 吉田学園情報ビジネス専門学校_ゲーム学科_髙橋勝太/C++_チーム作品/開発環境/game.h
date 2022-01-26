//=============================================================================
//
// �Q�[����ʏ��� [game.h]
// Author : takahashi shota
//
//=============================================================================

#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
//�}�N����`
//=============================================================================
#define TEXT_LENGTH (128)
#define DATA_LENGTH (64)
#define MAX_DATA (224)

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "main.h"
#include "scene.h"
#include "scene2D.h"
#include "debugproc.h"
#include "trafficLight.h"

//�O���錾
class CVertex;
class CObject3D;
class CScene3D;
class CCar;
class CTrafficLight;
class CSound;
class CPlayer3D;
class CPasserby;
class CDestination;

//=============================================================================
// �Q�[����ʃN���X�̒�`
//=============================================================================
class CGame : public CScene
{
public:
	CGame();									//�R���X�g���N�^
	~CGame();									//�f�X�g���N�^

	HRESULT Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY);	//����������
	void Uninit(void);											//�I������
	void Update(void);											//�X�V����
	void Draw(void);											//�`�揈��

	void Switch(void);											//�؂�ւ�����

	//�A�h���X�ۑ�
	static CVertex		*GetVertex(void) { return m_pVertex; }
	static CPasserby	*GetPasserby(int nCnt) { return m_pPasserby[nCnt]; }
	static CDestination		*GetDeliver(int nCnt) { return m_pDeliver[nCnt]; }

	//����������X�t�@�C�������点��
	static int GetObjectType3D(void);

	//�������������_�������点��
	static int GetVecter(void);
				
	//����Get
	static int GetTime(void);

	//�I���I�t�̐؂�ւ�
	static bool GetSwitchScene3D(void);
	static bool GetSwitchCollision(void);

	//�p�[�e�B�N���̐ݒ���e�L�X�g����ǂݍ��ފ֐�
	void LoadParticle(void);

	//�ʍs�l�̐ݒ���e�L�X�g����ǂݍ��ފ֐�
	void LoadPasserby(void);

	static void GameParticle(D3DXVECTOR3 pos, int nType);

	//�V���ɐ��������I�u�W�F�N�g3D�̐����L��
	static int GetMaxObject3D(void);

	//��������
	static CGame *Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY);

	//�M���@�̃|�C���^
	static CTrafficLight *m_pTrafficLight[MAX_LIGHT_AREA*MAX_LIGHT_POINT];

	//�M���@�̏��擾
	static CTrafficLight *GetTrafficLight(int nLightNum) { return m_pTrafficLight[nLightNum]; }

	//�Ԃ̏��擾
	static CCar				*GetCar(int nCnt) { return m_pCar[nCnt]; }

	//�v���C���[�̏�񏊓�
	static CPlayer3D		*GetPlayer3D(void) { return m_pPlayer3D; }

	//�T�E���h�̏��擾
	static int				GetPlayBGM(void) { return m_nPlayBGM; }
	static float			GetVolumeGameBGM(void) { return m_fVolumeBGM; }

	//�T�E���h�ݒ�
	void ControlBGM(void);

private:
	LPD3DXFONT	m_pTimeFontGame;						//�\���p�t�H���g
	CScene3D	*m_pScene3D[256];						//�������������A�h���X���L��
	CSound		*m_pSound;								//�T�E���h�|�C���^
	float		m_fOldVolumeBGM;						//�ύX�O��BGM

	//�ÓI�����o�ϐ�
	static CVertex		*m_pVertex;						//�A�h���X�ۑ��p�|�C���^
	static CPasserby	*m_pPasserby[256];				//�A�h���X�ۑ��p�|�C���^
	static CDestination	*m_pDeliver[3];					//�A�h���X�ۑ��p�|�C���^
	static CPlayer3D	*m_pPlayer3D;					//�A�h���X�ۑ��p�|�C���^
	static CCar			*m_pCar[256];					//�A�h���X�ۑ��p�|�C���^
	static int			m_nPlayBGM;						//�Đ����������ԍ�
	static float		m_fVolumeBGM;					//�{�����[��

	//�p�[�e�B�N���p
	static int m_nGParicleMax[DATA_LENGTH];               //�p�[�e�B�N���̔�����
	int m_nGParicleText;								  //���ǂݍ��񂾂��̃J�E���g
	int m_nGCountText;									  //�p�[�e�B�N���̐�
	static float m_GSizeX[DATA_LENGTH];                   //�p�[�e�B�N���̃T�C�Y
	static float m_GSizeY[DATA_LENGTH];                   //�p�[�e�B�N���̃T�C�Y
	static float m_fGSpeed[DATA_LENGTH];                  //�p�[�e�B�N���̃X�s�[�h
	static D3DXCOLOR m_GParticlecol[DATA_LENGTH];         //�p�[�e�B�N���̃J���[
	static int m_nGLife[DATA_LENGTH];                     //�p�[�e�B�N���̎���
	static float m_fGGravity[DATA_LENGTH];                //�p�[�e�B�N���̏d��

	//�ʍs�l�p�ϐ�
	int m_poscount;
	int m_writecount;
	int m_nPasserbyText;									//���ǂݍ��񂾂��̃J�E���g
	D3DXVECTOR3 m_PasserbyPos[256];							//�ʍs�l�̏����ꏊ
	D3DXVECTOR3 m_PasserbyGoalPos[256];						//�ʍs�l�̃S�[���ꏊ
	D3DXVECTOR3 m_PasserbyRandPos[256];						//�ʍs�l�̂�����̃S�[���ꏊ
	D3DXVECTOR3 m_startpos[256];
	D3DXVECTOR3 m_goalpos[256];
	D3DXVECTOR3 m_randpos[256];
};

#endif