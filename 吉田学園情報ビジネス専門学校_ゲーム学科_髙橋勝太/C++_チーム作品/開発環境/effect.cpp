//=============================================================================
//
// �G�t�F�N�g3D����[effect3D.cpp]
// Author; miura yudai
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "effect.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "player3D.h"

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CEffect3D::CEffect3D()
{
	//�����o�ϐ�������
	m_alpha = 1.0f;
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CEffect3D::~CEffect3D()
{
}

//=============================================================================
//�G�t�F�N�g�̏���������
//=============================================================================
HRESULT CEffect3D::Init(D3DXVECTOR3 pos, float fWidth, float fHight)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/smoke.png", &m_pTexture);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	//�|���S��3D�̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y + fHight, m_pos.z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHight, m_pos.z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y - fHight, m_pos.z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y - fHight, m_pos.z);

	//�@���x�N�g���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//�ʒu���̑��
	m_pos = pos;
	m_nLife = 15 + rand() % 25;	//�����������_��

	return S_OK;
}

//=============================================================================
//�G�t�F�N�g�̏I������
//=============================================================================
void CEffect3D::Uninit(void)
{
	//�e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	//���_�o�b�t�@�̊J��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//�G�t�F�N�g�̍X�V����
//=============================================================================
void CEffect3D::Update(void)
{
	//�|���S��3D�̃|�C���^
	VERTEX_3D *pVtx;

	//�J���[���ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, m_alpha);
	m_alpha -= 0.05f;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_col;
	pVtx[1].col = m_col;
	pVtx[2].col = m_col;
	pVtx[3].col = m_col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//�v�Z����
	m_fAngle = (float)(rand() % 628 - 314) / 100.0f;	//�����͈̔�
	m_move.x = sinf(m_fAngle) * 2.5f;					//x���̒e�̑��x
	m_move.y = 3.5f;									//y���̒e�̑��x
	m_move.z = cosf(m_fAngle) * 2.5f;					//z���̒e�̑��x

	//�ړ��ʂ̍X�V
	m_pos += m_move;

	//�����`�F�b�N
	m_nLife--;

	if (m_nLife == 0)//������0�ɂȂ�����
	{
		Uninit();//�g�p���Ȃ�
	}
}

//=============================================================================
//�G�t�F�N�g�̕`�揈��
//=============================================================================
void CEffect3D::Draw(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�r���[�}�g���b�N�X�擾�p
	D3DXMATRIX mtxView;

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�����_�[�X�e�[�g�̐ݒ�(���Z�����j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����(�r���{�[�h)
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);	//�t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�����_�[�X�e�[�g�̐ݒ�(���ɖ߂��j
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//�G�t�F�N�g�̍쐬����
//=============================================================================
CEffect3D * CEffect3D::Create(D3DXVECTOR3 pos, float fWidth, float fHight)
{
	//�G�t�F�N�g3D����d
	CEffect3D *pEffect = NULL;
	pEffect = new CEffect3D;

	//NULL�`�F�b�N
	if (pEffect != NULL)
	{
		//�������Ăяo��
		pEffect->Init(pos, fWidth, fHight);
	}

	return pEffect;
}

//=============================================================================
//�G�t�F�N�g�̃e�N�X�`������
//=============================================================================
void CEffect3D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//��������̎󂯓n��
	m_pTexture = pTexture;
}
//=============================================================================
//�G�t�F�N�g�̈ʒu����
//=============================================================================
void CEffect3D::SetPosition(D3DXVECTOR3 pos)
{
	//��������̎󂯓n��
	m_pos = pos;
}
