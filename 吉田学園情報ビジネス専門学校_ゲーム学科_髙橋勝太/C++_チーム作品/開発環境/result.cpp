//=============================================================================
//
// ���U���g���� [result.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "score.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "result.h"
#include "logo.h"
#include "fade.h"
#include "ranking.h"
#include "number.h"
#include "License.h"

//�ÓI�����o�ϐ��錾
CScore *CResult::m_pScore[5];

int nTimeGameResult = 0;			//����
int nTimeCounterResult = 0;			//���ԃJ�E���^�[
int g_nPointerResultX = 0;			//���݂̈ʒu���L��
bool g_bButtonDownResult = false;	//�A�Ŗh�~�p

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	//������
	g_nPointerResultX = 0;
	g_bButtonDownResult = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CResult::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//�w�i���S
	CLogo::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, RANKING_BG);

	//�����t���[��
	CLogo::Create(D3DXVECTOR3(1550.0f, 150.0f, 0.0f), 270, 100, RANKING_FREAM);

	//�����L���O���S
	CLogo::Create(D3DXVECTOR3(950.0f, 100.0f, 0.0f), 300, 60, RANKING_LOGO);

	//�v���C���[�X�R�A���S
	CLogo::Create(D3DXVECTOR3(350.0f, 200.0f, 0.0f), 250, 40, MAX_SCORE);

	// ���ʃ��S
	CLogo::Create(D3DXVECTOR3(500.0f, 500.0f, 0.0f), 90, 70, RANKING_FIRST);
	CLogo::Create(D3DXVECTOR3(500.0f, 620.0f, 0.0f), 90, 70, RANKING_SECOND);
	CLogo::Create(D3DXVECTOR3(500.0f, 740.0f, 0.0f), 90, 70, RANKING_THIRD);
	CLogo::Create(D3DXVECTOR3(500.0f, 860.0f, 0.0f), 70, 50, RANKING_FOUR);
	CLogo::Create(D3DXVECTOR3(500.0f, 980.0f, 0.0f), 70, 50, RANKING_FIVE);

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		//�����̃t���[��
		CLogo::Create(D3DXVECTOR3(1100.0f, 500.0f + (nCnt * 120.0f), 0.0f), 350, 70, RESULT_SCORE);
	}

	//�~���S
	CLogo::Create(D3DXVECTOR3(700.0f, 350.0f, 0.0f), 50, 50, RANKING_ADD);

	//=���S
	CLogo::Create(D3DXVECTOR3(1150.0f, 350.0f, 0.0f), 60, 50, RANKING_EQUAL);

	//�������S
	CLogo::Create(D3DXVECTOR3(1550.0f, 150.0f, 0.0f), 250, 100, RANKING_TUTORIAL);

	//�v���C���[�X�R�A
	CScore::Create(D3DXVECTOR3(600.0f, 350.0f, 0.0f), 30.0f, 40.0f);
	//�Q�[�����̃X�R�A���Ăяo��
	CScore::SetScore(CScore::GetScore());

	//�����L���O�f�[�^�̊Ǘ��@�ǂݍ��݁����ёւ�
	CRanking::Create(D3DXVECTOR3(1350.0f, 500.0f, 0.0f), 100.0f, 100.0f);

	//�Ƌ��V�X�e��
	CLicense::Create(D3DXVECTOR3(900.0f, 350.0f, 0.0f), 100, 70);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CResult::Update(void)
{
	//�L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	//�T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	//�A�Ŗh�~
	if (g_bButtonDownResult == false)
	{
		//��ʑJ��	����{�^�����������ꍇ
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
		{
			//�A�Ŗh�~
			g_bButtonDownResult = true;

			//���[�h�ݒ�
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_MENU);		// �V�[���J��
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CResult * CResult::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ���U���g��ʂ̐���
	CResult *pResult;
	pResult = new CResult;

	// NULL�`�F�b�N
	if (pResult != NULL)
	{
		pResult->Init(pos, fSizeX, fSizeY);
	}

	return pResult;
}
