//=============================================================================
//
// �V�[��3D����[Scene3D.cpp]
// Author; takahashi syota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "scene3D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
//�R���X�g���N�^
//=============================================================================
CScene3D::CScene3D()
{

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
//�|���S��3D�̏���������
//=============================================================================
HRESULT CScene3D::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

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

	//�J���[���ݒ�
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

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

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//�ʒu���̑��
	m_pos = pos;

	return S_OK;
}

//=============================================================================
//�|���S��3D�̏I������
//=============================================================================
void CScene3D::Uninit(void)
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
//�|���S��3D�̍X�V����
//=============================================================================
void CScene3D::Update(void)
{

}

//=============================================================================
//�|���S��3D�̕`�揈��
//=============================================================================
void CScene3D::Draw(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�r���[�}�g���b�N�X�擾�p
	D3DXMATRIX mtxView;

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�r���[�}�g���b�N�X���擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);//�t�s������߂�
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
//�|���S��3D�̃N���G�C�g����
//=============================================================================
CScene3D * CScene3D::Create(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�V�[��3D����
	CScene3D *pScene3D = NULL;
	pScene3D = new CScene3D;

	//�������Ăяo��
	pScene3D->Init(pos, fHight, fWidth);

	return pScene3D;
}
//=============================================================================
//�|���S��3D�̃e�N�X�`������
//=============================================================================
void CScene3D::SetTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//�e�N�X�`��������������
	m_pTexture = pTexture;
}

//=============================================================================
//�|���S��3D�̈ʒu���̎󂯓n��
//=============================================================================
void CScene3D::SetPosition(D3DXVECTOR3 pos)
{
	//�ʒu��������������
	m_pos = pos;
}

//=============================================================================
//�|���S��3D�̊p�x���̎󂯓n��
//=============================================================================
void CScene3D::SetRotation(D3DXVECTOR3 rot)
{
	//�ʒu��������������
	m_Rot = rot;
}

//=============================================================================
//�|���S��3D�̃J���[���̎󂯓n��
//=============================================================================
void CScene3D::SetCol(D3DXCOLOR col)
{
	//�|���S��3D�̃|�C���^
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �A�j���[�V����
//=============================================================================
void CScene3D::SetAnim(float tex, int nCnt, float tx, float ty)
{
	//�\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(tex * nCnt, tx);
	pVtx[1].tex = D3DXVECTOR2(tex + (tex * nCnt), tx);
	pVtx[2].tex = D3DXVECTOR2(tex * nCnt, ty);
	pVtx[3].tex = D3DXVECTOR2(tex + (tex * nCnt), ty);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
//=============================================================================
// �e�N�X�`���ݒ�
//=============================================================================
void CScene3D::SetTex(void)
{
	//�\���̂̃|�C���^�ϐ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}
