//=============================================================================
//
// �Q�[����ʏ��� [game.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "game.h"
#include "score.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "vertex.h"
#include "player3D.h"
#include "meshfield.h"
#include "scene3D.h"
#include "debugproc.h"
#include "AIguide.h"
#include "car.h"
#include "trafficLight.h"
#include "pointerX.h"
#include "destination.h"
#include "logo.h"
#include "city.h"
#include "map.h"
#include "passerby.h"
#include "License.h"
#include "delivery.h"
#include "vertex.h"
#include "debugproc.h"
#include "animation.h"
#include "particle.h"

//=============================================================================
//�}�N����`
//=============================================================================
//���X�g���W
#define DESTINATION_LIST1 (100)
#define DESTINATION_LIST2 (60)
#define DESTINATION_LIST3 (120)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
//�A�h���X�ۑ��ÓI�����o�ϐ��錾
CPasserby		*CGame::m_pPasserby[256];										//�󂯓n���p�@�ʍs�l
CCar			*CGame::m_pCar[256];											//�󂯓n���p�@��
CTrafficLight	*CGame::m_pTrafficLight[MAX_LIGHT_AREA*MAX_LIGHT_POINT] = {};	//�󂯓n���p�@�g���b�L���O�i�r
CPlayer3D		*CGame::m_pPlayer3D;											//�󂯓n���p�@�v���C���[
CDestination	*CGame::m_pDeliver[3];											//�󂯓n���p�@�z�B��
CVertex			*CGame::m_pVertex = NULL;										//�󂯓n���p�@���_�����蔻��

//�O���[�o���ϐ��錾
D3DXVECTOR3		g_posfile[256][4];					//�e�L�X�g�ɏ����o�����@���_�����蔻��p�@pos
D3DXVECTOR3		g_posfileObject3D[256];				//�e�L�X�g�ɏ����o�����@�I�u�W�F�N�g3D�p�@pos
D3DXVECTOR3		g_rotfileObject3D[256];				//�e�L�X�g�ɏ����o�����@�I�u�W�F�N�g3D�p�@rot
int				g_nTypefileObject3D[256];			//�e�L�X�g�ɏ����o�����@�I�u�W�F�N�g3D�p�@type
int				g_FileCnt = 0;						//���_�����蔻�萶���J�E���g�@TEXT�p
int				g_FileCntObject3D = 0;				//�I�u�W�F�N�g3D�����J�E���g�@TEXT�p
int				g_nPointerGameY = 0;				//UI�ʒu���
int				g_nObject3DType = 0;				//�I�u�W�F�N�g3D�^�C�v
int				g_VecCnt = 0;						//���_�����蔻�萶���J�E���g
bool			g_bMigrate = false;					//��ʑJ�ڗp
bool			g_bDeathFlagGame = false;			//���S�t���O�����p
bool			g_bSwitchScene3D = true;			//�����؂�ւ��X�C�b�`
bool			g_bSwitchCollision = false;			//�����؂�ւ��X�C�b�`

//����
int				nTimeGame = 0;						//����
int				nTimeCounter = 0;					//���ԃJ�E���^�[
int				nTimeLimitGame = 120;				//�J�ڎ��Ԑ���

//BGM
int				CGame::m_nPlayBGM;
float			CGame::m_fVolumeBGM;

