//=============================================================================
//
// �`���[�g���A����ʏ��� [tutorial.cpp]
// Author : Taiki hayasaka
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "manager.h"
#include "scene2D.h"
#include "logo.h"
#include "animation.h"
#include "gamepad.h"
#include "fade.h"

//�O���[�o���ϐ��錾
int		g_nCountTutrial = 0;				//�{�^���̉�������
bool	g_bUITutorial = false;				//UI�o��
bool	g_bButtonDownTutorial = false;		//�A�Ŗh�~�p

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CTutorial::CTutorial()
{
	//������
	g_nCountTutrial = 0;
	g_bUITutorial = false;
	g_bButtonDownTutorial = false;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CTutorial::~CTutorial()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CTutorial::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	//���[������
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, RULE);

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CTutorial::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CTutorial::Update(void)
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
	if (g_bButtonDownTutorial == false)
	{
		//��ʑJ��
		if (pGamepad->IsButtonDown(CInput::A) == true || pGamepad->IsButtonDown(CInput::B) == true || pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{
			//�{�^������������
			g_nCountTutrial++;

			if (g_nCountTutrial == 1 && g_bUITutorial == false)
			{
				//�������
				CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, CONTROLS_BG);
				g_bUITutorial = true;
			}

			if (g_nCountTutrial == 2)
			{
				//�{�^����������b�N����
				g_bButtonDownTutorial = true;

				// ���j���[��ʂɑJ��
				CFade::SetFade(CFade::FADE_OUT, CManager::MODE_MENU);	// �V�[���J��
			}
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CTutorial::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CTutorial * CTutorial::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CTutorial *pTutorial;
	pTutorial = new CTutorial;

	// NULL�`�F�b�N
	if (pTutorial != NULL)
	{
		pTutorial->Init(pos, fSizeX, fSizeY);
	}

	return pTutorial;
}
