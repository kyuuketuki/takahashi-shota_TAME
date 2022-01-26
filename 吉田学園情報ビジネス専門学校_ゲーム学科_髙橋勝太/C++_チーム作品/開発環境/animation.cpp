//=============================================================================
//
// �A�j���[�V�������� [animasion.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "animation.h"
#include "manager.h"
#include "logo.h"

// �ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 CAnimasion::m_pTexture[MAX_ANIME] = {};
int CAnimasion::m_nUninitType = NULL;

//�O���[�o���ϐ��錾
int g_nUninitType;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CAnimasion::CAnimasion(int nPriority, int nType) : CScene2D(nPriority, nType)
{
	//������
	m_nAnime = 0;
	m_nPatternAnime = 0;
	g_nUninitType = -1;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CAnimasion::~CAnimasion()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CAnimasion::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType)
{
	CScene2D::Init(pos, fSizeX, fSizeY);

	m_pos = pos;
	m_nType = nType;

	//�A�j���[�V�����̏�����
	CScene2D::SetTex(1, 0, 0);

	// �����o�ϐ��ɑ��
	m_nAnime = nAnime;

	// �摜�̕������ݒ�
	//�p�g�����v
	if (nType == 0)
	{
		m_fAnimeX = 0.2f;
		m_fAnimeY = 0.5f;
		m_nX = 5;
		m_nY = 2;
		m_fCriss = 1;
	}

	//�^�C�g���w�i
	if (nType == 1)
	{
		m_fAnimeX = 0.25f;
		m_fAnimeY = 0.33333333f;
		m_nX = 4;
		m_nY = 3;
		m_fCriss = 1;
	}

	//�Q�[���J�n�J�E���g�_�E��
	if (nType == 2)
	{
		m_fAnimeX = 0.16666666f;
		m_fAnimeY = 1.0f;
		m_nX = 6;
		m_nY = 1;
		m_fCriss = 0;
	}

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CAnimasion::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// �X�V����
//==============================================================================
void CAnimasion::Update(void)
{
	// �r���ύXUninit
	if (m_nType == g_nUninitType)
	{
		m_fCriss = 0;
		m_nAnime = 1;
	}

	// �A�j���[�V�����̍X�V����
	if ((m_nCounterAnime % m_nAnime) == 0)
	{
		// �A�j���[�V�����p�^�[���̍X�V
		m_nPatternAnime = (m_nPatternAnime + 1);

		// �A�j���[�V�����̍X�V
		CScene2D::SetAnime(m_fAnimeX, m_fAnimeY, m_nX, m_nY, m_nPatternAnime);
	}

	// �J�E���^�[��i�߂�
	m_nCounterAnime++;

	if (m_nType == 2)
	{
		//D3DX_PI/ m_nAnime;
		CScene2D::SetRevolution((D3DX_PI / m_nAnime) * m_nCounterAnime * 2);
	}

	// �X�V
	CScene2D::Update();

	// �A�j���[�V�������I�����Z�b�g
	if (m_nPatternAnime == m_nX * m_nY)
	{
		// �J�E���^�[�̃��Z�b�g
		m_nPatternAnime = 0;

		// ���[�v�����Ȃ��Ƃ�
		if (m_fCriss == 0)
		{
			// �I�������
			Uninit();

			if (m_nType == 2)
			{
				CLogo::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 160, 80, START);
			}
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CAnimasion::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//// �A���t�@�e�X�g
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, 0);

	// �`�揈��
	CScene2D::Draw();

	//// �A���t�@�e�X�g
	//pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//==============================================================================
// ��������
//==============================================================================
CAnimasion * CAnimasion::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType)
{
	//�@�A�j���[�V�����𐶐�
	CAnimasion *pAnimasion = NULL;
	pAnimasion = new CAnimasion;

	// NULL�`�F�b�N
	if (pAnimasion != NULL)
	{
		pAnimasion->Init(pos, fSizeX, fSizeY, nAnime, nType);
	}

	//�@�e�N�X�`��������
	pAnimasion->BindTexture(m_pTexture[nType]);

	return pAnimasion;
}

//==============================================================================
// �e�N�X�`���̓ǂݍ���
//==============================================================================
HRESULT CAnimasion::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ptranp.png", &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg_move.png", &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/countdown.jpg", &m_pTexture[2]);

	return S_OK;
}