int CGame::m_nGParicleMax[DATA_LENGTH] = {};             //�p�[�e�B�N���̔�����
float CGame::m_GSizeX[DATA_LENGTH] = {};
float CGame::m_GSizeY[DATA_LENGTH] = {};
float CGame::m_fGSpeed[DATA_LENGTH] = {};
D3DXCOLOR CGame::m_GParticlecol[DATA_LENGTH] = {};
int CGame::m_nGLife[DATA_LENGTH] = {};
float CGame::m_fGGravity[DATA_LENGTH] = {};

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CGame::CGame()
{
	//������
	nTimeGame = 0;					//����
	nTimeCounter = 0;				//���ԃJ�E���^�[
	nTimeLimitGame = 120;			//�^�C�����~�b�g
	g_bDeathFlagGame = false;		//���S�t���O�����p

	//���̑�
	m_nPlayBGM = 1;
	m_fVolumeBGM = 1.0f;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CGame::~CGame()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CGame::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//�Q�[���N�������C�Z���X������
	CLicense::SetLisenceType(CLicense::LISENCETYPE_GOLD);

	//�e�L�X�g����ǂݍ��݊֐�
	LoadParticle();//�p�[�e�B�N��
	LoadPasserby();//�ʍs�l

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�T�E���h���擾
	m_pSound = CManager::GetSound();

	//�T�E���hBGM
	m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME);

	// �t�H���g�I�u�W�F�N�g�̐���
	D3DXCreateFont(pDevice, 36, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pTimeFontGame);

	//���b�V���t�B�[���h
	CMeshfield::Create();

	//�p�[�e�B�N���̃e�N�X�`�����[�h
	CParticle::Load();

	//�X�R�A
	CScore::Create(D3DXVECTOR3(1800.0f, 50.0f, 0.0f), 30.0f, 40.0f);

	//�Q�[���N�����X�R�A������
	CScore::SetScore(0);

	//���_�����蔻��̐���
	D3DXVECTOR3 pos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVertex = CVertex::Create(pos2, pos2, pos2, pos2, pos2);

	//�}�b�v�̐���
	CMap::Create();

	//3D�v���C���[�̐���
	m_pPlayer3D = CPlayer3D::Create();

	//�ʍs�l�̐���
	for (int nCnt = 0; nCnt < m_nPasserbyText; nCnt++)
	{
		CPasserby::Create(m_PasserbyPos[nCnt], m_PasserbyGoalPos[nCnt], m_PasserbyRandPos[nCnt], PASSERBY_TYPE_MEN);//�ʍs�l
	}

	//���q�l�̐���
	CPasserby::Create(D3DXVECTOR3(-4495.0f, 70.0f, -55.0f + 150.0f), m_PasserbyGoalPos[0], m_PasserbyRandPos[0], PASSERBY_TYPE_CUSTOMER);//���q�l

	//�ړI�n
	char *TextName = "data/MODEL/goal.x";
	m_pDeliver[0] = CDestination::Create(D3DXVECTOR3(-4244.0f, 0.0f, -292.0f), TextName, 0);
	m_pDeliver[1] = CDestination::Create(D3DXVECTOR3(-12187.0f, 0.0f, 14467.0f), TextName, 1);
	m_pDeliver[2] = CDestination::Create(D3DXVECTOR3(-3563.0f, 0.0f, 22919.0f), TextName, 2);

	//�ړI�n�|�C���^�[
	char *TextName2 = "data/MODEL/point.x";
	CPointer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), TextName2);

	//�ԗ��p�K�C�h�̐���(�����������̐������F���݂̓S�[���Ɠ��l�Ȃ̂ł����ӂ�)
	char *TextName3 = "data/MODEL/goal.x";
	CAIguide::Create(D3DXVECTOR3(0.f, 0.f, 0.f), TextName3);

	//�M���@�̐���(�������̏�ԁF�������Ă���2��̃g���b�N�̒ʍs�ӏ��݂̂̐ݒu)
	for (int nCntArea = 0; nCntArea < MAX_LIGHT_AREA; nCntArea++)
	{
		for (int nCntPoint = 0; nCntPoint < MAX_LIGHT_POINT; nCntPoint++)
		{
			m_pTrafficLight[nCntArea*nCntPoint + nCntPoint] = CTrafficLight::Create(D3DXVECTOR3(0.f, 0.f, 0.f), nCntArea, nCntPoint);
		}
	}

	//�ԗ��̐���(���݂̐������F�g���b�N�{�X�[�p�[�J�[)
	m_pCar[0] = CCar::Create(D3DXVECTOR3(-2025.39f, 0.0f, -1746.39f), 0, CARTYPE_TRUCK);
	m_pCar[1] = CCar::Create(D3DXVECTOR3(5739.1f, 0.0f, -9422.22f), 1, CARTYPE_SUPER);

	//UI����
	{
		//�i�r�w�i
		CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 750.0f, SCREEN_HEIGHT / 2 + 330.0f, 0.0f), 160, 120, LIST);

		//�i�r�t���[��
		CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 770.0f, SCREEN_HEIGHT / 2 + 295.0f, 0.0f), 100, 30, GAMEFRAME);

		//���[������
		CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 680.0f, SCREEN_HEIGHT / 2 + 300.0f, 0.0f), 260, 100, GAMR_UI01);
	}

	//�Ƌ���
	CLicense::Create(D3DXVECTOR3(SCREEN_WIDTH - 200, 200, 0.0f), 150, 90);

	// �J�E���g
	CAnimasion::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 150, 150, 60, 2);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CGame::Uninit(void)
{
	//�T�E���h�ݒu
	m_pSound->Stop(CSound::SOUND_LABEL_BGM_GAME);
	m_nPlayBGM = 0;

	/*for (int nCnt = 0; nCnt < 256; nCnt++)
	{
		if (m_pPasserby[nCnt] != NULL)
		{
			m_pPasserby[nCnt]->Uninit();
			m_pPasserby[nCnt] = NULL;
		}
	}*/

	// �I�u�W�F�N�g�j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CGame::Update(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//�X�C�b�`����(�c�[��)
	Switch();

	//�f�o�b�N�p
#ifdef _DEBUG
	//��ʑJ��
	if (pInputKeyboard->GetTrigger(DIK_M) == true)
	{
		//���[�h�ݒ�
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);	// �V�[���J��
	}
