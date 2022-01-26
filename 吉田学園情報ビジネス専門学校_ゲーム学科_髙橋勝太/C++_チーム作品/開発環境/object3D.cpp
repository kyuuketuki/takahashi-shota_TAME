//=============================================================================
//
// 3D�I�u�W�F�N�g���� [object3D.cpp]
// Author : takahashi shota
//
//=============================================================================
#include "object3D.h"
#include "manager.h"

D3DXVECTOR3		CObject3D::m_rotStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3		CObject3D::m_moveStatic = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int g_nTypeObject = -1;
int CObject3D::m_nType;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject3D::CObject3D(int nPriority, int nType)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject3D::~CObject3D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CObject3D::Init(D3DXVECTOR3 pos, float fSizeX, float fSizeY)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//X�t�@�C���̓ǂݍ���
	//�f��
	D3DXLoadMeshFromX("data/MODEL/planet.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);

	//�ʒu
	m_pos = pos;

	//����
	m_rot = m_rotStatic;
	m_move = m_moveStatic;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CObject3D::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CObject3D::Update(void)
{
	m_fRot += 0.01f;
	m_rot.y = m_fRot;
	m_pos += m_move;
}

//=============================================================================
// �`�揈��
//=============================================================================
void CObject3D::Draw(void)
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

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//���f���p�[�c�̕`��
		m_pMesh->DrawSubset(nCntMat);
	}

	//�ۑ������}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// ��������
//=============================================================================
CObject3D * CObject3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nType)
{
	//�I�u�W�F�N�g3D�𐶐�
	CObject3D *pObject3D = NULL;
	pObject3D = new CObject3D;

	//NULL�`�F�b�N
	if (pObject3D != NULL)
	{
		m_nType = nType;
		m_moveStatic = move;
		m_rotStatic = rot;
		pObject3D->Init(pos, 0.0f, 0.0f);
	}

	return pObject3D;
}

//=============================================================================
// Get�֐�
//=============================================================================
D3DXMATRIX CObject3D::GetMtxWorld(void)
{
	return m_mtxWorld;
}
