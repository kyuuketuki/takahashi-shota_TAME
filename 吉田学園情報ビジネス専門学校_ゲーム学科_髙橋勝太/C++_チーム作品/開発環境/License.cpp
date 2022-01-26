//=============================================================================
//
// �Ƌ����� [License.cpp]
// Author : koseki yuto
//
//=============================================================================

#include "manager.h"
#include "game.h"
#include "player3D.h"
#include "License.h"

// �ÓI�����o�ϐ�
LPDIRECT3DTEXTURE9 CLicense::m_pTexture = NULL;
CLicense::LISENCETYPE CLicense::m_nLicenseType = LISENCETYPE_GOLD;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CLicense::CLicense(int nPriority, int nType) : CScene2D(nPriority, nType)
{
	//m_nLicenseType = LISENCETYPE_GOLD;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CLicense::~CLicense()
{

}

//=============================================================================
// ��������
//=============================================================================
CLicense *CLicense::Create(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// ���[�J���ϐ��錾
	CLicense *pLicense = NULL;
	pLicense = new CLicense;

	// ������
	if (pLicense != NULL)
	{
		pLicense->Init(pos, fSizeX, fSizeY);
	}

	// �e�N�X�`�����蓖��
	pLicense->BindTexture(m_pTexture);

	return pLicense;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CLicense::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// CScene2D������
	CScene2D::Init(pos, fSizeX, fSizeY);

	//�����L���O�g�p����
	if (m_nLicenseType == LISENCETYPE_NORMAL)
	{
		CScene2D::SetTex(1.0f, 0.5f, 0);
	}
	if (m_nLicenseType == LISENCETYPE_GOLD)
	{
		CScene2D::SetTex(1.0f, 0.5f, 1);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CLicense::Uninit(void)
{
	// CScene2D�I������
	CScene2D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CLicense::Update(void)
{
	CPlayer3D *pPlayer3D = CGame::GetPlayer3D();

	if (pPlayer3D->GetMotionType() == MOTIONTYPE_FALLMOVE)
	{
		CScene2D::SetTex(1.0f, 0.5f, 0);

		m_nLicenseType = LISENCETYPE_NORMAL;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CLicense::Draw(void)
{
	// CScene2D�`��
	CScene2D::Draw();
}

//=============================================================================
// �e�N�X�`�����[�h
//=============================================================================
HRESULT CLicense::Load(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���	
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/License.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
//�e�N�X�`���A�����[�h
//=============================================================================
void CLicense::Unload(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ���C�Z���X�^�C�v
//=============================================================================
void CLicense::SetLisenceType(LISENCETYPE lisencetype)
{
	//��������^�C�v�������Ă���
	m_nLicenseType = lisencetype;
}

