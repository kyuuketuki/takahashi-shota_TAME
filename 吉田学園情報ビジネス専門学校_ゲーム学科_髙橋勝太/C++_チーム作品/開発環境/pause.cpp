//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : taiki hayasaka
//
//=============================================================================
#include "pause.h"
#include "logo.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "animation.h"
#include "fade.h"

int nTimeGamePause = 0;				// ����
int nTimeCounterPause = 0;			// ���ԃJ�E���^�[
int g_nPointerPauseX = 0;			// �|�C���^�[�ʒu
bool g_bButtonDownPause = false;	// �A�Ŗh�~

//==============================================================================
// �}�N����`
//==============================================================================
#define MENU_PAUSE_MAX (3)				// �I���ł��郂�[�h�̐�
#define SCREEN_TRANSITION_SPEED (30)	// ��ʑJ�ڂ̑��x

#define LOGO_RESTART_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f))		// ���X�^�[�g�̃��S�̈ʒu
#define LOGO_MENU_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f))			// ���j���[�̂̃��S�̈ʒu
#define LOGO_EXIT_POS (D3DXVECTOR3(SCREEN_WIDTH / 2, 850.0f, 0.0f))			// �I���̃��S�̈ʒu	

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CPause::CPause(int nPriority, int nType) : CScene(nPriority, nType)
{
	// ������
	m_nFadeCntPause = 0;
	g_nPointerPauseX = 0;
	g_bButtonDownPause = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CPause::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �w�i
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, PAUSE01);

	// �t���[��
	CLogoPause::Create(D3DXVECTOR3(950.0f, 350.0f, 0.0f), 250, 60, FRAME);

	// ���S
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 350.0f, 0.0f), 170, 40, PAUSE_RESTART);	// ���X�^�[�g
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), 240, 40, PAUSE_MENU);		// ���j���[
	CLogoPause::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 850.0f, 0.0f), 240, 40, PAUSE_EXIT);		// �I��(���j���[�ɖ߂�)

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CPause::Uninit(void)
{
	// �I�u�W�F�N�g�j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CPause::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// �Q�[���p�b�h�̃|�C���^�錾
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	// �T�E���h���擾
	CSound *pSound;
	pSound = CManager::GetSound();

	// ����������ĂȂ��ꍇ
	if (g_bButtonDownPause == false)
	{
		// S �������������ꍇ
		if (pInputKeyboard->GetTrigger(DIK_S) == true || pGamepad->IsButtonDown(CInput::DOWN))
		{
			// �^�C�v��ς���
			g_nPointerPauseX++;

			// �^�C�v���ő�l����ɂȂ�����
			if (g_nPointerPauseX > MENU_PAUSE_MAX - 1)
			{
				// �^�C�v���ŏ��l�ɂ���
				g_nPointerPauseX = 0;
			}
		}

		// W�������������ꍇ
		if (pInputKeyboard->GetTrigger(DIK_W) == true || pGamepad->IsButtonDown(CInput::UP))
		{
			// �^�C�v��ς���
			g_nPointerPauseX--;

			// �^�C�v���ŏ��l��艺�ɂȂ�����
			if (g_nPointerPauseX < 0)
			{
				// �ő�l�ɂ���
				g_nPointerPauseX = MENU_PAUSE_MAX - 1;
			}
		}

		// �t���[�����ړ�����ꏊ�ݒ� ���[�h�̐ݒ�
		if (g_nPointerPauseX == 0)
		{								
			m_mode = CManager::MODE_GAME;					// ���X�^�[�g
			CLogo::TypeMove(LOGO_RESTART_POS, FRAME);		// �ʒu��ς���
		}
		if (g_nPointerPauseX == 1)
		{
			m_mode = CManager::MODE_MENU;					// ���j���[���
			CLogo::TypeMove(LOGO_MENU_POS, FRAME);		// �ʒu��ς���
		}
		if (g_nPointerPauseX == 2)
		{
			m_mode = CManager::MODE_TITLE;					// �^�C�g�����
			CLogo::TypeMove(LOGO_EXIT_POS, FRAME);			// �ʒu��ς���
		}

		// ��ʑJ��	����{�^�����������ꍇ
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) || pGamepad->IsButtonDown(CInput::B) == true)
		{
			// �A�Ŗh�~
			g_bButtonDownPause = true;
		}
	}

	// ���肳�ꂽ�ꍇ
	else if (g_bButtonDownPause == true)
	{
		// �t�F�[�h�J�E���g���Z
		m_nFadeCntPause++;

		// �t�F�[�h�܂ł̎���
		if ((m_nFadeCntPause % SCREEN_TRANSITION_SPEED) == 0)
		{
			// ���[�h�ݒ�
			CFade::SetFade(CFade::FADE_OUT, (CManager::MODE)m_mode);		// �V�[���J��
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
// ��������
//=============================================================================
CPause * CPause::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �|�C���^�ϐ��̐���
	CPause *pPause;
	pPause = new CPause;

	// NULL�`�F�b�N
	if (pPause != NULL)
	{
		pPause->Init(pos, fSizeX, fSizeY);
	}

	return pPause;
}