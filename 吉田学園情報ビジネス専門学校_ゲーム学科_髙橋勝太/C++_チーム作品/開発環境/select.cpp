//=============================================================================
//
// �@�̑I����ʏ��� [select.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "bg.h"
#include "input.h"
#include "manager.h"
#include "scene2D.h"
#include "logo.h"
#include "animation.h"
#include "polygon.h"
#include "select.h"
#include "fade.h"

//�O���[�o��
int g_nPointerSelectY = 0;
int g_nOldPinterY = 0;
bool g_bUseKeyDown = false;
bool g_bButtonDownSelect = false;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CSelect::CSelect()
{
	//������
	g_nPointerSelectY = 0;
	g_nOldPinterY = 0;
	g_bUseKeyDown = false;
	g_bButtonDownSelect = false;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CSelect::~CSelect()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CSelect::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//�w�i��\��
	CBg::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	//�t���[���𐶐�
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 130, 130, FRAME);

	//�@�̃��S�𐶐�
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER04);			//3
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER05);			//4
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER01);						//0
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER02);			//1
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 100, 100, PLAYER03);			//2

	//�X�e�[�^�X�\��
	CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER01_STATUS);
	CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER01_STATUS2);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CSelect::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CSelect::Update(void)
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

	//�J�[�\���ړ�
	//D�������������ꍇ
	if (pInputKeyboard->GetTrigger(DIK_D) == true || pGamepad->IsButtonDown(CInput::RIGHT))
	{
		g_bUseKeyDown = true;

		if (g_nPointerSelectY == 0)
		{
			g_nPointerSelectY = 1;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 1)
		{
			g_nPointerSelectY = 2;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 2)
		{
			g_nPointerSelectY = 3;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 - 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 3)
		{
			g_nPointerSelectY = 4;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 4)
		{
			g_nPointerSelectY = 0;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		//�T�E���hSE
		pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
	}

	//�J�[�\���ړ�
	//A�������������ꍇ
	if (pInputKeyboard->GetTrigger(DIK_A) == true || pGamepad->IsButtonDown(CInput::LEFT))
	{
		g_bUseKeyDown = true;

		if (g_nPointerSelectY == 0)
		{
			g_nPointerSelectY = 4;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 - 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 1)
		{
			g_nPointerSelectY = 0;
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 2)
		{
			g_nPointerSelectY = 1;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 400.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 3)
		{
			g_nPointerSelectY = 2;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 + 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		else if (g_nPointerSelectY == 4)
		{
			g_nPointerSelectY = 3;

			//���S�̈ړ�
			CLogo::TypeMove(D3DXVECTOR3(SCREEN_WIDTH / 2 - 800.0f, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), FRAME);
		}

		//�T�E���hSE
		pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
	}

	//�X�e�[�^�X�ݒ�
	if (g_bUseKeyDown == true)
	{
		//�\������Ă���X�e�[�^�X������
		if (g_nOldPinterY == 0)
		{
			CLogo::TypeUninit(SELECT_PLAYER01_STATUS);
			CLogo::TypeUninit(SELECT_PLAYER01_STATUS2);
		}

		if (g_nOldPinterY == 1)
		{
			CLogo::TypeUninit(SELECT_PLAYER02_STATUS);
			CLogo::TypeUninit(SELECT_PLAYER02_STATUS2);
		}

		if (g_nOldPinterY == 2)
		{
			CLogo::TypeUninit(SELECT_PLAYER03_STATUS);
			CLogo::TypeUninit(SELECT_PLAYER04_STATUS);
		}

		if (g_nOldPinterY == 3)
		{
			CLogo::TypeUninit(SELECT_PLAYER03_STATUS2);
			CLogo::TypeUninit(SELECT_PLAYER04_STATUS2);
		}

		if (g_nOldPinterY == 4)
		{
			CLogo::TypeUninit(SELECT_PLAYER05_STATUS);
			CLogo::TypeUninit(SELECT_PLAYER05_STATUS2);
		}

		//�O�̃|�C���^�[���L��
		g_nOldPinterY = g_nPointerSelectY;
		
		//�X�e�[�^�X��\��
		if (g_nPointerSelectY == 0)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER01_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER01_STATUS2);
		}

		if (g_nPointerSelectY == 1)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER02_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER02_STATUS2);
		}

		if (g_nPointerSelectY == 2)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER03_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER03_STATUS2);
		}

		if (g_nPointerSelectY == 3)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER04_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER04_STATUS2);
		}

		if (g_nPointerSelectY == 4)
		{
			CLogo::Create(D3DXVECTOR3(300.0f, 500.0f, 0.0f), 200, 100, SELECT_PLAYER05_STATUS);
			CLogo::Create(D3DXVECTOR3(1200.0f, 700.0f, 0.0f), 600, 300, SELECT_PLAYER05_STATUS2);
		}

		g_bUseKeyDown = false;
	}

	//�v���C���[�̃^�C�v
	CManager::SetPlayerType(g_nPointerSelectY);

	//���߂ĉ������Ƃ�
	if (g_bButtonDownSelect == false)
	{
		//��ʑJ��
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
		{
			//���[�h�ݒ�
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_GAME, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	// �V�[���J��

			//�A�Ŗh�~
			g_bButtonDownSelect = true;

			//�T�E���hSE
			pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CSelect::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CSelect * CSelect::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �@�̑I����ʂ̐���
	CSelect *pSelect;
	pSelect = new CSelect;

	// NULL�`�F�b�N
	if (pSelect != NULL)
	{
		pSelect->Init(pos, fSizeX, fSizeY);
	}

	return pSelect;
}
