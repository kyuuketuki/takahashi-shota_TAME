//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : Miura�@Yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "scene3D.h"

//=============================================================================
//�ÓI�����o�ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticle::CParticle(int nPriority, int nType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity)
{
	//�ʒu���̎󂯓n��
	m_pos = pos;

	//�֐��Ăяo��
	SetParticle(pos, fX, fY, fSpeed, col, nLife, fGravity);

	//����������
	CScene3D::Init(pos, m_SizeY, m_SizeX);

	//�e�N�X�`�����
	SetTex();

	//�v�Z����
	m_fAngle = (float)(rand() % 628 - 314) / D3DX_PI - 100.0f;//�����͈̔�
	m_move.x = cosf(m_fAngle / D3DX_PI) * m_fSpeed;//x���̒e�̑��x
	m_move.y = cosf(m_fAngle * D3DX_PI) * m_fSpeed;//y���̒e�̑��x
	m_move.z = sinf(m_fAngle) * m_fSpeed;          //z���̒e�̑��x

												   //���󂯓n��
	m_nLifeGravity = m_nLife;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CParticle::Update(void)
{
	//�ʒu�X�V
	m_pos += m_move;

	//�J���[�ύX
	SetCol(m_col);

	//�A���t�@�l�������Ă���
	m_col.a -= 0.01f;

	//���C�t�̔����܂ŗ�����d�͂�����
	if (m_nLife <= (m_nLifeGravity / 1.25f))
	{
		//�d�͔���
		m_move.y -= m_fGravity;
	}

	//���C�t����
	m_nLife--;
	if (m_nLife == 0)//������0�ɂȂ�����
	{
		//��������
		Uninit();
	}

	//�ʒu�X�V�̊֐��Ăяo��
	SetPosition(m_pos);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParticle::Draw(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�����_�[�X�e�[�g�̐ݒ�(���Z�����j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�`�揈��
	CScene3D::Draw();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�����_�[�X�e�[�g�̐ݒ�(���ɖ߂��j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ��������
//=============================================================================
CParticle * CParticle::Create(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity)
{
	//���f���𐶐�
	CParticle *pParticle = NULL;
	pParticle = new CParticle;

	//NULL�`�F�b�N
	if (pParticle != NULL)
	{
		//�I�u�W�F�N�g�̏���������
		pParticle->Init(pos, fX, fY, fSpeed, col, nLife, fGravity);

		//�e�N�X�`������
		pParticle->SetTexture(m_pTexture);
	}

	return pParticle;
}
//=============================================================================
// �e�N�X�`�����[�h����
//=============================================================================
void CParticle::Load(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/effect000.jpg", &m_pTexture);
}

//=============================================================================
// ����p�֐�����
//=============================================================================
void CParticle::SetParticle(D3DXVECTOR3 pos, float fX, float fY, float fSpeed, D3DXCOLOR col, int nLife, float fGravity)
{
	//�ʒu���
	m_pos = pos;

	//X�̃T�C�Y���
	m_SizeX = fX;

	//Y�̃T�C�Y���
	m_SizeY = fY;

	//�ړ��ʏ��
	m_fSpeed = fSpeed;

	//�J���[���
	m_col = col;

	//����
	m_nLife = nLife;

	//�d��
	m_fGravity = fGravity;
}
