//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "score.h"
#include"number.h"
#include "manager.h"

//�O���[�o���ϐ��錾
int CScore::m_nScore = NULL;				//�X�R�A�ւ̐ÓI�����o�ϐ�
CNumber		*CScore::m_apNumber[8] = {};	//�i���o�[�N���X�ւ̃|�C���^

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		//�ԍ��𐶐�
		m_apNumber[nCnt] = CNumber::Create(D3DXVECTOR3(pos.x - (nCnt * 70), pos.y, 0.0f), fSizeX, fSizeY);
	}
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	CManager::SetPlayerScore(m_nScore);

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		// ���_�o�b�t�@�̊J��
		if (m_apNumber[nCnt] != NULL)
		{
			m_apNumber[nCnt]->Uninit();
			delete m_apNumber[nCnt];
			m_apNumber[nCnt] = NULL;
		}
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		m_apNumber[nCnt]->Draw();
	}
}

//=============================================================================
// �X�R�A�̐���
//=============================================================================
CScore * CScore::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �X�R�A�̐���
	CScore *pScore = NULL;
	pScore = new CScore;

	// ������
	if (pScore != NULL)
	{
		pScore->Init(pos, fSizeX, fSizeY);
	}

	return pScore;
}

//=============================================================================
// �X�R�A�������ɕ��ѕς���@�X�R�A�ɒl������
//=============================================================================
void CScore::SetScore(int nScore)
{
	m_nScore = nScore;

	int nNumber[8];

	//�������v�Z
	nNumber[0] = m_nScore % 100000000 / 10000000;
	nNumber[1] = m_nScore % 10000000 / 1000000;
	nNumber[2] = m_nScore % 1000000 / 100000;
	nNumber[3] = m_nScore % 100000 / 10000;
	nNumber[4] = m_nScore % 10000 / 1000;
	nNumber[5] = m_nScore % 1000 / 100;
	nNumber[6] = m_nScore % 100 / 10;
	nNumber[7] = m_nScore % 10 / 1;

	//���̍��킹�Ă��炷
	m_apNumber[0]->SetNumber(nNumber[7]);
	m_apNumber[1]->SetNumber(nNumber[6]);
	m_apNumber[2]->SetNumber(nNumber[5]);
	m_apNumber[3]->SetNumber(nNumber[4]);
	m_apNumber[4]->SetNumber(nNumber[3]);
	m_apNumber[5]->SetNumber(nNumber[2]);
	m_apNumber[6]->SetNumber(nNumber[1]);
	m_apNumber[7]->SetNumber(nNumber[0]);
}

//=============================================================================
// �X�R�A��������
//=============================================================================
void CScore::AddScore(int nValue)
{
	m_nScore += nValue;

	int nNumber[8];

	//�������v�Z
	nNumber[0] =m_nScore % 100000000 / 10000000;
	nNumber[1] =m_nScore % 10000000 / 1000000;
	nNumber[2] =m_nScore % 1000000 / 100000;
	nNumber[3] =m_nScore % 100000 / 10000;
	nNumber[4] =m_nScore % 10000 / 1000;
	nNumber[5] =m_nScore % 1000 / 100;
	nNumber[6] =m_nScore % 100 / 10;
	nNumber[7] =m_nScore % 10 / 1;

	//���̍��킹�Ă��炷
	m_apNumber[0]->SetNumber(nNumber[7]);
	m_apNumber[1]->SetNumber(nNumber[6]);
	m_apNumber[2]->SetNumber(nNumber[5]);
	m_apNumber[3]->SetNumber(nNumber[4]);
	m_apNumber[4]->SetNumber(nNumber[3]);
	m_apNumber[5]->SetNumber(nNumber[2]);
	m_apNumber[6]->SetNumber(nNumber[1]);
	m_apNumber[7]->SetNumber(nNumber[0]);
}

//=============================================================================
// Get�֐�
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}