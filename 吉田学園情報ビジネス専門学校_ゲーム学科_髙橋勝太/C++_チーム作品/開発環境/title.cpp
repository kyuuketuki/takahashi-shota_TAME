//=============================================================================
//
// �^�C�g����ʏ��� [title.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "scene2D.h"
#include "logo.h"
#include "animation.h"
#include "gamepad.h"
#include "fade.h"
#include "player3D.h"
#include "scene3D.h"
#include "meshfield.h"
#include "model.h"
#include "animation.h"

//�O���[�o���ϐ��錾
int nTimeTitle = 0;					//����
int nTimeCounterTitle = 0;			//���ԃJ�E���^�[
int nNexEnterCount = 0;				//���SEnter�̐F�ω��Ɏg�p
int nSaveTime = -1;					//�A�j���[�V�����p���ԊǗ��p
int nSaveTime2 = -1;				//�A�j���[�V�����p���ԊǗ��p
int nFlashing = 30;					//PressEnter�̓_�ł����鑬�x�@�r���ő��x�ύX
bool g_bPressEnter = false;			//���S�̕\������
bool g_bNextEnter = false;			//���SEnter�̐F�ω��Ɏg�p
bool g_bButtonDownTitle = false;	//�A�Ŗh�~�p

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTitle::CTitle()
{
	//������
	nTimeTitle = 0;					//����
	nTimeCounterTitle = 0;			//���ԃJ�E���^�[
	nNexEnterCount = 0;				//���SEnter�̐F�ω��Ɏg�p
	nSaveTime = -1;					//�A�j���[�V�����p���ԊǗ��p
	nSaveTime2 = -1;				//�A�j���[�V�����p���ԊǗ��p
	nFlashing = 30;					//PressEnter�̓_�ł����鑬�x�@�r���ő��x�ύX
	g_bPressEnter = false;			//���S�̕\������
	g_bNextEnter = false;			//���SEnter�̐F�ω��Ɏg�p
	g_bButtonDownTitle = false;		//�A�Ŗh�~�p
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CTitle::~CTitle()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CTitle::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �����w�i
	CAnimasion::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 3, 1);

	// �p�g�����v
	CAnimasion::Create(D3DXVECTOR3(810.0f, 100.0f, 0.0f), 100, 80, 5, 0);

	// �^�C�g�����S
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 250.0f, 0.0f), 800, 220, TITLE_LOGO);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CTitle::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CTitle::Update(void)
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

	//�^�C�g�����S���\�����ꂽ���
	if (nTimeTitle >= 2 && nTimeCounterTitle >= 0)
	{
		//Enter���S�̕\��
		if (g_bPressEnter == false)
		{
			CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 200.0f, 0.0f), 300, 100, TITLE_UI_PRESS_ENTER);		//Enter���S
			g_bPressEnter = true;
		}

		//�_�ŏ����@NextEnter
		if (g_bPressEnter == true)
		{
			//�J�E���g�A�b�v
			nNexEnterCount++;
			if (nNexEnterCount == nFlashing)
			{
				//�����ɂ���
				if (g_bNextEnter == true)
				{
					CLogo::TypeChangeCollar(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), TITLE_UI_PRESS_ENTER);		//�F�ω�
					g_bNextEnter = false;
				}
				//�s�����ɂ���
				else if (g_bNextEnter == false)
				{
					CLogo::TypeChangeCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TITLE_UI_PRESS_ENTER);		//�F�ω�
					g_bNextEnter = true;
				}

				//���Z�b�g
				nNexEnterCount = 0;
			}
		}

		//���߂ĉ������ꍇ�̂�
		if (g_bButtonDownTitle == false)
		{
			//��ʑJ��
			if (pGamepad->IsButtonDown(CInput::A) == true || pGamepad->IsButtonDown(CInput::B) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
			{
				//��ʑJ�ڂɊւ���ϐ�
				nTimeCounterTitle = 0;

				//�_�łɊւ���ϐ�
				nFlashing = 5;
				nNexEnterCount = 0;

				//�{�^�����������^�C����ۑ�
				nSaveTime = nTimeTitle;
				nSaveTime2 = 30;

				//�A�Ŗh�~
				g_bButtonDownTitle = true;
			}
		}

		//�J�ڏ����A�j���[�V�������I����
		if ((nSaveTime + 1) == nTimeTitle && nSaveTime2 == nTimeCounterTitle)
		{
			//���[�h�ݒ�
			CFade::SetFade(CFade::FADE_OUT, CManager::MODE_MENU);	// �V�[���J��
		}
	}

	//�^�C����i�߂�
	nTimeCounterTitle++;

	//�J�E���^�[��60�ȏ�ɂȂ�����
	if (nTimeCounterTitle >= 60)
	{
		//�b����i�߂�
		nTimeTitle++;

		//�J�E���^�[��߂�
		nTimeCounterTitle = 0;
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CTitle::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CTitle * CTitle::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CTitle *pTitle;
	pTitle = new CTitle;

	// NULL�`�F�b�N
	if (pTitle != NULL)
	{
		pTitle->Init(pos, fSizeX, fSizeY);
	}

	return pTitle;
}
