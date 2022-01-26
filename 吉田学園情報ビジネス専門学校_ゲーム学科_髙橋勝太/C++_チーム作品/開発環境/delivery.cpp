//=============================================================================
//
// �ו���͂��鏈�� [deliver.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "delivery.h"
#include "scene3D.h"
#include "player3D.h"
#include "passerby.h"
#include "game.h"
#include "manager.h"
#include "destination.h"

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9	CDelivery::m_pTexture;
int					CDelivery::m_nNumber;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CDelivery::CDelivery()
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CDelivery::~CDelivery()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CDelivery::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/box.png ", &m_pTexture);

	//����������
	CScene3D::Init(pos, fHight, fWidth);

	//�e�N�X�`�����
	SetTex();

	//�����o�ϐ��̑��
	m_pos = pos;
	m_pos.y = 300.0f;
	m_nLife = 300;

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CDelivery::Uninit(void)
{
	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}

	//�I������
	CScene3D::Uninit();
}

//==============================================================================
// �X�V����
//==============================================================================
void CDelivery::Update(void)
{
	//�O�����̎擾
	D3DXVECTOR3 pP = CPlayer3D::GetPosition();			//�v���C���[�̈ʒu���
	bool bUse = CPasserby::GetUse();					//�����蔻��

	if (m_pos.y >= 130.0f)
	{
		m_move.y -= 0.2f;

		// �ړ��ʂ̉��Z
		m_pos += m_move;

		//�ʒu�̍X�V
		CScene3D::SetPosition(m_pos);
	}

	//�������[���̏ꍇ
	if (m_nLife <= 0)
	{
		//����
		Uninit();
	}

	m_nLife--;					//���������炷
}

//==============================================================================
// �`�揈��
//==============================================================================
void CDelivery::Draw(void)
{
	//�`�揈��
	CScene3D::Draw();
}

//==============================================================================
// ��������
//==============================================================================
CDelivery * CDelivery::Create(D3DXVECTOR3 pos)
{
	//����
	CDelivery *pDelivry = NULL;
	pDelivry = new CDelivery;

	//�������Ăяo��
	pDelivry->Init(pos, 60.0f, 60.0f);

	//�e�N�X�`���̎󂯓n��
	pDelivry->SetTexture(m_pTexture);

	return pDelivry;
}