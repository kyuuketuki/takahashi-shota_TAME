//=============================================================================
//
// �V�[��3D����[Visualization.cpp]
// Author; takahashi syota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "visualization.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "game.h"

//�O���[�o���ϐ��錾
D3DXVECTOR3 CVisualization::size[4];
D3DXCOLOR	CVisualization::m_col;
bool		CVisualization::m_bLife;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CVisualization::CVisualization()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CVisualization::~CVisualization()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CVisualization::Init(D3DXVECTOR3 pos, float fHight, float fWidth)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//�e�L�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/noname.png", &m_pTexture);

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
	pVtx[0].pos = D3DXVECTOR3(size[0].x, 100.0f, size[0].z);
	pVtx[1].pos = D3DXVECTOR3(size[1].x, 100.0f, size[1].z);
	pVtx[2].pos = D3DXVECTOR3(size[2].x, 100.0f, size[2].z);
	pVtx[3].pos = D3DXVECTOR3(size[3].x, 100.0f, size[3].z);

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

	m_nLife = 10;

	return S_OK;
}

//=============================================================================
//�|���S��3D�̏I������
//=============================================================================
void CVisualization::Uninit(void)
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
void CVisualization::Update(void)
{
	if (m_bLife == true)
	{
		m_nLife--;

		if (m_nLife <= 0)
		{
			Uninit();
		}
	}
}

//=============================================================================
//�|���S��3D�̕`�揈��
//=============================================================================
void CVisualization::Draw(void)
{
	//�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

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
	pDevice->SetTexture(0, NULL);

	//�����X�C�b�`
	if (CGame::GetSwitchScene3D() == false)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// ��������
//=============================================================================
CVisualization * CVisualization::Create(D3DXVECTOR3 pos, D3DXVECTOR3 fMaxSizeX, D3DXVECTOR3 fMinSizeX, D3DXVECTOR3 fMaxSizeY, D3DXVECTOR3 fMinSizeY, bool bLife, D3DXCOLOR col)
{
	//�V�[��3D����
	CVisualization *pVisualization = NULL;
	pVisualization = new CVisualization;

	//NULL�`�F�b�N
	if (pVisualization == NULL)
	{
		//�ÓI�����o�ϐ��ɑ��
		size[0] = fMaxSizeX;
		size[1] = fMinSizeX;
		size[2] = fMaxSizeY;
		size[3] = fMinSizeY;

		m_bLife = bLife;
		m_col = col;

		//�������Ăяo��
		pVisualization->Init(pos, 30.0f, 30.0f);
	}

	return pVisualization;
}