//==============================================================================
// �e�N�X�`���̔j��
//==============================================================================
void CAnimasion::Unload(void)
{
	for (int nCnt = 0; nCnt < MAX_ANIME; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}

//==============================================================================
// �j������^�C�v
//==============================================================================
void CAnimasion::UninitType(int nUninitType)
{
	g_nUninitType = nUninitType;
}

//==============================================================================
// �p�������O���[�o����������
//==============================================================================
void CAnimasion::UninitTypeClear(void)
{
	g_nUninitType = 0;
}

//=============================================================================
//
// �A�j���[�V�����w�i���� [animasionbg.cpp]
// Author : takahashi shota
//
//=============================================================================

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CAnimasionBg::CAnimasionBg(int nPriority, int nType) : CAnimasion(nPriority, nType)
{
	m_nAnime = 0;
	m_nPatternAnime = 0;
}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CAnimasionBg::~CAnimasionBg()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CAnimasionBg::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType)
{
	CScene2D::Init(pos, fSizeX, fSizeY);

	// �����o�ϐ��ɑ��
	m_pos = pos;
	m_nType = nType;

	// �A�j���[�V�����̏�����
	CScene2D::SetTex(1, 0, 0);

	// �A�j���[�V�����X�s�[�h
	m_nAnime = nAnime;

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CAnimasionBg::Uninit(void)
{
	CScene2D::Uninit();
}

//==============================================================================
// �X�V����
//==============================================================================
void CAnimasionBg::Update(void)
{
	if (m_fColorFa <= 1.0f)
	{
		m_fColorFa += 0.005f;
		CScene2D::SetCollar(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fColorFa));
	}

	// �A�j���[�V�����̍X�V����
	if ((m_nCounterAnime % m_nAnime) == 0)
	{
		// �A�j���[�V�����p�^�[���̍X�V
		m_nPatternAnime = (m_nPatternAnime + 1);

		// �A�j���[�V�����̍X�V
		CScene2D::SetAnime(m_fAnimeX, m_fAnimeY, m_nX, m_nY, m_nPatternAnime);
	}

	// �J�E���^�[��i�߂�
	m_nCounterAnime++;

	// �X�V
	CScene2D::Update();

	// �A�j���[�V�������I�����Z�b�g
	if (m_nPatternAnime == m_nX * m_nY)
	{
		// �J�E���^�[�̃��Z�b�g
		m_nPatternAnime = 0;

		// ���[�v�����Ȃ��Ƃ�
		if (m_fCriss == 0)
		{
			// �I�������
			Uninit();
		}
	}
}

//==============================================================================
// �`�揈��
//==============================================================================
void CAnimasionBg::Draw(void)
{
	CScene2D::Draw();
}

//==============================================================================
// ��������
//==============================================================================
CAnimasionBg * CAnimasionBg::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY, int nAnime, int nType)
{
	// �A�j���[�V�����w�i�𐶐�
	CAnimasionBg *pAnimasionBg = NULL;
	pAnimasionBg = new CAnimasionBg;

	// NULL�`�F�b�N
	if (pAnimasionBg != NULL)
	{
		pAnimasionBg->Init(pos, fSizeX, fSizeY, nAnime, nType);
	}

	// �e�N�X�`��������
	pAnimasionBg->BindTexture(m_pTexture[nType]);

	return pAnimasionBg;
}

//==============================================================================
// �r���j������
//==============================================================================
void CAnimasionBg::UninitTypeClear(void)
{
	for (int nCnt = 0; nCnt < MAX_ANIME; nCnt++)
	{
		if (m_pTexture[nCnt] != NULL)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = NULL;
		}
	}
}