#endif

	//��ʑJ��
	if (nTimeLimitGame <= 0)
	{
		//���[�h�ݒ�
		CFade::SetFade(CFade::FADE_OUT, CManager::MODE_RESULT);	// �V�[���J��
	}

	//�J�E���^�[����
	if (nTimeCounter >= 60)
	{
		nTimeLimitGame--;
		nTimeCounter = 0;	//�J�E���^�[
	}

	//�J�E���^�[�i�߂�
	nTimeCounter++;

	//UI�X�V����
	{
		//�t���[�������炷
		if (pInputKeyboard->GetTrigger(DIK_H) == true || pGamepad->IsButtonDown(CInput::A) == true)
		{
			if (g_nPointerGameY == 0)
			{
				CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 770.0f, SCREEN_HEIGHT / 2 + 295.0f + DESTINATION_LIST2, 0.0f), GAMEFRAME);//1
				g_nPointerGameY = 1;
			}

			else if (g_nPointerGameY == 1)
			{
				CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 770.0f, SCREEN_HEIGHT / 2 + 295.0f + DESTINATION_LIST3, 0.0f), GAMEFRAME);//2
				g_nPointerGameY = 2;
			}

			else if (g_nPointerGameY == 2)
			{
				CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 770.0f, SCREEN_HEIGHT / 2 + 295.0f, 0.0f), GAMEFRAME);//0
				g_nPointerGameY = 0;
			}

			//�i�r��؂�ւ���
			CPointer::SetPointNumber(g_nPointerGameY);
		}
	}

	//BGM�X�V����
	ControlBGM();
}

