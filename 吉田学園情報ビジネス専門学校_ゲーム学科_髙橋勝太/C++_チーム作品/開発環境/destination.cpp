//=============================================================================
//
// �ړI�n(�z�B��)�@���� [destination.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "destination.h"
#include "sceneX.h"
#include "player3D.h"
#include "score.h"
#include "delivery.h"
#include "game.h"

//�������ꂽ�񐔂��L��
D3DXVECTOR3 g_posDestination[3];				//�ʒu���L���@�Ăяo���p
int			g_CntDestination = 0;
int			g_CntDestination2 = 0;
int			g_nNumberDestination[3];			//�ʒu�i���o�[���L��
int			g_TimeDestination[3];				//��������
float		g_fRangeDestination[3];				//��������
bool		CDestination::m_bMotion = false;	//���[�V�����ω��p
bool		CDestination::m_bUseDestination;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDestination::CDestination(int nPriority, int nType)
{
	//������
	m_bUseDestination = false;

	///�����_���ɃA�C�e���𐶐�
	int nRand = rand() % 40;
	m_TimeLimit = 60 + nRand;

	//�^�C�����~�b�g
	m_MaxTimeLimit = m_TimeLimit;

	m_nRandomNumber = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDestination::~CDestination()
{
	//�O���[�o���ϐ��̏�����
	g_CntDestination = 0;
	g_CntDestination2 = 0;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CDestination::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	CSceneX::Init(pos, fSizeX, fSizeY);

	//�J�E���g���Z�b�g
	//�z�B���3�ɌŒ肷��
	if (g_CntDestination2 == 3)
	{
		g_CntDestination2 = 0;
	}

	//�������ꂽ�����L��
	m_nRandomNumber = g_CntDestination2;
	g_CntDestination2++;

	m_posdes = pos;

	return S_OK;
}


//=============================================================================
// �I������
//=============================================================================
void CDestination::Uninit(void)
{
	CSceneX::Uninit();

	//�l�̏�����
	g_CntDestination2 = NULL;

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CDestination::Update(void)
{
	//�^�C���J�E���^�[��60�ɂȂ����ꍇ
	if (m_nTimeCounter >= 60)
	{
		m_nTime++;

		//�������Ԃ̍X�V
		m_TimeLimit -= 1;

		//�^�C���J�E���^�[��0�ɂ���
		m_nTimeCounter = 0;
	}

	//�ʒu�����󂯎�� �v���C���[
	D3DXVECTOR3 p = CPlayer3D::GetPosition();

	//�ʒu�����󂯎��
	D3DXVECTOR3 m_pos = CSceneX::GetPosition();
	D3DXVECTOR3 pP = CPlayer3D::GetPosition();

	////�ʒu���v���C���[�̏�ɐݒ�
	//p = D3DXVECTOR3(p.x, 50.0f, p.z);

	//���������߂�
	float f = (float)sqrt((m_pos.x - p.x) * (m_pos.x - p.x) + (m_pos.z - p.z) * (m_pos.z - p.z));

	//�^�C���J�E���^�[��i�߂�
	m_nTimeCounter++;
	
	//�󂯓n���悤
	g_TimeDestination[m_nRandomNumber] = m_TimeLimit;
	g_fRangeDestination[m_nRandomNumber] = f;

	//���a�̑傫��
	float nRadius = 400.0f;

	//�����蔻��
	if (m_pos.x + nRadius >= pP.x && m_pos.x - nRadius <= pP.x&&
		m_pos.z + nRadius >= pP.z && m_pos.z - nRadius <= pP.z)
	{
		//�p�[�e�B�N������
		CGame::GameParticle(D3DXVECTOR3(m_pos.x, m_pos.y + 200.0f, m_pos.z), 0);

		//�ʒu��ς��鏈����������
		m_bUseDestination = true;

		//���[�V�����ω��p�̃t���O���擾
		m_bMotion = true;

		//�X�R�A���Z
		CScore::AddScore(300);

		//�ו���͂��鏈��
		CDelivery::Create(m_pos);
	}

	//���Ԑ؂�̏ꍇ
	if (m_TimeLimit <= 0)
	{
		//�ʒu��ς��鏈����������
		m_bUseDestination = true;
	}

	//���3�ړI�n��u��
	if (m_bUseDestination == true)
	{
		//�ʒu��Random�֐��Ŏw�肵���ʒu�Ɉړ�������
		CSceneX::SetPosition(CDestination::RandomPos());

		//�ʒu��������������
		g_posDestination[m_nRandomNumber] = CSceneX::GetPosition();

		//�V���ɐݒ�
		m_TimeLimit = 60;

		//�ʒu��ς���������I�t�ɂ���
		m_bUseDestination = false;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CDestination::Draw(void)
{
	CSceneX::Draw();
}

//=============================================================================
// �����_���Ȉʒu�킽��
//=============================================================================
D3DXVECTOR3 CDestination::RandomPos(void)
{
	//�����_���Ȕԍ�������
	int nRand = rand() % 9;

	//�������[�v
	while (1)
	{
		//���������ʒu�𓯂��Ȃ�@�ʒu�i���o�[�Ŕ��f
		if (g_nNumberDestination[0] == nRand || g_nNumberDestination[1] == nRand || g_nNumberDestination[2] == nRand)
		{
			//������x�l����ꂩ����
			nRand = rand() % 9;
		}

		//����ȊO
		else
		{
			break;
		}
	}

	D3DXVECTOR3 r;			//�L���p

	switch (nRand)
	{
	case 0:
		r = D3DXVECTOR3(-4244.0f, 0.0f, -292.0f);
		break;
	case 1:
		r = D3DXVECTOR3(-12187.0f, 0.0f, 14467.0f);
		break;
	case 2:
		r = D3DXVECTOR3(-3563.0f, 0.0f, 22919.0f);
		break;
	case 3:
		r = D3DXVECTOR3(9969.0f, 0.0f, 27973.0f);
		break;
	case 4:
		r = D3DXVECTOR3(10303.0f, 0.0f, -7403.0f);
		break;
	case 5:
		r = D3DXVECTOR3(10320.0f, 0.0f, -21817.0f);
		break;
	case 6:
		r = D3DXVECTOR3(-26730.0f, 0.0f, -20450.0f);
		break;
	case 7:
		r = D3DXVECTOR3(-19828.0f, 0.0f, -2622.0f);
		break;
	case 8:
		r = D3DXVECTOR3(14091.0f, 0.0f, -3941.0f);
		break;
	}

	//�ʒu�i���o�[������
	g_nNumberDestination[m_nRandomNumber] = nRand;
	m_nNumber = nRand;

	return r;
}

//=============================================================================
// ��������
//=============================================================================
CDestination * CDestination::Create(D3DXVECTOR3 pos, char * TextName, int RandNumber)
{
	// ���[�J���ϐ��錾
	CDestination *pDestination = NULL;
	pDestination = new CDestination;

	//�e�L�X�g�����킽��
	pDestination->BindTextName(TextName);

	//�ʒu�i���o�[������
	g_nNumberDestination[g_CntDestination] = RandNumber;

	//Get�p�ɏ�������
	g_posDestination[g_CntDestination] = pos;

	//�������ꂽ���ɐi�߂�
	g_CntDestination++;

	// ������
	if (pDestination != NULL)
	{
		pDestination->Init(pos, 0, 0);
	}

	return pDestination;
}

//=============================================================================
// Get�֐��@�ʒu
//=============================================================================
D3DXVECTOR3 CDestination::GetPosition(int nCnt)
{
	return g_posDestination[nCnt];
}

//=============================================================================
// Get�֐��@��������
//=============================================================================
int CDestination::GetLimit(int nCnt)
{
	return g_TimeDestination[nCnt];
}

int CDestination::GetRange(int nCnt)
{
	return (int)g_fRangeDestination[nCnt];
}
