//=============================================================================
//
// ���j���[���� [menu.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "menu.h"
#include "input.h"
#include "manager.h"
#include "scene2D.h"
#include "logo.h"
#include "animation.h"
#include "fade.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MENU_MODE_MAX (3)				// �I���ł��郂�[�h�̐�
#define SCREEN_TRANSITION_SPEED (30)	// ��ʑJ�ڂ̑��x
#define LOGO_CENTER (770)				// ��ʂ̒��S�iX���j

#define LOGO_GAMESTART_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, LOGO_CENTER - 170.0f, 0.0f))	// �Q�[���X�^�[�g�̃��S�̈ʒu
#define LOGO_TUTORIAL_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, LOGO_CENTER, 0.0f))			// �`���E�g���A���̂̃��S�̈ʒu
#define LOGO_END_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, LOGO_CENTER + 170.0f, 0.0f))		// �N�C�b�g�̃��S�̈ʒu	

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CMenu::CMenu()
{
	m_nFadeCnt = 0;
	m_nPointerMenuX = 0;
	m_bButtonDownMenu = false;
}

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CMenu::~CMenu()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
HRESULT CMenu::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �����w�i
	CAnimasion::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 3, 1);

	// �p�g�����v
	CAnimasion::Create(D3DXVECTOR3(810.0f, 50.0f, 0.0f), 100, 80, 5, 0);

	// �^�C�g�����S
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 300.0f, 0.0f), 800, 220, TITLE_LOGO);

	//�w�i�t���[��
	CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 770, 0.0f), 340, 260, MENU_FLAME);

	CLogo::Create(LOGO_GAMESTART_POS, 210, 40, TITLE_UI_GAMESTART);	// �Q�[���X�^�[�g

	CLogo::Create(LOGO_TUTORIAL_POS, 230, 40, TITLE_UI_TUTORIAL);	// �`���[�g���A��

	CLogo::Create(LOGO_END_POS, 230, 40, TITLE_UI_END);				// �I��

	CLogo::Create(LOGO_GAMESTART_POS, 280, 70, FRAME);				// �ړ��t���[��

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CMenu::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//==============================================================================
// �X�V����
//==============================================================================
void CMenu::Update(void)
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

	// ����������ĂȂ��ꍇ
	if (m_bButtonDownMenu == false)
	{
		//S�������������ꍇ
		if (pInputKeyboard->GetTrigger(DIK_S) == true || pGamepad->IsButtonDown(CInput::DOWN))
		{
			// �^�C�v��ς���
			m_nPointerMenuX++;

			// �^�C�v���ő�l����ɂȂ�����
			if (m_nPointerMenuX > MENU_MODE_MAX - 1)
			{
				// �^�C�v���ŏ��l�ɂ���
				m_nPointerMenuX = 0;
			}

			//�T�E���hSE
			pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
		}

		//W�������������ꍇ
		if (pInputKeyboard->GetTrigger(DIK_W) == true || pGamepad->IsButtonDown(CInput::UP))
		{
			// �^�C�v��ς���
			m_nPointerMenuX--;

			// �^�C�v���ŏ��l��艺�ɂȂ�����
			if (m_nPointerMenuX < 0)
			{
				// �ő�l�ɂ���
				m_nPointerMenuX = MENU_MODE_MAX - 1;
			}

			//�T�E���hSE
			pSound->Play(CSound::SOUND_LABEL_SE_MIGRATION);
		}

		// �t���[�����ړ�����ꏊ�ݒ� ���[�h�̐ݒ�
		if (m_nPointerMenuX == 0)
		{
			m_mode = CManager::MODE_GAME;
			CLogo::TypeMove(LOGO_GAMESTART_POS, FRAME);		//�ʒu��ς���
		}

		if (m_nPointerMenuX == 1)
		{
			m_mode = CManager::MODE_TUTORIAL;
			CLogo::TypeMove(LOGO_TUTORIAL_POS, FRAME);		//�ʒu��ς���
		}

		if (m_nPointerMenuX == 2)
		{
			m_mode = CManager::MODE_TITLE;
			CLogo::TypeMove(LOGO_END_POS, FRAME);			//�ʒu��ς���
		}

		//��ʑJ��	����{�^�����������ꍇ
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
		{
			//�A�Ŗh�~
			m_bButtonDownMenu = true;

			//�T�E���hSE
			pSound->Play(CSound::SOUND_LABEL_SE_DECISION);
		}
	}

	// ���肳�ꂽ�ꍇ
	else if (m_bButtonDownMenu == true)
	{
		// �t�F�[�h�J�E���g���Z
		m_nFadeCnt++;

		//�t�F�[�h�܂ł̎���
		if ((m_nFadeCnt % SCREEN_TRANSITION_SPEED) == 0)
		{
			//���[�h�ݒ�
			CFade::SetFade(CFade::FADE_OUT, (CManager::MODE)m_mode);		// �V�[���J��
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CMenu::Draw(void)
{

}

//==============================================================================
// ��������
//==============================================================================
CMenu * CMenu::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CMenu *pMenu;
	pMenu = new CMenu;

	// NULL�`�F�b�N
	if (pMenu != NULL)
	{
		pMenu->Init(pos, fSizeX, fSizeY);
	}

	return pMenu;
}