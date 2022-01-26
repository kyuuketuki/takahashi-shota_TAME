//=============================================================================
//
// ���f������ [model.cpp]
// Author : takahashi shota
//
//=============================================================================

//=============================================================================
//�C���N���[�h�ǂݍ���
//=============================================================================
#include "model.h"
#include "manager.h"
#include "player3D.h"
#include "scene.h"

//�O���[�o���ϐ��錾
int g_nCntModel = 0;

//==============================================================================
// �R���X�g���N�^
//==============================================================================
CModel::CModel(int nPriority, int nType)
{

}

//==============================================================================
// �f�X�g���N�^
//==============================================================================
CModel::~CModel()
{

}

//==============================================================================
// ����������
//==============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//�w�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pTextName, D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	//�ʒu
	m_pos = pos;

	//����
	m_rot = rot;

	return S_OK;
}

//==============================================================================
// �I������
//==============================================================================
void CModel::Uninit(void)
{
	//���b�V���̊J��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�}�e���A���̊J��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//�e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}


//==============================================================================
// �X�V����
//==============================================================================
void CModel::Update(void)
{

}

//==============================================================================
// �`�揈��
//==============================================================================
void CModel::Draw(void)
{
	//���[�J��
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ�
	D3DXMATERIAL *pMat;				//�}�e���A���ւ̃|�C���^

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����̔��]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�̔��]
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���݂̃}�e���A���̎擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^�擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	// �e�̃}�g���b�N�X
	D3DXMATRIX mtxParent;

	//�e�����݂���ꍇ
	if (m_pParent != NULL)
	{
		//�e�̃}�g���b�N�X���擾
		mtxParent = m_pParent->GetMtxWorld();
	}

	//�e�����݂��Ȃ��ꍇ
	else
	{
		//���݁i�ŐV�j�̃}�g���b�N�X���擾����
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	// �Z�o�����e�p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, m_pTexture);

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ������}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//==============================================================================
// ����
//==============================================================================
CModel * CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pTextName)
{
	//���f���𐶐�
	CModel *pModel = NULL;
	pModel = new CModel;

	//NULL�`�F�b�N
	if (pModel != NULL)
	{
		pModel->Init(pos, rot, pTextName);
	}

	return pModel;
}

//=============================================================================
// Get�֐��@���[�V�����ōX�V�@�ʒu
//=============================================================================
void CModel::MotionPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
// Get�֐��@���[�V�����ōX�V�@����
//=============================================================================
void CModel::MotionRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//=============================================================================
// �e�N�X�`���ւ̃|�C���^
//=============================================================================
void CModel::SetTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//��������e�N�X�`���������Ă���
	m_pTexture = pTexture;
}

//==============================================================================
// �e�̐ݒ�
//==============================================================================
void CModel::SetParent(CModel * pModel)
{
	m_pParent = pModel;
}

//==============================================================================
// ���[���h�}�g���b�N�X�̎󂯓n��
//==============================================================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_mtxWorld;
}
