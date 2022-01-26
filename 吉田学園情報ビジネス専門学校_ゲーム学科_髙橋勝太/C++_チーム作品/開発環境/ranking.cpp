//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "ranking.h"
#include "number.h"
#include "manager.h"
#include "logo.h"
#include "License.h"

//�O���[�o���ϐ��錾
int CRanking::m_nRankingScore[5][8] = {};				//�X�R�A�ւ̐ÓI�����o�ϐ�
CNumber		*CRanking::m_apRankingNumber[5][8] = {};	//�i���o�[�N���X�ւ̃|�C���^
int CRanking::m_nNumber[5][8] = {};
int CRanking::m_nRankScore[5] = {};
CNumber		*CRanking::m_apMaxScoreNumber[8] = {};
int CRanking::m_nMaxScoreNumber[8] = {};
int CRanking::m_nMaxScore[8] = {};
int CRanking::m_nMaxRankScore;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRanking::CRanking()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRanking::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	for (int nCount = 0; nCount < 5; nCount++)
	{
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			//�ԍ��𐶐�
			m_apRankingNumber[nCount][nCnt] = CNumber::Create(D3DXVECTOR3(pos.x - (nCnt * 70), pos.y + (nCount * 120.0f), 0.0f), 30, 40);
		}
	}

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		//�ԍ��𐶐�
		m_apMaxScoreNumber[nCnt] = CNumber::Create(D3DXVECTOR3(1800.0f - (nCnt * 70), 350.0f, 0.0f), 30, 40);
	}

	//�e�L�X�g����ǂݍ���
	LoadData();

	//�Ƌ��V�X�e���̏���
	if (CLicense::GetLisenceType() == CLicense::LISENCETYPE_GOLD)
	{
		//�Ƌ����S�[���h�̏ꍇ�X�R�A���S�{�ɂȂ�
		m_nMaxRankScore = CScore::GetScore() * 4;
	}
	else if (CLicense::GetLisenceType() == CLicense::LISENCETYPE_NORMAL)
	{
		//�Ƌ������ʂ̏ꍇ�X�R�A���Q�{�ɂȂ�
		m_nMaxRankScore = CScore::GetScore() * 2;
	}

	//�����L���O���ёւ�
	Ranking(m_nMaxRankScore);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRanking::Uninit(void)
{
	for (int nCount = 0; nCount < 5; nCount++)
	{
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			// ���_�o�b�t�@�̊J��
			if (m_apRankingNumber[nCount][nCnt] != NULL)
			{
				m_apRankingNumber[nCount][nCnt]->Uninit();
				delete m_apRankingNumber[nCount][nCnt];
				m_apRankingNumber[nCount][nCnt] = NULL;
			}
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CRanking::Update(void)
{
	//�J�E���g�A�b�v
	RankingCount++;

	//�J�E���g��150�ɂȂ�����
	if (RankingCount <= 150)
	{
		for (int nCount = 0; nCount < 5; nCount++)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				m_nRankingScore[nCount][nCnt] = rand() % 9 + 1;
			}
		}

		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			m_nMaxScore[nCnt] = rand() % 9 + 1;
		}

		//�����_�����Z�b�g
		RandRanking();
	}
	else
	{
		//�����L���O�Z�b�g
		SetRanking();
		MaxScoreRanking();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRanking::Draw(void)
{
	//�����L���O�\���̕`��
	for (int nCount = 0; nCount < 5; nCount++)
	{
		for (int nCnt = 0; nCnt < 8; nCnt++)
		{
			m_apRankingNumber[nCount][nCnt]->Draw();
		}
	}
	//�v�Z�����Ƃ��̃v���C���[�̃X�R�A
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		m_apMaxScoreNumber[nCnt]->Draw();
	}
}

//=============================================================================
// �X�R�A�̐���
//=============================================================================
CRanking * CRanking::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ���[�J���ϐ��錾
	CRanking *pRanking = NULL;
	pRanking = new CRanking;

	// ������
	if (pRanking != NULL)
	{
		pRanking->Init(pos, fSizeX, fSizeY);
	}

	return pRanking;
}

