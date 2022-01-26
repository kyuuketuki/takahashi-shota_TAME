//=============================================================================
//
// ���S���� [logo.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "logo.h"
#include "manager.h"
#include "scene2D.h"
#include "scene.h"
#include "player3D.h"

// �ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CLogo::m_apTextureLogo[MAX_UI] = {};

//�O���[�o���錾
int g_nTypeLogo = -1;			//���ꏈ��������^�C�v������
int g_nTypeMoveLogo = -1;		//���ꏈ��������^�C�v������
int g_nTypeCollarLogo = -1;		//���ꏈ��������^�C�v������
D3DXVECTOR3 g_posLogo;			//�ʒu
D3DXCOLOR	g_col;				//�F���

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLogo::CLogo(int nPriority, int nType) : CScene2D(nPriority, nType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLogo::~CLogo()
{
	g_nTypeLogo = -1;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// CScene2D�̏���������
	CScene2D::Init(pos, fSizeX, fSizeY);

	//�^�C�v����
	m_nType = nType;
	m_fSizeX = fSizeX;
	m_fSizeY = fSizeY;

	//���j���[��ʃt���[��
	if (m_nType == FRAME)
	{
		// �e�N�X�`�����W�ύX
		SetTex(1.0f, 0.5f, 1);
	}

	//�`���[�g���A����ʁ@�w�i
	if (m_nType == CONTROLS_BG)
	{
		// �e�N�X�`�����W�ύX
		SetTex(0.5f, 1.0f, 1);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLogo::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLogo::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//�Q�[���p�b�h�̃|�C���^�錾
	CGamepad *pGamepad;
	pGamepad = CManager::GetGamepad();

	// ��������̉摜�ύX
	if (m_nType == CONTROLS_BG)
	{
		// �G���^�[�������ꂽ��
		if (pInputKeyboard->GetTrigger(DIK_D) == true || pGamepad->IsButtonPush(CInput::RIGHT) == true)
		{
			// �e�N�X�`�����W�ύX
			SetTex(0.5f, 1.0f, 2);
		}

		// �G���^�[�������ꂽ��
		if (pInputKeyboard->GetTrigger(DIK_A) == true || pGamepad->IsButtonPush(CInput::LEFT) == true)
		{
			// �e�N�X�`�����W�ύX
			SetTex(0.5f, 1.0f, 1);
		}
	}

	//�X�^�[�g���̊g�又��
	if (m_nType == START)
	{
		//�����`�F�b�N
		if (m_nCountSize >= 35)
		{
			//�v���C���[�̈ړ�����
			CPlayer3D::SetMoveUse(true);

			//�X�^�[�gUI����
			Uninit();
		}

		//�g��I��
		else if (m_nCountSize >= 30)
		{
			//�������Ȃ�
		}

		else
		{
			//�g�又��
			//�T�C�Y�����
			CScene2D::SetSize(m_fSizeX + ((m_fSizeX / 10) * m_nCountSize), m_fSizeY + ((m_fSizeY / 10) * m_nCountSize));
		}

		//�����J�E���g
		m_nCountSize++;
	}

	// ���j���[��ʃt���[���X�V
	if (m_nType == FRAME)
	{
		// �G���^�[�������ꂽ��
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true || pGamepad->IsButtonDown(CInput::A) == true || pGamepad->IsButtonDown(CInput::B) == true)
		{
			// �e�N�X�`�����W�ύX
			SetTex(1.0f, 0.5f, 2);
		}
	}

	//�r���F�ύX
	if (m_nType == g_nTypeCollarLogo)
	{
		//�F��ς���
		CScene2D::SetCollar(g_col);
	}

	//�r���ړ�
	if (m_nType == g_nTypeMoveLogo)
	{
		//������
		g_nTypeMoveLogo = -1;

		//�ړ�������
		SetPosition(g_posLogo);

		//�X�V
		CScene2D::Update();
	}

	//�r���j��
	if (g_nTypeLogo == m_nType)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLogo::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CLogo * CLogo::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// ���[�J���ϐ��錾
	CLogo *pLogo = NULL;
	pLogo = new CLogo;

	// ������
	if (pLogo != NULL)
	{
		pLogo->Init(pos, fSizeX, fSizeY, nType);
	}

	// �e�N�X�`�����蓖��
	pLogo->BindTexture(m_apTextureLogo[nType]);

	return pLogo;
}

//=============================================================================
// �e�N�X�`���̓ǂݍ���
//=============================================================================
HRESULT CLogo::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/singouflame_short.png", &m_apTextureLogo[FRAME]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_flame.png", &m_apTextureLogo[GAMEFRAME]);

	// �^�C�g���A���j���[�@�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo000.png", &m_apTextureLogo[TITLE_LOGO]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &m_apTextureLogo[TITLE_UI_PRESS_ENTER]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gamestart.png", &m_apTextureLogo[TITLE_UI_GAMESTART]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial.png", &m_apTextureLogo[TITLE_UI_TUTORIAL]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/exit.png", &m_apTextureLogo[TITLE_UI_END]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/menu_flame.png", &m_apTextureLogo[MENU_FLAME]);

	// �`���[�g���A��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/controls_bg.png", &m_apTextureLogo[CONTROLS_BG]);

	// �|�[�Y �e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause.png", &m_apTextureLogo[PAUSE01]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/restart.png", &m_apTextureLogo[PAUSE_RESTART]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Menu.png", &m_apTextureLogo[PAUSE_MENU]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/exit.png", &m_apTextureLogo[PAUSE_EXIT]);

	//���X�g�w�i
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/navigate.png", &m_apTextureLogo[LIST]);

	//�Q�[��TUTORIAL
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/start.jpg", &m_apTextureLogo[START]);

	//�Q�[��TUTORIAL
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Gaming_telop_black.png", &m_apTextureLogo[RESULT_SCORE]);

	//�Q�[��TUTORIAL
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/Your Score.png", &m_apTextureLogo[MAX_SCORE]);

	//�Q�[�������L���OUI
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/1��.png", &m_apTextureLogo[RANKING_FIRST]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/2��.png", &m_apTextureLogo[RANKING_SECOND]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/3��.png", &m_apTextureLogo[RANKING_THIRD]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/4��.png", &m_apTextureLogo[RANKING_FOUR]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/5��.png", &m_apTextureLogo[RANKING_FIVE]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg.png", &m_apTextureLogo[RANKING_BG]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/�����L���O.png", &m_apTextureLogo[RANKING_LOGO]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/addscore.png", &m_apTextureLogo[RANKING_ADD]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/equal-red3.png", &m_apTextureLogo[RANKING_EQUAL]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/rankingtutorial.png", &m_apTextureLogo[RANKING_TUTORIAL]);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CAUTION_frame_yellow.png", &m_apTextureLogo[RANKING_FREAM]);

	//�Q�[�����ɕ\����������UI
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/����.jpg", &m_apTextureLogo[GAMR_UI01]);

	//���[������
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/���[������.jpg", &m_apTextureLogo[RULE]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CLogo::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		if (m_apTextureLogo != NULL)
		{
			m_apTextureLogo[nCnt]->Release();
			m_apTextureLogo[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �j���������^�C�v
//=============================================================================
void CLogo::TypeUninit(int nType)
{
	g_nTypeLogo = nType;
}

//=============================================================================
// �ʒu�����炵�����^�C�v
//=============================================================================
void CLogo::TypeMove(D3DXVECTOR3 pos, int nTypeMove)
{
	g_posLogo = pos;
	g_nTypeMoveLogo = nTypeMove;
}

//=============================================================================
// �w�肵���^�C�v�̐F��ς���
//=============================================================================
void CLogo::TypeChangeCollar(D3DXCOLOR col, int nTypeCollar)
{
	g_nTypeCollarLogo = nTypeCollar;
	g_col = col;
}

//=============================================================================
//
// ���C������ [logopause.cpp]
// Author : takahashi shota
//
//=============================================================================

//�|�[�Y��ʂɕ\�����������ꍇ

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLogoPause::CLogoPause(int nPriority, int nType) : CLogo(nPriority, nType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLogoPause::~CLogoPause()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLogoPause::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	CScene2D::Init(pos, fSizeX, fSizeY);

	CLogoPause::m_nType = nType;

	if (m_nType == FRAME)
	{
		// �e�N�X�`�����W�ύX
		SetTex(1.0f, 0.5f, 1);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLogoPause::Uninit(void)
{
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLogoPause::Update(void)
{
	// �L�[�{�[�h�擾
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	// ����X�V
	// �^�C�g���ړ�
	if (m_nType == 0)
	{
		// ���݂̈ʒu�̎擾
		D3DXVECTOR3 pos = GetPosition();

		// �ړ�������
		SetPosition(pos);

		// �X�V
		CScene2D::Update();
	}

	if (m_nType == 9)
	{
		// �F��ς���
		CScene2D::SetCollar(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	if (m_nType == g_nTypeCollarLogo)
	{
		// �F��ς���
		CScene2D::SetCollar(g_col);
	}

	// ����X�V
	if (m_nType == FRAME)
	{
		// �G���^�[�������ꂽ��
		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
		{// �e�N�X�`�����W�ύX
			SetTex(1.0f, 0.5f, 2);
		}
	}

	// �r���ړ�
	if (m_nType == g_nTypeMoveLogo)
	{
		// ������
		g_nTypeMoveLogo = -1;

		// �ړ�������
		SetPosition(g_posLogo);

		// �X�V
		CScene2D::Update();
	}

	// �r���I��
	if (g_nTypeLogo == m_nType)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLogoPause::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// ��������
//=============================================================================
CLogoPause * CLogoPause::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nType)
{
	// ���[�J���ϐ��錾
	CLogoPause *pLogoPause = NULL;
	pLogoPause = new CLogoPause;

	// ������
	if (pLogoPause != NULL)
	{
		pLogoPause->Init(pos, fSizeX, fSizeY, nType);
	}

	// �e�N�X�`�����蓖��
	pLogoPause->BindTexture(m_apTextureLogo[nType]);

	return pLogoPause;
}

//=============================================================================
// �e�N�X�`���̔j��
//=============================================================================
void CLogoPause::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_UI; nCnt++)
	{
		if (m_apTextureLogo != NULL)
		{
			m_apTextureLogo[nCnt]->Release();
			m_apTextureLogo[nCnt] = NULL;
		}
	}
}

//=============================================================================
// �j���������^�C�v
//=============================================================================
void CLogoPause::TypeUninit(int nType)
{
	g_nTypeLogo = nType;
}

//=============================================================================
// �ʒu�����炵�����^�C�v
//=============================================================================
void CLogoPause::TypeMove(D3DXVECTOR3 pos, int nTypeMove)
{
	g_posLogo = pos;
	g_nTypeMoveLogo = nTypeMove;
}

//=============================================================================
// �F��ς������^�C�v
//=============================================================================
void CLogoPause::TypeChangeCollar(D3DXCOLOR col, int nTypeCollar)
{
	g_nTypeCollarLogo = nTypeCollar;
	g_col = col;
}