//==============================================================================
// �`�揈��
//==============================================================================
void CGame::Draw(void)
{
	// ���[�J���ϐ��錾
	RECT rect = { 0,30,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//��ʂɕ\��������
	sprintf(&aStr[0], "�c�莞�� : %d\n", nTimeLimitGame);

	// �e�L�X�g�̕`��@�c�莞�Ԃ̕\��
	m_pTimeFontGame->DrawText(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================================================
// �؂�ւ�����
//==============================================================================
void CGame::Switch(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�v���C���[�̏���ǂݍ���
	D3DXVECTOR3 posPlayer3D = CPlayer3D::GetPosition();
	D3DXVECTOR3 rotPlayer3D = CPlayer3D::GetRotation();

	//�f�o�b�N�p
#ifdef _DEBUG
	//�����蔻��I���I�t
	//�L�[�{�[�hQ���������ꍇ
	if (pInputKeyboard->GetTrigger(DIK_O) == true)
	{
		//�����X�C�b�`�؂�ւ�
		if (g_bSwitchScene3D == false)
		{
			g_bSwitchScene3D = true;
		}

		else if (g_bSwitchScene3D == true)
		{
			g_bSwitchScene3D = false;
		}
	}
#endif
}

//==============================================================================
// Get�֐��^�C��
//==============================================================================
int CGame::GetTime(void)
{
	return nTimeGame;
}

//==============================================================================
// Get�֐� ���������I�u�W�F�N�g3D�@�\���p
//==============================================================================
int CGame::GetObjectType3D(void)
{
	return g_nObject3DType;
}

//==============================================================================
// Get�֐� �����������_���@�\���p
//==============================================================================
int CGame::GetVecter(void)
{
	return g_VecCnt;
}

//==============================================================================
// Get�֐� �����؂�ւ��X�C�b�`
//==============================================================================
bool CGame::GetSwitchScene3D(void)
{
	return g_bSwitchScene3D;
}

//==============================================================================
// Get�֐� �����蔻��؂�ւ��X�C�b�`
//==============================================================================
bool CGame::GetSwitchCollision(void)
{
	return g_bSwitchCollision;
}

//==============================================================================
// Get�֐� �V�ɐݒu�����I�u�W�F�N�g3D�̐�
//==============================================================================
int CGame::GetMaxObject3D(void)
{
	return g_FileCntObject3D;
}

//==============================================================================
// ��������
//==============================================================================
CGame * CGame::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CGame *pGame;
	pGame = new CGame;

	// NULL�`�F�b�N
	if (pGame != NULL)
	{
		pGame->Init(pos, fSizeX, fSizeY);
	}

	return pGame;
}

//==============================================================================
//BGM���ʑ��쏈��
//==============================================================================
void CGame::ControlBGM(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_F1) == true)
	{//�T�E���h���Đ��E�ꎞ��~
		if (m_nPlayBGM == 2)
		{
			m_nPlayBGM = 1;
		}
		else if (m_nPlayBGM == 1)
		{
			m_nPlayBGM = 2;
		}
		else if (m_nPlayBGM == 0)
		{
			m_nPlayBGM = 1;
			m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME);
			m_pSound->Pause(CSound::SOUND_LABEL_BGM_GAME);
		}

		if (m_nPlayBGM == 1)
		{
			m_pSound->PlayMiddle(CSound::SOUND_LABEL_BGM_GAME);
		}
		else if (m_nPlayBGM == 2)
		{
			m_pSound->Pause(CSound::SOUND_LABEL_BGM_GAME);
		}
	}
	if (pInputKeyboard->GetTrigger(DIK_F2) == true)
	{//�T�E���h���Đ��E��~
		if (m_nPlayBGM == 0)
		{
			m_nPlayBGM = 1;
		}
		else if (m_nPlayBGM == 1)
		{
			m_nPlayBGM = 0;
		}
		else if (m_nPlayBGM == 2)
		{
			m_nPlayBGM = 1;
		}

		if (m_nPlayBGM == 1)
		{
			m_pSound->Play(CSound::SOUND_LABEL_BGM_GAME);
		}
		else if (m_nPlayBGM == 0)
		{
			m_pSound->Stop(CSound::SOUND_LABEL_BGM_GAME);
		}
	}

	if (pInputKeyboard->GetPress(DIK_F3) == true)
	{
		m_fVolumeBGM += 0.005f;
		if (m_fVolumeBGM >= 1.0f)
		{
			m_fVolumeBGM = 1.0f;
		}
	}

	else if (pInputKeyboard->GetPress(DIK_F4) == true)
	{
		m_fVolumeBGM -= 0.005f;
		if (m_fVolumeBGM <= 0.0f)
		{
			m_fVolumeBGM = 0.0f;
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{
		if (m_fVolumeBGM != 0.0f)
		{
			m_fOldVolumeBGM = m_fVolumeBGM;
			m_fVolumeBGM = 0.0f;
		}
		else if (m_fVolumeBGM == 0.0f)
		{
			m_fVolumeBGM = m_fOldVolumeBGM;
		}
	}
	m_pSound->SetVolume(CSound::SOUND_LABEL_BGM_GAME, m_fVolumeBGM);
}
//===========================================================================
//�p�[�e�B�N���̐ݒ���e�L�X�g����ǂݍ���
//===========================================================================
void CGame::LoadParticle(void)
{
	FILE *pFile = NULL;

	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[128] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObject[DATA_LENGTH] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObjectEND[DATA_LENGTH] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�

												// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/ParticleSet.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	while (strcmp("END_SCRIPT", pText) != 0)
	{
		aText[0] = { NULL };

		if (bComment == false)
		{
			// �R�����g�A�E�g���Ă��Ȃ�
			fscanf(pFile, "%s", &aText[0]);

			if (aText[0] == '#')
			{
				// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g��
				bComment = true;
			}

			else
			{
				if (strcmp("PARTICLECOUNTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_PARTICLECOUNTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Size", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_GSizeX[m_nGParicleText]);
							fscanf(pFile, "%f", &m_GSizeY[m_nGParicleText]);
						}
						if (strcmp("Speed", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fGSpeed[m_nGParicleText]);
						}
						if (strcmp("Col", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].r);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].g);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].b);
							fscanf(pFile, "%f", &m_GParticlecol[m_nGParicleText].a);
						}
						if (strcmp("Life", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nGLife[m_nGParicleText]);
						}
						if (strcmp("Gravity", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_fGGravity[m_nGParicleText]);
						}
						if (strcmp("ParticleMax", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%d", &m_nGParicleMax[m_nGParicleText]);
						}
					}
					//�J�E���g�A�b�v
					m_nGParicleText++;
				}
			}
		}
		else if (bComment == true)
		{
			// �R�����g�A�E�g����
			// ���[�J���ϐ��錾
			char a = NULL;
			char b = NULL;
			fscanf(pFile, "%c", &a);
			while (a != '\n' && b != '\n')
			{
				fscanf(pFile, "%s", &aText[0]);
				fscanf(pFile, "%c", &b);
			}
			bComment = false;
		}
	}
	//�t�@�C�������
	fclose(pFile);
}
//===========================================================================
//�p�[�e�B�N���̐ݒ�
//===========================================================================
void CGame::GameParticle(D3DXVECTOR3 pos, int nType)
{
	for (int nCnt = 0; nCnt < m_nGParicleMax[nType]; nCnt++)
	{
		//�p�[�e�B�N������
		CParticle::Create(pos, m_GSizeX[nType], m_GSizeY[nType], m_fGSpeed[nType], m_GParticlecol[nType], m_nGLife[nType], m_fGGravity[nType]);
	}
}