//=============================================================================
// �X�R�A�������ɕ��ѕς���@�X�R�A�ɒl������
//=============================================================================
void CRanking::RandRanking(void)
{
	//���̃����L���O�T�p
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//�������v�Z
		m_nNumber[nCount][0] = m_nRankingScore[nCount][7] % 10 / 1;
		m_nNumber[nCount][1] = m_nRankingScore[nCount][6] % 10 / 1;
		m_nNumber[nCount][2] = m_nRankingScore[nCount][5] % 10 / 1;
		m_nNumber[nCount][3] = m_nRankingScore[nCount][4] % 10 / 1;
		m_nNumber[nCount][4] = m_nRankingScore[nCount][3] % 10 / 1;
		m_nNumber[nCount][5] = m_nRankingScore[nCount][2] % 10 / 1;
		m_nNumber[nCount][6] = m_nRankingScore[nCount][1] % 10 / 1;
		m_nNumber[nCount][7] = m_nRankingScore[nCount][0] % 10 / 1;
	}
	//���̃����L���O�T�p
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//���̍��킹�Ă��炷
		m_apRankingNumber[nCount][0]->SetNumber(m_nNumber[nCount][0]);
		m_apRankingNumber[nCount][1]->SetNumber(m_nNumber[nCount][1]);
		m_apRankingNumber[nCount][2]->SetNumber(m_nNumber[nCount][2]);
		m_apRankingNumber[nCount][3]->SetNumber(m_nNumber[nCount][3]);
		m_apRankingNumber[nCount][4]->SetNumber(m_nNumber[nCount][4]);
		m_apRankingNumber[nCount][5]->SetNumber(m_nNumber[nCount][5]);
		m_apRankingNumber[nCount][6]->SetNumber(m_nNumber[nCount][6]);
		m_apRankingNumber[nCount][7]->SetNumber(m_nNumber[nCount][7]);
	}

	//��̍ő�X�R�A�p
	//�������v�Z
	m_nMaxScoreNumber[0] = m_nMaxScore[7] % 10 / 1;
	m_nMaxScoreNumber[1] = m_nMaxScore[6] % 10 / 1;
	m_nMaxScoreNumber[2] = m_nMaxScore[5] % 10 / 1;
	m_nMaxScoreNumber[3] = m_nMaxScore[4] % 10 / 1;
	m_nMaxScoreNumber[4] = m_nMaxScore[3] % 10 / 1;
	m_nMaxScoreNumber[5] = m_nMaxScore[2] % 10 / 1;
	m_nMaxScoreNumber[6] = m_nMaxScore[1] % 10 / 1;
	m_nMaxScoreNumber[7] = m_nMaxScore[0] % 10 / 1;

	//��̍ő�X�R�A�p
	//���̍��킹�Ă��炷
	m_apMaxScoreNumber[0]->SetNumber(m_nMaxScoreNumber[0]);
	m_apMaxScoreNumber[1]->SetNumber(m_nMaxScoreNumber[1]);
	m_apMaxScoreNumber[2]->SetNumber(m_nMaxScoreNumber[2]);
	m_apMaxScoreNumber[3]->SetNumber(m_nMaxScoreNumber[3]);
	m_apMaxScoreNumber[4]->SetNumber(m_nMaxScoreNumber[4]);
	m_apMaxScoreNumber[5]->SetNumber(m_nMaxScoreNumber[5]);
	m_apMaxScoreNumber[6]->SetNumber(m_nMaxScoreNumber[6]);
	m_apMaxScoreNumber[7]->SetNumber(m_nMaxScoreNumber[7]);
}
//=============================================================================
// �X�R�A��������
//=============================================================================
void CRanking::SetRanking(void)
{
	for (int nCount = 0; nCount < 5; nCount++)
	{
		//�������v�Z
		m_nNumber[nCount][0] = m_nRankScore[nCount] % 100000000 / 10000000;
		m_nNumber[nCount][1] = m_nRankScore[nCount] % 10000000 / 1000000;
		m_nNumber[nCount][2] = m_nRankScore[nCount] % 1000000 / 100000;
		m_nNumber[nCount][3] = m_nRankScore[nCount] % 100000 / 10000;
		m_nNumber[nCount][4] = m_nRankScore[nCount] % 10000 / 1000;
		m_nNumber[nCount][5] = m_nRankScore[nCount] % 1000 / 100;
		m_nNumber[nCount][6] = m_nRankScore[nCount] % 100 / 10;
		m_nNumber[nCount][7] = m_nRankScore[nCount] % 10 / 1;
	}

	for (int nCount = 0; nCount < 5; nCount++)
	{
		//���̍��킹�Ă��炷
		m_apRankingNumber[nCount][0]->SetNumber(m_nNumber[nCount][7]);
		m_apRankingNumber[nCount][1]->SetNumber(m_nNumber[nCount][6]);
		m_apRankingNumber[nCount][2]->SetNumber(m_nNumber[nCount][5]);
		m_apRankingNumber[nCount][3]->SetNumber(m_nNumber[nCount][4]);
		m_apRankingNumber[nCount][4]->SetNumber(m_nNumber[nCount][3]);
		m_apRankingNumber[nCount][5]->SetNumber(m_nNumber[nCount][2]);
		m_apRankingNumber[nCount][6]->SetNumber(m_nNumber[nCount][1]);
		m_apRankingNumber[nCount][7]->SetNumber(m_nNumber[nCount][0]);
	}
}
//=============================================================================
// �ő�X�R�A��\��
//=============================================================================
void CRanking::MaxScoreRanking(void)
{
	//�������v�Z
	m_nMaxScoreNumber[0] = m_nMaxRankScore % 100000000 / 10000000;
	m_nMaxScoreNumber[1] = m_nMaxRankScore % 10000000 / 1000000;
	m_nMaxScoreNumber[2] = m_nMaxRankScore % 1000000 / 100000;
	m_nMaxScoreNumber[3] = m_nMaxRankScore % 100000 / 10000;
	m_nMaxScoreNumber[4] = m_nMaxRankScore % 10000 / 1000;
	m_nMaxScoreNumber[5] = m_nMaxRankScore % 1000 / 100;
	m_nMaxScoreNumber[6] = m_nMaxRankScore % 100 / 10;
	m_nMaxScoreNumber[7] = m_nMaxRankScore % 10 / 1;

	//���̍��킹�Ă��炷
	m_apMaxScoreNumber[0]->SetNumber(m_nMaxScoreNumber[7]);
	m_apMaxScoreNumber[1]->SetNumber(m_nMaxScoreNumber[6]);
	m_apMaxScoreNumber[2]->SetNumber(m_nMaxScoreNumber[5]);
	m_apMaxScoreNumber[3]->SetNumber(m_nMaxScoreNumber[4]);
	m_apMaxScoreNumber[4]->SetNumber(m_nMaxScoreNumber[3]);
	m_apMaxScoreNumber[5]->SetNumber(m_nMaxScoreNumber[2]);
	m_apMaxScoreNumber[6]->SetNumber(m_nMaxScoreNumber[1]);
	m_apMaxScoreNumber[7]->SetNumber(m_nMaxScoreNumber[0]);
}
//==============================================================================
// �t�@�C���̓ǂݍ���
//==============================================================================
void CRanking::LoadData(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;

	// �f�[�^�̓ǂݍ���
	pFile = fopen("data/TEXT/ranking.txt", "r");
	if (pFile != NULL)
	{ //�t�@�C���W�J�\
		for (int nCount = 0; nCount < 5; nCount++)
		{
			fscanf(pFile, "%d", &m_nRankScore[nCount]);
		}
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}
//==============================================================================
// �����L���O�̕��ёւ�
//==============================================================================
void CRanking::Ranking(int nScore)
{
	// ���[�J���ϐ��錾
	int nSubData;

	// �����L���O�O�̏��O
	if (m_nRankScore[5 - 1] <= nScore)
	{ // �X�R�A�������L���O���̏ꍇ
		m_nRankScore[5 - 1] = nScore;
	}

	// �v�Z����
	for (int nCnt = 0; nCnt < 5 - 1; nCnt++)
	{
		for (int nCnt2 = nCnt + 1; nCnt2 < 5; nCnt2++)
		{
			if (m_nRankScore[nCnt] < m_nRankScore[nCnt2])
			{
				nSubData = m_nRankScore[nCnt];
				m_nRankScore[nCnt] = m_nRankScore[nCnt2];
				m_nRankScore[nCnt2] = nSubData;
			}
		}
	}

	//�����L���O��ۑ�
	SaveData();
}
//==============================================================================
// �f�[�^�ۑ�����
//==============================================================================
void CRanking::SaveData(void)
{
	// ���[�J���ϐ��錾
	FILE *pFile;

	// �t�@�C���̓W�J
	pFile = fopen("data/TEXT/ranking.txt", "w");
	if (pFile != NULL)
	{ // �t�@�C���W�J�\
		for (int nCount = 0; nCount < 5; nCount++)
		{
			fprintf(pFile, "%d\n", m_nRankScore[nCount]);
		}
		fclose(pFile);
	}
	else
	{ // �t�@�C���W�J�s��
	}
}
//=============================================================================
// Get�֐�
//=============================================================================
int CRanking::GetRanking(void)
{
	return S_OK;
}