//===========================================================================
//�ʍs�l�̐ݒ���e�L�X�g����ǂݍ���
//===========================================================================
void CGame::LoadPasserby(void)
{
	FILE *pFile = NULL;

	char aEqual[2] = { NULL };					// �u=�v�ǂݎ��p�ϐ�
	bool bComment = false;						// �R�����g�A�E�g���邩
	char aText[228] = { NULL };			// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObject[256] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�
	char aSetObjectEND[256] = { NULL };	// �e�L�X�g�ǂݎ��p�ϐ�

												// strcmp�ǂݎ��p�|�C���^
	char *pText = &aText[0];
	char *pSetObject = &aSetObject[0];
	char *pSetObjectEND = &aSetObjectEND[0];

	pFile = fopen("data/TEXT/GetPasserby.txt", "r");

	aText[0] = { NULL };
	pSetObject[0] = { NULL };
	pSetObjectEND[0] = { NULL };

	while (strcmp("END_SCRIPT", pText) != 0)
	{
		aText[0] = { NULL };

		if (bComment == false)
		{
			// �R�����g�A�E�g���Ă��Ȃ�
			fscanf(pFile, "%s", &aText[0]);

			if (aText[0] == '#')
			{
				// ������̐擪���u#�v�Ȃ�΃R�����g�A�E�g��
				bComment = true;
			}

			else
			{
				if (strcmp("PASSERBYCOUNTSET", aText) == 0)
				{
					pSetObjectEND[0] = { NULL };

					while (strcmp("END_PASEERBYCOUNTSET", pSetObjectEND) != 0)
					{
						fscanf(pFile, "%s", &pSetObjectEND[0]);

						if (strcmp("Pos", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_PasserbyPos[m_nPasserbyText].x);
							fscanf(pFile, "%f", &m_PasserbyPos[m_nPasserbyText].y);
							fscanf(pFile, "%f", &m_PasserbyPos[m_nPasserbyText].z);
						}
						if (strcmp("GoalPos", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_PasserbyGoalPos[m_nPasserbyText].x);
							fscanf(pFile, "%f", &m_PasserbyGoalPos[m_nPasserbyText].y);
							fscanf(pFile, "%f", &m_PasserbyGoalPos[m_nPasserbyText].z);
						}
						if (strcmp("RandPos", pSetObjectEND) == 0)
						{
							//�G�̃J�E���g
							fscanf(pFile, "%s", &aEqual[0]);
							fscanf(pFile, "%f", &m_PasserbyRandPos[m_nPasserbyText].x);
							fscanf(pFile, "%f", &m_PasserbyRandPos[m_nPasserbyText].y);
							fscanf(pFile, "%f", &m_PasserbyRandPos[m_nPasserbyText].z);
						}
					}
					//�J�E���g�A�b�v
					m_nPasserbyText++;
				}
			}
		}
		else if (bComment == true)
		{
			// �R�����g�A�E�g����
			// ���[�J���ϐ��錾
			char a = NULL;
			char b = NULL;
			fscanf(pFile, "%c", &a);
			while (a != '\n' && b != '\n')
			{
				fscanf(pFile, "%s", &aText[0]);
				fscanf(pFile, "%c", &b);
			}
			bComment = false;
		}
	}
	//�t�@�C�������
	fclose(pFile);
}